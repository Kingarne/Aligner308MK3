// $Id: DAUCalibrationWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "DAUCalibrationWizard.h"
#include "UserMessages.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(DAUOffsetCalibration, CalibrationSheet)

DAUOffsetCalibration::DAUOffsetCalibration( void ) : CalibrationSheet( IDS_DAU_OFFSET_CALIBRATION_CAPTION, CalibrationSheet::DAUOffsetCalibration )
{
  AddPage( &m_page_0 ) ;
  AddPage( &m_page_1 ) ;
  AddPage( &m_page_2 ) ;
}

DAUOffsetCalibration::~DAUOffsetCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(DAUOffsetCalibration, CalibrationSheet)
END_MESSAGE_MAP()

BOOL DAUOffsetCalibration::OnInitDialog( void )
{
  BOOL selected = __super::OnInitDialog() ;
  return selected ;
}

IMPLEMENT_DYNAMIC(DAUScaleCalibration, CalibrationSheet)

DAUScaleCalibration::DAUScaleCalibration( void ) : CalibrationSheet( IDS_DAU_GAIN_CAL_PAGE_CAPTION, CalibrationSheet::DAUScaleCalibration )
{
  AddPage( &m_page_0 ) ;
  AddPage( &m_page_3 ) ;
  AddPage( &m_page_4 ) ;
  AddPage( &m_page_5 ) ;
  AddPage( &m_page_6 ) ;
  AddPage( &m_page_7 ) ;
  m_voltage_ref = 9.8 ;
  if (0 < DAU::GetDAU().GetSensorCount() && DAU::GetDAU().GetSensor( 0 ) -> m_invertPitch)
  {
    m_voltage_ref = 4.8 ;
  }
  m_page_3.m_voltage = m_voltage_ref ;
  m_page_5.m_voltage = -m_voltage_ref ;
}

DAUScaleCalibration::~DAUScaleCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(DAUScaleCalibration, CalibrationSheet)
END_MESSAGE_MAP()

BOOL DAUScaleCalibration::OnInitDialog( void )
{
  BOOL selected = __super::OnInitDialog() ;
  return selected ;
}
