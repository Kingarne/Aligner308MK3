// $Id: AlignerControls.cpp,v 1.2 2009/11/24 16:42:56 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignerControls.h"
#include ".\alignercontrols.h"

IMPLEMENT_DYNAMIC(WhiteBackgroundEdit, CEdit)

WhiteBackgroundEdit::WhiteBackgroundEdit( void )
{
    m_color = RGB(255, 255, 255);
    m_backgroundBrush.CreateSolidBrush(m_color) ;   
   
}

WhiteBackgroundEdit::~WhiteBackgroundEdit( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(WhiteBackgroundEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
  ON_WM_SETFOCUS()
  ON_WM_SETCURSOR()
END_MESSAGE_MAP()

HBRUSH WhiteBackgroundEdit::CtlColor( CDC *pDC, UINT controlColor )
{
 // (void)controlColor ;
  //pDC -> SetTextColor( RGB(0,0,0) ) ;
  
   pDC->SetBkColor( m_color ) ;  
   pDC->SetTextColor( RGB(0,0,255) ) ;
   pDC->SetDCPenColor( RGB(0,0,0) ) ;
   pDC->SetDCBrushColor( RGB(0,0,0) ) ;

   
  return m_backgroundBrush ;
}

void WhiteBackgroundEdit::SetBkColor(COLORREF color)
{
     m_backgroundBrush.DeleteObject();
     m_color = color;
     m_backgroundBrush.CreateSolidBrush(color) ;
}

void WhiteBackgroundEdit::OnSetFocus(CWnd* pOldWnd)
{
  //CEdit::OnSetFocus(pOldWnd);
  // Don't set focus on the edit box
}


BOOL WhiteBackgroundEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  // Don't change the cursor
  return TRUE;
  //return CEdit::OnSetCursor(pWnd, nHitTest, message);
}

