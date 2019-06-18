// $Id: AdapterCalibrationPage0.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class AdapterCalibrationPage0 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage0)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage0( void ) ;
	virtual ~AdapterCalibrationPage0( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_0 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
private:
  BOOL InitReference( void ) ;
public:
  CComboBox m_reference ;
  double m_timeConstant;
} ;
