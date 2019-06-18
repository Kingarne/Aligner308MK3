// $Id: AzimuthVerificationWizard.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "AlignVerBenchMarkPage1.h"
#include "AlignVerBenchMarkPage2.h"

// CAlignVerWizard

class CAzimuthVerificationWizard : public CAlignmentWizard
{
  friend class CAlignVerBenchMarkPage1;
  friend class CAlignVerBenchMarkPage2;

	DECLARE_DYNAMIC(CAzimuthVerificationWizard)

public:
  CAzimuthVerificationWizard( BOOL EnableFixPlane = TRUE, CWnd* pWndParent = NULL );
	virtual ~CAzimuthVerificationWizard();
  CAlignVerBenchMarkPage2 m_AlignVerBenchMarkPage2;

private:
  BOOL OnInitDialog();

  CAlignVerBenchMarkPage1 m_AlignVerBenchMarkPage1;
  //CAlignVerBenchMarkPage2 m_AlignVerBenchMarkPage2;
 
protected:
	DECLARE_MESSAGE_MAP()
};
