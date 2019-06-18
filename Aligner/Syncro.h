// $Id: Syncro.h,v 1.7 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "Name.h"
#include "Type.h"
#include "NominalAzimuth.h"
#include "Filter.h"
#include "DAUData.h"

// TODO Add resolution setting, now hard coded to 16 bits.
class Syncro : public Name, public NominalAzimuth, public UnitType, public Offset
{
public:
    Syncro( const CString &name ) ;
    virtual ~Syncro( void ) ;
public:
    void ResetFilter( void ) ;
    void ResetFilter( double damping ) ;
    void ResetFilter( double period, double theta, double tau ) ;
    double GetRoll( BOOL calibrated = TRUE ) const ;
    double GetPitch( BOOL calibrated = TRUE ) const ;
    double GetHeading( BOOL calibrated = TRUE ) const ;
    double GetUnfilteredRoll() const ;
    double GetUnfilteredPitch() const ;
    double GetUnfilteredHeading() const ;
    SyncroData GetData( void ) const ;
    virtual BOOL SetData( const SyncroData data ) ;
    BOOL SetData( const SyncroDataPro &data );
    BOOL SetData( double angle ) ;
    void SetGearing( double gearing ) ;
    double GetGearing( void ) const ;
    void SetRefVoltageA202( double refVoltageA202 ) ;
    double GetRefVoltageA202( void ) const ;
	CString GetSerialNumber( void ) const ;
	CString GetUnitTypeDescription( void ) const ;
    int GetResolution();
    void SetResolution(int res);
    void SetDigConverterType(CString type){m_digConverterType = type;}
    CString GetDigConverterType(){return m_digConverterType;}
    int GetStatus(){return m_status;}
    BOOL GetHighSeaComp(){return m_highSeaComp;}
    void SetHighSeaComp(BOOL b){m_highSeaComp=b;}

public:
  CString m_description ;
protected:
    SyncroData m_data ;
    DataFilter m_roll ;
    DataFilter m_pitch ;
    DataFilter m_heading ;
    double m_gearing ;
    double m_refVoltageA202 ;
    CString m_digConverterType;
    unsigned int m_rollResolution ;
    unsigned int m_pitchResolution ;
    unsigned int m_headingResolution ;
    int m_resolution;
    int m_status;
    BOOL m_highSeaComp;
} ;

inline
double Syncro::GetRoll( BOOL ) const {
  return m_roll.Get() ;
}

inline
double Syncro::GetPitch( BOOL ) const {
  return m_pitch.Get() ;
}

inline 
double Syncro::GetUnfilteredRoll( ) const {
    return m_roll.GetUnfiltered();
}

inline
double Syncro::GetUnfilteredPitch( ) const {
    return m_pitch.GetUnfiltered();
}

inline
double Syncro::GetUnfilteredHeading( ) const {
    return m_heading.GetUnfiltered();
}


inline
double Syncro::GetHeading( BOOL ) const {
    return m_heading.Get() ;
}

inline
SyncroData Syncro::GetData( void ) const {
  return m_data ;
}

inline
void Syncro::SetGearing( double gearing ) {
  m_gearing = gearing ;
}

inline
double Syncro::GetGearing( void ) const {
  return m_gearing ;
}

inline
void Syncro::SetRefVoltageA202( double refVoltageA202 ) {
  m_refVoltageA202 = refVoltageA202 ;
}

inline
double Syncro::GetRefVoltageA202( void ) const {
  return m_refVoltageA202 ;
}

inline
CString Syncro::GetSerialNumber( void ) const {
  return CString( _T("") ) ;
}

inline
CString Syncro::GetUnitTypeDescription( void ) const {
  return m_description ;
}
