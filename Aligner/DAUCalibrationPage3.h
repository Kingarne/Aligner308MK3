// $Id: DAUCalibrationPage3.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
#include "afxwin.h"

class DAUCalibrationPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(DAUCalibrationPage3)
	DECLARE_MESSAGE_MAP()
public:
	DAUCalibrationPage3( void ) ;
	virtual ~DAUCalibrationPage3( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_3 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
public:
	CStatic m_text;
	double m_voltage;

	BOOL inProgress;
} ;
