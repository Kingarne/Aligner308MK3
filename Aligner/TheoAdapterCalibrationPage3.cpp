// $Id: TheoAdapterCalibrationPage3.cpp,v 1.3 2014-10-17 07:35:20 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "TheoAdapterCalibrationPage3.h"
#include "CalibrationSheet.h"
//#include "LiveDataView.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPage3, CPropertyPage)

TheoAdapterCalibrationPage3::TheoAdapterCalibrationPage3() : CPropertyPage(TheoAdapterCalibrationPage3::IDD)
{
  // Empty
}

TheoAdapterCalibrationPage3::~TheoAdapterCalibrationPage3( void )
{
  // Empty
}

void TheoAdapterCalibrationPage3::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPage3, CPropertyPage)
END_MESSAGE_MAP()

BOOL TheoAdapterCalibrationPage3::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL TheoAdapterCalibrationPage3::OnSetActive( void )
{
    ShowGUI( SW_SHOW );
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    //theApp.SwitchView( theApp.m_pLiveDataView ) ;
    //static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetHandleError( FALSE ) ;
    BOOL result = TRUE;
    if( pSheet -> ShowBarGraph( TRUE, -80, 80, FALSE ) == FALSE )
    {
        result = FALSE;
    }
    pSheet -> m_measurement = 1 ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

	if( __super::OnSetActive() == FALSE )
    {
        result = FALSE;
    }
    
    MoveDlgToRightSideOfApp(pSheet);
	inProgress = false;

    return( result );
}

LRESULT TheoAdapterCalibrationPage3::OnWizardNext( void )
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

BOOL TheoAdapterCalibrationPage3::OnQueryCancel( void )
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

LRESULT TheoAdapterCalibrationPage3::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> ShowBarGraph( FALSE ); 
  
  return __super::OnWizardBack() ;
}

void TheoAdapterCalibrationPage3::ShowGUI( int cmdShow )
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( cmdShow );
  //GetDlgItem( IDC_IMAGE_1 )->ShowWindow( cmdShow );
}
