// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "DebugDlg.h"
#include ".\debugdlg.h"
#include "type.h"
#include <sstream>
#include "SystemConfigurationView.h"

// DebugDlg dialog

IMPLEMENT_DYNAMIC(DebugDlg, CDialog)
DebugDlg::DebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DebugDlg::IDD, pParent)
    , m_dauAcqOn(FALSE)
    , m_testData(0)
    , m_recTestData(_T(""))
    , m_recDACData(_T(""))
    , m_DACData(0)
    , m_DAUInfo(_T(""))
    , m_chAEnable(FALSE)
    , m_chBEnable(FALSE)
    , m_info(_T(""))
    /*, m_chInfo(_T(""))*/
    , m_logDACData(FALSE)
	, m_errorInfo(_T(""))
{
    m_timer = 0;
    m_lastCounters.resize(10);
}

DebugDlg::~DebugDlg()
{
}

void DebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEBUG_SENSOR_GRID, m_debugGrid);    
    DDX_Check(pDX, IDC_DAU_ACQ_ON, m_dauAcqOn);
	DDX_Control(pDX, IDC_FRAMES_LIST, m_framesList);
	DDX_Text(pDX, IDC_TEST_DATA, m_testData);
	DDX_Text(pDX, IDC_REC_TEST_DATA, m_recTestData);
	DDX_Text(pDX, IDC_REC_DAC_DATA, m_recDACData);
	DDX_Text(pDX, IDC_DAC_TEST, m_DACData);
	DDX_Text(pDX, IDC_DAU_STATIC, m_DAUInfo);
	DDX_Control(pDX, IDC_RATE_COMBO, m_rateCombo);
	DDX_Check(pDX, IDC_DAU_CH_A_ENABLE, m_chAEnable);
	DDX_Check(pDX, IDC_DAU_CH_B_ENABLE, m_chBEnable);
	DDX_Text(pDX, IDC_INFO, m_info);	
	DDX_Check(pDX, IDC_LOG_DAC_DATA, m_logDACData);
	DDX_Text(pDX, IDC_ERROR_COUNTS, m_errorInfo);
}


BEGIN_MESSAGE_MAP(DebugDlg, CDialog)
    ON_BN_CLICKED(IDC_DAU_ACQ_ON, OnBnClickedDauAcqOn)
    ON_BN_CLICKED(IDC_DAU_RESET_BUTT, OnBnClickedDauResetButt)
    ON_WM_SHOWWINDOW()
//    ON_WM_DESTROY()
    ON_WM_ACTIVATE()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_LOOP_TEST_BUTT, OnBnClickedLoopTestButt)
    ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
    ON_EN_CHANGE(IDC_TEST_DATA, OnEnChangeTestData)
    ON_BN_CLICKED(IDC_SEND_DAC_BUTT, OnBnClickedSendDacButt)
    ON_EN_CHANGE(IDC_DAC_TEST, OnEnChangeDacTest)
    ON_CBN_SELCHANGE(IDC_RATE_COMBO, OnCbnSelchangeRateCombo)
    ON_BN_CLICKED(IDC_DAC_TEST_BUTT, OnBnClickedDacTestButt)
    ON_BN_CLICKED(IDC_DAU_CH_A_ENABLE, OnBnClickedDauChAEnable)
    ON_BN_CLICKED(IDC_DAU_CH_B_ENABLE, OnBnClickedDauChBEnable)
    ON_BN_CLICKED(IDC_SIMUL_BUTTON, OnBnClickedSimulButton)
    ON_BN_CLICKED(IDC_SET_TO_FIX, OnBnClickedSetToFix)
    ON_BN_CLICKED(IDC_CLEAR_ALL, OnBnClickedClearAll)
    ON_BN_CLICKED(IDC_LOG_DAC_DATA, OnBnClickedLogDacData)
    ON_BN_CLICKED(IDC_RESTART_BUTT, OnBnClickedRestartButt)
    ON_EN_KILLFOCUS(IDC_DEBUG_FLAGS, OnEnKillfocusDebugFlags)
    ON_EN_KILLFOCUS(IDC_COMP_K, OnEnKillfocusK)   
    ON_BN_CLICKED(IDC_DEBUG_INFO_BUTTON, OnBnClickedDebugInfoButton)
    ON_BN_CLICKED(IDC_ENABLE_COMPENSATION, OnBnClickedEnableCompensation)
    ON_BN_CLICKED(IDC_SET_AZ_BUTTON, OnBnClickedSetAzButton)
	ON_BN_CLICKED(IDC_DUMP_SENSOR_STATES, &DebugDlg::OnBnClickedDumpSensorStates)
