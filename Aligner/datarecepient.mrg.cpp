// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\datarecepient.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:03
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: datarecepient.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "DataRecepient.h"
#include "UserMessages.h"

DataRecepient::DataRecepient( CWnd *pControlingWindow ) : m_pControlingWindow( pControlingWindow ), m_pSource(NULL)
{
	  m_showError = TRUE ;
    m_handleError = TRUE ;
}

DataRecepient::~DataRecepient(void)
{
	SetSource( NULL ) ;
}

void DataRecepient::SetSource( DAU *pDAU )
{
  if (m_pSource)
  {
    __unhook(&DAU::DataEvent, m_pSource, DataRecepient::DataEvent)
	//+++ Start Injected Code For Attribute 'unhook' from DataRecepient::SetSource
                #injected_line 24 ".\\DataRecepient.cpp"
(m_pSource)->__RemoveEventHandler_DAU_DataEvent((DataRecepient*) this, &DataRecepient::DataEvent)

	//--- End Injected Code For Attribute 'unhook'
 ;
    __unhook(&DAU::ErrorEvent, m_pSource, DataRecepient::ErrorEvent)
	//+++ Start Injected Code For Attribute 'unhook' from DataRecepient::SetSource
                #injected_line 25 ".\\DataRecepient.cpp"
(m_pSource)->__RemoveEventHandler_DAU_ErrorEvent((DataRecepient*) this, &DataRecepient::ErrorEvent)

	//--- End Injected Code For Attribute 'unhook'
 ;
  }

  if (( NULL != (m_pSource = pDAU) ))
  {
    __hook(&DAU::DataEvent, m_pSource, DataRecepient::DataEvent)
	//+++ Start Injected Code For Attribute 'hook' from DataRecepient::SetSource
                #injected_line 30 ".\\DataRecepient.cpp"
(m_pSource)->__AddEventHandler_DAU_DataEvent((DataRecepient*) this, &DataRecepient::DataEvent)

	//--- End Injected Code For Attribute 'hook'
 ;
    __hook(&DAU::ErrorEvent, m_pSource, DataRecepient::ErrorEvent)
	//+++ Start Injected Code For Attribute 'hook' from DataRecepient::SetSource
                #injected_line 31 ".\\DataRecepient.cpp"
(m_pSource)->__AddEventHandler_DAU_ErrorEvent((DataRecepient*) this, &DataRecepient::ErrorEvent)

	//--- End Injected Code For Attribute 'hook'
 ;
  }
}

void DataRecepient::DataEvent( const DAUValues &data )
{
  CSingleLock _(&m_dataLock, TRUE) ;
  m_data.push_back( data ) ;
	m_pControlingWindow -> PostMessage( UM_NEW_DATA ) ;
}

void DataRecepient::ErrorEvent( BOOL overFlow, CString offendingSensor )
{
  if (m_handleError)
  {
    if (m_showError)
    {
      CString errorText( _T("DAU Internal error") ) ;
      if (overFlow)
      {
        errorText.Format( _T("Channel %s overflow!"), offendingSensor ) ;
      }
      ::AfxMessageBox( errorText, MB_ICONERROR ) ;
    }
    m_pControlingWindow -> PostMessage( UM_DAU_ERROR ) ;
  }
}

BOOL DataRecepient::UpdateCurrentValues( BOOL purge )
{
	CSingleLock _(&m_dataLock, TRUE) ;
	BOOL dataUpdated ;
	if (( dataUpdated = (0 < m_data.size()) ))
	{
		if (purge)
		{
			m_currentValues.Set( *(m_data.rbegin()) ) ;
			m_data.clear() ;
		}
		else
		{
			m_currentValues.Set( *(m_data.begin()) ) ;
			m_data.pop_front() ;
		}
	}
	return dataUpdated ;
}