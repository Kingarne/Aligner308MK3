// $Id: TiltAlignmentPage2.cpp,v 1.12 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltAlignmentWizard.h"
#include "TiltAlignmentPage2.h"
#include "CreatePolynomial.h"
#include "Util.h"
#include "AlignerDoc.h"
#include "History.h"

#define COMMON_FLAT_TEST_TIMER		1

IMPLEMENT_DYNAMIC(CTiltAlignmentPage2, CPropertyPage)

CTiltAlignmentPage2::CTiltAlignmentPage2()
	: CPropertyPage(CTiltAlignmentPage2::IDD)
{
}

CTiltAlignmentPage2::~CTiltAlignmentPage2()
{
}

void CTiltAlignmentPage2::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
}

void CTiltAlignmentPage2::HideAllText()
{
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
}

void CTiltAlignmentPage2::HideAll()
{
	HideAllText();
	GetDlgItem(IDC_START_MEASURE)->ShowWindow(SW_HIDE);
}

void CTiltAlignmentPage2::StoreGraph()
{
	CString graphFileName;
	 
	if( m_pParent->m_pGraph->SaveGraphToUniqueFileName( graphFileName, FALSE ) == TRUE )
	{
	//	m_pParent->m_GraphFileManager.SaveFileName( graphFileName, FALSE );
	}
}

void CTiltAlignmentPage2::HideGraphButtons()
{
    
}

void CTiltAlignmentPage2::DisableAllButtons()
{
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );  
}

void CTiltAlignmentPage2::InitResultTable( void )
{
    if( m_pParent->m_CommonFlatTest == TRUE )
    {
        m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_CommonFlatTilt;
    }
    else
    {
	    m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_TiltAlignment;
    }
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
	m_pParent->m_pResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
	m_pParent->m_pResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
	m_pParent->m_pResultTable->m_InParam.pACA = NULL;
	m_pParent->m_pResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pParent->m_pResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pParent->m_pResultTable->m_InParam.SavedToDataBase = FALSE;
	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		m_pParent->m_pResultTable->m_InParam.pTemperature = m_Temperature;
	}
	else
	{
		m_pParent->m_pResultTable->m_InParam.pTemperature = NULL;
	}
	m_pParent->m_pResultTable->m_InParam.SamplingRate = 0;
    m_pParent->m_pResultTable->m_InParam.pFileGraphManager = &m_pParent->m_GraphFileManager;    
}

BEGIN_MESSAGE_MAP(CTiltAlignmentPage2, CPropertyPage)
    ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)    
    ON_WM_TIMER()
END_MESSAGE_MAP()

// CTiltAlignmentPage2 message handlers

BOOL CTiltAlignmentPage2::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_pParent = (CTiltAlignmentWizard*) GetParent();   
    m_MsgCaption = _T("");
    m_Text = _T("");
	m_CommonFlatTestTimerOn = FALSE;
	m_CommonFlatTestTimerEvent = 0;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltAlignmentPage2::OnSetActive() 
{
	if( m_pParent == NULL )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
        EndDialog( IDCANCEL );
        return( FALSE ); // prevent the page from being active
    }
	if( m_pParent->m_pResultTable == NULL )
	{
		m_pParent->m_pResultTable = new CResultTable( m_pParent );
	}
	InitResultTable();

	m_CommonFlatTestTimerOn = FALSE;
	m_CommonFlatTestTimerEvent = 0;
	m_pParent->m_EnableMoveOfWizard = TRUE;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    HideAll();
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

    m_NextBedPlaneFunction = NO_FUNCTION_DEFINED;
    MeasureRP();

    UpdateData( FALSE );
    m_pParent->m_Status = STATUS_PAGE_READY;
    g_AlignerData.ErrorDef = ERR_NO_ERROR;

    return CPropertyPage::OnSetActive();
}

BOOL CTiltAlignmentPage2::OnQueryCancel()
{
	StopTimer( FALSE );

	if( m_pParent->m_MeasurementInterrupted == TRUE )
	{
		return( TRUE );
	}
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_EXIT_WITHOUT_SAVE );

    if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) )
	{
		return( TRUE );
	}
	else
	{
		m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
		CString tmpStr;
		GetDlgItemText( IDC_INFO_TEXT, tmpStr );
		if( tmpStr == m_Text )
		{
			HideAllText();
			m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			m_pParent->m_Measure.m_InParam.Break = TRUE; 
		}
		else
		{
			m_Text.LoadString( IDS_WAITING_BEFORE_MEASUREMENT );
			CString tmpStr;
			GetDlgItemText( IDC_INFO_TEXT, tmpStr );
			if( tmpStr == m_Text )
			{
				StartTimer();
			}
		}
		return( FALSE );
	}
}

LRESULT CTiltAlignmentPage2::OnWizardBack() 
{
    return CPropertyPage::OnWizardBack();
}


