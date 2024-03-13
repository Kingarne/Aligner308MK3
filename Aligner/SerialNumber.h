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
  BOOL SetAdapterCaliber(const double cal) { m_caliber = cal; }
  BOOL SetAdapterDesc(const CString desc);
  BOOL SetCaliber(const CString& cal);
public:
  CString GetAdapterSerialNumber( void ) const ;
  double GetAdapterCaliber(void) { return m_caliber; }
  CString GetAdapterDesc(void) const;
private:
  CString m_serialNumber ;
  double m_caliber;
} ;

inline
AdapterSerialNumber::AdapterSerialNumber( void ) {
  m_serialNumber = _T("000") ;
  m_caliber = 0;
}

inline
CString AdapterSerialNumber::GetAdapterSerialNumber( void ) const {
  return m_serialNumber ;
}


inline
CString AdapterSerialNumber::GetAdapterDesc(void) const {
  CString serial = m_serialNumber;
  CString calStr="";
  if(m_caliber > 0)
   calStr.Format(" - %.0fmm", m_caliber);
  serial += calStr;
  return serial;
}

