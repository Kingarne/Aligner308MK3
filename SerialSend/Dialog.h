#pragma once

#include "DAUDlg.h"
#include "HFSender.h"
#include "CamCtrlDlg.h"

class Dialog : public CDialog
{
    DECLARE_MESSAGE_MAP()
public:
    Dialog( CWnd* pParent = NULL ) ;
    enum { IDD = IDD_SERIALSEND_DIALOG } ; 

    void Callback(LPARAM lp);
protected:
    virtual void DoDataExchange( CDataExchange *pDX ) ;
    virtual BOOL OnInitDialog( void ) ;

    afx_msg HCURSOR OnQueryDragIcon( void ) ;
    afx_msg void OnSysCommand( UINT nID, LPARAM lParam ) ;
    afx_msg void OnPaint( void ) ;
    afx_msg void OnBnClickedComSettingsButton( void ) ;
    afx_msg void OnBnClickedApplyButton();
    afx_msg void OnTimer( UINT nIDEvent ) ;
    
private:
    void ClearData( void ) ;
    void UpdateHex( void ) ;
//    public:
    HICON m_hIcon ;
    CString m_text ;
    BYTE *m_pData ;
    int m_dataLength ;
    CComboBox m_frequency ;
    CComboBox m_comPort ;
    SerialPort m_serialPort ;
    BOOL m_hex ;
    BOOL m_addEndChar ;
    afx_msg void OnBnClickedDauButton();

    DAUDlg m_dauDlg;
	CamCtrlDlg m_camCtrlDlg;

    afx_msg void OnBnClickedAddStopChar();

    HFSender m_HFSender;
public:
    afx_msg void OnDestroy();
	afx_msg void OnBnClickedCamCtrlButton();
} ;
