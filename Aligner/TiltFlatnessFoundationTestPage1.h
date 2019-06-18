// $Id: TiltFlatnessFoundationTestPage1.h,v 1.2 2013/10/01 13:25:11 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltFlatnessTestWizard ;

class CTiltFlatnessFoundationTestPage1 : public CPropertyPage
{
  friend class CTiltFlatnessTestWizard;
  friend class CTiltFlatnessTestPage2;
  friend class CTiltFlatnessTestPage3;

	DECLARE_DYNAMIC(CTiltFlatnessFoundationTestPage1)

public:
	CTiltFlatnessFoundationTestPage1();
	virtual ~CTiltFlatnessFoundationTestPage1();

// Dialog Data
	enum { IDD = IDD_TILT_FLATNESS_FOUNDATION_TEST_PAGE_1 };

private:
  CTiltFlatnessTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_FilterTimeConstant;
	CButton m_SperryActiveBox;
    
    int m_armLength1;
    int m_armLength2;
    double m_refAngle;
  //int m_LengthCh[SIZE_OF_ARRAYS];
  //double m_AngleCh[SIZE_OF_ARRAYS];
//  int m_AngleCh[SIZE_OF_ARRAYS];


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