void CTiltAlignmentPage2::AdjustIfAfterCommonFlatTest( void )
{
    if( m_pParent->m_CommonFlatTest == TRUE )
    {
        m_Text.LoadString( IDS_AUTOMATICALLY_ADJUST_CALIBRATION_DATA );

        if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
        {
            CreatePolynomial poly;
            CString serialNumber;        
            serialNumber = GetUnitTypeSerialNumber( g_AlignerData.RefObjNo );
            poly.UpdateRollOffsetCalibrationData( serialNumber, GetEac( g_AlignerData.RefObjNo ) );
            poly.UpdatePitchOffsetCalibrationData( serialNumber, GetEal( g_AlignerData.RefObjNo ) );

            for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
            {
                serialNumber = GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] );
                //poly.UpdateOffsetData( SensorRollOffsetCalibrationFromSN(), InsertIntoSensorRollOffsetCalibration(), DeleteSensorRollOffsetCalibration(), serialNumber, d );
                poly.UpdateRollOffsetCalibrationData( serialNumber, GetEac( g_AlignerData.ObjNo[i] ) );
                poly.UpdatePitchOffsetCalibrationData( serialNumber, GetEal( g_AlignerData.ObjNo[i] ) );
            }
			CommonFlatTilt::UpdateCalibrationFlag(TRUE);
			//DBInterface::Instance()->UpdateAdjustedCalibration("CommonFlatTiltHistory", serialNumber, SysSetup->GetOperatorName(), a_0);


        }
    }
}


BOOL CTiltAlignmentPage2::OnWizardFinish()
{
    switch( m_pParent->m_Status )
    {
        case STATUS_PAGE_READY:
		{			
			ShowPolarGraph();
		
			m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
			int keep = m_pParent->m_pResultTable->ShowReport();

			if (keep)
			{
				AdjustIfAfterCommonFlatTest();
			}

			return CPropertyPage::OnWizardFinish();	
		}
        break;

    case STATUS_PAGE_ERROR:
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        m_Text.LoadString( IDS_INTERNAL_MEASURE_ERROR_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
        break;
    case STATUS_PAGE_NOT_READY:
    default:
        m_MsgCaption.LoadString( IDS_NOT_READY_CAPTION );
        m_Text.LoadString( IDS_TASKS_NOT_COMPLETED_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        break;
  }

  

  return FALSE;  // prevent the page from changing
}

void CTiltAlignmentPage2::OnReset()
{	
    m_pParent->m_Status = STATUS_PAGE_CANCELED;
    g_AlignerData.ErrorDef = ERR_CANCEL;
 
	return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltAlignmentPage2::MeasureRP()
{
    g_AlignerData.NoOfBedPlanes = 0;
    InitMeasureParam();

    if( m_pParent->m_CommonFlatTest == TRUE )
    {
        g_AlignerData.NoOfBedPlanes = g_AlignerData.NoOfCorr + 1;
        m_Text.LoadString( IDS_COMMON_FLAT_TEST_PROCEDURE );
    }
    else
    {
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if( ( IsPlane( g_AlignerData.ObjNo[i] ) == TRUE ) || (IsGun( g_AlignerData.ObjNo[i] ) == TRUE) || (IsTheo(g_AlignerData.ObjNo[i]) == TRUE))
            {
                g_AlignerData.NoOfBedPlanes++;
            }
        }

        if( (IsPlane( g_AlignerData.RefObjNo ) == TRUE) || (IsGun( g_AlignerData.RefObjNo ) == TRUE) || (IsTheo(g_AlignerData.ObjNo[i]) == TRUE))
        {
            g_AlignerData.NoOfBedPlanes++;
        }

        m_Text.LoadString( IDS_TILT2_ALIGNMENT_PROCEDURE );
    }
    SetDlgItemText( IDC_TEXT_1, m_Text );
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

    switch( g_AlignerData.NoOfBedPlanes )
    {
    case 0: 
        BedPlane0Init();
        break;
    case 1: 
        BedPlane1Init();
        break;
    default:
        if( ( IsFixed( g_AlignerData.RefObjNo ) == TRUE ) && ( m_pParent->m_CommonFlatTest == FALSE ) )
        {
            BedPlane1Init();
        }
        else
        {
        BedPlane2Init();
        }
        break;
    }
}

void CTiltAlignmentPage2::BedPlane0Init()
{
    m_pParent->m_Measure.m_InParam.UseElev = FALSE;

    m_Text.LoadString( IDS_TILT2_ATTACH_SENSOR_FIX_PLANE );
    SetDlgItemText( IDC_TEXT_1, m_Text );
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

    EnableStartButton( IDS_START_FIRST_MEASURE );
    m_NextBedPlaneFunction = BED_PLANE_0_MEASURE_1;
}

void CTiltAlignmentPage2::BedPlane0Measure1()
{
    int i;

    for( i=0; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_Measure.m_InParam.FiObj[i] = 0;
    }
    m_pParent->m_Measure.m_InParam.FiRef = 0;

    if( CallMeasure( g_AlignerData.ACR, g_AlignerData.ACP ) == FALSE )
    {
        return;
    }
    m_NextBedPlaneFunction = FUNCTIONS_READY;
}

void CTiltAlignmentPage2::BedPlane1Init()
{
    int i;
      
    if( g_AlignerData.Along == TRUE )
    {
        m_FiBed = 0.0;
        m_BedPlaneDir.LoadString( IDS_PLUS_0_DEG );
    }
    else
    {
        m_FiBed = -M_PI/2;
        m_BedPlaneDir.LoadString( IDS_MINUS_90_DEG );
    }
    m_Text.LoadString( IDS_TILT2_ATTACH_SENSOR_FIX_PLANE );
    SetDlgItemText( IDC_TEXT_1, m_Text );
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

    AfxFormatString1( m_Text, IDS_TILT2_PUT_PLATFORMS_IN_AZIMUTH_X, m_BedPlaneDir );
    CString tmpStr;
    tmpStr.LoadString( IDS_TILT2_ELEVATION_SHALL_BE_FIXED_TO_0 );
    m_Text += _T("\n") + tmpStr;
    SetDlgItemText( IDC_TEXT_2, m_Text );
    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );

    g_AlignerData.ElevErrorExist = TRUE;
    m_pParent->m_Measure.m_InParam.UseElev = FALSE;

    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        if( ( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE ) && ( GetEal( g_AlignerData.ObjNo[i] ) == INVALID_VALUE ) )
        {
            g_AlignerData.ElevErrorExist = FALSE;
        }
    }

    if( ( IsFixed( g_AlignerData.RefObjNo ) == FALSE ) && ( GetEal( g_AlignerData.RefObjNo ) == INVALID_VALUE ) )
    {
        g_AlignerData.ElevErrorExist = FALSE;
    }

    if( g_AlignerData.ElevErrorExist == TRUE )
    {
        m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
        m_Text.LoadString( IDS_TILT2_ELEVATION_ERROR_ALREADY_MEASURED_USE_OLD );
        if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDYES )
        {
            m_pParent->m_Measure.m_InParam.UseElev = TRUE;
        }
    }

	if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
    {
        m_pParent->m_Measure.m_InParam.FiRef = 0;
    }
    else
    {
        m_pParent->m_Measure.m_InParam.FiRef = m_FiBed;
    }

    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        if( ( IsPlane( g_AlignerData.ObjNo[i] ) == TRUE ) || ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) || (IsTheo(g_AlignerData.ObjNo[i]) == TRUE))
        {
            m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;
        }
        else
        {
            m_pParent->m_Measure.m_InParam.FiObj[i] = 0;
        }
    }

    EnableStartButton( IDS_START_FIRST_MEASURE );
    m_NextBedPlaneFunction = BED_PLANE_1_MEASURE_1;
}

