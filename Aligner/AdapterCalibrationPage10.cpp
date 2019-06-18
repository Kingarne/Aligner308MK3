// AdapterCalibrationPage10.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "AdapterCalibrationPage10.h"
#include ".\adaptercalibrationpage10.h"
#include "CalibrationSheet.h"


// AdapterCalibrationPage10 dialog

IMPLEMENT_DYNAMIC(AdapterCalibrationPage10, CPropertyPage)
AdapterCalibrationPage10::AdapterCalibrationPage10()
	: CPropertyPage(AdapterCalibrationPage10::IDD)
{
}

AdapterCalibrationPage10::~AdapterCalibrationPage10()
{
}

void AdapterCalibrationPage10::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AdapterCalibrationPage10, CPropertyPage)
END_MESSAGE_MAP()


// AdapterCalibrationPage10 message handlers

BOOL AdapterCalibrationPage10::OnSetActive()
{
    // TODO: Add your specialized code here and/or call the base class
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

    MoveDlgToRightSideOfApp(pSheet);

    return CPropertyPage::OnSetActive();
}
