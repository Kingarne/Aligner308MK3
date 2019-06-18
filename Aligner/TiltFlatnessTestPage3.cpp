// $Id: TiltFlatnessTestPage3.cpp,v 1.10 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltFlatnessTestWizard.h"
#include "Util.h"
#include "AlignerDoc.h"

IMPLEMENT_DYNAMIC(CTiltFlatnessTestPage3, CPropertyPage)

CTiltFlatnessTestPage3::CTiltFlatnessTestPage3()
	: CPropertyPage(CTiltFlatnessTestPage3::IDD)
  , m_AzimuthAngle(0)
{
}

CTiltFlatnessTestPage3::~CTiltFlatnessTestPage3()
{
}

void CTiltFlatnessTestPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE, m_AzimuthAngle);
  DDV_MinMaxDouble(pDX, m_AzimuthAngle, 0, 360);
}

void CTiltFlatnessTestPage3::ShowGraphButtons()
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

void CTiltFlatnessTestPage3::HideGraphButtons()
{
  GetDlgItem( IDC_GRAPH_BOUNDARY )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_PRINT_GRAPH )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
}

void CTiltFlatnessTestPage3::DisableAllButtons()
{
  GetDlgItem( IDC_PRINT_GRAPH )->EnableWindow( FALSE );
  GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( FALSE );
	GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
	GetDlgItem( IDC_SHOW_RESULT_GRAPH )->EnableWindow( FALSE );
	GetDlgItem( IDC_SHOW_ERROR_GRAPH )->EnableWindow( FALSE );
	GetDlgItem( IDC_SHOW_POLAR_GRAPH )->EnableWindow( FALSE );
	GetDlgItem( IDC_SHOW_RESULT_TABLE )->EnableWindow( FALSE );
	GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->EnableWindow( FALSE );
}

void CTiltFlatnessTestPage3::InitResultTable( void )
{
  m_pParent->m_pResultTable->m_InParam.Version = TILT_FLATNESS_TEST;
	//m_pParent->m_pResultTable->m_InParam.Title.LoadString( IDS_TILT_FLATNESS_FOUNDATION_TEST );
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

BOOL CTiltFlatnessTestPage3::IsAlreadyMeasured( double newAngle )
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

BEGIN_MESSAGE_MAP(CTiltFlatnessTestPage3, CPropertyPage)
  ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)
  ON_EN_KILLFOCUS(IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE, OnEnKillfocusTiltFlatnessTestPage3AzimuthAngle)
  ON_BN_CLICKED(IDC_SHOW_POLAR_GRAPH, OnBnClickedShowPolarGraph)
  ON_BN_CLICKED(IDC_SHOW_ERROR_GRAPH, OnBnClickedShowErrorGraph)
  ON_BN_CLICKED(IDC_SHOW_RESULT_GRAPH, OnBnClickedShowResultGraph)
  ON_BN_CLICKED(IDC_SHOW_RESULT_TABLE, OnBnClickedShowResultTable)
  ON_BN_CLICKED(IDC_PRINT_GRAPH, OnBnClickedPrintGraph)
  ON_BN_CLICKED(IDC_SAVE_GRAPH, OnBnClickedSaveGraph)
	ON_BN_CLICKED(IDC_FINISH_MEASURE, &CTiltFlatnessTestPage3::OnBnClickedFinishMeasure)
END_MESSAGE_MAP()


// CTiltFlatnessTestPage3 message handlers

BOOL CTiltFlatnessTestPage3::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CTiltFlatnessTestWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltFlatnessTestPage3::OnSetActive() 
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
	m_Repeat = FALSE;
  MeasureRollPathInit();

  m_pParent->m_Measure.m_InParam.Break = FALSE;
  GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_HIDE);

  UpdateData( FALSE );

  return CPropertyPage::OnSetActive();
}

BOOL CTiltFlatnessTestPage3::OnQueryCancel()
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
			//HideAllText();
			m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
			m_pParent->m_Measure.m_InParam.Break = TRUE; 
		}
		return( FALSE );
	}
}