void CTiltAlignmentPage2::BedPlane1Measure1()
{
    int i;

    if( CallMeasure( m_dA0R, m_dA0P ) == FALSE )
    {
        return;
    }

    if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
    {
        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            g_AlignerData.ACR[i] = m_dA0R[i];
            g_AlignerData.ACP[i] = m_dA0P[i];
        }
        m_NextBedPlaneFunction = FUNCTIONS_READY;
    }
    else
    {
        if( g_AlignerData.Along == TRUE )
        {
            m_FiBed = M_PI;
            m_BedPlaneDir.LoadString( IDS_PLUS_180_DEG );
        }
        else
        {
            m_FiBed = M_PI/2;
            m_BedPlaneDir.LoadString( IDS_PLUS_90_DEG );
        }
        AfxFormatString1( m_Text, IDS_TILT2_PUT_PLATFORMS_IN_AZIMUTH_X, m_BedPlaneDir );
        SetDlgItemText( IDC_TEXT_1, m_Text );
        GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );
	    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
	    GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );

	    if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
        {
            m_pParent->m_Measure.m_InParam.FiRef = 0;
        }
        else
        {
            m_pParent->m_Measure.m_InParam.FiRef = m_FiBed;
        }

        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if( ( IsPlane( g_AlignerData.ObjNo[i] ) == TRUE) || ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) || (IsTheo(g_AlignerData.ObjNo[i]) == TRUE))
            {
                m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;
            }
            else
            {
                m_pParent->m_Measure.m_InParam.FiObj[i] = 0;
            }
        }

        m_pParent->m_EnableMoveOfWizard = TRUE;
        EnableStartButton( IDS_FIRST_MEASURE_READY );
        m_NextBedPlaneFunction = BED_PLANE_1_MEASURE_2;
    }
}

