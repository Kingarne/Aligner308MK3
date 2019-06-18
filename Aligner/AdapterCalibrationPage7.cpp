// $Id: AdapterCalibrationPage7.cpp,v 1.3 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AdapterCalibrationPage7.h"
#include "CalibrationSheet.h"
//#include "LiveDataView.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage7, CPropertyPage)

AdapterCalibrationPage7::AdapterCalibrationPage7() : CPropertyPage(AdapterCalibrationPage7::IDD)
{
  // Empty
}

AdapterCalibrationPage7::~AdapterCalibrationPage7( void )
{
  // Empty
}

void AdapterCalibrationPage7::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage7, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage7::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage7::OnSetActive( void )
{
    ShowGUI( SW_SHOW );
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    //theApp.SwitchView( theApp.m_pLiveDataView ) ;
    //static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetHandleError( FALSE ) ;
    BOOL result = TRUE;
    if( pSheet -> ShowBarGraph( TRUE, 0, 100 ) == FALSE )
    {
        result = FALSE;
    }
    pSheet -> m_measurement = 2 ;
	    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

	    if( __super::OnSetActive() == FALSE )
    {
        result = FALSE;
    }

    MoveDlgToRightSideOfApp(pSheet);
	inProgress = false;

    return( result );
}

LRESULT AdapterCalibrationPage7::OnWizardNext( void )
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

BOOL AdapterCalibrationPage7::OnQueryCancel( void )
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

LRESULT AdapterCalibrationPage7::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> ShowBarGraph( FALSE );
  return __super::OnWizardBack() ;
}

void AdapterCalibrationPage7::ShowGUI( int cmdShow )
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( cmdShow );
  GetDlgItem( IDC_IMAGE_1 )->ShowWindow( cmdShow );
}
