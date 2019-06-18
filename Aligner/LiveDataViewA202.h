// $Id: LiveDataViewA202.h,v 1.4 2012/07/17 09:33:08 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "AlignerControls.h"
#include "DataRecepient.h"
#include "Defines.h"
#include "ResultTable.h"
#include "ConfigGrid.h"

// LiveDataViewA202 form view

class LiveDataViewA202 : public CFormView, public DataRecepient
{
public:
	DECLARE_DYNCREATE(LiveDataViewA202)
    DECLARE_MESSAGE_MAP()

	enum { IDD = IDD_LIVE_DATA_FORM_A202 };

public:
	LiveDataViewA202();      
	virtual ~LiveDataViewA202();
    void UpdateData( void ) ;
    void UpdateText( void ) ;
    void SetShowOnlySelected( BOOL flag ) ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    virtual void OnInitialUpdate( void ) ;

    afx_msg LRESULT OnNewData( WPARAM, LPARAM ) ;
    afx_msg LRESULT OnDauError( WPARAM, LPARAM );
    afx_msg void OnDestroy();
    afx_msg void OnCbnSelchangeDauFrequency();
    afx_msg void OnBnClickedLiveDataClose();
    afx_msg void OnBnClickedZeroToRange();
    afx_msg void OnBnClickedPlusMinusRange();

private:
	void UpdateSyncroData( void ) ;
    void ResetFilters( void ) ;
    void InitFrequencyControl( void ) ;
    void SetSyncroChannelState(int ch, BOOL connected);

    WhiteBackgroundEdit m_syncroValue[DAU_MAX_NB_SYNCRO_CHANNELS] ;
	WhiteBackgroundEdit m_syncroDescription[DAU_MAX_NB_SYNCRO_CHANNELS] ;
    
    CStatic m_syncroHeader [DAU_MAX_NB_SYNCRO_CHANNELS] ;    
    CButton m_sdError ;
    CComboBox m_frequencyControl ;
    BOOL m_showOnlySelected ;
    int m_frequency ; // TODO: Kludge, please refactor.
    LiveDataA202Param m_liveDataA202Param[DAU_MAX_NB_SYNCRO_CHANNELS];
    int m_dataFreqMode;
    map<CString, int> m_indexMap;

    ChannelLiveGrid m_channelLiveGrid;
    ChannelLiveDigGrid m_channelLiveDigGrid;

    BOOL m_newDataArrived;
    int m_timer;
    void StoreLiveDataToFile(CString name);
    CString MakeStringLength(CString str, int length, char fillCh=' ');

    CString m_liveDataDir;
    CString m_liveDataFilename;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
    afx_msg void OnBnClickedLiveDataSavePrint();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);    
    afx_msg void OnTimer(UINT nIDEvent);
};


