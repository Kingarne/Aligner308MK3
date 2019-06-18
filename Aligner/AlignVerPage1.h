// $Id: AlignVerPage1.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CAlignmentWizard ;

class CAlignVerPage1 : public CPropertyPage
{
  friend class CAlignVerWizard;
  friend class CAlignVerHorizonPage1;

	DECLARE_DYNAMIC(CAlignVerPage1)

public:
	CAlignVerPage1();
	virtual ~CAlignVerPage1();

// Dialog Data
//	enum { IDD = IDD_ALIGN_VER_PAGE_1 };

private:
  CAlignmentWizard *m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  int m_SelectedTest;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardNext();
  void OnReset();
  void ShowPlatformTestSetup();
  void HideAllText();

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedAlignVerSetupTest();
  afx_msg void OnBnClickedAlignVerHorizonRadio();
  afx_msg void OnBnClickedAlignVerBenchmarkRadio();
};
