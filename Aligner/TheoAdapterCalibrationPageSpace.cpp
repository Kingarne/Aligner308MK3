// TheoAdapterCalibrationPageSpace.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "TheoAdapterCalibrationPageSpace.h"
#include ".\TheoAdapterCalibrationPageSpace.h"
#include "CalibrationSheet.h"
#include "util.h"


// TheoAdapterCalibrationPageSpace dialog

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPageSpace, CPropertyPage)
TheoAdapterCalibrationPageSpace::TheoAdapterCalibrationPageSpace()
	: CPropertyPage(TheoAdapterCalibrationPageSpace::IDD)
{
}

TheoAdapterCalibrationPageSpace::~TheoAdapterCalibrationPageSpace()
{
}

void TheoAdapterCalibrationPageSpace::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPageSpace, CPropertyPage)
END_MESSAGE_MAP()


// TheoAdapterCalibrationPageSpace message handlers

BOOL TheoAdapterCalibrationPageSpace::OnSetActive()
{
    // TODO: Add your specialized code here and/or call the base class
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    
	if(pSheet->m_backPressed)
	{
		pSheet->m_backPressed = false;
		pSheet->PressButton( PSBTN_BACK ) ;
		return CPropertyPage::OnSetActive();
	}
	
	pSheet->SetWizardButtons( 0 ) ;

	//pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;

    MoveDlgToRightSideOfApp(pSheet);

    return CPropertyPage::OnSetActive();
}
