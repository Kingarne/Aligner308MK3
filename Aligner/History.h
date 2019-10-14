// $Id: History.h,v 1.12 2014-02-04 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

enum MeasType {
	MT_TiltAlignment = 1,
	MT_TiltFlatnessPl,
	MT_TiltFlatnessFo,
	MT_AzimuthAlign,
	MT_GyroPerf,
	MT_LiveGraph,
	MT_VerifAbsolute,
	MT_VerifRelative,
	MT_CommonFlatTilt,
	MT_SensorValidation
};


//Remove use DECLARE_MEASUREMENT
#define DECLARE_HISTORY \
public: \
  static BOOL AddData(  Data &data ) ; \
  static BOOL SetComment( const CString &comment ) ; \
  static BOOL AddItem( const ItemData item ) ; \
	static BOOL AddItemErr( const ItemErrData item ) ; \
	static BOOL AddExtItem( const ExtItemData extItem ) ; \
  static BOOL AddGraph( CString filename, BOOL include = TRUE ) ; \
	static BOOL DeleteLast( void ) ; \
  static void ResetMainID( void ) ; \
public: \
	static void SetLastID( LONG lastID ) ; \
	static LONG GetLastID( void ) ; \
	static LONG GetMainID( void ) ; \
private: \
  static LONG m_lastID ; \
  static LONG m_lastItemID ; \
  static LONG m_mainID ;

#define DECLARE_MEASUREMENT \
public: \
  static BOOL AddData( Data &data ) ; \
  static BOOL AddChannel( const ChannelData item ) ; \
	static BOOL AddChannelErr( const ChannelErrData item ) ; \
	static BOOL AddExtData( const ExtChannelData extItem ) ; \
  static void ResetMeasID( void ) ; \
public: \
	static void SetMeasTypeID( LONG lastID ) ; \
	static LONG GetMeasTypeID( void ) ; \
	static LONG GetMeasID( void ) ; \
private: \
  static LONG m_measTypeID ; \
  static LONG m_measChID ; \
  static LONG m_measID ;


//Remove when done with MeasurementData
class HistoryData
{
public:
    HistoryData(){}
    HistoryData(DBTIMESTAMP ts){m_time=ts;}
    
    DBTIMESTAMP m_time ;
	double m_timeConstant;
	CString m_comment;
    CString calibInfo;
};

class MeasurementBase
{
public:
	MeasurementBase() {}
	MeasurementBase(DBTIMESTAMP ts) { m_time = ts; }

	DBTIMESTAMP m_time;
	double m_timeConstant;
	CString m_comment;
	CString calibInfo;
	int type;
	CString m_refChannel;
};

class ChannelBase
{
public:
	CString m_station;
	CString m_channel;
	CString m_sensorSerialNumber;
	CString m_adapterSerialNumber;
	int m_type;
	double m_nominalAzimuth;
	double m_roll;
	double m_pitch;
	double m_tilt;
	double m_angle;
	double m_elevation;	
};

class ChannelErrBase
{
public:
	double m_azimuth;
	double m_error;
};

class TiltAlignment
{
private:
    TiltAlignment( void ) {} ;
public:
    class Data : public MeasurementBase
    {
    public:       
        CString m_lineOfSightDirection ;
        bool m_elevationCompensation ;		
    } ;

    class ChannelData : public ChannelBase
    {
		public:
        double m_bias ;
    } ;
    class ChannelErrData
    {
    public:
    } ;
    class ExtChannelData
    {
    public:
    } ;
	DECLARE_MEASUREMENT;
} ;

class TiltAndFlatness
{
private:
	TiltAndFlatness( void ) {} ;
public:
	class Data : public MeasurementBase
	{
	public:
		LONG m_numberOfMeasurements;
	} ;

	class ChannelData : public ChannelBase
	{
	public:
   
		double m_standardDeviation ;
		double m_maximumDeviation ;
		double m_azimuth ;
	} ;
	class ChannelErrData :public ChannelErrBase
	{
	public:
		double m_azimuth ;
		double m_error ;  
	} ;
	class ExtChannelData
	{
  
	} ;
	DECLARE_MEASUREMENT;
} ;

class TiltAndFlatnessFo
{
private:
	TiltAndFlatnessFo( void ) {} ;
public:
    class Data : public MeasurementBase
	{
	  public:
		LONG m_numberOfMeasurements ;
	} ;

  class ChannelData :public ChannelBase
  {
  public:
	  double m_elevation2;
	  double m_standardDeviation;
	  double m_bottomError;
	  double m_maximumDeviation;
	  double m_azimuth;	
  } ;

  class ChannelErrData:public ChannelErrBase
  {
  public:
    double m_error2 ;  
	double m_indexArmLength1;
	double m_indexArmLength2;
    double m_dh;
  } ;

