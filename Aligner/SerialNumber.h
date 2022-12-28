// $Id: SerialNumber.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class SerialNumber
{
public:
  SerialNumber( void ) ;
  virtual ~SerialNumber( void ) ;

  BOOL SetSerialNumber( const CString &serialNumber ) ;
  CString GetSerialNumber( void ) const ;

private:
  CString m_serialNumber ;
} ;

inline
SerialNumber::SerialNumber( void ) {
  m_serialNumber = _T("000") ;
}

inline
CString SerialNumber::GetSerialNumber( void ) const {
  return m_serialNumber ;
}

class AdapterSerialNumber
{
public:
  AdapterSerialNumber( void ) ;
  virtual ~AdapterSerialNumber( void ) ;
public:
  BOOL SetAdapterSerialNumber( const CString &serialNumber ) ;
public:
  CString GetAdapterSerialNumber( void ) const ;
private:
  CString m_serialNumber ;
} ;

inline
AdapterSerialNumber::AdapterSerialNumber( void ) {
  m_serialNumber = _T("000") ;
}

inline
CString AdapterSerialNumber::GetAdapterSerialNumber( void ) const {
  return m_serialNumber ;
}
