//	 $Id: SelectDAUDialog.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002-2003

#include "stdafx.h"
#include "Resource.h"
#include "SelectDAUDialog.h"

IMPLEMENT_DYNAMIC(SelectDAUDialog, CDialog)

SelectDAUDialog::SelectDAUDialog(map<LONG,CString> *pSerialNumberMap, CWnd *pParent) : CDialog(SelectDAUDialog::IDD, pParent)
{
  ASSERT(pSerialNumberMap) ;
  m_pSerialNumberMap = pSerialNumberMap ;
}

SelectDAUDialog::~SelectDAUDialog( void )
{
  // Empty
}

void SelectDAUDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX) ;
  DDX_Control(pDX, IDC_COMBO1, m_dauControl) ;
  DDX_CBString(pDX, IDC_COMBO1, m_dau) ;
}

BEGIN_MESSAGE_MAP(SelectDAUDialog, CDialog)
END_MESSAGE_MAP()

static const char DAU_REGISTER_SECTION []        = "" ;
static const char DAU_REGISTER_SERIAL []         = "dau" ;
static const char DAU_REGISTER_SERIAL_DEFAULT [] = "" ;

BOOL SelectDAUDialog::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  for (map<LONG,CString>::iterator i = m_pSerialNumberMap -> begin() ; i != m_pSerialNumberMap -> end() ; i++)
  {
    int index = m_dauControl.AddString( i -> second ) ;
    if (i -> second == m_dau)
    {
      m_dauControl.SetCurSel( index ) ;
    }
  }
  if (CB_ERR == m_dauControl.GetCurSel())
  {
    m_dauControl.SetCurSel( 0 ) ;
  }
  return TRUE ;
}
