// $Id: DAUCalibrationPage1.h,v 1.2 2014-07-15 13:22:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class DAUCalibrationPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(DAUCalibrationPage1)
	DECLARE_MESSAGE_MAP()
public:
	DAUCalibrationPage1( void ) ;
	virtual ~DAUCalibrationPage1( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_1 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual LRESULT OnWizardNext( void ) ;

	BOOL inProgress;
} ;
