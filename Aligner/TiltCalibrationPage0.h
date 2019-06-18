// $Id: TiltCalibrationPage0.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class TiltCalibrationPage0 : public CPropertyPage
{
	DECLARE_DYNAMIC(TiltCalibrationPage0)
	DECLARE_MESSAGE_MAP()
public:
	TiltCalibrationPage0( void ) ;
	virtual ~TiltCalibrationPage0( void ) ;
private:
	enum { IDD = IDD_TILT_CALIBRATION_PAGE_0 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
private:
  BOOL InitReference( void ) ;
private:
  CComboBox m_reference;
} ;
