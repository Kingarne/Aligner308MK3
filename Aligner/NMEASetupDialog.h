// $Id: NMEASetupDialog.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class NMEASetupDialog : public CDialog
{
	DECLARE_DYNAMIC(NMEASetupDialog)
public:
	NMEASetupDialog( CWnd *pParent = NULL ) ;
	virtual ~NMEASetupDialog( void ) ;
	enum { IDD = IDD_NMEA_SETUP };
public:
  virtual BOOL OnInitDialog( void );
private:
    void InitComPortControl( void ) ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
public:
  CComboBox m_comPortControl;
  CString m_port ;
	DECLARE_MESSAGE_MAP()
} ;
