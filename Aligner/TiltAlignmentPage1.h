// $Id: TiltAlignmentPage1.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltAlignmentPage1 : public CPropertyPage
{
  friend class CTiltAlignmentWizard;
  friend class CTiltAlignmentPage2;

	DECLARE_DYNAMIC(CTiltAlignmentPage1)

public:
	CTiltAlignmentPage1();
	virtual ~CTiltAlignmentPage1();

// Dialog Data
	enum { IDD = IDD_TILT_ALIGNMENT_PAGE_1 };

private:
  CTiltAlignmentWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  CString m_LOSDirection;
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
  BOOL IsOnlyFixedSelected();
  //BOOL IsGunPresent();
  void ShowLOSDirectionSelection();
  void HideLOSDirectionSelection();
  void ShowSettingsForAxes();
	void EnableDisableSperry();
  void HideSperry();

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedTilt1SetupMeasurement();
  afx_msg void OnEnKillfocusTilt1FilterTimeConstant();
  afx_msg void OnBnClickedTilt1LosDir0180();
  afx_msg void OnBnClickedTilt1LosDir9090();
  afx_msg void OnBnClickedSetupAxes();
};
