// $Id: SetupLiveDataGraphDlg.cpp,v 1.11 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SetupLiveDataGraphDlg.h"
#include "SetupAxesDlg.h"
#include "SetupAxesDlgA202.h"
#include "Filter.h"
#include "Util.h"
#include "Measure.h"
#include ".\setuplivedatagraphdlg.h"

BOOL CSetupLiveDataGraphDlg::m_StoreToFile = FALSE;

int CSetupLiveDataGraphDlg::m_Frequency = FREQUENCY_FOR_LIVE_DATA_GRAPH;
CString CSetupLiveDataGraphDlg::m_TextFileName = (CString)DEFAULT_LIVE_DATA_FILE_NAME + (CString)DEFAULT_LIVE_DATA_TEXT_FILE_EXT;
CString CSetupLiveDataGraphDlg::m_TextFileDir = DEFAULT_FILE_DIR;

// BOOL CSetupLiveDataGraphDlg::m_TestChY2A202[NUM_CHANNELS];
// BOOL CSetupLiveDataGraphDlg::m_TestCh[NUM_CHANNELS];
BOOL CSetupLiveDataGraphDlg::m_FirstTime = TRUE;

IMPLEMENT_DYNAMIC(CSetupLiveDataGraphDlg, CDialog)

CSetupLiveDataGraphDlg::CSetupLiveDataGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(theApp.IsAligner202Enabled()==TRUE ? CSetupLiveDataGraphDlg::IDD_A202 : CSetupLiveDataGraphDlg::IDD_A308, pParent)
	, m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
  if( m_FirstTime == TRUE )
  {
    m_FirstTime = FALSE;
//      memset( m_TestCh, FALSE, NUM_CHANNELS * sizeof( BOOL ) );
//      memset( m_TestChY2A202, FALSE, NUM_CHANNELS * sizeof( BOOL ) );
     m_NoOfChY2A202 = 0;
  }

    m_IDMap[1] = CtrlID(IDC_TEST_CH_1, IDC_TEST_CH_1_Y2, IDC_TEXT_CH_1);
    m_IDMap[2] = CtrlID(IDC_TEST_CH_2, IDC_TEST_CH_2_Y2, IDC_TEXT_CH_2);
    m_IDMap[3] = CtrlID(IDC_TEST_CH_3, IDC_TEST_CH_3_Y2, IDC_TEXT_CH_3);
    m_IDMap[4] = CtrlID(IDC_TEST_CH_4, IDC_TEST_CH_4_Y2, IDC_TEXT_CH_4);
    m_IDMap[5] = CtrlID(IDC_TEST_CH_5, IDC_TEST_CH_5_Y2, IDC_TEXT_CH_5);
    m_IDMap[6] = CtrlID(IDC_TEST_CH_6, IDC_TEST_CH_6_Y2, IDC_TEXT_CH_6);
    m_IDMap[7] = CtrlID(IDC_TEST_CH_7, IDC_TEST_CH_7_Y2, IDC_TEXT_CH_7);
    m_IDMap[8] = CtrlID(IDC_TEST_CH_8, IDC_TEST_CH_8_Y2, IDC_TEXT_CH_8);
    m_IDMap[9] = CtrlID(IDC_TEST_CH_9, IDC_TEST_CH_9_Y2, IDC_TEXT_CH_9);
    m_IDMap[10] = CtrlID(IDC_TEST_CH_10, IDC_TEST_CH_10_Y2, IDC_TEXT_CH_10);
    m_IDMap[11] = CtrlID(IDC_TEST_CH_11, IDC_TEST_CH_11_Y2, IDC_TEXT_CH_11);
    m_IDMap[12] = CtrlID(IDC_TEST_CH_12, IDC_TEST_CH_12_Y2, IDC_TEXT_CH_12);
}

CSetupLiveDataGraphDlg::~CSetupLiveDataGraphDlg()
{
}

void CSetupLiveDataGraphDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    if(theApp.IsAligner308Enabled())
    {
        DDX_Text(pDX, IDC_FILTER_TIME_CONSTANT, m_FilterTimeConstant);    
    }
    DDX_Control(pDX, IDC_DAU_FREQUENCY, m_FrequencyControl) ;
    DDX_Text(pDX, IDC_FILE_NAME, m_TextFileName);
}

void CSetupLiveDataGraphDlg::SetObjectData()
{
    int obj=1;
    map<int, LiveGraphInfo>::iterator iter;
    g_AlignerData.NoOfCorr=0;
    for(iter=g_AlignerData.liveGraphMap.begin();iter!=g_AlignerData.liveGraphMap.end();iter++)
    {
        if(iter->second.roll != Axis_None || iter->second.pitch != Axis_None || iter->second.heading != Axis_None)
        {
             g_AlignerData.ObjNo[obj++] = iter->second.id;
             g_AlignerData.NoOfCorr++;    
        }
    }

}

