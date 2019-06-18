// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\dau.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:01
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: dau.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "DAU.h"

#include "UserMessages.h"
#include "ProbeProgress.h"
#include "DAUSetupDialog.h"
#include "SensorDatabase.h"
#include "Database.h"
#include "SelectDAUDialog.h"
#include "Defines.h"
#include "SensorAlignmentSheet.h"
#include "DigitalProperties.h"
#include "SyncroProperties.h"
#include "SensorAlignmentPage.h"

#include "Aligner.h"
#include "PortSetup.h"
#include "SystemConfigurationView.h"

DAU &DAU::GetDAU( void )
{
  return sm_dau ;
}

DAU DAU::sm_dau ;

DAU::DAU( void )
{
  m_frequency = HIGHEST_FREQUENCY_INDEX ;
  m_errorEvent = FALSE ;
  m_enableErrorEvent = FALSE ;
  m_errorEventFlag = FALSE ;
  m_running = FALSE ;
  m_timeoutCounter = 0 ;
}

DAU::~DAU( void )
{
  ClearConfig() ;
  Close() ;
}

UINT DAU::run( void *pThis )
{
  return ((DAU *)pThis) -> RunReadThread() ;
}

class Slayer
{
public:
  Slayer( void ) {} ;
  ~Slayer( void ) {
    ::OutputDebugString( m_text ) ;
  }
  CString &operator()( void ) {
    return m_text ;
  }
private:
  CString m_text ;
} ;


UINT DAU::RunReadThread( void )
{
  UINT length = 0 ;
  while (!m_stop)
  {
#if defined(DAU_SIMULATION)
    m_simulatedData.m_mainBoard.m_temperatureChannel++ ;
    m_data = m_simulatedData ;
    Sleep( 33 ) ;
    if (m_simulationActive)
    {
      NewData() ;
    }
    continue ;
#else
    Sleep( 0 ) ;
    int bytesRead = m_connectionPort.Read( reinterpret_cast<char *>(&m_data) + length, sizeof(m_data) - length ) ;
    length += bytesRead ;
    if (0 == bytesRead)
    {
      if (m_running)
      {
        if (0 == m_timeoutCounter)
        {
          m_errorEvent = TRUE ;
          m_timeoutCounter = -1 ;
          NewData() ;
        }
        else
        {
          if (0 < m_timeoutCounter)
          {
            m_timeoutCounter-- ;
          }
        }
      }
      continue ;
    }
    else
    {
          m_timeoutCounter = 6 ;
    }
    if (sizeof(m_data) == length)
    {
      if (m_data.m_mainBoard.m_identityData == m_serial)
      {
        if (m_data.CheckChecksum())
        {
            if (!m_sampleAndHold)
            {
              m_data.InvertSensorData( m_protocolVersion ) ;
            }
            NewData() ;
        }
        else
        {
            Slayer()().Format( _T("Bad checksum\n") ) ;
        }
        length = 0 ;
        continue ;
      }
      else
      {
        for (int i = 0 ; i < sizeof(m_data) - 1 ; i++)
        {
          reinterpret_cast<char *>(&m_data) [i] = reinterpret_cast<char *>(&m_data) [i + 1] ;
        }
        length-- ;
      }
    }
#endif
  }

  return 0 ;
}

BOOL DAU::Open( const CString &dauPort, const CString &d2Port, const CString &d1Port, const CString &d3Port, const CString &d4Port, BOOL showDauSelection )
{
  Close() ;
  ClearConfig() ;

  if( dauPort.GetLength() == 0 )
  {
    ::AfxMessageBox( IDS_NO_DAU_PORT_DEFINED_RUN_DAU_PORT_SETUP ) ;
    return FALSE ;
  }

  if (!OpenPort( dauPort ) || !Probe() || !LoadConfig( d1Port, d2Port, d3Port, d4Port, showDauSelection ))
  {
    Close() ;
    AfxGetApp() -> PostThreadMessage( UM_TIMER, 0, 0 ) ;   
    return FALSE ;
  }
  m_stop = 0 ;
  ::AfxBeginThread( run, this ) ;
  return TRUE ;
}

BOOL DAU::Close( void )
{
  m_running = FALSE ;
  m_stop = 1 ;
  Sleep( 200 ) ;
  m_connectionPort.Close() ;
  return TRUE ;
}

BOOL DAU::IsOpen( void ) const
{
  return m_connectionPort.IsOpen() ;
}

void DAU::ClearConfig( void )
{
  for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    delete *i ;
  }
  for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
  {
    delete *i ;
  }
  m_sensorMap.clear() ;
  m_syncroMap.clear() ;
  m_sensorPointers.clear() ;
  m_syncroPointers.clear() ;
}

//BOOL DAU::Config( void )
//{
//  DAUSetupDialog dialog ;
//  dialog.m_mask = m_mask ;
//  dialog.m_port = m_port ;
//  dialog.m_serialNumber = GetSerialNumber() ;
//  if (IDOK == dialog.DoModal())
//  {
//    m_mask = dialog.m_mask ;
//    m_port = dialog.m_port ;
//    SetSerialNumber( dialog.m_serialNumber ) ;
//    return TRUE ;
//  }
//  return FALSE ;
//}

static const char DAU_REGISTER_SECTION []          = "" ;
static const char DAU_REGISTER_PORTNAME []         = "port" ;
static const char DAU_REGISTER_PORTNAME_DEFAULT [] = "COM1" ;
static const char DAU_REGISTER_SERIAL []           = "serial" ;
static const char DAU_REGISTER_SERIAL_DEFAULT []   = "" ;
static const char DAU_REGISTER_ID []               = "id" ;
static UINT       DAU_REGISTER_ID_DEFAULT          = 0 ;

