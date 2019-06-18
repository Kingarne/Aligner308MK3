// $Id: MainBoard.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "DAUData.h"

class MainBoard
{
public:
  MainBoard( void ) ;
  ~MainBoard( void ) ;
public:
  void SetData( const MainBoardData &data ) ;
  void SetTemperatureData( const TemperatureData &data ) ;
  void SetTemperatureData( const TemperatureDataPro &data );
  //void SetTestVoltage( WORD testVoltage ) ;
  MainBoardData GetData( void ) const ;
  TemperatureData GetTemperatureData( void ) const ;
  WORD GetIdentity( void ) const ;
  BOOL IsAcqusitionInProgress( void ) const ;
  BOOL IsResetPerformed( void ) const ;
  BOOL IsFifoHalfFull( void ) const ;
  BOOL IsFifoEmpty( void ) const ;
  BOOL IsSDError( void ) const ;
  WORD GetTemperatureChannel( void ) const ;
  //WORD GetTestVoltage( void ) const ;
  double GetTemperature( void ) const ;
private:
  MainBoardData m_data ;
  TemperatureData m_temperatureData ;
  TemperatureCalibrationData m_temperatureCalibrationData ;
  //WORD m_testVoltage ;
} ;

inline
void MainBoard::SetData( const MainBoardData &data )
{
  m_data = data ;
}

inline
void MainBoard::SetTemperatureData( const TemperatureDataPro &data )
{
    TemperatureData d;
    d.m_temperature = data.m_temperature;
    SetTemperatureData(d);
}

inline
void MainBoard::SetTemperatureData( const TemperatureData &data )
{
  m_temperatureData = data ;
}

//inline
//void MainBoard::SetTestVoltage( WORD testVoltage )
//{
//  m_testVoltage = testVoltage ;
//}

//inline
//WORD MainBoard::GetTestVoltage( void ) const
//{
//  return m_testVoltage ;
//}

inline
MainBoardData MainBoard::GetData( void ) const
{
  return m_data ;
}

inline
TemperatureData MainBoard::GetTemperatureData( void ) const
{
  return m_temperatureData ;
}

inline
WORD MainBoard::GetIdentity( void ) const
{
  return m_data.m_identityData ;
}

inline
BOOL MainBoard::IsAcqusitionInProgress( void ) const
{
  return m_data.m_acqusitionInProgress ;
}

inline
BOOL MainBoard::IsResetPerformed( void ) const
{
  return m_data.m_resetPerformed ;
}


inline
BOOL MainBoard::IsFifoHalfFull( void ) const
{
  return m_data.m_fifoHalfFull ;
}

inline
BOOL MainBoard::IsFifoEmpty( void ) const
{
  return m_data.m_fifoEmpty ;
}

inline
BOOL MainBoard::IsSDError( void ) const
{
  return m_data.m_sdError ;
}

inline
WORD MainBoard::GetTemperatureChannel( void ) const
{
  return m_data.m_temperatureChannel ;
}

inline
double MainBoard::GetTemperature( void ) const
{
  return m_temperatureCalibrationData( m_temperatureData.m_temperature ) ;
}
