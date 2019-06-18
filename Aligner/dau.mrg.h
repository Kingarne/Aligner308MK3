// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\dau.mrg.h
// compiler-generated file created 05/10/06 at 15:51:56
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

// $Id: dau.mrg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
#pragma pack(push)
#pragma pack(1)

class DAUValues ;

#define HIGHEST_FREQUENCY_INDEX 16  // 61.04 Hz
#define LOWEST_FREQUENCY_INDEX  22  // 0.95 Hz

#include "Sensor.h"
#include "MainBoard.h"
#include "SerialPort.h"
#include "Digital.h"
#include "Database.h"

/**
*  @brief Encapsulation of a Data Acqusition Unit (DAU).
*
*  @note Exit must be called or the instance destroyed before the application
*  terminates. If not, an access violation may occur.       OO
*/
[
  event_source(native)
]
class DAU : public SerialNumber
{
public:
  DAU( void ) ;
  virtual ~DAU( void ) ;
public:
  __event void DataEvent( const DAUValues &data ) ;
  __event void ErrorEvent( BOOL overflow, CString offendingSensor ) ;
public:
  BOOL Open( const CString &dauPort, const CString &d1Port, const CString &d2Port, const CString &d3Port, const CString &d4Port, BOOL showDauSelection = TRUE ) ;
  BOOL IsOpen( void ) const ;
  BOOL Close( void ) ;
  BOOL Start( DWORD milliSeconds = 200 ) ;
  BOOL StartWithTest( int testVoltage ) ;
  BOOL Restart( BOOL showMessages = TRUE );
  BOOL Stop( void ) ;
  BOOL ConfigSensors( BOOL sensorsOnly = TRUE, BOOL hideGui = FALSE ) ;
  void AddListener( CWnd *pNewListener ) const ;
  void RemoveListener( CWnd *pExistingListener ) const ;
  BOOL SaveConfigToRegistry( void ) ;
  BOOL DoLoopTest( void ) ;
  BOOL DoOldLoopTest( BOOL shortVersion = FALSE ) ;
  void SetOverrange( BOOL flag ) ;
  int GetProtocolVersion( void ) ;
  void SetIgnoreElevation( BOOL ignoreElevation ) ;
#if defined(DAU_SIMULATION)
  void DoSimulation( void ) ;
#endif
private:
  void ClearConfig( void ) ;
  BOOL LoadConfig( const CString &d1Port, const CString &d2Port, const CString &d3Port, const CString &d4Port, BOOL showDauSelection = TRUE ) ;
  BOOL LoadSensorConfig( LONG id ) ;
  BOOL LoadSyncroConfig( LONG id ) ;
  //BOOL Config( void ) ;
  BOOL OpenPort( const CString port ) ;
  BOOL Probe( void ) ;
  BOOL Send( const char *pData, UINT size, BOOL skipPause = FALSE ) ;
  BOOL SendSetFrequency( int frequency ) ;
  BOOL SendSetResolution( int mask ) ;
  BOOL SendStart( void ) ;
  BOOL SendStop( void ) ;
  BOOL SendReset( void ) ;
  BOOL SendResetAndStart( void ) ;
  BOOL SendTest( int data, int voltage ) ;
  void SetDefaultCalibration( void ) ;
  CString DAU::GetPreviousSerialNumber( void ) const ;
public:
  BOOL GetEnableAzimuthMeasurment( void ) const ;
  BOOL GetEnableSperryMeasurment( void ) const ;
public:
  virtual void NewData( void ) ;
  void NotifyListeners( void ) ;
public:
  BOOL IsCalibrated( void ) const ;
  int GetProtocolVersion( void ) const ;
  void EnableErrorEvent( BOOL enable ) ;
  BOOL IsErrorEventEnabled( void ) const ;
  BOOL IsErrorEventFlag( void ) const ;
  void ClearErrorEventFlag( void );
public:
  Sensor *GetSensor( int i ) ;
  Sensor *GetSensor( const CString &name ) ;
  int GetSensorCount( void ) const ;
  int GetConnectedSensorCount( void ) const ;
  int GetConnectedSensorCount( BOOL skipAdapters ) const ;
  Syncro *GetSyncro( int i ) ;
  Syncro *GetSyncro( const CString &name ) ;
  int GetSyncroCount( void ) const ;
  const MainBoard *GetMainboard( void ) const ;
private:
  static UINT run( LPVOID pThis ) ;
  UINT RunReadThread( void ) ;
  UINT ReadSample( void ) ;
  mutable CCriticalSection m_listenersLock ;
  mutable set<CWnd *> m_listeners ;
public:
  int GetMask( void ) const ;
  BOOL SetResolutionMask( int mask );
  int GetFrequency( void ) const ;
  void SetFrequency( int frequency ) ;
  BOOL GetEnableAzimuthMeasurement( void ) const;
  BOOL GetEnableSperryMeasurement( void ) const;
  void SetEnableAzimuthMeasurement( BOOL flag ) ;
  void SetEnableSperryMeasurement( BOOL flag ) ;
  short GetMeasuredTestVoltage( void );
private:
  int m_frequency ;
  CString m_port ;
  int m_mask ;
  int m_stop ;
  SerialPort m_connectionPort ;
  BOOL m_isCalibrated ;
  int m_serial ;
  double m_period ;
  OVERLAPPED m_overlapped ;
  DAUData m_data ;
private:
  LONG m_protocolVersion ;
  BOOL m_sampleAndHold ;
  BOOL m_enableSperry ;
  BOOL m_enableAzimuth ;
private:
  MainBoard m_mainBoard ;
  vector<Sensor *> m_sensorPointers ;
  vector<Syncro *> m_syncroPointers ;
  vector<Digital *> m_digitalPointers ;
  map<CString, Sensor *> m_sensorMap ;
  map<CString, Syncro *> m_syncroMap ;
public:
  static double BitfieldToFrequency( UINT bitfield ) ;
  static double BitfieldToPeriod( UINT bitfield ) ;
public:
  static DAU &DAU::GetDAU( void ) ;
private:
  static DAU sm_dau ;
  BOOL m_running ;
  int m_timeoutCounter ;
  BOOL m_errorEvent ;
  BOOL m_enableErrorEvent ; // added by R2 040628
  BOOL m_errorEventFlag ;// added by R2 050324
#if defined(DAU_SIMULATION)
private:
  BOOL m_simulationActive ;
  DAUData m_simulatedData ;
public:
  DAUData *GetSimulationDataPointer( void ) {
    return &m_simulatedData ;
  }
  BOOL *GetSimulationErrorPointer( void ) {
    return &m_errorEvent ;
  }
#endif