[
  db_command { SELECT ID, serialNumber, protocolVersion, sampleAndHold FROM DAUSetup WHERE serialID = ? }
]
class /*+++ symbol was renamed:  DAUSetupFromID
 :---*/ _DAUSetupFromIDAccessor {
public:
  DECL_DB_V(1, LONG, ID) ;
  DECL_DB_L(2, TCHAR, serialNumber, 4) ;
  DECL_DB_V(3, LONG, protocolVersion) ;
	DECL_DB_V(4, LONG, sampleAndHold) ;
	PARM_DB_V(1, LONG, serialID) ;
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 220 ".\\DAU.cpp"

				DEFINE_COMMAND_EX(_DAUSetupFromIDAccessor, L" SELECT ID, serialNumber, protocolVersion, sampleAndHold FROM DAUSetup WHERE serialID = ? ")
			
#injected_line 220 ".\\DAU.cpp"

						BEGIN_ACCESSOR_MAP(_DAUSetupFromIDAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_ID),  _SIZE_TYPE(m_ID), 0, 0, offsetbuf(m_ID), 0, offsetbuf(m_status_ID))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_protocolVersion),  _SIZE_TYPE(m_protocolVersion), 0, 0, offsetbuf(m_protocolVersion), 0, offsetbuf(m_status_protocolVersion))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sampleAndHold),  _SIZE_TYPE(m_sampleAndHold), 0, 0, offsetbuf(m_sampleAndHold), 0, offsetbuf(m_status_sampleAndHold))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 220 ".\\DAU.cpp"

					BEGIN_PARAM_MAP(_DAUSetupFromIDAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialID),  _SIZE_TYPE(m_serialID), 0, 0, offsetbuf(m_serialID), 0, offsetbuf(m_status_serialID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 220 ".\\DAU.cpp"

class DAUSetupFromID;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 220 ".\\DAU.cpp"

class DAUSetupFromID : public CCommand<CAccessor<_DAUSetupFromIDAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT ID, serialNumber, protocolVersion, sampleAndHold FROM DAUSetup WHERE serialID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { SELECT [name], [offset], [order] FROM DAUSetupSyncroChannel WHERE [foreignID] = ? ORDER BY [order] ASC }
]
class /*+++ symbol was renamed:  DAUSetupSyncroFromID
 :---*/ _DAUSetupSyncroFromIDAccessor {
public:
  DECL_DB_L(1, TCHAR, name, 3) ;
  DECL_DB_V(2, LONG, offset) ;
	PARM_DB_V(1, LONG, id) ;
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 240 ".\\DAU.cpp"

				DEFINE_COMMAND_EX(_DAUSetupSyncroFromIDAccessor, L" SELECT [name], [offset], [order] FROM DAUSetupSyncroChannel WHERE [foreignID] = ? ORDER BY [order] ASC ")
			
#injected_line 240 ".\\DAU.cpp"

						BEGIN_ACCESSOR_MAP(_DAUSetupSyncroFromIDAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_name),  _SIZE_TYPE(m_name), 0, 0, offsetbuf(m_name), offsetbuf(m_length_name), offsetbuf(m_status_name))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_offset),  _SIZE_TYPE(m_offset), 0, 0, offsetbuf(m_offset), 0, offsetbuf(m_status_offset))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 240 ".\\DAU.cpp"

					BEGIN_PARAM_MAP(_DAUSetupSyncroFromIDAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_id),  _SIZE_TYPE(m_id), 0, 0, offsetbuf(m_id), 0, offsetbuf(m_status_id))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 240 ".\\DAU.cpp"

class DAUSetupSyncroFromID;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 240 ".\\DAU.cpp"

class DAUSetupSyncroFromID : public CCommand<CAccessor<_DAUSetupSyncroFromIDAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT [name], [offset], [order] FROM DAUSetupSyncroChannel WHERE [foreignID] = ? ORDER BY [order] ASC ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


BOOL DAU::LoadSensorConfig( LONG id )
{
  HRESULT hResult ;
  DAUSetupSensorFromID sensors ;
  SET_DB_VALUE(sensors,id,id) ;
  if (( FAILED(hResult = ::OpenTable( sensors )) ))
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    return FALSE ;
  }
  
  hResult = sensors.MoveFirst() ;
  while (S_OK == hResult)
  {
    CString label = sensors.m_name ;
    Sensor *pSensor = new Sensor( label, sensors.m_type ) ;
    m_sensorPointers.push_back( pSensor ) ;
    pSensor -> SetOffset( sensors.m_offset ) ;
    ChannelCalibrationData roll(sensors.m_rollOffset, sensors.m_rollGain) ;
    ChannelCalibrationData pitch(sensors.m_pitchOffset, sensors.m_pitchGain) ;
    pSensor -> SetChannelCalibrationData( roll, pitch ) ;
    m_sensorMap [label] = pSensor ;
    SystemSetup::AddSensor( sensors.m_offset ) ;
    hResult = sensors.MoveNext() ;
  }
  return TRUE ;
}

BOOL DAU::LoadSyncroConfig( LONG id )
{
  HRESULT hResult ;
  DAUSetupSyncroFromID syncros ;
  SET_DB_VALUE(syncros,id,id) ;
  if (( FAILED(hResult = ::OpenTable( syncros )) ))
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    return FALSE ;
  }
  
  hResult = syncros.MoveFirst() ;
  while (S_OK == hResult)
  {
    CString label = syncros.m_name ;
    Syncro *pSyncro = new Syncro( label ) ;
    pSyncro -> SetOffset( syncros.m_offset ) ;
    m_syncroPointers.push_back( pSyncro ) ;
    m_syncroMap [label] = pSyncro ;
    SystemSetup::AddSyncro( syncros.m_offset ) ;
    hResult = syncros.MoveNext() ;
  }
  return TRUE ;
}

