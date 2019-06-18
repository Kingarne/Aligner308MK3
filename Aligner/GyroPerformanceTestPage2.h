// $Id: GyroPerformanceTestPage2.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CGyroPerformanceTestWizard ;

class CGyroPerformanceTestPage2 : public CPropertyPage
{
  friend class CGyroPerformanceTestWizard;
  friend class CGyroPerformanceTestPage1;

	DECLARE_DYNAMIC(CGyroPerformanceTestPage2)

  CGyroPerformanceTestWizard* m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  CString m_TimeStamp;
  CString m_DateStamp;

public:
	CGyroPerformanceTestPage2();
	virtual ~CGyroPerformanceTestPage2();

// Dialog Data
	enum { IDD = IDD_GYRO_PERFORMANCE_TEST_PAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardBack();
  BOOL OnWizardFinish();
  void OnReset();
  void GyroProc();
  void InitMeasureParam();
  //void EnableStartButton( int textID );
  void EnableStartButton();
  BOOL CallMeasure( double *pRoll, double *pPitch );
  void HideAll();
  void HideAllText();
  void ShowGraphButtons();
  void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedStartMeasure();
  afx_msg void OnBnClickedShowPolarGraph();
  afx_msg void OnBnClickedShowResultTable();
  afx_msg void OnBnClickedPrintGraph();
  afx_msg void OnBnClickedSaveGraph();
};