void CSetupLiveDataGraphDlg::InitFrequencyControl( void )
{
    
    m_FrequencyControl.InsertString(0,"1.91Hz");
    m_FrequencyControl.SetItemData(0, 3);
    m_FrequencyControl.InsertString(0,"7.63Hz");
    m_FrequencyControl.SetItemData(0, 2);
    m_FrequencyControl.InsertString(0,"30.52Hz");
    m_FrequencyControl.SetItemData(0, 1);
    m_FrequencyControl.InsertString(0,"122.07Hz");
    m_FrequencyControl.SetItemData(0, 0);
    
    int mode = DAU::GetDAU().m_comThr.GetSamplingMode();
    m_FrequencyControl.SetCurSel(mode);
}

void CSetupLiveDataGraphDlg::ShowSettingsForAxes()
{
	if(theApp.IsAligner202Enabled())
	{
		CString XAxisSetting, Y1AxisSetting, Y2AxisSetting;
		CSetupAxesDlgA202 setupAxesDlgA202;
		setupAxesDlgA202.GetSettingsForAxes( &XAxisSetting, &Y1AxisSetting, &Y2AxisSetting );
		SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
		SetDlgItemText( IDC_AXIS_Y_VALUE, Y1AxisSetting );
		SetDlgItemText( IDC_AXIS_Y2_VALUE, Y2AxisSetting );
	}
	else
	{
		CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
		CSetupAxesDlg setupAxesDlg( FALSE, TRUE );
		setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
		SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
		SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
        SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	}
}

void CSetupLiveDataGraphDlg::SetFilter( BOOL filterOn )
{
    m_FilterOn = filterOn;

    if( m_FilterOn == TRUE )
    { // 3:rd order filter
        CheckRadioButton( IDC_FILTER_ON, IDC_FILTER_OFF, IDC_FILTER_ON );
        GetDlgItem( IDC_FILTER_TIME_CONSTANT )->EnableWindow( TRUE );
		if( IsTauOK() == FALSE )
		{
			GetDlgItem( IDC_FILTER_TIME_CONSTANT )->SetFocus();
		}
    }
    else
    { // No filter
        CheckRadioButton( IDC_FILTER_ON, IDC_FILTER_OFF, IDC_FILTER_OFF );
        GetDlgItem( IDC_FILTER_TIME_CONSTANT )->EnableWindow( FALSE );
    }
}

void CSetupLiveDataGraphDlg::EraseUnused()
{
    vector<int> ids;
    map<int, LiveGraphInfo>::iterator iter;
    for(iter=g_AlignerData.liveGraphMap.begin();iter!=g_AlignerData.liveGraphMap.end();iter++)
    {
        if(iter->second.roll == Axis_None &&  iter->second.pitch == Axis_None && iter->second.heading == Axis_None)
        {
            ids.push_back(iter->first);
        }
    }
        
    for(int i=0;i<ids.size();i++)
    {
        g_AlignerData.liveGraphMap.erase(ids[i]);
    }
}

BOOL CSetupLiveDataGraphDlg::InitGraph()
{
    m_pGraph = static_cast<CGraphView *>(theApp.m_pGraphView);
    CString str;

    EraseUnused();

    if( theApp.IsAligner202Enabled())
    {        
        int numY1 = CountGraphs(Axis_Y1);
        int numY2 = CountGraphs(Axis_Y2);
        m_pGraph->InitDefaultLiveDataGraph( numY1, numY2);
        str.LoadString( IDS_Y1_DEG_UNIT );
        m_pGraph->SetAxisYUnit( &str );
        str.LoadString( IDS_Y2_DEG_UNIT );
        m_pGraph->SetAxisY2Unit( &str );

		m_pGraph->SetAxisYMinAutoScaleOn( FALSE );
		m_pGraph->SetAxisYMaxAutoScaleOn( FALSE );
		m_pGraph->SetAxisY2MinAutoScaleOn( FALSE );
		m_pGraph->SetAxisY2MaxAutoScaleOn( FALSE );
        CSetupAxesDlgA202 setupAxesDlgA202;
		m_pGraph->SetAxisYMin( setupAxesDlgA202.GetMinY1Deg() );
		m_pGraph->SetAxisYMax( setupAxesDlgA202.GetMaxY1Deg() );
		m_pGraph->SetAxisY2Min( setupAxesDlgA202.GetMinY2Deg() );
		m_pGraph->SetAxisY2Max( setupAxesDlgA202.GetMaxY2Deg() );
    }
    else
    {
        int numY1 = CountGraphs(Axis_Y1);
        m_pGraph->InitDefaultLiveDataGraph(numY1); //roll and pitch

        if( GetMRad() == TRUE )
        {
            str.LoadString( IDS_MRAD_UNIT );
        }
        else
        {
            str.LoadString( IDS_ARCMIN_UNIT );  
        }
        m_pGraph->SetAxisYUnit( &str );

	    if( g_AlignerData.RY == 0 )
	    {
		    m_pGraph->SetAxisYMinAutoScaleOn( TRUE );
		    m_pGraph->SetAxisYMaxAutoScaleOn( TRUE );
	    }
        else 
        {
		    m_pGraph->SetAxisYMinAutoScaleOn( FALSE );
		    m_pGraph->SetAxisYMaxAutoScaleOn( FALSE );
            CSetupAxesDlg setupAxesDlg;
            m_pGraph->SetAxisYMin( (double)setupAxesDlg.GetMinY() );
            m_pGraph->SetAxisYMax( (double)setupAxesDlg.GetMaxY() );
	    }
    }
 
    m_pGraph->SetSource( &(DAU::GetDAU()) );

    if( m_FilterOn == TRUE )
    { // 3:rd order filter
        ResetAllFilters( m_pGraph->GetSource(), DAU::GetDAU().GetPeriod(), TETA, m_FilterTimeConstant );
    }
    else
    { // No filter
        ResetAllFilters( m_pGraph->GetSource() );
    }
    m_pGraph ->ClearMeasuredLiveGraphValues( TRUE );

  //read the start temperatures, before the real start
//   m_pGraph -> GetSource() -> SetFrequency( 0x10 ) ; // 60 Hz
// 	m_pGraph -> GetSource() -> Start() ;
//     Sleep(200);
//     m_pGraph -> GetSource() -> Stop() ;
//     m_pGraph ->UpdateCurrentValues( TRUE );

    //start the real livegraph
  //DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);		  
//   m_pGraph -> GetSource() -> SetFrequency( m_Frequency ) ;
//     m_pGraph -> GetSource() -> Start() ;

    return( TRUE );
}