	//+++ Start Injected Code For Attribute 'event'
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
    __EventingCriticalSectionStub __EventingCS;
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
    struct __eventNode_DAU_DataEvent 
    {
        virtual void  __invoke(const ::DAUValues& i1) = 0;
        virtual int __isEqual(void*, void*) = 0;
        virtual int __isEqual(void*) = 0;
        __eventNode_DAU_DataEvent* next;
    };
    template <class T>
        struct __InvokeHandlers_DAU_DataEvent : __eventNode_DAU_DataEvent 
    {
        T* p;
        void ( T::*pmfn) (const ::DAUValues& i1);
        void  __invoke(const ::DAUValues& i1) 
        {
            return (p->*pmfn) (i1);
        }
        int __isEqual(void* p, void* pfn) 
        {
            return ((T*) p == this->p) && (__eventingGetAddr<T>::__getMAddr(pfn) == (void (T::*) ()) pmfn);
        }
        int __isEqual(void* p) 
        {
            return ((T*) p == this->p);
        }
    };
    __eventNode_DAU_DataEvent* __eventHandlerList_DAU_DataEvent;
    template <class T>
        long __AddEventHandler_DAU_DataEvent(T* pThis, void ( T::*pmfn) (const ::DAUValues& i1)) 
    {
        __InvokeHandlers_DAU_DataEvent<T>* newHead = new __InvokeHandlers_DAU_DataEvent<T>;
        newHead->p = pThis;
        newHead->pmfn = pmfn;
        __EventingCS.Lock();
        newHead->next = __eventHandlerList_DAU_DataEvent;
        __eventHandlerList_DAU_DataEvent = (__eventNode_DAU_DataEvent*) newHead;
        __EventingCS.Unlock();
        return 0;
    }
    template <class T>
        long __RemoveEventHandler_DAU_DataEvent(T* pThis, void ( T::*pmfn) (const ::DAUValues& i1)) 
    {
        __EventingCS.Lock();
        __eventNode_DAU_DataEvent* node = __eventHandlerList_DAU_DataEvent;
        __eventNode_DAU_DataEvent* prev = 0;
        for (; node != 0; node = node->next) {
            if (node->__isEqual(pThis,
                            __eventingGetAddr<T>::__getVAddr((void (T::*) ()) pmfn)) != 0) {
                if (prev == 0) {
                    __eventHandlerList_DAU_DataEvent = node->next;
                } 
                else {
                    prev->next = node->next;
                }
                delete node;
                __EventingCS.Unlock();
                return 0;
            }
            prev = node;
        }
        __EventingCS.Unlock();
        return 1;
    }
    template <class T>
        void __RemoveAllEventHandlers_DAU_DataEvent(T* pThis) 
    {
        __EventingCS.Lock();
        __eventNode_DAU_DataEvent* node = __eventHandlerList_DAU_DataEvent;
        __eventNode_DAU_DataEvent* prev = 0;
        while (node != 0) {
            if (node->__isEqual(pThis) != 0) {
                __eventNode_DAU_DataEvent* dead = node;
                if (prev == 0) {
                    __eventHandlerList_DAU_DataEvent = node = node->next;
                } 
                else {
                    prev->next = node = node->next;
                }
                delete dead;
            } 
            else {
                prev = node;
                node = node->next;
            }
        }
        __EventingCS.Unlock();
    }
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
    struct __eventNode_DAU_ErrorEvent 
    {
        virtual void  __invoke(::BOOL i1,::CString i2) = 0;
        virtual int __isEqual(void*, void*) = 0;
        virtual int __isEqual(void*) = 0;
        __eventNode_DAU_ErrorEvent* next;
    };
    template <class T>
        struct __InvokeHandlers_DAU_ErrorEvent : __eventNode_DAU_ErrorEvent 
    {
        T* p;
        void ( T::*pmfn) (::BOOL i1,::CString i2);
        void  __invoke(::BOOL i1,::CString i2) 
        {
            return (p->*pmfn) (i1,i2);
        }
        int __isEqual(void* p, void* pfn) 
        {
            return ((T*) p == this->p) && (__eventingGetAddr<T>::__getMAddr(pfn) == (void (T::*) ()) pmfn);
        }
        int __isEqual(void* p) 
        {
            return ((T*) p == this->p);
        }
    };
    __eventNode_DAU_ErrorEvent* __eventHandlerList_DAU_ErrorEvent;
    template <class T>
        long __AddEventHandler_DAU_ErrorEvent(T* pThis, void ( T::*pmfn) (::BOOL i1,::CString i2)) 
    {
        __InvokeHandlers_DAU_ErrorEvent<T>* newHead = new __InvokeHandlers_DAU_ErrorEvent<T>;
        newHead->p = pThis;
        newHead->pmfn = pmfn;
        __EventingCS.Lock();
        newHead->next = __eventHandlerList_DAU_ErrorEvent;
        __eventHandlerList_DAU_ErrorEvent = (__eventNode_DAU_ErrorEvent*) newHead;
        __EventingCS.Unlock();
        return 0;
    }
    template <class T>
        long __RemoveEventHandler_DAU_ErrorEvent(T* pThis, void ( T::*pmfn) (::BOOL i1,::CString i2)) 
    {
        __EventingCS.Lock();
        __eventNode_DAU_ErrorEvent* node = __eventHandlerList_DAU_ErrorEvent;
        __eventNode_DAU_ErrorEvent* prev = 0;
        for (; node != 0; node = node->next) {
            if (node->__isEqual(pThis,
                            __eventingGetAddr<T>::__getVAddr((void (T::*) ()) pmfn)) != 0) {
                if (prev == 0) {
                    __eventHandlerList_DAU_ErrorEvent = node->next;
                } 
                else {
                    prev->next = node->next;
                }
                delete node;
                __EventingCS.Unlock();
                return 0;
            }
            prev = node;
        }
        __EventingCS.Unlock();
        return 1;
    }
    template <class T>
        void __RemoveAllEventHandlers_DAU_ErrorEvent(T* pThis) 
    {
        __EventingCS.Lock();
        __eventNode_DAU_ErrorEvent* node = __eventHandlerList_DAU_ErrorEvent;
        __eventNode_DAU_ErrorEvent* prev = 0;
        while (node != 0) {
            if (node->__isEqual(pThis) != 0) {
                __eventNode_DAU_ErrorEvent* dead = node;
                if (prev == 0) {
                    __eventHandlerList_DAU_ErrorEvent = node = node->next;
                } 
                else {
                    prev->next = node = node->next;
                }
                delete dead;
            } 
            else {
                prev = node;
                node = node->next;
            }
        }
        __EventingCS.Unlock();
    }
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
    void __RemoveAllEventHandlers(void* pThis) 
    {
        __RemoveAllEventHandlers_DAU_DataEvent(pThis);
        __RemoveAllEventHandlers_DAU_ErrorEvent(pThis);
    }

