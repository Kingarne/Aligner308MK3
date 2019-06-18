#include "stdafx.h"
#include "SerialPort.h"

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

SerialPort::SerialPort( const SerialPort & )
{
	// Empty
}

SerialPort::~SerialPort( void )
{
	Close() ;
}

BOOL SerialPort::Open( const TCHAR *pPortName, BOOL configInRegistry, TCHAR *pKeyName, HKEY hKey )
{
    CString msg ;
	CString port;
	port.Format("\\\\.\\%s",pPortName);
	if (( IsOpen() ||
		   (INVALID_HANDLE_VALUE == (m_hDevice = CreateFile( port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL ))) ))
	{
		return FALSE ;
	}
	m_configInRegistry = configInRegistry ;
	m_hKey = hKey ;
	m_keyName = pKeyName ;
	m_deviceName = port ;
	if (m_configInRegistry)
	{
		LoadConfig() ;
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
		SaveConfig() ;
	}
	BOOL result = CloseHandle( m_hDevice ) ;
	m_hDevice = NULL ;
	return result ;
}

BOOL SerialPort::IsOpen( void ) const
{
	return NULL != m_hDevice ;
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

void SerialPort::LoadConfig( void )
{
	if (!IsOpen())
	{
		return ;
	}

	HKEY hSubKey ;
	if (ERROR_SUCCESS != RegCreateKeyEx( m_hKey, m_keyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return ;
	}

	COMMCONFIG configuration ;
	DWORD configurationSize = sizeof(configuration) ;
	LONG status = RegQueryValueEx( hSubKey, m_deviceName.c_str(), NULL, NULL, reinterpret_cast<unsigned char *>(&configuration), &configurationSize ) ;
	if (ERROR_SUCCESS != status)
	{
		RegCloseKey( hSubKey ) ;
		return ;
	}
	if (0 == SetCommConfig( m_hDevice, &configuration, configurationSize ))
  {
    CString message ;
    FormatMessageFromLastError( message, GetLastError() ) ;
  }
	RegCloseKey( hSubKey ) ;
}

/**
 *  @todo Handle registry errors in a better way (minor fix) ;
 */
void SerialPort::SaveConfig( void )
{
	if (!IsOpen())
	{
		return ;
	}
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
		return ;
	}
	HKEY hSubKey ;

	if (ERROR_SUCCESS != RegCreateKeyEx( m_hKey, m_keyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return ;
	}
	
	LONG status = RegSetValueEx( hSubKey, m_deviceName.c_str(), 0, NULL, reinterpret_cast<unsigned char *>(&configuration), configurationSize ) ;
	if (ERROR_SUCCESS != status)
	{
		/* */
	}

	RegCloseKey( hSubKey ) ;
}

void SerialPort::DoConfig( void )
{
	COMMCONFIG configuration ;
	DWORD configurationSize ;
	configurationSize = sizeof(configuration) ;

	if (0 == GetCommConfig( m_hDevice, &configuration, &configurationSize ))
	{
        CString msg ;
        FormatMessageFromLastError( msg, GetLastError() ) ;
        ::AfxMessageBox( msg ) ;
		return ;
	}

	configuration.dcb.BaudRate = 9600;
// 	if (0 == CommConfigDialog( m_deviceName.c_str(), NULL, &configuration ))
// 	{
//     DWORD errorCode = GetLastError() ;
//     if( errorCode != ERROR_CANCELLED )
//     {
//       CString message ;
//       FormatMessageFromLastError( message, errorCode ) ;
//       ::AfxMessageBox( message ) ;
//     }
//     return ;
// 	}

	if (0 == SetCommConfig( m_hDevice, &configuration, configurationSize ))
	{
		return ;
	}
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