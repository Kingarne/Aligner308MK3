// $Id: AdapterCalibrationPage8.cpp,v 1.3 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AdapterCalibrationPage8.h"
#include "CalibrationSheet.h"
//#include "LiveDataView.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage8, CPropertyPage)

AdapterCalibrationPage8::AdapterCalibrationPage8() : CPropertyPage(AdapterCalibrationPage8::IDD)
{
  // Empty
}

AdapterCalibrationPage8::~AdapterCalibrationPage8( void )
{
  // Empty
}

void AdapterCalibrationPage8::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage8, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage8::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage8::OnSetActive( void )
{
    ShowGUI( SW_SHOW );
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet -> m_measurement = 3 ;
    //theApp.SwitchView( theApp.m_pLiveDataView ) ;
    //static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetHandleError( FALSE ) ;
    BOOL result = TRUE;
    if( pSheet -> ShowBarGraph( TRUE, -100, 0 ) == FALSE )
    {
        result = FALSE;
    }
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

	if( __super::OnSetActive() == FALSE )
    {
        result = FALSE;
    }

    MoveDlgToRightSideOfApp(pSheet);
	inProgress = false;

    return( result );
}

LRESULT AdapterCalibrationPage8::OnWizardNext( void )
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
	pSheet -> ShowBarGraph( FALSE );
	ShowGUI( SW_HIDE );
	theApp.SwitchView( static_cast<CGraphView *>(theApp.m_pGraphView) ) ;
	pSheet -> StartMeasurement( FALSE ) ;
	return -1 ;
}

LRESULT AdapterCalibrationPage8::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet -> IsMeasuring())
  {
    pSheet -> StopMeasurement() ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }

  int result = ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
  if( result == IDYES )
  {
    pSheet -> ShowBarGraph( FALSE );
    return( 0 );
  }
  return( -1 ); 
}


BOOL AdapterCalibrationPage8::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet -> IsMeasuring())
  {
    pSheet -> StopMeasurement() ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  }
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

void AdapterCalibrationPage8::ShowGUI( int cmdShow )
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( cmdShow );
  GetDlgItem( IDC_IMAGE_1 )->ShowWindow( cmdShow );
}
