// $Id: AdapterCalibrationPage2.cpp,v 1.3 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AdapterCalibrationPage2.h"
#include "CalibrationSheet.h"
#include "GlobalData.h"
//#include "LiveDataView.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage2, CPropertyPage)

AdapterCalibrationPage2::AdapterCalibrationPage2() : CPropertyPage(AdapterCalibrationPage2::IDD)
{
  // Empty
}

AdapterCalibrationPage2::~AdapterCalibrationPage2( void )
{
  // Empty
}

void AdapterCalibrationPage2::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage2, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage2::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage2::OnSetActive( void )
{      
    ShowGUI( SW_SHOW );
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    //theApp.SwitchView( theApp.m_pLiveDataView ) ;
    //static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetHandleError( FALSE ) ;
    BOOL result = TRUE;
    if( pSheet -> ShowBarGraph( TRUE, -50, 50 ) == FALSE )
    {
        result = FALSE;
    }
    pSheet -> m_measurement = 0 ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

	if( __super::OnSetActive() == FALSE )
    {
        result = FALSE;
    }
    
    MoveDlgToRightSideOfApp(pSheet);
    inProgress = false;

    return( result );
}

LRESULT AdapterCalibrationPage2::OnWizardNext( void )
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

BOOL AdapterCalibrationPage2::OnQueryCancel( void )
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

LRESULT AdapterCalibrationPage2::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> ShowBarGraph( FALSE );
  return __super::OnWizardBack() ;
}

void AdapterCalibrationPage2::ShowGUI( int cmdShow )
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( cmdShow );
  GetDlgItem( IDC_TEXT_2 )->ShowWindow( cmdShow );
  GetDlgItem( IDC_IMAGE_1 )->ShowWindow( cmdShow );
}
