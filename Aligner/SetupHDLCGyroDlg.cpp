// SetupHDLCGyroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SetupHDLCGyroDlg.h"
#include ".\setuphdlcgyrodlg.h"


// SetupHDLCGyroDlg dialog

IMPLEMENT_DYNAMIC(SetupHDLCGyroDlg, CDialog)
SetupHDLCGyroDlg::SetupHDLCGyroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetupHDLCGyroDlg::IDD, pParent),m_speed(0)
{
}

SetupHDLCGyroDlg::~SetupHDLCGyroDlg()
{
}

void SetupHDLCGyroDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_HDLC_SPEED, m_HDLCSpeed);
}


BEGIN_MESSAGE_MAP(SetupHDLCGyroDlg, CDialog)
    ON_CBN_SELCHANGE(IDC_HDLC_SPEED, OnCbnSelchangeHdlcSpeed)
END_MESSAGE_MAP()


// SetupHDLCGyroDlg message handlers

BOOL SetupHDLCGyroDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_HDLCSpeed.AddString("614.4kbs");
    m_HDLCSpeed.SetItemData(0,0);
    m_HDLCSpeed.AddString("307.2kbs");
    m_HDLCSpeed.SetItemData(1,1);
    m_HDLCSpeed.AddString("153.6kbs");
    m_HDLCSpeed.SetItemData(2,2);
    m_HDLCSpeed.AddString("115.2kbs");
    m_HDLCSpeed.SetItemData(3,3);

    m_HDLCSpeed.SetCurSel(m_speed);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void SetupHDLCGyroDlg::OnCbnSelchangeHdlcSpeed()
{
    int sel = m_HDLCSpeed.GetCurSel();
    m_speed = m_HDLCSpeed.GetItemData(sel);
}
