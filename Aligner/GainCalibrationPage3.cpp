// $Id: GainCalibrationPage3.cpp,v 1.3 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainCalibrationPage3.h"
#include "GainAzimuthCalibration.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage3, CPropertyPage)

GainCalibrationPage3::GainCalibrationPage3( void ) : CPropertyPage(GainCalibrationPage3::IDD)
{
  // empty
}

GainCalibrationPage3::~GainCalibrationPage3( void )
{
  // Empty
}

void GainCalibrationPage3::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GAIN_TEXT, m_text) ;
}

BEGIN_MESSAGE_MAP(GainCalibrationPage3, CPropertyPage)
END_MESSAGE_MAP()

BOOL GainCalibrationPage3::OnSetActive( void )
{
    GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> PlotConverted( 0 ) ;
    pSheet -> m_measurement = 2 ;
    pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT ) ;
    CString text ;
    text.Format( IDS_GAIN_CALIBRATION_FORMAT_PAGE_3, pSheet -> m_sensorData.begin() -> m_pSource -> GetSerialNumber() ) ;
    m_text.SetWindowText( text ) ;
    if (!pSheet -> VerifyStability( 0, 1 ))
    {
        ::AfxMessageBox( _T("The calibration fixture has moved more than 1 mrad between measurements\nPress the back button to redo the last measurment") ) ;
        pSheet -> SetWizardButtons( PSWIZB_BACK ) ;
    }    
	inProgress = false;
    return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage3::OnWizardNext( void )
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
	pSheet -> StartMeasurement( FALSE, - pSheet -> m_alpha / 2.0, 0.0, TRUE ) ;
	return -1 ;
}

BOOL GainCalibrationPage3::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage3::OnWizardBack( void )
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