LRESULT CTiltFlatnessTestPage3::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

BOOL CTiltFlatnessTestPage3::OnWizardFinish()
{
	switch( m_pParent->m_Status )
	{
		case STATUS_PAGE_READY:
		m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
		m_Text.LoadString( IDS_EXIT_WITH_OPPORTUNITY_TO_SAVE );

		if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
		{
			
			if( m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS )
			{
				OnBnClickedShowErrorGraph();
			}			
			OnBnClickedShowResultGraph();
		    OnBnClickedShowPolarGraph();    
			m_pParent->m_GraphFileManager.MoveUnwantedToTemporaryDir();  

			m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
			m_pParent->m_pResultTable->ShowReport( TRUE );
            m_pParent->m_deleteReport = TRUE;
			m_Text.LoadString( IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD );

			if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
			{
				DisableAllButtons();
				CString graphFileName;
				m_pParent->ExitResultTable( FALSE);
				OnBnClickedShowPolarGraph();				

				if( m_pParent->m_PolarGraphFileName.GetLength() != 0 )
				{
					m_Text.LoadString( IDS_SAVE_THE_GRAPH_TO_THE_LOG_RECORD );
					BOOL keepPolar = (MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1) == IDYES);
					m_pParent->m_GraphFileManager.IncludeToResultTable( keepPolar, m_pParent->m_PolarGraphFileName );
					
				}
				OnBnClickedShowResultGraph();
				//m_pParent->SaveResultGraphFile();

				if( m_pParent->m_ResultGraphFileName.GetLength() != 0 )
				{
					m_Text.LoadString( IDS_SAVE_THE_GRAPH_TO_THE_LOG_RECORD );
					BOOL keepResult = (MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1) == IDYES);
					m_pParent->m_GraphFileManager.IncludeToResultTable( keepResult, m_pParent->m_ResultGraphFileName );					
				}

				if( m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS )
				{
					OnBnClickedShowErrorGraph();
					//m_pParent->SaveErrorGraphFile();

					if( m_pParent->m_ErrorGraphFileName.GetLength() != 0 )
					{
						m_Text.LoadString( IDS_SAVE_THE_GRAPH_TO_THE_LOG_RECORD );
						BOOL keepError = (MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1) == IDYES);
						m_pParent->m_GraphFileManager.IncludeToResultTable( keepError, m_pParent->m_ErrorGraphFileName );						
					}
				}				
                m_pParent->m_GraphFileManager.MoveUnwantedToTemporaryDir();  
				return CPropertyPage::OnWizardFinish();
			}
			else
			{
				m_pParent->m_pResultTable->CloseReport();
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

void CTiltFlatnessTestPage3::OnReset()
{
	m_pParent->ExitResultTable( m_pParent->m_deleteReport, TRUE );
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
    CAlignerDoc *pDoc = static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(theApp.m_pMainWnd) -> GetActiveDocument());
    if( pDoc != NULL )
    {
        pDoc->SaveProject();
    }
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltFlatnessTestPage3::MeasureRollPathInit()
{
  CString tmpStr;
  HideAllText();

	if( m_Repeat == FALSE )
	{
		m_pParent->m_N++;
	}
  tmpStr.Format( _T("%d"), m_pParent->m_N );
  AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_NEXT_MEAS_NO, tmpStr );
  SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, m_Text );
  GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );

  if( g_AlignerData.Found == TRUE )
  { //TODO g_AlignerData.Found == TRUE
  }
  else
  {
    m_Text.LoadString( IDS_TILT_FLATNESS_TEST_TRAIN_PLATFORMS );
    SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );

    m_Text.LoadString( IDS_TILT_FLATNESS_TEST_ENTER_ANGLE );
    SetDlgItemText( IDC_ENTER_ANGLE_TEXT, m_Text );
    GetDlgItem( IDC_ENTER_ANGLE_TEXT )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->EnableWindow( TRUE );
    GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->ShowWindow( SW_SHOW );
  }

  EnableStartButton();
}

