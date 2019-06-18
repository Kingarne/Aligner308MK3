// $Id: AdapterCalibration.h,v 1.3 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "CalibrationSheet.h"
#include "AdapterCalibrationPage0.h"
#include "AdapterCalibrationPage1.h"
#include "AdapterCalibrationPage2.h"
#include "AdapterCalibrationPage3.h"
#include "AdapterCalibrationPage4.h"
#include "AdapterCalibrationPage7.h"
#include "AdapterCalibrationPage8.h"
#include "AdapterCalibrationPage9.h"
#include "AdapterCalibrationPage10.h"

class AdapterCalibration : public CalibrationSheet
{
	DECLARE_DYNAMIC(AdapterCalibration)
public:
	AdapterCalibration( UINT nIDCaption ) ;
	virtual ~AdapterCalibration( void ) ;
protected:
	DECLARE_MESSAGE_MAP()
protected:
  virtual BOOL OnInitDialog( void ) ;
private:
  AdapterCalibrationPage0 m_page0 ;
  AdapterCalibrationPage1 m_page1 ;
  AdapterCalibrationPage2 m_page2 ;
//  AdapterCalibrationPage3 m_page3 ;
  AdapterCalibrationPage4 m_page4 ;
//  AdapterCalibrationPage3 m_page5 ;
  AdapterCalibrationPage7 m_page7 ;
  AdapterCalibrationPage8 m_page8 ;
  AdapterCalibrationPage9 m_page9 ;
 // AdapterCalibrationPage10 m_page10 ;
 // AdapterCalibrationPage10 m_page11 ;
  AdapterCalibrationPage10 m_page12 ;
  AdapterCalibrationPage10 m_page13 ;
public:
//	afx_msg void OnDestroy();
} ;
