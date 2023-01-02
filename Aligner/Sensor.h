// $Id: Sensor.h,v 1.12 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "Name.h"
#include "Filter.h"
#include "Type.h"
#include "NominalAzimuth.h"
#include "SerialNumber.h"
#include "DAUData.h"
#include "GlobalData.h"

#define DEFAULT_CHANNEL_OFFSET 0
#define DEFAULT_CHANNEL_SCALE  1.0
#define DEFAULT_SENSOR_SCALE_SENSOREX   3.052E-6
#define DEFAULT_SENSOR_SCALE_LUCAS      3.1952E-6
#define DEFAULT_SENSOR_AZIMUT  0.0 

struct ChannelCalibrationData
{
  DBTIMESTAMP m_time;
  int m_offset ;
  double m_scale ;
public:
  ChannelCalibrationData( void ) {
    m_offset = DEFAULT_CHANNEL_OFFSET ;
    m_scale = DEFAULT_CHANNEL_SCALE ;
  }
  ChannelCalibrationData( int offset, double scale, DBTIMESTAMP ts) {
    m_offset = offset ;
    m_scale = scale ;
    m_time = ts;
  }
} ;

struct ParallaxData
{
    double x;
    double y;
    double z;
    double dx;
    double dy;
    double dz;
};

struct SensorCalibrationData
{
  double m_offset ;
  double m_scale ;
  double m_azimut ;
public:
  SensorCalibrationData( double offset, double scale, double azimut ) {
    m_offset = offset ;
    m_scale = scale ;
    m_azimut = azimut ;
  }
} ;

struct SensorTemperatureCalibrationData
{
	DBTIMESTAMP m_time;
	double m_offset ;
	double m_linear ;
	double m_quadratic ;
	double m_cubic ;
public:
	SensorTemperatureCalibrationData( double offset, double linear, double quadratic, double cubic )
	{
		m_offset = offset ;
		m_linear = linear ;
		m_quadratic = quadratic ;
		m_cubic = cubic ; 
	}
public:
	double operator()( double temperature ) {
	return m_offset + temperature * (m_linear + temperature * (m_quadratic + temperature * m_cubic)) ;
	}
} ;

struct AdapterCalibrationData
{
  DBTIMESTAMP m_time;
  double m_elevation ;
  double m_azimuth ;
public:
  AdapterCalibrationData( void ) {
    m_elevation = 0.0 ;
    m_azimuth = 0.0 ;
  }
  AdapterCalibrationData( double elevation, double azimuth, DBTIMESTAMP ts) {
    m_elevation = elevation ;
    m_azimuth = azimuth ;
    m_time = ts;
  }
} ;

struct ElevationData
{
    ElevationData(){elevAlong=0.0f;elevAlongAbs=0.0f;elevAcross=0.0f;}
    static ModeT mode;
    static BOOL useElev;
    static BOOL elevErrorExist;
    double elevAlong;
    //double elevAlongSum;
    double elevAlongAbs;
    double elevAcross;
};

/**
 *  @brief Encapsulation of a Sensor Unit (SU).
 *
 *  A Sensor instance represents a physical Sensor Unit. A Sensor Unit consists
 *  of a temperature sensor and two inclination sensors.
 */
class Sensor : public SerialNumber, public AdapterSerialNumber, public Name, public NominalAzimuth, public UnitType, public Offset
{
public:
  Sensor( const CString &name, int type = 0 ) ;
	virtual ~Sensor( void ) ;
public:
    void ResetFilter( void ) ;
    void ResetFilter( double damping ) ;
    void ResetFilter( double period, double theta, double tau, double initialRollValue = 0.0, double initialPitchValue = 0.0 ) ;
    BOOL SetData( const SensorDataPro &data );
    BOOL SetData( const SensorData &data ) ;  
    SensorData GetData( void ) const ;
    double GetRoll( BOOL calibrated = TRUE ) const ;
    double GetPitch( BOOL calibrated = TRUE ) const ;
    double GetUnfilteredRoll() const ;
    double GetUnfilteredPitch() const ;
    void SetTemperatureData( const TemperatureData data ) ;
    void SetTemperatureData( const TemperatureDataPro& data );
    TemperatureData GetTemperatureData( void ) const ;
    double GetTemperature( void ) const ;
	template<class T>
	void LoadCalibration( T &table, CString &serialNumber, SensorTemperatureCalibrationData &data );
    BOOL LoadCalibration( void ) ;
    void SetChannelCalibrationData( ChannelCalibrationData roll, ChannelCalibrationData pitch ) ;
    void SetCalibrationData( SensorCalibrationData roll, SensorCalibrationData pitch ) ; 
    void SetCalibrationFromTemperature( void ) ;
    void SetDefaultCalibration( void ) ;
	static void SetLatitudeCompensation( double latitude ) ;
    void SetAdapterCalibrationData( AdapterCalibrationData adapterCalibration ) ;
	AdapterCalibrationData GetAdapterCalibrationData( void ) const;
	double GetDefaultScale( void ) const ;
	void SetDefaultScale( double defScale );
    int GetSensorType( void ) const ;
    void SetIgnoreElevation( BOOL ignoreElevation ) ;
    void SetCurrentAz(double az){m_currentAz = az;}
    void SetRotateBackAng(double d, BOOL mode=TRUE){m_rotateBackAng = d;m_doRotateBack=mode;TRACE("Set rotate back(%d):%f\n",m_doRotateBack,d);}
    double GetRotateBackAng(){return m_rotateBackAng;}
    double GetCurrentAz(){return m_currentAz;}
    ParallaxData GetParallaxData(){return m_parallaxData;}
    void SetCentrifugPitchComp(double compVal);
    void SetCentrifugRollComp(double compVal);
	  void Dump(ofstream& dump);
    int DaysToCalibrationExp();    