void CTiltAlignmentPage2::BedPlane1Measure2()
{
    int i;
    //keep the variable names from the old pascal code
    double dA180R[SIZE_OF_ARRAYS];
    double dA180P[SIZE_OF_ARRAYS];
    double ER[SIZE_OF_ARRAYS];
    double EP[SIZE_OF_ARRAYS];
    double tmpEac, EacSum;
    double tmpEal, EalSum;

    if( CallMeasure( dA180R, dA180P ) == FALSE )
    {
        return;
    }

    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        g_AlignerData.ACP[i] = ( m_dA0P[i] + dA180P[i] ) / 2;
        g_AlignerData.ACR[i] = ( m_dA0R[i] + dA180R[i] ) / 2;
        ER[i] = ( m_dA0R[i] - dA180R[i] ) / 2;
        EP[i] = ( m_dA0P[i] - dA180P[i] ) / 2;
    }

    if( g_AlignerData.Along == TRUE )
    {
        m_FiBed = 0.0;
    }
    else
    {
        m_FiBed = -M_PI/2;
    }

	if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
    {
		SetEac( g_AlignerData.RefObjNo, 0 );
        SetEal( g_AlignerData.RefObjNo, 0 );

        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if( ( IsPlane( g_AlignerData.ObjNo[i] ) == TRUE ) || ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) || (IsTheo(g_AlignerData.ObjNo[i]) == TRUE))
            {
                m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;
				E1( &tmpEac, &tmpEal, ER[i], EP[i], m_FiBed );
				SetEac( g_AlignerData.ObjNo[i], tmpEac );
				SetEal( g_AlignerData.ObjNo[i], tmpEal );
            }
            else
            {
				SetEac( g_AlignerData.ObjNo[i], 0 );
				SetEal( g_AlignerData.ObjNo[i], 0 );
            }
        }
    }
    else
    {
        m_pParent->m_Measure.m_InParam.FiRef = m_FiBed;
        EacSum = 0;
        EalSum = 0;

        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			E1( &tmpEac, &tmpEal, ER[i], EP[i], m_pParent->m_Measure.m_InParam.FiRef );
			SetEac( g_AlignerData.RefObjNo, tmpEac );
			SetEal( g_AlignerData.RefObjNo, tmpEal );
			EacSum -= GetEac( g_AlignerData.RefObjNo );
			EalSum -= GetEal( g_AlignerData.RefObjNo );
        }
		SetEac( g_AlignerData.RefObjNo, EacSum / g_AlignerData.NoOfCorr );
		SetEal( g_AlignerData.RefObjNo, EalSum / g_AlignerData.NoOfCorr );
    }
    m_NextBedPlaneFunction = FUNCTIONS_READY;
}

void CTiltAlignmentPage2::BedPlane2Init()
{
  int i;

  if( g_AlignerData.Along == TRUE )
  {
    m_FiBed = 0.0;
    m_pParent->m_Measure.m_InParam.FiRef = 0.0;
    m_BedPlaneDir.LoadString( IDS_PLUS_0_DEG );
    m_BedPlaneNegDir.LoadString( IDS_PLUS_180_DEG );
  }
  else
  {
    m_FiBed = -M_PI / 2;
    m_pParent->m_Measure.m_InParam.FiRef = -M_PI / 2;
    m_BedPlaneDir.LoadString( IDS_MINUS_90_DEG );
    m_BedPlaneNegDir.LoadString( IDS_PLUS_90_DEG );
  }

  if( m_pParent->m_CommonFlatTest == TRUE )
  {
    m_Text.LoadString( IDS_PUT_ALL_SENSORS_IN_AZIMUTH_0 );
		GetDlgItem( IDC_IMAGE_1 )->ShowWindow( SW_SHOW );
  }
  else
  {
    AfxFormatString1( m_Text, IDS_TILT2_PUT_PLATFORMS_IN_AZIMUTH_X, m_BedPlaneDir );
	  CString tmpStr;
    tmpStr.LoadString( IDS_TILT2_ELEVATION_SHALL_BE_FIXED_TO_0 );
	  m_Text += _T("\n") + tmpStr;
    SetDlgItemText( IDC_TEXT_3, m_Text );
    GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_SHOW );
    m_Text.LoadString( IDS_TILT2_ATTACH_SENSOR_FIX_PLANE );
  }
  SetDlgItemText( IDC_TEXT_2, m_Text );
  GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );

  g_AlignerData.ElevErrorExist = TRUE;
  m_pParent->m_Measure.m_InParam.UseElev = FALSE;

  for(  i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    m_pParent->m_Measure.m_InParam.FiObj[i] = 0;

		if( ( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE ) || ( m_pParent->m_CommonFlatTest == TRUE ) )
    {
      m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;

			if( GetEal( g_AlignerData.ObjNo[i] ) == INVALID_VALUE )
      {
        g_AlignerData.ElevErrorExist = FALSE;
      }
    }
  }

  if( m_pParent->m_CommonFlatTest == FALSE )
  {
	  if( GetEal( g_AlignerData.RefObjNo ) == INVALID_VALUE )
    {
      g_AlignerData.ElevErrorExist = FALSE;
    }

    if( g_AlignerData.ElevErrorExist == TRUE )
    {
      m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
      m_Text.LoadString( IDS_TILT2_ELEVATION_ERROR_ALREADY_MEASURED_USE_OLD );
      if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDYES )
      {
        m_pParent->m_Measure.m_InParam.UseElev = TRUE;
      }
    }

	  if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
	  {
		  m_Text.LoadString( IDS_ONLY_ONE_MEASUREMENT );
		  SetDlgItemText( IDC_TEXT_1, m_Text );
		  GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );
	  }
	  else
	  {
		  m_Text.LoadString( IDS_TILT2_COMPLETE_EVALUATION );
		  SetDlgItemText( IDC_TEXT_1, m_Text );
		  GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );
	  }
  }

  EnableStartButton( IDS_START_FIRST_MEASURE );
  m_NextBedPlaneFunction = BED_PLANE_2_MEASURE_1;
}