void CTiltFlatnessTestPage3::EnableStartButton()
{
  m_Text.LoadString( IDS_START_MEASURE );
  SetDlgItemText( IDC_START_MEASURE, m_Text );
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
  GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );
}

void CTiltFlatnessTestPage3::OnBnClickedStartMeasure()
{
    CString tmpStr, tmpStr1;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    m_Text.LoadString( IDS_BREAK );
    GetDlgItemText( IDC_START_MEASURE, tmpStr );

    if( tmpStr == m_Text )
    {
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
        m_pParent->m_Measure.m_InParam.Break = TRUE; 
        return;
    }

    if( ( m_AzimuthAngle < 0 ) || ( m_AzimuthAngle > 360 ) )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        tmpStr1.LoadString( IDS_INVALID_AZIMUTH_ANGLE );
        tmpStr.LoadString( IDS_TILT_FLATNESS_TEST_ENTER_ANGLE );
        MessageBox( tmpStr1 + tmpStr, m_MsgCaption, MB_ICONERROR|MB_OK );    
        return;
    }

	if( IsAlreadyMeasured( m_AzimuthAngle ) == TRUE )
	{
		m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
		tmpStr.Format( _T("%.1f"), m_AzimuthAngle );
		AfxFormatString1( tmpStr1, IDS_ANGLE_ALREADY_MEASURED_AGAIN, tmpStr );

		if( IDNO == MessageBox( tmpStr1, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) )
		{
			return;
		}
	}

    MoveDlgToRightSideOfApp(m_pParent );
    //MoveDlgToRightSideOfTheGraph( m_pParent );
    HideGraphButtons();
    GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->EnableWindow( FALSE );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
    SetDlgItemText( IDC_START_MEASURE, m_Text );
    m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = FALSE;

    tmpStr.Format( _T("%d"), m_pParent->m_N );
	AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_MEAS_NO, tmpStr );
    SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, m_Text );
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );

    m_pParent->m_XAngle[m_pParent->m_N] = m_AzimuthAngle;
    g_AlignerData.Bear = m_AzimuthAngle;
    m_pParent->m_Xn = DEGREES_TO_RADIANS( m_AzimuthAngle );

	if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
    {
        m_pParent->m_Measure.m_InParam.FiRef = 0;    
    }
    else
    {
        m_pParent->m_Measure.m_InParam.FiRef = m_pParent->m_Xn;
        
    }
    m_pParent->m_Measure.m_InParam.rotateBackRef = m_pParent->m_Xn;

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
	    if( IsFixed( g_AlignerData.ObjNo[i] ) == TRUE )
        {
            m_pParent->m_Measure.m_InParam.FiObj[i] = 0;            
        }
        else
        {
            m_pParent->m_Measure.m_InParam.FiObj[i] = m_pParent->m_Xn;            
        }
        m_pParent->m_Measure.m_InParam.rotateBack[i] = m_pParent->m_Xn;
    }

    if( CallMeasure( m_pParent->m_dR, m_pParent->m_dP ) == FALSE )
    {
		if( m_Repeat == TRUE )
		{
			MeasureRollPathInit();
		}
		else
		{
			RestoreGlobals();
			GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

			m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED );
			SetDlgItemText( IDC_INFO_TEXT, m_Text );
			GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
			m_pParent->m_EnableMoveOfWizard = TRUE;
		}
        return;
    }

    ShowGraphButtons();
    m_Text.LoadString( IDS_MEASUREMENT_READY );
    SetDlgItemText( IDC_INFO_TEXT, m_Text );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_SHOW );
    m_pParent->m_EnableMoveOfWizard = TRUE;

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_Y[i][m_pParent->m_N] = m_pParent->m_dP[i];
    }

    tmpStr.Format( _T("%d"), m_pParent->m_N );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_N_MEAS_COMPLETED, tmpStr );

    if( (m_pParent->m_N >= MIN_NO_OF_FLATNESS_MEASUREMENTS) && (m_pParent->m_N < MAX_NO_OF_FLATNESS_MEASUREMENTS) )
    {
		GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_SHOW);		
    }
    else if( m_pParent->m_N >= MAX_NO_OF_FLATNESS_MEASUREMENTS  )
    {
        m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
        tmpStr.LoadString( IDS_TILT_FLATNESS_TEST_NO_MORE_MEASUREMENTS );
        m_Text += tmpStr;
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        m_pParent->m_Leave = TRUE;
    }

    if( m_pParent->m_Leave == FALSE )
    {
        MeasureRollPathInit();
    }
    else
    {
        RestoreGlobals();
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        MeasureRollPathContinue();
    }
}


