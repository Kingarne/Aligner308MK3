// $Id: TiltFlatnessTestPage1.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltFlatnessTestWizard ;

class CTiltFlatnessTestPage1 : public CPropertyPage
{
  friend class CTiltFlatnessTestWizard;
  friend class CTiltFlatnessTestPage2;
  friend class CTiltFlatnessTestPage3;

	DECLARE_DYNAMIC(CTiltFlatnessTestPage1)

public:
	CTiltFlatnessTestPage1();
	virtual ~CTiltFlatnessTestPage1();

// Dialog Data
	enum { IDD = IDD_TILT_FLATNESS_TEST_PAGE_1 };

private:
  CTiltFlatnessTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_FilterTimeConstant;
	CButton m_SperryActiveBox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardNext();
  void OnReset();
  void ShowSetup();
  BOOL IsPlaneOrGunSelected();
	void ShowSettingsForAxes();
	void EnableDisableSperry();

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedTiltFlatnessTestSetupMeasurement();
  afx_msg void OnEnKillfocusTiltFlatnessTestFilterTimeConstant();
  afx_msg void OnBnClickedSetupAxes();
};
