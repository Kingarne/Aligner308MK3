// $Id: TheoAdapterCalibrationPage3.h,v 1.2 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TheoAdapterCalibrationPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPage3)
	DECLARE_MESSAGE_MAP()
public:
	TheoAdapterCalibrationPage3( void ) ;
	virtual ~TheoAdapterCalibrationPage3( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_3 };
	void ShowGUI( int cmdShow );
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnInitDialog( void ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;

	bool inProgress;
} ;
