// $Id: DataRecepient.h,v 1.3 2012/03/01 10:00:20 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "DAU.h"

class DataRecepient
{
public:
	DataRecepient( CWnd *pControlingWindow ) ;
	virtual ~DataRecepient( void ) ;
public:
	DAU *GetSource( void ) ;
  void DataEvent( const DAUValues &data ) ;
  void ErrorEvent( ErrorEventInfo& info ) ;
	void SetSource( DAU *pDAU ) ;
	BOOL UpdateCurrentValues( BOOL purge = FALSE ) ;
  void SetHandleError( BOOL handle = TRUE ) { m_handleError = handle ;  }
  static int GetNumRecepient(){return sm_refCount;}
protected:

  static int sm_refCount; 
  CWnd *m_pControlingWindow ;
  DAU *m_pSource ;
  CCriticalSection m_dataLock ;
  list<DAUValues> m_data ;
  DAUValues m_currentValues ;
public:
  BOOL m_showError ;
  BOOL m_handleError ;
} ;

inline
DAU *DataRecepient::GetSource( void )
{
	return m_pSource ;
}
