// $Id: TiltAlignmentWizard.cpp,v 1.3 2011/10/13 10:09:12 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltAlignmentWizard.h"

IMPLEMENT_DYNAMIC(CTiltAlignmentWizard, CAlignmentWizard)

CTiltAlignmentWizard::CTiltAlignmentWizard( BOOL commonFlatTest /*= FALSE*/, CWnd* pWndParent ): CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
	m_TiltAlignPage1.m_pPSP->dwFlags |= PSP_USETITLE;
	m_TiltAlignPage2.m_pPSP->dwFlags |= PSP_USETITLE;

    //m_TiltAlignPage2.m_pPSP->dwFlags &= ~PSP_HASHELP;

	if( commonFlatTest == TRUE )
	{
		m_TiltAlignPage1.m_pPSP->pszTitle = (LPCSTR)IDS_COMMON_FLAT_TEST_PAGE_CAPTION;
		m_TiltAlignPage2.m_pPSP->pszTitle = (LPCSTR)IDS_COMMON_FLAT_TEST_PAGE_CAPTION;
	}
	else
	{
		m_TiltAlignPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_ALIGNMENT_PAGE_CAPTION;
		m_TiltAlignPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_ALIGNMENT_PAGE_CAPTION;
	}

    AddPage( &m_TiltAlignPage1 );
	AddPage( &m_TiltAlignPage2 );
    m_CommonFlatTest = commonFlatTest;
    DAU::GetDAU().SetIgnoreElevation( commonFlatTest ) ;
}

CTiltAlignmentWizard::~CTiltAlignmentWizard()
{
    DAU::GetDAU().SetIgnoreElevation( FALSE ) ;
}

BEGIN_MESSAGE_MAP(CTiltAlignmentWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CTiltAlignmentWizard message handlers

BOOL CTiltAlignmentWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( (m_CommonFlatTest == TRUE) ? IDS_COMMON_FLAT_TEST : IDS_TILT_ALIGNMENT );
  
  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}
