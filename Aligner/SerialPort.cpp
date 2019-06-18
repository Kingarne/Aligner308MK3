// $Id: SerialPort.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SerialPort.h"
//#include "MPAP_Drv.h"

vector<CString> SerialPortSet::m_portNames ;
set<UINT> SerialPortSet::m_reservedPorts ;

BOOL SerialPortSet::Init( void )
{
  TCHAR devices [65535] ;
  DWORD length = QueryDosDevice( NULL, devices, sizeof(devices) ) ;
  if (0 < length)
  {
      TCHAR* pDeviceName = devices ;
      while ((TCHAR)0 != *pDeviceName)
      {
        if ((3 < _tcslen( pDeviceName )) && !_tcsnicmp( "COM", pDeviceName, 3 ))
        {
          m_portNames.push_back( CString( pDeviceName ) ) ;
        }
        pDeviceName += _tcslen( pDeviceName ) + 1 ;
      }
	  if(0)// MPAP_Init() == MPAP_NoError )
	  {
		m_portNames.push_back( "PCMCIA Comm" ) ;
		//ReOpenComm(CommModeCRCPreset) ;
	  }
  }
  return TRUE ;
}

void SerialPortSet::Exit( void )
{
  m_portNames.clear() ;
  m_reservedPorts.clear() ;
}

BOOL SerialPortSet::NextValidPort( UINT &port )
{
  while (port < m_portNames.size())
  {
    if (m_reservedPorts.end() == m_reservedPorts.find( port ))
    {
       return TRUE ;
    }
    port++ ;
  }
  return FALSE ;
}

BOOL SerialPortSet::GetFirstPort( UINT &port )
{
  port = 0 ;
  return NextValidPort( port ) ;
}

BOOL SerialPortSet::GetNextPort( UINT &port )
{
  port++ ;
  return NextValidPort( port ) ;
}

BOOL SerialPortSet::ReservePort( UINT port )
{
  return (BOOL)(m_reservedPorts.insert( port ).second) ;
}

BOOL SerialPortSet::ReleasePort( UINT port )
{
  return (0 != m_reservedPorts.erase( port )) ;
}

CString SerialPortSet::GetPortName( UINT port ) 
{
  return m_portNames [port] ;
}

/**
 *  @brief Default constructor.
 *
 *  The default constructor set the instance created to an uninitialized
 *  state.
 *
 *  @see Open(), DoConfig()
 */
SerialPort::SerialPort( void )
{ 
	m_hDevice = NULL ;
}

/**
 *  @brief Copy constructor (unusabe).
 *
 *  The copy constructor is declared private to prevent accidental use.
 *  SerialPort isn't designed for copy or asignment operations. The
 *  encapsulated communication handle should be closed once and once only.
 */
SerialPort::SerialPort( const SerialPort & )
{
	// Empty
}

/**
 *  @brief Destructor
 *
 *  The destructor automatically calls Close() and performes required
 *  cleanup.
 */
SerialPort::~SerialPort( void )
{
	Close() ;
}

/**
 *  @brief Open a communication resource.
 *
 *  @param pPortName - pointer to TCHAR array holding the zero terminated name
 *                     name of the communication resource.
 *  @param configInRegistry
 */
