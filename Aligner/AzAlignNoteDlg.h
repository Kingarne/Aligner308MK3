#pragma once


// AzAlignNoteDlg dialog

class AzAlignNoteDlg : public CDialog
{
	DECLARE_DYNAMIC(AzAlignNoteDlg)

public:
	AzAlignNoteDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AzAlignNoteDlg();

	bool m_isGunPresent;
	int m_command;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AZ_ALIGN_NOTE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRunSvtButton();
	afx_msg void OnBnClickedRunGatcButton();
	afx_msg void OnBnClickedOk();
};
