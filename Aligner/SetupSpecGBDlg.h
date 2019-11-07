// $Id: SetupSpecGBDlg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

class CSetupSpecGBDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupSpecGBDlg)

public:
	CSetupSpecGBDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupSpecGBDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_SPEC_GB };

private:
	CString m_Text, m_MsgCaption;
	CString m_ChText[SIZE_OF_ARRAYS];
	int m_SelectedReference;
	int m_ObjNo[SIZE_OF_ARRAYS];
	int m_NoOfCorr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void ReferenceSelected();
	void TestEnabled();
	void SetObjectData();
	bool CheckSensorsConnected(int index);
	bool CheckAllSensorsConnected();

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
  afx_msg void OnBnClickedTestCh();
  afx_msg void OnEnKillfocus();
  BOOL m_TestCh[SIZE_OF_ARRAYS];
};
