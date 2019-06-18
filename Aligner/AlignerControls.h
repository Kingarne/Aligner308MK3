// $Id: AlignerControls.h,v 1.2 2009/11/24 16:42:56 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class WhiteBackgroundEdit : public CEdit
{
	DECLARE_DYNAMIC(WhiteBackgroundEdit)
	DECLARE_MESSAGE_MAP()

public:
	WhiteBackgroundEdit( void ) ;
	virtual ~WhiteBackgroundEdit( void ) ;
    
    void SetBkColor(COLORREF color);

private:
  afx_msg HBRUSH CtlColor( CDC *pDC, UINT controlColor ) ;
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

private:
  CBrush m_backgroundBrush ;
    COLORREF m_color;
} ;


