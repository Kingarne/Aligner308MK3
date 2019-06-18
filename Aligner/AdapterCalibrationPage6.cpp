// $Id: AdapterCalibrationPage6.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AdapterCalibrationPage6.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage6, CPropertyPage)

AdapterCalibrationPage6::AdapterCalibrationPage6() : CPropertyPage(AdapterCalibrationPage6::IDD)
{
  // Empty
}

AdapterCalibrationPage6::~AdapterCalibrationPage6( void )
{
  // Empty
}

void AdapterCalibrationPage6::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage6, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage6::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage6::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return __super::OnSetActive() ;
}

LRESULT AdapterCalibrationPage6::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return __super::OnWizardNext() ;
}

BOOL AdapterCalibrationPage6::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}