BOOL DAU::LoadConfig( const CString &d1Port, const CString &d2Port, const CString &d3Port, const CString &d4Port, BOOL showDauSelection )
{
  m_mask = 255 ;
  m_frequency = HIGHEST_FREQUENCY_INDEX ;

  DAUSetupFromID table ;
#if defined(DAU_SIMULATION)
  m_serial = 7 ;
#endif
  SET_DB_VALUE(table,serialID,m_serial) ;
  if (S_OK != ::OpenTable( table ))
  {
    return FALSE ;
  }
  HRESULT hResult = table.MoveFirst() ;
  map<LONG,CString> serialList ;
  while (S_OK == hResult)
  {
    serialList [table.m_ID] = CString( table.m_serialNumber ) ;
    hResult = table.MoveNext() ;
  }

  LONG id ;
  switch (serialList.size())
  {
  case 0:
    AfxMessageBox( IDS_NO_CORRESPONDING_DAU ) ;
    return FALSE ;

  default:
    {
      SelectDAUDialog dialog (&serialList) ;
      dialog.m_dau = DAU::GetDAU().GetPreviousSerialNumber() ;
      if (showDauSelection)
      {
        dialog.DoModal() ;
      }
      SetSerialNumber( dialog.m_dau ) ;
    }
    break ;
  }

  hResult = table.MoveFirst() ;
  while (S_OK == hResult && table.m_serialNumber != GetSerialNumber()) 
  {
    hResult = table.MoveNext() ;
  }
  ASSERT(S_OK == hResult) ;
  m_protocolVersion = table.m_protocolVersion ;
  m_sampleAndHold = table.m_sampleAndHold ;
  id = table.m_ID ;

  //if (0 == m_protocolVersion)
  //{
  //  if (!DoOldLoopTest( TRUE ))
  //  {
  //    return FALSE ;
  //  }
  //}

  if (d1Port != _T(""))
  {
    Digital *pDigital = new Digital( CString( D1 ), theApp.IsAligner202Enabled() ) ;
    m_syncroPointers.push_back( pDigital ) ;
    m_syncroMap [ D1 ] = pDigital ;
    pDigital -> SetPort( d1Port ) ;
  }

  if (d2Port != _T(""))
  {
    Digital *pDigital = new Digital( CString( D2 ), theApp.IsAligner202Enabled() ) ;
    m_syncroPointers.push_back( pDigital ) ;
    m_syncroMap [ D2 ] = pDigital ;
    pDigital -> SetPort( d2Port ) ;
  }

  if (d3Port != _T(""))
  {
    Digital *pDigital = new Digital( CString( D3 ), theApp.IsAligner202Enabled() ) ;
    m_syncroPointers.push_back( pDigital ) ;
    m_syncroMap [ D3 ] = pDigital ;
    pDigital -> SetPort( d3Port ) ;
  }

  if (d4Port != _T(""))
  {
    Digital *pDigital = new Digital( CString( D4 ), theApp.IsAligner202Enabled() ) ;
    m_syncroPointers.push_back( pDigital ) ;
    m_syncroMap [ D4 ] = pDigital ;
    pDigital -> SetPort( d4Port ) ;
  }

  SystemSetup::ClearUnits() ;
  if (!LoadSyncroConfig( id ))
  {
    return FALSE ;
  }
  if (!LoadSensorConfig( id ))
  {
    return FALSE ;
  }
  return TRUE ;
}
 
// TODO, check that it's possible to save the values!
BOOL DAU::SaveConfigToRegistry( void )
{
  AfxGetApp() -> WriteProfileString( DAU_REGISTER_SECTION, DAU_REGISTER_SERIAL, GetSerialNumber() ) ;
  return TRUE ;
}

BOOL DAU::DoOldLoopTest( BOOL shortVersion )
{
  if (!IsOpen())
  {
    return FALSE ;
  }
#if defined(DAU_SIMULATION)
  return TRUE ;
#endif
  DAUData data ;
  UINT size ;
  ProbeProgress progress( shortVersion ? 60 : 655 ) ;
  progress.ShowWindow( SW_SHOW ) ;
  SendStop() ;
  Sleep( 500 ) ;
  while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
  {
    // Empty
  }
  SendSetFrequency( 0x0F ) ;
  SendSetResolution( 0xFF ) ;
  for (int DA = 0 ; DA <= 65535 ; DA += 100 + (shortVersion ? 1000 : 0) )
  {
    SendTest( 0xF0, DA >> 4 | 0xF000 ) ;
    SendStart() ;
    m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ) ;
    SendStop() ;
    if (!m_sampleAndHold)
    {
      data.InvertSensorData( m_protocolVersion ) ;
    }
    int diff = 32768 - DA - data.m_testVoltage ;
    if (abs( diff ) > 32)
    {
      CString text(_T("DAU Loop test error\n")) ;
      CString tmp ;
      tmp.Format( _T("DA = %d voltage = %d diff = %d"), DA, (int)data.m_testVoltage, diff ) ;
      text += tmp ;
      ::AfxMessageBox( text  ) ;
      return FALSE ;
    }
    progress.Step() ;
  }
  return TRUE ;

}

