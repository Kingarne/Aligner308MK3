#pragma once
#include "afxcmn.h"
#include "SensorSimulDlg.h"

// SimulDlg dialog

class SimulDlg : public CDialog
{
	DECLARE_DYNAMIC(SimulDlg)

public:
	SimulDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SimulDlg();

// Dialog Data
	enum { IDD = IDD_DAU_SIMUL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_sensorList;
    virtual BOOL OnInitDialog();
    BOOL m_bSimulate;
    afx_msg void OnBnClickedSimulate();

    vector<SensorSimulDlg*> m_sensorDlgVec;
    afx_msg void OnNMDblclkSenorList(NMHDR *pNMHDR, LRESULT *pResult);
    void UpdateList();
    afx_msg void OnTimer(UINT nIDEvent);
    int m_timer;

    DAUData* m_pSimulatedData;
    unsigned short ToProtocolFormat(double deg);
    unsigned short ToSensorXFormat(double deg);
};