	//--- End Injected Code For Attribute 'event'
} ;

//+++ Start Injected Code For Attribute 'event'
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
inline void  DAU::DataEvent(const ::DAUValues& i1) 
{
    __EventingCS.Lock();
    __eventNode_DAU_DataEvent* node = __eventHandlerList_DAU_DataEvent;
    for (; node != 0; node = node->next) {
        node->__invoke(i1);
    }
    __EventingCS.Unlock();
}
#injected_line 36 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"
inline void  DAU::ErrorEvent(::BOOL i1,::CString i2) 
{
    __EventingCS.Lock();
    __eventNode_DAU_ErrorEvent* node = __eventHandlerList_DAU_ErrorEvent;
    for (; node != 0; node = node->next) {
        node->__invoke(i1,i2);
    }
    __EventingCS.Unlock();
}

//--- End Injected Code For Attribute 'event'


inline
short DAU::GetMeasuredTestVoltage( void ) {
	return( m_data.m_testVoltage );
}

inline
Sensor *DAU::GetSensor( int i ) {
  return m_sensorPointers [i] ;
}

inline
int DAU::GetSensorCount( void ) const {
  return (int)(m_sensorPointers.size()) ;
}

inline
int DAU::GetConnectedSensorCount( void ) const {
  int connected = 0 ;
  for (vector<Sensor *>::const_iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    if (Unit::Unused != (*i) -> GetType())
    {
      connected++ ;
    }
  }
  return connected ;
}

