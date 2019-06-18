// $Id: MainFrm.h,v 1.4 2011/01/27 09:28:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "DebugDlg.h"
#include "ColorStatusBar.h"

class CMainFrame : public CFrameWnd
{
protected:
	CMainFrame( void ) ;
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual BOOL PreCreateWindow( CREATESTRUCT &cs ) ;
public:
	virtual ~CMainFrame( void ) ;
#ifdef _DEBUG
	virtual void AssertValid( void ) const ;
	virtual void Dump( CDumpContext &dc ) const ;
#endif
protected:
	ColorStatusBar  m_wndStatusBar ;
	CToolBar    m_wndToolBar ;
	CReBar      m_wndReBar ;
	CDialogBar  m_wndDlgBar ;
protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct ) ;
	afx_msg void OnHelpFinder();
	afx_msg void OnHelp();
	afx_msg void OnContextHelp();
	DECLARE_MESSAGE_MAP()

    LRESULT OnSyncDigial( WPARAM, LPARAM );
    LRESULT OnDACTestErr( WPARAM, LPARAM );
	LRESULT OnDAUStatus( WPARAM wp, LPARAM lp);

public:
  afx_msg void OnClose();
  afx_msg void OnDebugPanel();

    DebugDlg m_debugDlg;
} ;
