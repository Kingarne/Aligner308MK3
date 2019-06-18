// $Id: DAUCalibrationPage3.cpp,v 1.3 2014-07-15 13:22:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "DAUCalibrationPage3.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage3, CPropertyPage)

DAUCalibrationPage3::DAUCalibrationPage3( void ) : CPropertyPage(DAUCalibrationPage3::IDD)
, m_voltage(0)
{
}

DAUCalibrationPage3::~DAUCalibrationPage3( void )
{
}

void DAUCalibrationPage3::DoDataExchange(CDataExchange* pDX)
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  CPropertyPage::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_TEXT, m_text) ;
  DDX_Text(pDX, IDC_VOLTAGE, m_voltage) ;
  DDV_MinMaxDouble(pDX, m_voltage, pSheet -> m_voltage_ref - 0.2, pSheet -> m_voltage_ref + 0.2) ;
}

BOOL DAUCalibrationPage3::OnSetActive( void )
{
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT ) ;
	pSheet -> InitVoltage( pSheet -> m_voltage_ref ) ;
	CString text ; 
	text.Format( _T("The voltage is set to appr +%3.1f [V] DC. Enter the exact value measured by an accurate volt meter."), pSheet -> m_voltage_ref ) ;
	m_text.SetWindowText( text ) ;
	pSheet -> m_measurement = 0 ;
  	inProgress = false;
	return __super::OnSetActive() ;
}

LRESULT DAUCalibrationPage3::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	if (UpdateData( TRUE ))
	{
		inProgress = true;
		pSheet->m_voltage_0 = m_voltage ;
		pSheet->SetWizardButtons( 0 ) ;
		pSheet->StartMeasurement( m_voltage ) ;
	}
	
	return -1 ;
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage3, CPropertyPage)
END_MESSAGE_MAP()
