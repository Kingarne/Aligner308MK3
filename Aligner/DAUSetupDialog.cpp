// $Id: DAUSetupDialog.cpp,v 1.3 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "DAUSetupDialog.h"
#include "SerialPort.h"

IMPLEMENT_DYNAMIC(DAUSetupDialog, CDialog)

DAUSetupDialog::DAUSetupDialog( CWnd* pParent )	: CDialog( DAUSetupDialog::IDD, pParent )
{
  // Empty
}

DAUSetupDialog::~DAUSetupDialog( void )
{
  // Empty
}

void DAUSetupDialog::DoDataExchange( CDataExchange* pDX )
{
  CDialog::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_CONVERTER_1_ROLL, m_converterOneRoll) ;
  DDX_Control(pDX, IDC_CONVERTER_1_PITCH, m_converterOnePitch) ;
  DDX_Control(pDX, IDC_CONVERTER_2_ROLL, m_converterTwoRoll) ;
  DDX_Control(pDX, IDC_CONVERTER_2_PITCH, m_converterTwoPitch) ;
  DDX_Control(pDX, IDC_DAU_SERIAL_NUMBER, m_serialNumberControl);
  if (pDX -> m_bSaveAndValidate)
  {
    m_serialNumberControl.GetWindowText( m_serialNumber ) ;
  }
}

BEGIN_MESSAGE_MAP(DAUSetupDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(ID_DAU_CHANGE, OnBnClickedDauChange)
END_MESSAGE_MAP()

void DAUSetupDialog::InitSerialNumber( void )
{
//   UniqueSNFromDAUCorrection table ;
//   HRESULT hResult = ::OpenTable( table ) ;
//   if (S_OK == hResult)
//   {
//     HRESULT hResult = table.MoveFirst() ;
//     UINT index = 0 ;
//     while( S_OK == hResult)
//     {
//       CString text ;
//       text.Format( "%03d", atoi(table.m_serialNumber) ) ;
//       m_serialNumberControl.AddString( text ) ;
//       if (text == m_serialNumber)
//       {
//         m_serialNumberControl.SetCurSel( index ) ;
//       }
//       hResult = table.MoveNext() ;
//       index++ ;
//     }
//   }
//   else
//   {
//     ASSERT(0) ; // TODO: Add proper error handling.
//     return ;
//   }
}

BOOL DAUSetupDialog::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  m_converterOneRoll.SetCurSel( (m_mask & 0xC0) >> 6 ) ;
  m_converterOnePitch.SetCurSel( (m_mask & 0x30) >> 4 ) ;
  m_converterTwoRoll.SetCurSel( (m_mask & 0x0C) >> 2 ) ;
  m_converterTwoPitch.SetCurSel( (m_mask & 0x03) >> 0 ) ;
  return TRUE ;
}

void DAUSetupDialog::OnBnClickedOk( void )
{
  m_mask = ((m_converterOneRoll.GetCurSel() & 0x3) << 6) |
           ((m_converterOnePitch.GetCurSel() & 0x3) << 4) |
           ((m_converterTwoRoll.GetCurSel() & 0x3) << 2) |
           ((m_converterTwoPitch.GetCurSel() & 0x3) << 0) ;
  OnOK() ;
}

void DAUSetupDialog::OnBnClickedDauChange( void )
{
  SerialPort temporaryPort ;
  if (temporaryPort.Open( FALSE, m_port, TRUE ))
  {
    temporaryPort.DoConfig() ;
    temporaryPort.SaveConfig() ;
    temporaryPort.Close() ;
  }
  else
  {
    // TODO: Add error notification.
  }
}
