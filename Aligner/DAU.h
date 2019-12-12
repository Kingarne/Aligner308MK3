// $Id: DAU.h,v 1.23 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
#pragma pack(push)
#pragma pack(1)

class DAUValues ;

#include "Sensor.h"
#include "MainBoard.h"
#include "SerialPort.h"
#include "Digital.h"
#include "DAOComThr.h"
#include "DataCompensator.h"

#define DIG_D1 0x01
#define DIG_D2 0x02

#define ET_OUT_OF_RANGE 1
#define ET_COM_ERROR    2

struct DAUSetupData
{
    int DBId;
    CString serialStr;
    int serial;
    int protocolVersion;
    int sampleAndHold;
};


struct ErrorEventInfo
{
	ErrorEventInfo(int t, int st=-1, CString os=""):type(t),subType(st),errorInfo(os){}
	int type;
	int subType;
	CString errorInfo;
};

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
  friend class DebugDlg;
  friend class CAlignerDoc;
  friend class XMLHandler;
public:  
    __event void DataEvent( const DAUValues &data ) ;
  __event void ErrorEvent(ErrorEventInfo& errorInfo ) ;
public:
    BOOL StartDAUComThread();
    BOOL StopDAUComThread();
    BOOL Setup() ;
    BOOL IsOpen( void ) const ;
    BOOL Clear( void ) ;
    BOOL Start();
    BOOL Restart(BOOL showMsg=FALSE);
    BOOL StartWithTest( int testVoltage );
    BOOL Stop( void ) ;
    BOOL ConfigSensors( BOOL sensorsOnly = TRUE) ;  
        
	void SetOverrangeDetection(BOOL flag);
    void SetOverrangeDetectionActive( BOOL flag ) ;
    int GetProtocolVersion( void ) ;
    void SetIgnoreElevation( BOOL ignoreElevation ) ;
    void SendInvalidGyroDataMsg();
    void SendDAUDeadMsg();
    void ClearConfig( void ) ;
    void SetSelected(BOOL b);	
    private:
      
    BOOL LoadConfig() ;
    BOOL LoadSensorConfig( LONG id ) ;
    BOOL LoadSyncroConfig( LONG id ) ;	
        
    BOOL Probe( void ) ;
    BOOL ProbePro( void ) ;																										//	A308Pro
									    //	A308Pro
    void SetDefaultCalibration( void ) ;  
    void UpdateUARTProtocoll();
    
    void SetResolutionsToDAU();
    void UpdateResolutionsFromDAU();    

	void DumpSensorStates();

public:

    int GetDAUDBId(){return m_dauDBData.DBId;}
    BOOL GetEnableAzimuthMeasurment( void ) const ;
    BOOL GetEnableSperryMeasurment( void ) const ;
    //virtual void NewData( void ) ;
    void NotifyListeners( void ) ;

    BOOL IsCalibrated( void ) const ;
    int GetProtocolVersion( void ) const ;
    void EnableErrorEvent( BOOL enable ) ;
    BOOL IsErrorEventEnabled( void ) const ;
    BOOL CheckDigitalDataReception();
    void InitDataCompensator(BOOL activeate=TRUE);
    void SetEnableDataCompensator(BOOL b){m_dataCompensator.SetEnable(b);}
    BOOL GetEnableDataCompensator(){m_dataCompensator.GetEnable();}
    void ResetSensorCompensations();

    Sensor *GetSensor( int i ) ;
    Sensor *GetSensor( const CString &name ) ;
    int GetSensorCount( void ) const ;
    int GetConnectedSensorCount( void ) const ;
	int GetConnectedSensorCount( UnitType::Types t ) const ;
    Syncro *GetSyncro( int i ) ;
    Syncro *GetSyncro( const CString &name ) ;
    Syncro *GetFirstSelectedSyncro();
    int GetSyncroCount( void ) const ;
    const MainBoard *GetMainboard( void ) const ;

    int GetMask( void ) const ;
    BOOL SetResolutionMask( int mask );
    double GetFrequency( void );
    double GetPeriod( void );    
    BOOL GetEnableAzimuthMeasurement( void ) const;
    BOOL GetEnableSperryMeasurement( void ) const;
    void SetEnableAzimuthMeasurement( BOOL flag ) ;
    void SetEnableSperryMeasurement( BOOL flag ) ;    
    int GetStatus();
    void CheckComErrors();

    void SetDACVoltage( int voltage );
    short GetMeasuredDACVoltage( void );
    short GetFilteredMeasuredDACVoltage( void );
    void UpdateSyncroSettings();
    void AddBuddyUsedChannel(CString str);
    void RemoveBuddyUsedChannel(CString str);
    bool IsBuddyChannelUsed(CString str);
    set<CString> m_usedBuddyChannelSet;	    
    Syncro* GetHighSeaCompGyro();
	Sensor *GetSensorFromSN(const CString &sn);

	int m_timerIntervall;
	BOOL CheckDataRate(int numframesHandled);
	LARGE_INTEGER m_lastTimerCheck;
	LARGE_INTEGER m_freq;

	DAUDataPro m_dauDataPro ;
    MainBoardRegsPro m_mainBoardRegsPro;
    short m_recDACVal;
    deque<short> m_recentRecDACVal;
    int m_maxNumRecentDacVal;
    unsigned short m_recSetDACVal;    
    BOOL m_logDACData;

    static DAU &DAU::GetDAU( void ) ;
    int HandleDAUFrames();    
    DAOComThread m_comThr;
    vector<int> m_dauFramesCounter;
    CWnd* m_pParent;
    void SyncDigital();
    BOOL m_digitalDataArrived;
	DataCompensator m_dataCompensator;