BOOL DAU::DoLoopTest( void )
{
  if (0 == m_protocolVersion)
  {
    return DoOldLoopTest() ;
  }
  if (!IsOpen())
  {
    return FALSE ;
  }
#if defined(DAU_SIMULATION)
  return TRUE ;
#endif
  DAUData data ;
  UINT size ;
  ProbeProgress progress( 655 ) ;
  progress.ShowWindow( SW_SHOW ) ;
  SendStop() ;
  while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
  {
    // Empty
  }
  SendSetFrequency( 0x0F ) ;
  SendSetResolution( 0xFF ) ;
  for (int DA = 0 ; DA <= 65535 ; DA += 100)
  {
    SendTest( 0, DA ) ;
    SendStart() ;
    m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ) ;
    SendStop() ;
    if (!m_sampleAndHold)
    {
      data.InvertSensorData( m_protocolVersion ) ;
    }
    int diff = 32768 - DA - data.m_testVoltage ;
    if (abs( diff ) > 12)
    {
      ::AfxMessageBox( _T("DAU Loop test error") ) ;
      return FALSE ;
    }
    progress.Step() ;
  }
  return TRUE ;
}

CString DAU::GetPreviousSerialNumber( void ) const
{
  return AfxGetApp() -> GetProfileString( DAU_REGISTER_SECTION, DAU_REGISTER_SERIAL, DAU_REGISTER_SERIAL_DEFAULT ) ;
}

void DAU::SetDefaultCalibration( void )
{
  for (unsigned int i = 0 ; i < m_sensorPointers.size() ; i++)
  {
    m_sensorPointers [i] -> SetChannelCalibrationData( ChannelCalibrationData(), ChannelCalibrationData() ) ;
  }
}

BOOL DAU::Start( DWORD milliSeconds )
{
  if (!IsOpen())
  {
    return FALSE ;
  }
  SendStop() ;
  Sleep( milliSeconds ) ;
  SendSetFrequency( m_frequency ) ;
  SendSetResolution( m_mask ) ;
  SendTest( 0, 0 ) ;
  SendStart() ;
#if defined(DAU_SIMULATION)
  m_simulationActive = TRUE ;
#endif
  m_timeoutCounter = 6 ;
  m_running = TRUE ;
  return TRUE ;
}

BOOL DAU::StartWithTest( int testVoltage )
{
#if defined(DAU_SIMULATION)
  return TRUE ;
#endif
  if (!IsOpen())
  {
    return FALSE ;
  }
  SendStop() ;
  Sleep( 100 ) ;
  SendSetFrequency( m_frequency ) ;
  SendSetResolution( m_mask ) ;
  SendTest( 0, testVoltage ) ;
  SendStart() ;
  m_timeoutCounter = 6 ;
  m_running = TRUE ;
  return TRUE ;
}

BOOL DAU::Restart( BOOL showMessages )
{
  Close();
  CString dauPortName;
  dauPortName = PortSetup::GetDAUPort();
  if( dauPortName.GetLength() == 0 )
  {
    ::AfxMessageBox( IDS_NO_DAU_PORT_DEFINED_RUN_DAU_PORT_SETUP ) ;
    return FALSE ;
  }

  //if( showMessages == TRUE )
  //{
  //  if( IDOK != ::AfxMessageBox( IDS_UNABLE_TO_OPEN_COM_PORT_CHECK_CABLES, MB_OKCANCEL ) )
  //  {
  //    ::AfxMessageBox( IDS_UNABLE_TO_OPEN_COM_PORT_CHECK_CABLES_AND_RESTART ) ;
  //    return FALSE ;
  //  }
  //}

  BOOL result = OpenPort( dauPortName );

  if ( result == FALSE )
  {
    if( showMessages == FALSE )
    {
      return FALSE ;
    }
    else
    {
      BOOL tryAgain = TRUE;
      while( tryAgain == TRUE )
      {        
        if( IDOK == ::AfxMessageBox( IDS_UNABLE_TO_OPEN_COM_PORT_CHECK_CABLES, MB_OKCANCEL ) )
        {
          dauPortName = PortSetup::GetDAUPort();
          if( dauPortName.GetLength() == 0 )
          {
            ::AfxMessageBox( IDS_NO_DAU_PORT_DEFINED_RUN_DAU_PORT_SETUP ) ;
            return FALSE ;
          }

          if (OpenPort( dauPortName ) )
          {
            tryAgain = FALSE;
          }
        }
        else
        {
          tryAgain = FALSE;
          ::AfxMessageBox( IDS_UNABLE_TO_OPEN_COM_PORT_CHECK_CABLES_AND_RESTART ) ;
          return FALSE ;
        }
      }
    }
  }

  m_stop = 0 ;
  ::AfxBeginThread( run, this ) ;
  ClearErrorEventFlag();
  return TRUE ;
}

BOOL DAU::Stop( void )
{
  SendStop() ;
#if defined(DAU_SIMULATION)
  m_simulationActive = FALSE ;
#endif
  m_running = FALSE ;
  return TRUE ;
}

#include "Aligner.h"
#include "Util.h"

