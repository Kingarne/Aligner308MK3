// $Id: AlignerView.h,v 1.2 2010/09/10 07:41:02 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AlignerDoc.h"

class CAlignerView : public CView
{
protected:
	CAlignerView( void ) ;
	DECLARE_DYNCREATE(CAlignerView)
public:
	virtual ~CAlignerView( void ) ;
public:
	CAlignerDoc *GetDocument( void ) const ;
public:
	virtual void OnDraw( CDC *pDC ) ;
  virtual BOOL PreCreateWindow( CREATESTRUCT &cs ) ;
  virtual void OnInitialUpdate( void ) ;
#ifdef _DEBUG
	virtual void AssertValid( void ) const;
	virtual void Dump( CDumpContext &dc ) const ;
#endif
protected:
	DECLARE_MESSAGE_MAP()

    int m_timer;	
	BOOL ShouldReveiveData();
	
public:
  afx_msg void OnDestroy();
  afx_msg void OnTimer(UINT nIDEvent);
} ;

#ifndef _DEBUG
inline CAlignerDoc *CAlignerView::GetDocument( void ) const {
  return reinterpret_cast<CAlignerDoc*>( m_pDocument ) ;
}
#endif
