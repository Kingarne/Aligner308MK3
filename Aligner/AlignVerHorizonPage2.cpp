// $Id: AlignVerHorizonPage2.cpp,v 1.9 2014-07-11 09:29:30 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltVerificationWizard.h"
#include "Util.h"
#include "GlobalData.h"
#include "AlignerDoc.h"

#define MIN_RANGE 0
#define MAX_RANGE 99999
#define MIN_BEAR  0
#define MAX_BEAR  360

IMPLEMENT_DYNAMIC(CAlignVerHorizonPage2, CPropertyPage)

CAlignVerHorizonPage2::CAlignVerHorizonPage2()
	: CPropertyPage(CAlignVerHorizonPage2::IDD)
  , m_Bear(0)
  , m_Range(0)
{
}

CAlignVerHorizonPage2::~CAlignVerHorizonPage2()
{
}

void CAlignVerHorizonPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ALIGN_VER_BEAR, m_Bear);
    DDV_MinMaxDouble(pDX, m_Bear, MIN_BEAR, MAX_BEAR);
    DDX_Text(pDX, IDC_ALIGN_VER_RANGE, m_Range);
    DDV_MinMaxDouble(pDX, m_Range, MIN_RANGE, MAX_RANGE);
}

void CAlignVerHorizonPage2::RangeBearing( RBTypeT RBType )
{
    if( RBType == None )
    {
        return;
    }
    CString tmpStr;

    if( RBType != OnlyBear )
    {
        tmpStr.LoadString( IDS_ENTER_CURRENT_RANGE );
        SetDlgItemText( IDC_RANGE_TEXT, tmpStr );
        GetDlgItem( IDC_RANGE_TEXT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_ALIGN_VER_RANGE )->ShowWindow( SW_SHOW );
    }

    if( RBType != OnlyRange )
    {
        tmpStr.LoadString( IDS_ENTER_CURRENT_BEARING );
        SetDlgItemText( IDC_BEAR_TEXT, tmpStr );
        GetDlgItem( IDC_BEAR_TEXT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_ALIGN_VER_BEAR )->ShowWindow( SW_SHOW );
    }
}

