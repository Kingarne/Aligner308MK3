#pragma once
#include "afxcmn.h"


// ComSettingsDlg dialog

class ComSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(ComSettingsDlg)

public:
	ComSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ComSettingsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_COM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_gyroList;
    virtual BOOL OnInitDialog();
    afx_msg void OnNMDblclkGyroList(NMHDR *pNMHDR, LRESULT *pResult);
    void UpdateList();

    map<int, CString> m_parityMap;
    map<int, CString> m_HDLCSpeedMap;
protected:
    virtual void OnOK();

    void StoreDigitalSettings();
};
