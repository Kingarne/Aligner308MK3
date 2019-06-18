#pragma once
#include "afxwin.h"


// SetupHDLCGyroDlg dialog

class SetupHDLCGyroDlg : public CDialog
{
	DECLARE_DYNAMIC(SetupHDLCGyroDlg)

public:
	SetupHDLCGyroDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetupHDLCGyroDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_HDLC_GYRO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_HDLCSpeed;
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeHdlcSpeed();

    int m_speed;
};
