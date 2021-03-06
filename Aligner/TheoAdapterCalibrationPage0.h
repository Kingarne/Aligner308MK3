// $Id: TheoAdapterCalibrationPage0.h,v 1.1 2014-07-11 09:28:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TheoAdapterCalibrationPage0 : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPage0)
	DECLARE_MESSAGE_MAP()
public:
	TheoAdapterCalibrationPage0( void ) ;
	virtual ~TheoAdapterCalibrationPage0( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_0 };
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
