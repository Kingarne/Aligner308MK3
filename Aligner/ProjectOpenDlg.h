#pragma once


// ProjectOpenDlg dialog

class ProjectOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(ProjectOpenDlg)

public:
	ProjectOpenDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ProjectOpenDlg();

	ProjectData m_selectedProj;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECT_OPEN_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkProjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();

protected:
	vector<ProjectData> m_projects;

	
	void InitProjList();
public:
	CListCtrl m_projList;
	afx_msg void OnBnClickedDeleteButton();
};
