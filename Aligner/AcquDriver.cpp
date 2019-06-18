// $Id: AcquDriver.cpp,v 1.2 2009/10/30 15:06:44 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AcquDriver.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(CAcquDriver, CWnd)

CAcquDriver::CAcquDriver( void )
{
  // Empty
}

CAcquDriver::~CAcquDriver( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(CAcquDriver, CWnd)
END_MESSAGE_MAP()


/**
 *  @note This code contains a hack to simplify converting old procedural based code.
 *  Classes inheriting from CWnd can call ACQUDRIVER as if it was a normal function.
 *  DoEvents will handle events while waiting for AQCU_DATA_READY. The class using
 *  ACQUDRIVER should handle UM_NEW_DATA and set m_InParam.SourceStatus to ACQU_DATA_READY.
 *  to force the function to return in a normal manner.
 *
 *  In this manner it's simple to convert flowcharts to state machines.
 *
 *  Altough a beauty, this hack _must_ die before final release!
 */

void CAcquDriver::ACQUDRIVER( void )
{
  if( m_InParam.pSource == NULL )
  {
    g_AlignerData.ErrorDef = ERR_NO_SOURCE ;
    return ;
  }
  m_InParam.pSource->EnableErrorEvent( TRUE );
  m_InParam.SourceStatus = ACQU_GET_DATA ;
static int c =0;
  do
  {
     DoEvents() ;

    if ( m_InParam.SourceStatus == ACQU_DATA_READY )
    {
      m_InParam.SourceStatus = ACQU_IDLE ;
      int sI, syncroCount, sensorCount;
      syncroCount = m_InParam.pSource->GetSyncroCount();
      sensorCount = m_InParam.pSource->GetSensorCount();

      for ( int i = 0 ; i < syncroCount ; i++ )
      {
        sI = CONVERT_SYNCRO_CHANNEL_TO_ARRAY_INDEX( i );
        CString str;
        str = m_InParam.pSource -> GetSyncro( i ) -> GetName();
        m_OutParam.AbsRoll [sI] = -RADIANS_TO_MRADIANS( m_InParam.pSource -> GetSyncro( i ) -> GetRoll( TRUE ) ) ;
        m_OutParam.AbsPitch [sI] = -RADIANS_TO_MRADIANS( m_InParam.pSource -> GetSyncro( i ) -> GetPitch( TRUE ) ) ;
				m_OutParam.RollRawData [sI] = m_InParam.pSource -> GetSyncro( i ) -> GetData().m_roll ;
				m_OutParam.PitchRawData [sI] = m_InParam.pSource -> GetSyncro( i ) -> GetData().m_pitch ;
      }

      for( int i = 0 ; i < sensorCount ; i++ )
      {
        sI = CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX( i ) ;
        CString str;
        str = m_InParam.pSource -> GetSensor( i ) -> GetName();
        m_OutParam.AbsRoll[sI]  = -RADIANS_TO_MRADIANS( m_InParam.pSource -> GetSensor( i ) -> GetRoll( TRUE ) ) ;
        m_OutParam.AbsPitch[sI] = -RADIANS_TO_MRADIANS( m_InParam.pSource -> GetSensor( i ) -> GetPitch( TRUE ) ) ;
				m_OutParam.TempT[sI] = m_InParam.pSource -> GetSensor( i ) -> GetTemperature() ;
				m_OutParam.RollRawData[sI] = m_InParam.pSource -> GetSensor( i ) -> GetData().m_roll ;
				m_OutParam.PitchRawData[sI] = m_InParam.pSource -> GetSensor( i ) -> GetData().m_pitch ;
      }
			m_OutParam.TempT[MAIN_BOARD_ARRAY_INDEX] = m_InParam.pSource -> GetMainboard() -> GetTemperature() ;
    }
    Sleep(10);
  } while( (m_InParam.SourceStatus == ACQU_GET_DATA) &&
           (g_AlignerData.ErrorDef == ERR_NO_ERROR) ) ;

  m_InParam.pSource->EnableErrorEvent( FALSE );
}