END_MESSAGE_MAP()


// DebugDlg message handlers

void DebugDlg::OnBnClickedDauAcqOn()
{
    UpdateData(TRUE);

    DAU::GetDAU().m_comThr.SetDAUAcqState(m_dauAcqOn);
    
    TRACE("Aqc %d\n",m_dauAcqOn);

}

void DebugDlg::InitControls()
{
    m_dauAcqOn = DAU::GetDAU().m_mainBoardRegsPro.m_dataAcqEnable;
    m_chAEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlA_Enable;
    m_chBEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlB_Enable;

    UpdateData(FALSE);
}

BOOL DebugDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitControls();
    
    m_debugGrid.Init();
    
    m_framesList.InsertColumn(0, "Type", LVCFMT_LEFT, 70); 
    m_framesList.InsertColumn(1, "Num", LVCFMT_LEFT, 50); 
    m_framesList.InsertColumn(2, "Hz", LVCFMT_LEFT, 50); 

    m_framesList.InsertItem(0, "Sensor");
    m_framesList.InsertItem(1, "HDLC A");
    m_framesList.InsertItem(2, "HDLC B");
    m_framesList.InsertItem(3, "UART A");
    m_framesList.InsertItem(4, "UART B");
    m_framesList.InsertItem(5, "FPGA Reg");

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CH_COMBO);   
    pCombo->AddString("J3");
    pCombo->AddString("J4");
    pCombo->AddString("J5");
    pCombo->AddString("J6");
    pCombo->AddString("J7");
    pCombo->AddString("J8");
    pCombo->AddString("J9");
    pCombo->SelectString(-1, "J3");

    QueryPerformanceFrequency(&m_freq);

    m_DAUInfo.Format("DAU,  S/N: %d,  prot:%d.%d",DAU::GetDAU().m_mainBoardRegsPro.m_dauSerialNo, DAU::GetDAU().m_mainBoardRegsPro.m_dauVersion/10,DAU::GetDAU().m_mainBoardRegsPro.m_dauVersion%10 );

    CString str;
    double rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_1220];    
    str.Format("0 (%.1fHz)",rate);
    m_rateCombo.AddString(str);
    
    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_305];
    str.Format("1 (%.1fHz)",rate);
    m_rateCombo.AddString(str);
    
    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_76];
    str.Format("2 (%.1fHz)",rate);
    m_rateCombo.AddString(str);
    
    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_19];
    str.Format("3 (%.1fHz)",rate);
    m_rateCombo.AddString(str);

    m_rateCombo.SetCurSel(0);
    
    m_logDACData = DAU::GetDAU().m_logDACData;

	CWnd* pWnd= GetDlgItem(IDC_DEBUG_FLAGS);
	str.Format("%d",g_AlignerData.debugFlag);
	pWnd->SetWindowText(str);	

    CButton* pButt = (CButton*)GetDlgItem(IDC_ENABLE_COMPENSATION);
    pButt->SetCheck(DAU::GetDAU().m_dataCompensator.GetEnable());
    str.Format("Enable Compensation (%d)",DAU::GetDAU().m_dataCompensator.GetActivate());
    pButt->SetWindowText(str);
	
    pWnd = GetDlgItem(IDC_COMP_K);
    str.Format("%d",(int)DAU::GetDAU().m_dataCompensator.m_rollFilter.GetK());
    pWnd->SetWindowText(str);
    
    char*p =new char[1];
    *p=1;

    //      // Set WS_EX_LAYERED on this window 
//      SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
//      //     // Make this window 70% alpha
//      int alpha = 70;    
//      SetLayeredWindowAttributes(0, (255 * alpha) / 100, LWA_ALPHA);

     return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void DebugDlg::OnBnClickedDauResetButt()
{
    DAU::GetDAU().m_comThr.DAUReset();
}

void DebugDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
    
    TRACE("bShow %d\n",bShow);
    if(bShow == 1)
    {
        if(m_timer == 0)
            m_timer = SetTimer(1, 1000, NULL);
    } 
}


void DebugDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CDialog::OnActivate(nState, pWndOther, bMinimized);

   
   InitControls();
    
    TRACE("state %d\n",nState);
}

void DebugDlg::UpdateCounters()
{
    vector<int>& counters = DAU::GetDAU().m_dauFramesCounter;
    

    static int count=0;
	static int lastTotalRead=0;
    CString str;
    
    LARGE_INTEGER systime;    
    QueryPerformanceCounter(&systime);

    LARGE_INTEGER diff;
    diff.QuadPart = (systime.QuadPart-m_lastUpdate.QuadPart);
    double sec = (double)diff.QuadPart / (double)m_freq.QuadPart;
    
	m_DAUInfo.Format("DAU,  S/N: %d,  prot:%d.%d",DAU::GetDAU().m_mainBoardRegsPro.m_dauSerialNo, DAU::GetDAU().m_mainBoardRegsPro.m_dauVersion/10,DAU::GetDAU().m_mainBoardRegsPro.m_dauVersion%10 );

    str.Format("%d",counters[FRAME_TYPE_SENSOR]);
    m_framesList.SetItemText(0,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_SENSOR]-m_lastCounters[FRAME_TYPE_SENSOR])*sec);
    m_framesList.SetItemText(0,2,str);

    str.Format("%d",counters[FRAME_TYPE_HDLC_A]);
    m_framesList.SetItemText(1,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_HDLC_A]-m_lastCounters[FRAME_TYPE_HDLC_A])*sec);
    m_framesList.SetItemText(1,2,str);

    str.Format("%d",counters[FRAME_TYPE_HDLC_B]);
    m_framesList.SetItemText(2,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_HDLC_B]-m_lastCounters[FRAME_TYPE_HDLC_B])*sec);
    m_framesList.SetItemText(2,2,str);

    str.Format("%d",counters[FRAME_TYPE_UART_A]);
    m_framesList.SetItemText(3,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_UART_A]-m_lastCounters[FRAME_TYPE_UART_A])*sec);
    m_framesList.SetItemText(3,2,str);

    str.Format("%d",counters[FRAME_TYPE_UART_B]);
    m_framesList.SetItemText(4,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_UART_B]-m_lastCounters[FRAME_TYPE_UART_B])*sec);
    m_framesList.SetItemText(4,2,str);

    str.Format("%d",counters[FRAME_TYPE_REG]);
    m_framesList.SetItemText(5,1,str);
    str.Format("%.2f",(counters[FRAME_TYPE_REG]-m_lastCounters[FRAME_TYPE_REG])*sec);
    m_framesList.SetItemText(5,2,str);

    QueryPerformanceCounter(&m_lastUpdate);    
    m_lastCounters = counters;
    
	//Update test byte    
    m_recTestData.Format("Received Test Data: %d",DAU::GetDAU().m_mainBoardRegsPro.m_testByte);
    //Update DAC value
    short dac = DAU::GetDAU().m_recDACVal;
    short filteredDac = DAU::GetDAU().GetFilteredMeasuredDACVoltage();
    stringstream ss;
    ss << std::hex << dac;    
    int recDac = DAU::GetDAU().m_recSetDACVal;
    recDac = recDac - 32768;
    int dacDiff = recDac - DAU::GetDAU().m_recDACVal;
    m_recDACData.Format("Received DAC Data: %d,%d (%s) (%d) (%d)",dac,filteredDac, ss.str().c_str(),DAU::GetDAU().m_recSetDACVal, dacDiff);

    m_rateCombo.SetCurSel(DAU::GetDAU().m_comThr.GetSamplingMode());
    
    CWnd* pWnd = GetDlgItem(IDC_DAU_CH_A_ENABLE);
    str.Format("Ch A Enable %s",DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlA_HdlcUartSel == 0 ? "(UART)" : "(HDLC)");    
    pWnd->SetWindowText(str);
    pWnd = GetDlgItem(IDC_DAU_CH_B_ENABLE);
    str.Format("Ch B Enable %s",DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlB_HdlcUartSel == 0 ? "(UART)" : "(HDLC)");    
    pWnd->SetWindowText(str);

	double totalReadPerSec= (DAU::GetDAU().m_comThr.m_totalBytesRead - lastTotalRead)*sec;
	lastTotalRead = DAU::GetDAU().m_comThr.m_totalBytesRead;
	m_info.Format("Error event active: %d\r\nOverrange Detection:%d\r\nfifo:%d\r\nrx:%d\r\ndata/s:%d tot:%d",DAU::GetDAU().IsErrorEventEnabled(),DAU::GetDAU().m_overangeDetectionActive, DAU::GetDAU().m_comThr.m_fifo.size(), DAU::GetDAU().m_comThr.m_rxBytes, (int)totalReadPerSec, DAU::GetDAU().m_comThr.m_totalBytesRead);

    m_errorInfo = "";
	vector<ErrorInfo>& comErr = DAU::GetDAU().m_comThr.m_errorHandler.m_comErr;
	m_errorInfo.Format("CRC: %d (%d)\r\nSeqNum: %d (%d)\r\nOffs: %d (%d)",comErr[COM_ERR_INVALID_CRC].count, comErr[COM_ERR_INVALID_CRC].active,
		comErr[COM_ERR_INVALID_SEQ_NUM].count, comErr[COM_ERR_INVALID_SEQ_NUM].active,
		comErr[COM_ERR_INVALID_START_OFFS].count, comErr[COM_ERR_INVALID_START_OFFS].active);

    m_debugGrid.UpdateGrid();

    m_dauAcqOn = DAU::GetDAU().m_mainBoardRegsPro.m_dataAcqEnable;
    m_chAEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlA_Enable;
    m_chBEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlB_Enable;

    CButton* pButt = (CButton*)GetDlgItem(IDC_ENABLE_COMPENSATION);
    pButt->SetCheck(DAU::GetDAU().m_dataCompensator.GetEnable());
    str.Format("Enable Compensation (%d)",DAU::GetDAU().m_dataCompensator.GetActivate());
    pButt->SetWindowText(str);

    UpdateData(FALSE);
}