private:
    static UINT run( LPVOID pThis ) ;
    		    
        void SortSyncros();
        void SortSensors();
        void Log2File(short val, CString file);
        void AddToReceivedDACVal(short val);

    CString m_port ;
    int m_mask ;
    int m_stop ;
      
    BOOL m_isCalibrated ;
    int m_serial ;
    double m_period ;
    OVERLAPPED m_overlapped ;
    DAUData m_data ;
    map<int, CString> m_temperatureChMap;
    DAUSetupData m_dauDBData;
  //***   A308Pro   ***//																//***   A308Pro 
//***      ***//
private:
    LONG m_protocolVersion ;
    BOOL m_sampleAndHold ;
    BOOL m_enableSperry ;
    BOOL m_enableAzimuth ;
      
    MainBoard m_mainBoard ;
    vector<Sensor *> m_sensorPointers ;
    vector<Syncro *> m_syncroPointers ;  
    map<CString, Sensor *> m_sensorMap ;
    map<CString, Syncro *> m_syncroMap ;

    static DAU sm_dau ;
    BOOL m_running ;
    int m_timeoutCounter ;  
    BOOL m_enableErrorEvent ; // added by R2 040628
	
	BOOL m_overangeDetectionActive;	

    std::deque<DAUFrame> m_DAUFramesQue;    
    CRITICAL_SECTION m_lock;
    void FrameCallback(WPARAM wp, LPARAM lp);
    static void StaticFrameCallback(PVOID pData, WPARAM wp, LPARAM lp)
    {        
        DAU* pObj = (DAU*)pData;
        pObj->FrameCallback(wp, lp);

    }
	void StatusCallback(WPARAM wp, LPARAM lp);
	static void StaticStatusCallback(PVOID pData, WPARAM wp, LPARAM lp)
	{        
		DAU* pObj = (DAU*)pData;
		pObj->StatusCallback(wp, lp);

	}
    int HandleSensorData(DAUFrame& frame);
    int HandleDigitalData(DAUFrame& frame, Digital* pDigital);
    void InvertSensorData(DAUFrame& frame);
    CString HandleSyncroData308(DAUFrame& frame);
    CString HandleSyncroData202(DAUFrame& frame);
    
public:
    DAUData m_simulatedData;
    BOOL m_simulationActive ;

    CString m_activeHighSeaGyro;        

    void SetPropertyPageBuddies(map<CString, CPropertyPage*>& syncMap);
} ;

//	DAUDataPro DAU::sm_dauDataPro ;																	//	A308Pro

inline
short DAU::GetMeasuredDACVoltage( void ) 
{
	return m_recDACVal;
//    return( m_data.m_testVoltage );
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
    if (UnitType::Unused != (*i) -> GetType())
    {
      connected++ ;
    }
  }
  return connected ;
}

