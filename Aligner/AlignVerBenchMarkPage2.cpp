// $Id: AlignVerBenchMarkPage2.cpp,v 1.9 2013/04/26 14:41:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AlignVerBenchMarkPage1.h"
#include "AlignmentWizard.h"
#include "Util.h"
#include "SetupAxesDlg.h"
#include "SetupTestPlaneDlg.h"
#include "AzimuthVerificationWizard.h"
#include "LiveDataView.h"
#include "AlignerDoc.h"

IMPLEMENT_DYNAMIC(CAlignVerBenchMarkPage2, CPropertyPage)

CAlignVerBenchMarkPage2::CAlignVerBenchMarkPage2()
	: CPropertyPage(CAlignVerBenchMarkPage2::IDD)
{
}

CAlignVerBenchMarkPage2::~CAlignVerBenchMarkPage2()
{
}

void CAlignVerBenchMarkPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

void CAlignVerBenchMarkPage2::InitMeasureParam()
{
  //set inparameters for measure
  //this params already initiated:
  //m_pParent->m_Measure.m_InParam.pSource = NULL;
  //m_pParent->m_Measure.m_InParam.SourceStatus = ACQU_IDLE;
  DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			

  m_pParent->m_Measure.m_InParam.Break = FALSE;
  memcpy( m_pParent->m_Measure.m_InParam.ObjNo, g_AlignerData.ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
  m_pParent->m_Measure.m_InParam.RefObjNo = g_AlignerData.RefObjNo;
  m_pParent->m_Measure.m_InParam.Mode1 = m_ModeA;
  m_pParent->m_Measure.m_InParam.Tao = g_AlignerData.TaoAz;
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
  m_pParent->m_Measure.m_InParam.commonFlatTest = FALSE;
    


//   m_pParent -> m_pSource -> SetFrequency( m_pParent -> m_Measure.m_InParam.Fs ) ;
// 	m_pParent -> m_pSource -> Start() ;
}

void CAlignVerBenchMarkPage2::ShowGraphButtons()
{
  GetDlgItem( IDC_GRAPH_BOUNDARY )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_SHOW );
	GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( TRUE );
	CString graphFileName;
	if( m_pParent->m_pGraph->SaveGraphToUniqueFileName( graphFileName ) == TRUE )
	{
		m_pParent->m_GraphFileManager.SaveFileName( graphFileName, FALSE );
	}
}

void CAlignVerBenchMarkPage2::HideGraphButtons()
{
  GetDlgItem( IDC_GRAPH_BOUNDARY )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
}

void CAlignVerBenchMarkPage2::DisableAllButtons()
{
  GetDlgItem( IDC_PRINT_GRAPH )->EnableWindow( FALSE );
  GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( FALSE );
	GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
	GetDlgItem( IDC_CONTINUE_MEASURE )->EnableWindow( FALSE );
	GetDlgItem( IDC_SHOW_RESULT_TABLE )->EnableWindow( FALSE );
}

void CAlignVerBenchMarkPage2::InitResultTable( void )
{
	if( m_pParent->m_AziMode == BenchAzi )
	{
		m_pParent->m_pResultTable->m_InParam.Version = AZIMUTH_VERIFICATION_BENCHMARK;
	}
	else if( m_pParent->m_AziMode == StabAzi )
	{
		m_pParent->m_pResultTable->m_InParam.Version = AZIMUTH_VERIFICATION_GYROSTAB;
	}
	else
	{
		m_pParent->m_pResultTable->m_InParam.Version = AZIMUTH_ALIGN;
	}
  //m_pParent->m_pResultTable->m_InParam.Title = m_TitleAzi;
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
    m_pParent->m_pResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
    m_pParent->m_pResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
    m_pParent->m_pResultTable->m_InParam.pACA = m_ACA;
	m_pParent->m_pResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pParent->m_pResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pParent->m_pResultTable->m_InParam.SavedToDataBase = FALSE;
	m_pParent->m_pResultTable->m_InParam.pTemperature = NULL;
	m_pParent->m_pResultTable->m_InParam.SamplingRate = 0;
    m_pParent->m_pResultTable->m_InParam.pFileGraphManager = &m_pParent->m_GraphFileManager;
}

