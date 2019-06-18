// $Id: SetupMeasureDlg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

#define MIN_ANGLE   0
#define MAX_ANGLE   360     //[deg]
#define MIN_LENGTH  0
#define MAX_LENGTH  100000  //[mm]

class CSetupMeasureDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupMeasureDlg)

public:
	CSetupMeasureDlg( BOOL CommonFlatTest = FALSE, BOOL Found = FALSE, BOOL Flatness = FALSE, CWnd* pParent = NULL );   // standard constructor
	virtual ~CSetupMeasureDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_MEASURE };

private:
  CString m_Text, m_MsgCaption;
  CString m_ChText[SIZE_OF_ARRAYS];
  BOOL m_TestCh[SIZE_OF_ARRAYS];
  int m_LengthCh[SIZE_OF_ARRAYS];
  double m_AngleCh[SIZE_OF_ARRAYS];
//  int m_AngleCh[SIZE_OF_ARRAYS];
  int m_SelectedReference;
  BOOL m_Flatness;
  int m_ObjNo[SIZE_OF_ARRAYS];
  int m_NoOfCorr;
  BOOL m_CommonFlatTest;

  //keep the variable names from the old pascal code
  BOOL m_Fou;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();
  void ReferenceSelected();
  void TestEnabled();
  void SetObjectData();
  int GetNoOfToBeTested();
  void CommonFlatTestCheck( int channelNo );

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