void CAlignVerHorizonPage2::InitMeasureParam()
{
  //set inparameters for measure
  //this params already initiated:
  //m_pParent->m_Measure.m_InParam.pSource = NULL;
  //m_pParent->m_Measure.m_InParam.SourceStatus = ACQU_IDLE;

    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			

    m_pParent->m_Measure.m_InParam.Break = FALSE;
    memcpy( m_pParent->m_Measure.m_InParam.ObjNo, g_AlignerData.ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
    m_pParent->m_Measure.m_InParam.RefObjNo = g_AlignerData.RefObjNo;
    m_pParent->m_Measure.m_InParam.Mode1 = g_AlignerData.Mode;
    m_pParent->m_Measure.m_InParam.Tao = g_AlignerData.TaoTilt;
    m_pParent->m_Measure.m_InParam.FiRef = 0;
    memset( m_pParent->m_Measure.m_InParam.FiObj, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( m_pParent->m_Measure.m_InParam.ZParCorr, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    m_pParent->m_Measure.m_InParam.CountObj = g_AlignerData.NoOfCorr;
    m_pParent->m_Measure.m_InParam.RangeX = g_AlignerData.RX;
    m_pParent->m_Measure.m_InParam.RangeY = g_AlignerData.RY;
    m_pParent->m_Measure.m_InParam.Fs = m_pParent->m_pSource->GetFrequency();
	m_pParent->m_Measure.m_InParam.SperryActive = m_pParent->GetSperryActive();
    m_pParent->m_Measure.m_InParam.commonFlatTest = FALSE;
    
}

void CAlignVerHorizonPage2::StoreGraph()
{
    CString graphFileName;
	if( m_pParent->m_pGraph->SaveGraphToUniqueFileName( graphFileName ) == TRUE )
	{
		//m_pParent->m_GraphFileManager.SaveFileName( graphFileName, FALSE );
	}
}

void CAlignVerHorizonPage2::HideGraphButtons()
{
    
}

void CAlignVerHorizonPage2::DisableAllButtons()
{
 	GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
	DisableBearingAndRangeBox();
}

void CAlignVerHorizonPage2::InitResultTable( void )
{
	if( m_pParent->m_EnableReference == FALSE )
	{
		m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_VerifAbsolute;// HORIZON_ABSOLUTE_MODE;
		//m_pParent->m_pResultTable->m_InParam.Title.LoadString( IDS_TILT_VERIFICATION_AIR_TARGET_TEST );
	}
	else
	{
		m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_VerifRelative;
		//m_pParent->m_pResultTable->m_InParam.Title.LoadString( IDS_TILT_VERIFICATION_HORIZON_TEST );
	}
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;  
	m_pParent->m_pResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
    m_pParent->m_pResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
    m_pParent->m_pResultTable->m_InParam.pACA = NULL;
	m_pParent->m_pResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pParent->m_pResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pParent->m_pResultTable->m_InParam.SavedToDataBase = FALSE;
	m_pParent->m_pResultTable->m_InParam.pTemperature = NULL;
	m_pParent->m_pResultTable->m_InParam.SamplingRate = 0;
    m_pParent->m_pResultTable->m_InParam.pFileGraphManager = &m_pParent->m_GraphFileManager;
}

void CAlignVerHorizonPage2::EnableBearingAndRangeBox()
{
    GetDlgItem( IDC_ALIGN_VER_BEAR )->EnableWindow( TRUE );
    GetDlgItem( IDC_ALIGN_VER_RANGE )->EnableWindow( TRUE );
}

void CAlignVerHorizonPage2::DisableBearingAndRangeBox()
{
    GetDlgItem( IDC_ALIGN_VER_BEAR )->EnableWindow( FALSE );
    GetDlgItem( IDC_ALIGN_VER_RANGE )->EnableWindow( FALSE );
}

BOOL CAlignVerHorizonPage2::IsAlreadyMeasured( double newAngle )
{
	for( int i=1; i<m_pParent->m_N; i++ )
	{
        if( m_pParent->m_XAngle[i] == newAngle )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BEGIN_MESSAGE_MAP(CAlignVerHorizonPage2, CPropertyPage)
    ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)
    ON_EN_KILLFOCUS(IDC_ALIGN_VER_BEAR, OnEnKillfocusAlignVerBear)
    ON_EN_KILLFOCUS(IDC_ALIGN_VER_RANGE, OnEnKillfocusAlignVerRange)
    ON_BN_CLICKED(IDC_FINISH_MEASURE, &CAlignVerHorizonPage2::OnBnClickedFinishMeasure)
END_MESSAGE_MAP()

// CAlignVerHorizonPage2 message handlers

BOOL CAlignVerHorizonPage2::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_pParent = (CAlignmentWizard*)GetParent();
    ASSERT(m_pParent -> IsKindOf( RUNTIME_CLASS(CAlignmentWizard) ));
    m_MsgCaption = _T("");
    m_Text = _T("");
    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAlignVerHorizonPage2::OnSetActive() 
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

    m_pParent->m_EnableMoveOfWizard = TRUE;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    HideAll();
    m_Bear = 0;
    m_Range = 0;

    if( HorizonTestInit() == FALSE )
    {
        //CPropertyPage::OnWizardBack();
        return( FALSE ); // prevent the page from being active
    }
    InitMeasureParam();
    HorizonTestLoopInit();

    m_pParent->m_Measure.m_InParam.Break = FALSE;
	GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_HIDE);

    UpdateData( FALSE );

    return CPropertyPage::OnSetActive();
}

BOOL CAlignVerHorizonPage2::OnQueryCancel()
{
	g_AlignerData.UseParallax = m_UseParallaxValueToBeRestoredOnCancel;

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
			//HideAllText();
			m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			m_pParent->m_Measure.m_InParam.Break = TRUE; 
		}
		return( FALSE );
	}
}

LRESULT CAlignVerHorizonPage2::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

