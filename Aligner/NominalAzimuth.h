// $Id: NominalAzimuth.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class NominalAzimuth
{
public:
  NominalAzimuth( void ) ;
  virtual ~NominalAzimuth( void ) ;
public:
  void SetNominalAzimuth( double azimuth ) ;
  double GetNominalAzimuth( void ) const ;
	double GetNominalAzimuthDegree( void ) const ;
	double GetNominalAzimuthRad( void ) const ;
private:
  double m_azimuth ;
} ;

inline
NominalAzimuth::NominalAzimuth( void )
{
  m_azimuth = 0.0 ;
}

inline
void NominalAzimuth::SetNominalAzimuth( double azimuth )
{
  m_azimuth = azimuth ;
}

inline
double NominalAzimuth::GetNominalAzimuth( void ) const {
  return m_azimuth ;
}

inline
double NominalAzimuth::GetNominalAzimuthDegree( void ) const {
	return m_azimuth ;
}

inline
double NominalAzimuth::GetNominalAzimuthRad( void ) const {
  return m_azimuth * M_PI / 180.0 ;
}

#define RADIANS_TO_DEGREES( rad )			( ( 180.0 * (rad) ) / M_PI )
