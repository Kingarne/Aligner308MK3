// $Id: Type.h,v 1.4 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define OVERRANGE_LEVEL 31600

class UnitType
{
public:
  enum Types {
    Unused = 0,
    Fixed = 1,
    Platform = 2,
    Gun = 3,
    Coarse = 4,
    Fine = 5,
    Composite = 6,
	Theo = 7
  } ;
public:
	static CString GetUnitText( Types type ) ;
    static bool TypeHasAdapter( Types type ) ;
public:
  UnitType( void ) ;
  virtual ~UnitType( void ) ;
public:
  static void Rotate( double &rotatedX, double &rotatedY, double angle, double x, double y );
public:
  void SetType( Types type ) ;
  Types GetType( void ) const ;
  void SetOverrangeDetection( BOOL overrangeDetection ) ;
  BOOL GetOverrangeDetection( void ) const ;
  void SetOverrangeDetectionActive( BOOL activeOverrangeDetection ) ;
  BOOL GetOverrangeDetectionActive( void ) const ;
  void SetIsSelected( BOOL selectionStatus = TRUE ) ;
  BOOL IsSelected( void ) const ;
  BOOL ShallDoOverrangeDetection();
protected:
  Types m_type ;
  BOOL m_overrangeDetection ;
  BOOL m_activeOverrangeDetection ;
  BOOL m_isSelected ;
} ;

class Offset
{
public:
  Offset( void ) ;
  virtual ~Offset( void ) ;
public:
  int GetOffset( void ) const ;
  void SetOffset( int offset ) ;
private:
  int m_offset ;
} ;

inline
int Offset::GetOffset( void ) const
{
  return m_offset ;
}

inline
void Offset::SetOffset( int offset )
{
  m_offset = offset ;
}
