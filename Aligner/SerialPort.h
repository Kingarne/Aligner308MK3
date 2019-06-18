// $Id: SerialPort.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#ifndef SERIALPORT_KEYMAIN
#define SERIALPORT_KEYMAIN HKEY_CURRENT_USER
#endif

#ifndef SERIALPORT_KEYNAME
#define SERIALPORT_KEYNAME _T("Software\\Schill Reglerteknik AB\\SerialPort")
#endif

class SerialPortSet
{
private:
  SerialPortSet( void ) ;
public:
  static BOOL Init( void ) ;
  static void Exit( void ) ;
  static BOOL GetFirstPort( UINT &port ) ;
  static BOOL GetNextPort( UINT &port ) ;
  static BOOL ReservePort( UINT port ) ;
  static BOOL ReleasePort( UINT port ) ;
  static CString GetPortName( UINT port ) ;
private:
  static BOOL NextValidPort( UINT &port ) ;
private:
  static vector<CString> m_portNames ;
  static set<UINT> m_reservedPorts ;
} ;

class SerialPort
{
	public:
		SerialPort( void ) ;
		virtual ~SerialPort( void ) ;
	private:
		SerialPort( const SerialPort & ) ;
	public:
        BOOL dUMMY( const TCHAR *pDeviceName ) ;
		BOOL Open( BOOL checkIfDauDefault, const TCHAR *pDeviceName,
		       BOOL configInRegistry = FALSE,
				   TCHAR *pRegistryKeyName = SERIALPORT_KEYNAME,
				   HKEY = SERIALPORT_KEYMAIN ) ;
		BOOL GetTimeout( COMMTIMEOUTS &timeouts ) const ;
		BOOL SetTimeout( COMMTIMEOUTS &timeouts ) ;
		void CancelIO( void ) ;
		BOOL Close( void ) ;
		BOOL IsOpen( void ) const ;
		UINT Write( const char *pData, unsigned int dataSize )  ;
		UINT Read( char *pData, unsigned int dataSize ) ;
		BOOL Read( char *pData, unsigned int dataSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pCompletionRoutine ) ;
		BOOL DoModal( void ) ;
		BOOL DoConfig( void ) ;
		BOOL LoadConfig( void ) ;
		BOOL SaveConfig( void ) ;
    BOOL SetToDefault( void ) ;
    BOOL IsDefault( void ) ;
	private:
		string m_deviceName ;
		BOOL m_configInRegistry ;
		string m_keyName ;
		HKEY m_hKey ;
		HANDLE m_hDevice ;
} ;
