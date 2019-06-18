// SimulDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SimulDlg.h"
#include ".\simuldlg.h"


// SimulDlg dialog

IMPLEMENT_DYNAMIC(SimulDlg, CDialog)
SimulDlg::SimulDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SimulDlg::IDD, pParent)
    , m_bSimulate(FALSE)
    , m_timer(0)
{
}

SimulDlg::~SimulDlg()
{
    vector<SensorSimulDlg*>::iterator iter;
    for(iter = m_sensorDlgVec.begin();iter!=m_sensorDlgVec.end() ;iter++)
    {        
        if(*iter != NULL)
            delete *iter;        
    }
    m_sensorDlgVec.clear();
}

void SimulDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENOR_LIST, m_sensorList);
    DDX_Check(pDX, IDC_SIMULATE, m_bSimulate);
}


BEGIN_MESSAGE_MAP(SimulDlg, CDialog)
    ON_BN_CLICKED(IDC_SIMULATE, OnBnClickedSimulate)    
    ON_NOTIFY(NM_DBLCLK, IDC_SENOR_LIST, OnNMDblclkSenorList)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// SimulDlg message handlers

BOOL SimulDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_pSimulatedData = &DAU::GetDAU().m_simulatedData;

    m_sensorList.SetExtendedStyle(m_sensorList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

    m_sensorList.InsertColumn(0, "Sensor", LVCFMT_LEFT, 70); 
    m_sensorList.InsertColumn(1, "Roll", LVCFMT_LEFT, 50); 
    m_sensorList.InsertColumn(2, "Pitch", LVCFMT_LEFT, 50); 
    m_sensorList.InsertColumn(3, "Temp", LVCFMT_LEFT, 50); 
        
    CString str;
    SensorSimulDlg* pDlg;
    for(int i=0;i<9;i++)
    {        
        str.Format("J%d",i+1);

        pDlg = new SensorSimulDlg;
        pDlg->m_id = str;
        m_sensorDlgVec.push_back(pDlg);
                
        m_sensorList.InsertItem(i, str);
    }
    

    UpdateList();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void SimulDlg::UpdateList()
{
    CString str;
    int i=0;
    SensorSimulDlg* pDlg;
    vector<SensorSimulDlg*>::iterator iter;
    for(iter = m_sensorDlgVec.begin();iter!=m_sensorDlgVec.end() ;iter++,i++)
    {
        pDlg = *iter;
        str.Format("%.2f",pDlg->m_rollAng);
        m_sensorList.SetItemText(i, 1, str);    

        str.Format("%.2f",pDlg->m_pitchAng);
        m_sensorList.SetItemText(i, 2, str);    
    
    }



}


void SimulDlg::OnBnClickedSimulate()
{    
    UpdateData(TRUE);
    if(m_bSimulate)
    {
        if(m_timer == 0)
            m_timer= SetTimer(1, 100, NULL);
    }
    else
    {
        if(m_timer != 0)
        {
            KillTimer(m_timer);
            m_timer = 0;
        }
    }

    DAU::GetDAU().m_simulationActive = m_bSimulate;
}

void SimulDlg::OnNMDblclkSenorList(NMHDR *pNMHDR, LRESULT *pResult)
{    
    int item;
    if((item = m_sensorList.GetNextItem(-1, LVNI_SELECTED)) != -1)
    {
        TRACE("Sensor:%d\n",item);
        if(m_sensorDlgVec[item]->m_hWnd == NULL)
            m_sensorDlgVec[item]->Create(IDD_SENSOR_SIMUL_DLG);
    
        m_sensorDlgVec[item]->ShowWindow(m_sensorDlgVec[item]->IsWindowVisible() ? SW_HIDE : SW_SHOW);
    }


    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void Swap(unsigned short& data)
{
    short low = data & 0x00ff;
    short hi = data >> 8 & 0x00ff;
    data = MAKEWORD(hi, low);
}


unsigned short SimulDlg::ToProtocolFormat(double deg)
{
    double r = (deg / 180.0f) * 65536 ;
    unsigned short ang = (short)(r+0.5f);
    //Swap(ang);    
    return ang;
}

unsigned short SimulDlg::ToSensorXFormat(double deg)
{
    double mrad = ToMRad(deg);
    unsigned short ang = mrad/100.0*32768;
    
    return ang;
}


void SimulDlg::OnTimer(UINT nIDEvent)
{
    vector<SensorSimulDlg*>::iterator iter;
    for(iter = m_sensorDlgVec.begin();iter!=m_sensorDlgVec.end() ;iter++)
    {        
        (*iter)->Update();
    }
    
    UpdateList();
    
    for(int i=0;i<2;i++)
    {        
        //Note, swaped in recived data!
        m_pSimulatedData->m_syncro[i].m_roll = ToSensorXFormat(m_sensorDlgVec[1-i]->m_rollAng);
        m_pSimulatedData->m_syncro[i].m_pitch = ToSensorXFormat(m_sensorDlgVec[1-i]->m_pitchAng);
    }

    for(int i=2;i<9;i++)
    {        
        m_pSimulatedData->m_sensor[i-2].m_roll = ToSensorXFormat(m_sensorDlgVec[i]->m_rollAng);
        m_pSimulatedData->m_sensor[i-2].m_pitch = ToSensorXFormat(m_sensorDlgVec[i]->m_pitchAng);
    }



    CDialog::OnTimer(nIDEvent);
}
