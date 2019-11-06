// ProjectOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SystemSetupCalibDlg.h"
#include "afxdialogex.h"


// SystemSetupCalibDialog dialog


IMPLEMENT_DYNAMIC(SystemSetupCalibDialog, CDialog)

SystemSetupCalibDialog::SystemSetupCalibDialog(ProjectData &dataSource, CWnd *pParent) : CDialog(SystemSetupCalibDialog::IDD, pParent), m_proj(dataSource)
{
	m_proj.m_projectName = "";
}

SystemSetupCalibDialog::~SystemSetupCalibDialog(void)
{
	// Empty
}

void SystemSetupCalibDialog::DoDataExchange(CDataExchange *pDX)
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
	DDX_Control(pDX, IDC_PLATFORM_COMBO, m_platformCombo);
	
}

BEGIN_MESSAGE_MAP(SystemSetupCalibDialog, CDialog)	
	ON_CBN_SELCHANGE(IDC_PLATFORM_COMBO, OnCbnSelchangePlatform)
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_NAME, OnTextChanged)
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_PLACE, OnTextChanged)	
	ON_EN_CHANGE(IDC_SYSTEM_SETUP_PROJECT, OnTextChanged)
	ON_BN_CLICKED(IDC_SYSTEM_SETUP_BROWSE, OnBnClickedBrowseButton)	
	ON_BN_CLICKED(IDOK, &SystemSetupCalibDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL SystemSetupCalibDialog::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	

	if (theApp.IsAligner202Enabled())
	{		
		GetDlgItem(IDC_EDIT_LATITUDE)->ShowWindow(SW_HIDE);		
		CString str;
		str.LoadString(IDS_OBJECT);
		SetDlgItemText(IDC_SHIP_BOUNDARY, str);
	
		m_platformCombo.ShowWindow(SW_HIDE);
		//m_data.m_shipName = "N/A";
		//UpdateShips();        
	}
	else
	{		
		UpdatePlatforms();
	}
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(m_proj.m_time);
	COleDateTime time(m_proj.m_time);
	m_date.SetWindowText(time.Format());// VAR_DATEVALUEONLY ) );
	HandleTextChanged();
	DAU::GetDAU().EnableErrorEvent(FALSE);
	return TRUE;
}

void SystemSetupCalibDialog::HandleTextChanged(void)
{	
	CEdit *pProject = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PROJECT));
	CEdit *pName = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_NAME));
	CEdit *pPlace = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PLACE));
	CButton *pOK = static_cast<CButton *>(GetDlgItem(IDOK));
	CString text;

	bool enable = true;
	pProject->GetWindowText(text);
	if (!text.GetLength())
	{
		enable = false;
	}
	pName->GetWindowText(text);
	if (!text.GetLength())
	{
		enable = false;
	}
	pPlace->GetWindowText(text);
	if (!text.GetLength())
	{
		enable = false;
	}
	pOK->EnableWindow(enable);

}

void SystemSetupCalibDialog::HandleNewMode(void)
{	
	/*CEdit *pProject = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PROJECT));
	CEdit *pName = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_NAME));
	CEdit *pPlace = static_cast<CEdit *>(GetDlgItem(IDC_SYSTEM_SETUP_PLACE));
	CComboBox *pUnits = static_cast<CComboBox *>(GetDlgItem(IDC_SYSTEM_SETUP_UNITS));
	
	
	//pProject->SetWindowText(_T("Calibration"));
//	pProject->SetReadOnly(TRUE);
//	pName->SetReadOnly(FALSE);
//	pPlace->SetReadOnly(FALSE);		
//	m_platformCombo.EnableWindow(FALSE);
//	pUnits->SetCurSel(0);
//	pUnits->EnableWindow(FALSE);
		

	
	DAU::GetDAU().EnableErrorEvent(FALSE);
	HandleTextChanged();
	*/
}






void SystemSetupCalibDialog::UpdatePlatforms()
{
	m_platformCombo.ResetContent();
//	m_proj.m_shipName = "";
//	m_proj.m_shipClass = m_proj.m_shipID = 0;
	m_platforms.clear();
	
	DBInterface::Instance()->GetPlatforms(m_platforms);
	if (m_platforms.size() == 0)
		return;

	CString str;
	for (int i = 0; i< m_platforms.size(); i++)
	{		
		str.Format("%03d", m_platforms[i].m_sn);
		int index = m_platformCombo.AddString(str);
			m_platformCombo.SetItemData(index, (DWORD_PTR)&m_platforms[i]);
	}
	
	m_platformCombo.SelectString(0, str);
	

}

void SystemSetupCalibDialog::OnTextChanged(void)
{
	HandleTextChanged();
}

void SystemSetupCalibDialog::OnBnClickedBrowseButton(void)
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



void SystemSetupCalibDialog::OnCbnSelchangePlatform(void)
{
	CString str;
	int index;
	if ((index = m_platformCombo.GetCurSel()) == CB_ERR)
		return;

	m_platformCombo.GetLBText(index, str);
	//m_proj.m_platform. = str;
	Platform* p = (Platform*)m_platformCombo.GetItemData(index);
	m_proj.m_platform = *p;


	//HandleTextChanged();
}

void SystemSetupCalibDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if (m_proj.m_projectName == "")
		return;	

	if (DBInterface::Instance()->ProjectExist(m_proj.m_projectName, "ProjectCalibration"))
	{
		::AfxMessageBox(_T("Project already exist."));
		return;
	}

	OnCbnSelchangePlatform();

	CDialog::OnOK();
}
