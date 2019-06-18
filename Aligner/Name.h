// $Id: Name.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class Name
{
public:
  Name( const CString &name ) ;
  virtual ~Name( void ) ;
public:
  CString GetName( void ) const ;
  void SetName( const CString &name ) ;
private:
  CString m_name ;
} ;

inline
Name::Name( const CString &name ) : m_name(name)
{
    /* Empty */
}

inline
CString Name::GetName( void ) const {
  return m_name ;
}

inline
void Name::SetName( const CString &name ) {
  m_name = name ;
}