void CTiltFlatnessTestPage3::OnBnClickedFinishMeasure()
{
	m_pParent->m_N--;
	m_pParent->m_Leave = TRUE;
	RestoreGlobals();
	GetDlgItem(IDC_START_MEASURE)->EnableWindow(FALSE);
	MeasureRollPathContinue();
}


void CTiltFlatnessTestPage3::RestoreGlobals()
{
  g_AlignerData.UseElevError = m_pParent->m_Measure.m_InParam.UseElev;
}

BOOL CTiltFlatnessTestPage3::CallMeasure( double *pRoll, double *pPitch )
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
                if( m_pParent->m_pSource != NULL )
                {
                    m_pParent->m_pSource->Start();
                }
                else
                {
                    DAU::GetDAU().Start();
                }
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

void CTiltFlatnessTestPage3::MeasureRollPathContinue()
{
    m_pParent->m_Ymax = -100;
    m_pParent->m_Ymin = 100;

    //Zero error for ref.
    for( int j=1; j<=m_pParent->m_N; j++ )
    {
        m_pParent->m_SineFitError[0][j] = 0.0f;
    }
    
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        SineFit( m_pParent->m_XAngle, m_pParent->m_Y, i, m_pParent->m_N, &(g_AlignerData.ACR[i]), &(g_AlignerData.ACP[i]), &(m_pParent->m_MeanSineFit[i]) );
        CartToVec( g_AlignerData.ACR[i], g_AlignerData.ACP[i], &(g_AlignerData.VecAmp[i]), &(g_AlignerData.VecArg[i]) );
        m_pParent->m_Fi[i] = DEGREES_TO_RADIANS( g_AlignerData.VecArg[i] );
        for( int j=1; j<=m_pParent->m_N; j++ )
        {
            m_pParent->m_SineFitError[i][j] = m_pParent->m_Y[i][j] - ( m_pParent->m_MeanSineFit[i] + g_AlignerData.VecAmp[i] * cos( DEGREES_TO_RADIANS( m_pParent->m_XAngle[j] ) - m_pParent->m_Fi[i] ) );

        //       if( g_AlignerData.Found == TRUE )
        //       {
        // 				m_pParent->m_SineFitError[i][j] = ( m_pParent->m_SineFitError[i][j] * GetIndexArmLength(1) ) / 1000;
        //       }

            if( fabs( m_pParent->m_SineFitError[i][j] ) > fabs( m_pParent->m_MaxSineFitError[i] ) )
            {
                m_pParent->m_MaxSineFitError[i] = m_pParent->m_SineFitError[i][j];
                m_pParent->m_XAngleForMaxSineFitError[i] = m_pParent->m_XAngle[j];
            }
            m_pParent->m_Eave[i] += m_pParent->m_SineFitError[i][j];
            m_pParent->m_Esquare[i] += SQUARE( m_pParent->m_SineFitError[i][j] );
            if( fabs( m_pParent->m_MaxSineFitError[i]) > m_pParent->m_ERan )
            {
                m_pParent->m_ERan = fabs( m_pParent->m_MaxSineFitError[i] );
            }
            if( m_pParent->m_Y[i][j] > m_pParent->m_Ymax )
            {
                m_pParent->m_Ymax = m_pParent->m_Y[i][j];
            }
            if( m_pParent->m_Y[i][j] < m_pParent->m_Ymin )
            {
                m_pParent->m_Ymin = m_pParent->m_Y[i][j];
            }
        }
        g_AlignerData.Sigma[i] = sqrt( m_pParent->m_Esquare[i] / ( m_pParent->m_N - 1 ) );

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
    }

    if( m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS )
    {
        GetDlgItem( IDC_SHOW_ERROR_GRAPH )->EnableWindow( TRUE );
    }
    else
    {
        GetDlgItem( IDC_SHOW_ERROR_GRAPH )->EnableWindow( FALSE );
    }
    GetDlgItem( IDC_TILT_FLATNESS_TEST_SHOW_GROUP )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_SHOW_RESULT_GRAPH )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_SHOW_ERROR_GRAPH )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_SHOW_POLAR_GRAPH )->ShowWindow( SW_SHOW );
    m_pParent->SetWizardButtons( PSWIZB_FINISH );
}

