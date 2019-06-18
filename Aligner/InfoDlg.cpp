// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// InfoDlg dialog

IMPLEMENT_DYNAMIC(InfoDlg, CDialog)

InfoDlg::InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InfoDlg::IDD, pParent)
	, m_info(_T(""))
	, m_check(FALSE)
{

}

InfoDlg::~InfoDlg()
{
}

void InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFO_TEXT, m_info);
	DDX_Check(pDX, IDC_SHOW_AGAIN_CHECK, m_check);
}


BEGIN_MESSAGE_MAP(InfoDlg, CDialog)
	ON_BN_CLICKED(IDC_SHOW_AGAIN_CHECK, &InfoDlg::OnBnClickedShowAgainCheck)
END_MESSAGE_MAP()


// InfoDlg message handlers


BOOL InfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_title);

	if (!m_mode)
	{

		CWnd* pWnd = GetDlgItem(IDOK);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDCANCEL);
		pWnd->SetWindowText("Close");
	}


	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void InfoDlg::OnBnClickedShowAgainCheck()
{
	UpdateData(TRUE);
}
