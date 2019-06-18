// $Id: TiltOffsetCalibration.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "TiltOffsetCalibration.h"

IMPLEMENT_DYNAMIC(TiltOffsetCalibration, CalibrationSheet)

TiltOffsetCalibration::TiltOffsetCalibration( UINT nIDCaption ) : CalibrationSheet( nIDCaption, CalibrationSheet::OffsetCalibration )
{
  AddPage( &m_page0 ) ;
  AddPage( &m_page1 ) ;
  AddPage( &m_page2 ) ;
  AddPage( &m_page3 ) ;
  AddPage( &m_page4 ) ;
}

TiltOffsetCalibration::~TiltOffsetCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(TiltOffsetCalibration, CalibrationSheet)
END_MESSAGE_MAP()

BOOL TiltOffsetCalibration::OnInitDialog( void )
{
  return __super::OnInitDialog() ;
}
