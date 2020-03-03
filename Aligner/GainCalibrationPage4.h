// $Id: GainCalibrationPage4.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class GainCalibrationPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage4)
	DECLARE_MESSAGE_MAP()
public:
	GainCalibrationPage4( void ) ;
	virtual ~GainCalibrationPage4( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_4 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;
	BOOL inProgress;

	void ShowWindow(int id, int status);
	LRESULT StartMeasure(void);
	void StopTimer();
	void StartTimer();
	int m_timer;

	afx_msg void OnTimer(UINT_PTR nIDEvent);


public:
	afx_msg void OnBnClickedBreakButton();
} ;
