// $Id: GyroPerformanceTestPage2.cpp,v 1.8 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GyroPerformanceTestWizard.h"
#include "SetupDriftDlg.h"
#include "Util.h"
#include "AlignerDoc.h"

// CGyroPerformanceTestPage2 dialog

IMPLEMENT_DYNAMIC(CGyroPerformanceTestPage2, CPropertyPage)
CGyroPerformanceTestPage2::CGyroPerformanceTestPage2()
	: CPropertyPage(CGyroPerformanceTestPage2::IDD)
{
}

CGyroPerformanceTestPage2::~CGyroPerformanceTestPage2()
{
}

void CGyroPerformanceTestPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

void CGyroPerformanceTestPage2::HideAllText()
{
  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
}

void CGyroPerformanceTestPage2::HideAll()
{
  HideAllText();
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_GROUP )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_POLAR_GRAPH )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_HIDE );
}

void CGyroPerformanceTestPage2::ShowGraphButtons()
{
	GetDlgItem( IDC_GRAPH_BOUNDARY )->ShowWindow( SW_SHOW );
	GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_SHOW );
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_SHOW );
	//GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( TRUE );
	
}

void CGyroPerformanceTestPage2::HideGraphButtons()
{
  GetDlgItem( IDC_GRAPH_BOUNDARY )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_HIDE );
  //GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
}

void CGyroPerformanceTestPage2::DisableAllButtons()
{
  GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_HIDE );
  //GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_SHOW_POLAR_GRAPH )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_HIDE );
}

void CGyroPerformanceTestPage2::InitResultTable( void )
{
	m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_GyroPerf;// GYRO_PERFORMANCE_TEST;
	//m_pParent->m_pResultTable->m_InParam.Title.LoadString( IDS_GYRO_PERFORMANCE_TEST );
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


BEGIN_MESSAGE_MAP(CGyroPerformanceTestPage2, CPropertyPage)
  ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)
  ON_BN_CLICKED(IDC_SHOW_POLAR_GRAPH, OnBnClickedShowPolarGraph)
  ON_BN_CLICKED(IDC_SHOW_RESULT_TABLE, OnBnClickedShowResultTable)
  ON_BN_CLICKED(IDC_PRINT_GRAPH, OnBnClickedPrintGraph)
  ON_BN_CLICKED(IDC_SAVE_GRAPH, OnBnClickedSaveGraph)
END_MESSAGE_MAP()


// CGyroPerformanceTestPage2 message handlers

BOOL CGyroPerformanceTestPage2::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CGyroPerformanceTestWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CGyroPerformanceTestPage2::OnSetActive() 
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
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    return( FALSE ); // prevent the page from being active
  }

  InitMeasureParam();

  m_Text.LoadString( IDS_ATTACH_A_SENSOR_ON_THE_FIXED_PLANE );
  SetDlgItemText( IDC_INTRO_TEXT, m_Text );
  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );

  EnableStartButton();

  UpdateData( FALSE );

  m_pParent->m_Status = STATUS_PAGE_READY;
  g_AlignerData.ErrorDef = ERR_NO_ERROR;

  return CPropertyPage::OnSetActive();
}

BOOL CGyroPerformanceTestPage2::OnQueryCancel()
{
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
		return( FALSE );
	}
}

LRESULT CGyroPerformanceTestPage2::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

