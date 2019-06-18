// $Id: DAUCalibrationPage5.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
#include "afxwin.h"

class DAUCalibrationPage5 : public CPropertyPage
{
	DECLARE_DYNAMIC(DAUCalibrationPage5)
	DECLARE_MESSAGE_MAP()
public:
	DAUCalibrationPage5( void ) ;
	virtual ~DAUCalibrationPage5( void ) ;
private:
	enum { IDD = IDD_DAU_CALIBRATION_PAGE_5 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
public:
	double m_voltage ;
private:
	CStatic m_text ;

	BOOL inProgress;

} ;
