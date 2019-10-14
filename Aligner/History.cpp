// $Id: History.cpp,v 1.10 2014-02-04 09:29:31 riar Exp $F
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "History.h"
#include "Database.h"
#include "Digital.h"
#include "DAU.h"
#include "GlobalData.h"
#include "DBInterface.h"

BOOL DoDelete( LONG index )
{
    return DBInterface::Instance()->DeleteRecord("Measurement", index);        
}

#define IMPLEMENT_HISTORY(name) \
  LONG name##History::m_lastID ; \
  LONG name##History::m_lastItemID ; \
  LONG name##History::m_mainID ; \
  void name##History::SetLastID( LONG lastID ) { m_lastID = lastID ; } \
  LONG name##History::GetLastID( void ) { return m_lastID ; } \
  LONG name##History::GetMainID( void ) { return m_mainID ; } \
  BOOL name##History::DeleteLast( void ) { \
    return DoDelete( m_mainID ) ; \
  } \
  BOOL name##History::SetComment( const CString &comment) { \
  CString str(#name); \
  str.Format("%s%s",#name, "History"); \
  return DBInterface::Instance()->UpdateComment(str, m_mainID, comment);\
  } \
  BOOL name##History::AddGraph( CString filename, BOOL include ) { \
    if (m_mainID == 0) { \
        HistoryData hd; \
        CTime::GetCurrentTime().GetAsDBTIMESTAMP(hd.m_time) ; \
        DBInterface::Instance()->InsertHistoryItem(hd); \
        int lastId = 0; \
        DBInterface::Instance()->GetLastCounter(lastId); \
        m_mainID = lastId ; \
    } \
    return DBInterface::Instance()->InsertGraph(m_mainID, filename, include); \
  } \
  void name##History::ResetMainID( void ) { \
    m_mainID = 0 ; \
  }

#define IMPLEMENT_MEASUREMENT(name) \
  LONG name::m_measTypeID ; \
  LONG name::m_measChID ; \
  LONG name::m_measID ; \
  void name::SetMeasTypeID( LONG measTypeID ) { m_measTypeID = measTypeID ; } \
  LONG name::GetMeasTypeID( void ) { return m_measTypeID ; } \
  LONG name::GetMeasID( void ) { return m_measID ; } \
  void name::ResetMeasID( void ) { \
    m_measID = 0 ; \
  }

IMPLEMENT_MEASUREMENT(TiltAlignment) ;
IMPLEMENT_MEASUREMENT(TiltAndFlatness) ;
IMPLEMENT_MEASUREMENT(TiltAndFlatnessFo) ;
IMPLEMENT_HISTORY(GyroPerformanceTest) ;
IMPLEMENT_MEASUREMENT(AzimuthAlignment) ;
IMPLEMENT_HISTORY(AzimuthVerificationBenchmark) ;
IMPLEMENT_HISTORY(AzimuthVerificationGyrostability) ;
IMPLEMENT_HISTORY(HorizonAbsoluteMode) ;
IMPLEMENT_HISTORY(HorizonRelativeMode) ;
IMPLEMENT_HISTORY(CommonFlatTilt) ;
IMPLEMENT_HISTORY(SensorValidation);
IMPLEMENT_HISTORY(LiveGraphErrors) ;
IMPLEMENT_HISTORY(LiveDataA202Errors) ;

void LoadSigndefString( CString &signdefText )
{
  if( g_AlignerData.SignDef == -1 )
  {
    signdefText.LoadString( IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_NEG ) ;
  }
  else
  {
    signdefText.LoadString( IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_POS ) ;
  }
}

BOOL TiltAlignment::AddData( Data &data )
{
    data.type = MeasType::MT_TiltAlignment;
    
	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);        
    m_measID = lastId;

    DBInterface::Instance()->InsertTiltAlignment(data, m_measID);
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_measTypeID = lastId;

    return TRUE ;
}

BOOL TiltAlignment::AddChannel( const ChannelData chData )
{
    return DBInterface::Instance()->InsertTiltAlignmentChannel(chData, m_measTypeID);
}

BOOL TiltAndFlatness::AddData( Data &data )
{
	data.type = MeasType::MT_TiltFlatnessPl;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertTiltAndFlatness(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;

    return TRUE ;
}

BOOL TiltAndFlatness::AddChannel( const ChannelData item)
{
	BOOL res = DBInterface::Instance()->InsertTiltAndFlatnessChannel(item, m_measTypeID);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measChID = lastId;
	return TRUE;
}

BOOL TiltAndFlatness::AddChannelErr( const ChannelErrData item )
{
	return DBInterface::Instance()->InsertTiltAndFlatnessChannelErr(item, m_measChID);
}


BOOL TiltAndFlatness::AddExtData(const ExtChannelData extItem)
{
  return TRUE ;
}

BOOL TiltAndFlatnessFo::AddData( Data &data )
{
	data.type = MeasType::MT_TiltFlatnessFo;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertTiltAndFlatnessFo(data, m_measID);	
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;

 
    return TRUE ;
}

BOOL TiltAndFlatnessFo::AddChannel( const ChannelData item )
{
	BOOL res = DBInterface::Instance()->InsertTiltAndFlatnessFoChannel(item, m_measTypeID);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measChID = lastId;

	return res;
}

BOOL TiltAndFlatnessFo::AddChannelErr( const ChannelErrData item )
{
	return DBInterface::Instance()->InsertTiltAndFlatnessFoChannelErr(item, m_measChID);
}


BOOL TiltAndFlatnessFo::AddExtData( const ExtChannelData extItem )
{
  return TRUE ;
}


BOOL GyroPerformanceTestHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);
    	
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);  
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertGyroPerformanceTest(data, m_mainID);
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastID = lastId;

    return TRUE ;
}