BOOL CGyroPerformanceTestPage2::OnWizardFinish()
{
	switch( m_pParent->m_Status )
	{
		case STATUS_PAGE_READY:
		{
			m_MsgCaption.LoadString(IDS_QUESTION_CAPTION);

			OnBnClickedShowPolarGraph();

			m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
			m_pParent->m_pResultTable->ShowReport(TRUE);
			m_pParent->m_deleteReport = TRUE;
			m_Text.LoadString(IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD);

			int res = MessageBox(m_Text, m_MsgCaption, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);
			m_pParent->ExitResultTable(res != IDYES);
			m_pParent->ExitResultTable(FALSE);

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

void CGyroPerformanceTestPage2::OnReset()
{
	m_pParent->ExitResultTable( m_pParent->m_deleteReport );
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CGyroPerformanceTestPage2::GyroProc()
{
}

void CGyroPerformanceTestPage2::OnBnClickedStartMeasure()
{
  HideAllText();
  m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
  m_Text.LoadString( IDS_BREAK );
  CString tmpStr;
  GetDlgItemText( IDC_START_MEASURE, tmpStr );

  if( tmpStr == m_Text )
  {
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
    m_pParent->m_Measure.m_InParam.Break = TRUE; 
    return;
  }

  MoveDlgToRightSideOfApp(m_pParent );
  //MoveDlgToRightSideOfTheGraph( m_pParent );
  HideGraphButtons();
  SetDlgItemText( IDC_START_MEASURE, m_Text );
  m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
  SetDlgItemText( IDC_INFO_TEXT, m_Text );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
  m_pParent->m_EnableMoveOfWizard = FALSE;

  if( CallMeasure( g_AlignerData.ACR, g_AlignerData.ACP ) == FALSE )
  {
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

    m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;
    return;
  }

  GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

  ShowGraphButtons();
  
  CString graphFileName;
  if (m_pParent->m_pGraph->SaveGraphToUniqueFileName(graphFileName, TRUE) == TRUE)
  {
	  m_pParent->m_GraphFileManager.SaveFileName(graphFileName, GraphType::GT_Live, TRUE);
  }

  m_Text.LoadString( IDS_MEASUREMENT_READY );
  SetDlgItemText( IDC_INFO_TEXT, m_Text );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
  m_pParent->m_EnableMoveOfWizard = TRUE;

  for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    CartToVec( g_AlignerData.ACR[i], g_AlignerData.ACP[i], &(g_AlignerData.VecAmp[i]), &(g_AlignerData.VecArg[i]) );
  }

  GetDlgItem( IDC_SHOW_GROUP )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_SHOW_POLAR_GRAPH )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_SHOW );
  m_pParent->SetWizardButtons( PSWIZB_FINISH );
}

void CGyroPerformanceTestPage2::InitMeasureParam()
{
  //set inparameters for measure
  //this params already initiated:
  //m_pParent->m_Measure.m_InParam.pSource = NULL;
  //m_pParent->m_Measure.m_InParam.SourceStatus = ACQU_IDLE;
  DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			
    
  m_pParent->m_Measure.m_InParam.Break = FALSE;
  memcpy( m_pParent->m_Measure.m_InParam.ObjNo, g_AlignerData.ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
  m_pParent->m_Measure.m_InParam.RefObjNo = g_AlignerData.RefObjNo;
  m_pParent->m_Measure.m_InParam.Mode1 = Gyr;
  m_pParent->m_Measure.m_InParam.Tao = g_AlignerData.TaoGyro;
  m_pParent->m_Measure.m_InParam.FiRef = 0;
  memset( m_pParent->m_Measure.m_InParam.FiObj, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Measure.m_InParam.ZParCorr, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Measure.m_InParam.rotateBack, 0, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  m_pParent->m_Measure.m_InParam.CountObj = g_AlignerData.NoOfCorr;
  m_pParent->m_Measure.m_InParam.RangeX = g_AlignerData.RX;
  m_pParent->m_Measure.m_InParam.RangeY = g_AlignerData.RY;
  m_pParent->m_Measure.m_InParam.UseElev = FALSE; 
  m_pParent->m_Measure.m_InParam.Fs = m_pParent->m_pSource->GetFrequency();
	m_pParent->m_Measure.m_InParam.SperryActive = m_pParent->GetSperryActive();
  m_pParent->m_Measure.m_InParam.commonFlatTest = FALSE;

    
//     m_pParent -> m_pSource -> SetFrequency( m_pParent -> m_Measure.m_InParam.Fs ) ;
// 	m_pParent -> m_pSource -> Start() ;
}

void CGyroPerformanceTestPage2::EnableStartButton()
{
  m_Text.LoadString( IDS_START_MEASURE );
  SetDlgItemText( IDC_START_MEASURE, m_Text );
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
}

BOOL CGyroPerformanceTestPage2::CallMeasure( double *pRoll, double *pPitch )
{
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

void CGyroPerformanceTestPage2::OnBnClickedShowPolarGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );

    //init the graph
    m_pParent->m_pGraph->InitDefaultPolarGraph( g_AlignerData.NoOfCorr );

    //set data and labels
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
		double newVecArg;
		newVecArg = AdjustDegAngle( g_AlignerData.VecArg[i], m_pParent->m_pGraph->GetPolarRange0ToPlusMinus180(), 1 );
		m_pParent->m_pGraph->SetXYData( i, newVecArg, g_AlignerData.VecAmp[i] );
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

void CGyroPerformanceTestPage2::OnBnClickedShowResultTable()
{
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;    
    OnBnClickedShowPolarGraph(); 
    m_pParent->m_pResultTable->ShowReport( FALSE );
	m_pParent->m_deleteReport = TRUE;
}

void CGyroPerformanceTestPage2::OnBnClickedPrintGraph()
{
    m_pParent->m_pGraph->PrintLiveGraph();
}

void CGyroPerformanceTestPage2::OnBnClickedSaveGraph()
{
//	GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( FALSE );
//	m_pParent->m_GraphFileManager.IncludeToResultTable( TRUE, m_pParent->m_pGraph->m_LastSavedGraphFileName );
}