// $Id: TiltCalibrationPage3.h,v 1.2 2014-07-15 13:22:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TiltCalibrationPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(TiltCalibrationPage3)
	DECLARE_MESSAGE_MAP()

public:
	TiltCalibrationPage3( void ) ;
	virtual ~TiltCalibrationPage3( void ) ;
	enum { IDD = IDD_TILT_CALIBRATION_PAGE_3 } ;

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;

	bool inProgress;
} ;