BOOL SerialPort::Open( BOOL checkIfDauDefault, const TCHAR *pPortName, BOOL configInRegistry, TCHAR *pKeyName, HKEY hKey )
{
    CString msg ;
	//if (( IsOpen() ||
	//	   (!(m_hDevice = CreateFile( pPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL ))) ))
	//{
	//	return FALSE ;
	//}
	if ( IsOpen() )
	{
		return FALSE ;
	}
	m_hDevice = CreateFile( pPortName, GENERIC_READ | GENERIC_WRITE, /*FILE_SHARE_DELETE*/ 0, NULL, /*CREATE_ALWAYS*/ OPEN_EXISTING, 0, NULL ) ;
	if ( int(m_hDevice) == -1 ) //  || m_hDevice == ERROR_ALREADY_EXISTS  ) 
	{
		if ( DeleteFile( pPortName ) )		//	Retry after trying to "delete" the "file"
		{
			m_hDevice = CreateFile( pPortName, GENERIC_READ | GENERIC_WRITE, /*FILE_SHARE_DELETE*/ 0, NULL, /*CREATE_ALWAYS*/ OPEN_EXISTING, 0, NULL ) ;
		}
	}

//	if (!(m_hDevice = CreateFile( pPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL )))
	//{
	//	return FALSE ;
	//}

	m_configInRegistry = configInRegistry ;
	m_hKey = hKey ;
	m_keyName = pKeyName ;
	m_deviceName = pPortName ;
	if (m_configInRegistry)
	{
		if( FALSE == LoadConfig() )
    {
      Close();
      return( FALSE );
    }
	}

  if( checkIfDauDefault == TRUE )
  {
    if( FALSE == IsDefault() )
    {
      Close();
      return( FALSE );
    }
  }
	return TRUE ;
}

BOOL SerialPort::GetTimeout( COMMTIMEOUTS &timeouts ) const
{
	if (!IsOpen())
	{
		return FALSE ;
	}
	return GetCommTimeouts( m_hDevice, &timeouts ) ;
}

BOOL SerialPort::SetTimeout( COMMTIMEOUTS &timeouts )
{
	if (!IsOpen())
	{
		return FALSE ;
	}
	return SetCommTimeouts( m_hDevice, &timeouts ) ;
}

void SerialPort::CancelIO( void )
{
	if (!IsOpen())
	{
		return ;
	}
	CancelIo( m_hDevice ) ;
}

BOOL SerialPort::Close( void )
{
	if (!IsOpen())
	{
		return FALSE ;
	}

	if (m_configInRegistry)
	{
		if( FALSE == SaveConfig() )
    {
	    CloseHandle( m_hDevice ) ;
	    m_hDevice = NULL ;
	    return( FALSE ) ;
    }
	}
	BOOL result = CloseHandle( m_hDevice ) ;
	m_hDevice = NULL ;
	return result ;
}

BOOL SerialPort::IsOpen( void ) const
{
	if (( m_hDevice == NULL ) || ( int(m_hDevice) == -1 ))
//	if (( m_hDevice == NULL ) || 
//			( m_hDevice == 0x00000000 ) || 
//			( m_hDevice == 0xffffffff ))
//			( int(m_hDevice) == -1 ))
	{
		return FALSE ;
	}
	else
	{
		return TRUE  ;
	}
//	return NULL != m_hDevice ;
}

void FormatMessageFromLastError( CString &message, DWORD messageId )
{
  TCHAR *pMessage ;
  FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    messageId,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    reinterpret_cast<LPTSTR>(&pMessage),
    0,
    NULL
  ) ;  
  message = pMessage ;
  LocalFree( pMessage ) ;
}

