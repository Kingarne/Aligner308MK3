// $Id: TiltCalibrationPage1.h,v 1.2 2014-07-15 13:22:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TiltCalibrationPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(TiltCalibrationPage1)
	DECLARE_MESSAGE_MAP()
public:
	TiltCalibrationPage1( void ) ;
	virtual ~TiltCalibrationPage1( void );
	enum { IDD = IDD_TILT_CALIBRATION_PAGE_1 };

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel();
	virtual LRESULT OnWizardBack( void ) ;
	virtual LRESULT OnWizardNext();
private:
	BOOL InitSensor( void ) ;
	CComboBox m_sensor [3] ;

	bool inProgress;

} ;