    SensorTemperatureCalibrationData m_rollOffsetTemperatureCalibration ;
    SensorTemperatureCalibrationData m_rollGainTemperatureCalibration ;
    SensorTemperatureCalibrationData m_rollAzimuthTemperatureCalibration ;
    SensorTemperatureCalibrationData m_pitchOffsetTemperatureCalibration ;
    SensorTemperatureCalibrationData m_pitchGainTemperatureCalibration ;
    SensorTemperatureCalibrationData m_pitchAzimuthTemperatureCalibration ;
private:

	  ChannelCalibrationData m_rollChannelCalibration ;
    ChannelCalibrationData m_pitchChannelCalibration ;
    SensorCalibrationData m_rollCalibration ;
    SensorCalibrationData m_pitchCalibration ;
    TemperatureCalibrationData m_temperatureCalibration ;
    SensorData m_data ;
    DataFilter m_roll ;
    DataFilter m_pitch ;
    DataFilter m_uncalibratedRoll ;
    DataFilter m_uncalibratedPitch ;
    TemperatureData m_temperatureData ;
    double m_temperature ;
	AdapterCalibrationData m_adapterCalibration ;
	double m_defaultScale ;
    BOOL m_ignoreElevation ;
    ParallaxData m_parallaxData;
    double m_currentAz;
    double m_centrifugRollComp;
    double m_centrifugPitchComp;
    double m_rotateBackAng;
    BOOL m_doRotateBack;

public:
    BOOL m_invertPitch ; //TODO:Refactor please!
    CString m_description ; //TODO:Refactor please!
    BOOL m_overrangeDetection ; //TODO:Refactor please!

    ElevationData m_elevData;    
    double m_zParCorr;

private:
	static double GetLatitudeCompensation( void ) ;
    void ElevCompensate(double& roll, double& pitch);
    COleDateTime GetTempCalTime();
    static double sm_latitudeCompensation ;
} ;

/**
 *  @bried Return the current roll angle.
 *
 *  Returns the roll angle as set by the latest call to SetData().
 *
 *  @param calibrated - A BOOL value that indicates wheter to use calibrated
 *  or default vaules. The default (TRUE) is to use calibrated values.
 *
 *  @returns The roll angle in radians.
 */
inline
double Sensor::GetRoll( BOOL calibrated ) const {
    if (calibrated) {
      return m_roll.Get() ;
    } else {
      return m_uncalibratedRoll.Get() ;
    }
  }

/**
 *  @bried Return the current pitch angle.
 *
 *  Returns the pitch angle in radians as set by the latest call to SetData().
 *
 *  @param calibrated - A BOOL value that indicates wheter to use calibrated
 *  or default vaules. The default (TRUE) is to use calibrated values.
 *
 *  @returns A double representing the pitch angle.
 */
inline
double Sensor::GetPitch( BOOL calibrated ) const {
    if (calibrated) {
      return m_pitch.Get() ;
    } else {
      return m_uncalibratedPitch.Get() ;
    }
  }

inline 
double Sensor::GetUnfilteredRoll( ) const {
    return m_roll.GetUnfiltered();
}

inline
double Sensor::GetUnfilteredPitch( ) const {
    return m_pitch.GetUnfiltered();
}


/**
 *  @brief Return current sensor data.
 *
 *  Returns the data set by the latest call to SetData.
 *
 *  @returns Data as a SensorData instance.
 */
inline
SensorData Sensor::GetData( void ) const {
    return m_data ;
  }

/**
 *  @brief Return current temperature.
 *
 *  Return the temperature in centigrades a set by the latest call to SetTemperatureData.
 *
 *  @returns The temperature as a double.
 */
inline
double Sensor::GetTemperature( void ) const {
    return m_temperature ;
  }

/**
 *  @brief Return current temperature data.
 *
 *  Return the data set by the latest call to SetTemperatureData.
 *
 *  @returns Data as a SensorTemperaturData instance.
 */
inline
TemperatureData Sensor::GetTemperatureData( void ) const {
  return m_temperatureData ;
}

inline
AdapterCalibrationData Sensor::GetAdapterCalibrationData( void ) const {
	return m_adapterCalibration ;
}

inline
void Sensor::SetAdapterCalibrationData( AdapterCalibrationData adapterCalibration ) {
  m_adapterCalibration = adapterCalibration;
}

inline
double Sensor:: GetDefaultScale( void ) const {
	return m_defaultScale ;
}

inline
void Sensor:: SetDefaultScale( double defScale ) {
    m_defaultScale = defScale;
}

inline
int Sensor::GetSensorType( void ) const {
	return m_invertPitch ? 0 : 1 ;
}

inline
double UnitsToMilliunits( double units ) {
	return - 1000.0 * units ;
}

inline
void Sensor::SetIgnoreElevation( BOOL ignoreElevation )
{
    m_ignoreElevation = ignoreElevation ;
}
