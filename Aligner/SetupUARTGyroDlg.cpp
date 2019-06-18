// SetupUARTGyroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SetupUARTGyroDlg.h"
#include ".\setupuartgyrodlg.h"


// SetupUARTGyroDlg dialog

IMPLEMENT_DYNAMIC(SetupUARTGyroDlg, CDialog)
SetupUARTGyroDlg::SetupUARTGyroDlg(CWnd* pParent /*=NULL*/)
: CDialog(SetupUARTGyroDlg::IDD, pParent),
m_rate(0),
m_bits(0),
m_prty(0)
{
}

SetupUARTGyroDlg::~SetupUARTGyroDlg()
{
}

void SetupUARTGyroDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BAUD_RATE, m_baudRate);
    DDX_Control(pDX, IDC_DATA_BITS, m_dataBits);
    DDX_Control(pDX, IDC_PARITY, m_parity);
}


BEGIN_MESSAGE_MAP(SetupUARTGyroDlg, CDialog)
    ON_CBN_SELCHANGE(IDC_BAUD_RATE, OnCbnSelchangeBaudRate)
    ON_CBN_SELCHANGE(IDC_DATA_BITS, OnCbnSelchangeDataBits)
    ON_CBN_SELCHANGE(IDC_PARITY, OnCbnSelchangeParity)
END_MESSAGE_MAP()


// SetupUARTGyroDlg message handlers

BOOL SetupUARTGyroDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    vector<int> rates;
    
    rates.push_back(115200);
    rates.push_back(114400);
    rates.push_back(57600);
    rates.push_back(38400);
    rates.push_back(19200);
    rates.push_back(9600);
    rates.push_back(4800);
    rates.push_back(2400);
    rates.push_back(1200);
    rates.push_back(600);
    rates.push_back(300);

    CString str;
    vector<int>::iterator iter;
    int i=0, select=0;
    for(iter =  rates.begin();iter!= rates.end();iter++,i++)
    {
        str.Format("%d bps",*iter);
        m_baudRate.AddString(str);
        m_baudRate.SetItemData(i,*iter);
        if(*iter == m_rate)
            select = i;
    }
    m_baudRate.SetCurSel(select);

    m_dataBits.AddString("5bits");
    m_dataBits.SetItemData(0,0);
    m_dataBits.AddString("6bits");
    m_dataBits.SetItemData(1,1);
    m_dataBits.AddString("7bits");
    m_dataBits.SetItemData(2,2);
    m_dataBits.AddString("8bits");
    m_dataBits.SetItemData(3,3);
    m_dataBits.SetCurSel(m_bits-5);

    m_parity.AddString("None");
    m_parity.SetItemData(0,0);
    m_parity.AddString("Even");
    m_parity.SetItemData(1,1);
    m_parity.AddString("Odd");
    m_parity.SetItemData(2,2);
    m_parity.SetCurSel(m_prty);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void SetupUARTGyroDlg::OnCbnSelchangeBaudRate()
{
    m_rate = m_baudRate.GetItemData(m_baudRate.GetCurSel());
}

void SetupUARTGyroDlg::OnCbnSelchangeDataBits()
{
    m_bits = m_dataBits.GetItemData(m_dataBits.GetCurSel())+5;
}

void SetupUARTGyroDlg::OnCbnSelchangeParity()
{
     m_prty = m_parity.GetItemData(m_parity.GetCurSel());
}
