// $Id: TheoAdapterCalibrationPageStart.cpp,v 1.1 2014-07-11 09:28:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TheoAdapterCalibrationPageStart.h"
#include "CalibrationSheet.h"
#include "util.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPageStart, CPropertyPage)

TheoAdapterCalibrationPageStart::TheoAdapterCalibrationPageStart() : CPropertyPage(TheoAdapterCalibrationPageStart::IDD)
{
  // Empty
}

TheoAdapterCalibrationPageStart::~TheoAdapterCalibrationPageStart( void )
{
  // Empty
}

void TheoAdapterCalibrationPageStart::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
  
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPageStart, CPropertyPage)
	
END_MESSAGE_MAP()
 
BOOL TheoAdapterCalibrationPageStart::InitReference( void )
{
 
	return TRUE ;
}

BOOL TheoAdapterCalibrationPageStart::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  if (!InitReference())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  
  return focus ;
}

BOOL TheoAdapterCalibrationPageStart::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;  
  
  MoveDlgToRightSideOfApp(pSheet);
  
  return __super::OnSetActive() ;
}

LRESULT TheoAdapterCalibrationPageStart::OnWizardNext( void )
{
 
  return __super::OnWizardNext() ;
}