void CAlignVerBenchMarkPage2::ShowLiveData()
{
	//set the frequency for live data
	DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);	
    //m_pParent -> m_pSource -> SetFrequency( FREQUENCY_FOR_LIVE_DATA ) ;
    //m_pParent -> m_pSource -> Start() ;
    m_pParent -> m_showError = FALSE ;
	
	static_cast<CLiveDataView *>(theApp.m_pLiveDataView)->SetFilterTypeAndTau( TAU_FOR_LIVE_DATA );
    theApp.SwitchView( static_cast<CLiveDataView *>(theApp.m_pLiveDataView) );
}

BEGIN_MESSAGE_MAP(CAlignVerBenchMarkPage2, CPropertyPage)
  ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)
  ON_BN_CLICKED(IDC_SHOW_RESULT_TABLE, OnBnClickedShowResultTable)
  ON_BN_CLICKED(IDC_CONTINUE_MEASURE, OnBnClickedContinueMeasure)
  ON_BN_CLICKED(IDC_PRINT_GRAPH, OnBnClickedPrintGraph)
  ON_BN_CLICKED(IDC_SAVE_GRAPH, OnBnClickedSaveGraph)
  ON_MESSAGE(UM_DAU_ERROR, OnDauError)
END_MESSAGE_MAP()

// CAlignVerBenchMarkPage2 message handlers

BOOL CAlignVerBenchMarkPage2::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CAlignmentWizard*)GetParent();
  ASSERT(m_pParent -> IsKindOf( RUNTIME_CLASS(CAlignmentWizard) ));
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

static void ShowChannel( int arrayIndex )
{
 	int index ;
    if (IsSyncro( arrayIndex ) == TRUE)
 	{
 		index = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
        if (index < DAU::GetDAU().GetSyncroCount())
        {
            DAU::GetDAU().GetSyncro( index ) -> SetIsSelected( TRUE ) ;
            DAU::GetDAU().GetSyncro( index ) -> SetOverrangeDetectionActive( TRUE ) ;
        }
 	}
 	else if (IsSensor( arrayIndex ) == TRUE)
 	{
 		index = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex ) ;
        if (index < DAU::GetDAU().GetSensorCount())
        {
            DAU::GetDAU().GetSensor( index ) -> SetIsSelected( TRUE ) ;
            DAU::GetDAU().GetSensor( index ) -> SetOverrangeDetectionActive( TRUE ) ;
        }
 	}
}

BOOL CAlignVerBenchMarkPage2::OnSetActive() 
{
  if( m_pParent == NULL )
  {
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE; // prevent the page from being active
  }
	if( m_pParent->m_pResultTable == NULL )
	{
		m_pParent->m_pResultTable = new CResultTable( m_pParent );
	}
	InitResultTable();

	m_pParent->m_EnableMoveOfWizard = TRUE;
  m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
  HideAll();
  m_MeasNo = 0;
  m_ShowLiveData = FALSE;
	static_cast<CLiveDataView *>(theApp.m_pLiveDataView)->SetViewMode( ViewCalibrated );

  ClearSelectedChannels();
  //for (int i = 0 ; i < DAU::GetDAU().GetSyncroCount() ; i++)
  //{
  //  DAU::GetDAU().GetSyncro( i ) -> SetIsSelected( FALSE ) ;
  //}
  //for (int i = 0 ; i < DAU::GetDAU().GetSensorCount() ; i++)
  //{
  //  DAU::GetDAU().GetSensor( i ) -> SetIsSelected( FALSE ) ;
  //}
 	if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
 	{
 		ShowChannel( g_AlignerData.RefObjNo );
 	}
  for( int i=1; i<= g_AlignerData.NoOfCorr; i++ )
 	{
 		ShowChannel( g_AlignerData.ObjNo[i] );
 	}
  static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetShowOnlySelected( TRUE ) ;

  m_UseResultFromFirstMeasurement = FALSE;
  if( InitMeasureAz() == FALSE )
  {
    EndDialog( IDCANCEL );
    return FALSE; // prevent the page from being active    
  }

  InitMeasureParam();

  UpdateData( FALSE );

  return CPropertyPage::OnSetActive();
}

