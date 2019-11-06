// ProjectOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SystemSetupDlg.h"
#include "afxdialogex.h"


// SystemSetupDialog dialog


IMPLEMENT_DYNAMIC(SystemSetupDialog, CDialog)

SystemSetupDialog::SystemSetupDialog(ProjectData &dataSource, CWnd *pParent) : CDialog(SystemSetupDialog::IDD, pParent), m_proj(dataSource)
{
	m_proj.m_projectName = "";
}

SystemSetupDialog::~SystemSetupDialog(void)
{
	// Empty
}

void SystemSetupDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_SYSTEM_SETUP_DIRECTORY, m_folder);
	DDX_Text(pDX, IDC_SYSTEM_SETUP_PROJECT, m_proj.m_projectName);
	DDV_MaxChars(pDX, m_proj.m_projectName, 32);
	DDX_Text(pDX, IDC_SYSTEM_SETUP_NAME, m_proj.m_operatorName);
	DDV_MaxChars(pDX, m_proj.m_operatorName, 32);
	DDX_Text(pDX, IDC_EDIT_LATITUDE, m_proj.m_latitude);
	DDV_MinMaxDouble(pDX, m_proj.m_latitude, -80, 80);
	DDX_Control(pDX, IDC_SYSTEM_SETUP_DATE, m_date);
	DDX_Text(pDX, IDC_SYSTEM_SETUP_PLACE, m_proj.m_location);
	DDX_CBIndex(pDX, IDC_SYSTEM_SETUP_UNITS, m_proj.m_unit);
	DDX_Control(pDX, IDC_SHIP_COMBO, m_shipCombo);
}

BEGIN_MESSAGE_MAP(SystemSetupDialog, CDialog)	
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_NAME, OnTextChanged)
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_PLACE, OnTextChanged)
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_SHIP, OnTextChanged)
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_PROJECT, OnTextChanged)
	ON_BN_CLICKED(IDC_SYSTEM_SETUP_BROWSE, OnBnClickedBrowseButton)
	ON_CBN_SELCHANGE(IDC_SHIP_COMBO, OnCbnSelchangeShipName)
	ON_BN_CLICKED(IDOK, &SystemSetupDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL SystemSetupDialog::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	if (theApp.IsAligner202Enabled())
	{
		GetDlgItem(IDC_LATITUDE_BOUNDARY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LATITUDE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MODE_BOUNDARY)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_UNITS_BOUNDARY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSTEM_SETUP_UNITS)->ShowWindow(SW_HIDE);
		CString str;
		str.LoadString(IDS_OBJECT);
		SetDlgItemText(IDC_SHIP_BOUNDARY, str);
	
		m_shipCombo.ShowWindow(SW_HIDE);
		//m_data.m_shipName = "N/A";
		//UpdateShips();        
	}
	else
	{		
		UpdateShips();
	}
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(m_proj.m_time);
	COleDateTime time(m_proj.m_time);
	m_date.SetWindowText(time.Format());// VAR_DATEVALUEONLY ) );
	HandleTextChanged();

	DAU::GetDAU().EnableErrorEvent(FALSE);
	return TRUE;
}

void SystemSetupDialog::HandleTextChanged(void)
{	
	CEdit *pProject = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PROJECT));
	CEdit *pName = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_NAME));
	CEdit *pPlace = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PLACE));
	CButton *pOK = static_cast<CButton *>(GetDlgItem(IDOK));
	CString text;
	
	bool enable = true;
	pProject->GetWindowText(text);
	if (0 == text.GetLength())
	{
		enable = false;		
	}

	if (theApp.IsAligner308Enabled())
	{
		if (m_shipCombo.GetCurSel() == CB_ERR)
		{
			enable = false;				
		}
	}
	pOK->EnableWindow(enable);
}

void SystemSetupDialog::HandleNewMode(void)
{	
	/*CEdit *pProject = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PROJECT));
	CEdit *pName = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_NAME));
	CEdit *pPlace = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PLACE));
	CComboBox *pUnits = static_cast<CComboBox *>(GetDlgItem(IDC_SYSTEM_SETUP_UNITS));
	
	pProject->SetWindowText(_T(""));
	pProject->SetReadOnly(FALSE);
	pName->SetReadOnly(FALSE);
	pPlace->SetReadOnly(FALSE);	
	pUnits->SetCurSel(m_proj.m_unit);
	pUnits->EnableWindow(TRUE);
	m_shipCombo.EnableWindow(TRUE);
	
	DAU::GetDAU().EnableErrorEvent(FALSE);
	HandleTextChanged();*/
}

void SystemSetupDialog::OnCbnSelchangeShipName(void)
{
	CString str;
	int index;
	if ((index = m_shipCombo.GetCurSel()) == CB_ERR)
		return;

	m_shipCombo.GetLBText(index, str);
	m_proj.m_shipName = str;

	long data = m_shipCombo.GetItemData(index);
	m_proj.m_shipID = LOWORD(data);
	m_proj.m_shipClass = HIWORD(data);

	//HandleTextChanged();
}

void SystemSetupDialog::OnCbnSelchangeSystemSetupDialogMode(void)
{
	HandleNewMode();
}




void SystemSetupDialog::UpdateShips()
{
	m_shipCombo.ResetContent();

	vector<Ship> ships;
	DBInterface::Instance()->GetShips(ships);
	if (ships.size() == 0)
		return;

	for (auto iter = ships.begin(); iter != ships.end(); iter++)
	{
		int index = m_shipCombo.AddString(iter->m_name);
		m_shipCombo.SetItemData(index, MAKELONG(iter->m_ID, iter->m_classID));
	}

	m_shipCombo.SelectString(0, m_proj.m_shipName);
	

}

void SystemSetupDialog::OnTextChanged(void)
{
	HandleTextChanged();
}

void SystemSetupDialog::OnBnClickedBrowseButton(void)
{
	TCHAR path[MAX_PATH];
	TCHAR title[] = _T("Select project location");
	BROWSEINFO browseInfo = { m_hWnd, NULL, path, title, 0, NULL, 0, 0 };
	ITEMIDLIST *pItemList;
	if (NULL != (pItemList = SHBrowseForFolder(&browseInfo)))
	{
		TCHAR buffer[MAX_PATH];
		if (SHGetPathFromIDList(pItemList, buffer))
		{
			SetDlgItemText(IDC_SYSTEM_SETUP_DIRECTORY, buffer);
		}
	}
}


void SystemSetupDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if (m_proj.m_projectName == "")
		return;	

	if (DBInterface::Instance()->ProjectExist(m_proj.m_projectName, "Project"))
	{
		::AfxMessageBox(_T("Project already exist."));
		return;
	}

	OnCbnSelchangeShipName();


	CDialog::OnOK();
}
