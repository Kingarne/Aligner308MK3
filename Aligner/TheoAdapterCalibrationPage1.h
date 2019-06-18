// $Id: TheoAdapterCalibrationPage1.h,v 1.1 2014-07-11 09:28:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TheoAdapterCalibrationPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPage1)
	DECLARE_MESSAGE_MAP()
public:
	TheoAdapterCalibrationPage1( void ) ;
	virtual ~TheoAdapterCalibrationPage1( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_1 };
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
