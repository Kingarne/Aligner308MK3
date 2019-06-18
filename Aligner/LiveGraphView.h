// $Id: LiveGraphView.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "DAU.h"
#include "Defines.h"

class CLiveGraphView : public CFormView
{
public:
	DECLARE_DYNCREATE(CLiveGraphView)
public:
	CLiveGraphView( void ) ;
	virtual ~CLiveGraphView( void ) ;

public:
	enum { IDD = IDD_LIVE_GRAPH_FORM };
#ifdef _DEBUG
	virtual void AssertValid( void ) const ;
	virtual void Dump( CDumpContext &dc ) const ;
#endif

public:
  void SetSource( DAU *pDAU ) ;
  void UpdateData( void ) ;

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	DECLARE_MESSAGE_MAP()

protected:
  afx_msg LRESULT OnNewData( WPARAM, LPARAM ) ;
  afx_msg void OnBnClickedLiveGraphQuit() ;
  afx_msg void OnDestroy();

protected:
  CWnd m_chart ;
  IChart2D_DualPtr m_pChart ;

private:
  DAU *m_pSource ;
  int m_ChannelNo[SIZE_OF_ARRAYS];
  int m_NoOfChannels;

public:
  virtual void OnInitialUpdate();
  afx_msg void OnBnClickedLiveGraphPrint();
  afx_msg void OnBnClickedLiveGraphSave();
} ;
