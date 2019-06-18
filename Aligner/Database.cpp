// $Id: Database.cpp,v 1.3 2010/08/13 13:44:46 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "resource.h"
#include "Database.h"
#include <algorithm>

Database::Database( void )
{
   ASSERT(0) ;
}

Database::~Database( void )
{
   ASSERT(0) ;
}

CDataSource Database::sm_source ;
CSession Database::sm_session ;

void ManipulateConnectionString(CString& str)
{    
    char a1[] = "=dro-wssaP esa-bat-aD"; //Database password=    
    reverse(a1, a1+strlen(a1));

    char a2[] = "arb n ona k";  //kanonbra
    std::reverse(a2, a2+strlen(a2));

    CString searchStr(a1); 
    searchStr.Remove('-');
    
    CString password = a2; 
    password.Remove(' ');

    int index = str.Find(searchStr, 0);
    if(index > 0)
    {
        str.Insert(index+searchStr.GetLength()+1, password);
    }
}

HRESULT Database::Open( void )
{
  USES_CONVERSION ;
  Registry reg;

  CString connectionString = reg.GetStringValue("ConnectionString", "") ;    
  //ManipulateConnectionString(connectionString);
  
  HRESULT hResult ;
#if defined(_DEBUG)
  CString queryString = reg.GetStringValue("Query", "") ;    
  if (0 == queryString.GetLength())
  {
#endif
  if ((S_OK == (hResult = sm_source.OpenFromInitializationString( A2COLE( (LPCSTR)connectionString), true )) ))
  {
    if (( S_OK != (hResult = sm_session.Open( sm_source )) ))
    {
      sm_source.Close() ;
    }
  }
#if defined(_DEBUG)
  } else {
  if (( S_OK == (hResult = sm_source.Open()) ))
  {
    if (( S_OK != (hResult = sm_session.Open( sm_source )) ))
    {
      sm_source.Close() ;
    }
    else
    {
      BSTR initializationString ;
      sm_source.GetInitializationString( &initializationString, TRUE ) ;
      // TODO: Hande the unusual error of "unable to write to registry".
      reg.SetStringValue("ConnectionString", OLE2A(initializationString));  
      //::AfxGetApp() -> WriteProfileString( DATABASE_REGISTER_SECTION, DATABASE_REGISTER_CONNECTIONSTRING, OLE2A(initializationString)) ;
    }
  } }
#endif
  
//   CString str;
//   str.Format("DB:%d\n",hResult);
//   ::AfxMessageBox(str);
  return hResult ;
}

HRESULT Database::StartTransaction( void )
{
  return sm_session.StartTransaction( ISOLATIONLEVEL_SERIALIZABLE ) ;
}

HRESULT Database::Commit( void )
{
  return sm_session.Commit() ;
}

void Database::Close( void )
{
  sm_session.Close() ;
  sm_source.Close() ;
}

void Database::DisplayError( HRESULT hResult, const TCHAR *pFileName, int lineNumber )
{
  CString text ;
  CString positionText ;
  text.LoadString( IDS_DB_SUBSYSTEM_FAILED ) ; 
  if (NULL != pFileName)
  {
    positionText.Format( _T("%s:%d\n"), pFileName, lineNumber ) ;
  }
  else
  {
    positionText.Format( _T("No line number information supplied\n") ) ;
  }
  text += positionText ;
  _com_error error( hResult ) ;
  text += error.ErrorMessage() ;
  text += _T("\n") ;
  CString epilog ;
  epilog.LoadString( IDS_TERMINATION ) ;
  text += epilog ;
  AfxMessageBox( text ) ;
}
