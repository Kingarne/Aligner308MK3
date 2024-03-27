// FixTheoAdapterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "FixTheoAdapterDlg.h"
#include "afxdialogex.h"


// FixTheoAdapterDlg dialog

IMPLEMENT_DYNAMIC(FixTheoAdapterDlg, CDialog)

FixTheoAdapterDlg::FixTheoAdapterDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_THEO_FIX_ADAPTER_CAL_DIALOG, pParent)
{

}

FixTheoAdapterDlg::~FixTheoAdapterDlg()
{
}

void FixTheoAdapterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADAPTER_COMBO, m_adapterCombo);
}


BEGIN_MESSAGE_MAP(FixTheoAdapterDlg, CDialog)
	ON_BN_CLICKED(IDC_UPDATE_CAL_BUTTON, &FixTheoAdapterDlg::OnBnClickedUpdateCalButton)
	ON_CBN_SELCHANGE(IDC_ADAPTER_COMBO, &FixTheoAdapterDlg::OnCbnSelchangeAdapterCombo)
END_MESSAGE_MAP()


// FixTheoAdapterDlg message handlers


BOOL FixTheoAdapterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	vector<CString> adapters;
	DBInterface::Instance()->GetTheoAdapters(AdapterData::Type::Fix, adapters);
	for (auto i = adapters.begin(); i != adapters.end(); i++)
	{
		m_adapterCombo.AddString(*i);
	}

	if (adapters.size() > 0)
	{
		m_adapterCombo.SelectString(0, adapters[0]);
		UpdateCalibrationTime();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void FixTheoAdapterDlg::UpdateCalibrationTime()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_CAL);
	int selectedIndex = m_adapterCombo.GetCurSel();
	if (selectedIndex != CB_ERR)
	{
		CString selectedText;
		m_adapterCombo.GetLBText(selectedIndex, selectedText);
		DBTIMESTAMP time;
		DBInterface::Instance()->GetAdapterCalibTime(selectedText, time, UnitType::Types::Theo);
		CString timeStr;
		timeStr.Format("Calibrated: %02d/%02d/%02d %02d:%02d:%02d", time.year - 2000, time.month, time.day, time.hour, time.minute, time.second);
		pWnd->SetWindowText(timeStr);
	}
}

void FixTheoAdapterDlg::OnBnClickedUpdateCalButton()
{
	int selectedIndex = m_adapterCombo.GetCurSel();
	if (selectedIndex != CB_ERR)
	{
		CString selectedText;
		m_adapterCombo.GetLBText(selectedIndex, selectedText);
		DBInterface::Instance()->UpdateTheoAdapterCalibration(selectedText, 0, 0);
		UpdateCalibrationTime();
	}
}


void FixTheoAdapterDlg::OnCbnSelchangeAdapterCombo()
{
	UpdateCalibrationTime();
	
}