inline
int DAU::GetConnectedSensorCount( BOOL skipAdapters ) const {
  int connected = 0 ;
  for (vector<Sensor *>::const_iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    if (Unit::Unused != (*i) -> GetType())
    {
      if ((skipAdapters && Unit::Gun != (*i) -> GetType()) ||
        (!skipAdapters && Unit::Gun == (*i) -> GetType()))
      {
        connected++ ;
      }
    }
  }
  return connected ;
}

inline
Syncro *DAU::GetSyncro( int i ) {
  return m_syncroPointers [i] ;
}

inline
int DAU::GetSyncroCount( void ) const {
  return (int)(m_syncroPointers.size()) ;
}

inline
const MainBoard *DAU::GetMainboard( void ) const {
  return &m_mainBoard ;
}

inline
int DAU::GetMask( void ) const {
  return m_mask ;
}

inline 
BOOL DAU::SetResolutionMask( int mask ) {
  if( ( mask < 0x00 ) || ( mask > 0xFF ) )
  {
    return( FALSE );
  }
  m_mask = mask;
  return( TRUE );
}

inline
int DAU::GetFrequency( void ) const {
  return m_frequency ;
}

inline
void DAU::SetFrequency( int frequency ) {
  m_frequency = frequency ;
}

inline
BOOL DAU::GetEnableAzimuthMeasurement( void ) const
{
  return m_enableAzimuth ;
}

inline
BOOL DAU::GetEnableSperryMeasurement( void ) const
{
  return m_enableSperry ;
}

inline
void DAU::SetEnableAzimuthMeasurement( BOOL flag ) 
{
  m_enableAzimuth = flag ;
}

inline
void DAU::SetEnableSperryMeasurement( BOOL flag )
{
  m_enableSperry = flag ;
}


inline
int DAU::GetProtocolVersion( void ) const
{
  return m_protocolVersion ;
}

inline
void DAU::EnableErrorEvent( BOOL enable )
{
  m_enableErrorEvent = enable;
}

inline
BOOL DAU::IsErrorEventEnabled( void ) const
{
  return( m_enableErrorEvent );
}

inline
BOOL DAU::IsErrorEventFlag( void ) const 
{
  return( m_errorEventFlag );
}

inline
void DAU::ClearErrorEventFlag( void )
{
  m_errorEventFlag = FALSE;
}

class SingleSensor
{
public:
  SingleSensor() {} ;
  SingleSensor( double roll, double pitch, double uncalibratedRoll, double uncalibratedPitch, __int16 rawRoll, __int16 rawPitch, double temperature = 0, __int16 rawTemperature = 0  ) :
  m_roll(roll), m_pitch(pitch), m_uncalibratedRoll(uncalibratedRoll), m_uncalibratedPitch(uncalibratedPitch), m_rawRoll(rawRoll), m_rawPitch(rawPitch), m_temperature(temperature), m_rawTemperature(rawTemperature) {
  } ;
public:
  double GetRoll( void ) const {
    return m_roll ;
  }
  double GetPitch( void ) const {
    return m_pitch ;
  }
  double GetUncalibratedRoll( void ) const {
    return m_uncalibratedRoll ;
  }
  double GetUncalibratedPitch( void ) const {
    return m_uncalibratedPitch ;
  }
  __int16 GetRawRoll( void ) const {
    return m_rawRoll ;
  }
  __int16 GetRawPitch( void ) const {
    return m_rawPitch ;
  }
  double GetTemperature( void ) const {
    return m_temperature ;
  }
  __int16 GetRawTemperature( void ) const {
    return m_rawTemperature ;
  }
private:
  double m_roll ;
  double m_pitch ;
  double m_uncalibratedRoll ;
  double m_uncalibratedPitch ;
  __int16 m_rawRoll ;
  __int16 m_rawPitch ;
  double m_temperature ;
  __int16 m_rawTemperature ;
} ;

