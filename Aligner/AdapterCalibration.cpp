// $Id: AdapterCalibration.cpp,v 1.3 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "AdapterCalibration.h"


IMPLEMENT_DYNAMIC(AdapterCalibration, CalibrationSheet)

//AdapterCalibration::AdapterCalibration( UINT nIDCaption ) : m_page3(0), m_page5(1), CalibrationSheet( nIDCaption, CalibrationSheet::AdapterCalibration )
AdapterCalibration::AdapterCalibration( UINT nIDCaption ) : CalibrationSheet( nIDCaption, CalibrationSheet::AdapterCalibration )
{
  AddPage( &m_page0 ) ;
  AddPage( &m_page1 ) ;
  AddPage( &m_page2 ) ;
  //AddPage( &m_page10 ) ;
  AddPage( &m_page4 ) ;
  //AddPage( &m_page11 ) ;
  AddPage( &m_page7 ) ;
  //AddPage( &m_page12 ) ;
  AddPage( &m_page8 ) ;
  //AddPage( &m_page13 ) ;
  AddPage( &m_page9 ) ;
}

AdapterCalibration::~AdapterCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(AdapterCalibration, CalibrationSheet)
//	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL AdapterCalibration::OnInitDialog( void )
{
  return __super::OnInitDialog() ;
}

//void AdapterCalibration::OnDestroy()
//{
//	CalibrationSheet::OnDestroy();
//
//	ClearSelectedChannels();
//}
