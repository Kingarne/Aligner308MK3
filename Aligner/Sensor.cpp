// $Id: Sensor.cpp,v 1.12 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "DAUData.h"
#include "Type.h"
#include "Sensor.h"
#include "CalibInfo.h"
#include <algorithm>
#include <sstream>

double Sensor::sm_latitudeCompensation = 1.0 ;

ModeT ElevationData::mode = Diff;
BOOL ElevationData::useElev = FALSE;
BOOL ElevationData::elevErrorExist = FALSE;

static double SensorScaleFromChannelType( int channelType )
{
	double scale = DEFAULT_SENSOR_SCALE_SENSOREX ;
	if (0 == channelType)
	{
		scale = DEFAULT_SENSOR_SCALE_LUCAS ;
	}
	return scale ;
}

Sensor::Sensor( const CString &name , int channelType ) : 
  m_defaultScale( SensorScaleFromChannelType( channelType ) ), 
  m_rollOffsetTemperatureCalibration( 0, 0, 0, 0),
	m_rollGainTemperatureCalibration( 1.0, 0, 0, 0),
  m_rollAzimuthTemperatureCalibration( 0, 0, 0, 0),
  m_pitchOffsetTemperatureCalibration( 0, 0, 0, 0),
  m_pitchGainTemperatureCalibration( 1.0, 0, 0, 0),
  m_pitchAzimuthTemperatureCalibration( 0, 0, 0, 0),
  m_rollCalibration( 0, GetDefaultScale(), 0 ),
  m_pitchCalibration( 0, GetDefaultScale(), 0 ),
  m_rotateBackAng(0.0f),m_doRotateBack(FALSE)
{
    m_overrangeDetection = TRUE ;
	m_invertPitch = 1 == channelType ? TRUE : FALSE ;
    m_ignoreElevation = FALSE ;
    m_currentAz = 0.0f;
    m_zParCorr = 0.0f;
    m_centrifugRollComp = 0.0f;
    m_centrifugPitchComp = 0.0f;    
    m_name = name;
    SetData( SensorData() ) ;
    SetTemperatureData( TemperatureData() ) ;
}

Sensor::~Sensor( void )
{
  // Empty
}

BOOL Sensor::SetSerialNumber(const CString& serialNumber)
{
  if (3 == serialNumber.GetLength() || 0 == serialNumber.GetLength())
  {
    m_serialNumber = serialNumber;
    return TRUE;
  }
  return FALSE;
}

BOOL Sensor::SetAdapterDesc(const CString desc)
{
  if (!SetAdapterSerialNumber(desc))
    return FALSE;

  BOOL b = true;
  if (m_type != UnitType::Types::Theo)
    b = SetCaliber(desc.Right(desc.GetLength() - 6));

  return b;
}

CString Sensor::GetAdapterDesc(void) const
{
  CString serial = m_adapterData.m_serialNumber;
  CString calStr = "";
  if (m_type != UnitType::Types::Theo)
    calStr.Format(" - %.0fmm", m_adapterData.m_caliber);
  serial += calStr;
  return serial;
}

BOOL Sensor::SetCaliber(const CString& cal)
{
  if (cal.GetLength() > 0)
  {
    double d;
    stringstream ss((LPCTSTR)cal, std::stringstream::in);
    ss >> d;

    m_adapterData.m_caliber = d;
    return TRUE;
  }
  return FALSE;
}


BOOL Sensor::SetAdapterSerialNumber(const CString& serialNumber)
{
  if (serialNumber.GetLength() >= 3 || 0 == serialNumber.GetLength())
  {
    if (isdigit(serialNumber.GetAt(0)));
    {            
      m_adapterData.m_serialNumber = serialNumber.Left(3);
      if (m_type == UnitType::Types::Theo)
      {
        DBInterface::Instance()->GetTheoAdapterType(m_adapterData.m_serialNumber, m_adapterData.m_type);
      }
      return TRUE;
    }
  }
  return FALSE;
}

void Sensor::ResetFilter( void )
{
  m_roll.Reset() ;
  m_pitch.Reset() ;
  m_uncalibratedRoll.Reset() ;
  m_uncalibratedPitch.Reset() ;
}

void Sensor::ResetFilter( double damping )
{
  m_roll.Reset( damping ) ;
  m_pitch.Reset( damping ) ;
  m_uncalibratedRoll.Reset( damping ) ;
  m_uncalibratedPitch.Reset( damping ) ;
}