BOOL CAlignVerBenchMarkPage2::OnQueryCancel()
{
	if( m_pParent->m_MeasurementInterrupted == TRUE )
	{
		return( TRUE );
	}

  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  if( m_pParent->m_AziMode == NormAzi )
  {
    m_Text.LoadString( IDS_EXIT_WITH_TEMPORARY_SAVE );
  }
  else
  {
    m_Text.LoadString( IDS_EXIT_WITHOUT_SAVE );
  }

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

BOOL CAlignVerBenchMarkPage2::InitMeasureAz()
{
  //FsCom = $F1; {30.52 Hz}
  g_AlignerData.ErrorDef = ERR_NO_ERROR;
  m_Er = FALSE;
  m_C0AziCorrY = 0;
  m_C0AziCorrX = 0;
  m_ModeA = Azim;
  BOOL GunExist = FALSE;

  for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
		if( IsGun( g_AlignerData.ObjNo[i] ) == TRUE )	
    {
      GunExist = TRUE;
    }
  }

	if( IsGun( g_AlignerData.RefObjNo ) == TRUE )
  {
    GunExist = TRUE;
  }

/*  if( (GunExist == TRUE) && (g_AlignerData.GunAdapterTiltCheckOK == FALSE) )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_RUN_GUN_ADAPTER_TILT_CHECK_FIRST );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return( FALSE ); 
  }*/

  switch( m_pParent->m_AziMode )
  {
  case NormAzi:
		m_TitleAzi.LoadString( IDS_AZIMUTH_ALIGNMENT_ERRORS );
    m_Text.LoadString( IDS_KEEP_ALL_PLATFORMS_FIXED );
    SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );
    break;
  case BenchAzi:
		m_TitleAzi.LoadString( IDS_AZIMUTH_VERIFICATION_BENCHMARK_TEST );
    m_Text.LoadString( IDS_KEEP_PLATFORMS_IN_FIXED_ANGLES_RELATIVE_TO_SHIPS_DECK );
    SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );
    break;
  case StabAzi:
		m_TitleAzi.LoadString( IDS_AZIMUTH_VERIFICATION_GYROSTAB_TEST );
    m_Text.LoadString( IDS_KEEP_PLATFORMS_IN_FIXED_ANGLES_RELATIVE_TO_SHIPS_GYRO );
    SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );
    break;
  default:
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INVALID_AZIMUTH_MODE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    return( FALSE );
    break;
  }

  if( g_AlignerData.LogData.ResultFromFirstMeasurementExists == TRUE )
  {
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_RESULT_FROM_FIRST_MEAS_EXISTS_PROCEED_WITH_SECOND_MEAS );

    if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDYES )
    {
      m_UseResultFromFirstMeasurement = TRUE;
    }

    //if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDNO )
    //{
    //  g_AlignerData.LogData.ResultFromFirstMeasurementExists = FALSE;
    //}
  }

  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );

	//if(  g_AlignerData.LogData.ResultFromFirstMeasurementExists == TRUE )
	if(  m_UseResultFromFirstMeasurement == TRUE )
	{
		m_Text.LoadString( IDS_CONTINUE );
		SetDlgItemText( IDC_CONTINUE_MEASURE, m_Text );
		OnBnClickedStartMeasure();
	}

  return( TRUE );
}

