// $Id: SelectDAUDialog.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class SelectDAUDialog : public CDialog
{
public:
	DECLARE_DYNAMIC(SelectDAUDialog)
	DECLARE_MESSAGE_MAP()
public:
	SelectDAUDialog( map<LONG,CString> *pSerialNumberMap, CWnd* pParent = NULL ) ;
	virtual ~SelectDAUDialog( void ) ;
public:
	enum { IDD = IDD_SELECT_DAU } ;
public:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
private:
  void SetupDAU( void ) ;
public:
  CComboBox m_dauControl ;
  CString m_dau ;
private:
  map<LONG,CString> *m_pSerialNumberMap ;
} ;