BOOL SerialPort::LoadConfig( void )
{
	if (!IsOpen())
	{
		return( FALSE ) ;
	}

	HKEY hSubKey ;
	if (ERROR_SUCCESS != RegCreateKeyEx( m_hKey, m_keyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return( FALSE ) ;
	}

	COMMCONFIG configuration ;
	DWORD configurationSize = sizeof(configuration) ;
	LONG status ;
  int ready = 2;

  //if the COMX port not in the register, 
  //set it to default values and save to the register 
  //and try once more
  do
  {
    ready--;
    status = RegQueryValueEx( hSubKey, m_deviceName.c_str(), NULL, NULL, reinterpret_cast<unsigned char *>(&configuration), &configurationSize ) ;
    
    if (ERROR_SUCCESS != status)
	  {
      if( (status == ERROR_FILE_NOT_FOUND) || (status == ERROR_PATH_NOT_FOUND) )
      {
        if( FALSE == SetToDefault() )
        {
		      RegCloseKey( hSubKey ) ;
          return( FALSE ) ;
        }
        if( FALSE == SaveConfig() )
        {
		      RegCloseKey( hSubKey ) ;
          return( FALSE ) ;
        }
      }
      else
      {
        CString message ;
        FormatMessageFromLastError( message, status ) ;
        ::AfxMessageBox( message ) ;
		    RegCloseKey( hSubKey ) ;
        return( FALSE ) ;
      }
	  }
    else
    {
      ready = 0;
    }
  } while( ready > 0 );

	if (0 == SetCommConfig( m_hDevice, &configuration, configurationSize ))
  {
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
    ::AfxMessageBox( message ) ;
    RegCloseKey( hSubKey ) ;
    return( FALSE );
  }
  RegCloseKey( hSubKey ) ;
  return( TRUE );
}

/**
 *  @todo Handle registry errors in a better way (minor fix) ;
 */
BOOL SerialPort::SaveConfig( void )
{
	if (!IsOpen())
	{
		return( FALSE ) ;
	}
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
    ::AfxMessageBox( message ) ;
		return( FALSE ) ;
	}
	HKEY hSubKey ;

	if (ERROR_SUCCESS != RegCreateKeyEx( m_hKey, m_keyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return( FALSE ) ;
	}
	
	LONG status = RegSetValueEx( hSubKey, m_deviceName.c_str(), 0, NULL, reinterpret_cast<unsigned char *>(&configuration), configurationSize ) ;
	if (ERROR_SUCCESS != status)
	{
    RegCloseKey( hSubKey ) ;
		return( FALSE ) ;
	}

	RegCloseKey( hSubKey ) ;
  return( TRUE ) ;
}

BOOL SerialPort::DoConfig( void )
{
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
    ::AfxMessageBox( message ) ;
		return( FALSE ) ;
	}

	if (0 == CommConfigDialog( m_deviceName.c_str(), NULL, &configuration ))
	{
    DWORD errorCode = GetLastError() ;
    if( errorCode != ERROR_CANCELLED )
    {
      CString message ;
      FormatMessageFromLastError( message, errorCode ) ;
      ::AfxMessageBox( message ) ;
    }
    return( FALSE ) ;
	}

	if (0 == SetCommConfig( m_hDevice, &configuration, configurationSize ))
	{
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
    ::AfxMessageBox( message ) ;
		return( FALSE ) ;
	}

  return( TRUE ) ;
}

BOOL SerialPort::SetToDefault( void )
{
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
		BYTE *pByte = new BYTE[configurationSize];

		if (0 == GetCommConfig( m_hDevice,(LPCOMMCONFIG)pByte, &configurationSize ))
		{

			CString message ;
			FormatMessageFromLastError( message, GetLastError() ) ;
			::AfxMessageBox( message ) ;
			return( FALSE ) ;
		}
		memcpy(&configuration, pByte, sizeof(configuration));
		delete pByte;
	}
        
    configuration.dcb.BaudRate = 115200;       /* Baudrate at which running       */
  //configuration.dcb.fBinary = 0;     /* Binary Mode (skip EOF check)    */
  //configuration.dcb.fParity = 1;     /* Enable parity checking          */
  configuration.dcb.fOutxCtsFlow = 0; /* CTS handshaking on output       */
  configuration.dcb.fOutxDsrFlow = 0; /* DSR handshaking on output       */
  configuration.dcb.fDtrControl = 1;  /* DTR Flow control                */
  configuration.dcb.fDsrSensitivity = 0; /* DSR Sensitivity              */
  configuration.dcb.fTXContinueOnXoff = 0; /* Continue TX when Xoff sent */
  configuration.dcb.fOutX = 0;       /* Enable output X-ON/X-OFF        */
  configuration.dcb.fInX = 0;        /* Enable input X-ON/X-OFF         */
  //configuration.dcb.fErrorChar = 0;  /* Enable Err Replacement          */
  //configuration.dcb.fNull = 0;       /* Enable Null stripping           */
  configuration.dcb.fRtsControl = 0;  /* Rts Flow control                */
  //configuration.dcb.fAbortOnError = 0; /* Abort all reads and writes on Error */
  //configuration.dcb.fDummy2 = 0;     /* Reserved                        */
  //configuration.dcb.wReserved = 0;       /* Not currently used              */
  //configuration.dcb.XonLim = 2048;          /* Transmit X-ON threshold         */
  //configuration.dcb.XoffLim = 512;         /* Transmit X-OFF threshold        */
  configuration.dcb.ByteSize = 8;        /* Number of bits/byte, 4-8        */
  configuration.dcb.Parity = 0;          /* 0-4=None,Odd,Even,Mark,Space    */
  configuration.dcb.StopBits = 0;        /* 0,1,2 = 1, 1.5, 2               */
  //configuration.dcb.XonChar = 17;         /* Tx and Rx X-ON character        */
  //configuration.dcb.XoffChar = 19;        /* Tx and Rx X-OFF character       */
  //configuration.dcb.ErrorChar = 0;       /* Error replacement char          */
  //configuration.dcb.EofChar = 0;         /* End of Input character          */
  //configuration.dcb.EvtChar = 0;         /* Received Event character        */
  //configuration.dcb.wReserved1 = 0;      /* Fill for now.                   */

	if (0 == SetCommConfig( m_hDevice, &configuration, configurationSize ))
	{
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
    ::AfxMessageBox( message ) ;
		return( FALSE ) ;
	}

  return( TRUE ) ;
}

