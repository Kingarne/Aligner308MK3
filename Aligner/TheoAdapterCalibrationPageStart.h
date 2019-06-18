// $Id: TheoAdapterCalibrationPageStart.h,v 1.1 2014-07-11 09:28:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TheoAdapterCalibrationPageStart : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPageStart)
	DECLARE_MESSAGE_MAP()
public:
	TheoAdapterCalibrationPageStart( void ) ;
	virtual ~TheoAdapterCalibrationPageStart( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_START };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
private:
  BOOL InitReference( void ) ;
public:
  
} ;