void Sensor::ResetFilter( double period, double theta, double tau, double initialRollValue, double initialPitchValue )
{
  m_roll.Reset( period, theta, tau, initialRollValue ) ;
  m_pitch.Reset( period, theta, tau, initialPitchValue ) ;
  m_uncalibratedRoll.Reset( period, theta, tau, initialRollValue ) ;
  m_uncalibratedPitch.Reset( period, theta, tau, initialPitchValue ) ;
}

BOOL Sensor::SetData( const SensorDataPro &data )
{
    SensorData d;
    d.m_pitch = data.m_pitch;
    d.m_roll = data.m_roll;
    return SetData(d);
}

void Sensor::ElevCompensate(double& roll, double& pitch)
{
    
    if(m_zParCorr != INVALID_VALUE)
        pitch -= m_zParCorr;
    
    if(!m_elevData.elevErrorExist || !m_elevData.useElev)
        return;

    switch(m_elevData.mode)
    {
        case Diff:
            if(m_elevData.elevAcross != INVALID_VALUE)
                roll -= m_elevData.elevAcross;// dauData.AbsRoll[i] -= GetEac( i );            
            if(m_elevData.elevAlong != INVALID_VALUE)
                pitch -= m_elevData.elevAlong; //dauData.AbsPitch[i] -= GetEal( i );
        break;
        
        case Stab:
        case StabDiff:
            if(m_elevData.elevAlongAbs != INVALID_VALUE)
                pitch -= m_elevData.elevAlongAbs; //dauData.AbsPitch[i] -= GetEalAbs( i );
            break;
        
//         case StabDiff:
//              //dauData.AbsPitch[m_InParam.ObjNo[i]] -=  g_AlignerData.EalS[i];
//             break;
        default:break;
    } 

}

BOOL Sensor::SetData( const SensorData &data )
{
    m_data = data ;
    double channelRoll =  (data.m_roll  - m_rollChannelCalibration.m_offset)  / m_rollChannelCalibration.m_scale ;
    double channelPitch = (data.m_pitch - m_pitchChannelCalibration.m_offset) / m_pitchChannelCalibration.m_scale ;
    channelPitch = m_invertPitch ? -channelPitch : channelPitch;
    
    m_uncalibratedRoll.Set( asin( GetLatitudeCompensation() * GetDefaultScale() * channelRoll) ) ;
    m_uncalibratedPitch.Set( asin( GetLatitudeCompensation() * GetDefaultScale() * channelPitch) ) ;
	double roll  = asin( GetLatitudeCompensation() * (m_rollCalibration.m_scale * channelRoll - m_rollCalibration.m_azimut * channelPitch + m_rollCalibration.m_offset) ) ;
	double pitch = asin( GetLatitudeCompensation() * (m_pitchCalibration.m_scale * channelPitch + m_pitchCalibration.m_azimut * channelRoll  + m_pitchCalibration.m_offset) ) ;
    Rotate( roll, pitch, -GetNominalAzimuthRad() , roll, pitch ) ;
    pitch += ((!m_ignoreElevation) ? m_adapterCalibration.m_elevation : 0) + m_adapterCalibration.m_elevCorrOffset;
    Rotate( roll, pitch, -m_adapterCalibration.m_azimuth, roll, pitch ) ;
   
    ElevCompensate(roll, pitch);
    
     if(m_doRotateBack && GetType() != Fixed)
     {
         roll = 0;
     }

    Rotate( roll, pitch, -m_currentAz, roll, pitch ) ; 
   
    roll += m_centrifugRollComp;
    pitch += m_centrifugPitchComp;

     if(m_doRotateBack)
     {                 
         Rotate( roll, pitch, m_rotateBackAng, roll, pitch ) ; 
     }

    m_roll.Set( roll ) ;
    m_pitch.Set( pitch ) ;
    return !(ShallDoOverrangeDetection() && ((abs(data.m_pitch) > OVERRANGE_LEVEL) || (abs(data.m_roll) > OVERRANGE_LEVEL))) ; // TODO: Add overrange constant.
}

COleDateTime Sensor::GetTempCalTime()
{
  vector<DATE> vec;

  //Temperature calibration time.
  vec.push_back(COleDateTime(m_rollOffsetTemperatureCalibration.m_time).m_dt);
  vec.push_back(COleDateTime(m_rollGainTemperatureCalibration.m_time).m_dt);
  vec.push_back(COleDateTime(m_rollAzimuthTemperatureCalibration.m_time).m_dt);
  vec.push_back(COleDateTime(m_pitchOffsetTemperatureCalibration.m_time).m_dt);
  vec.push_back(COleDateTime(m_pitchGainTemperatureCalibration.m_time).m_dt);
  vec.push_back(COleDateTime(m_pitchAzimuthTemperatureCalibration.m_time).m_dt);
  
  //Sensor calibration time
  vec.push_back(COleDateTime(m_rollChannelCalibration.m_time).m_dt);

  

  COleDateTime ts(*std::min_element(vec.begin(), vec.end()));
 
  return ts;
}

