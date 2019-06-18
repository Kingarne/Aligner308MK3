// $Id: NMEA.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class NMEA : public NMEAConnectionListener
{
public:
  NMEA( void ) ;
  virtual ~NMEA( void ) ;
public:
  BOOL Open( void ) ;
  BOOL Close( void ) ;
  BOOL IsOpen( void ) const ;
  BOOL Config( void ) ;
  void ConfigPort( void ) ;
  NMEAConnection *GetConnection( void ) ;
  BOOL LoadConfigFromRegistry( void ) ;
  BOOL SaveConfigToRegistry( void ) ;
public:
  virtual void NewData( const NMEAData &data ) ;
public:
  void AddListener( CWnd *pNewListener ) const ;
  void RemoveListener( CWnd *pExistingListener ) const ;
  void NotifyListeners( void ) ;
public:
  double GetAngle( void ) const ;
private:
  mutable CCriticalSection m_listenersLock ;
  mutable set<CWnd *> m_listeners ;
  NMEAConnection *m_pConnection ;
	CWinThread *m_pReadData ;
public:
  CString m_port ; // TODO: This should be private, add access method.
  double m_angle ;
} ;

inline
NMEAConnection *NMEA::GetConnection( void ) {
    return m_pConnection ;
}

inline
void NMEAConnection::SetDeviceName( const CString &deviceName ) {
  m_deviceName = deviceName ;
}

inline
BOOL NMEA::IsOpen( void ) const {
  return NULL != m_pConnection ;
}

inline
double NMEA::GetAngle( void ) const {
  return m_angle ;
}