	class ExtChannelData
  {
  public:
	/*	double m_azimuthAngle ;
		double m_deviationJ3 ;
		double m_deviationJ4 ;
		double m_deviationJ5 ;
		double m_deviationJ6 ;
		double m_deviationJ7 ;
		double m_deviationJ8 ;
		double m_deviationJ9 ;*/
  } ;
	DECLARE_MEASUREMENT ;
} ;


class GyroPerformance
{
private:
    GyroPerformance( void ) {} ;
public:
    class Data : public MeasurementBase
    {
    public:
    } ;

    class ChannelData:public ChannelBase
    {
    public:
     
    } ;
  class ChannelErrData
  {
  public:
  } ;
  class ExtChannelData
  {
  public:
  } ;
  DECLARE_MEASUREMENT;
} ;


class AzimuthAlignment
{
private:
  AzimuthAlignment( void ) {} ;
public:
  class Data : public MeasurementBase
  {
  public:
  
	double m_rollExcentricity ;
  } ;

  class ChannelData :public ChannelBase
  {
  public:
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ChannelErrData
  {
  public:
  } ;
  class ExtChannelData
  {
  public:
  } ;
  DECLARE_MEASUREMENT ;
} ;


class AzimuthVerificationBenchmarkHistory
{
private:
  AzimuthVerificationBenchmarkHistory( void ) {} ;
public:
  class Data : public HistoryData
  {
  public:
    double m_timeConstant ;
	double m_rollExcentricity ;
    CString m_comment ;
	CString m_measuredUnit ;
	//	DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
		double m_nominalAzimuth ;
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;


class AzimuthVerificationGyrostabilityHistory
{
private:
  AzimuthVerificationGyrostabilityHistory( void ) {} ;
public:
  class Data : public HistoryData
  {
  public:
    double m_timeConstant ;
	double m_rollExcentricity ;
    CString m_comment ;
	CString m_measuredUnit ;
	//DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
		double m_nominalAzimuth ;
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;


class HorizonAbsoluteModeHistory
{
private:
  HorizonAbsoluteModeHistory( void ) {} ;
public:
  class Data : public HistoryData
  {
  public:
    double m_timeConstant ;
	CString m_parallaxCompensation ;
    CString m_elevationCompensation ;
    CString m_comment ;
	CString m_measuredUnit ;
	//DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
		double m_maximumDeviation ;
		double m_azimuth ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;


class HorizonRelativeModeHistory
{
private:
  HorizonRelativeModeHistory( void ) {} ;
public:
  class Data : public HistoryData
  {
  public:
    double m_timeConstant ;
	CString m_parallaxCompensation ;
    CString m_elevationCompensation ;
    CString m_comment ;
	CString m_measuredUnit ;
	//	DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
		double m_maximumDeviation ;
		double m_azimuth ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;


class CommonFlatTiltHistory
{
private:
	CommonFlatTiltHistory( void ) {} ;
public:
	class Data : public HistoryData
	{
		public:
		double m_timeConstant ;
		CString m_comment ;
		CString m_measuredUnit ;
	//DBTIMESTAMP m_time ;
	} ;

  static BOOL UpdateCalibrationFlag(BOOL b);

	class ItemData
	{
		public:
		CString m_station ;
		CString m_channel ;
		CString m_sensorSerialNumber ;
		double m_roll ;
		double m_pitch ;
		double m_parallellBias ;
		double m_perpendicularBias ;
		double m_temperature ;
	} ;
	class ItemErrData
	{
		public:
	} ;
	class ExtItemData
	{
	  public:
	} ;
	DECLARE_HISTORY ;
  
} ;

class SensorValidationHistory
{
private:
	SensorValidationHistory(void) {};
public:
	class Data : public HistoryData
	{
	public:
		double m_timeConstant;
		CString m_comment;
		CString m_measuredUnit;		
	};

	static BOOL UpdateCalibrationFlag(BOOL b);

	class ItemData
	{
	public:
		CString m_station;
		CString m_channel;
		CString m_sensorSerialNumber;
		double m_rollSc;
		double m_pitchSc;
		double m_rollAzErr;
		double m_pitchAzErr;
		double m_temperature;
	};
	class ItemErrData
	{
		public:
	};
	class ExtItemData
	{
		public:
	};
	DECLARE_HISTORY;

};


class LiveGraphErrorsHistory
{
private:
  LiveGraphErrorsHistory( void ) {} ;
public:
  class Data : public HistoryData
  {
  public:
    double m_timeConstant ;
    double m_samplingRate ;
    CString m_comment ;
	CString m_measuredUnit ;
	//DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
		CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_temperature ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;



class LiveDataA202ErrorsHistory
{
private:
  LiveDataA202ErrorsHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_samplingRate ;
    CString m_comment ;
	DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
		CString m_channel ;
    double m_value ;
    double m_gearing ;
    double m_refVoltage ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

//BOOL MoveFilesToTemporary( LONG mainId ) ;
