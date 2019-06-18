// $Id: TiltVerificationWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignmentWizard.h"
#include "TiltVerificationWizard.h"

// CTiltVerificationWizard

IMPLEMENT_DYNAMIC(CTiltVerificationWizard, CAlignmentWizard)

CTiltVerificationWizard::CTiltVerificationWizard( BOOL EnableReference /*= FALSE*/, CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
  m_EnableReference = EnableReference;
  m_AlignVerHorizonPage1.m_pPSP->dwFlags |= PSP_USETITLE;
	m_AlignVerHorizonPage2.m_pPSP->dwFlags |= PSP_USETITLE;

  if( m_EnableReference == FALSE )
  {
    m_AlignVerHorizonPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_VERIFICATION_HORIZON_TEST;
		m_AlignVerHorizonPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_VERIFICATION_HORIZON_TEST;
  }
  else
  {
    m_AlignVerHorizonPage1.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_VERIFICATION_AIR_TARGET_TEST;
		m_AlignVerHorizonPage2.m_pPSP->pszTitle = (LPCSTR)IDS_TILT_VERIFICATION_AIR_TARGET_TEST;
  }

  AddPage( &m_AlignVerHorizonPage1 );
  AddPage( &m_AlignVerHorizonPage2 );
}

CTiltVerificationWizard::~CTiltVerificationWizard()
{
  // Empty
}

BEGIN_MESSAGE_MAP(CTiltVerificationWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CTiltVerificationWizard message handlers

BOOL CTiltVerificationWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( IDS_TILT_ALIGNMENT );
  m_AziMode = NormAzi;

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}

