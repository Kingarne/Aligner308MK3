// $Id: PresentDialog.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CResultTable;

class CPresentDialog : public CDialog
{
	DECLARE_DYNAMIC(CPresentDialog)

public:
	CPresentDialog( CResultTable* pResultTable = NULL, CWnd* pReportWindow = NULL );   // standard constructor
	virtual ~CPresentDialog();

// Dialog Data
	enum { IDD = IDD_PRESENT_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();

  CString m_Comment;
	CResultTable* m_pResultTable;

	DECLARE_MESSAGE_MAP()
protected:
  afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
  afx_msg void OnEnKillfocusCommentBox();
};
