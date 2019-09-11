// $Id: LiveDataView.h,v 1.4 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "DataRecepient.h"
#include "AlignerControls.h"
#include "DataRecepient.h"
#include "Defines.h"
#include "afxwin.h"

enum ViewMode { ViewCalibrated, ViewUncalibrated, ViewRaw } ;

[
  event_receiver(native)
]
class CLiveDataView : public CFormView, public DataRecepient
{
public:
	DECLARE_DYNCREATE(CLiveDataView)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_LIVE_DATA_FORM } ;
public:
	CLiveDataView( void ) ; 
	virtual ~CLiveDataView( void ) ;
public:
    void UpdateData( void ) ;
    void UpdateText( void ) ;
    void SetViewMode( ViewMode viewMode );
    void SetFilterTypeAndTau( double tau );
    void SetShowOnlySelected( BOOL flag ) ;
    void ResetFilters( double factor ) ;
    void LiveRestart( BOOL showMessages );
    void LiveClose( void );
    void TestVoltageAccept( void );

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    virtual void OnInitialUpdate( void ) ;
protected:
    afx_msg LRESULT OnNewData( WPARAM, LPARAM ) ;
    afx_msg LRESULT OnDauError( WPARAM, LPARAM );
    afx_msg void OnBnClickedCalibrated( void ) ;
    afx_msg void OnBnClickedUncalibrated( void ) ;
    afx_msg void OnBnClickedRaw();
    afx_msg void OnDestroy();
    afx_msg void OnCbnSelchangeFilterType();
    afx_msg void OnCbnSelchangeDauFrequency();
    afx_msg void OnEnChangeFilterConstant() ;
    afx_msg void OnBnClickedApplyTau() ;
    afx_msg void OnEnKillfocusFilterConstant();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnBnClickedTestVoltageAccept() ;
    afx_msg void OnBnClickedLiveRestart();
    afx_msg void OnBnClickedLiveClose();
private:
    void UpdateSensorData( void ) ;
	void UpdateSyncroData( void ) ;
    void UpdateMainboardData( void ) ;
    void UpdateMeasuredTestVoltage( void ) ;
	void SetTauLimits( void );
    void InitFrequencyControl( void ) ;
	BOOL IsTauOK( void );
    void SetSensorChannelState(int ch, BOOL connected);
    void SetSyncroChannelState(int ch, BOOL connected);
private:
    WhiteBackgroundEdit m_sensorRoll [DAU_MAX_NB_SENSOR_CHANNELS] ;
    WhiteBackgroundEdit m_sensorPitch [DAU_MAX_NB_SENSOR_CHANNELS] ;
    WhiteBackgroundEdit m_sensorTemp [DAU_MAX_NB_SENSOR_CHANNELS] ;    
    WhiteBackgroundEdit m_sensorDescription [DAU_MAX_NB_SENSOR_CHANNELS] ;
    WhiteBackgroundEdit m_syncroRoll [DAU_MAX_NB_SYNCRO_CHANNELS] ;
    WhiteBackgroundEdit m_syncroPitch [DAU_MAX_NB_SYNCRO_CHANNELS] ;
	WhiteBackgroundEdit m_syncroHeading[DAU_MAX_NB_SYNCRO_CHANNELS];
    WhiteBackgroundEdit m_syncroDescription [DAU_MAX_NB_SYNCRO_CHANNELS] ;
    WhiteBackgroundEdit m_mainBoardTemperature ;
    
    CButton m_sdError ;
    CStatic m_syncroHeader [DAU_MAX_NB_SYNCRO_CHANNELS] ;
    CStatic m_sensorHeader [DAU_MAX_NB_SENSOR_CHANNELS] ;
    CComboBox m_frequencyControl ;
    ViewMode m_viewMode ;
    CComboBox m_filterType;
    CEdit m_filterConstant;
    double m_minTau ;
    double m_maxTau ;
    BOOL m_showOnlySelected ;
    CEdit m_testVoltageControl ;
    CButton m_testVoltageAcceptControl ;
    WhiteBackgroundEdit m_measuredTestVoltageControl;
    int m_timer;
    BOOL m_newDataArrived;

	void DrawValue(CWnd* pWnd, CString valtxt, bool clearOnly=false);

public:
    int m_frequency ; // TODO: Kludge, please refactor.
    int m_dataFreqMode;  

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    afx_msg void OnBnClickedSyncroChannel4Header();
    afx_msg void OnTimer(UINT nIDEvent);
} ;
