// $Id: SetupTestPlaneDlg.h,v 1.2 2012/03/01 10:00:20 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

#define MIN_ZPAR  -100  //[m]
#define MAX_ZPAR  0     //[m]

class CSetupTestPlaneDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupTestPlaneDlg)

public:
	CSetupTestPlaneDlg( BOOL EnableZPar = FALSE, BOOL EnableReference = TRUE, BOOL EnableFixPlane = FALSE, CWnd* pParent = NULL );   // standard constructor
	virtual ~CSetupTestPlaneDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_TEST_PLANE };

private:
  CString m_Text, m_MsgCaption;
  CString m_ChText[SIZE_OF_ARRAYS];
  BOOL m_TestCh[SIZE_OF_ARRAYS];
  //double m_ZParCh[SIZE_OF_ARRAYS];
  int m_SelectedReference;
  BOOL m_Flatness;
  int m_ObjNo[SIZE_OF_ARRAYS];
  int m_NoOfCorr;
  BOOL m_EnableZPar;
  BOOL m_EnableReference;
  BOOL m_EnableFixPlane;
  BOOL m_BenchMarkTest;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();
  void ReferenceSelected();
  void TestEnabled();
  void SetObjectData();
  BOOL CheckNoOfConnectedObjects();
	void ShowAllZPar( BOOL Show );
	void DisableAllZPar( BOOL Disable );
	void EnableDisableZPar( BOOL DoIt, BOOL enable, int iD );
	void HideAllReferenceItems();

	DECLARE_MESSAGE_MAP()
  afx_msg void OnBnClickedCh1();
  afx_msg void OnBnClickedCh2();
  afx_msg void OnBnClickedCh3();
  afx_msg void OnBnClickedCh4();
  afx_msg void OnBnClickedCh5();
  afx_msg void OnBnClickedCh6();
  afx_msg void OnBnClickedCh7();
  afx_msg void OnBnClickedCh8();
  afx_msg void OnBnClickedCh9();
  afx_msg void OnBnClickedCh10();
  afx_msg void OnBnClickedCh11();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedTestCh1();
  afx_msg void OnBnClickedTestCh2();
  afx_msg void OnBnClickedTestCh3();
  afx_msg void OnBnClickedTestCh4();
	afx_msg void OnBnClickedTestCh5();
	afx_msg void OnBnClickedTestCh6();
	afx_msg void OnBnClickedTestCh7();
	afx_msg void OnBnClickedTestCh8();
	afx_msg void OnBnClickedTestCh9();
	afx_msg void OnBnClickedTestCh10();
  afx_msg void OnBnClickedTestCh11();
  afx_msg void OnEnKillfocus();
};