BOOL CAlignVerBenchMarkPage2::MeasureAzFirst()
{
  if( CallMeasure( g_AlignerData.ACR, g_AlignerData.LogData.dPPrim ) == FALSE )
  {
    RestoreGlobals();
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
    GetDlgItem( IDC_CONTINUE_MEASURE )->EnableWindow( FALSE );
    HideAllText();
    m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;

    return( FALSE );
  }
  g_AlignerData.LogData.RPrim[g_AlignerData.RefObjNo] = g_AlignerData.ACR[1];
  g_AlignerData.LogData.ResultFromFirstMeasurementExists = TRUE;

	for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		m_TempL[i] = ReadTemperature( i );
  }
  return( TRUE );
}

BOOL CAlignVerBenchMarkPage2::MeasureAzSecond()
{
  if( CallMeasure( g_AlignerData.ACR, m_dPBis ) == FALSE )
  {
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_SAVE_DATA_FROM_THE_FIRST_MEASUREMENT );

    if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO ) == IDNO )
    {
      g_AlignerData.LogData.ResultFromFirstMeasurementExists = FALSE;
    }

    RestoreGlobals();
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
    GetDlgItem( IDC_CONTINUE_MEASURE )->EnableWindow( FALSE );
    HideAllText();
    m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;

    return( FALSE );
  }

  m_RBis[g_AlignerData.RefObjNo] = g_AlignerData.ACR[1];
  g_AlignerData.RExc = g_AlignerData.LogData.RPrim[g_AlignerData.RefObjNo] - m_RBis[g_AlignerData.RefObjNo];

#ifndef _DEBUG
  if( fabs( g_AlignerData.RExc ) < 5 )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_ROLL_EXCITATION_TO_SMALL );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );

    RestoreGlobals();
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
  
    HideAllText();
    m_Text.LoadString( IDS_ROLL_EXCITATION_TO_SMALL );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;
		m_pParent->m_MeasurementInterrupted = TRUE;

    return( FALSE );
  }
#endif

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		SetTemperature( i, (m_TempL[i] + ReadTemperature( i )) / 2 );
  }

  double tmp;

  for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    //Calculation of total change in pitch
    m_dP[i] = g_AlignerData.LogData.dPPrim[i] - m_dPBis[i];
    //Calculation of appr. error in pitch caused by deviation between planes in pitch
		m_dPError[i] = ( 1 - cos( MRADIANS_TO_RADIANS( g_AlignerData.LogData.RPrim[g_AlignerData.RefObjNo] ) ) ) * g_AlignerData.LogData.dPPrim[i] - ( 1 - cos( MRADIANS_TO_RADIANS( m_RBis[g_AlignerData.RefObjNo] ) ) ) * m_dPBis[i];
    tmp = sin( MRADIANS_TO_RADIANS( g_AlignerData.LogData.RPrim[g_AlignerData.RefObjNo] ) ) - sin( MRADIANS_TO_RADIANS( m_RBis[g_AlignerData.RefObjNo] ) );

    if( fabs( tmp ) > DBL_EPSILON )
    {
      m_ACA[i] = -( m_dP[i] + m_dPError[i] ) / tmp;
    }
    else
    {
      m_ACA[i] = 0.0;
    }
    //m_ACA[i] +=  -m_MAT[g_AlignerData.ObjNo[i]] + m_MAT[g_AlignerData.RefObjNo] + 1000 * g_AlignerConst::KConv * ( g_AlignerData.AzOri[g_AlignerData.ObjNo[i]] - g_AlignerData.AzOri[g_AlignerData.RefObjNo] );
		m_ACA[i] +=  GetNominalAzimuthMRad( g_AlignerData.ObjNo[i] ) - GetNominalAzimuthMRad( g_AlignerData.RefObjNo );
  }
  //g_AlignerData.LogData.ResultFromFirstMeasurementExists = FALSE;
 
  return( TRUE );
}

LRESULT CAlignVerBenchMarkPage2::OnWizardBack() 
{
  return CPropertyPage::OnWizardBack();
}

