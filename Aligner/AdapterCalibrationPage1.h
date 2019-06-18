// $Id: AdapterCalibrationPage1.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class AdapterCalibrationPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage1)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage1( void ) ;
	virtual ~AdapterCalibrationPage1( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_1 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
private:
	BOOL InitSensorAdapter( void ) ;
public:
	CComboBox m_sensorAdapterControl;
	CString m_sensorAdapterString;
} ;
