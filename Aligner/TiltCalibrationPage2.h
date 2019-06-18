// $Id: TiltCalibrationPage2.h,v 1.2 2014-07-15 13:22:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TiltCalibrationPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(TiltCalibrationPage2)
	DECLARE_MESSAGE_MAP()

public:
	TiltCalibrationPage2( void ) ;
	virtual ~TiltCalibrationPage2( void ) ;
	enum { IDD = IDD_TILT_CALIBRATION_PAGE_2 } ;

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;

public:
	CStatic m_text ;
	bool inProgress;
} ;