BOOL CAlignVerBenchMarkPage2::OnWizardFinish()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_EXIT_WITH_OPPORTUNITY_TO_SAVE );

    if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
    {
	//		m_pParent->m_GraphFileManager.MoveUnwantedToTemporaryDir();      
			m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
			m_pParent->m_pResultTable->ShowReport( TRUE );
            m_pParent->m_deleteReport = TRUE;
			m_Text.LoadString( IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD );
			g_AlignerData.LogData.ResultFromFirstMeasurementExists = FALSE;

			if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
			{
				DisableAllButtons();				
				m_pParent->ExitResultTable(FALSE);				
                return CPropertyPage::OnWizardFinish();
			}
			else
			{
		//		m_pParent->m_pResultTable->CloseReport();
			}
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

void CAlignVerBenchMarkPage2::OnReset()
{
	if( m_ShowLiveData == TRUE )
	{
		//theApp.SwitchView( theApp.m_pAlignerView );
		theApp.SwitchView( theApp.m_pSystemConfigurationView );
	}
	m_pParent->ExitResultTable( m_pParent->m_deleteReport );
	m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
    CAlignerDoc *pDoc = static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(theApp.m_pMainWnd) -> GetActiveDocument());
    if( pDoc != NULL )
    {
        pDoc->SaveProject();
    }
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CAlignVerBenchMarkPage2::OnBnClickedStartMeasure()
{
    CString tmpStr, tmpStr1;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    m_Text.LoadString( IDS_BREAK );
    GetDlgItemText( IDC_START_MEASURE, tmpStr );

    if( tmpStr == m_Text ) //Break
    {
        //HideAllText();
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        m_pParent->m_Measure.m_InParam.Break = TRUE; 
        return;
    }
    MoveDlgToRightSideOfApp(m_pParent );
    //MoveDlgToRightSideOfTheGraph( m_pParent );
    HideGraphButtons();
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
    SetDlgItemText( IDC_START_MEASURE, m_Text );

    //if( g_AlignerData.LogData.ResultFromFirstMeasurementExists == FALSE )
	    if( m_UseResultFromFirstMeasurement == FALSE )
    {
        m_MeasNo = 1;
        MeasFC( m_MeasNo );
    }
    else
    {
        m_MeasNo = 2;
        MeasFC( m_MeasNo );
    }
}

void CAlignVerBenchMarkPage2::RestoreGlobals()
{
  g_AlignerData.UseElevError = m_pParent->m_Measure.m_InParam.UseElev;
}

BOOL CAlignVerBenchMarkPage2::CallMeasure( double *pRoll, double *pPitch )
{
    //restore the frequency for the measurement
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);	

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

void CAlignVerBenchMarkPage2::HideAllText()
{
  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
}

void CAlignVerBenchMarkPage2::HideAll()
{
  HideAllText();
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_ALIGN_VER_SHOW_GROUP )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_CONTINUE_MEASURE )->ShowWindow( SW_HIDE );
}

void CAlignVerBenchMarkPage2::OnBnClickedShowResultTable()
{
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
	m_pParent->m_pResultTable->ShowReport( FALSE );
	m_pParent->m_deleteReport = TRUE;
}

