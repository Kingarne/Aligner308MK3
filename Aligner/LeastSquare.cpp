// $Id: LeastSquare.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "leastsquare.h"

LeastSquare::LeastSquare( void )
{
  Clear() ;
}

LeastSquare::~LeastSquare( void )
{
  // Empty
}

void LeastSquare::Clear( void )
{
  x0 = 0 ;
  x1 = 0 ;
  x2 = 0 ;
  x3 = 0 ;
  x4 = 0 ;
  x5 = 0 ;
  x6 = 0 ;
  y1 = 0 ;
  y2 = 0 ;
  y3 = 0 ;
  y4 = 0 ;
}

void LeastSquare::Add( double x, double y )
{
  double p ;
  x0 += 1 ;
  y1 += y ;
  x1 += p = x ;
  y2 += y * p ;
  x2 += p *= x ;
  y3 += y * p ;
  x3 += p *= x ;
  y4 += y * p ;
  x4 += p *= x ;
  x5 += p *= x ;
  x6 += p *= x ;
}

void LeastSquare::Delete( double x, double y )
{
  double p ;
  x0 -= 1 ;
  y1 -= y ;
  x1 -= p = x ;
  y2 -= y * p ;
  x2 -= p *= x ;
  y3 -= y * p ;
  x3 -= p *= x ;
  y4 -= y * p ;
  x4 -= p *= x ;
  x5 -= p *= x ;
  x6 -= p *= x ;
}

int LeastSquare::Create( int order, double &a0, double &a1, double &a2, double &a3 )
{
  double denom ;
  a0 = a1 = a2 = a3 = 0 ;
  switch (order)
  {
  case 3:
    if (3 < x0)
    {
      denom = (x4*x4*x2*x2+2.0*x0*x4*x5*x3+2.0*x3*x2*x1*x6+x0*x4*x2*x6-x2*x2*x2*x6-2.0*x4*x5*x2*x1-x0*x4*x4*x4+2.0*x4*x4*x1*x3+x1*x1*x5*x5-2.0*x5*x3*x3*x1-3.0*x4*x2*x3*x3-x0*x3*x3*x6+x3*x3*x3*x3+2.0*x2*x2*x5*x3-x0*x2*x5*x5-x4*x1*x1*x6);
      if (0 != denom)
      {
        a0 = (-x1*x4*x5*y3-x4*x2*x5*y2-y1*x3*x3*x6-x5*x3*x3*y2-x4*x3*x3*y3-x1*x4*y2*x6-y1*x4*x4*x4+x3*y2*x4*x4+y1*x4*x2*x6-y4*x5*x3*x1+x1*y4*x4*x4+x2*y3*x4*x4+y4*x2*x2*x5-2.0*x3*x4*x2*y4+x3*x3*x3*y4+2.0*y1*x4*x5*x3+x2*x3*y2*x6+x1*x3*y3*x6+x3*x2*x5*y3-y1*x5*x5*x2+x1*x5*x5*y2-x2*x2*y3*x6)/denom ;
        a1 = -(-x3*x1*x4*y4+x1*x3*x5*y3-x1*y1*x5*x5+x1*x5*x2*y4-x2*x1*y3*x6+x4*x1*y1*x6-x3*x3*x3*y3+x3*x3*x5*y1+x3*x3*x4*y2+x2*x3*x3*y4-2.0*x2*x5*x3*y2-x3*x5*x0*y4-x2*x3*y1*x6+x2*x4*x3*y3-x3*y1*x4*x4+x0*x3*y3*x6+x0*y2*x5*x5+x2*x4*x5*y1-x0*x4*x5*y3+x0*y4*x4*x4+x2*x2*y2*x6-x0*x4*y2*x6-x2*x2*x4*y4)/denom ;
        a2 = (x1*x1*x5*y4-y3*x1*x1*x6-x1*x4*x5*y1-x1*x5*x3*y2+x1*x2*y2*x6+x3*x1*y1*x6+2.0*x4*x1*x3*y3-x4*x1*x2*y4-x3*x3*x1*y4+x5*x0*x4*y2+y1*x5*x2*x3-x5*y4*x0*x2-x4*x3*x3*y1-x0*x3*y2*x6+x0*x2*y3*x6+x0*x3*x4*y4-x4*x2*x3*y2-x2*x2*y1*x6-x3*x3*x2*y3+x2*y1*x4*x4-x0*y3*x4*x4+x3*x3*x3*y2+x2*x2*x3*y4)/denom ;
        a3 = (-x1*x3*x3*y3+x4*x4*x1*y1-x2*x3*x3*y2-2.0*x4*x3*x2*y1-x0*x3*x3*y4-x4*x1*x1*y4-x0*y2*x4*x4-x5*x3*x1*y1+x0*x5*x3*y2+x4*x2*x2*y2+x2*x2*x3*y3-x4*x1*x2*y3+x2*x2*x5*y1+x1*x1*x5*y3-x0*x2*x5*y3-x2*x2*x2*y4+x4*x1*x3*y2-x5*x2*x1*y2+x3*x3*x3*y1+x0*x4*x3*y3+2.0*x3*x2*x1*y4+x0*x4*x2*y4)/denom ;
        return 3 ;
      }
    }

  case 2:
    if (2 < x0)
    {
      denom = (-x2*x2*x2+x2*x0*x4+2.0*x2*x1*x3-x1*x1*x4-x0*x3*x3);
      if (0 != denom)
      {
        a0 = (x3*x1*y3-x3*x3*y1+x2*x3*y2-x4*x1*y2+x4*y1*x2-y3*x2*x2)/denom ;
        a1 = -(x1*x4*y1-x1*x2*y3-x3*x2*y1+x3*x0*y3-x0*x4*y2+x2*x2*y2)/denom ;
        a2 = (x2*x1*y2-y1*x2*x2+x2*x0*y3+x1*x3*y1-x1*x1*y3-x0*x3*y2)/denom ;
        return 2 ;
      }
    }

  case 1:
    if (1 < x0)
    {
      denom = (x2*x0-x1*x1) ;
      if (0 != denom)
      {
        a0 = -(-x2*y1+x1*y2)/denom ;
        a1 = (-x1*y1+x0*y2)/denom ;
        return 1 ;
      }
    }

  case 0:
    if (0 != x0)
    {
      a0 = y1 / x0 ;
      return 0 ;
    }
  }
  return - 1 ;
}

Polynomial::Polynomial( void )
{
  m_order = -1 ;
}

int Polynomial::Create( int order, LeastSquare &source )
{
  return m_order = source.Create( order, m_a_0, m_a_1, m_a_2, m_a_3 ) ;
}

int Polynomial::GetOrder( void )
{
  return m_order ;
}

double Polynomial::operator()( double x )
{
  return m_a_0 + x * (m_a_1 + x * (m_a_2 + x * m_a_3)) ;
}
