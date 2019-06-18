// $Id: AzimuthAlignPage1.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

#define RUN_SVT  100
#define RUN_GATC 101

class CAzimuthAlignWizard ;

class CAzimuthAlignPage1 : public CPropertyPage
{
  friend class CAzimuthAlignWizard;

	DECLARE_DYNAMIC(CAzimuthAlignPage1)

private:
  CAzimuthAlignWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_FilterTimeConstant;
	CButton m_SperryActiveBox;

public:
	CAzimuthAlignPage1();
	virtual ~CAzimuthAlignPage1();

// Dialog Data
	enum { IDD = IDD_AZIMUTH_ALIGN_PAGE_1 };

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
  afx_msg void OnBnClickedSetupMeasurement();
};
