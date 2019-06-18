// $Id: DAUSetupDialog.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class DAUSetupDialog : public CDialog
{
	DECLARE_DYNAMIC(DAUSetupDialog)
	DECLARE_MESSAGE_MAP()
public:
	DAUSetupDialog( CWnd* pParent = NULL ) ;
	virtual ~DAUSetupDialog( void ) ;
protected:
	enum { IDD = IDD_DAU_SETUP } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
protected:
  afx_msg void OnBnClickedOk( void ) ;
  afx_msg void OnBnClickedDauChange( void ) ;
private:
  void InitSerialNumber( void ) ;
public:
  int m_mask ;
  CString m_port ;
  CComboBox m_converterOneRoll ;
  CComboBox m_converterOnePitch ;
  CComboBox m_converterTwoRoll ;
  CComboBox m_converterTwoPitch ;
  CComboBox m_serialNumberControl ;  
  CString m_serialNumber ;  
} ;
