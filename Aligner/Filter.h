// $Id: Filter.h,v 1.5 2013/02/13 14:25:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define MIN_FILTER_TIME_CONSTANT  0.09
#define MAX_FILTER_TIME_CONSTANT  100.0

/**
 *  @brief Implementation of discrete DataFilters.
 *
 *  DataFilter implements simple DataFilters of third, first and zeroth (no DataFilter) order.
 */
class DataFilter
{
public:
    DataFilter( void ) ;
    virtual ~DataFilter( void ) ;

    void Reset( void ) ;
    void Reset( double damping ) ;
    void Reset( double period, double theta, double tau, double initialValue = 0.0 ) ;
    void Set( double value ) ;
    double Get( void ) const ;
    double GetUnfiltered( void ) const {return m_unfiltered;}

private:
    enum { None = 0, FirstOrder = 1, ThirdOrder = 3 } m_type ;
    double m_tau ;
    double m_theta ;
    double m_period ;
    double m_x ;
    double m_y ;
    double m_z ;    

    double m_unfiltered;
} ;

inline
double DataFilter::Get( void ) const {
  return m_x ;
}

// CNoiseFilter
class CNoiseFilter
{
private:
  double m_Tk, m_Tk1, m_Tk2;        //adaptive DataFilter coefficients
  double m_A, m_B, m_C, m_D, m_K;   //constant DataFilter coefficients

public:
	CNoiseFilter();
	virtual ~CNoiseFilter();

    void Init( unsigned int FsCom, double Tetak, double WOk );
    void DataFilter( double Xk, double *pYk );
};

// Nice to have function
// Used by CFilter3 and CNoiseFilter
void freqToTs( unsigned int FsCom, double *Ts );
