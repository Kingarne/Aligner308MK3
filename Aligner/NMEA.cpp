// $Id: NMEA.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Include.h"
#include "NMEASetupDialog.h"

NMEA::NMEA(void)
{
  m_pConnection = NULL ;
}

NMEA::~NMEA(void)
{
  // Empty
}

void NMEA::NewData( const NMEAData &data )
{
  m_angle = data.m_heading ;
  NotifyListeners() ;
}

void NMEA::AddListener( CWnd *pNewListener ) const
{
  ASSERT(NULL != pNewListener) ;
  CSingleLock _(&m_listenersLock, TRUE) ;
  m_listeners.insert( pNewListener ) ;
}

void NMEA::RemoveListener( CWnd *pExistingListener ) const
{
  ASSERT(NULL != pExistingListener) ;
  CSingleLock _(&m_listenersLock, TRUE) ;
  m_listeners.erase( pExistingListener ) ;
}


void NMEA::NotifyListeners( void )
{
  CSingleLock _(&m_listenersLock, TRUE) ;
  for (set<CWnd *>::iterator i = m_listeners.begin() ; i != m_listeners.end() ; i++)
  {
    (*i)-> PostMessage( UM_NEW_NMEA ) ;
  }
}


BOOL NMEA::Close( void )
{
  if (m_pConnection)
  {
    HANDLE handle = m_pConnection -> m_hThread ;
    m_pConnection -> PostThreadMessage( UM_QUIT, 0, 0 ) ;
    WaitForSingleObject( handle, INFINITE ) ;
    m_pConnection = NULL ;
  }
  return TRUE ;
}

BOOL NMEA::Config( void )
{
  NMEASetupDialog dialog ;
  dialog.m_port = m_port ;
  if (IDOK == dialog.DoModal())
  {
    m_port = dialog.m_port ;
    return TRUE ;
  }
  return FALSE ;
}

BOOL NMEA::Open( void )
{
	/*
  if (( !(m_pConnection = reinterpret_cast<NMEAConnection *>(::AfxBeginThread( RUNTIME_CLASS(NMEAConnection), 0, 0, CREATE_SUSPENDED ))) ))
  {
    // TODO: This is a "baderror" and should not ...
    return FALSE ;
  }
  m_pConnection -> SetDeviceName( m_port ) ;
  m_pConnection -> SetListener( this ) ;
  m_pConnection -> ResumeThread() ;

  if (!m_pConnection -> WaitForEvent())
  {
    Close() ;
    return FALSE ;
  }
	*/
  return TRUE ;
}

void NMEA::ConfigPort( void )
{
  GetConnection() -> DoConfig() ;
}

static const char NMEA_REGISTER_SECTION []          = "NMEA" ;
static const char NMEA_REGISTER_PORTNAME []         = "port" ;
static const char NMEA_REGISTER_PORTNAME_DEFAULT [] = "COM2" ;

BOOL NMEA::LoadConfigFromRegistry( void )
{
  m_port = AfxGetApp() -> GetProfileString(
      NMEA_REGISTER_SECTION, NMEA_REGISTER_PORTNAME, NMEA_REGISTER_PORTNAME_DEFAULT ) ;
  return TRUE ;
}

BOOL NMEA::SaveConfigToRegistry( void )
{
  AfxGetApp() ->
    WriteProfileString( NMEA_REGISTER_SECTION, NMEA_REGISTER_PORTNAME, m_port ) ;
  return TRUE ;
}