void CSetupLiveDataGraphDlg::SetTauLimits( void )
{
	//double samplePeriod = DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() );
    double samplePeriod = 1.0f / DAU::GetDAU().m_comThr.GetSamplingRate();
    
    m_minTau = TAU_FREQUENCY_FACTOR * samplePeriod;

	if( m_minTau < MIN_FILTER_TIME_CONSTANT_LIVE_DATA )
	{
		m_minTau = MIN_FILTER_TIME_CONSTANT_LIVE_DATA;
	}
	m_maxTau = MAX_FILTER_TIME_CONSTANT;
}

BOOL CSetupLiveDataGraphDlg::IsTauOK( void )
{
    if( theApp.IsAligner202Enabled() == FALSE )
    {
	    UpdateData( FALSE );

        if( m_FilterOn == FALSE )
        { // No filter
		    return( TRUE );
	    }

	    if( (m_FilterTimeConstant < m_minTau) || (m_FilterTimeConstant > m_maxTau) )
	    {
		    CString MsgCaption, MsgText, Max, Min;
            MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
            Max.Format( "%.2f", m_maxTau ) ;
		    Min.Format( "%.2f", m_minTau ) ;
		    AfxFormatString2( MsgText, IDS_TAU_OUT_OF_LIMITS_MUST_BE_BETWEEN_MIN_MAX, Min, Max );
		    MessageBox( MsgText, MsgCaption, MB_ICONINFORMATION|MB_OK );
		    return( FALSE );
	    }
    }
	return( TRUE );
}

BOOL CSetupLiveDataGraphDlg::IsFileNameOK( void )
{
	//Check if the file exists
	CFile File;

 	if( m_TextFileDir.Find(".") == 0 )
	{ // the string starts with "." 
        char ca[MAX_PATH];
		GetCurrentDirectory( sizeof(ca), ca );
		m_TextFileDir = ca;
        m_TextFileDir += _T("\\");
	}

	if( File.Open( m_TextFileDir + m_TextFileName, CFile::modeRead, NULL ) ) 
	{
		File.Close();
		m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
		AfxFormatString1( m_Text, IDS_FILE_EXISTS_REPLACE_IT, m_TextFileDir + m_TextFileName );

		if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDNO )
		{   
			return( FALSE );
		}
	}
    m_FileNameChecked = TRUE;
    return( TRUE );
}

