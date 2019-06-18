// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\sensor.mrg.cpp
// compiler-generated file created 05/10/06 at 15:50:57
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: sensor.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "Database.h"
#include "DAUData.h"
#include "SerialNumber.h"
#include "Type.h"
#include "Sensor.h"
#include "SensorDatabase.h"

double Sensor::sm_latitudeCompensation = 1.0 ;

static double SensorScaleFromChannelType( int channelType )
{
	double scale = DEFAULT_SENSOR_SCALE_SENSOREX ;
	if (0 == channelType)
	{
		scale = DEFAULT_SENSOR_SCALE_LUCAS ;
	}
	return scale ;
}

Sensor::Sensor( const CString &name , int channelType ) : Name( name ),
  m_defaultScale( SensorScaleFromChannelType( channelType ) ), 
  m_rollOffsetTemperatureCalibration( 0, 0, 0, 0),
	m_rollGainTemperatureCalibration( 1.0, 0, 0, 0),
  m_rollAzimuthTemperatureCalibration( 0, 0, 0, 0),
  m_pitchOffsetTemperatureCalibration( 0, 0, 0, 0),
  m_pitchGainTemperatureCalibration( 1.0, 0, 0, 0),
  m_pitchAzimuthTemperatureCalibration( 0, 0, 0, 0),
  m_rollCalibration( 0, GetDefaultScale(), 0 ),
  m_pitchCalibration( 0, GetDefaultScale(), 0 )
{
  SetData( SensorData() ) ;
  SetTemperatureData( TemperatureData() ) ;
  m_overrangeDetection = TRUE ;
	m_invertPitch = 1 == channelType ? TRUE : FALSE ;
    m_ignoreElevation = FALSE ;
}

Sensor::~Sensor( void )
{
  // Empty
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

BOOL Sensor::SetData( const SensorData &data )
{
  m_data = data ;
  double channelRoll =  (data.m_roll  - m_rollChannelCalibration.m_offset)  / m_rollChannelCalibration.m_scale ;
  double channelPitch = (data.m_pitch - m_pitchChannelCalibration.m_offset) / m_pitchChannelCalibration.m_scale ;
  if (m_invertPitch)
  {
    channelPitch = -channelPitch ;
  }
  m_uncalibratedRoll.Set( asin( GetLatitudeCompensation() * GetDefaultScale() * channelRoll) ) ;
  m_uncalibratedPitch.Set( asin( GetLatitudeCompensation() * GetDefaultScale() * channelPitch) ) ;
	double roll  = asin( GetLatitudeCompensation() * (m_rollCalibration.m_scale  * channelRoll  - m_rollCalibration.m_azimut  * channelPitch + m_rollCalibration.m_offset) ) ;
	double pitch = asin( GetLatitudeCompensation() * (m_pitchCalibration.m_scale * channelPitch + m_pitchCalibration.m_azimut * channelRoll  + m_pitchCalibration.m_offset) ) ;
  Rotate( roll, pitch, -(M_PI * GetNominalAzimuth() / 180.0) , roll, pitch ) ;
  if (!m_ignoreElevation)
  {
    pitch += m_adapterCalibration.m_elevation ;
  }
  Rotate( roll, pitch, - m_adapterCalibration.m_azimuth, roll, pitch ) ;
  m_roll.Set( roll ) ;
  m_pitch.Set( pitch ) ;
  return !(m_overrangeDetection && ((abs(data.m_pitch) > OVERRANGE_LEVEL) || (abs(data.m_roll) > OVERRANGE_LEVEL))) ; // TODO: Add overrange constant.
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

template<class T>
void Sensor::LoadCalibration( T &table, CString &serialNumber, SensorTemperatureCalibrationData &data )
{
  SET_DB_STRING(table, serialNumber, serialNumber ) ;
  HRESULT hResult = ::OpenTable( table ) ;
  if (S_OK == hResult)
  {
    if (S_OK == table.MoveFirst())
    {
      data = SensorTemperatureCalibrationData( table.m_a_0, table.m_a_1, table.m_a_2, table.m_a_3 ) ;
    }
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    ASSERT(0) ;
  }
}

[
	db_command { SELECT azimuth, elevation FROM GunAdapterCalibration WHERE serialNumber = ? }
]
class /*+++ symbol was renamed:  GunAdapterCalibrationFromSN
 :---*/ _GunAdapterCalibrationFromSNAccessor {
public:
  DECL_DB_V(1, double, azimuth) ;
  DECL_DB_V(2, double, elevation) ;
  PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 146 ".\\Sensor.cpp"

				DEFINE_COMMAND_EX(_GunAdapterCalibrationFromSNAccessor, L" SELECT azimuth, elevation FROM GunAdapterCalibration WHERE serialNumber = ? ")
			
#injected_line 146 ".\\Sensor.cpp"

						BEGIN_ACCESSOR_MAP(_GunAdapterCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 146 ".\\Sensor.cpp"

					BEGIN_PARAM_MAP(_GunAdapterCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 146 ".\\Sensor.cpp"

class GunAdapterCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 146 ".\\Sensor.cpp"

class GunAdapterCalibrationFromSN : public CCommand<CAccessor<_GunAdapterCalibrationFromSNAccessor> >
    			
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
            szCommand = L" SELECT azimuth, elevation FROM GunAdapterCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


BOOL Sensor::LoadCalibration( void )
{
  SetDefaultCalibration() ;
  LoadCalibration( SensorRollOffsetCalibrationFromSN(), GetSerialNumber(), m_rollOffsetTemperatureCalibration ) ;
  LoadCalibration( SensorRollGainCalibrationFromSN(), GetSerialNumber(), m_rollGainTemperatureCalibration ) ;
  LoadCalibration( SensorRollAzimuthCalibrationFromSN(), GetSerialNumber(), m_rollAzimuthTemperatureCalibration ) ;
  LoadCalibration( SensorPitchOffsetCalibrationFromSN(), GetSerialNumber(), m_pitchOffsetTemperatureCalibration ) ;
  LoadCalibration( SensorPitchGainCalibrationFromSN(), GetSerialNumber(), m_pitchGainTemperatureCalibration ) ;
  LoadCalibration( SensorPitchAzimuthCalibrationFromSN(), GetSerialNumber(), m_pitchAzimuthTemperatureCalibration ) ;
	if (Unit::Gun == GetType())
	{
		GunAdapterCalibrationFromSN table ;
    SET_DB_STRING(table, serialNumber, (LPCTSTR)GetAdapterSerialNumber()) ;
		HRESULT hResult ;
		hResult = ::OpenTable( table ) ;
		hResult = table.MoveFirst() ;
    m_adapterCalibration = AdapterCalibrationData( MilliUnitsToUnits( table.m_elevation ), MilliUnitsToUnits( table.m_azimuth ) ) ;
	}
	else
	{
		m_adapterCalibration = AdapterCalibrationData() ;
	}
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