void DebugDlg::OnTimer(UINT nIDEvent)
{
    UpdateCounters();


    
    CDialog::OnTimer(nIDEvent);
}


void DebugDlg::StopTimer()
{
    if(m_timer != 0)
    {
        KillTimer(m_timer);
        m_timer = 0;
    }
}

void DebugDlg::OnOK()
{
    StopTimer();

    CDialog::OnOK();
}

void DebugDlg::OnCancel()
{
    StopTimer();

    CDialog::OnCancel();
}

void DebugDlg::OnBnClickedLoopTestButt()
{
    int result = DAU::GetDAU().m_comThr.LoopTest();

    CString str;
    str.Format("Loop test result: %s",result==1?"OK":"FAIL");
    ::AfxMessageBox(str);

}

void DebugDlg::OnBnClickedButton3()
{
    UpdateData(TRUE);
    DAU::GetDAU().m_comThr.SendTestData(m_testData);
}

void DebugDlg::OnEnChangeTestData()
{
    UpdateData(TRUE);
}

void DebugDlg::OnBnClickedSendDacButt()
{
    UpdateData(TRUE);
    DAU::GetDAU().m_comThr.SendDACData(m_DACData);
}

void DebugDlg::OnEnChangeDacTest()
{
    UpdateData(TRUE);
}

void DebugDlg::OnCbnSelchangeRateCombo()
{
    CString str;
    int index;
    if((index = m_rateCombo.GetCurSel()) == CB_ERR)
        return;

    m_rateCombo.GetLBText(index, str);    
    
    int rate = atoi(str);
    
    //Set rate
    DAU::GetDAU().m_comThr.SetSamplingMode(rate);
}

void DebugDlg::OnBnClickedDacTestButt()
{
    DAU::GetDAU().m_comThr.BeginDACTest();
}

void DebugDlg::OnBnClickedDauChAEnable()
{
    UpdateData(TRUE);

    DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProA, m_chAEnable);
    
}

void DebugDlg::OnBnClickedDauChBEnable()
{
    UpdateData(TRUE);
    DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProB, m_chBEnable);
}

