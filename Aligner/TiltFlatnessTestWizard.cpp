// $Id: TiltFlatnessTestWizard.cpp,v 1.3 2013/09/25 11:47:40 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltFlatnessTestWizard.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(CTiltFlatnessTestWizard, CAlignmentWizard)

CTiltFlatnessTestWizard::CTiltFlatnessTestWizard( CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
    m_TiltFlatnessTestPage1.m_pPSP->dwFlags |= PSP_USETITLE;
    m_TiltFlatnessTestPage2.m_pPSP->dwFlags |= PSP_USETITLE;
    m_TiltFlatnessTestPage3.m_pPSP->dwFlags |= PSP_USETITLE;

    m_TiltFlatnessFoundationTestPage1.m_pPSP->dwFlags |= PSP_USETITLE;
    m_TiltFlatnessFoundationTestPage2.m_pPSP->dwFlags |= PSP_USETITLE;
    m_TiltFlatnessFoundationTestPage3.m_pPSP->dwFlags |= PSP_USETITLE;


	//ALIGNMENT_MODE currentAlignmentMode;
	//currentAlignmentMode = theApp.GetAlignmentMode();
    // if( g_AlignerData.Found == TRUE )
    if (theApp.GetAlignmentMode() == TiltFlatnesstestFoundationMode)
    {
		g_AlignerData.Found = TRUE ;
        m_TiltFlatnessFoundationTestPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;
        m_TiltFlatnessFoundationTestPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;
        m_TiltFlatnessFoundationTestPage3.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;
        m_TiltFlatnessTestPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;
        m_TiltFlatnessTestPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;
        m_TiltFlatnessTestPage3.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE_CAPTION;

		    //	This is the order !!!
        AddPage( &m_TiltFlatnessFoundationTestPage1 );
		    AddPage( &m_TiltFlatnessFoundationTestPage2 );
        AddPage( &m_TiltFlatnessFoundationTestPage3 );

    }
    else
    {
		g_AlignerData.Found = FALSE ;
        m_TiltFlatnessTestPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_PLATFORM_TEST_PAGE_CAPTION;
        m_TiltFlatnessTestPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_PLATFORM_TEST_PAGE_CAPTION;
        m_TiltFlatnessTestPage3.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_FLATNESS_PLATFORM_TEST_PAGE_CAPTION;
        AddPage( &m_TiltFlatnessTestPage1 );
        AddPage( &m_TiltFlatnessTestPage2 );
        AddPage( &m_TiltFlatnessTestPage3 );
    }


}

CTiltFlatnessTestWizard::~CTiltFlatnessTestWizard()
{
}

BEGIN_MESSAGE_MAP(CTiltFlatnessTestWizard, CAlignmentWizard)
END_MESSAGE_MAP()


// CTiltFlatnessTestWizard message handlers
void CTiltFlatnessTestWizard::ChangeType()
{
  RemovePage(2);
  if (g_AlignerData.FoundationType == FoundationT::Rectangular)
  {    
    AddPage(&m_TiltFlatnessFoundationTestPage4);
  }
  else
  {
    AddPage(&m_TiltFlatnessFoundationTestPage3);
  }
  
}


BOOL CTiltFlatnessTestWizard::OnInitDialog()
{
  if( g_AlignerData.Found == TRUE )
  {
    g_AlignerData.AlignMode.LoadString( IDS_TILT_FLATNESS_FOUNDATION_TEST );
  }
  else
  {
    g_AlignerData.AlignMode.LoadString( IDS_TILT_FLATNESS_PLATFORM_TEST );
  }

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}