void DAU::NewData( void )
{
  BOOL outOfRange = FALSE ;
  CString offendingSensor ;

  if (theApp.IsAligner202Enabled())
  {
    for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
    {
      Digital *pDigital ;
      if (( pDigital = dynamic_cast<Digital *>(*i) ))
      {
        //(*i)->GetName()
        // TODO: This will assume that a 'Digital' gyro has an offset!
        (*i) -> SetData( m_data.m_syncro [(*i) -> GetOffset()] ) ;
      }
    }

    Syncro *pA1 = GetSyncro( CString(_T("A1")) ) ;
    Syncro *pB1 = GetSyncro( CString(_T("B1")) ) ;
    Syncro *pC1 = GetSyncro( CString(_T("C1")) ) ;
    Syncro *pA2 = GetSyncro( CString(_T("A2")) ) ;
    Syncro *pB2 = GetSyncro( CString(_T("B2")) ) ;
    Syncro *pC2 = GetSyncro( CString(_T("C2")) ) ;

    if( (pA1 != NULL) && (pB1 != NULL) && (pC1 != NULL) && (pA2 != NULL) && (pB2 != NULL) && (pC2 != NULL) )
    {
      SyncroData data ;
      data = m_data.m_syncro [0] ;
      data.m_pitch = data.m_roll ; // J1 roll = A1
      pA1 -> SetData( data ) ;

      data = m_data.m_syncro [0] ;
      data.m_roll = data.m_pitch ; // B1 = J1 pitch
      pB1 -> SetData( data ) ;

      data = m_data.m_syncro [1] ;
      data.m_pitch = data.m_roll ;
      pA2 -> SetData( data ) ;

      data = m_data.m_syncro [1] ;
      data.m_roll = data.m_pitch ;
      pB2 -> SetData( data ) ;

      if (1 == pA1 -> GetType() && 2 == pB1 -> GetType())
      {
        double composite ;
        GetCompositeAngle( pA1 -> GetRoll(), pB1 -> GetRoll(), pB1 -> GetGearing(), &composite ) ;
        pC1 -> SetData( composite ) ;
      }
      else if (1 == pB1 -> GetType() && 2 == pA1 -> GetType())
      {
        double composite ;
        GetCompositeAngle( pB1 -> GetRoll(), pA1 -> GetRoll(), pA1 -> GetGearing(), &composite ) ;
        pC1 -> SetData( composite ) ;
      }
      else 
      {
        pC1 -> SetData( 0 ) ;
      }

      if (1 == pA2 -> GetType() && 2 == pB2 -> GetType())
      {
        double composite ;
        GetCompositeAngle( pA2 -> GetRoll(), pB2 -> GetRoll(), pB2 -> GetGearing(), &composite ) ;
        pC2 -> SetData( composite ) ;
      }
      else if (1 == pB2 -> GetType() && 2 == pA2 -> GetType())
      {
        double composite ;
        GetCompositeAngle( pB2 -> GetRoll(), pA2 -> GetRoll(), pA2 -> GetGearing(), &composite ) ;
        pC2 -> SetData( composite ) ;
      }
      else 
      {
        pC2 -> SetData( 0 ) ;
      }
    }
  }
  else // this is 308
  {
    for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
    {
      // TODO: This will assume that a 'Digital' gyro has an offset!
      if (!((*i) -> SetData( m_data.m_syncro [(*i) -> GetOffset()] )) && Sensor::Unused != (*i) -> GetType())
      {
        //if( (*i) -> IsSelected() || (*i) -> GetOverrangeDetectionActive() )
        if( (*i) -> GetOverrangeDetectionActive() )
        {
          offendingSensor = (*i) -> GetName() ;
          outOfRange = TRUE ;
        }
      }
    }

    for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
    {
      if (!((*i) -> SetData( m_data.m_sensor [(*i) -> GetOffset()] )) && Sensor::Unused != (*i) ->GetType())
      {
        //if( (*i)->IsSelected() || (*i) -> GetOverrangeDetectionActive() )
        if( (*i) -> GetOverrangeDetectionActive() )
        {
          offendingSensor = (*i) -> GetName() ;
          outOfRange = TRUE ;
        }
      }
    }
  }

  if (0 != m_protocolVersion)
  {
    switch (m_data.m_mainBoard.m_temperatureChannel)
    {
    case 6:

    m_mainBoard.SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
    break ;

    case 7:
      if (0 < m_sensorPointers.size())
      {
        m_sensorPointers [0] -> SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
      }
      break ;
    default:
      if ((unsigned int)(1 + m_data.m_mainBoard.m_temperatureChannel) < m_sensorPointers.size())
      {
        m_sensorPointers [1 + m_data.m_mainBoard.m_temperatureChannel] -> SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
      }
      break ;
    }
  }
  else
  {
    int sensor = m_data.m_temperature.m_old [1] & 7 ;
    if (sensor < m_sensorPointers.size())
    {
      m_sensorPointers [sensor] -> SetTemperatureData( TemperatureData( static_cast<__int16>(m_data.m_temperature.m_old [0] * 263.9358 ) ) ) ;
    }
  }
  m_mainBoard.SetData( m_data.m_mainBoard ) ;
  NotifyListeners() ;

  if ( (m_errorEvent || outOfRange) && ( TRUE == IsErrorEventEnabled() ) )
  {
    m_errorEventFlag = m_errorEvent;
    m_errorEvent = FALSE ;
    CSingleLock _(&m_listenersLock, TRUE) ;
    try
    {
      Stop();
      __raise ErrorEvent( outOfRange, offendingSensor ) ;
    }
    catch (...)
    {
    }
    EnableErrorEvent( FALSE ) ;
  }
}

Sensor *DAU::GetSensor( const CString &name )
{
  map<CString,Sensor *>::iterator position = m_sensorMap.find( name ) ;
  return position != m_sensorMap.end() ? (*position).second : NULL ;
}

Syncro *DAU::GetSyncro( const CString &name )
{
  map<CString,Syncro *>::iterator position = m_syncroMap.find( name ) ;
  return position != m_syncroMap.end() ? (*position).second : NULL ;
}

void DAU::AddListener( CWnd *pNewListener ) const
{
  ASSERT(NULL != pNewListener) ;
  CSingleLock _(&m_listenersLock, TRUE) ;
  m_listeners.insert( pNewListener ) ;
}

void DAU::RemoveListener( CWnd *pExistingListener ) const
{
  ASSERT(NULL != pExistingListener) ;
  CSingleLock _(&m_listenersLock, TRUE) ;
  m_listeners.erase( pExistingListener ) ;
}

