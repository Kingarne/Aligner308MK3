// $Id: SetupDriftDlg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

class CSetupDriftDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDriftDlg)

public:
	CSetupDriftDlg( CWnd* pParent = NULL );   // standard constructor
	virtual ~CSetupDriftDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_DRIFT };

private:
  CString m_Text, m_MsgCaption;
  CString m_ChText[SIZE_OF_ARRAYS];
  int m_SelectedReference;

  //keep the variable names from the old pascal code

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();
  void SetGyroData();

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
	BOOL m_TestCh[SIZE_OF_ARRAYS]; //index [0] not used
};