void CTiltFlatnessTestPage3::OnEnKillfocusTiltFlatnessTestPage3AzimuthAngle()
{
  UpdateData( TRUE );
}

void CTiltFlatnessTestPage3::HideAllText()
{
  GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_ENTER_ANGLE_TEXT )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
}

void CTiltFlatnessTestPage3::HideAll()
{
  HideAllText();
  GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_TILT_FLATNESS_TEST_SHOW_GROUP )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_RESULT_GRAPH )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_RESULT_TABLE )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_ERROR_GRAPH )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_SHOW_POLAR_GRAPH )->ShowWindow( SW_HIDE );
}

	/***************************************************************************/
	/*													OnBnClickedShowResultGraph											 */
	/***************************************************************************/
void CTiltFlatnessTestPage3::OnBnClickedShowResultGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
  UnitModeT UMode;
  CString str;
  TiltFlatnessTestGraphInParam graphParams;
  graphParams.PlMode = Sine;
  graphParams.mode = RPath;
  graphParams.RefNo = g_AlignerData.RefObjNo;
  graphParams.timeConstant = m_pParent->m_Measure.m_InParam.Tao;
  graphParams.Points = m_pParent->m_N;
  graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
  graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
  
  m_pParent->m_pGraph->InitDefaultPlotGraph( 2 * g_AlignerData.NoOfCorr );
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
			/***  The code below replaces the out-commented code below  ***/
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
	/*													OnBnClickedShowErrorGraph											 */
	/***************************************************************************/
void CTiltFlatnessTestPage3::OnBnClickedShowErrorGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
  UnitModeT UMode;
  int YRan;
  CString str;
  TiltFlatnessTestGraphInParam graphParams;
  graphParams.PlMode = Linear;
  graphParams.mode = RPath;
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
    YRan = RoundToInt( 2000 * m_pParent->m_ERan + 1.5 );

    if( YRan > 2000 )
    {
      YRan = RoundToInt( YRan / 1000 +1.5 );

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
    YRan = RoundToInt( 120 * g_AlignerData.Kh * m_pParent->m_ERan + 1.5 );
    if( YRan > 500 )
    {
      YRan = RoundToInt( YRan / 60 + 1.5 );
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

void CTiltFlatnessTestPage3::OnBnClickedShowPolarGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );

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

void CTiltFlatnessTestPage3::OnBnClickedShowResultTable()
{
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
	if (m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS)
	{
		OnBnClickedShowErrorGraph();
	}
	OnBnClickedShowResultGraph();
    OnBnClickedShowPolarGraph(); 
    m_pParent->m_pResultTable->ShowReport( FALSE );
	m_pParent->m_deleteReport = TRUE;
}

void CTiltFlatnessTestPage3::OnBnClickedPrintGraph()
{
  m_pParent->m_pGraph->PrintLiveGraph();
}

void CTiltFlatnessTestPage3::OnBnClickedSaveGraph()
{
	GetDlgItem( IDC_SAVE_GRAPH )->EnableWindow( FALSE );
	m_pParent->m_GraphFileManager.IncludeToResultTable( TRUE, m_pParent->m_pGraph->m_LastSavedGraphFileName );
}