void DAU::NotifyListeners( void )
{
  CSingleLock _(&m_listenersLock, TRUE) ;
  DAUValues newValues( *this ) ;
  try
  {
    __raise DataEvent( newValues ) ;
  }
  catch (...)
  {
  }
}

static double DegreesToRadians( double degrees )
{
  return M_PI * degrees / 180.0 ;
}

static double RadiansToDegrees( double radians )
{
  return 180.0 * radians / M_PI ;
}

BOOL DAU::ConfigSensors( BOOL sensorsOnly, BOOL hideGui )
{
  if (0 == m_sensorPointers.size() && (0 == m_syncroPointers.size() || sensorsOnly))
  {
    return( FALSE ) ;
  }

  SensorAlignmentSheet sheet( IDS_SENSORALIGNMENT_CONFIGURATION_TITLE ) ;

  if (!sensorsOnly)
  {

    for (UINT i = 0 ; i < m_syncroPointers.size() ; i++)
    {
      Digital *pDigital ;
      if (( pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ))
      {
        DigitalProperties *pProperties = new DigitalProperties ;
        pProperties -> GetPSP().pszTitle = m_syncroPointers [i] -> GetName() ;
        pProperties -> GetPSP().dwFlags |= PSP_USETITLE ;
        pProperties -> m_description = m_syncroPointers [i] -> GetUnitDescription() ;
        pProperties -> m_nominalAzimuth = m_syncroPointers [i] -> GetNominalAzimuth() ;
        Digital *pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ;
        if (pDigital)
        {
          pProperties -> m_type = pDigital-> GetSyncroType() ;
        }
        pDigital -> Stop() ;
        sheet.AddPage( pProperties ) ;
      }
      else
      {
        SyncroProperties *pProperties = new SyncroProperties ;
        pProperties -> GetPSP().pszTitle = m_syncroPointers [i] -> GetName() ;
        pProperties -> GetPSP().dwFlags |= PSP_USETITLE ;
        pProperties -> m_gearing = m_syncroPointers [i] -> GetGearing() ;
        pProperties -> m_description = m_syncroPointers [i] -> GetUnitDescription() ;
        pProperties -> m_type = static_cast<int>(m_syncroPointers [i] -> GetType()) ;
        pProperties -> m_nominalAzimuth = m_syncroPointers [i] -> GetNominalAzimuth() ;
        pProperties -> m_overrangeDetection = m_syncroPointers [i] -> GetOverrangeDetection() ;
        sheet.AddPage( pProperties ) ;
      }
    }
  }

  for (int i = (int)m_sensorPointers.size() - 1 ; 0 <= i ; i--)
  {
    SensorAlignmentPage *pProperties = new SensorAlignmentPage( sensorsOnly ) ;
    pProperties -> GetPSP().pszTitle = m_sensorPointers [i] -> GetName() ;
    pProperties -> GetPSP().dwFlags |= PSP_USETITLE ;
    CString serialNumber = m_sensorPointers [i] -> GetSerialNumber() ;
    pProperties -> m_serialNumber = 0 == m_sensorPointers [i] -> GetType() ? CString(_T("")) : serialNumber ; //Allow serialnumber 000, R2 050413
    //pProperties -> m_serialNumber = "000" == serialNumber ? CString(_T("")) : serialNumber ;
    serialNumber = m_sensorPointers [i] -> GetAdapterSerialNumber() ;
    pProperties -> m_adapterNumberString = "000" == serialNumber ? CString(_T("")) : serialNumber ;
    pProperties -> m_description = m_sensorPointers [i] -> m_description ;
    pProperties -> m_overrangeDetection = m_sensorPointers [i] -> m_overrangeDetection ;
    pProperties -> m_type = m_sensorPointers [i] -> GetType() ;
    pProperties -> m_nominalAzimuth = m_sensorPointers [i] -> GetNominalAzimuth() ;
    sheet.AddPage( pProperties ) ;
  }

  int result = IDOK;
  if (hideGui || IDOK == (result = sheet.DoModal()))
  {
    if (!sensorsOnly)
    {
      for (UINT i = 0 ; i < m_syncroPointers.size() ; i++)
      {
        Digital *pDigital ;
        if (( pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ))
        {
          DigitalProperties *pProperties = static_cast<DigitalProperties *>(sheet.GetPage( i )) ;
          m_syncroPointers [i] -> SetType( static_cast<Unit::Types>(0 != pProperties -> m_type ? 1 : 0) ) ;
          Digital *pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ;
          if (pDigital)
          {
            pDigital -> SetSyncroType( static_cast<Unit::Types>(pProperties -> m_type) ) ;
          }
          m_syncroPointers [i] -> m_description = pProperties -> m_description ;
          m_syncroPointers [i] -> SetNominalAzimuth( pProperties -> m_nominalAzimuth ) ;
          if (0 != pDigital -> GetType())
          {
            pDigital -> Start() ;
          }
        }
        else
        {    
          SyncroProperties *pProperties = static_cast<SyncroProperties *>(sheet.GetPage( i )) ;
          m_syncroPointers [i] -> SetType( static_cast<Unit::Types>(pProperties -> m_type) ) ;
          m_syncroPointers [i] -> SetGearing( pProperties -> m_gearing ) ;
          m_syncroPointers [i] -> m_description = pProperties -> m_description ;
          m_syncroPointers [i] -> SetNominalAzimuth( pProperties -> m_nominalAzimuth ) ;
          m_syncroPointers [i] -> SetOverrangeDetection( pProperties -> m_overrangeDetection ) ;
          m_syncroPointers [i] -> SetIsSelected( FALSE );
        }
      }
      if( theApp.IsAligner202Enabled() == TRUE )
      {
        //check if composite channel, C, must be disabled, unused
        CString name;
        int type, typeA1 = -1, typeB1 = -1, typeA2 = -1, typeB2 = -1;
        int syncroPointerC1 = -1, syncroPointerC2 = -1;
        for (UINT i = 0 ; i < m_syncroPointers.size() ; i++)
        {
          name = m_syncroPointers[i]->GetName();
          type = static_cast<int>( m_syncroPointers[i]->GetType() );

          if( name == C1 )
          {
            syncroPointerC1 = i;
          }
          else if( name == C2 )
          {
            syncroPointerC2 = i;
          }
          else if( ( name == A1 ) && ( type != 0 ) )
          {
            typeA1 = type;
          }
          else if( ( name == B1 ) && ( type != 0 ) )
          {
            typeB1 = type;
          }
          else if( ( name == A2 ) && ( type != 0 ) )
          {
            typeA2 = type;
          }
          else if( ( name == B2 ) && ( type != 0 ) )
          {
            typeB2 = type;
          }
        }
        
        if( syncroPointerC1 != -1 )
        {
          if( ( typeA1 == -1 ) || ( typeB1 == -1 ) || ( typeA1 == typeB1 )  )
          {
            m_syncroPointers[syncroPointerC1]->SetType( static_cast<Unit::Types>( 0 ) );
          }
        }
        if( syncroPointerC2 != -1 )
        {
          if( ( typeA2 == -1 ) || ( typeB2 == -1 ) || ( typeA2 == typeB2 )  )
          {
            m_syncroPointers[syncroPointerC2]->SetType( static_cast<Unit::Types>( 0 ) );
          }
        }
      }      
    }
    for (int i = static_cast<int>(m_sensorPointers.size() - 1) ; 0 <= i ; i--)
    {
      SensorAlignmentPage *pProperties = static_cast<SensorAlignmentPage *>(sheet.GetPage( static_cast<int>((sensorsOnly ? 0 : m_syncroPointers.size()) + m_sensorPointers.size() - i - 1) )) ;
      m_sensorPointers [i] -> SetSerialNumber( pProperties -> m_serialNumber ) ;
      m_sensorPointers [i] -> SetAdapterSerialNumber( pProperties -> m_adapterNumberString ) ;
      m_sensorPointers [i] -> m_description = pProperties -> m_description ;
      m_sensorPointers [i] -> m_overrangeDetection = pProperties -> m_overrangeDetection ;
      m_sensorPointers [i] -> SetType( static_cast<Unit::Types>(pProperties -> m_type) ) ;
      m_sensorPointers [i] -> SetNominalAzimuth( pProperties -> m_nominalAzimuth ) ;
      m_sensorPointers [i] -> LoadCalibration() ;
      m_sensorPointers [i] -> SetIsSelected( FALSE );
    }
  }

  while (0 < sheet.GetPageCount())
  {
    CPropertyPage *pProperties = sheet.GetPage( 0 ) ;
    sheet.RemovePage( pProperties ) ;
    delete pProperties ;
  }

  if( IDOK == result )
  {
    return( TRUE );
  }
  else
  {
    return( FALSE );
  }
}