BOOL CAlignVerHorizonPage2::OnWizardFinish()
{
	switch (m_pParent->m_Status)
	{
	case STATUS_PAGE_READY:
	{				
		if (m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS)
		{
			ShowErrorGraph();
		}
		ShowResultGraph();
		ShowPolarGraph();
		
		m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
		m_pParent->m_pResultTable->ShowReport();		
			   
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

void CAlignVerHorizonPage2::OnReset()
{	
	m_pParent->m_Status = STATUS_PAGE_CANCELED;
	g_AlignerData.ErrorDef = ERR_CANCEL;
  
	return CPropertyPage::OnReset(); //Calls OnCancel()
}

BOOL CAlignVerHorizonPage2::HorizonTestInit()
{
    CString tmpStr;
    HideAllText();

    m_pParent->m_Leave = FALSE;
    g_AlignerData.ElevErrorExist = TRUE;
    m_pParent->m_Measure.m_InParam.UseElev = FALSE;
    m_Repeat = FALSE;

    if( m_pParent->m_EnableReference == FALSE )
    {
        g_AlignerData.RefObjNo = -1;

        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
	        if( GetEalAbs( g_AlignerData.ObjNo[i] ) == INVALID_VALUE )
            {
                g_AlignerData.ElevErrorExist = FALSE;
            }
        }
    }
    else
    {
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if( g_AlignerData.EalS[i] == INVALID_VALUE )
            {
                g_AlignerData.ElevErrorExist = FALSE;
            }
        }
    }

    if( g_AlignerData.ElevErrorExist == TRUE )
    {
        m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
        m_Text.LoadString( IDS_TILT2_ELEVATION_ERROR_ALREADY_MEASURED_USE_OLD );

        if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDYES )
        {
            m_Range = g_AlignerData.Range;

            if( m_pParent->m_UseParallaxComp == g_AlignerData.UseParallax )
            {
                m_pParent->m_Measure.m_InParam.UseElev = TRUE;
            }
            else
            {
                m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );

                if( g_AlignerData.UseParallax == TRUE )
                {
                    m_Text.LoadString( IDS_PLEASE_CHANGE_PARALLAX_COMP_ON );
                }
                else
                {
                    m_Text.LoadString( IDS_PLEASE_CHANGE_PARALLAX_COMP_OFF );
                }        
                MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
                return( FALSE );
		    }
        }
        m_UseParallaxValueToBeRestoredOnCancel = g_AlignerData.UseParallax;
	}
	else
	{
		m_UseParallaxValueToBeRestoredOnCancel = m_pParent->m_UseParallaxComp;
	}
    g_AlignerData.UseParallax = m_pParent->m_UseParallaxComp;

	//***  070131 : Slight diff in text in absolute and rel (Horizon and Air) modes
	if( m_pParent->m_EnableReference == FALSE )
	{
		//m_pParent->m_pResultTable->m_InParam.Version = HORIZON_ABSOLUTE_MODE;
  	    m_Text.LoadString( IDS_USE_MODES_OR_SIMILAR );
	}
	else
	{
	    //		m_pParent->m_pResultTable->m_InParam.Version = AIR_TARGET_RELATIVE_MODE;
  	    m_Text.LoadString( IDS_USE_MODES_OR_SIMILAR_AIR );
	}
	    //	m_Text.LoadString( IDS_USE_MODES_OR_SIMILAR );
	    //***  End 070131

    if( m_pParent->m_UseParallaxComp == TRUE )
    {
        tmpStr.LoadString( IDS_Z_PARALLAX_IS_MADE );
	}
    else
    {
  	    //***  070205 : Slight diff in text in absolute and rel (Horizon and Air) modes
		if( m_pParent->m_EnableReference == FALSE )	
		{																					// Absolute (horizon) mode
  		    tmpStr.LoadString( IDS_IMPORTANT_TARGET_MUST_BE );
		}
		else
		{																					// Relative (air) mode
  		    tmpStr.LoadString( IDS_SLAVE_GUNS_TO_THE_TRACKER );
	        //***  End 070205
        }
    }
    m_Text +=  _T("\n") + tmpStr;

    if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
    {
        tmpStr.LoadString( IDS_TWO_UNKNOWN_VARIABLES_REQUIRE );
    }
    else
    {
        tmpStr.LoadString( IDS_THREE_UNKNOWN_VARIABLES_REQUIRE );
    }
    m_Text +=  _T("\n") + tmpStr;
    SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );

    m_pParent->m_N = 0;
    m_pParent->m_Ymax = -100;
    m_pParent->m_Ymin = 100;
    m_pParent->m_ERan = 0;
    memset( m_pParent->m_MaxSineFitError, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( m_pParent->m_Esquare, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( m_pParent->m_Eave, 0, SIZE_OF_ARRAYS * sizeof( double ) );

    return( TRUE );
 }

void CAlignVerHorizonPage2::HorizonTestLoopInit()
{
    CString tmpStr, tmpStr1;
	    m_RBType = None;

    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_BEAR_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_RANGE_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_ALIGN_VER_BEAR )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_ALIGN_VER_RANGE )->ShowWindow( SW_HIDE );

	if( m_Repeat == FALSE )
	{
		m_pParent->m_N++;
	}
    tmpStr.Format( _T("%d"), m_pParent->m_N );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_NEXT_MEAS_NO, tmpStr );
    SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, m_Text );
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );

    if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
    {
        if( m_pParent->m_N == 1 )
        {
            m_Text.LoadString( IDS_DESIGNATE_PLATFORMS_TO_BEARING_0 );
            m_Bear = 0;
        }
        else
        {
            m_Text.LoadString( IDS_DESIGNATE_PLATFORMS_TO_BEARING_90 );
            m_Bear = 90;
        }

        if( m_pParent->m_UseParallaxComp == TRUE )
        {
            m_RBType = OnlyRange;
        }
        else
        {
            tmpStr1.Format( _T("%.0f"), m_Range );
            AfxFormatString1( tmpStr, IDS_RANGE_SHALL_BE, tmpStr1 );
            m_Text += _T("\n") + tmpStr;
            m_RBType = None;
        }
    }
    else
    {
  	    //***  070205 : Slight diff in text in absolute and rel (Horizon and Air) modes
		if( m_pParent->m_EnableReference == FALSE )	
		{																					// Absolute (horizon) mode
            m_Text.LoadString( IDS_DESIGNATE_PLATFORMS_TO_DESIRED_BEARING );
		}
		else
		{																					// Relative (air) mode
            m_Text.LoadString( IDS_DESIGNATE_PLATFORMS_TO_DESIRED_BEARING_AIR );
		}
	    //***  End 070205
        if( ( m_pParent->m_N != 1 ) && ( m_pParent->m_UseParallaxComp == FALSE ) )
        {
            tmpStr1.Format( _T("%.0f"), m_Range );
            AfxFormatString1( tmpStr, IDS_RANGE_SHALL_BE, tmpStr1 );
            m_Text += _T("\n") + tmpStr;
            m_RBType = OnlyBear;
        }
        else
        {
            m_RBType = Both;
        }
    }
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    RangeBearing( m_RBType );
	EnableBearingAndRangeBox();
    EnableStartButton();
}

