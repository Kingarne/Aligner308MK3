// $Id: AzimuthAlignWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignmentWizard.h"
#include "AzimuthAlignWizard.h"


// CAzimuthAlignWizard

IMPLEMENT_DYNAMIC(CAzimuthAlignWizard, CAlignmentWizard)

CAzimuthAlignWizard::CAzimuthAlignWizard( CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
  m_AzimuthAlignPage1.m_pPSP->dwFlags |= PSP_USETITLE;
	m_AlignVerBenchMarkPage2.m_pPSP->dwFlags |= PSP_USETITLE;
  m_AzimuthAlignPage1.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_ALIGN_PAGE_CAPTION;
	m_AlignVerBenchMarkPage2.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_ALIGN_PAGE_CAPTION;

  AddPage( &m_AzimuthAlignPage1 );
  AddPage( &m_AlignVerBenchMarkPage2 );
}

CAzimuthAlignWizard::~CAzimuthAlignWizard()
{
  // Empty
}

BEGIN_MESSAGE_MAP(CAzimuthAlignWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CAzimuthAlignWizard message handlers

BOOL CAzimuthAlignWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( IDS_AZIMUTH_ALIGN_PAGE_CAPTION );
  m_AziMode = NormAzi;

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}
