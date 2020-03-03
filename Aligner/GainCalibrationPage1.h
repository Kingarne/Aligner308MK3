// $Id: GainCalibrationPage1.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class GainCalibrationPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage1)
	DECLARE_MESSAGE_MAP()
public:
	GainCalibrationPage1( void ) ;
	virtual ~GainCalibrationPage1( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_1 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;
private:
	BOOL InitSensor( void ) ;
private:
	CComboBox m_sensor ;

	BOOL inProgress;
	
	void ShowWindow(int id, int status);
	LRESULT StartMeasure(void);
	void StopTimer();
	void StartTimer();
	int m_timer;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBreakButton();
} ;
