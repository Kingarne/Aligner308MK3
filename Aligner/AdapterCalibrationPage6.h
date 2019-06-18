// $Id: AdapterCalibrationPage6.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class AdapterCalibrationPage6 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage6)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage6( void ) ;
	virtual ~AdapterCalibrationPage6( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_6 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
} ;