void CAlignVerHorizonPage2::EnableStartButton()
{
  m_Text.LoadString( IDS_START_MEASURE );
  SetDlgItemText( IDC_START_MEASURE, m_Text );
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
}

void CAlignVerHorizonPage2::OnBnClickedStartMeasure()
{
    CString tmpStr, tmpStr1;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    m_Text.LoadString( IDS_BREAK );
    GetDlgItemText( IDC_START_MEASURE, tmpStr );

    if( tmpStr == m_Text ) //Break
    {
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        m_pParent->m_Measure.m_InParam.Break = TRUE; 
        return;
    }

    if( ( m_Bear < MIN_BEAR ) || ( m_Bear > MAX_BEAR ) )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        tmpStr1.LoadString( IDS_INVALID_BEARING );
        tmpStr.LoadString( IDS_ENTER_CURRENT_BEARING );
        MessageBox( tmpStr1 + tmpStr, m_MsgCaption, MB_ICONERROR|MB_OK );    
        return;
    }

	if( IsAlreadyMeasured( m_Bear ) == TRUE )
	{
		m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
		tmpStr.Format( _T("%.1f"), m_Bear );
		AfxFormatString1( tmpStr1, IDS_ANGLE_ALREADY_MEASURED_AGAIN, tmpStr );

		if( IDNO == MessageBox( tmpStr1, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) )
		{
			return;
		}
	}

    if( ( m_Range <= MIN_RANGE ) || ( m_Range > MAX_RANGE ) )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        tmpStr1.LoadString( IDS_INVALID_RANGE );
        tmpStr.LoadString( IDS_ENTER_CURRENT_RANGE );
        MessageBox( tmpStr1 + tmpStr, m_MsgCaption, MB_ICONERROR|MB_OK );    
        return;
    }

    m_pParent->m_XAngle[m_pParent->m_N] = m_Bear;
    double angRad = m_Bear*M_PI/180.0f;

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(g_AlignerData.ObjNo[i]));
        m_pParent->m_Measure.m_InParam.FiObj[i] = angRad;
        m_pParent->m_Measure.m_InParam.rotateBack[i] = angRad;
        if(pSensor)
        {
            if( m_pParent->m_UseParallaxComp == TRUE )    
            {
                double zParCorr = ( GetZPar( g_AlignerData.ObjNo[i] ) ) / ( m_Range + 0.001 );
                m_pParent->m_Measure.m_InParam.ZParCorr[g_AlignerData.ObjNo[i]] = zParCorr;
                //Tell sensor                        
                pSensor->m_zParCorr = -zParCorr;
            }
        }
    }
    if(( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) && ( m_pParent->m_EnableReference == TRUE ) )
    {
        Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(g_AlignerData.RefObjNo));    
        if(pSensor)
        {
            m_pParent->m_Measure.m_InParam.FiRef = angRad;
            m_pParent->m_Measure.m_InParam.rotateBackRef = angRad;
            if( m_pParent->m_UseParallaxComp == TRUE )    
            {
                double zParCorr = ( GetZPar( g_AlignerData.RefObjNo ) ) / ( m_Range + 0.001 );
                m_pParent->m_Measure.m_InParam.ZParCorr[g_AlignerData.RefObjNo] = zParCorr;
                //Tell sensor
                pSensor->m_zParCorr = -zParCorr;
            }
        }
    }      

    MoveDlgToRightSideOfApp(m_pParent );
    //MoveDlgToRightSideOfTheGraph( m_pParent );
    HideGraphButtons();
    DisableBearingAndRangeBox();

    SetDlgItemText( IDC_START_MEASURE, m_Text );
    m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = FALSE;

    tmpStr.Format( _T("%d"), m_pParent->m_N );
	AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_MEAS_NO, tmpStr );
    SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, m_Text );
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );
    	
	RestoreGlobals();

    if( CallMeasure( g_AlignerData.ACR, g_AlignerData.ACP ) == FALSE )
    {
		if( m_Repeat == TRUE )
		{
			HorizonTestLoopInit();
		}
		else
		{
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			DisableBearingAndRangeBox();
			m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
			SetDlgItemText( IDC_INFO_TEXT, m_Text );
			GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
			m_pParent->m_EnableMoveOfWizard = TRUE;
		}
        return;
    }

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_Y[i][m_pParent->m_N] = g_AlignerData.ACP[i];

        if( ( m_pParent->m_Measure.m_InParam.UseElev == TRUE ) && ( m_pParent->m_N ==1 ) )
        {
            m_pParent->m_Y[i][m_pParent->m_N+2] = -g_AlignerData.ACP[i];
            m_pParent->m_XAngle[m_pParent->m_N+2] = m_pParent->m_XAngle[m_pParent->m_N]+180;
        }
    }

    StoreGraph();
    EnableBearingAndRangeBox();
    m_Text.LoadString( IDS_MEASUREMENT_READY );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;

    tmpStr.Format( _T("%d"), m_pParent->m_N );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_N_MEAS_COMPLETED, tmpStr );

    if( (m_pParent->m_N >= MIN_NO_OF_FLATNESS_MEASUREMENTS) && (m_pParent->m_N < MAX_NO_OF_FLATNESS_MEASUREMENTS) )
    {
		GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_SHOW);
	/*	m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
        tmpStr.LoadString( IDS_TILT_FLATNESS_TEST_MORE_MEASUREMENTS );
        m_Text += tmpStr;
        if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDNO )
        {
         m_pParent->m_Leave = TRUE;
        }*/
    }
    else if( (( m_pParent->m_Measure.m_InParam.UseElev == TRUE ) && ( m_pParent->m_N == 2 )) || ( m_pParent->m_N == MAX_NO_OF_FLATNESS_MEASUREMENTS ) )
    {
        m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
        tmpStr.LoadString( IDS_TILT_FLATNESS_TEST_NO_MORE_MEASUREMENTS );
        m_Text += tmpStr;
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        m_pParent->m_Leave = TRUE;
    }

    if( m_pParent->m_Leave == FALSE )
    {
        HorizonTestLoopInit();
    }
    else
    {
        RestoreGlobals();
        DisableBearingAndRangeBox();
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        HorizonTestContinue();
    }
}

