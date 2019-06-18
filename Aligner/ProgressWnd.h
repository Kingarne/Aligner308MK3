// $Id: ProgressWnd.h,v 1.3 2009/12/08 19:14:22 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class ProgressWnd : public CDialog
{
	DECLARE_DYNAMIC(ProgressWnd)
	DECLARE_MESSAGE_MAP()
public:
	ProgressWnd( int steps, CString title="Progress", CWnd *pParent = NULL ) ;
    ProgressWnd(CWnd *pParent =NULL) ;
    virtual ~ProgressWnd( void ) ;

  enum { IDD = IDD_PROBE_PROGRESS } ;

  void Init(CString title, int steps);
  void Done();
  void SetPos( int pos ) ;
  void SetSteps( int steps ) ;
  int Step( void ) ;
  BOOL IsCanceled(){return m_canceled;}
    void SetElapsedTime(double e);
    void SetInfoText(CString& str);
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
private:
  CProgressCtrl m_progressControl ;
  int m_steps ;
    BOOL m_canceled;
    int m_timer;
    double m_elapsedTime;
public:
    CString m_info;
    afx_msg void OnBnClickedButton1();
    
    afx_msg void OnTimer(UINT nIDEvent);
    CString m_info2Text;
} ;
