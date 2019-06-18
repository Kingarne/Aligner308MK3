// $Id: DAUCalibrationPage1.cpp,v 1.3 2014-07-15 13:22:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "DAUCalibrationPage1.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage1, CPropertyPage)

DAUCalibrationPage1::DAUCalibrationPage1( void ) : CPropertyPage(DAUCalibrationPage1::IDD)
{
}

DAUCalibrationPage1::~DAUCalibrationPage1( void )
{
}

void DAUCalibrationPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange( pDX ) ;
}

BOOL DAUCalibrationPage1::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT ) ;
    pSheet -> m_measurement = 0 ;
	inProgress = false;
    return __super::OnSetActive() ;
}

LRESULT DAUCalibrationPage1::OnWizardNext( void )
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
	pSheet->StartMeasurement() ;
	return -1 ;
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage1, CPropertyPage)
END_MESSAGE_MAP()
