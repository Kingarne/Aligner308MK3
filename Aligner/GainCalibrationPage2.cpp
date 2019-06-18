// $Id: GainCalibrationPage2.cpp,v 1.3 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainCalibrationPage2.h"
#include "GainAzimuthCalibration.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage2, CPropertyPage)

GainCalibrationPage2::GainCalibrationPage2( void )
  : CPropertyPage(GainCalibrationPage2::IDD)
{
  // Empty
}

GainCalibrationPage2::~GainCalibrationPage2( void )
{
  // Empty
}

void GainCalibrationPage2::DoDataExchange( CDataExchange *pDX )
{
	CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(GainCalibrationPage2, CPropertyPage)
END_MESSAGE_MAP()

BOOL GainCalibrationPage2::OnSetActive( void )
{
    GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> PlotConverted( 0 ) ;
    pSheet -> m_measurement = 1 ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;    
    inProgress = false;
	return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage2::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	inProgress = true;
	pSheet -> SetWizardButtons( 0 ) ;
	pSheet -> StartMeasurement( FALSE, 0.0, - pSheet -> m_alpha / 2.0, TRUE ) ;
	return -1 ;
}

BOOL GainCalibrationPage2::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage2::OnWizardBack()
{
  GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
  if (pSheet -> IsMeasuring())
  {
    pSheet -> StopMeasurement() ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}
