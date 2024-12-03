// $Id: TiltCalibrationPage2.cpp,v 1.4 2014-07-15 13:22:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltCalibrationPage2.h"
#include "CalibrationSheet.h"
#include "util.h"

IMPLEMENT_DYNAMIC(TiltCalibrationPage2, CPropertyPage)

TiltCalibrationPage2::TiltCalibrationPage2( void ): CPropertyPage(TiltCalibrationPage2::IDD)
{
  // Empty
}

TiltCalibrationPage2::~TiltCalibrationPage2( void )
{
  // Empty
}

void TiltCalibrationPage2::DoDataExchange(CDataExchange* pDX)
{
  __super::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_OFFSET_CALIBRATION_TEXT, m_text);
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage2, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage2::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet->PlotConverted( 0 ) ;
    pSheet->m_measurement = 1 ;
    pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT ) ;
    CString text ;
    text.Format( IDS_TILT_CALIBRATION_FORMAT_PAGE_2, pSheet->m_referenceData.m_pSource->GetSerialNumber() ) ;
    m_text.SetWindowText( text ) ;
    
    MoveDlgToRightSideOfApp(pSheet);
    inProgress = false;

	return __super::OnSetActive() ;
}

LRESULT TiltCalibrationPage2::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	
	inProgress = true;
	pSheet->SetWizardButtons( 0 ) ;
	pSheet->StartMeasurement( TRUE ) ;
	return -1 ;
}

BOOL TiltCalibrationPage2::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet->StopMeasurement() ;
  pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT TiltCalibrationPage2::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet->IsMeasuring())
  {
    pSheet->StopMeasurement() ;
    pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}
