// $Id: GainCalibrationPage2.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class GainCalibrationPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage2)
	DECLARE_MESSAGE_MAP()
public:
	GainCalibrationPage2( void ) ;
	virtual ~GainCalibrationPage2( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_2 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;
	BOOL inProgress;
} ;

