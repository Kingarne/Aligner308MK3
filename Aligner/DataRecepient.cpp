// $Id: DataRecepient.cpp,v 1.6 2012/03/01 10:00:19 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "DataRecepient.h"
#include "UserMessages.h"

int DataRecepient::sm_refCount = 0;

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
  if(pDAU == NULL && m_pSource != NULL)
  {
    __unhook(&DAU::DataEvent, m_pSource, &DataRecepient::DataEvent) ;
    __unhook(&DAU::ErrorEvent, m_pSource, &DataRecepient::ErrorEvent) ;
    m_pSource = NULL;    
    sm_refCount--;
  }
    
  if(m_pSource == NULL)
  {
      if (( NULL != (m_pSource = pDAU) ))
    {
        __hook(&DAU::DataEvent, m_pSource, &DataRecepient::DataEvent) ;
        __hook(&DAU::ErrorEvent, m_pSource, &DataRecepient::ErrorEvent) ;
        sm_refCount++;
    }
  }
}

void DataRecepient::DataEvent( const DAUValues &data )
{
  CSingleLock _(&m_dataLock, TRUE) ;
  //TRACE("Data Event\n");
  m_data.push_back( data ) ;
  m_pControlingWindow->PostMessage( UM_NEW_DATA ) ;
}

void DataRecepient::ErrorEvent( ErrorEventInfo& info )
{
  if (m_handleError)
  {
    if (m_showError)
    {
      m_showError = FALSE;
		CString errorText( _T("DAU Internal error") ) ;
      switch(info.type)
	  {
		case ET_OUT_OF_RANGE:
			errorText.Format( _T("Channel %s overflow!"), info.errorInfo ) ;	
			break;

		case ET_COM_ERROR:
			errorText.Format( _T("DAU Com Error:%s"), info.errorInfo) ;	
			break;
		default:break;
	  }
		  
      ::AfxMessageBox( errorText, MB_ICONERROR ) ;
		m_showError = TRUE;
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