inline
int DAU::GetConnectedSensorCount( UnitType::Types t ) const {
  int connected = 0 ;
  for (vector<Sensor *>::const_iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    if ((*i)->GetType() == t)
    {            
        connected++ ;      
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
double DAU::GetFrequency( void ) 
{
  return m_comThr.GetSamplingRate();
}

inline
double DAU::GetPeriod( void ) 
{
    return 1.0f / m_comThr.GetSamplingRate();
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
    //TRACE("Set ErrorEvent enable:%d\n",enable);
    m_enableErrorEvent = enable;

	if(m_enableErrorEvent)
	{
		//TRACE("Clear active\n");
		m_comThr.m_errorHandler.SetActive(FALSE);
		
	}
}

inline
BOOL DAU::IsErrorEventEnabled( void ) const
{
  return( m_enableErrorEvent );
}


struct SingleSensorData
{
	SingleSensorData(){value=0.0f;uncalibratedValue=0.0f;rawValue=0;unfilteredValue=0;}
	SingleSensorData(double v, double unF, double unCal, __int16 raw){value=v;unfilteredValue=unF;uncalibratedValue=unCal;rawValue=raw;}
	double value;
	double unfilteredValue;
	double uncalibratedValue;    
	__int16 rawValue;
};

class SingleSensor
{
public:
	SingleSensor() {} ;
	SingleSensor(SingleSensorData roll, SingleSensorData pitch, SingleSensorData heading, int status=DS_OK);
	SingleSensor( double roll, double pitch, double unfilteredRoll, double unfilteredPitch, double uncalibratedRoll, double uncalibratedPitch, __int16 rawRoll, __int16 rawPitch, double temperature = 0, __int16 rawTemperature = 0, int status=DS_OK  );
public:
	double GetRoll( BOOL useCalibrated ) const {
		if(useCalibrated){return m_roll.value ;}
		else{return m_roll.uncalibratedValue;}
	}
	double GetPitch( BOOL useCalibrated) const {
		if(useCalibrated){return m_pitch.value ;}
		else{return m_pitch.uncalibratedValue;}
	}
	void SetRoll( double roll ){  m_roll.value=roll ;}
	void SetPitch( double pitch ){ m_pitch.value=pitch; }
	void SetHeading( double heading ){ m_heading.value=heading; }

	double GetRoll( void ) const { return m_roll.value ;}
	double GetPitch( void ) const { return m_pitch.value ; }
	double GetHeading( void ) const { return m_heading.value ; }

	double GetUncalibratedRoll( void ) const { return m_roll.uncalibratedValue ; }
	double GetUncalibratedPitch( void ) const { return m_pitch.uncalibratedValue ; }
	double GetUncalibratedHeading( void ) const { return m_heading.uncalibratedValue ; }

	double GetUnfilteredRoll( void ) const { return m_roll.unfilteredValue ; }
	double GetUnfilteredPitch( void ) const { return m_pitch.unfilteredValue ; }
	double GetUnfilteredHeading( void ) const { return m_heading.unfilteredValue ; }

	__int16 GetRawRoll( void ) const { return m_roll.rawValue ; }
	__int16 GetRawPitch( void ) const { return m_pitch.rawValue ; }
	__int16 GetRawHeading( void ) const { return m_heading.rawValue ; }

	double GetTemperature( void ) const { return m_temperature.value ; }
	__int16 GetRawTemperature( void ) const { return m_temperature.rawValue ; }

	void Compensate(double roll, double pitch){m_roll.value+=roll;m_pitch.value+=pitch;}
	int GetStatus(){return m_status;}

private:

	SingleSensorData m_roll;
	SingleSensorData m_pitch;
	SingleSensorData m_heading;
	SingleSensorData m_temperature;
	int m_status;

	//     double m_roll ;
	//   double m_pitch ;
	//   double m_uncalibratedRoll ;
	//   double m_uncalibratedPitch ;
	//   __int16 m_rawRoll ;
	//   __int16 m_rawPitch ;
	//   double m_temperature ;
	//   __int16 m_rawTemperature ;
} ;

class DAUValues
{
public:
	DAUValues( void ) ;
	DAUValues( DAU &source ) ;
	DAUValues( const DAUValues &source ) ;

	void Set( const DAUValues &source ) ;
	int GetSensorCount( void ) const { return (int)m_sensors.size() ; }
	int GetSyncroCount( void ) const { return (int)m_syncros.size() ; }
	SingleSensor &GetSensor( int index ) { return m_sensors [index] ; }
	SingleSensor &GetSyncro( int index ) { return m_syncros [index] ; }
	const SingleSensor &GetSensor( int index ) const { return m_sensors [index] ; }
	const SingleSensor &GetSyncro( int index ) const { return m_syncros [index] ; }
	void CompensateSensorData(double roll, double pitch);

private:
	vector<SingleSensor> m_sensors ;
	vector<SingleSensor> m_syncros ;
} ;



// 
// [
//   db_command { SELECT id, name, type, offset, rollGain, pitchGain, rollOffset, pitchOffset FROM DAUSetupSensorChannel WHERE foreignID = ? ORDER BY name DESC }
// ]
// class DAUSetupSensorFromID
// {
// public:
//   DECL_DB_V(1, LONG, index) ;
//   DECL_DB_L(2, TCHAR, name, 3) ;
//   DECL_DB_V(3, LONG, type) ;
//   DECL_DB_V(4, LONG, offset) ;
//   DECL_DB_V(5, double, rollGain) ;
//   DECL_DB_V(6, double, pitchGain) ;
//   DECL_DB_V(7, LONG, rollOffset) ;
//   DECL_DB_V(8, LONG, pitchOffset) ;
//   PARM_DB_V(1, LONG, id) ;
//   void GetRowsetProperties( CDBPropSet *pPropSet )
//   {
//     pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
//     pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
//     pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
//     pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
//   }
// } ;

#pragma pack(pop)