void CAlignVerHorizonPage2::OnBnClickedFinishMeasure()
{
	m_pParent->m_N--;
	RestoreGlobals();
	DisableBearingAndRangeBox();
	GetDlgItem(IDC_START_MEASURE)->EnableWindow(FALSE);
	HorizonTestContinue();
}


void CAlignVerHorizonPage2::RestoreGlobals()
{
  g_AlignerData.UseElevError = m_pParent->m_Measure.m_InParam.UseElev;
  g_AlignerData.Bear = m_Bear;
  g_AlignerData.Range = m_Range;
}

BOOL CAlignVerHorizonPage2::CallMeasure( double *pRoll, double *pPitch )
{
	m_Repeat = FALSE;

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

            if( DAU::GetDAU().IsOpen() == FALSE || MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDYES )
            {
                DAU::GetDAU().Start();                
			    m_Repeat = TRUE;
            }
			else
			{
				m_pParent->m_MeasurementInterrupted = TRUE;
			}
			return FALSE;
        }
  }while( ( g_AlignerData.ErrorFlag == TRUE ) || ( g_AlignerData.Reject == TRUE ) );

    //Now the measurement is ready, get the timestamp to be stored in the graph and table
    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_pParent->m_MeasurementReadyTimeStamp );

    memcpy( pRoll, m_pParent->m_Measure.m_OutParam.DeltaRoll, SIZE_OF_ARRAYS * sizeof( double ) );
    memcpy( pPitch, m_pParent->m_Measure.m_OutParam.DeltaPitch, SIZE_OF_ARRAYS * sizeof( double ) );
	CopyTemperature( m_pParent->m_Measure.m_OutParam.TempT );

    return TRUE;
}