BEGIN_MESSAGE_MAP(CSetupLiveDataGraphDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedShow)
    ON_BN_CLICKED(IDC_TEST_CH_1, OnBnClickedTestCh1Y1)
    ON_BN_CLICKED(IDC_TEST_CH_2, OnBnClickedTestCh2Y1)
    ON_BN_CLICKED(IDC_TEST_CH_3, OnBnClickedTestCh3Y1)
    ON_BN_CLICKED(IDC_TEST_CH_4, OnBnClickedTestCh4Y1)
    ON_BN_CLICKED(IDC_TEST_CH_5, OnBnClickedTestCh5Y1)
    ON_BN_CLICKED(IDC_TEST_CH_6, OnBnClickedTestCh6Y1)
    ON_BN_CLICKED(IDC_TEST_CH_7, OnBnClickedTestCh7Y1)
    ON_BN_CLICKED(IDC_TEST_CH_8, OnBnClickedTestCh8Y1)
    ON_BN_CLICKED(IDC_TEST_CH_9, OnBnClickedTestCh9Y1)
    ON_BN_CLICKED(IDC_TEST_CH_10, OnBnClickedTestCh10Y1)
    ON_BN_CLICKED(IDC_TEST_CH_11, OnBnClickedTestCh11Y1)
    ON_BN_CLICKED(IDC_TEST_CH_12, OnBnClickedTestCh12Y1)
    ON_BN_CLICKED(IDC_TEST_CH_1_Y2, OnBnClickedTestCh1Y2)
    ON_BN_CLICKED(IDC_TEST_CH_2_Y2, OnBnClickedTestCh2Y2)
    ON_BN_CLICKED(IDC_TEST_CH_3_Y2, OnBnClickedTestCh3Y2)
    ON_BN_CLICKED(IDC_TEST_CH_4_Y2, OnBnClickedTestCh4Y2)
    ON_BN_CLICKED(IDC_TEST_CH_5_Y2, OnBnClickedTestCh5Y2)
    ON_BN_CLICKED(IDC_TEST_CH_6_Y2, OnBnClickedTestCh6Y2)
    ON_BN_CLICKED(IDC_TEST_CH_7_Y2, OnBnClickedTestCh7Y2)
    ON_BN_CLICKED(IDC_TEST_CH_8_Y2, OnBnClickedTestCh8Y2)
    ON_BN_CLICKED(IDC_TEST_CH_9_Y2, OnBnClickedTestCh9Y2)
    ON_BN_CLICKED(IDC_TEST_CH_10_Y2, OnBnClickedTestCh10Y2)
    ON_BN_CLICKED(IDC_TEST_CH_11_Y2, OnBnClickedTestCh11Y2)
    ON_BN_CLICKED(IDC_TEST_CH_12_Y2, OnBnClickedTestCh12Y2)
    ON_BN_CLICKED(IDC_FILTER_ON, OnBnClickedFilterOn)
	ON_BN_CLICKED(IDC_FILTER_OFF, OnBnClickedFilterOff)
	ON_EN_KILLFOCUS(IDC_FILTER_TIME_CONSTANT, OnEnKillfocusFilterTimeConstant)
    ON_CBN_SELCHANGE(IDC_DAU_FREQUENCY, OnCbnSelchangeDauFrequency)
    ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCRBUTTONDOWN()
    ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
    ON_EN_KILLFOCUS(IDC_FILE_NAME, OnEnKillfocusFileName)
	ON_BN_CLICKED(IDC_DATA_STORAGE_ON, OnBnClickedDataStorageOn)
	ON_BN_CLICKED(IDC_DATA_STORAGE_OFF, OnBnClickedDataStorageOff)
    ON_BN_CLICKED(IDC_ZERO_TO_RANGE, OnBnClickedZeroToRange)
    ON_BN_CLICKED(IDC_PLUS_MINUS_RANGE, OnBnClickedPlusMinusRange)
END_MESSAGE_MAP()


void CSetupLiveDataGraphDlg::ShowRow(int id, int val)
{
    GetDlgItem(m_IDMap[id].textCh)->ShowWindow( val );
    GetDlgItem( m_IDMap[id].testChY1)->ShowWindow( val );
    if(theApp.IsAligner202Enabled())
    {
        GetDlgItem( m_IDMap[id].testChY2)->ShowWindow( val );
    }
}

void CSetupLiveDataGraphDlg::EnableRow(int id, BOOL val)
{
    GetDlgItem(m_IDMap[id].textCh)->EnableWindow( val );
    GetDlgItem( m_IDMap[id].testChY1)->EnableWindow( val );
    if(theApp.IsAligner202Enabled())
    {
        GetDlgItem( m_IDMap[id].testChY2)->EnableWindow( val );
    }
}

// CSetupLiveDataGraphDlg message handlers

BOOL CSetupLiveDataGraphDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_FilterTimeConstant = 10;
    m_FileNameChecked = FALSE;
    //StoreOverflowSettings();

    if( theApp.IsAligner202Enabled() == TRUE )
    {
        if( theApp.IsRangeZeroToRangeA202() == TRUE )
        {
            OnBnClickedZeroToRange();
        }
        else
        {
            OnBnClickedPlusMinusRange();
        }
        m_FilterOn = FALSE;    
        GetDlgItem( IDC_AXIS_Y2_VALUE )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_AXIS_Y2 )->ShowWindow( SW_SHOW );
    }
    else
    {
        GetDlgItem( IDC_FILTER_BOUNDARY )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_FILTER_ON )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_FILTER_OFF )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_FILTER_TIME_CONSTANT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_FILTER_TIME_CONSTANT_TEXT )->ShowWindow( SW_SHOW );
        SetFilter( FALSE );
    }

    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);
    InitFrequencyControl();
    ShowSettingsForAxes();
    m_ShowGraphPressed = FALSE;
	SetTauLimits();

	if( m_StoreToFile == FALSE ) 
	{
		CheckRadioButton( IDC_DATA_STORAGE_ON, IDC_DATA_STORAGE_OFF, IDC_DATA_STORAGE_OFF );
		GetDlgItem( IDC_FILE_NAME )->EnableWindow( FALSE );
		GetDlgItem( IDC_BROWSE )->EnableWindow( FALSE );
	}
	else
	{
		CheckRadioButton( IDC_DATA_STORAGE_ON, IDC_DATA_STORAGE_OFF, IDC_DATA_STORAGE_ON );
		GetDlgItem( IDC_FILE_NAME )->EnableWindow( TRUE );
		GetDlgItem( IDC_BROWSE )->EnableWindow( TRUE );
	}


    g_AlignerData.liveGraphMap.clear();
    if(theApp.IsAligner308Enabled())
    {
        for( int ch=FIRST_ARRAY_INDEX_SYNCRO; ch<=LAST_ARRAY_INDEX_SENSOR; ch++ )
        { 	
            BOOL connected = IsConnected(ch);

            CString desc = GetUnitTypeDescription(ch).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            CString chName = GetChannelName(ch);

            if(connected)
            {
                g_AlignerData.liveGraphMap[ch] = LiveGraphInfo(chName, ch);
            }

            m_ChText[ch].Format("%s - %s",chName, desc);

            GetDlgItem(m_IDMap[ch].textCh)->ShowWindow( SW_SHOW );
            GetDlgItem(m_IDMap[ch].testChY1)->ShowWindow( SW_SHOW );

            //check which channels are connected
            if(IsConnected(ch))
            {
                //Enable text and box for the connected channels      
                GetDlgItem( m_IDMap[ch].textCh )->EnableWindow( TRUE );
                GetDlgItem( m_IDMap[ch].testChY1 )->EnableWindow( TRUE );
                m_graphMap308[m_IDMap[ch].testChY1] = &g_AlignerData.liveGraphMap[ch];                
            }
            SetDlgItemText( m_IDMap[ch].textCh, m_ChText[ch] );  
        }
    }
    else //Aligner 202
    {
        int id = 1;
        for( int ch=FIRST_ARRAY_INDEX_SYNCRO; ch<=LAST_ARRAY_INDEX_SYNCRO; ch++ )
        { 	                                  
            BOOL connected = IsConnected(ch);
            //check which channels are connected
            ShowRow(id, SW_SHOW);
            EnableRow(id, connected);                        

            CString desc = GetUnitTypeDescription(ch).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            CString chName = GetChannelName(ch);
            CString typeName = GetTypeName(ch);

            if(connected)
            {
                g_AlignerData.liveGraphMap[ch] = LiveGraphInfo(chName, ch, typeName);
            }

            Digital* pDig = GetDigital(GetChannelName(ch));
            if(pDig)
            {
                g_AlignerData.liveGraphMap[ch].dig = TRUE;
                //Show roll, pitch, heading
                m_ChText[id].Format("%s - Roll - %s ",chName, desc);
                SetDlgItemText( m_IDMap[id].textCh, m_ChText[id]);  
                if(connected)
                {
                    m_graphMap202[m_IDMap[id].testChY1] = &g_AlignerData.liveGraphMap[ch].roll;
                    m_graphMap202[m_IDMap[id].testChY2] = &g_AlignerData.liveGraphMap[ch].roll;
                }

                id++;
                ShowRow(id, SW_SHOW);
                EnableRow(id, IsConnected(ch));
                m_ChText[id].Format("%s - Pitch - %s",chName, desc);
                SetDlgItemText( m_IDMap[id].textCh, m_ChText[id]);  
                if(connected)
                {
                    m_graphMap202[m_IDMap[id].testChY1] = &g_AlignerData.liveGraphMap[ch].pitch;
                    m_graphMap202[m_IDMap[id].testChY2] = &g_AlignerData.liveGraphMap[ch].pitch;
                }

                id++;
                ShowRow(id, SW_SHOW);
                EnableRow(id, IsConnected(ch));
                m_ChText[id].Format("%s - Heading - %s",chName, desc);
                SetDlgItemText( m_IDMap[id].textCh, m_ChText[id]);  
                if(connected)
                {
                    m_graphMap202[m_IDMap[id].testChY1] = &g_AlignerData.liveGraphMap[ch].heading;
                    m_graphMap202[m_IDMap[id].testChY2] = &g_AlignerData.liveGraphMap[ch].heading;
                }
            }
            else
            {
                //Enable text and box for the connected channels                      
                m_ChText[id].Format("%s - %s",chName, desc);
                SetDlgItemText( m_IDMap[id].textCh, m_ChText[id] );                                  
                if(connected)
                {
                    m_graphMap202[m_IDMap[id].testChY1] = &g_AlignerData.liveGraphMap[ch].roll;
                    m_graphMap202[m_IDMap[id].testChY2] = &g_AlignerData.liveGraphMap[ch].roll;
                }
            }            
            id++;
        }
    }
    //DumpGraphInfo();
    UpdateData( FALSE );

    return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupLiveDataGraphDlg::OnBnClickedShow()
{
    if( theApp.IsAligner202Enabled() == FALSE )
    {
	    if( IsTauOK() == FALSE )
	    {
		    GetDlgItem( IDC_FILTER_TIME_CONSTANT )->SetFocus();
		    return;
	    }
    }
	m_pGraph->HideGraph();

    m_NoOfCorr = CountGraphs(Axis_Y1);

    if( m_NoOfCorr == 0 )
    {
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            m_Text.LoadString( IDS_SETUP_MEASURE_SETUP_ONE_TO_SHOW_ON_Y1 );
        }
        else
        {
            m_Text.LoadString( IDS_SETUP_MEASURE_SETUP_ONE_TO_SHOW );
        }
        m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        return;
    }

	if( ( m_StoreToFile == TRUE ) && ( m_FileNameChecked == FALSE ) )
	{
        if( IsFileNameOK() == FALSE )
        {
            return;
        }
	}