int Sensor::DaysToAdapterCalibrationExp(int& limit)
{

  if (!UnitType::TypeHasAdapter(GetType()))
  {
    limit = 30;
    return CAL_TIME_LIMIT;
  }

  COleDateTime now = COleDateTime::GetCurrentTime();
  COleDateTime fixTime = COleDateTime(m_adapterCalibration.m_time).m_dt;
  COleDateTimeSpan ts = now - fixTime; 
  int left = 0;
  if (m_type == Theo && m_adapterData.m_type == AdapterData::Type::Fix)
  {   
    left = CAL_THEO_FIX_TIME_LIMIT - ts.GetDays();   
    limit = 3;
  }
  else
  {
    left = CAL_TIME_LIMIT - ts.GetDays();
    limit = 30;
  }
  
  return left;
}

int Sensor::DaysToCalibrationExp(int &limit)
{
  bool valid = true;
  TRACE("HasValidCalibration %s\n", GetSerialNumber());
  COleDateTime calTime = GetTempCalTime();
  COleDateTime now = COleDateTime::GetCurrentTime();

  COleDateTimeSpan ts = now - calTime;
  int left = CAL_TIME_LIMIT - ts.GetDays();
  limit = 30;

  return left;
}

void Sensor::SetCentrifugPitchComp(double compVal)
{
    double fac = (m_parallaxData.x+ cos(m_currentAz)*m_parallaxData.dx - sin(m_currentAz)*m_parallaxData.dy);
    fac /= 9.81f;
    m_centrifugPitchComp = compVal * fac;    
}

void Sensor::SetCentrifugRollComp(double compVal)
{
    double fac = (m_parallaxData.y + sin(m_currentAz)*m_parallaxData.dx) + cos(m_currentAz)*m_parallaxData.dy;
    fac /= 9.81f;
    m_centrifugRollComp = (compVal * fac);    
}

void Sensor::SetTemperatureData( const TemperatureDataPro& data )
{
    TemperatureData t;
    t.m_temperature = data.m_temperature;
    SetTemperatureData(t);
}

void Sensor::SetTemperatureData( const TemperatureData data )
{
  m_temperatureData = data ;
  m_temperature = m_temperatureCalibration( data.m_temperature ) ;
	SetCalibrationFromTemperature() ;
}

void Sensor::SetDefaultCalibration( void )
{
  m_rollCalibration = SensorCalibrationData( 0, GetDefaultScale(), 0 ) ;
  m_pitchCalibration = SensorCalibrationData( 0, GetDefaultScale(), 0 ) ;
}

static double MilliUnitsToUnits( double value )
{
  return value / 1000.0 ;
}

void Sensor::SetCalibrationFromTemperature( void )
{
    m_rollCalibration = SensorCalibrationData(
    MilliUnitsToUnits( m_rollOffsetTemperatureCalibration( m_temperature ) ),
    GetDefaultScale() * m_rollGainTemperatureCalibration( m_temperature ),
    GetDefaultScale() * MilliUnitsToUnits( m_rollAzimuthTemperatureCalibration( m_temperature ) ) ) ;
    m_pitchCalibration = SensorCalibrationData(
    MilliUnitsToUnits( m_pitchOffsetTemperatureCalibration( m_temperature ) ),
    GetDefaultScale() * m_pitchGainTemperatureCalibration( m_temperature ),
    GetDefaultScale() * MilliUnitsToUnits( m_pitchAzimuthTemperatureCalibration( m_temperature ) ) ) ;
}

BOOL Sensor::LoadCalibration( void )
{
    SetDefaultCalibration() ;    
	
    DBInterface::Instance()->GetSensorCalibrationData("SensorRollOffsetCalibration", GetSerialNumber(), m_rollOffsetTemperatureCalibration ) ;
    DBInterface::Instance()->GetSensorCalibrationData("SensorRollGainCalibration", GetSerialNumber(), m_rollGainTemperatureCalibration ) ;
    DBInterface::Instance()->GetSensorCalibrationData("SensorRollAzimuthCalibration", GetSerialNumber(), m_rollAzimuthTemperatureCalibration ) ;
    DBInterface::Instance()->GetSensorCalibrationData("SensorPitchOffsetCalibration", GetSerialNumber(), m_pitchOffsetTemperatureCalibration ) ;
    DBInterface::Instance()->GetSensorCalibrationData("SensorPitchGainCalibration", GetSerialNumber(), m_pitchGainTemperatureCalibration ) ;
    DBInterface::Instance()->GetSensorCalibrationData("SensorPitchAzimuthCalibration", GetSerialNumber(), m_pitchAzimuthTemperatureCalibration ) ;
     
    m_adapterCalibration = AdapterCalibrationData() ;

    if (UnitType::TypeHasAdapter(m_type))
    {
      AdapterCalibrationData data;
      DBInterface::Instance()->GetAdapterCalibration(GetAdapterSerialNumber(), m_type, data);
      m_adapterCalibration = AdapterCalibrationData(MilliUnitsToUnits(data.m_elevation), MilliUnitsToUnits(data.m_azimuth), data.m_time, data.m_elevCorrOffset);
    }
	
    ParallaxData defaultParallax = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    m_parallaxData = defaultParallax;
    DBInterface::Instance()->GetStationParallaxes(m_description, m_parallaxData);        

    return TRUE ;
}

