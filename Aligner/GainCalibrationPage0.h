// $Id: GainCalibrationPage0.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002-2003

#pragma once

#include "resource.h"

class GainCalibrationPage0 : public CPropertyPage
{
  DECLARE_DYNAMIC(GainCalibrationPage0)
  DECLARE_MESSAGE_MAP()
public:
  GainCalibrationPage0( void ) ;
  virtual ~GainCalibrationPage0( void ) ;
private:
  enum { IDD = IDD_GAIN_CALIBRATION_PAGE_0 } ;
protected:
  virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
protected:
  afx_msg void OnCbnSelchangeTableSerialNumber( void ) ;
private:
  BOOL InitReference( void ) ;
  BOOL InitTable( void ) ;
private:
  CComboBox m_reference ;
  CComboBox m_table ;

  vector<Platform> m_platforms;
} ;
