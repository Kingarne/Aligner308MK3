// $Id: TiltAlignmentWizard.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "TiltAlignmentPage1.h"
#include "TiltAlignmentPage2.h"

// CTiltAlignmentWizard

class CTiltAlignmentWizard : public CAlignmentWizard
{
	DECLARE_DYNAMIC(CTiltAlignmentWizard)

public:
  CTiltAlignmentWizard( BOOL commonFlatTest = FALSE, CWnd* pWndParent = NULL );
	virtual ~CTiltAlignmentWizard();

  BOOL m_CommonFlatTest;

private:
  BOOL OnInitDialog();
	//void OnDestroy();

  CTiltAlignmentPage1 m_TiltAlignPage1;
  CTiltAlignmentPage2 m_TiltAlignPage2;

protected:
	DECLARE_MESSAGE_MAP()
};