void CAlignVerBenchMarkPage2::MeasFC( int Lineversion )
{
  CString tmpStr1;

  switch( Lineversion )
  {
  case 1:
    //Writeln('Force the ship to heel a small amount in either direction');
    //Write('Press <ENTER> to start the first measurement - <ESC> to quit');
    m_WhatToDoText.LoadString( IDS_FORCE_THE_SHIP_TO_HEEL_IN_EITHER_DIRECTION );
    tmpStr1.LoadString( IDS_CHECK_LIVE_DATA );
    m_WhatToDoText += _T("\n") + tmpStr1;
    m_ShowLiveData = TRUE;
    break;
  case 2:
    //Writeln('Force the ship to heel a small amount in the other direction');
    //Write('Press <ENTER> to start the second measurement - <ESC> to quit');
    m_WhatToDoText.LoadString( IDS_FORCE_THE_SHIP_TO_HEEL_IN_THE_OTHER_DIRECTION );
    tmpStr1.LoadString( IDS_CHECK_LIVE_DATA );
    m_WhatToDoText += _T("\n") + tmpStr1;

    m_Text.LoadString( IDS_FIRST_MEASURE_READY );
    m_ShowLiveData = FALSE;
    break;
  default:
    break;
  }

  if( m_ShowLiveData == TRUE )
  {
    SetDlgItemText( IDC_INFO_TEXT, m_WhatToDoText );
		HideGraphButtons();
    ShowLiveData();
  }
  else
  {
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
		//if( g_AlignerData.LogData.ResultFromFirstMeasurementExists == FALSE )
		if( m_UseResultFromFirstMeasurement == FALSE )
		{
			ShowGraphButtons();
		}
  }

  m_pParent->m_EnableMoveOfWizard = TRUE;
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_CONTINUE_MEASURE )->ShowWindow( SW_SHOW ); 
  GetDlgItem( IDC_CONTINUE_MEASURE )->EnableWindow( TRUE ); 
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
}

void CAlignVerBenchMarkPage2::OnBnClickedContinueMeasure()
{
    HideGraphButtons();

    if( m_ShowLiveData == FALSE )
    {
        SetDlgItemText( IDC_INFO_TEXT, m_WhatToDoText );
        GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
		m_Text.LoadString( IDS_START_MEASURE );
		SetDlgItemText( IDC_CONTINUE_MEASURE, m_Text );
        m_ShowLiveData = TRUE;
        m_pParent->m_EnableMoveOfWizard = TRUE;
        ShowLiveData();
        return;
    }

    m_pParent -> m_showError = TRUE ;
	m_Text.LoadString( IDS_CONTINUE );
	SetDlgItemText( IDC_CONTINUE_MEASURE, m_Text );
    GetDlgItem( IDC_CONTINUE_MEASURE )->EnableWindow( FALSE );
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
    m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    MoveDlgToRightSideOfApp(m_pParent );
  //MoveDlgToRightSideOfTheGraph( m_pParent );
    m_pParent->m_EnableMoveOfWizard = FALSE;

    switch( m_MeasNo )
    {
    case 1:
        if( MeasureAzFirst() == TRUE )
        {
            m_MeasNo = 2;
            MeasFC( m_MeasNo );
        }
        else
        {
            m_MeasNo = 0;
        }
        break;
    case 2:
        if( MeasureAzSecond() == TRUE )
        {
            ShowGraphButtons();
            m_MeasNo = 0;
            RestoreGlobals();
            GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
            HideAllText();
            m_pParent->m_EnableMoveOfWizard = TRUE;
            m_Text.LoadString( IDS_MEASUREMENT_READY );
            SetDlgItemText( IDC_INFO_TEXT, m_Text );
            GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
            GetDlgItem( IDC_ALIGN_VER_SHOW_GROUP )->ShowWindow( SW_SHOW );
            GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_SHOW );
            m_pParent->SetWizardButtons( PSWIZB_FINISH );
        }
        else
        {
            m_MeasNo = 0;
        }
        break;
    default:
        break;
    }
}

void CAlignVerBenchMarkPage2::OnBnClickedPrintGraph()
{
  m_pParent->m_pGraph->PrintLiveGraph();
}

void CAlignVerBenchMarkPage2::OnBnClickedSaveGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( FALSE );
	m_pParent->m_GraphFileManager.IncludeToResultTable( TRUE, m_pParent->m_pGraph->m_LastSavedGraphFileName );
}

LRESULT CAlignVerBenchMarkPage2::OnDauError( WPARAM, LPARAM )
{
  // this message, UM_DAU_ERROR, is sent from LiveDataView
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED_REPEAT_IT );

	if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDNO )
  {
    static_cast<CLiveDataView *>(theApp.m_pLiveDataView)->LiveClose();
  }
  else
  {
    static_cast<CLiveDataView *>(theApp.m_pLiveDataView)->LiveRestart( TRUE );
  }

	return TRUE;
}
