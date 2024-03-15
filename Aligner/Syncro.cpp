// $Id: Syncro.cpp,v 1.7 2013/02/13 14:25:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Aligner.h"
#include "Syncro.h"

Syncro::Syncro( const CString &name ) 
{
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        SetGearing( 1 ) ;
    }
    else
    {
        SetGearing( 36 ) ;
    }
    SetRefVoltageA202( SysSetup->GetRefVoltage(name));
    m_rollResolution = 1 << 16 ; // TODO add access methods for this!
    m_pitchResolution = 1 << 16 ; // TODO add access methods for this!
    m_digConverterType = "";
    m_status = DS_OK;
    m_highSeaComp = FALSE;
    m_offset = 0;
    m_name = name;
}

Syncro::~Syncro( void )
{
  // Empty
}

void Syncro::ResetFilter( void )
{
    m_roll.Reset() ;
    m_pitch.Reset() ;
    m_heading.Reset() ;
}

void Syncro::ResetFilter( double damping )
{
    m_roll.Reset( damping ) ;
    m_pitch.Reset( damping ) ;
    m_heading.Reset( damping) ;
}

void Syncro::ResetFilter( double period, double theta, double tau )
{
    m_roll.Reset( period, theta, tau ) ;
    m_pitch.Reset( period, theta, tau ) ;
    m_heading.Reset( period, theta, tau ) ;
}

BOOL Syncro::SetData( const SyncroDataPro &data )
{
    SyncroData d;
    d.m_pitch = data.m_pitch;
    d.m_roll = data.m_roll;
    d.m_heading = 0;
    return SetData(d);
}

BOOL Syncro::SetData( const SyncroData data )
{
    m_data = data ;
    double roll = 2 * M_PI / m_gearing * data.m_roll / m_rollResolution ;
    double pitch = 2 * M_PI / m_gearing * data.m_pitch / m_pitchResolution ;
    double heading = 2 * M_PI / m_gearing * data.m_heading / m_headingResolution ;
    Rotate( roll, pitch, -(M_PI * GetNominalAzimuth() / 180.0) , roll, pitch ) ;
    m_roll.Set( roll ) ;
    m_pitch.Set( pitch ) ;
    m_heading.Set( heading ) ;
    return !(ShallDoOverrangeDetection() && ((abs(data.m_pitch) > OVERRANGE_LEVEL) || (abs(data.m_roll) > OVERRANGE_LEVEL)) || (abs(data.m_heading) > OVERRANGE_LEVEL)) ;
}

BOOL Syncro::SetData( double angle )
{
    m_roll.Set( angle ) ;
    m_pitch.Set( angle ) ;
    m_heading.Set( angle ) ;
    return TRUE ;
}

int Syncro::GetResolution()
{
    return m_resolution;
}

void Syncro::SetResolution(int resolution)
{
    m_resolution = resolution;
}
