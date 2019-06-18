// $Id: GyroPerformanceTestWizard.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignmentWizard.h"
#include "GyroPerformanceTestWizard.h"


// CGyroPerformanceTestWizard

IMPLEMENT_DYNAMIC(CGyroPerformanceTestWizard, CAlignmentWizard)

CGyroPerformanceTestWizard::CGyroPerformanceTestWizard( CWnd* pWndParent )
  :CAlignmentWizard( AFX_IDS_APP_TITLE, pWndParent )
{
  m_GyroPerformanceTestPage1.m_pPSP->dwFlags |= PSP_USETITLE;
	m_GyroPerformanceTestPage2.m_pPSP->dwFlags |= PSP_USETITLE;
  m_GyroPerformanceTestPage1.m_pPSP->pszTitle = (LPCSTR)IDS_GYRO_PERFORMANCE_TEST;
	m_GyroPerformanceTestPage2.m_pPSP->pszTitle = (LPCSTR)IDS_GYRO_PERFORMANCE_TEST;

  AddPage( &m_GyroPerformanceTestPage1 );
  AddPage( &m_GyroPerformanceTestPage2 );
}

CGyroPerformanceTestWizard::~CGyroPerformanceTestWizard()
{
  // Empty
}

BEGIN_MESSAGE_MAP(CGyroPerformanceTestWizard, CAlignmentWizard)
END_MESSAGE_MAP()

// CGyroPerformanceTestWizard message handlers

BOOL CGyroPerformanceTestWizard::OnInitDialog()
{
  g_AlignerData.AlignMode.LoadString( IDS_GYRO_PERFORMANCE_TEST );

  if (!__super::OnInitDialog())
  {
    return FALSE ;
  }
  return TRUE ;
}
