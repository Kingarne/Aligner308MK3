// $Id: TiltCalibrationPage3.cpp,v 1.3 2014-07-15 13:22:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltCalibrationPage3.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TiltCalibrationPage3, CPropertyPage)

TiltCalibrationPage3::TiltCalibrationPage3( void ) : CPropertyPage(TiltCalibrationPage3::IDD)
{
  // Empty
}

TiltCalibrationPage3::~TiltCalibrationPage3( void )
{
  // Empty
}

void TiltCalibrationPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage3, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage3::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet -> PlotConverted( 2 ) ;
    pSheet -> m_measurement = 2 ;
    pSheet -> SetWizardButtons( PSWIZB_BACK |  PSWIZB_NEXT ) ;
    MoveDlgToRightSideOfApp(pSheet);
    inProgress = false;
	return __super::OnSetActive() ;
}

LRESULT TiltCalibrationPage3::OnWizardNext( void )
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
	pSheet -> StartMeasurement() ;
	return -1 ;
}

BOOL TiltCalibrationPage3::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT TiltCalibrationPage3::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet -> IsMeasuring())
  {
    pSheet -> StopMeasurement() ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

