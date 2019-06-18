// $Id: GyroPerformanceTestPage1.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CGyroPerformanceTestWizard ;

class CGyroPerformanceTestPage1 : public CPropertyPage
{
  friend class CGyroPerformanceTestWizard;

	DECLARE_DYNAMIC(CGyroPerformanceTestPage1)

private:
  CGyroPerformanceTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_FilterTimeConstant;
	CButton m_SperryActiveBox;

public:
	CGyroPerformanceTestPage1();
	virtual ~CGyroPerformanceTestPage1();

// Dialog Data
	enum { IDD = IDD_GYRO_PERFORMANCE_TEST_PAGE_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardNext();
  void OnReset();
  void ShowSetup();
	void ShowSettingsForAxes();
	void EnableDisableSperry();

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnEnKillfocusFilterTimeConstant();
  afx_msg void OnBnClickedSetupAxes();
  afx_msg void OnBnClickedSetupDrift();
};
