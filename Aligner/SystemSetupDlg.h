#pragma once

// SystemSetupDialog dialog

class SystemSetupDialog : public CDialog
{
public:
	SystemSetupDialog(ProjectData &dataSource, CWnd *pParent = NULL);
	virtual ~SystemSetupDialog(void);

	ProjectData m_proj;
	WhiteBackgroundEdit m_date;

protected:
	enum { IDD = IDD_SYSTEM_SETUP };

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog(void);
	void HandleNewMode(void);
	void HandleTextChanged(void);

	afx_msg void OnCbnSelchangeSystemSetupDialogMode(void);
	afx_msg void OnCbnSelchangeShipName(void);
	afx_msg void OnTextChanged(void);
	afx_msg void OnBnClickedBrowseButton(void);


	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(SystemSetupDialog)

	void UpdateShips();
	CComboBox m_shipCombo;

	CString m_folder;
	
/*	CString m_name;
	CString m_operator;
	CString m_location;
	float m_latitude;
	int m_mode;
	int m_unit;

	CString m_shipName;
	*/
public:
	afx_msg void OnBnClickedOk();
};
