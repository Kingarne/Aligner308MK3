// $Id: TiltFlatnessTestPage3.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltFlatnessTestWizard ;

class CTiltFlatnessTestPage3 : public CPropertyPage
{
  friend class CTiltFlatnessTestWizard;
  friend class CTiltFlatnessTestPage1;
  friend class CTiltFlatnessTestPage2;

  DECLARE_DYNAMIC(CTiltFlatnessTestPage3)

private:
  CTiltFlatnessTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_AzimuthAngle;
  CString m_TimeStamp;
  CString m_DateStamp;
	BOOL m_Repeat;

public:
	CTiltFlatnessTestPage3();
	virtual ~CTiltFlatnessTestPage3();

// Dialog Data
	enum { IDD = IDD_TILT_FLATNESS_TEST_PAGE_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardBack();
  BOOL OnWizardFinish();
  void OnReset();
  void MeasureRollPathInit();
  void MeasureRollPathContinue();
  void RestoreGlobals();
  //void EnableStartButton( int textID );
  void EnableStartButton();
  BOOL CallMeasure( double *pRoll, double *pPitch );
  void HideAllText();
  void HideAll();
  void ShowGraphButtons();
  void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );
	BOOL IsAlreadyMeasured( double newAngle );

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedStartMeasure();
  afx_msg void OnEnKillfocusTiltFlatnessTestPage3AzimuthAngle();
  afx_msg void OnBnClickedShowPolarGraph();
  afx_msg void OnBnClickedShowErrorGraph();
  afx_msg void OnBnClickedShowResultGraph();
  afx_msg void OnBnClickedShowResultTable();
  afx_msg void OnBnClickedPrintGraph();
  afx_msg void OnBnClickedSaveGraph();
public:
	afx_msg void OnBnClickedFinishMeasure();
};