//  SetObjectData();
  //store the channels to be tested
//   g_AlignerData.NoOfCorr = m_NoOfCorr;
//   memcpy( g_AlignerData.ObjNo, m_ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
    //DAU::GetDAU().InitDataCompensator();        


    if( InitGraph() == FALSE )
    {
        //TODO error message
        return;
    }

    m_ShowGraphPressed = TRUE;
    //MoveDlgToRightSideOfTheGraph( this );

	double Ts;

    double freq = DAU::GetDAU().GetFrequency();
    FreqToTs( freq, &Ts );
      
    long NStop = RoundToLong( g_AlignerData.RX / Ts );
    long maxNoOfSamplesOnSerie;
      
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        maxNoOfSamplesOnSerie =  MAX_NO_OF_SAMPLES_IN_GRAPH / m_NoOfCorr;
    }
    else
    {
        maxNoOfSamplesOnSerie =  MAX_NO_OF_SAMPLES_IN_GRAPH / m_NoOfCorr; // roll and pitch
    }
	m_pGraph->SetReductionFactorForGraphData( NStop, maxNoOfSamplesOnSerie );
	m_pGraph->SetAxisXUnitAndMax( (double)g_AlignerData.RX );

	//Get the timestamp to be stored in the graph and table
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_pGraph->m_LiveGraphStartTimeStamp );
	m_pGraph->m_LiveDataGraphParam.TimeStamp = GetTimeStampStr( &(m_pGraph->m_LiveGraphStartTimeStamp), TRUE );
    m_pGraph->m_LiveDataGraphParam.DateStamp = GetDateStampStr( &(m_pGraph->m_LiveGraphStartTimeStamp) );
	m_pGraph->m_LiveDataGraphParam.StoreToFile = m_StoreToFile;
	m_pGraph->m_LiveDataGraphParam.CounterStopValue = NStop;
	m_pGraph->m_LiveDataGraphParam.FileName = m_TextFileDir + m_TextFileName;
	m_pGraph->m_LiveDataGraphParam.ContinousGraph = g_AlignerData.ContinousGraph;

	if( m_FilterOn == TRUE )
	{
		m_pGraph->m_LiveDataGraphParam.TimeConstant = m_FilterTimeConstant;
	}
	else
	{
		m_pGraph->m_LiveDataGraphParam.TimeConstant = 0;
	}
	m_pGraph->m_LiveDataGraphParam.Ts = Ts;
    m_pGraph->m_LiveDataGraphParam.initGraph = TRUE;
    m_pGraph->m_LiveDataGraphParam.startLiveData = TRUE;
    m_pGraph->m_LiveGraphFileManager.Reset();
	m_pGraph->ShowLiveDataGraphWithText( &(m_pGraph->m_LiveDataGraphParam) );

    if( m_pGraph->GetSource() != NULL )
    {
        SetSelectedChannels( m_pGraph->GetSource(), TRUE );
        m_pGraph->GetSource()->EnableErrorEvent( TRUE );
    }
    else
    {
        SetSelectedChannels(NULL, TRUE );
        DAU::GetDAU().EnableErrorEvent( TRUE );
    }

	OnOK();
}

void CSetupLiveDataGraphDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CSetupLiveDataGraphDlg::DumpGraphInfo()
{
    map<int, LiveGraphInfo>::iterator iter;
    for(iter=g_AlignerData.liveGraphMap.begin();iter!=g_AlignerData.liveGraphMap.end();iter++)
    {
        TRACE("%s, %d, %d, %d\n",iter->second.name,iter->second.roll, iter->second.pitch, iter->second.heading);        
    }

}

int CSetupLiveDataGraphDlg::CountGraphs(int axis)
{   
    int count=0;
    map<int, LiveGraphInfo>::iterator iter;
    for(iter=g_AlignerData.liveGraphMap.begin();iter!=g_AlignerData.liveGraphMap.end();iter++)
    {
        if(iter->second.roll == axis)
            count++;
        
        if(iter->second.pitch == axis)
            count++;

        if(iter->second.heading == axis)
            count++;
    }
    return count;
}


void CSetupLiveDataGraphDlg::OnChannelClicked(int id, int opId, int axis)
{
    if(theApp.IsAligner308Enabled())
    {
        OnChannelClicked308(id);
    }
    else
    {
        OnChannelClicked202(id, opId, axis);
    }
}