BOOL GyroPerformanceTestHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertGyroPerformanceTestItem(item, m_lastID);
}

BOOL AzimuthAlignment::AddData(Data &data )
{
	data.type = MeasType::MT_AzimuthAlign;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertAzimuthAlignment(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;
    
    return TRUE ;
}

BOOL AzimuthAlignment::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertAzimuthAlignmentChannel(item, m_measTypeID);
}

BOOL AzimuthVerificationBenchmarkHistory::AddData(Data &data )
{    
    DBInterface::Instance()->InsertHistoryItem(data);    

    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);      
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertAzimuthVerificationBenchmark(data, m_mainID);                        
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastID = lastId ;

    return TRUE ;
}

BOOL AzimuthVerificationBenchmarkHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertAzimuthVerificationBenchmarkItem(item, m_lastID);                        
}

BOOL AzimuthVerificationGyrostabilityHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);    
	
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertAzimuthVerificationGyrostability(data, m_mainID);                        
    DBInterface::Instance()->GetLastCounter(lastId);
    m_lastID = lastId ;

    return TRUE ;
}

BOOL AzimuthVerificationGyrostabilityHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertAzimuthVerificationGyrostabilityItem(item, m_lastID);                            
}

BOOL HorizonAbsoluteModeHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);
    
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);  
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertHorizonAbsoluteMode(data, m_mainID);           
    DBInterface::Instance()->GetLastCounter(lastId);
    m_lastID = lastId ;

    return TRUE ;
}

BOOL HorizonAbsoluteModeHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertHorizonAbsoluteModeItem(item, m_lastID);                                
}

BOOL HorizonRelativeModeHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);  
	    
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId); 
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertHorizonRelativeMode(data, m_mainID);           
    DBInterface::Instance()->GetLastCounter(lastId);

    m_lastID = lastId ;
  
    return TRUE ;
}

BOOL HorizonRelativeModeHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertHorizonRelativeModeItem(item, m_lastID);                                    
}

BOOL CommonFlatTiltHistory::UpdateCalibrationFlag(BOOL b)
{
	return DBInterface::Instance()->UpdateCalibrationFlag("CommonFlatTiltHistory", m_mainID, b);
}

BOOL CommonFlatTiltHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);  	
	
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId); 
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);
  
    m_mainID = lastId;
    DBInterface::Instance()->InsertCommonFlatTilt(data, m_mainID);               
    DBInterface::Instance()->GetLastCounter(lastId);
    m_lastID = lastId;

    return TRUE ;
}

BOOL CommonFlatTiltHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertCommonFlatTiltItem(item, m_lastID);                                        
}

BOOL SensorValidationHistory::UpdateCalibrationFlag(BOOL b)
{
	return DBInterface::Instance()->UpdateCalibrationFlag("SensorValidationHistory", m_mainID, b);
}

BOOL SensorValidationHistory::AddData(Data &data)
{
	DBInterface::Instance()->InsertHistoryItem(data);

	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	DBInterface::Instance()->InsertHistoryPrintItem(lastId);

	m_mainID = lastId;
	DBInterface::Instance()->InsertSensorValidation(data, m_mainID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_lastID = lastId;

	return TRUE;
}

BOOL SensorValidationHistory::AddItem(const ItemData item)
{
	return DBInterface::Instance()->InsertSensorValidationItem(item, m_lastID);
}


BOOL LiveGraphErrorsHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);    
	
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);    
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertLiveGraphErrors(data, m_mainID);       
    DBInterface::Instance()->GetLastCounter(lastId);
    m_lastID = lastId ;
         
    return TRUE ;
}

BOOL LiveGraphErrorsHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertLiveGraphErrorsItem(item, m_lastID);                                           
}

BOOL LiveDataA202ErrorsHistory::AddData(Data &data )
{        	 
    DBInterface::Instance()->InsertHistory2Item(data.m_time); 
    
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);

    m_mainID = lastId;
    DBInterface::Instance()->InsertLiveDataA202Errors(data, m_mainID);        
    DBInterface::Instance()->GetLastCounter(lastId);
    m_lastID = lastId ;

    return TRUE ;
}

BOOL LiveDataA202ErrorsHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertLiveDataA202ErrorsItem(item, m_lastID);                                              
}

//#include "Util.h"

// BOOL MoveFilesToTemporary( LONG mainId )
// {
//     MoveFiles move ;
//     SET_DB_VALUE(move,ID,mainId) ;
//     HRESULT hResult = ::OpenTable( move ) ;
//     if (FAILED(hResult))
//     {
//         return FALSE ;
//     }
//     hResult = move.MoveFirst() ;
//     while (S_OK == hResult)
//     {
//         MoveFileToTemporaryDirectory( CString( move.m_filename ) ) ;
//         hResult = move.MoveNext() ;
//     }
//     return TRUE ;
// }
    
