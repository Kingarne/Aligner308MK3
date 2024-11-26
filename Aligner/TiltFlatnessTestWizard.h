// $Id: TiltFlatnessTestWizard.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignmentWizard.h"
#include "TiltFlatnessTestPage1.h"
#include "TiltFlatnessTestPage2.h"
#include "TiltFlatnessTestPage3.h"
#include "TiltFlatnessFoundationTestPage1.h"
#include "TiltFlatnessFoundationTestPage2.h"
#include "TiltFlatnessFoundationTestPage3.h"
#include "TiltFlatnessFoundationTestPage4.h"

// CTiltFlatnessTestWizard

class CTiltFlatnessTestWizard : public CAlignmentWizard
{
  friend class CAlignmentWizard;
  friend class CTiltFlatnessTestPage1;
  friend class CTiltFlatnessTestPage2;
  friend class CTiltFlatnessTestPage3;
  friend class CTiltFlatnessFoundationTestPage1 ;
  friend class CTiltFlatnessFoundationTestPage2 ;
  friend class CTiltFlatnessFoundationTestPage3 ;
  friend class CTiltFlatnessFoundationTestPage4;

	friend class CSetupFoundationMeasureDlg ;

  DECLARE_DYNAMIC(CTiltFlatnessTestWizard)


public:
  CTiltFlatnessTestWizard( CWnd* pWndParent = NULL );
	virtual ~CTiltFlatnessTestWizard();
  
  void ChangeType();

private:
  BOOL OnInitDialog();

  CTiltFlatnessTestPage1 m_TiltFlatnessTestPage1;
  CTiltFlatnessTestPage2 m_TiltFlatnessTestPage2;
  CTiltFlatnessTestPage3 m_TiltFlatnessTestPage3;
  CTiltFlatnessFoundationTestPage1 m_TiltFlatnessFoundationTestPage1;
  CTiltFlatnessFoundationTestPage2 m_TiltFlatnessFoundationTestPage2;
  CTiltFlatnessFoundationTestPage3 m_TiltFlatnessFoundationTestPage3;
  CTiltFlatnessFoundationTestPage4 m_TiltFlatnessFoundationTestPage4;
  
protected:
	DECLARE_MESSAGE_MAP()
};


