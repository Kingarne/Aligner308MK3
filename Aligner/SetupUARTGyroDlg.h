#pragma once
#include "afxwin.h"


// SetupUARTGyroDlg dialog

class SetupUARTGyroDlg : public CDialog
{
	DECLARE_DYNAMIC(SetupUARTGyroDlg)

public:
	SetupUARTGyroDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetupUARTGyroDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_UART_GYRO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_baudRate;
    CComboBox m_dataBits;
    CComboBox m_parity;
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeBaudRate();
    afx_msg void OnCbnSelchangeDataBits();
    afx_msg void OnCbnSelchangeParity();

    int m_rate;
    int m_bits;
    int m_prty;
};