BOOL SerialPort::IsDefault( void )
{
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
		BYTE *pByte = new BYTE[configurationSize];

		if (0 == GetCommConfig( m_hDevice,(LPCOMMCONFIG)pByte, &configurationSize ))
		{

			CString message ;
			FormatMessageFromLastError( message, GetLastError() ) ;
			::AfxMessageBox( message ) ;
			return( FALSE ) ;
		}
		memcpy(&configuration, pByte, sizeof(configuration));
		delete pByte;
	}

  if( 
    (configuration.dcb.BaudRate != 115200) ||       /* Baudrate at which running       */
    (configuration.dcb.fOutxCtsFlow != 0) || /* CTS handshaking on output       */
    (configuration.dcb.fOutxDsrFlow != 0) || /* DSR handshaking on output       */
    (configuration.dcb.fDtrControl != 1) ||  /* DTR Flow control                */
    (configuration.dcb.fDsrSensitivity != 0) || /* DSR Sensitivity              */
    (configuration.dcb.fTXContinueOnXoff != 0) || /* Continue TX when Xoff sent */
    (configuration.dcb.fOutX != 0) ||       /* Enable output X-ON/X-OFF        */
    (configuration.dcb.fInX != 0) ||        /* Enable input X-ON/X-OFF         */
    (configuration.dcb.fRtsControl != 0) ||  /* Rts Flow control                */
    (configuration.dcb.ByteSize != 8) ||        /* Number of bits/byte, 4-8        */
    (configuration.dcb.Parity != 0) ||          /* 0-4=None,Odd,Even,Mark,Space    */
    (configuration.dcb.StopBits != 0)         /* 0,1,2 = 1, 1.5, 2               */
  )
  {
    CString message ;
    message = _T("Invalid port configuration");
    ::AfxMessageBox( message ) ;
    return( FALSE ) ;
  }

  return( TRUE ) ;
}

UINT SerialPort::Write( const char *pData, UINT dataSize )
{
	if (NULL == m_hDevice)
	{
		return 0 ;
	}

	DWORD dataWritten ;
	if (WriteFile( m_hDevice, pData, dataSize, &dataWritten, NULL ))
	{
		return dataWritten ;
	}
	return 0 ;
}

UINT SerialPort::Read( char *pData, UINT dataSize )
{
	if (NULL == m_hDevice)
	{
		return 0 ;
	}

	DWORD dataRead ;
	if (ReadFile( m_hDevice, pData, dataSize, &dataRead, NULL ))
	{
		return dataRead ;
	}
	return 0 ;
}

BOOL SerialPort::Read( char *pData, unsigned int dataSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pCompletionRoutine )
{
	if (NULL == m_hDevice)
	{
		return 0 ;
	}
	BOOL status = ReadFileEx( m_hDevice, pData, dataSize, pOverlapped, pCompletionRoutine ) ;
  return status ;
}