void CTiltAlignmentPage2::BedPlane2Measure1()
{
  int i;

  if( CallMeasure( m_Arl, m_Apl ) == FALSE )
  {
    return;
  }

	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		memcpy( m_Temperature, m_pParent->m_Measure.m_OutParam.TempT, SIZE_OF_ARRAYS * sizeof( double ) );
	}

  if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
  {
    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
      g_AlignerData.ACR[i] = m_Arl[i];
      g_AlignerData.ACP[i] = m_Apl[i];
    }
    m_NextBedPlaneFunction = FUNCTIONS_READY;
  }
  else
  {
    m_pParent->m_Measure.m_InParam.FiRef += M_PI;
		CString tmpStr;

		if( m_pParent->m_CommonFlatTest == TRUE )
		{
			AfxFormatString1( tmpStr, IDS_SENSOR_SN_XXX, GetSensorSerialNumber( g_AlignerData.RefObjNo ) );
			AfxFormatString2( m_Text, IDS_TILT2_PUT_DEVICE_IN_AZIMUTH_X, tmpStr, m_BedPlaneNegDir );
		}
		else
		{
			AfxFormatString2( m_Text, IDS_TILT2_PUT_DEVICE_IN_AZIMUTH_X, GetUnitTypeDescription( g_AlignerData.RefObjNo ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ), m_BedPlaneNegDir );
		}
    
    if( m_pParent->m_CommonFlatTest == TRUE )
    {
      AfxFormatString1( tmpStr, IDS_TILT2_LET_SENSORS_REMAIN_IN_X, m_BedPlaneDir );
      GetDlgItem( IDC_IMAGE )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_IMAGE_1 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_IMAGE_2 )->ShowWindow( SW_SHOW );
    }
    else
    {
      AfxFormatString1( tmpStr, IDS_TILT2_LET_PLATFORMS_REMAIN_IN_X, m_BedPlaneDir );
    }
    m_Text += _T("\n") + tmpStr;

    SetDlgItemText( IDC_TEXT_1, m_Text );
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );

    m_pParent->m_EnableMoveOfWizard = TRUE;
    EnableStartButton( IDS_FIRST_MEASURE_READY );
    m_NextBedPlaneFunction = BED_PLANE_2_MEASURE_2;
  }
}

void CTiltAlignmentPage2::BedPlane2Measure2()
{
    int i;

    if( CallMeasure( m_Drl, m_Dpl ) == FALSE )
    {
        return;
    }

	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		for( int i=0; i<SIZE_OF_ARRAYS; i++ )
		{
			m_Temperature[i] += m_pParent->m_Measure.m_OutParam.TempT[i];
		}
	}

	m_FiBed += M_PI;

    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
		if( ( IsFixed( g_AlignerData.ObjNo[i] ) == TRUE ) && ( m_pParent->m_CommonFlatTest == FALSE ) )
        {
            g_AlignerData.ACR[i] = ( m_Arl[i] + m_Drl[i] ) / 2;
            g_AlignerData.ACP[i] = ( m_Apl[i] + m_Dpl[i] ) / 2;
        }
        else
        {
            m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;
        }
    }

    if( m_pParent->m_CommonFlatTest == TRUE )
    {
        AfxFormatString1( m_Text, IDS_TILT2_PUT_SENSORS_IN_AZIMUTH_X, m_BedPlaneNegDir );
        GetDlgItem( IDC_IMAGE )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_IMAGE_2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_IMAGE_3 )->ShowWindow( SW_SHOW );
    }
    else
    {
        AfxFormatString1( m_Text, IDS_TILT2_PUT_PLATFORMS_IN_AZIMUTH_X, m_BedPlaneNegDir );	  
    }
    CString tmpStr;

	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		CString sensorSN;
		AfxFormatString1( sensorSN, IDS_SENSOR_SN_XXX, GetSensorSerialNumber( g_AlignerData.RefObjNo ) );
		AfxFormatString2( tmpStr, IDS_TILT2_LET_DEVICE_Y_REMAIN_IN_X, sensorSN, m_BedPlaneNegDir );
	}
	else
	{
		AfxFormatString2( tmpStr, IDS_TILT2_LET_DEVICE_Y_REMAIN_IN_X, GetUnitTypeDescription( g_AlignerData.RefObjNo ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ), m_BedPlaneNegDir );
	}
    m_Text += _T("\n") + tmpStr;
    SetDlgItemText( IDC_TEXT_1, m_Text );
    GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
	    GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );
	    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );

    m_pParent->m_EnableMoveOfWizard = TRUE;
    EnableStartButton( IDS_SECOND_MEASURE_READY );
    m_NextBedPlaneFunction = BED_PLANE_2_MEASURE_3;
}

