// $Id: GainCalibrationPage3.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class GainCalibrationPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage3)
	DECLARE_MESSAGE_MAP()
public:
	GainCalibrationPage3( void ) ;
	virtual ~GainCalibrationPage3( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_3 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;
private:
	CStatic m_text ;

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
