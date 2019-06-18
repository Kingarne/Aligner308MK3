// $Id: NMEASetupDialog.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Include.h"

IMPLEMENT_DYNAMIC(NMEASetupDialog, CDialog)
NMEASetupDialog::NMEASetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NMEASetupDialog::IDD, pParent)
{
}

NMEASetupDialog::~NMEASetupDialog()
{
}

void NMEASetupDialog::DoDataExchange( CDataExchange *pDX )
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_NMEA_COM_PORT, m_comPortControl);
  if (pDX -> m_bSaveAndValidate)
  {
      m_comPortControl.GetWindowText( m_port ) ;
  }
}


BEGIN_MESSAGE_MAP(NMEASetupDialog, CDialog)
END_MESSAGE_MAP()

void NMEASetupDialog::InitComPortControl( void )
{
  UINT port ;
  if (!SerialPortSet::GetFirstPort( port ))
  {
    return ; // TODO: Add notification that there are no COM ports aviable.
  }

  UINT index = 0 ;
  do
  {
    m_comPortControl.AddString( SerialPortSet::GetPortName( port ) ) ;
    if (SerialPortSet::GetPortName( port ) == m_port)
    {
      m_comPortControl.SetCurSel( index ) ;
      m_comPortControl.SetWindowText( SerialPortSet::GetPortName( port ) ) ;
    }
    index++ ;
  }
  while (SerialPortSet::GetNextPort( port )) ;
}

BOOL NMEASetupDialog::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  InitComPortControl() ;
  return TRUE ;
}

// NMEASetupDialog message handlers

