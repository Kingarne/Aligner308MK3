// $Id: TiltFlatnessFoundationTestPage2.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltFlatnessTestWizard ;

class CTiltFlatnessFoundationTestPage2 : public CPropertyPage
{
  friend class CTiltFlatnessTestWizard;
  friend class CTiltFlatnessFoundationTestPage1;
  friend class CTiltFlatnessTestPage3;

  DECLARE_DYNAMIC(CTiltFlatnessFoundationTestPage2)

private:
  CTiltFlatnessTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;

public:
	CTiltFlatnessFoundationTestPage2();
	virtual ~CTiltFlatnessFoundationTestPage2();

// Dialog Data
	enum { IDD = IDD_TILT_FLATNESS_FOUNDATION_TEST_PAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardBack();
  LRESULT OnWizardNext();
  void OnReset();
  void InitMeasureParam();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedIntroText();
	afx_msg void OnStnClickedText3();
};
