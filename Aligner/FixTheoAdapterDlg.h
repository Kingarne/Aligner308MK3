#pragma once

#include "resource.h"
// FixTheoAdapterDlg dialog

class FixTheoAdapterDlg : public CDialog
{
	DECLARE_DYNAMIC(FixTheoAdapterDlg)

public:
	FixTheoAdapterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FixTheoAdapterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THEO_FIX_ADAPTER_CAL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void UpdateCalibrationTime();
public:
	virtual BOOL OnInitDialog();
	CComboBox m_adapterCombo;
	afx_msg void OnBnClickedUpdateCalButton();
	afx_msg void OnCbnSelchangeAdapterCombo();
};