void CAlignVerHorizonPage2::HorizonTestContinue()
{
    if( m_pParent->m_Measure.m_InParam.UseElev == TRUE )
    {
        m_pParent->m_N = MIN_NO_OF_FLATNESS_MEASUREMENTS;
    }

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        SineFit( m_pParent->m_XAngle, m_pParent->m_Y, i, m_pParent->m_N, &(g_AlignerData.ACR[i]), &(g_AlignerData.ACP[i]), &(m_pParent->m_MeanSineFit[i]) );
        CartToVec( g_AlignerData.ACR[i], g_AlignerData.ACP[i], &(g_AlignerData.VecAmp[i]), &(g_AlignerData.VecArg[i]) );
        m_pParent->m_Fi[i] = DEGREES_TO_RADIANS( g_AlignerData.VecArg[i] );
        
        for( int j=1; j<=m_pParent->m_N; j++ )
        {
        m_pParent->m_SineFitError[i][j] = m_pParent->m_Y[i][j] - (m_pParent->m_MeanSineFit[i] + g_AlignerData.VecAmp[i] * cos( DEGREES_TO_RADIANS( m_pParent->m_XAngle[j] ) - m_pParent->m_Fi[i] ));

        if( fabs( m_pParent->m_SineFitError[i][j] ) > fabs( m_pParent->m_MaxSineFitError[i] ) )
        {
            m_pParent->m_MaxSineFitError[i] = m_pParent->m_SineFitError[i][j];
            m_pParent->m_XAngleForMaxSineFitError[i] = m_pParent->m_XAngle[j];
        }
        m_pParent->m_Eave[i] = m_pParent->m_Eave[i] + m_pParent->m_SineFitError[i][j];
        m_pParent->m_Esquare[i] = m_pParent->m_Esquare[i] + SQUARE( m_pParent->m_SineFitError[i][j] );

        if( m_pParent->m_Y[i][j] > m_pParent->m_Ymax )
        {
            m_pParent->m_Ymax = m_pParent->m_Y[i][j];
        }

        if( m_pParent->m_Y[i][j] < m_pParent->m_Ymin )
        {
            m_pParent->m_Ymin = m_pParent->m_Y[i][j];
        }

        if( fabs(m_pParent->m_MaxSineFitError[i]) > m_pParent->m_ERan )
        {
            m_pParent->m_ERan = fabs( m_pParent->m_MaxSineFitError[i] );
        }
        }
    
        g_AlignerData.Sigma[i] = sqrt( m_pParent->m_Esquare[i] / ( m_pParent->m_N - 1) );

        for( int j=1; j<=360; j++ )
        {
            m_pParent->m_F = m_pParent->m_MeanSineFit[i] + g_AlignerData.VecAmp[i] * cos( DEGREES_TO_RADIANS( j ) - m_pParent->m_Fi[i] );

            if( m_pParent->m_F > m_pParent->m_Ymax )
            {
                m_pParent->m_Ymax = m_pParent->m_F;
            }
            else if( m_pParent->m_F < m_pParent->m_Ymin )
            {
                m_pParent->m_Ymin = m_pParent->m_F;
            }
        }

        if( m_pParent->m_Measure.m_InParam.UseElev == FALSE )
        {
            g_AlignerData.EalS[i] = m_pParent->m_MeanSineFit[i];
			
            SetEalAbs( g_AlignerData.ObjNo[i], g_AlignerData.EalS[i] );

//             if( m_pParent->m_EnableReference == FALSE )
//             {
// 			    SetEalAbs( g_AlignerData.ObjNo[i], g_AlignerData.EalS[i] );
//             }
        }
    }
	m_UseParallaxValueToBeRestoredOnCancel = g_AlignerData.UseParallax;

    
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
    m_pParent->SetWizardButtons( PSWIZB_FINISH );
}

void CAlignVerHorizonPage2::OnEnKillfocusAlignVerBear()
{
    UpdateData( TRUE );
}

void CAlignVerHorizonPage2::HideAllText()
{
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_BEAR_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_RANGE_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
}

void CAlignVerHorizonPage2::HideAll()
{
    HideAllText();
    GetDlgItem( IDC_ALIGN_VER_BEAR )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_ALIGN_VER_RANGE )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );   
    
}

	/***************************************************************************/
	/*													ShowResultGraph										 */
	/***************************************************************************/
