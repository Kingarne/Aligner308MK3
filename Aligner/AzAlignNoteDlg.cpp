// AzAlignNoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "AzAlignNoteDlg.h"
#include "afxdialogex.h"


// AzAlignNoteDlg dialog

IMPLEMENT_DYNAMIC(AzAlignNoteDlg, CDialog)

AzAlignNoteDlg::AzAlignNoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_AZ_ALIGN_NOTE_DIALOG, pParent)
{
	m_isGunPresent = false;
	m_command = -1;
}

AzAlignNoteDlg::~AzAlignNoteDlg()
{
}

void AzAlignNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AzAlignNoteDlg, CDialog)
	ON_BN_CLICKED(IDC_RUN_SVT_BUTTON, &AzAlignNoteDlg::OnBnClickedRunSvtButton)
	ON_BN_CLICKED(IDC_RUN_GATC_BUTTON, &AzAlignNoteDlg::OnBnClickedRunGatcButton)
	ON_BN_CLICKED(IDOK, &AzAlignNoteDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// AzAlignNoteDlg message handlers


BOOL AzAlignNoteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd;
	if (!m_isGunPresent)
	{
		pWnd = GetDlgItem(IDC_STATIC_GATC);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_RUN_GATC_BUTTON);
		pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void AzAlignNoteDlg::OnBnClickedRunSvtButton()
{
	m_command = 1;
	CDialog::OnOK();
}


void AzAlignNoteDlg::OnBnClickedRunGatcButton()
{
	m_command = 2;
	CDialog::OnOK();
}


void AzAlignNoteDlg::OnBnClickedOk()
{
	m_command = 0;
	CDialog::OnOK();
}
