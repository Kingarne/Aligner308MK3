// $Id: AzimuthAlignWizard.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "AzimuthAlignPage1.h"
#include "AlignVerBenchMarkPage2.h"

// CAzimuthAlignWizard

class CAzimuthAlignWizard : public CAlignmentWizard
{
  friend class CAzimuthAlignPage1;
  friend class CAlignVerBenchMarkPage2;

	DECLARE_DYNAMIC(CAzimuthAlignWizard)

public:
  CAzimuthAlignWizard( CWnd* pWndParent = NULL );
	virtual ~CAzimuthAlignWizard();

  CAlignVerBenchMarkPage2 m_AlignVerBenchMarkPage2;
private:
  BOOL OnInitDialog();

  CAzimuthAlignPage1 m_AzimuthAlignPage1;
  //CAlignVerBenchMarkPage2 m_AlignVerBenchMarkPage2;

protected:
	DECLARE_MESSAGE_MAP()
};
