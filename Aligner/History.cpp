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
    return DBInterface::Instance()->DeleteRecord("History", index);        
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

IMPLEMENT_HISTORY(TiltAlignmentErrors) ;
IMPLEMENT_HISTORY(TiltAndFlatness) ;
IMPLEMENT_HISTORY(TiltAndFlatnessFo) ;
IMPLEMENT_HISTORY(GyroPerformanceTest) ;
IMPLEMENT_HISTORY(AzimuthAlignmentErrors) ;
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

BOOL TiltAlignmentErrorsHistory::AddData(  Data &data )
{
    int lastId=0;
    DBInterface::Instance()->InsertHistoryItem(data);
    DBInterface::Instance()->GetLastCounter(lastId);  
    
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);    
    m_mainID = lastId;

    DBInterface::Instance()->InsertTiltAlignmentErrors(data, m_mainID);
    DBInterface::Instance()->GetLastCounter(lastId);  

    m_lastID = lastId;
    return TRUE ;
}

BOOL TiltAlignmentErrorsHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertTiltAlignmentErrorsItem(item, m_lastID);
}

BOOL TiltAndFlatnessHistory::AddData( Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);    
	
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);  
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);
    
    m_mainID = lastId;
    DBInterface::Instance()->InsertTiltAndFlatness(data, m_mainID);
    
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastID = lastId;

    return TRUE ;
}

BOOL TiltAndFlatnessHistory::AddItem( const ItemData item)
{
    BOOL res = DBInterface::Instance()->InsertTiltAndFlatnessItem(item, m_lastID);
    int lastId;
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastItemID = lastId;
    return res;
}

BOOL TiltAndFlatnessHistory::AddItemErr( const ItemErrData item )
{
	return DBInterface::Instance()->InsertTiltAndFlatnessItemErr(item, m_lastItemID);
}


BOOL TiltAndFlatnessHistory::AddExtItem( const ExtItemData extItem )
{
  return TRUE ;
}

BOOL TiltAndFlatnessFoHistory::AddData( Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);    

    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);  
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);
  
    m_mainID = lastId;
    DBInterface::Instance()->InsertTiltAndFlatnessFo(data, m_mainID);
   
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastID = lastId;
     
    return TRUE ;
}

BOOL TiltAndFlatnessFoHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertTiltAndFlatnessFoItem(item, m_lastID);
}

BOOL TiltAndFlatnessFoHistory::AddItemErr( const ItemErrData item )
{
	return DBInterface::Instance()->InsertTiltAndFlatnessFoItemErr(item, m_lastID);
}


BOOL TiltAndFlatnessFoHistory::AddExtItem( const ExtItemData extItem )
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

BOOL AzimuthAlignmentErrorsHistory::AddData(Data &data )
{
    DBInterface::Instance()->InsertHistoryItem(data);
       
    int lastId = 0;
    DBInterface::Instance()->GetLastCounter(lastId);  
    DBInterface::Instance()->InsertHistoryPrintItem(lastId);
  
    m_mainID = lastId;
    DBInterface::Instance()->InsertAzimuthAlignmentErrors(data, m_mainID);                
    DBInterface::Instance()->GetLastCounter(lastId);  
    m_lastID = lastId;
    
    return TRUE ;
}

BOOL AzimuthAlignmentErrorsHistory::AddItem( const ItemData item )
{
    return DBInterface::Instance()->InsertAzimuthAlignmentErrorsItem(item, m_lastID);                    
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
    
