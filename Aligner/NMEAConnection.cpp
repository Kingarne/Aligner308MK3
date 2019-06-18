// $Id: NMEAConnection.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Include.h"

IMPLEMENT_DYNCREATE(NMEAConnection, CWinThread)

NMEAConnection::NMEAConnection( void )
{
  TRACE("NMEAConnection::NMEAConnection\n") ;
  m_deviceName = _T("COM2") ;
	m_pReadData = NULL ;
  m_dataLimit = 5 ;
 	m_stop = 0 ;
  m_pListener = NULL ;
}

NMEAConnection::~NMEAConnection( void )
{
  TRACE("NMEAConnection::~NMEAConnection\n") ;
  StopReadThread() ;
}

BOOL NMEAConnection::InitInstance( void )
{
  TRACE("NMEAConnection::InitInstance\n");
	if (!m_connectionPort.Open( m_deviceName, TRUE ))
	{
    TRACE("NMEAConnection::Unable to open COM port: %s\n", m_deviceName) ;
		return FALSE ;
	}
  TRACE("NMEAConnection::InitInstance opened connection port\n");
	COMMTIMEOUTS timeout ;
	timeout.ReadIntervalTimeout = 0 ;
	timeout.ReadTotalTimeoutConstant = 200 ;
	timeout.ReadTotalTimeoutMultiplier = 0 ;
	timeout.WriteTotalTimeoutConstant = 0 ;
	timeout.WriteTotalTimeoutMultiplier = 0 ;
	if (!m_connectionPort.SetTimeout( timeout ))
	{
    TRACE("Unable to set Timeout on COM port: %s\n", m_deviceName) ;
		m_connectionPort.Close() ;
		return FALSE ;
	}
  TRACE("NMEAConnection::InitInstance Set timeout\n");

	if (( !(m_pReadData = AfxBeginThread( run, this )) ))
	{
    TRACE("Unable to set start read thread\n") ;
    m_connectionPort.Close() ;
    return FALSE ;
  }
  TRACE("NMEAConnection::InitInstance started thread\n");

  TRACE("Setting NMEA event\n");
  return m_instanceReady.SetEvent() ;
}

int NMEAConnection::ExitInstance( void )
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(NMEAConnection, CWinThread)
  ON_THREAD_MESSAGE(UM_QUIT, OnQuit)
END_MESSAGE_MAP()

void NMEAConnection::OnQuit( WPARAM, LPARAM )
{
  TRACE("NMEAConnection::OnQuit\n") ;
  PostQuitMessage( 0 ) ;
}

void NMEAConnection::StopReadThread( void )
{
  TRACE("NMEAConnection::StopReadThread\n") ;
  m_stop = 1 ;
	if (m_pReadData)
	{
		WaitForSingleObject( m_pReadData -> m_hThread, INFINITE ) ;
		m_pReadData = NULL ;
	}
	m_stop = 0 ;
}

BOOL NMEAConnection::WaitForEvent( void )
{
  TRACE("NMEAConnection::WaitForEvent\n") ;
  CSingleLock lock( &m_instanceReady ) ;
  return lock.Lock( 2000 ) ;
}

void NMEAConnection::DoConfig( void )
{
  if (m_connectionPort.IsOpen())
  {
    m_connectionPort.DoConfig() ;
  }
}

void NMEAConnection::SetListener( NMEAConnectionListener *pListener )
{
  m_pListener = pListener ;
}

UINT NMEAConnection::run( void *pThis )
{
  TRACE("NMEAConnection::run( ... )\n") ;
	return ((NMEAConnection *)pThis) -> RunReadThread() ;
}

UINT NMEAConnection::RunReadThread( void )
{
  TRACE("NMEAConnection::RunReadThread\n") ;
	while (!m_stop)
	{
    char buffer [16] ;
    UINT length ;
    // TODO: Merge UNIX NMEA parser here.
    if (( 16 == (length = m_connectionPort.Read( buffer, sizeof(buffer))) ))
    {
      buffer [6] = '\000' ;
      if (CString( "$HEHDT" ) == buffer)
      {
        buffer [12] = '\000' ;
        if (m_pListener)
        {
          m_pListener -> NewData( NMEAData( atof( buffer + 7 ) ) ) ;
        }
      }
    }
  }
  TRACE("NMEAConnection::RunReadThread caught m_stop != 0 \n") ;
	return 0 ;
}