class DAUValues
{
public:
  DAUValues( void ) ;
  DAUValues( DAU &source ) ;
  DAUValues( const DAUValues &source ) ;
public:
  void Set( const DAUValues &source ) ;
public:
  int GetSensorCount( void ) const {
    return (int)m_sensors.size() ;
  }
  int GetSyncroCount( void ) const {
    return (int)m_syncros.size() ;
  }
  const SingleSensor &GetSensor( int index ) const {
    return m_sensors [index] ;
  }
  const SingleSensor &GetSyncro( int index ) const {
    return m_syncros [index] ;
  }
private:
  vector<SingleSensor> m_sensors ;
  vector<SingleSensor> m_syncros ;
} ;

[
  db_command { SELECT id, name, type, offset, rollGain, pitchGain, rollOffset, pitchOffset FROM DAUSetupSensorChannel WHERE foreignID = ? ORDER BY name DESC }
]
class /*+++ symbol was renamed:  DAUSetupSensorFromID
 :---*/ _DAUSetupSensorFromIDAccessor {
public:
  DECL_DB_V(1, LONG, index) ;
  DECL_DB_L(2, TCHAR, name, 3) ;
  DECL_DB_V(3, LONG, type) ;
  DECL_DB_V(4, LONG, offset) ;
  DECL_DB_V(5, double, rollGain) ;
  DECL_DB_V(6, double, pitchGain) ;
  DECL_DB_V(7, LONG, rollOffset) ;
  DECL_DB_V(8, LONG, pitchOffset) ;
  PARM_DB_V(1, LONG, id) ;
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 373 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"

				DEFINE_COMMAND_EX(_DAUSetupSensorFromIDAccessor, L" SELECT id, name, type, offset, rollGain, pitchGain, rollOffset, pitchOffset FROM DAUSetupSensorChannel WHERE foreignID = ? ORDER BY name DESC ")
			
#injected_line 373 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"

						BEGIN_ACCESSOR_MAP(_DAUSetupSensorFromIDAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_index),  _SIZE_TYPE(m_index), 0, 0, offsetbuf(m_index), 0, offsetbuf(m_status_index))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_name),  _SIZE_TYPE(m_name), 0, 0, offsetbuf(m_name), offsetbuf(m_length_name), offsetbuf(m_status_name))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_type),  _SIZE_TYPE(m_type), 0, 0, offsetbuf(m_type), 0, offsetbuf(m_status_type))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_offset),  _SIZE_TYPE(m_offset), 0, 0, offsetbuf(m_offset), 0, offsetbuf(m_status_offset))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_rollGain),  _SIZE_TYPE(m_rollGain), 0, 0, offsetbuf(m_rollGain), 0, offsetbuf(m_status_rollGain))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_pitchGain),  _SIZE_TYPE(m_pitchGain), 0, 0, offsetbuf(m_pitchGain), 0, offsetbuf(m_status_pitchGain))
_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_rollOffset),  _SIZE_TYPE(m_rollOffset), 0, 0, offsetbuf(m_rollOffset), 0, offsetbuf(m_status_rollOffset))
_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_pitchOffset),  _SIZE_TYPE(m_pitchOffset), 0, 0, offsetbuf(m_pitchOffset), 0, offsetbuf(m_status_pitchOffset))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 373 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"

					BEGIN_PARAM_MAP(_DAUSetupSensorFromIDAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_id),  _SIZE_TYPE(m_id), 0, 0, offsetbuf(m_id), 0, offsetbuf(m_status_id))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 373 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"

class DAUSetupSensorFromID;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 373 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\DAU.h"

class DAUSetupSensorFromID : public CCommand<CAccessor<_DAUSetupSensorFromIDAccessor> >
    			
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
            szCommand = L" SELECT id, name, type, offset, rollGain, pitchGain, rollOffset, pitchOffset FROM DAUSetupSensorChannel WHERE foreignID = ? ORDER BY name DESC ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


#pragma pack(pop)
