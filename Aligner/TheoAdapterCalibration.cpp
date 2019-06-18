// $Id: TheoAdapterCalibration.cpp,v 1.2 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "TheoAdapterCalibration.h"


IMPLEMENT_DYNAMIC(TheoAdapterCalibration, CalibrationSheet)

TheoAdapterCalibration::TheoAdapterCalibration( UINT nIDCaption ) : CalibrationSheet( nIDCaption, CalibrationSheet::TheoAdapterCalibration )
{
	AddPage( &m_pageStart);
	AddPage( &m_page0 ) ;
	AddPage( &m_page1 ) ;
	AddPage( &m_page2 ) ;
	//AddPage( &m_pageSpace ) ;
	AddPage( &m_page3 ) ;
	AddPage( &m_pageFin ) ;
}

TheoAdapterCalibration::~TheoAdapterCalibration( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibration, CalibrationSheet)
//	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL TheoAdapterCalibration::OnInitDialog( void )
{
  return __super::OnInitDialog() ;
}