void CAlignVerHorizonPage2::ShowResultGraph()
{
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
    m_YRan = RoundToInt( g_AlignerData.Kh * (m_pParent->m_Ymax - m_pParent->m_Ymin) + 1.5 );
    m_YBias = RoundToInt( g_AlignerData.Kh * (m_pParent->m_Ymax + m_pParent->m_Ymin) / 2 );

    CString str;
    UnitModeT UMode;
    TiltFlatnessTestGraphInParam graphParams;
    graphParams.PlMode = Sine;
    graphParams.mode = m_pParent->m_Measure.m_InParam.Mode1;
    graphParams.RefNo = g_AlignerData.RefObjNo;
    graphParams.timeConstant = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.Points = m_pParent->m_N;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
      
    m_pParent->m_pGraph->InitDefaultPlotGraph( 2* g_AlignerData.NoOfCorr );
    //modify the default plot graph for tilt&flatness
    m_pParent->m_pGraph->SetUseCommonXValues( FALSE );
    m_pParent->m_pGraph->SetAxisXMin( 0 );
    m_pParent->m_pGraph->SetAxisXMax( 360 );
    str.LoadString( IDS_DEG_UNIT );
    m_pParent->m_pGraph->SetAxisXUnit( &str );

    if( GetMRad() == TRUE )
    {
        UMode = mr;
        str.LoadString( IDS_MRAD_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
    }
    else
    {
        UMode = arcmin;
        str.LoadString( IDS_ARCMIN_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
    }
      
    int k = 1;
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_pGraph->SetLinePattern( k, oc2dLineNone );
        m_pParent->m_pGraph->SetPointShape( k,  m_pParent->m_pGraph->GetPointShape( i ) );
        m_pParent->m_pGraph->SetPointColor( k, m_pParent->m_pGraph->GetColor( i ) );
		    m_pParent->m_pGraph->SetSerieLabel( k, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
        k++;
        m_pParent->m_pGraph->SetLinePattern( k, oc2dLineSolid );//sineFit
        m_pParent->m_pGraph->SetLineColor( k, m_pParent->m_pGraph->GetColor( i ) );//sineFit
        m_pParent->m_pGraph->SetPointShape( k, oc2dShapeNone );//sineFit
        m_pParent->m_pGraph->SetSerieLabel( k, _T(" ") );//sineFit
        k++;
    }

    double yPlot;
    //set tilt&flatness data for the graph
    for( int j=1; j<=m_pParent->m_N; j++ )
    {
        k = 1;
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			    /***  1.14 ; The code below replaces the out-commented code below  ***/
			    /***  Y[i,j] already scaled with arcmin factor .Kh  ***/
        yPlot = m_pParent->m_Y[i][j];
			    /***  Initially, g_AlignerData.Kh is set = 1 or = MTA (3,4..) if mrad or arcmin is selected, respectively  ***/
    			
		    //if( UMode == mr )
    //   {
    //     yPlot = m_pParent->m_Y[i][j];
    //   }
    //   else
    //   { //UMode == arcmin
    //     yPlot = m_pParent->m_Y[i][j] * g_AlignerData.Kh;
    //   }
        m_pParent->m_pGraph->SetXYData( k, m_pParent->m_XAngle[j], yPlot );
        k += 2;
        }
        m_pParent->m_pGraph->UpdateData();
    }

    //set the sineFit data for the graph
    for( int deg=0; deg<=360; deg++ )
    {
        k = 2;
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
        m_pParent->m_F = m_pParent->m_MeanSineFit[i] + g_AlignerData.VecAmp[i] * cos( DEGREES_TO_RADIANS( deg ) - m_pParent->m_Fi[i] );
        yPlot = m_pParent->m_F;
        m_pParent->m_pGraph->SetXYData( k, deg, yPlot );
        k += 2;
        }
        m_pParent->m_pGraph->UpdateData();
    }
        
    m_pParent->m_pGraph->ShowTiltFlatnessTestGraphWithText( &graphParams );
	m_pParent->SaveResultGraphFile();
}

	/***************************************************************************/
	/*													ShowErrorGraph											 */
	/***************************************************************************/