void DebugDlg::OnBnClickedSimulButton()
{
    if(m_simulDlg.m_hWnd == NULL)
        m_simulDlg.Create(IDD_DAU_SIMUL);

    m_simulDlg.ShowWindow(m_simulDlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);


}

void DebugDlg::OnBnClickedSetToFix()
{
    vector<Sensor*>& sensors = DAU::GetDAU().m_sensorPointers ;    

    vector<Sensor*>::iterator iter;
    for(iter = sensors.begin();iter!=sensors.end();iter++)
    {
        TRACE("%s, type:%d\n",(*iter)->GetSerialNumber(),(*iter)->GetType());
        (*iter)->SetSerialNumber("000");
        (*iter)->SetType(UnitType::Fixed); //
    }

    static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
    static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSyncroConfiguration( SW_SHOW );
}

void DebugDlg::OnBnClickedClearAll()
{
    vector<Sensor*>& sensors = DAU::GetDAU().m_sensorPointers ;    
    vector<Syncro*>& syncros = DAU::GetDAU().m_syncroPointers ;    

    vector<Sensor*>::iterator iter;
    for(iter = sensors.begin();iter!=sensors.end();iter++)
    {
        TRACE("%s, type:%d\n",(*iter)->GetSerialNumber(),(*iter)->GetType());
        (*iter)->SetSerialNumber("000");
        (*iter)->SetType(UnitType::Unused); //
    }

    vector<Syncro*>::iterator syIter;
    for(syIter = syncros.begin();syIter!=syncros.end();syIter++)
    {        
        (*syIter)->SetType(UnitType::Unused); //
        
        Digital* pDig = dynamic_cast<Digital*>(*syIter);
        if(pDig)
        {
            pDig->SetSyncroType(DigChTypeProUnused);
        }
    }
    static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
    static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSyncroConfiguration( SW_SHOW );
}

void DebugDlg::OnBnClickedLogDacData()
{
    UpdateData(TRUE);
    
    DAU::GetDAU().m_logDACData = m_logDACData;
    
}

void DebugDlg::OnBnClickedRestartButt()
{
	DAU::GetDAU().Restart();
}

void DebugDlg::OnEnKillfocusDebugFlags()
{
	CWnd* pWnd= GetDlgItem(IDC_DEBUG_FLAGS);
	CString str;
	pWnd->GetWindowText(str);

	int n;
	stringstream ss((LPCTSTR)str,std::stringstream::in);
	ss >> std::hex >> n;

	g_AlignerData.debugFlag = n;

}

void DebugDlg::OnEnKillfocusK()
{
    CWnd* pWnd= GetDlgItem(IDC_COMP_K);
    CString str;
    pWnd->GetWindowText(str);
    
    int k = atoi(str);
    
    DAU::GetDAU().m_dataCompensator.m_rollFilter.SetK(k);    
    DAU::GetDAU().m_dataCompensator.m_pitchFilter.SetK(k);    

}


void DebugDlg::OnBnClickedDebugInfoButton()
{
	CString totalStr,str;
	str.Format("0x01 - Dump fifo when error (150B)");
	totalStr += str + "\r\n";
	str.Format("0x02 - Dump frame");
	totalStr += str + "\r\n";
    str.Format("0x04 - Dump MINS");
    totalStr += str + "\r\n";

	::AfxMessageBox(totalStr);
}

void DebugDlg::OnBnClickedEnableCompensation()
{
    CButton* pButt = (CButton*)GetDlgItem(IDC_ENABLE_COMPENSATION);
    DAU::GetDAU().m_dataCompensator.SetEnable(pButt->GetCheck());
}

void DebugDlg::OnBnClickedSetAzButton()
{
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CH_COMBO);    
    int selected=0;
    if((selected = pCombo->GetCurSel()) != CB_ERR) 
    {
        CWnd* pEdit = GetDlgItem(IDC_AZIM);
        CString str;
        pEdit->GetWindowText(str);
        double az = atof(str)*_PI/180.0f;
        Sensor *pSensor = DAU::GetDAU().GetSensor( selected );
        if(pSensor)
        {
            pSensor->SetCurrentAz(az);
        }
    }
}


void DebugDlg::OnBnClickedDumpSensorStates()
{
	DAU::GetDAU().DumpSensorStates();
}
