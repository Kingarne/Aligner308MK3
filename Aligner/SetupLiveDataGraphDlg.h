// $Id: SetupLiveDataGraphDlg.h,v 1.4 2012/07/02 14:32:59 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"
#include "GraphView.h"

#define NUM_CHANNELS (12+1)
#define FREQUENCY_FOR_LIVE_DATA_GRAPH		0x15
#define DEFAULT_LIVE_DATA_FILE_NAME								_T("LiveGraph")
#define DEFAULT_LIVE_DATA_TEXT_FILE_EXT								_T(".txt")

struct CtrlID
{
    CtrlID(){}
    CtrlID(int id1, int id2, int id3){testChY1 = id1;testChY2=id2;textCh=id3;}
    int testChY1;
    int testChY2;
    int textCh;
};

class CSetupLiveDataGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupLiveDataGraphDlg)

public:
	CSetupLiveDataGraphDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupLiveDataGraphDlg();
    BOOL IsFirstTime( void ) const;
    void SetFirstTime( BOOL firstTime );


// Dialog Data
	enum { IDD_A308 = IDD_SETUP_LIVE_DATA_GRAPH };
    enum { IDD_A202 = IDD_SETUP_LIVE_DATA_GRAPH_A202 };

private:
    //BOOL m_backupOfOverflowSettings[SIZE_OF_ARRAYS];
    CString m_Text, m_MsgCaption;
    CString m_ChText[NUM_CHANNELS];
//     static BOOL m_TestCh[NUM_CHANNELS];
//     static BOOL m_TestChY2A202[NUM_CHANNELS];
     static BOOL m_FirstTime;
    int m_ObjNo[NUM_CHANNELS];
    int m_NoOfCorr;
	double m_FilterTimeConstant;
	CComboBox m_FrequencyControl;
	static int m_Frequency; // TODO: Kludge, please refactor.
    double m_dataFreq;

    BOOL m_FilterOn;
    CGraphView *m_pGraph;
    BOOL m_ShowGraphPressed;
	double m_minTau; 
	double m_maxTau;
    int m_NoOfChY2A202;
    static BOOL m_StoreToFile;
    BOOL m_FileNameChecked;
public:
    static CString m_TextFileDir;
    static CString m_TextFileName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    BOOL OnInitDialog();
    void SetObjectData();
	void InitFrequencyControl( void );
    void ShowSettingsForAxes( void );
    void SetFilter( BOOL filterOn );
    BOOL InitGraph( void );
	void SetTauLimits( void );
	BOOL IsTauOK( void );
    void ClickedTestCh();
    BOOL IsFileNameOK( void );
    void ShowRow(int id, int val);
    void EnableRow(int id, BOOL val);
    void DumpGraphInfo();
    //void StoreOverflowSettings( void );
    //void RestoreOverflowSettings( void );
    void OnChannelClicked(int id, int opId, int axis);
    void OnChannelClicked202(int id, int opId, int axis);
    void OnChannelClicked308(int id);
    int CountGraphs(int axis);    
    void EraseUnused();

    map<int, CtrlID> m_IDMap;
    map<int, int*> m_graphMap202;
    map<int, LiveGraphInfo*> m_graphMap308;

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedTestCh1Y1();
    afx_msg void OnBnClickedTestCh2Y1();
    afx_msg void OnBnClickedTestCh3Y1();
    afx_msg void OnBnClickedTestCh4Y1();
    afx_msg void OnBnClickedTestCh5Y1();
    afx_msg void OnBnClickedTestCh6Y1();
    afx_msg void OnBnClickedTestCh7Y1();
    afx_msg void OnBnClickedTestCh8Y1();
    afx_msg void OnBnClickedTestCh9Y1();
    afx_msg void OnBnClickedTestCh10Y1();
    afx_msg void OnBnClickedTestCh11Y1();
    afx_msg void OnBnClickedTestCh12Y1();
    afx_msg void OnBnClickedTestCh1Y2();
    afx_msg void OnBnClickedTestCh2Y2();
    afx_msg void OnBnClickedTestCh3Y2();
    afx_msg void OnBnClickedTestCh4Y2();
    afx_msg void OnBnClickedTestCh5Y2();
    afx_msg void OnBnClickedTestCh6Y2();
    afx_msg void OnBnClickedTestCh7Y2();
    afx_msg void OnBnClickedTestCh8Y2();
    afx_msg void OnBnClickedTestCh9Y2();
    afx_msg void OnBnClickedTestCh10Y2();
    afx_msg void OnBnClickedTestCh11Y2();
    afx_msg void OnBnClickedTestCh12Y2();
    afx_msg void OnBnClickedSetupAxes();
	afx_msg void OnBnClickedFilterOn();
	afx_msg void OnBnClickedFilterOff();
	afx_msg void OnEnKillfocusFilterTimeConstant();
	afx_msg void OnEnKillfocusSampleFrequency();
    afx_msg void OnCbnSelchangeDauFrequency();
    afx_msg void OnBnClickedShow();
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnBnClickedBrowse();
    afx_msg void OnEnKillfocusFileName();
	afx_msg void OnBnClickedDataStorageOn();
	afx_msg void OnBnClickedDataStorageOff();
    afx_msg void OnBnClickedZeroToRange();
    afx_msg void OnBnClickedPlusMinusRange();
};

inline
BOOL CSetupLiveDataGraphDlg::IsFirstTime( void ) const {
  return m_FirstTime;
}

inline
void CSetupLiveDataGraphDlg::SetFirstTime( BOOL firstTime ) {
    m_FirstTime = firstTime;
}
