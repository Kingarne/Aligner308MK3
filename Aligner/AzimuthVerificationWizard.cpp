// $Id: AzimuthVerificationWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignmentWizard.h"
#include "AzimuthVerificationWizard.h"

// CAzimuthVerificationWizard

IMPLEMENT_DYNAMIC(CAzimuthVerificationWizard, CAlignmentWizard)

CAzimuthVerificationWizard::CAzimuthVerificationWizard( BOOL EnableFixPlane /*= TRUE*/, CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
  m_EnableFixPlane = EnableFixPlane;
  m_AlignVerBenchMarkPage1.m_pPSP->dwFlags |= PSP_USETITLE;
	m_AlignVerBenchMarkPage2.m_pPSP->dwFlags |= PSP_USETITLE;

  if( m_EnableFixPlane == TRUE )
  {
    m_AlignVerBenchMarkPage1.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_VERIFICATION_BENCHMARK_TEST;
		m_AlignVerBenchMarkPage2.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_VERIFICATION_BENCHMARK_TEST;
  }
  else
  {
    m_AlignVerBenchMarkPage1.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_VERIFICATION_GYROSTAB_TEST;
		m_AlignVerBenchMarkPage2.m_pPSP->pszTitle = (LPCSTR)IDS_AZIMUTH_VERIFICATION_GYROSTAB_TEST;
  }

  AddPage( &m_AlignVerBenchMarkPage1 );
  AddPage( &m_AlignVerBenchMarkPage2 );
}

CAzimuthVerificationWizard::~CAzimuthVerificationWizard()
{
  // Empty
}

BEGIN_MESSAGE_MAP(CAzimuthVerificationWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CAzimuthVerificationWizard message handlers

BOOL CAzimuthVerificationWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( IDS_TILT_ALIGNMENT );
  m_AziMode = NormAzi;

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}
