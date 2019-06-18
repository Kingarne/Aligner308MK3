#pragma once



// InfoDlg dialog

class InfoDlg : public CDialog
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InfoDlg();

// Dialog Data
	enum { IDD = IDD_INFO_DLG };

	void SetTitle(CString str) { m_title = str; }
	void SetInfo(CString str) { m_info = str; }
	bool GetCheck(){return m_check;}
	void SetYesNoMode(bool b) { m_mode = b; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	BOOL m_check;
	CString m_title;
	CString m_info;
	bool m_mode;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedShowAgainCheck();
};
