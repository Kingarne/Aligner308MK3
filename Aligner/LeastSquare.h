// $Id: LeastSquare.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class LeastSquare
{
public:
  LeastSquare( void ) ;
  ~LeastSquare( void ) ;
public:
  void Clear( void ) ;
  void Add( double x, double y ) ;
  void Delete( double x, double y ) ;
  int Create( int order, double &a0, double &a1, double &a2, double &a3 ) ;
private:
  double x0 ;
  double x1 ;
  double x2 ;
  double x3 ;
  double x4 ;
  double x5 ;
  double x6 ;
  double y1 ;
  double y2 ;
  double y3 ;
  double y4 ;
} ;

class Polynomial
{
public:
  Polynomial( void ) ;
public:
  int Create( int order, LeastSquare &source ) ;
  int GetOrder( void ) ;
public:
  double operator()( double x ) ;
public:
  double m_a_0 ;
  double m_a_1 ;
  double m_a_2 ;
  double m_a_3 ;
  int m_order ;
} ;
