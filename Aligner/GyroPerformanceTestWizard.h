// $Id: GyroPerformanceTestWizard.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "GyroPerformanceTestPage1.h"
#include "GyroPerformanceTestPage2.h"

class CGyroPerformanceTestWizard : public CAlignmentWizard
{
	DECLARE_DYNAMIC(CGyroPerformanceTestWizard)

public:
  CGyroPerformanceTestWizard( CWnd* pWndParent = NULL );
	virtual ~CGyroPerformanceTestWizard();

private:
  BOOL OnInitDialog();

  CGyroPerformanceTestPage1 m_GyroPerformanceTestPage1;
  CGyroPerformanceTestPage2 m_GyroPerformanceTestPage2;

protected:
	DECLARE_MESSAGE_MAP()
};