void CSetupLiveDataGraphDlg::OnChannelClicked202(int id, int opId, int axis)
{    
    if(m_graphMap202.find(id) == m_graphMap202.end())
        return;

    CButton* pButt = (CButton*)GetDlgItem(opId);
    pButt->SetCheck(FALSE);

    pButt = (CButton*)GetDlgItem(id);
    *m_graphMap202[id] = pButt->GetCheck() ? axis : Axis_None ;

//    DumpGraphInfo();
}

void CSetupLiveDataGraphDlg::OnChannelClicked308(int id)
{    
    if(m_graphMap308.find(id) == m_graphMap308.end())
        return;
    
    CButton* pButt = (CButton*)GetDlgItem(id);
    m_graphMap308[id]->roll = pButt->GetCheck() ? Axis_Y1 : Axis_None ;
    m_graphMap308[id]->pitch = pButt->GetCheck() ? Axis_Y1 : Axis_None ;

//    DumpGraphInfo();
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh1Y1()
{
    OnChannelClicked(IDC_TEST_CH_1, IDC_TEST_CH_1_Y2, Axis_Y1);    
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh2Y1()
{
    OnChannelClicked(IDC_TEST_CH_2, IDC_TEST_CH_2_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh3Y1()
{
    OnChannelClicked(IDC_TEST_CH_3, IDC_TEST_CH_3_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh4Y1()
{
    OnChannelClicked(IDC_TEST_CH_4, IDC_TEST_CH_4_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh5Y1()
{
    OnChannelClicked(IDC_TEST_CH_5, IDC_TEST_CH_5_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh6Y1()
{
    OnChannelClicked(IDC_TEST_CH_6, IDC_TEST_CH_6_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh7Y1()
{
    OnChannelClicked(IDC_TEST_CH_7, IDC_TEST_CH_7_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh8Y1()
{
    OnChannelClicked(IDC_TEST_CH_8, IDC_TEST_CH_8_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh9Y1()
{
    OnChannelClicked(IDC_TEST_CH_9, IDC_TEST_CH_9_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh10Y1()
{
    OnChannelClicked(IDC_TEST_CH_10, IDC_TEST_CH_10_Y2, Axis_Y1);
}
void CSetupLiveDataGraphDlg::OnBnClickedTestCh11Y1()
{
    OnChannelClicked(IDC_TEST_CH_11, IDC_TEST_CH_11_Y2, Axis_Y1);
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh12Y1()
{
    OnChannelClicked(IDC_TEST_CH_12, IDC_TEST_CH_12_Y2, Axis_Y1);
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh1Y2()
{
    OnChannelClicked202(IDC_TEST_CH_1_Y2, IDC_TEST_CH_1, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh2Y2()
{
    OnChannelClicked202(IDC_TEST_CH_2_Y2, IDC_TEST_CH_2, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh3Y2()
{
    OnChannelClicked202(IDC_TEST_CH_3_Y2, IDC_TEST_CH_3, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh4Y2()
{
    OnChannelClicked202(IDC_TEST_CH_4_Y2, IDC_TEST_CH_4, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh5Y2()
{
    OnChannelClicked202(IDC_TEST_CH_5_Y2, IDC_TEST_CH_5, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh6Y2()
{
    OnChannelClicked202(IDC_TEST_CH_6_Y2, IDC_TEST_CH_6, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh7Y2()
{
    OnChannelClicked202(IDC_TEST_CH_7_Y2, IDC_TEST_CH_7, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh8Y2()
{
    OnChannelClicked202(IDC_TEST_CH_8_Y2, IDC_TEST_CH_8, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh9Y2()
{
    OnChannelClicked202(IDC_TEST_CH_9_Y2, IDC_TEST_CH_9, Axis_Y2);   
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh10Y2()
{
    OnChannelClicked202(IDC_TEST_CH_10_Y2, IDC_TEST_CH_10, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh11Y2()
{
    OnChannelClicked202(IDC_TEST_CH_11_Y2, IDC_TEST_CH_11, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::OnBnClickedTestCh12Y2()
{
    OnChannelClicked202(IDC_TEST_CH_12_Y2, IDC_TEST_CH_12, Axis_Y2);  
}

void CSetupLiveDataGraphDlg::ClickedTestCh()
{
  UpdateData( FALSE );

  if( m_ShowGraphPressed == TRUE )
  {
    OnBnClickedShow();
  }
}

void CSetupLiveDataGraphDlg::OnBnClickedFilterOn()
{
  SetFilter( TRUE );
}

void CSetupLiveDataGraphDlg::OnBnClickedFilterOff()
{
  SetFilter( FALSE );
}

void CSetupLiveDataGraphDlg::OnEnKillfocusFilterTimeConstant()
{
  UpdateData( TRUE );
	if( IsTauOK() == FALSE )
	{
		GetDlgItem( IDC_FILTER_TIME_CONSTANT )->SetFocus();
	}
}

void CSetupLiveDataGraphDlg::OnCbnSelchangeDauFrequency( void )
{
   // m_Frequency = m_FrequencyControl.GetCurSel() + HIGHEST_FREQUENCY_INDEX;
    
    int rate = m_FrequencyControl.GetItemData(m_FrequencyControl.GetCurSel());   
    DAU::GetDAU().m_comThr.SetSamplingMode(rate);    
    TRACE("freq:%.2f, %.2f",m_dataFreq, DAU::GetDAU().m_comThr.GetSamplingRate());
//   DAU::GetDAU().SetFrequency( m_Frequency ) ;
// 	DAU::GetDAU().Start() ;
	SetTauLimits();
	IsTauOK();
}

void CSetupLiveDataGraphDlg::OnBnClickedSetupAxes()
{
  if( theApp.IsAligner202Enabled() == TRUE )
  {
    CSetupAxesDlgA202 setupAxesDlgA202;
    setupAxesDlgA202.DoModal();
  }
  else
  {
    CSetupAxesDlg setupAxesDlg( FALSE, TRUE );
    if( ( setupAxesDlg.DoModal() == IDOK ) && ( m_ShowGraphPressed == TRUE ) )
	  {
		  m_pGraph->HideGraph();

		  if( g_AlignerData.RY == 0 )
		  {
			  m_pGraph->SetAxisYMinAutoScaleOn( TRUE );
			  m_pGraph->SetAxisYMaxAutoScaleOn( TRUE );
		  }
          else
		  {
			  m_pGraph->SetAxisYMinAutoScaleOn( FALSE );
			  m_pGraph->SetAxisYMaxAutoScaleOn( FALSE );
              CSetupAxesDlg setupAxesDlg;
		      m_pGraph->SetAxisYMin( (double)setupAxesDlg.GetMinY() );
		      m_pGraph->SetAxisYMax( (double)setupAxesDlg.GetMaxY() );
		  }
		  OnBnClickedShow();
	  }
  }
  ShowSettingsForAxes();
}

void CSetupLiveDataGraphDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  if( m_ShowGraphPressed == FALSE )
  {
    CDialog::OnNcLButtonDown(nHitTest, point);
  }
}

void CSetupLiveDataGraphDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
  if( m_ShowGraphPressed == FALSE )
  {
    CDialog::OnNcRButtonDown(nHitTest, point);
  }
}

void CSetupLiveDataGraphDlg::OnBnClickedBrowse()
{
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|";
	CFileDialog dlg(FALSE,_T("txt"),(LPCTSTR)m_TextFileName,OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT,szFilter);
  dlg.m_ofn.lpstrInitialDir = m_TextFileDir;

	if (dlg.DoModal() == IDOK)
	{
    CString oldFileDir = m_TextFileDir;
		m_TextFileDir = dlg.GetPathName();
    m_TextFileName = dlg.GetFileName();

    int iPathLength = m_TextFileDir.GetLength() - m_TextFileName.GetLength();
    if( iPathLength >= 0 ) 
    {
      m_TextFileDir.GetBufferSetLength( iPathLength );
    }
    else 
    {
      m_TextFileDir = oldFileDir;
    }
    m_FileNameChecked = TRUE;
	}  	

	UpdateData(FALSE);
}

void CSetupLiveDataGraphDlg::OnEnKillfocusFileName()
{
  UpdateData( TRUE );
  if( m_TextFileName.Find(".txt") == -1 )
  { // ".txt" not found
    m_TextFileName += ".txt";
    UpdateData(FALSE);
	}
  if( IsFileNameOK() == FALSE )
  {
    GetDlgItem( IDC_FILE_NAME )->SetFocus();
  }
}

void CSetupLiveDataGraphDlg::OnBnClickedDataStorageOn()
{
	m_StoreToFile = TRUE;
  CheckRadioButton( IDC_DATA_STORAGE_ON, IDC_DATA_STORAGE_OFF, IDC_DATA_STORAGE_ON );
	GetDlgItem( IDC_FILE_NAME )->EnableWindow( TRUE );
	GetDlgItem( IDC_BROWSE )->EnableWindow( TRUE );
}

void CSetupLiveDataGraphDlg::OnBnClickedDataStorageOff()
{
	m_StoreToFile = FALSE;
	CheckRadioButton( IDC_DATA_STORAGE_ON, IDC_DATA_STORAGE_OFF, IDC_DATA_STORAGE_OFF );
	GetDlgItem( IDC_FILE_NAME )->EnableWindow( FALSE );
	GetDlgItem( IDC_BROWSE )->EnableWindow( FALSE );
}

void CSetupLiveDataGraphDlg::OnBnClickedZeroToRange()
{
  theApp.SetRangeZeroToRangeA202( TRUE );
  CheckRadioButton( IDC_ZERO_TO_RANGE, IDC_PLUS_MINUS_RANGE, IDC_ZERO_TO_RANGE );
}

void CSetupLiveDataGraphDlg::OnBnClickedPlusMinusRange()
{
  theApp.SetRangeZeroToRangeA202( FALSE );
  CheckRadioButton( IDC_ZERO_TO_RANGE, IDC_PLUS_MINUS_RANGE, IDC_PLUS_MINUS_RANGE );
}