void CTiltAlignmentPage2::BedPlane2Measure3()
{
    int i;
    double Brl[SIZE_OF_ARRAYS];
    double Bpl[SIZE_OF_ARRAYS];
    double ER[SIZE_OF_ARRAYS];
    double EP[SIZE_OF_ARRAYS];
	    double tmpEal, tmpEac;

    if( CallMeasure( Brl, Bpl ) == FALSE )
    {
        return;
    }

	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		for( int i=0; i<SIZE_OF_ARRAYS; i++ )
		{
			m_Temperature[i] = ( m_Temperature[i] + m_pParent->m_Measure.m_OutParam.TempT[i] ) / 3;
		}
	}

	m_FiBed -= M_PI;
    m_pParent->m_Measure.m_InParam.FiRef -= M_PI;

    for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
		if( ( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE ) || ( m_pParent->m_CommonFlatTest == TRUE ) )
        {
            g_AlignerData.ACR[i] = ( m_Arl[i] + Brl[i] ) / 2;
            g_AlignerData.ACP[i] = ( m_Apl[i] + Bpl[i] ) / 2;
            ER[g_AlignerData.ObjNo[i]] = ( m_Drl[i] - Brl[i] ) / 2;
            EP[g_AlignerData.ObjNo[i]] = ( m_Dpl[i] - Bpl[i] ) / 2;
            m_pParent->m_Measure.m_InParam.FiObj[i] = m_FiBed;
			E1( &tmpEac, &tmpEal, ER[g_AlignerData.ObjNo[i]], EP[g_AlignerData.ObjNo[i]], m_FiBed);
			SetEac( g_AlignerData.ObjNo[i], tmpEac );
			SetEal( g_AlignerData.ObjNo[i], tmpEal );
        }
    }
    ER[g_AlignerData.RefObjNo] = ( m_Drl[1] - m_Arl[1] ) / 2;
    EP[g_AlignerData.RefObjNo] = ( m_Dpl[1] - m_Apl[1] ) / 2;
	E1( &tmpEac, &tmpEal, ER[g_AlignerData.RefObjNo], EP[g_AlignerData.RefObjNo], m_pParent->m_Measure.m_InParam.FiRef );
	SetEac( g_AlignerData.RefObjNo, tmpEac );
	SetEal( g_AlignerData.RefObjNo, tmpEal );

    m_NextBedPlaneFunction = FUNCTIONS_READY;
}

void CTiltAlignmentPage2::OnBnClickedStartMeasure()
{
    HideAllText();
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    m_Text.LoadString( IDS_BREAK );
    CString tmpStr;
    GetDlgItemText( IDC_START_MEASURE, tmpStr );

    if( tmpStr == m_Text )
    {
		if( m_CommonFlatTestTimerOn == TRUE )
		{
			StopTimer( TRUE );
		}
		else
		{
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			m_pParent->m_Measure.m_InParam.Break = TRUE; 
		}
		return;
    }

    if( m_pParent->m_CommonFlatTest == FALSE )
    {
        MoveDlgToRightSideOfApp(m_pParent );
        //MoveDlgToRightSideOfTheGraph( m_pParent );
    }
    HideGraphButtons();
    SetDlgItemText( IDC_START_MEASURE, m_Text );
    m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = FALSE;

    switch( m_NextBedPlaneFunction )
    {
        case BED_PLANE_0_MEASURE_1:
            BedPlane0Measure1();
            break;
        case BED_PLANE_1_MEASURE_1:
            BedPlane1Measure1();
            break;
        case BED_PLANE_1_MEASURE_2:
            BedPlane1Measure2();
            break;
        case BED_PLANE_2_MEASURE_1:
		    if( StartTimer() == TRUE )
		    {
			    return;
		    }
            BedPlane2Measure1();
            break;
        case BED_PLANE_2_MEASURE_2:
		    if( StartTimer() == TRUE )
		    {
			    return;
		    }
            BedPlane2Measure2();
            break;
    case BED_PLANE_2_MEASURE_3:
		    if( StartTimer() == TRUE )
		    {
			    return;
		    }
            BedPlane2Measure3();
            break;
    default:
            //TODO error message
            return;
            break;
    }

	    CheckMeasureResult();
}

