// $Id: NMEAConnection.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

/**
 *  @note An implementation of NMEAConnectionListener should never wait on a syncronization
 *        object. This to avoid a deadlock. Care should be taken to avoid a race
 *        condition.
 */
class NMEAConnectionListener
{
public:
  virtual void NewData( const NMEAData &data ) = 0 ;
} ;

class NMEAConnection : public CWinThread
{
	DECLARE_DYNCREATE(NMEAConnection)
protected:
	NMEAConnection( void ) ;
	virtual ~NMEAConnection( void ) ;
public:
	virtual BOOL InitInstance( void ) ;
	virtual int ExitInstance( void ) ;
public:
  void SetDeviceName( const CString &deviceName ) ;
  void SetListener( NMEAConnectionListener *pListener ) ;
private:
  afx_msg void OnNewNMEAData( WPARAM, LPARAM ) ;
  afx_msg void OnQuit( WPARAM, LPARAM ) ;
public:
  void DoConfig( void ) ;
  BOOL WaitForEvent( void ) ;
	BOOL GetData( NMEAData &data ) ;
private:
	void AddData( NMEAData &data ) ;
	void Purge( void ) ;
	UINT RunReadThread( void ) ;
  void StopReadThread( void ) ;
private:
  CString m_deviceName ;
  NMEAConnectionListener *m_pListener ;
	INT m_stop ;
	CWinThread *m_pReadData ;
private:
	SerialPort m_connectionPort ;
	CCriticalSection m_dataLock ;
  CEvent m_instanceReady ;
	list<NMEAData> m_dataEntries ;
  UINT m_dataLimit ;
private:
  int m_mode ;
private:
	static UINT run( void *pThis ) ;
protected:
	DECLARE_MESSAGE_MAP()
} ;
