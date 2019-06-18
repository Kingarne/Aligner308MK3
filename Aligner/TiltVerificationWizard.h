// $Id: TiltVerificationWizard.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "AlignVerHorizonPage1.h"
#include "AlignVerHorizonPage2.h"

// CTiltVerificationWizard

class CTiltVerificationWizard : public CAlignmentWizard
{
  friend class CAlignVerHorizonPage1;
  friend class CAlignVerHorizonPage2;

	DECLARE_DYNAMIC(CTiltVerificationWizard)

public:
  CTiltVerificationWizard( BOOL EnableReference = FALSE, CWnd* pWndParent = NULL );
	virtual ~CTiltVerificationWizard();

private:
  BOOL OnInitDialog();

  CAlignVerHorizonPage1 m_AlignVerHorizonPage1;
  CAlignVerHorizonPage2 m_AlignVerHorizonPage2;

protected:
	DECLARE_MESSAGE_MAP()
};
