// $Id: Type.cpp,v 1.5 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Type.h"

CString UnitType::GetUnitText( Types type )
{
  CString typeText ;
  switch (type)
  {
    case Unused:    typeText = _T("Unused");    break ;
    case Fixed:     typeText = _T("Fixed");     break ;
    case Platform:  typeText = _T("Platform");  break ;
    case Gun:       typeText = _T("Gun");       break ;
    case Coarse:    typeText = _T("Coarse");    break ;
    case Fine:      typeText = _T("Fine");      break ;
    case Composite: typeText = _T("Composite"); break ;
	case Theo:		typeText = _T("Theo"); break ;
	default:ASSERT(0); break ;
  }
  return typeText ;
}

bool UnitType::TypeHasAdapter( Types type ) 
{
	if(type == Gun || type == Theo)
		return true;

	return false;
}

UnitType::UnitType( void ) : m_type(Unused), m_overrangeDetection(TRUE), m_activeOverrangeDetection(FALSE), m_isSelected(FALSE)
{
  // Empty
}

UnitType::~UnitType( void )
{
  // Empty
}

UnitType::Types UnitType::GetType( void ) const
{
  return m_type ;
}

void UnitType::SetType( UnitType::Types type )
{
  m_type = type ;
}

void UnitType::SetOverrangeDetection( BOOL overrangeDetection )
{
  m_overrangeDetection = overrangeDetection ;
}

BOOL UnitType::GetOverrangeDetection( void ) const
{
  return m_overrangeDetection ;
}

void UnitType::SetOverrangeDetectionActive( BOOL activeOverrangeDetection )
{
  m_activeOverrangeDetection = activeOverrangeDetection ;
}

BOOL UnitType::GetOverrangeDetectionActive( void ) const
{
  return m_activeOverrangeDetection ;
}

BOOL UnitType::ShallDoOverrangeDetection()
{
    return (m_overrangeDetection && m_activeOverrangeDetection);
}

void UnitType::SetIsSelected( BOOL selectionStatus )
{
  m_isSelected = selectionStatus ;
}

BOOL UnitType::IsSelected( void ) const
{
  return m_isSelected ;
}

void UnitType::Rotate( double &rotatedX, double &rotatedY, double angle, double x, double y )
{
  rotatedX = x * cos( angle ) + y * sin( angle ) ;
  rotatedY = y * cos( angle ) - x * sin( angle ) ;
}

