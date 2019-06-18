// $Id: DAUCalibrationWizard.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "DataRecepient.h"
#include "CalibrationSheet.h"
#include "DAUCalibrationPage0.h"
#include "DAUCalibrationPage1.h"
#include "DAUCalibrationPage2.h"
#include "DAUCalibrationPage3.h"
#include "DAUCalibrationPage4.h"
#include "DAUCalibrationPage5.h"
#include "DAUCalibrationPage6.h"
#include "DAUCalibrationPage7.h"

class DAUOffsetCalibration : public CalibrationSheet
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(DAUOffsetCalibration)
public:
    DAUOffsetCalibration( void ) ;
    virtual ~DAUOffsetCalibration( void ) ;
protected:
  BOOL OnInitDialog( void ) ;
private:
  DAUCalibrationPage0 m_page_0 ;
  DAUCalibrationPage1 m_page_1 ;
  DAUCalibrationPage2 m_page_2 ;
} ;

class DAUScaleCalibration : public CalibrationSheet
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(DAUScaleCalibration)
public:
    DAUScaleCalibration( void ) ;
    virtual ~DAUScaleCalibration( void ) ;
protected:
  BOOL OnInitDialog( void ) ;
private:
  DAUCalibrationPage0 m_page_0 ;
  DAUCalibrationPage3 m_page_3 ;
  DAUCalibrationPage4 m_page_4 ;
  DAUCalibrationPage5 m_page_5 ;
  DAUCalibrationPage6 m_page_6 ;
  DAUCalibrationPage7 m_page_7 ;
} ;