void CAlignVerHorizonPage2::ShowErrorGraph()
{
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
    UnitModeT UMode;
    int m_YRan;
    CString str;
    TiltFlatnessTestGraphInParam graphParams;
    graphParams.PlMode = Linear;
    graphParams.mode = m_pParent->m_Measure.m_InParam.Mode1;
    graphParams.RefNo = g_AlignerData.RefObjNo;
    graphParams.timeConstant = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.Points = m_pParent->m_N;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );

    m_pParent->m_pGraph->InitDefaultPlotGraph( g_AlignerData.NoOfCorr );
    //modify the default plot graph for tilt&flatness
    m_pParent->m_pGraph->SetUseCommonXValues( TRUE );
    m_pParent->m_pGraph->SetAxisXMin( 0 );
    m_pParent->m_pGraph->SetAxisXMax( 360 );  
    str.LoadString( IDS_DEG_UNIT );
    m_pParent->m_pGraph->SetAxisXUnit( &str );

    if( ( GetMRad() == TRUE ) || ( g_AlignerData.Found == TRUE ) )
    {
        m_YRan = RoundToInt( 2000 * m_pParent->m_ERan + 1.5 );

        if( m_YRan > 2000 )
        {
        m_YRan = RoundToInt( m_YRan / 1000 +1.5 );

        if( g_AlignerData.Found == TRUE )
        {
            UMode = mm;
            str.LoadString( IDS_MM_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        else
        {
            UMode = mr;
            str.LoadString( IDS_MRAD_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        }
        else
        {
        if( g_AlignerData.Found == TRUE )
        {
            UMode = um;
            str.LoadString( IDS_UM_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        else
        {
            UMode = ur;
            str.LoadString( IDS_URAD_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        }
    }
    else
    {
        m_YRan = RoundToInt( 120 * g_AlignerData.Kh * m_pParent->m_ERan + 1.5 );
        if( m_YRan > 500 )
        {
        m_YRan = RoundToInt( m_YRan / 60 + 1.5 );
        UMode = arcmin;
        str.LoadString( IDS_ARCMIN_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        else
        {
        UMode = arcsec;
        str.LoadString( IDS_ARCSEC_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
    }

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_pGraph->SetLinePattern( i, oc2dLineNone );
        m_pParent->m_pGraph->SetPointShape( i,  m_pParent->m_pGraph->GetPointShape( i ) );
        m_pParent->m_pGraph->SetPointColor( i, m_pParent->m_pGraph->GetColor( i ) );
		    m_pParent->m_pGraph->SetSerieLabel( i, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
    }

    double yPlot;

    //set tilt&flatness data for the graph
    for( int j=1; j<=m_pParent->m_N; j++ )
    {
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {

    			
			    /***  1.14 ; The code below replaces the out-commented code below  ***/
			    /***  SineFitError already scaled with arcmin factor .Kh  ***/
    //			yPlot = m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
			    yPlot = m_pParent->m_SineFitError[i][j] ;
			    if ((UMode == ur) || (UMode == um))
				    yPlot = yPlot * 1000 ;
			    if (UMode == arcsec)
				    yPlot = yPlot * 60 ;
			/***  Initially, g_AlignerData.Kh is set = 1 or = MTA (3,4..) if mrad or arcmin is selected, respectively  ***/

        //if( (UMode == mr) || (UMode == mm) )
        //{
        //  yPlot = m_pParent->m_SineFitError[i][j];
        //}
        //else if( UMode == arcmin )
        //{
        //  yPlot = m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
        //}
        //else if( (UMode == ur) || (UMode == um) )
        //{
        //  yPlot = 1000 * m_pParent->m_SineFitError[i][j];
        //}
        //else
        //{ //arcsec
        //  yPlot = 60 * m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
        //}
        m_pParent->m_pGraph->SetXYData( i, m_pParent->m_XAngle[j], yPlot );
        }
        m_pParent->m_pGraph->UpdateData();
    }

    m_pParent->m_pGraph->ShowTiltFlatnessTestGraphWithText( &graphParams );
	m_pParent->SaveErrorGraphFile();
}

void CAlignVerHorizonPage2::ShowPolarGraph()
{
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );

    //init the graph
    m_pParent->m_pGraph->InitDefaultPolarGraph( g_AlignerData.NoOfCorr );

    //set data and labels
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_pGraph->SetXYData( i, g_AlignerData.VecArg[i], g_AlignerData.VecAmp[i] );
		    m_pParent->m_pGraph->SetSerieLabel( i, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
    }
    m_pParent->m_pGraph->UpdateData();
    PolarGraphInParam graphParams;  
    graphParams.mode = m_pParent->m_Measure.m_InParam.Mode1;
    graphParams.tao = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.RefNo = m_pParent->m_Measure.m_InParam.RefObjNo;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
    graphParams.commonFlatTest = FALSE;
    m_pParent->m_pGraph->ShowPolarGraphWithText( &graphParams );
	m_pParent->SavePolarGraphFile();
}


void CAlignVerHorizonPage2::OnEnKillfocusAlignVerRange()
{
    UpdateData( TRUE );
}

