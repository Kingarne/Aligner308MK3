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
IMPLEMENT_MEASUREMENT(GyroPerformance) ;
IMPLEMENT_MEASUREMENT(AzimuthAlignment) ;
//IMPLEMENT_HISTORY(AzimuthVerificationBenchmark) ;
//IMPLEMENT_HISTORY(AzimuthVerificationGyrostability) ;
IMPLEMENT_MEASUREMENT(HorizonAbsoluteMode) ;
IMPLEMENT_MEASUREMENT(HorizonRelativeMode) ;
IMPLEMENT_MEASUREMENT(CommonFlatTilt) ;
IMPLEMENT_MEASUREMENT(SensorValidation);
IMPLEMENT_MEASUREMENT(LiveGraph) ;
IMPLEMENT_MEASUREMENT(LiveDataA202) ;

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


BOOL GyroPerformance::AddData(Data &data )
{
	data.type = MeasType::MT_GyroPerf;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertGyroPerformance(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;

    return TRUE ;
}

BOOL GyroPerformance::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertGyroPerformanceChannel(item, m_measTypeID);
}

BOOL AzimuthAlignment::AddData(Data &data )
{
	//data.type = MeasType::MT_AzimuthAlign;

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
/*
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
*/
BOOL HorizonAbsoluteMode::AddData(Data &data )
{
	data.type = MeasType::MT_VerifAbsolute;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertHorizonAbsoluteMode(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;

    return TRUE ;
}

BOOL HorizonAbsoluteMode::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertHorizonAbsoluteModeChannel(item, m_measTypeID);
}

BOOL HorizonRelativeMode::AddData(Data &data )
{
	data.type = MeasType::MT_VerifRelative;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertHorizonRelativeMode(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;

	return TRUE;
    return TRUE ;
}

BOOL HorizonRelativeMode::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertHorizonRelativeModeChannel(item, m_measTypeID);
}

BOOL CommonFlatTilt::UpdateCalibrationFlag(BOOL b)
{
	return DBInterface::Instance()->UpdateCalibrationFlag("CommonFlatTilt", m_measID, b);
}

BOOL CommonFlatTilt::AddData(Data &data )
{
	data.type = MeasType::MT_CommonFlatTilt;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertCommonFlatTilt(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;
    return TRUE ;
}

BOOL CommonFlatTilt::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertCommonFlatTiltChannel(item, m_measTypeID);
}

BOOL SensorValidation::UpdateCalibrationFlag(BOOL b)
{
	return DBInterface::Instance()->UpdateCalibrationFlag("SensorValidation", m_measID, b);
}

BOOL SensorValidation::AddData(Data &data)
{
	data.type = MeasType::MT_SensorValidation;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertSensorValidation(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;	

	return TRUE;
}

BOOL SensorValidation::AddChannel(const ChannelData item)
{
	return DBInterface::Instance()->InsertSensorValidationChannel(item, m_measTypeID);
}


BOOL LiveGraph::AddData(Data &data )
{
	data.type = MeasType::MT_LiveGraph;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertLiveGraph(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;
         
    return TRUE ;
}

BOOL LiveGraph::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertLiveGraphChannel(item, m_measTypeID);
}

BOOL LiveDataA202::AddData(Data &data )
{        	 
	data.type = MeasType::MT_LiveGraph;

	DBInterface::Instance()->InsertMeasurement(data);
	int lastId = 0;
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measID = lastId;

	DBInterface::Instance()->InsertLiveDataA202(data, m_measID);
	DBInterface::Instance()->GetLastCounter(lastId);
	m_measTypeID = lastId;
    return TRUE ;
}

BOOL LiveDataA202::AddChannel( const ChannelData item )
{
    return DBInterface::Instance()->InsertLiveDataA202Channel(item, m_measTypeID);
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
    
