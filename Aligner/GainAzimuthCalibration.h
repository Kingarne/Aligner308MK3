// $Id: GainAzimuthCalibration.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "CalibrationSheet.h"
#include "GainCalibrationPage0.h"
#include "GainCalibrationPage1.h"
#include "GainCalibrationPage2.h"
#include "GainCalibrationPage3.h"
#include "GainCalibrationPage4.h"
#include "GainCalibrationPage5.h"
#include "GainCalibrationPage6.h"

struct GainAzimuthCalibrationData {
  Sensor *m_pSource ;
  double m_roll [4]  ;
  double m_pitch [4] ;
  double m_temperature [4] ;
public:
  GainAzimuthCalibrationData( Sensor *pSource = NULL ) {
    m_pSource = pSource ;
  }
} ;

class GainAzimuthCalibration : public CalibrationSheet
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(GainAzimuthCalibration)
public:
	GainAzimuthCalibration(UINT nIDCaption, BOOL sensorValidation=TRUE) ;
	virtual ~GainAzimuthCalibration( void ) ;
protected:
  virtual BOOL OnInitDialog( void ) ;
private:
  GainCalibrationPage0 m_page0 ;
  GainCalibrationPage1 m_page1 ;
  GainCalibrationPage2 m_page2 ;
  GainCalibrationPage3 m_page3 ;
  GainCalibrationPage4 m_page4 ;
  GainCalibrationPage5 m_page5;
  GainCalibrationPage6 m_page6 ;
} ;
