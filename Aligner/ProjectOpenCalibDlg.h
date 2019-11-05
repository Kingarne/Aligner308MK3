#pragma once


// ProjectOpenCalibDlg dialog

class ProjectOpenCalibDlg : public CDialog
{
	DECLARE_DYNAMIC(ProjectOpenCalibDlg)

public:
	ProjectOpenCalibDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ProjectOpenCalibDlg();

	ProjectData m_selectedProj;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECT_CALIB_OPEN_DLG};
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

	int GetSelectedItem(CListCtrl *plctrl);
	void InitProjList();
public:
	CListCtrl m_projList;
};
