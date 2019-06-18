// $Id: GainCalibrationPage4.cpp,v 1.3 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainCalibrationPage4.h"
#include "GainAzimuthCalibration.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage4, CPropertyPage)

GainCalibrationPage4::GainCalibrationPage4( void ): CPropertyPage(GainCalibrationPage4::IDD)
{
  // Empty
}

GainCalibrationPage4::~GainCalibrationPage4( void )
{
  // Empty
}

void GainCalibrationPage4::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(GainCalibrationPage4, CPropertyPage)
END_MESSAGE_MAP()

BOOL GainCalibrationPage4::OnSetActive( void )
{
    GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> PlotConverted( 1 ) ;
    pSheet -> m_measurement = 3 ;
    pSheet -> SetWizardButtons( PSWIZB_BACK |  PSWIZB_NEXT ) ;
    inProgress = false;
    return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage4::OnWizardNext( void )
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
	pSheet -> StartMeasurement( FALSE, pSheet -> m_alpha / 2.0, 0.0, TRUE ) ;
	return -1 ;
}

BOOL GainCalibrationPage4::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage4::OnWizardBack( void )
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
