// $Id: SystemConfigurationView.h,v 1.5 2012/06/01 13:37:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "AlignerControls.h"
#include "ConfigGrid.h"
#include "ColorStatic.h"
// SystemConfigurationView form view

class SystemConfigurationView : public CFormView
{
	DECLARE_DYNCREATE(SystemConfigurationView)
	DECLARE_MESSAGE_MAP()

//protected:
public:
	SystemConfigurationView();           // protected constructor used by dynamic creation
	virtual ~SystemConfigurationView();

public:
	enum { IDD_A308 = IDD_SYSTEM_CONFIGURATION_FORM };
    enum { IDD_A202 = IDD_SYSTEM_CONFIGURATION_FORM_A202 };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void ShowAll( int nCmdShow );
	void ShowGeneralConfiguration( int nCmdShow );
	void ShowSensorConfiguration( int nCmdShow );
	void ShowSyncroConfiguration( int nCmdShow );
	void ShowInfo(int show);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void UpdateGeneralConfiguration( void );
	void UpdateSensorConfiguration( void );
	void UpdateSyncroConfiguration( void );
	void UpdateSyncro(CString DName, CString JName, int index);

private:
    WhiteBackgroundEdit m_GeneralProject;
	WhiteBackgroundEdit m_GeneralShip;
	WhiteBackgroundEdit m_GeneralOperator;
	WhiteBackgroundEdit m_GeneralDauSN;
	WhiteBackgroundEdit m_GeneralDate;
	WhiteBackgroundEdit m_GeneralPlace;
	WhiteBackgroundEdit m_GeneralLatitude;
    WhiteBackgroundEdit m_GeneralUnit;
    UINT_PTR m_timerId ;

//    WhiteBackgroundEdit m_SyncroNominalAzimuth[4];
	
    SensorGrid m_sensorGrid;
    GyroGrid m_gyroGrid;
    ChannelGrid m_channelGrid;
		
		CFont m_font;
		WhiteBackgroundEdit m_descBox1;
		CStatic m_desc1;
		WhiteBackgroundEdit m_descBox2;
		CStatic m_desc2;
		WhiteBackgroundEdit m_descBox3;
		CStatic m_desc3;
		

public:
	virtual void OnInitialUpdate();
    
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    //void SetSensorChannelState(int ch, BOOL connected);
    //void SetSyncroChannelState(int ch, BOOL connected);

public:
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


