// $Id: GainAzimuthCalibration.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainAzimuthCalibration.h"

#define DISPLAY_INSTRUCTIONS 1
#define PERFORM_MEASUREMENT  2

IMPLEMENT_DYNAMIC(GainAzimuthCalibration, CalibrationSheet)

GainAzimuthCalibration::GainAzimuthCalibration( UINT nIDCaption, BOOL sensorValidation ) : CalibrationSheet( nIDCaption, CalibrationSheet::ScaleCalibration )
{
	if (sensorValidation)
	{
		AddPage(&m_page0);
		AddPage(&m_page1);
		AddPage(&m_page2);
		AddPage(&m_page3);
		AddPage(&m_page4);
		AddPage(&m_page6);
	}
	else
	{
		AddPage(&m_page0);
		AddPage(&m_page1);
		AddPage(&m_page2);
		AddPage(&m_page3);
		AddPage(&m_page4);
		AddPage(&m_page5);
	}
}

GainAzimuthCalibration::~GainAzimuthCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(GainAzimuthCalibration, CalibrationSheet)
END_MESSAGE_MAP()

BOOL GainAzimuthCalibration::OnInitDialog( void )
{
  return __super::OnInitDialog() ;
}
