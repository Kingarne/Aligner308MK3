// $Id: GainCalibrationPage4.cpp,v 1.3 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainCalibrationPage4.h"
#include "GainAzimuthCalibration.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage4, CPropertyPage)

GainCalibrationPage4::GainCalibrationPage4( void ): CPropertyPage(GainCalibrationPage4::IDD)
{
	m_timer=0;
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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BREAK_BUTTON, &GainCalibrationPage4::OnBnClickedBreakButton)
END_MESSAGE_MAP()

BOOL GainCalibrationPage4::OnSetActive( void )
{
	StopTimer();
	ShowWindow(IDC_WAIT_TEXT, SW_HIDE);
	ShowWindow(IDC_BREAK_BUTTON, SW_HIDE);

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
	
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent());
	ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CalibrationSheet)));
	pSheet->SetWizardButtons(PSWIZB_BACK);

	ShowWindow(IDC_WAIT_TEXT, SW_SHOW);
	ShowWindow(IDC_BREAK_BUTTON, SW_SHOW);
	StartTimer();

	return -1 ;
}

BOOL GainCalibrationPage4::OnQueryCancel( void )
{
	StopTimer();
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	pSheet -> StopMeasurement() ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
	return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage4::OnWizardBack( void )
{
	StopTimer();
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


void GainCalibrationPage4::OnBnClickedBreakButton()
{
	StartMeasure();
}

void GainCalibrationPage4::ShowWindow(int id, int status)
{
	CWnd *pWnd = GetDlgItem(id);
	pWnd->ShowWindow(status);
}

void GainCalibrationPage4::StopTimer()
{
	if (m_timer)
	{
		KillTimer(m_timer);
		m_timer = 0;
	}
}


void GainCalibrationPage4::StartTimer()
{
	if (!m_timer)
	{
		m_timer = SetTimer(1, 15000, 0);
	}
}

void GainCalibrationPage4::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timer)
	{
		StartMeasure();
	}

	CPropertyPage::OnTimer(nIDEvent);
}

LRESULT GainCalibrationPage4::StartMeasure(void)
{
	StopTimer();
	ShowWindow(IDC_BREAK_BUTTON, SW_HIDE);
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent());
	ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CalibrationSheet)));
	inProgress = true;
	pSheet->SetWizardButtons(PSWIZB_BACK);
	pSheet->StartMeasurement(FALSE, pSheet->m_alpha / 2.0, 0.0, TRUE);

	return S_OK;
}
