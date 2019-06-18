// $Id: TiltOffsetCalibration.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "CalibrationSheet.h"
#include "TiltCalibrationPage0.h"
#include "TiltCalibrationPage1.h"
#include "TiltCalibrationPage2.h"
#include "TiltCalibrationPage3.h"
#include "TiltCalibrationPage4.h"

class TiltOffsetCalibration : public CalibrationSheet
{
	DECLARE_DYNAMIC(TiltOffsetCalibration)
public:
	TiltOffsetCalibration( UINT nIDCaption ) ;
	virtual ~TiltOffsetCalibration( void ) ;
protected:
	DECLARE_MESSAGE_MAP()
protected:
  virtual BOOL OnInitDialog( void ) ;
private:
  TiltCalibrationPage0 m_page0 ;
  TiltCalibrationPage1 m_page1 ;
  TiltCalibrationPage2 m_page2 ;
  TiltCalibrationPage3 m_page3 ;
  TiltCalibrationPage4 m_page4 ;
} ;