void Sensor::SetCalibrationData( SensorCalibrationData roll, SensorCalibrationData pitch )
{
    m_rollCalibration = roll ;
    m_pitchCalibration = pitch ;
}

void Sensor::SetChannelCalibrationData( ChannelCalibrationData roll, ChannelCalibrationData pitch )
{
    m_rollChannelCalibration = roll ;
    m_pitchChannelCalibration = pitch ;
}

double Sensor::GetLatitudeCompensation( void )
{
	return sm_latitudeCompensation ;
}

inline
double square( double x ) {
	return x * x ;
}

void Sensor::SetLatitudeCompensation( double latitude )
{
	latitude *= M_PI / 180.0 ;
  sm_latitudeCompensation = 1.00388 / (( 1.0 + 0.0053024 * square( sin( latitude ) ) - 5.9E-6 * square( sin( 2.0 * latitude ) ) )) ;
}

void Sensor::Dump(ofstream& dump)
{
	dump << GetName() << endl;

	dump << "RollOffsetTemperatureCalibration: " << m_rollOffsetTemperatureCalibration.m_offset << ", " << m_rollOffsetTemperatureCalibration.m_linear << ", "
		<< m_rollOffsetTemperatureCalibration.m_cubic << ", " << m_rollOffsetTemperatureCalibration.m_quadratic << endl;

	dump << "RollGainTemperatureCalibration: " << m_rollGainTemperatureCalibration.m_offset << ", " << m_rollGainTemperatureCalibration.m_linear << ", "
		<< m_rollGainTemperatureCalibration.m_cubic << ", " << m_rollGainTemperatureCalibration.m_quadratic << endl;

	dump << "RollAzimuthTemperatureCalibration: " << m_rollAzimuthTemperatureCalibration.m_offset << ", " << m_rollAzimuthTemperatureCalibration.m_linear << ", "
		<< m_rollAzimuthTemperatureCalibration.m_cubic << ", " << m_rollAzimuthTemperatureCalibration.m_quadratic << endl;

	dump << "PitchOffsetTemperatureCalibration: " << m_pitchOffsetTemperatureCalibration.m_offset << ", " << m_pitchOffsetTemperatureCalibration.m_linear << ", "
		<< m_pitchOffsetTemperatureCalibration.m_cubic << ", " << m_pitchOffsetTemperatureCalibration.m_quadratic << endl;

	dump << "PitchGainTemperatureCalibration: " << m_pitchGainTemperatureCalibration.m_offset << ", " << m_pitchGainTemperatureCalibration.m_linear << ", "
		<< m_pitchGainTemperatureCalibration.m_cubic << ", " << m_pitchGainTemperatureCalibration.m_quadratic << endl;

	dump << "PitchAzimuthTemperatureCalibration: " << m_pitchAzimuthTemperatureCalibration.m_offset << ", " << m_pitchAzimuthTemperatureCalibration.m_linear << ", "
		<< m_pitchAzimuthTemperatureCalibration.m_cubic << ", " << m_pitchAzimuthTemperatureCalibration.m_quadratic << endl;

	
	dump << "RollChannelCalibration: " << m_rollChannelCalibration.m_offset << ", " << m_rollChannelCalibration.m_scale << endl;
	dump << "PitchChannelCalibration: " << m_pitchChannelCalibration.m_offset << ", " << m_pitchChannelCalibration.m_scale << endl;

	dump << "RollCalibration: " << m_rollCalibration.m_offset << ", " << m_rollCalibration.m_scale << ", " << m_rollCalibration.m_azimut << endl;
	dump << "PitchCalibration: " << m_pitchCalibration.m_offset << ", " << m_pitchCalibration.m_scale << ", " << m_pitchCalibration.m_azimut << endl;

	



}