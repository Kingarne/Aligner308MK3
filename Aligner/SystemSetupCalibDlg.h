#pragma once

// SystemSetupCalibDialog dialog

class SystemSetupCalibDialog : public CDialog
{
public:
	SystemSetupCalibDialog(ProjectData &dataSource, CWnd *pParent = NULL);
	virtual ~SystemSetupCalibDialog(void);



	ProjectData m_proj;	
	CString m_folder;
	WhiteBackgroundEdit m_date;

protected:
	enum { IDD = IDD_SYSTEM_SETUP_CALIBRATION };

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog(void);
	void HandleNewMode(void);
	void HandleTextChanged(void);
	
	afx_msg void OnTextChanged(void);
	afx_msg void OnBnClickedBrowseButton(void);
	afx_msg void OnCbnSelchangePlatform(void);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(SystemSetupCalibDialog)

	void UpdatePlatforms();
	CComboBox m_platformCombo;
	
	vector<Platform> m_platforms;
	
	
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
