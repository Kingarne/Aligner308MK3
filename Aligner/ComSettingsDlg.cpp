// omSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "ComSettingsDlg.h"
#include ".\comsettingsdlg.h"
#include "SystemSetup.h"
#include "SetupHDLCGyroDlg.h"
#include "SetupUARTGyroDlg.h"

// ComSettingsDlg dialog

IMPLEMENT_DYNAMIC(ComSettingsDlg, CDialog)
ComSettingsDlg::ComSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ComSettingsDlg::IDD, pParent)
{
}

ComSettingsDlg::~ComSettingsDlg()
{
}

void ComSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_GYRO_LIST, m_gyroList);
}


BEGIN_MESSAGE_MAP(ComSettingsDlg, CDialog)
    ON_NOTIFY(NM_DBLCLK, IDC_GYRO_LIST, OnNMDblclkGyroList)
END_MESSAGE_MAP()


// ComSettingsDlg message handlers

BOOL ComSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_gyroList.SetExtendedStyle(m_gyroList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
    m_gyroList.InsertColumn(0, "Gyro", LVCFMT_LEFT, 120); 
    m_gyroList.InsertColumn(1, "Speed", LVCFMT_LEFT, 100); 
    m_gyroList.InsertColumn(2, "Bits", LVCFMT_LEFT, 50); 
    m_gyroList.InsertColumn(3, "Parity", LVCFMT_LEFT, 50); 

    m_parityMap[0] = "None";
    m_parityMap[1] = "Even";
    m_parityMap[2] = "Odd";

    m_HDLCSpeedMap[0] = "614.4 kbps";
    m_HDLCSpeedMap[1] = "307.2 kbps";
    m_HDLCSpeedMap[2] = "153.6 kbps";
    m_HDLCSpeedMap[3] = "115.2 kbps";        

   UpdateList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void ComSettingsDlg::UpdateList()
{
    m_gyroList.DeleteAllItems();
    map<int, ValidSyncroInfo>& syncros = SysSetup->m_validSyncroTypes;
    map<int, ValidSyncroInfo>::iterator iter;    
    int i=0;
    CString str;
    for(iter = syncros.begin();iter!=syncros.end();iter++,i++)
    {
        m_gyroList.InsertItem(i, iter->second.name);                
        m_gyroList.SetItemData(i, iter->second.type);
    
        if(iter->second.serialType == DigChSerialTypeProUart)
        {
            str.Format("%d Baud",iter->second.rate);//19660800/(iter->second.rate+1));
            m_gyroList.SetItemText(i,1,str);
            str.Format("%d",iter->second.bits);
            m_gyroList.SetItemText(i,2,str);            
            m_gyroList.SetItemText(i,3,m_parityMap[iter->second.parity]);
        }
        else
        {            
            m_gyroList.SetItemText(i,1,m_HDLCSpeedMap[iter->second.rate]);        
            m_gyroList.SetItemText(i,2,"-");            
            m_gyroList.SetItemText(i,3,"-");
        }
    }
}

void ComSettingsDlg::OnNMDblclkGyroList(NMHDR *pNMHDR, LRESULT *pResult)
{
    int item;
    if((item = m_gyroList.GetNextItem(-1, LVNI_SELECTED)) != -1)
    {    
        map<int, ValidSyncroInfo>& syncros = SysSetup->m_validSyncroTypes;
        int type = m_gyroList.GetItemData(item);
        int serialType = syncros[type].serialType;
        //int type = syncros[type].type;
        TRACE("type:%d\n",type);

        switch(serialType)
        {
            case DigChSerialTypeProUart:            
            {
                SetupUARTGyroDlg dlg;
                dlg.m_rate = syncros[type].rate;
                dlg.m_bits = syncros[type].bits;
                dlg.m_prty = syncros[type].parity;
                if(dlg.DoModal() == IDOK)
                {
                    syncros[type].rate = dlg.m_rate;
                    syncros[type].bits = dlg.m_bits;
                    syncros[type].parity = dlg.m_prty;
                }
            }
            break;
            case DigChSerialTypeProHdlc:
            {
                SetupHDLCGyroDlg dlg;
                dlg.m_speed = syncros[type].rate;
                if(dlg.DoModal() == IDOK)
                {
                    syncros[type].rate = dlg.m_speed;
                }
            }
            default:break;
        }
        
        UpdateList();

    }
    *pResult = 0;
}

void ComSettingsDlg::OnOK()
{
    StoreDigitalSettings();

    CDialog::OnOK();
}


void ComSettingsDlg::StoreDigitalSettings()
{
    Registry reg;
    vector<CString> settings;
    CString entry;

    map<int, ValidSyncroInfo>& syncros = SysSetup->m_validSyncroTypes;
    map<int, ValidSyncroInfo>::iterator it;
    for(it = syncros.begin();it!=syncros.end();it++)
    {
        bool isUART = it->second.serialType == DigChSerialTypeProUart;
        entry.Format("%d %d %d %d %d %s",it->second.type, it->second.serialType, it->second.rate, it->second.bits, it->second.parity, it->second.name);
        settings.push_back(entry);
    }        

    reg.SetMultiStringValue("Setup\\Digital", settings );

}