void CTiltAlignmentPage2::CheckMeasureResult()
{
    switch( m_NextBedPlaneFunction )
    {
    default:
    case NO_FUNCTION_DEFINED:
        //TODO error message
        break;
    case BED_PLANE_0_MEASURE_1:
    case BED_PLANE_1_MEASURE_1:
    case BED_PLANE_2_MEASURE_1:
		   StoreGraph();
        break;
    case BED_PLANE_1_MEASURE_2:
    case BED_PLANE_2_MEASURE_2:
    case BED_PLANE_2_MEASURE_3:
		StoreGraph();
        break;
    case FUNCTIONS_READY:
		{
			RestoreGlobals();
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

			HideAllText();
			m_Text.LoadString( IDS_MEASUREMENT_READY );
			SetDlgItemText( IDC_INFO_TEXT, m_Text );
			GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
			m_pParent->m_EnableMoveOfWizard = TRUE;

			for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
			{
				CartToVec( g_AlignerData.ACR[i], g_AlignerData.ACP[i], &(g_AlignerData.VecAmp[i]), &(g_AlignerData.VecArg[i]) );
			}

			GetDlgItem( IDC_IMAGE_1 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_IMAGE_2 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_IMAGE_3 )->ShowWindow( SW_HIDE );
			StoreGraph();
			m_pParent->SetWizardButtons( PSWIZB_FINISH );
		}
    break;
    case FUNCTION_INTERRUPTED:
        RestoreGlobals();
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

        HideAllText();
        m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
        SetDlgItemText( IDC_INFO_TEXT, m_Text );
        GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
        m_pParent->m_EnableMoveOfWizard = TRUE;
        break;
    }
}