BOOL DAU::OpenPort( const CString port )
{
  if (!m_connectionPort.Open( TRUE, m_port = port, TRUE ))
  {
    return FALSE ;
  }

  COMMTIMEOUTS timeout ;
  timeout.ReadIntervalTimeout = 0 ;
  timeout.ReadTotalTimeoutConstant = 200 ;
  timeout.ReadTotalTimeoutMultiplier = 0 ;
  timeout.WriteTotalTimeoutConstant = 0 ;
  timeout.WriteTotalTimeoutMultiplier = 0 ;
  if (!m_connectionPort.SetTimeout( timeout ))
  {
    m_connectionPort.Close() ;
    return FALSE ;
  }
  return TRUE ;
}

BOOL DAU::Probe( void )
{
  DAUData data ;
  UINT size ;
  ProbeProgress progress( 20 ) ;
  progress.ShowWindow( SW_SHOW ) ;
  SendStop() ;
  Sleep( 200 ) ;
  while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
  {
    // Empty
  }
  SendSetFrequency( 0x0F ) ;
  SendSetResolution( 0xFF ) ;
  SendTest( (-15) & 0xFF, (-4095) & 0xFFFF ) ;
  SendStart() ;
  progress.Step() ;
  m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ) ;
#if defined(DAU_SIMULATION)
  m_serial = 1 ;
  return TRUE ;
#else

  m_serial = (data.m_mainBoard.m_identityData ^ (__int8)(-15)) & 0xFF ;
#endif
  SendStop() ;
  progress.Step() ;
  if (data.m_mainBoard.m_identityData == 1 || m_serial == 1)
  {
    m_serial = 1 ;
    return TRUE ; // DoOldLoopTest( TRUE ) ;
  }
  while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
  {
    // Empty
  }
  for (int i = 0x01 ; i <= 0x80 ; i <<= 1)
  {
    progress.Step() ;
#if defined(DAU_SIMULATION)
    Sleep(10) ;
    continue ;
#endif
    if (1 != m_serial)
    {
      SendTest( i, 0 ) ;
    }
    else
    {
      SendTest( -15, 0 ) ;
    }
    SendStart() ;
    size = m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ) ;
    data.m_mainBoard.m_identityData ^= 1 != m_serial ? m_serial : (__int8)(-15) ;
    if (sizeof(data) != size || data.m_mainBoard.m_identityData != i)
    {
      Close() ;
      return FALSE ;
    }
    SendStop() ;
    progress.Step() ;
    while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
    {
      // Empty
    }
  }
  SendTest( 0, 0 ) ;
  SendStop() ;
  progress.Step() ;
  while (m_connectionPort.Read( reinterpret_cast<char *>(&data), sizeof(data) ))
  {
    // Empty
  }
  progress.Step() ;
  return TRUE ;
}

