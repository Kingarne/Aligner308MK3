// $Id: AlignVerBenchMarkPage1.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CAlignmentWizard ;

class CAlignVerBenchMarkPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignVerBenchMarkPage1)

private:
  CAlignmentWizard *m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_FilterTimeConstant;
  BOOL m_EnableZPar;
  BOOL m_EnableReference;
	CButton m_SperryActiveBox;

public:
	CAlignVerBenchMarkPage1();
	virtual ~CAlignVerBenchMarkPage1();

// Dialog Data
	enum { IDD = IDD_ALIGN_VER_BENCHMARK_PAGE_1 };

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
  afx_msg void OnEnKillfocusAlignVerFilterTimeConstant();
  afx_msg void OnBnClickedSetupAxes();
	afx_msg void OnBnClickedAlignVerSetupTest();
};