void CTiltAlignmentPage2::InitMeasureParam()
{
    //set inparameters for measure
    //this params already initiated:
    //m_pParent->m_Measure.m_InParam.pSource = NULL;
    //m_pParent->m_Measure.m_InParam.SourceStatus = ACQU_IDLE;
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			

    m_pParent->m_Measure.m_InParam.Break = FALSE;
    memcpy( m_pParent->m_Measure.m_InParam.ObjNo, g_AlignerData.ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
    m_pParent->m_Measure.m_InParam.RefObjNo = g_AlignerData.RefObjNo;
    m_pParent->m_Measure.m_InParam.Mode1 = Diff;
    m_pParent->m_Measure.m_InParam.Tao = g_AlignerData.TaoTilt;
    m_pParent->m_Measure.m_InParam.FiRef = 0;
    memset( m_pParent->m_Measure.m_InParam.FiObj, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( m_pParent->m_Measure.m_InParam.ZParCorr, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( m_pParent->m_Measure.m_InParam.rotateBack, 0, SIZE_OF_ARRAYS * sizeof( BOOL ) );
    m_pParent->m_Measure.m_InParam.rotateBackRef = 0.0f;
    m_pParent->m_Measure.m_InParam.CountObj = g_AlignerData.NoOfCorr;
    m_pParent->m_Measure.m_InParam.RangeX = g_AlignerData.RX;
    m_pParent->m_Measure.m_InParam.RangeY = g_AlignerData.RY;
    m_pParent->m_Measure.m_InParam.UseElev = FALSE;     
	m_pParent->m_Measure.m_InParam.Fs = m_pParent->m_pSource->GetFrequency();
	m_pParent->m_Measure.m_InParam.SperryActive = m_pParent->GetSperryActive();
    m_pParent->m_Measure.m_InParam.commonFlatTest = m_pParent->m_CommonFlatTest;    
    
//  m_pParent -> m_pSource -> SetFrequency( m_pParent -> m_Measure.m_InParam.Fs ) ;
// 	m_pParent -> m_pSource -> Start() ;
}

void CTiltAlignmentPage2::RestoreGlobals()
{
  g_AlignerData.UseElevError = m_pParent->m_Measure.m_InParam.UseElev;
}

void CTiltAlignmentPage2::EnableStartButton( int textID )
{
    if( textID != IDS_START_FIRST_MEASURE )
    {
        m_Text.LoadString( textID );
        SetDlgItemText( IDC_INFO_TEXT, m_Text );
        GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    }

    m_Text.LoadString( IDS_START_MEASURE );
    SetDlgItemText( IDC_START_MEASURE, m_Text );
    GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
}

// void CTiltAlignmentPage2::SetupCurrentAz()
// {
//     if(!IsFixed(g_AlignerData.RefObjNo))
//     {
//         GetSensor()
//     }
// }

BOOL CTiltAlignmentPage2::CallMeasure( double *pRoll, double *pPitch )
{
	//if( m_pParent->m_CommonFlatTest == TRUE )
	//{
	//	m_Delay.StartDelay( 30 );
	//}
    SetupSensors(m_pParent->m_Measure.m_InParam);

    do
    {
        m_pParent->m_Measure.Measure();

		if( g_AlignerData.ErrorDef == ERR_CANCEL )
		{
			return( FALSE );
		}
      
        if( ( g_AlignerData.ErrorFlag == TRUE ) || ( g_AlignerData.Reject == TRUE ) )
        {
            m_pParent->m_Measure.m_InParam.Break = FALSE;
            m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
            m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED_REPEAT_IT );

            if( DAU::GetDAU().IsOpen() == FALSE || MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDNO )
            {
			    m_pParent->m_MeasurementInterrupted = TRUE;
                m_NextBedPlaneFunction = FUNCTION_INTERRUPTED;
                return FALSE;
            }
            else
            {
                if( m_pParent->m_pSource != NULL )
                {
                    m_pParent->m_pSource->Start();
                }
                else
                {
                    DAU::GetDAU().Start();
                }
                m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
                SetDlgItemText( IDC_INFO_TEXT, m_Text );
                GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
                GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
            }
        }
    }while( ( g_AlignerData.ErrorFlag == TRUE ) || ( g_AlignerData.Reject == TRUE ) );

    //Now the measurement is ready, get the timestamp to be stored in the graph and table
    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_pParent->m_MeasurementReadyTimeStamp );

    memcpy( pRoll, m_pParent->m_Measure.m_OutParam.DeltaRoll, SIZE_OF_ARRAYS * sizeof( double ) );
    memcpy( pPitch, m_pParent->m_Measure.m_OutParam.DeltaPitch, SIZE_OF_ARRAYS * sizeof( double ) );
	CopyTemperature( m_pParent->m_Measure.m_OutParam.TempT );

    return TRUE;
}

void CTiltAlignmentPage2::ShowPolarGraph()
{
	//init the graph
    m_pParent->m_pGraph->InitDefaultPolarGraph( g_AlignerData.NoOfCorr );

    //set data and labels
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    { 
		    double newVecArg;
		    newVecArg = AdjustDegAngle( g_AlignerData.VecArg[i], m_pParent->m_pGraph->GetPolarRange0ToPlusMinus180(), 1 );
		    m_pParent->m_pGraph->SetXYData( i, newVecArg, g_AlignerData.VecAmp[i] );

		    if( m_pParent->m_CommonFlatTest == TRUE )
		    {
			    CString sensorSN;
			    AfxFormatString1( sensorSN, IDS_SN_XXX, GetSensorSerialNumber( g_AlignerData.ObjNo[i] ) );
			    m_pParent->m_pGraph->SetSerieLabel( i, sensorSN );
		    }
		    else
		    {
			    m_pParent->m_pGraph->SetSerieLabel( i, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
		    }
    }
    m_pParent->m_pGraph->UpdateData();
    PolarGraphInParam graphParams;
    graphParams.mode = m_pParent->m_Measure.m_InParam.Mode1;
    graphParams.tao = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.RefNo = m_pParent->m_Measure.m_InParam.RefObjNo;
	graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
	graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
    graphParams.commonFlatTest = m_pParent->m_CommonFlatTest;
    m_pParent->m_pGraph->ShowPolarGraphWithText( &graphParams );
	m_pParent->SavePolarGraphFile();
}


void CTiltAlignmentPage2::StartMeasure( void )
{
  m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
  SetDlgItemText( IDC_INFO_TEXT, m_Text );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );

  switch( m_NextBedPlaneFunction )
  {
  case BED_PLANE_2_MEASURE_1:
    BedPlane2Measure1();
    break;
  case BED_PLANE_2_MEASURE_2:
    BedPlane2Measure2();
    break;
  case BED_PLANE_2_MEASURE_3:
    BedPlane2Measure3();
    break;
  default:
    //TODO error message
    return;
    break;
  }

	CheckMeasureResult();
}

BOOL CTiltAlignmentPage2::StartTimer()
{
	if( m_pParent->m_CommonFlatTest == TRUE )
	{
		m_pParent->m_pGraph->HideGraph();


		if( (m_CommonFlatTestTimerEvent = SetTimer( COMMON_FLAT_TEST_TIMER, 30000, 0 )) == 0 )
		{
			return( FALSE );
		}

		m_CommonFlatTestTimerOn = TRUE;
		m_Text.LoadString( IDS_WAITING_BEFORE_MEASUREMENT );
		SetDlgItemText( IDC_INFO_TEXT, m_Text );
		GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
		return( TRUE );
	}
	return( FALSE );
}

void CTiltAlignmentPage2::StopTimer( BOOL measure )
{
	if( m_CommonFlatTestTimerOn == TRUE )
	{
		if( KillTimer( m_CommonFlatTestTimerEvent ) == 0 )
		{
			return;
		}
		m_CommonFlatTestTimerOn = FALSE;
		GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );

		if( measure == TRUE )
		{
            MoveDlgToRightSideOfApp(m_pParent );
            //MoveDlgToRightSideOfTheGraph( m_pParent );
			StartMeasure();
		}
	}
}

void CTiltAlignmentPage2::OnTimer(UINT nIDEvent)
{
	//CPropertyPage::OnTimer(nIDEvent);
    MoveDlgToTheCenterOfMainWnd( m_pParent );
	KillTimer( m_CommonFlatTestTimerEvent );
	m_CommonFlatTestTimerOn = FALSE;
    MoveDlgToRightSideOfApp(m_pParent );
    //MoveDlgToRightSideOfTheGraph( m_pParent );
	StartMeasure();
}