BOOL DAU::SendStart( void )
{
  const char startCommand [] = { '\x01', '\xF9' } ;
  return Send( startCommand, sizeof(startCommand) ) ;
}

BOOL DAU::SendStop( void )
{
  const char stopCommand [] = { '\x01', '\xF8' } ;
  return Send( stopCommand, sizeof(stopCommand) ) ;
}

BOOL DAU::SendReset( void )
{
  const char resetCommand [] = { '\x01', '\xFA' } ;
  return Send( resetCommand, sizeof(resetCommand) ) ;
}

BOOL DAU::SendResetAndStart( void )
{
  const char resetAndStartCommand [] = { '\x01', '\xFB' } ;
  return Send( resetAndStartCommand, sizeof(resetAndStartCommand) ) ;
}

BOOL DAU::SendSetFrequency( int frequency )
{
  ASSERT(frequency >= 0) ;
  ASSERT(frequency <= 0x1F) ;
  char frequencyCommand [] = { '\x02', '\xE0' } ;
  frequencyCommand [1] |= static_cast<char>(frequency & 0x1F) ;
  m_period = BitfieldToPeriod( frequency ) ;
  return Send( frequencyCommand, sizeof(frequencyCommand) ) ;
}

BOOL DAU::SendSetResolution( int mask )
{
  ASSERT(mask >= 0) ;
  ASSERT(mask <= 0xFF) ;
  char maskCommand [] = { '\x08', '\x00' } ;
  maskCommand [1] |= static_cast<char>(mask & 0xFF) ;
  return Send( maskCommand, sizeof(maskCommand) ) ;
}

BOOL DAU::SendTest( int data, int voltage )
{
  ASSERT(data >= 0) ;
  ASSERT(data <= 0xFF) ;
  ASSERT(voltage >= 0) ;
  ASSERT(voltage <= 0xFFFF) ;
  char testCommand [] = { '\x04', '\x00', '\x00', '\x00' } ;
  testCommand [1] |= static_cast<char>(data & 0xFF) ;
  testCommand [2] |= static_cast<char>(voltage & 0xFF) ;
  testCommand [3] |= static_cast<char>((voltage >> 8 )& 0xFF) ;
  return Send( testCommand, sizeof(testCommand) ) ;
}

BOOL DAU::Send( const char *pData, UINT size, BOOL skipPause )
{
  do
  {
    UINT sizeSend = size ;
    if (!m_connectionPort.Write( pData, sizeSend ))
    {
      return FALSE ;
    }
    pData += sizeSend ;
    size -= sizeSend ;
  }
  while (0 < size) ;
  if (!skipPause)
  {
    Sleep( 1 ) ;
  }
  return TRUE ;
}

double DAU::BitfieldToFrequency( UINT bitfield )
{
  ASSERT(bitfield<0x20) ;
  return 4000000.0 / (double)(1L << (bitfield & 0x1F)) ;
}

double DAU::BitfieldToPeriod( UINT bitfield )
{
  ASSERT(bitfield<0x20) ;
  return 1.0 / BitfieldToFrequency( bitfield ) ;
}

DAUValues::DAUValues( DAU &source )
{
  for (int i = 0 ; i < source.GetSensorCount() ; i++)
  {
    Sensor *pSensor = source.GetSensor( i ) ;
    m_sensors.push_back( SingleSensor(
      pSensor -> GetRoll(),
      pSensor -> GetPitch(),
      pSensor -> GetRoll( FALSE ),
      pSensor -> GetPitch( FALSE ),
      pSensor -> GetData().m_roll,
      pSensor -> GetData().m_pitch,
      pSensor -> GetTemperature(),
      pSensor -> GetTemperatureData().m_temperature ) ) ;
  }
  for (int i = 0 ; i < source.GetSyncroCount() ; i++)
  {
    Syncro *pSyncro = source.GetSyncro( i ) ;
    m_syncros.push_back( SingleSensor(
      pSyncro -> GetRoll(),
      pSyncro -> GetPitch(),
      pSyncro -> GetRoll( FALSE ),
      pSyncro -> GetPitch( FALSE ),
      pSyncro -> GetData().m_roll,
      pSyncro -> GetData().m_pitch ) ) ;
  }
}

DAUValues::DAUValues( void )
{
  // Empty.
}

DAUValues::DAUValues( const DAUValues &source )
{
  m_sensors.insert( m_sensors.begin(), source.m_sensors.begin(), source.m_sensors.end() ) ;
  m_syncros.insert( m_syncros.begin(), source.m_syncros.begin(), source.m_syncros.end() ) ;
}

void DAUValues::Set( const DAUValues &source )
{
  m_sensors.clear() ;
  m_syncros.clear() ;
  m_sensors.insert( m_sensors.begin(), source.m_sensors.begin(), source.m_sensors.end() ) ;
  m_syncros.insert( m_syncros.begin(), source.m_syncros.begin(), source.m_syncros.end() ) ;
}

void DAU::SetOverrange( BOOL flag )
{
  for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    (*i) -> SetOverrangeDetectionActive( flag ) ;
  }
  for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
  {
    (*i) -> SetOverrangeDetectionActive( flag ) ;
  }
}

int DAU::GetProtocolVersion( void )
{
  return m_protocolVersion ;
}

void DAU::SetIgnoreElevation( BOOL ignoreElevation )
{
    for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
    {
        (*i) -> SetIgnoreElevation( ignoreElevation ) ;
    }
}
