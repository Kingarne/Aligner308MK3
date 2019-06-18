#pragma once

#include "AlignmentWizard.h"
#include "AlignVerPage1.h"
#include "AlignVerHorizonPage1.h"
#include "AlignVerHorizonPage2.h"
#include "AlignVerBenchMarkPage1.h"
#include "AlignVerBenchMarkPage2.h"

// CAlignVerWizard

class CAlignVerWizard : public CAlignmentWizard
{
  friend class CAlignVerPage1;
  friend class CAlignVerHorizonPage1;
  friend class CAlignVerHorizonPage2;
  friend class CAlignVerBenchMarkPage1;
  friend class CAlignVerBenchMarkPage2;

	DECLARE_DYNAMIC(CAlignVerWizard)

public:
  CAlignVerWizard( CWnd* pWndParent = NULL );
	virtual ~CAlignVerWizard();

  BOOL OnInitDialog();

  CAlignVerPage1 m_AlignVerPage1;
  CAlignVerHorizonPage1 m_AlignVerHorizonPage1;
  CAlignVerHorizonPage2 m_AlignVerHorizonPage2;
  CAlignVerBenchMarkPage1 m_AlignVerBenchMarkPage1;
  CAlignVerBenchMarkPage2 m_AlignVerBenchMarkPage2;

protected:
	DECLARE_MESSAGE_MAP()
};
