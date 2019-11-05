// ProjectOpenCalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "ProjectOpenCalibDlg.h"
#include "afxdialogex.h"


// ProjectOpenCalibDlg dialog

IMPLEMENT_DYNAMIC(ProjectOpenCalibDlg, CDialog)

ProjectOpenCalibDlg::ProjectOpenCalibDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PROJECT_OPEN_DLG, pParent)
{

}

ProjectOpenCalibDlg::~ProjectOpenCalibDlg()
{
}

void ProjectOpenCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_projList);
}


BEGIN_MESSAGE_MAP(ProjectOpenCalibDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_PROJECT_LIST, &ProjectOpenCalibDlg::OnNMDblclkProjectList)
	ON_BN_CLICKED(IDOK, &ProjectOpenCalibDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ProjectOpenCalibDlg message handlers


BOOL ProjectOpenCalibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	DBInterface::Instance()->GetCalibrationProjects(m_projects);

	m_projList.SetExtendedStyle(m_projList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_projList.InsertColumn(0, "Name", LVCFMT_LEFT, 120);
	m_projList.InsertColumn(1, "Time", LVCFMT_LEFT, 120);
	m_projList.InsertColumn(2, "Operator", LVCFMT_LEFT, 100);
	m_projList.InsertColumn(3, "Platform", LVCFMT_LEFT, 100);	
	m_projList.InsertColumn(4, "Place", LVCFMT_LEFT, 100);
	m_projList.InsertColumn(5, "Latitude", LVCFMT_LEFT, 60);
	
	
	InitProjList();
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ProjectOpenCalibDlg::InitProjList()
{
	m_projList.DeleteAllItems();
	int i = 0;
	for (auto iter = m_projects.begin(); iter != m_projects.end(); iter++)
	{
		CString str;
	
		
		COleDateTime time(iter->m_time);

		m_projList.InsertItem(i, iter->m_projectName);
		m_projList.SetItemText(i, 1, time.Format(_T("%Y-%m-%d %H:%M:%S")));
		m_projList.SetItemText(i, 2, iter->m_operatorName);
		str.Format("%03d", iter->m_platform.m_sn);
		m_projList.SetItemText(i, 3, str);
		m_projList.SetItemText(i, 4, iter->m_location);
		str.Format("%.1f", iter->m_latitude);
		m_projList.SetItemText(i, 5, str);
			
		m_projList.SetItemData(i, (DWORD_PTR)&(*iter));
	}



}

void ProjectOpenCalibDlg::OnNMDblclkProjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnBnClickedOk();

	*pResult = 0;
}


int ProjectOpenCalibDlg::GetSelectedItem(CListCtrl *plctrl)
{
	POSITION pos = plctrl->GetFirstSelectedItemPosition();
	int selected = -1;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = plctrl->GetNextSelectedItem(pos);
			selected = nItem;
		}
	}
	//returns -1 if not selected;
	return selected;
}

void ProjectOpenCalibDlg::OnBnClickedOk()
{
	int selected = GetSelectedItem(&m_projList);
	if ( selected < 0)
		return;

	m_selectedProj = *((ProjectData*)m_projList.GetItemData(selected));
	m_selectedProj.m_dauSerial = DAU::GetDAU().m_mainBoardRegsPro.m_dauSerialNo;

	CDialog::OnOK();
}
