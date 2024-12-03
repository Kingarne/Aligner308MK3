// $Id: TheoAdapterCalibrationPage2.cpp,v 1.3 2014-10-17 07:35:20 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "TheoAdapterCalibrationPage2.h"
#include "CalibrationSheet.h"
#include "GlobalData.h"
#include "util.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPage2, CPropertyPage)

TheoAdapterCalibrationPage2::TheoAdapterCalibrationPage2() : CPropertyPage(TheoAdapterCalibrationPage2::IDD)
{
	inProgress = false;
}

TheoAdapterCalibrationPage2::~TheoAdapterCalibrationPage2( void )
{
  // Empty
}

void TheoAdapterCalibrationPage2::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPage2, CPropertyPage)
END_MESSAGE_MAP()

BOOL TheoAdapterCalibrationPage2::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL TheoAdapterCalibrationPage2::OnSetActive( void )
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

LRESULT TheoAdapterCalibrationPage2::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

	inProgress = true;
	pSheet->SetWizardButtons( 0 ) ;
	pSheet->ShowBarGraph( FALSE );
	ShowGUI( SW_HIDE );
	theApp.SwitchView( static_cast<CGraphView *>(theApp.m_pGraphView) ) ;
	pSheet -> StartMeasurement( FALSE ) ;
	return -1;//Dont do NEXT yet.
}

BOOL TheoAdapterCalibrationPage2::OnQueryCancel( void )
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

LRESULT TheoAdapterCalibrationPage2::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> ShowBarGraph( FALSE );
  return __super::OnWizardBack() ;
}

void TheoAdapterCalibrationPage2::ShowGUI( int cmdShow )
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( cmdShow );
 // GetDlgItem( IDC_IMAGE_1 )->ShowWindow( cmdShow );
}
