#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SimulDlg.h"
#include "DebugGrid.h"

// DebugDlg dialog

class DebugDlg : public CDialog
{
	DECLARE_DYNAMIC(DebugDlg)

public:
	DebugDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DebugDlg();

// Dialog Data
	enum { IDD = IDD_DEBUG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    void InitControls();
public:
    afx_msg void OnBnClickedDauAcqOn();
    BOOL m_dauAcqOn;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedDauResetButt();
    CListCtrl m_framesList;
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//    afx_msg void OnDestroy();
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

    int m_timer;
    afx_msg void OnTimer(UINT nIDEvent);
    void UpdateCounters();    
protected:
    virtual void OnOK();
    virtual void OnCancel();
    void StopTimer();

    LARGE_INTEGER m_lastUpdate;
    LARGE_INTEGER m_freq;
    vector<int> m_lastCounters;
    DebugGrid m_debugGrid;    
public:
    afx_msg void OnBnClickedLoopTestButt();
    int m_testData;
    CString m_recTestData;
    afx_msg void OnBnClickedButton3();
    afx_msg void OnEnChangeTestData();
    CString m_recDACData;
    int m_DACData;
    afx_msg void OnBnClickedSendDacButt();
    afx_msg void OnEnChangeDacTest();
    CString m_DAUInfo;
    CComboBox m_rateCombo;
    afx_msg void OnCbnSelchangeRateCombo();
    afx_msg void OnBnClickedDacTestButt();
    BOOL m_chAEnable;
    BOOL m_chBEnable;
    afx_msg void OnBnClickedDauChAEnable();
    afx_msg void OnBnClickedDauChBEnable();
    afx_msg void OnBnClickedSimulButton();

    SimulDlg m_simulDlg;
    afx_msg void OnBnClickedSetToFix();
    CString m_info;    
    afx_msg void OnBnClickedClearAll();
    afx_msg void OnBnClickedLogDacData();
    BOOL m_logDACData;
	CString m_errorInfo;
	afx_msg void OnBnClickedRestartButt();	
	afx_msg void OnEnKillfocusDebugFlags();
    afx_msg void OnEnKillfocusK();
    afx_msg void OnBnClickedDebugInfoButton();
    afx_msg void OnBnClickedEnableCompensation();
    afx_msg void OnBnClickedSetAzButton();
	afx_msg void OnBnClickedDumpSensorStates();
};
