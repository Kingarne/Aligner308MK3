// $Id: AlignVerWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Include.h"
#include "AlignmentWizard.h"
#include "AlignVerWizard.h"

// CAlignVerWizard

IMPLEMENT_DYNAMIC(CAlignVerWizard, CAlignmentWizard)

CAlignVerWizard::CAlignVerWizard( CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
  m_AlignVerPage1.m_pPSP->dwFlags |= PSP_USETITLE;
  m_AlignVerPage1.m_pPSP->pszTitle = (LPCSTR)IDS_ALIGN_VER_PAGE_CAPTION;
  m_AlignVerHorizonPage1.m_pPSP->dwFlags |= PSP_USETITLE;
  m_AlignVerHorizonPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_VERIFICATION_HORIZON_TEST;
  m_AlignVerBenchMarkPage1.m_pPSP->dwFlags |= PSP_USETITLE;
  m_AlignVerBenchMarkPage1.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_VERIFICATION;

  AddPage( &m_AlignVerPage1 );
  AddPage( &m_AlignVerHorizonPage1 );
  AddPage( &m_AlignVerHorizonPage2 );
  AddPage( &m_AlignVerBenchMarkPage1 );
  AddPage( &m_AlignVerBenchMarkPage2 );
}

CAlignVerWizard::~CAlignVerWizard()
{
  // Empty
}

BEGIN_MESSAGE_MAP(CAlignVerWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CAlignVerWizard message handlers

BOOL CAlignVerWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( IDS_TILT_ALIGNMENT );
  m_AziMode = NormAzi;

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}
