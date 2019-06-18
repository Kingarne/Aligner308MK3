// $Id: SerialNumber.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "SerialNumber.h"

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

BOOL AdapterSerialNumber::SetAdapterSerialNumber( const CString &serialNumber )
{
  // TODO: Added verification of numbers only.
  if (3 == serialNumber.GetLength() || 0 == serialNumber.GetLength())
  {
    m_serialNumber = serialNumber ;
    return TRUE ;
  }
  return FALSE ;
}
