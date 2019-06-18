 // $Id: Filter.cpp,v 1.4 2013/02/13 14:25:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "Filter.h"
#include <Float.h> //DBL_EPSILON

DataFilter::DataFilter( void )
{
    m_type = None ;
}

DataFilter::~DataFilter( void )
{
    // Empty
}

void DataFilter::Reset( void )
{
    //  CString debugText ;
    //  debugText.Format( _T("DataFilter::Reset()") ) ;
    //  ::OutputDebugString( debugText ) ;
    m_type = None ;
    m_x = 0.0 ;
}

void DataFilter::Reset( double damping )
{
    //  CString debugText ;
    //  debugText.Format( _T("DataFilter::Reset( damping = %f\n"), damping ) ;
    //  ::OutputDebugString( debugText ) ;
    m_type = FirstOrder ;
    m_theta = damping ;
    m_x = 0.0 ;
}

void DataFilter::Reset( double period, double theta, double tau, double initialValue )
{
    //  CString debugText ;
    //  debugText.Format( _T("DataFilter::Reset( period = %f, theta = %f, tau = %f, initialValue = %f\n"), period, theta, tau, initialValue ) ;
    //  ::OutputDebugString( debugText ) ;
    m_type = ThirdOrder ;
    m_period = period ;
    m_theta = theta ;
    m_tau = tau ;
    m_x = initialValue ;
    m_y = initialValue ;
    m_unfiltered = initialValue;
	m_z = ( 2 * theta * initialValue ) / tau;
}

void DataFilter::Set( double value )
{
    m_unfiltered = value;
    switch (m_type)
    {
    case None:
        m_x = value ;
        break ;

    case FirstOrder:
        m_x = (1.0 - m_theta) * value + m_theta * m_x ;
        break ;

    case ThirdOrder:
        {
        double delta ;
        delta = (value - m_y) ; 
        m_y += m_period * (m_z - 2 * m_theta * m_y / m_tau) ;
        m_z += m_period * delta / m_tau / m_tau ;
        m_x += m_period * (m_y - m_x) / m_tau ;
        }
        break ;

    default:
        ASSERT(0) ;  // TODO: Handle this severe internal error?
        break ;
    }
}

CNoiseFilter::~CNoiseFilter()
{
}

void CNoiseFilter::Init( unsigned int FsCom, double Tetak, double W0k )
{
    double Expr1, Expr2, Ts; 

    m_Tk = m_Tk1 = m_Tk2 = 0;
    m_K = m_A = m_B = m_C = m_D = 0;
    FreqToTs( FsCom, &Ts );

    if( W0k*Ts > DBL_EPSILON )
    {
        Expr1 = 1 + 4*Tetak / (W0k*Ts) + ( 2 / (W0k*Ts) ) * ( 2 / (W0k*Ts) );
        Expr2 = 1 + 2*Tetak*2 / M_PI + ( 2/M_PI ) * ( 2/M_PI );

        if( (Expr2 > DBL_EPSILON) & (Expr1 > DBL_EPSILON) )
        {
            m_K = Expr1 / Expr2;
            m_A = 2 * ( 1 - ( 2 / (W0k*Ts) ) * ( 2 / (W0k*Ts) ) ) / Expr1;
            m_B = ( 1 - 4*Tetak / (W0k*Ts) + ( 2 / (W0k*Ts) ) * ( 2 / (W0k*Ts) ) ) / Expr1;
            m_C = ( 2 * ( 1 - ( 2/M_PI ) * ( 2/M_PI ) ) ) / Expr2;
            m_D = ( 1 - 2*Tetak*2 / M_PI + ( 2/M_PI ) * ( 2/M_PI ) ) / Expr2;
        }
    }
}

// double Xk    the invalue to be DataFiltered
// double *pYk  the DataFiltered outvalue
void CNoiseFilter::DataFilter( double Xk, double *pYk )
{
    m_Tk = Xk - m_C*m_Tk1 - m_D*m_Tk2;
    *pYk = m_K * (m_Tk + m_A*m_Tk1 + m_B*m_Tk2);
    m_Tk2 = m_Tk1;
    m_Tk1 = m_Tk;
}
