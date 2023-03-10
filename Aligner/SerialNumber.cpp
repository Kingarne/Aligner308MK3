// $Id: SerialNumber.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "SerialNumber.h"
#include <sstream>

SerialNumber::~SerialNumber( void )
{
  // Empty
}

BOOL SerialNumber::SetSerialNumber( const CString &serialNumber )
{
  // TODO: Added verification of numbers only.
  if (3 == serialNumber.GetLength() || 0 == serialNumber.GetLength())
  {
    m_serialNumber = serialNumber ;
    return TRUE ;
  }
  return FALSE ;
}

AdapterSerialNumber::~AdapterSerialNumber( void )
{
  // Empty
}

BOOL AdapterSerialNumber::SetAdapterDesc(const CString desc)
{
  if (!SetAdapterSerialNumber(desc))
    return FALSE;

  return SetCaliber(desc.Right(desc.GetLength() - 6));

}

BOOL AdapterSerialNumber::SetCaliber(const CString& cal)
{
  // TODO: Added verification of numbers only.
  if (cal.GetLength() > 0)
  {
    double d;
    stringstream ss((LPCTSTR)cal, std::stringstream::in);
    ss >> d;

    m_caliber = d;
    return TRUE;
  }
  return FALSE;
}


BOOL AdapterSerialNumber::SetAdapterSerialNumber( const CString &serialNumber )
{
  // TODO: Added verification of numbers only.  
  if (serialNumber.GetLength() >= 3 || 0 == serialNumber.GetLength())
  {
    if (isdigit(serialNumber.GetAt(0)));
    {
      m_serialNumber = serialNumber.Left(3);
      return TRUE;
    }
  }
  return FALSE ;
}
