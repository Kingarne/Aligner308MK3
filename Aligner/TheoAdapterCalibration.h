// $Id: TheoAdapterCalibration.h,v 1.2 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "CalibrationSheet.h"
#include "TheoAdapterCalibrationPageStart.h"
#include "TheoAdapterCalibrationPage0.h"
#include "TheoAdapterCalibrationPage1.h"
#include "TheoAdapterCalibrationPage2.h"
#include "TheoAdapterCalibrationPage3.h"
#include "TheoAdapterCalibrationPageSpace.h"
#include "TheoAdapterCalibrationPageFinish.h"

class TheoAdapterCalibration : public CalibrationSheet
{
	DECLARE_DYNAMIC(TheoAdapterCalibration)
public:
	TheoAdapterCalibration( UINT nIDCaption ) ;
	virtual ~TheoAdapterCalibration( void ) ;
protected:
	DECLARE_MESSAGE_MAP()

  virtual BOOL OnInitDialog( void ) ;

private:

	TheoAdapterCalibrationPageStart m_pageStart;
	TheoAdapterCalibrationPage0 m_page0;
	TheoAdapterCalibrationPage1 m_page1;
	TheoAdapterCalibrationPage2 m_page2;
	//TheoAdapterCalibrationPageSpace m_pageSpace;
	TheoAdapterCalibrationPage3 m_page3;
	TheoAdapterCalibrationPageFinish m_pageFin;
} ;
