// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\calibrationsheet.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:32
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: calibrationsheet.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "Aligner.h"
#include "CalibrationSheet.h"
#include "SystemSetup.h"
#include "Database.h"
#include "UserMessages.h"
#include "ASound.h"
#include "Util.h"

#define DISPLAY_INSTRUCTIONS 1
#define PERFORM_MEASUREMENT  2
#define PERFORM_AVERAGE_MEASURMENT 3

#if defined(_DEBUG)
#define TAU 0.2
#else
#define TAU 1.0
#endif
#define THETA 0.9

IMPLEMENT_DYNAMIC(CalibrationSheet, CPropertySheet)

/** Top level CPropertySheet derived class for handling calibration measurements.
 *  @param caption, UINT marking string resource to use as sheet title.
 */
CalibrationSheet::CalibrationSheet( UINT caption, CalibrationType calibrationType ) : CPropertySheet( caption ), DataRecepient( this )
{
  if( calibrationType == ScaleCalibration && caption == IDS_DAILY_AZIMUTH_ERROR_TEST_SETUP_TITLE )
  {
    m_calibrationType = DayToDayScaleCalibration;
  }
  else
  {
    m_calibrationType = calibrationType ;
  }
  SetTimeConstant( TAU ) ;
  DAU::GetDAU().SetOverrange( FALSE ) ;
  m_voltage = 0 ;
  SetWizardMode() ;
}

/**
 *  There is nothing to destroy , all cleanup is handled, and should be handled, in OnDestroy.
 */
CalibrationSheet::~CalibrationSheet( void )
{
  // Empty
}

BEGIN_MESSAGE_MAP(CalibrationSheet, CPropertySheet)
  ON_WM_DESTROY()
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
	ON_MESSAGE(UM_DAU_ERROR, OnDauError)
  ON_WM_SETCURSOR()
END_MESSAGE_MAP()

void CalibrationSheet::ResetFilter( BOOL useFilter )
{
  if (useFilter)
  {
    m_referenceData.m_pSource -> ResetFilter( DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() ), THETA, m_timeConstant ) ;
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
      i -> m_pSource -> ResetFilter( DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() ), THETA, m_timeConstant ) ;
    }
  }
  else
  {
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
      i -> m_roll [m_measurement] = 0 ;
      i -> m_pitch [m_measurement] = 0 ; 
      i -> m_temperature [m_measurement] = 0 ;
      i -> m_pSource -> ResetFilter() ;
    }
  }
}

BOOL CalibrationSheet::OnInitDialog( void )
{
  __super::OnInitDialog() ;
  m_mode = DISPLAY_INSTRUCTIONS ;
  m_showAsSum = FALSE ;
  m_pitchOffset = 0.0 ;
  m_rollOffset = 0.0 ;
  m_pBarGraph = NULL;

  switch (m_calibrationType)
  {
  case DayToDayScaleCalibration:
  case ScaleCalibration:
  case OffsetCalibration:
  case AdapterCalibration:
    DAU::GetDAU().SetFrequency( 0x11 ) ; // 30 Hz
    m_pGraph = static_cast<CGraphView *>(theApp.m_pGraphView) ;
    m_pGraph -> InitCalibrationGraph( (int)(2 * m_sensorData.size()), 0 ) ;
  	theApp.SwitchView( theApp.m_pGraphView ) ;
    break ;

  case DAUOffsetCalibration:
  case DAUScaleCalibration:
    DAU::GetDAU().SetFrequency( 0x10 ) ; // 60 Hz
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
  m_hCursor = ::LoadCursor( NULL, IDC_WAIT ) ;
  DAU::GetDAU().Start() ;
	SetSource( &(DAU::GetDAU()) ) ;
  return TRUE ;
}

void CalibrationSheet::OnDestroy( void )
{
	SetSource( NULL ) ;
	theApp.SwitchView( static_cast<CGraphView *>(theApp.m_pSystemConfigurationView) ) ;
  DAU::GetDAU().Stop() ;
  ::DestroyCursor( m_hCursor ) ;
  __super::OnDestroy() ;
}

BOOL CalibrationSheet::IsMeasuring( void ) const
{
  return( DISPLAY_INSTRUCTIONS != m_mode );
}

void CalibrationSheet::StopMeasurement( void )
{
  DAU::GetDAU().EnableErrorEvent( FALSE ) ;
  DAU::GetDAU().Stop() ;
  m_mode = DISPLAY_INSTRUCTIONS ;
}

void CalibrationSheet::InitVoltage( double voltage )
{
  m_voltage = voltage ;
  DAU::GetDAU().StartWithTest( 0 < m_voltage ? (m_voltage < 6 ? 48496 : 64880) : (m_voltage > -6 ? 17039 : 655) ) ;
  DAU::GetDAU().Stop() ;  
}

void CalibrationSheet::StartMeasurement( double voltage )
{
//  m_voltage = voltage ;
  StartMeasurement() ;
}

void CalibrationSheet::StartMeasurement( BOOL showAsSum, double pitchOffset, double rollOffset, BOOL fixYAxis )
{
  ASSERT(DISPLAY_INSTRUCTIONS == m_mode) ;
  if (0 == m_voltage)
  {
    DAU::GetDAU().Start() ;
  }
  else
  {
    DAU::GetDAU().StartWithTest( 0 < m_voltage ? (m_voltage < 6 ? 48496 : 64880) : (m_voltage > -6 ? 17039 : 655) ) ;
  }
  DAU::GetDAU().EnableErrorEvent( TRUE ) ;
  ActivateSensors() ;
  m_showAsSum = showAsSum ;
  m_pitchOffset = pitchOffset ;
  m_rollOffset = rollOffset ;
  switch (m_calibrationType)
  {
  case DayToDayScaleCalibration:
  case ScaleCalibration:
  case OffsetCalibration:
  case AdapterCalibration:
    InitGraph( fixYAxis ) ;
    m_mode = PERFORM_MEASUREMENT ;
    ResetFilter( TRUE ) ;
    break ;

  case DAUOffsetCalibration:
  case DAUScaleCalibration:
    ResetFilter( FALSE ) ;
    m_mode = PERFORM_AVERAGE_MEASURMENT ;
  break ;

  default:
    ASSERT(0) ;
    break ;
  }
  m_sample = 0 ;
  DAU::GetDAU().EnableErrorEvent( TRUE ) ;
}

LRESULT CalibrationSheet::OnNewData( WPARAM, LPARAM )
{
  while( UpdateCurrentValues() )
  { 
    switch (m_mode)
    {
    case DISPLAY_INSTRUCTIONS:
      break ;

    case PERFORM_AVERAGE_MEASURMENT:
      UpdateAverageData() ;
      if (PointToSample() <= ++m_sample)
      {
        DAU::GetDAU().EnableErrorEvent( FALSE ) ;
        FinalizeAverageData() ;
        m_mode = DISPLAY_INSTRUCTIONS ;
        ASound().PlayNiceSound();
        PressButton( PSBTN_NEXT ) ;
        return TRUE ;
      }
      break ;

    case PERFORM_MEASUREMENT:
      if( m_calibrationType == AdapterCalibration || m_calibrationType == DayToDayScaleCalibration )
      {
        UpdateGraph( TRUE ) ;
      }
      else
      {
        UpdateGraph( FALSE ) ;
      }

      if (PointToSample() <= ++m_sample)
      {
        DAU::GetDAU().EnableErrorEvent( FALSE ) ;
        if( m_calibrationType == AdapterCalibration || m_calibrationType == DayToDayScaleCalibration )
        {
          FinalizeMeasurmentData( TRUE ) ;
        }
        else
        {
          FinalizeMeasurmentData( FALSE ) ;
        }
        m_mode = DISPLAY_INSTRUCTIONS ;
        ASound().PlayNiceSound();
        PressButton( PSBTN_NEXT ) ;
        return TRUE ;
      }
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
  }
  return TRUE ;
}

LRESULT CalibrationSheet::OnDauError( WPARAM, LPARAM )
{
  switch (m_mode)
  {
  case DISPLAY_INSTRUCTIONS:
    break ;

  case PERFORM_AVERAGE_MEASURMENT:
  case PERFORM_MEASUREMENT:
    DAU::GetDAU().Restart( TRUE );
		PressButton( PSBTN_CANCEL ) ;
		break ;

	default:
		ASSERT(0) ;
		break ;
	}
	return TRUE ;
}

void CalibrationSheet::UpdateGraph( BOOL useCalibrated )
{
    int serie = 1 ;
    double xPosition = m_sample * DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() ) ;
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
      if (m_showAsSum)
      {
        m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_pitchOffset + m_referenceData.m_pSource -> GetRoll( useCalibrated )  + i -> m_pSource -> GetRoll( useCalibrated )) ) ) ;
        m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_rollOffset + m_referenceData.m_pSource -> GetPitch( useCalibrated ) + i -> m_pSource -> GetPitch( useCalibrated )) ) ) ;
      }
      else
      {
        if (0 == m_pitchOffset)
        {
          m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_pitchOffset + m_referenceData.m_pSource -> GetRoll( useCalibrated )  - i -> m_pSource -> GetRoll( useCalibrated )) ) ) ;
          m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_rollOffset + m_referenceData.m_pSource -> GetPitch( useCalibrated ) - i -> m_pSource -> GetPitch( useCalibrated )) ) ) ;
        }
        else
        {
          m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_pitchOffset + m_referenceData.m_pSource -> GetPitch( useCalibrated )  + i -> m_pSource -> GetRoll( useCalibrated )) ) ) ;
          m_pGraph -> SetXYData( serie++, xPosition, UnitsToMilliunits( ( m_rollOffset + m_referenceData.m_pSource -> GetRoll( useCalibrated ) - i -> m_pSource -> GetPitch( useCalibrated )) ) ) ;
        }
      }
      m_pGraph -> UpdateData() ;
    }
}

void CalibrationSheet::FinalizeMeasurmentData( BOOL useCalibrated )
{
  m_referenceData.m_roll [m_measurement] = m_referenceData.m_pSource -> GetRoll( useCalibrated ) ;
  m_referenceData.m_pitch [m_measurement] = m_referenceData.m_pSource -> GetPitch( useCalibrated ) ;
  m_referenceData.m_temperature [m_measurement] = m_referenceData.m_pSource -> GetTemperature() ;

  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    i -> m_roll [m_measurement] = i -> m_pSource -> GetRoll( useCalibrated ) ;
    i -> m_pitch [m_measurement] = i -> m_pSource -> GetPitch( useCalibrated ) ;
    i -> m_temperature [m_measurement] = i -> m_pSource -> GetTemperature() ;
  }
}

static void AddText( CGraphView *pGraph, CString &text, int &position )
{
  IChartLabel_DualPtr pLabel = pGraph -> m_pChart -> GetChartLabels() -> Add() ;
  pLabel -> GetText() -> PutText( _bstr_t( text ) ) ;
  pLabel -> PutAnchor( oc2dAnchorEast ) ;
  pLabel -> PutAttachMethod( oc2dAttachCoord ) ;
  pLabel -> GetAttachCoord() -> PutX( pGraph -> GetControlWidth() - 200 ) ;
  pLabel -> GetAttachCoord() -> PutY( position ) ;
  position += 14 ;
}

static void AddText( CGraphView *pGraph, CString &text1, CString &text2, int &position )
{
  CString text = text1 + text2 ;
  AddText( pGraph, text, position ) ;
}

void CalibrationSheet::PlotConverted( int mode )
{
  CString text ;
  if (4 == mode)
  {
    int position = m_position + 5 ;
    text.Format(  _T("Reference %3s"), m_referenceData.m_pSource -> GetSerialNumber() ) ;
    AddText( m_pGraph, text, position ) ;
  }
  else
  {
    m_position += 5 ;
    text.Format(  _T("Reference %3s %+5.1f [°C]"), m_referenceData.m_pSource -> GetSerialNumber(), m_referenceData.m_temperature [m_measurement] ) ;
    AddText( m_pGraph, text, m_position ) ;
    m_position += 5 ;
  }
  int j = 0 ;
  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    double roll ;
    double pitch ;
    switch (mode)
    {
    case 0:
      roll =  RADIANS_TO_MRADIANS(  i -> m_roll  [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(  i -> m_pitch [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 1:
      roll = RADIANS_TO_MRADIANS(  i -> m_pitch [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i -> m_roll  [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      //roll = RADIANS_TO_MRADIANS(  i -> m_pitch [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      //roll = RADIANS_TO_MRADIANS(- i -> m_roll  [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 2:
      roll  = RADIANS_TO_MRADIANS(- i -> m_roll  [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i -> m_pitch [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 3:
      roll  = RADIANS_TO_MRADIANS(- i -> m_roll  [m_measurement] + m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i -> m_pitch [m_measurement] + m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 4:
      break ;

    default:
      ASSERT(0) ;
    }
    int serie = ( (ScaleCalibration == m_calibrationType || DayToDayScaleCalibration == m_calibrationType) && 0 == m_rollOffset ) ? 2 - j : j + 1 ; j++ ;
    CString label ;
    if (4 == mode)
    {
      label.Format(  _T("Roll  %3s"), i -> m_pSource -> GetSerialNumber() ) ;
    }
    else
    {
      label.Format(  _T("Roll  %3s %+7.3f [mrad] %+5.1f [°C]"), i -> m_pSource -> GetSerialNumber(), roll,  i -> m_temperature [m_measurement] ) ;
    }
    m_pGraph -> SetSerieLabel( serie, label ) ;
    serie = ( (ScaleCalibration == m_calibrationType || DayToDayScaleCalibration == m_calibrationType) && 0 == m_rollOffset ) ? 2 - j : j + 1 ; j++ ;
    if (4 == mode)
    {
      label.Format(  _T("Pitch %3s"), i -> m_pSource -> GetSerialNumber() ) ;
    }
    else
    {
      label.Format(  _T("Pitch %3s %+7.3f [mrad] %+5.1f [°C]"), i -> m_pSource -> GetSerialNumber(), pitch, i -> m_temperature [m_measurement] ) ;
    }
    m_pGraph -> SetSerieLabel( serie, label ) ;
  }
}

void CalibrationSheet::UpdateAverageData( void )
{
  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    i -> m_roll [m_measurement] += i -> m_pSource -> GetData().m_roll ;
    i -> m_pitch [m_measurement] += i -> m_pSource -> GetData().m_pitch ;
    i -> m_temperature [m_measurement] += i -> m_pSource -> GetTemperature() ;
  }
}

void CalibrationSheet::InitGraph( BOOL fixYAxis )
{
  m_pGraph -> SetNoOfSeries( (int)(2 * m_sensorData.size()), FALSE ) ;
  m_pGraph -> m_pChart -> PutIsBatched( TRUE ) ;
  m_pGraph -> m_pChart -> GetChartLabels() -> RemoveAll() ;
  m_pGraph -> SetAxisXMin( 0, FALSE ) ;
  m_pGraph -> SetAxisXMax( m_timeConstant * 15, FALSE ) ;
  m_pGraph -> SetAxisYMin( -5, FALSE ) ;
  m_pGraph -> SetAxisYMax( +5, FALSE ) ;
  if (fixYAxis)
  {
    m_pGraph -> SetAxisYMinAutoScaleOn( FALSE, FALSE ) ;
    m_pGraph -> SetAxisYMaxAutoScaleOn( FALSE, FALSE ) ;
    m_pGraph -> SetAxisY2MinAutoScaleOn( FALSE, FALSE ) ;
    m_pGraph -> SetAxisY2MaxAutoScaleOn( FALSE, FALSE ) ;
  }
  else
  {
    m_pGraph -> SetAxisYMinAutoScaleOn( TRUE, FALSE ) ;
    m_pGraph -> SetAxisYMaxAutoScaleOn( TRUE, FALSE ) ;
    m_pGraph -> SetAxisY2MinAutoScaleOn( TRUE, FALSE ) ;
    m_pGraph -> SetAxisY2MaxAutoScaleOn( TRUE, FALSE ) ;
  }
  m_pGraph -> SetAxisXMinAutoScaleOn( FALSE, FALSE ) ;
  m_pGraph -> SetAxisXMaxAutoScaleOn( FALSE, FALSE ) ;
  m_pGraph -> SetAxisXUnit( &CString(_T("time [s]")), FALSE ) ;
  m_pGraph -> SetAxisYUnit(  &CString(_T("[mrad]")), FALSE ) ;

  int position = 13 ;
  switch (m_calibrationType)
  {
  case DayToDayScaleCalibration:
    m_pGraph -> SetAxisY2Enabled( TRUE ) ;
    m_pGraph -> SetAxisY2Min( m_pitchOffset == 0 ? m_rollOffset - 5 : m_pitchOffset - 5, FALSE ) ;
    m_pGraph -> SetAxisY2Max( m_pitchOffset == 0 ? m_rollOffset + 5 : m_pitchOffset + 5, FALSE ) ;
    m_pGraph -> SetAxisY2Unit(  &CString(_T("[mrad]")), FALSE ) ;
    AddText( m_pGraph, CString( _T("Daily Azimuth Error Test") ), position ) ;
    break ;

  case ScaleCalibration:
    m_pGraph -> SetAxisY2Enabled( TRUE ) ;
    m_pGraph -> SetAxisY2Min( m_pitchOffset == 0 ? m_rollOffset - 5 : m_pitchOffset - 5, FALSE ) ;
    m_pGraph -> SetAxisY2Max( m_pitchOffset == 0 ? m_rollOffset + 5 : m_pitchOffset + 5, FALSE ) ;
    m_pGraph -> SetAxisY2Unit(  &CString(_T("[mrad]")), FALSE ) ;
    AddText( m_pGraph, CString( _T("Sensor Scale Factor and") ), position ) ;
    AddText( m_pGraph, CString( _T("Azimuth Calibration") ), position ) ;
    break ;

  case OffsetCalibration:
    m_pGraph -> SetAxisY2Enabled( FALSE ) ;
    AddText( m_pGraph, CString( _T("Sensor Offset Calibration") ), position ) ;
    break ;

  case AdapterCalibration:
    m_pGraph -> SetAxisY2Enabled( FALSE ) ;
    AddText( m_pGraph, CString( _T("Gun Adapter Calibration") ), position ) ;
    break ;
  }
  AddText( m_pGraph, CString(_T("")), position ) ;
  AddText( m_pGraph, COleDateTime::GetCurrentTime().Format( _T("%x %X") ), position ) ;
  AddText( m_pGraph, CString( _T("Operator: ") ), SystemSetup::GetOperatorName(), position ) ;
  AddText( m_pGraph, CString( _T("Place: ") ), SystemSetup::GetPlace(), position ) ;
  CString text ;
  text.Format( _T("%5.2f"), SystemSetup::GetLatitude() ) ;
  AddText( m_pGraph, CString( _T("Latitude: ") ), text, position ) ;
  AddText( m_pGraph, CString( _T("DAU S/N: ") ), DAU::GetDAU().GetSerialNumber(), position ) ;
  text.Format( _T("Time constant: %3.1f s"), 1.0 ) ;
  AddText( m_pGraph, text, position ) ;
  for (int i = 0 ; i < (int)(2 * m_sensorData.size()) ; i++)
  {
    CString label ;
    //label += _T("S/N ") ;
    //label += m_sensorData [i/2].m_pSource -> GetSerialNumber() ;
    //label += _T(" - ") ;
    //label += _T("S/N ") ;
    //label += m_referenceData.m_pSource -> GetSerialNumber() ;
    //label += i % 2 ? _T(" P") : _T(" R") ;
    if ( (ScaleCalibration == m_calibrationType || DayToDayScaleCalibration == m_calibrationType) && 0 == m_rollOffset )
    {
      m_pGraph -> SetLinePattern( 2 - i, oc2dLineSolid, FALSE ) ;
      m_pGraph -> SetLineColor( 2 - i, m_pGraph -> GetColor( i ), FALSE ) ;
      //m_pGraph -> SetSerieLabel( 2 - i, label ) ;
    }
    else
    {
      m_pGraph -> SetLinePattern( i + 1, oc2dLineSolid, FALSE ) ;
      m_pGraph -> SetLineColor( i + 1, m_pGraph -> GetColor( i ), FALSE ) ;
      //m_pGraph -> SetSerieLabel( i + 1, label ) ;
    }
  }
  CString footerText ;
  footerText.LoadString( IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_UP ) ;
  m_pGraph -> m_pChart -> GetFooter() -> GetText() -> PutText( footerText.AllocSysString() ) ;
  m_pGraph -> m_pChart -> PutIsBatched( FALSE ) ;

  m_pGraph -> UpdateGraph() ;
  m_position = position ;
  PlotConverted( 4 ) ;
}

void CalibrationSheet::FinalizeAverageData( void )
{
  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    double debug ;
    debug = i -> m_roll [m_measurement] /= PointToSample() ;
    debug = i -> m_pitch [m_measurement] /= PointToSample() ;
    debug = i -> m_temperature [m_measurement] /= PointToSample() ;
  }
}

[
	db_command(L"")
]
class /*+++ symbol was renamed:  SensorDataDB
 :---*/ _SensorDataDBAccessor {
public:
	[ db_column(1, status=m_dwserialNumberStatus, length=m_dwserialNumberLength) ] TCHAR m_serialNumber [4] ;
	[ db_column(2, status=m_dwdauSerialNumberStatus, length=m_dwdauSerialNumberLength) ] TCHAR m_dauSerialNumber [4] ;
  [ db_column(3, status=m_dwoperatorStatus, length=m_dwoperatorLength ) ] TCHAR m_operator [50] ;
  [ db_column(4, status=m_dwtimeStatus, length=m_dwtimeLength) ] DBTIMESTAMP m_time ;
  [ db_column(5, status=m_dwtemperatureStatus, length=m_dwtemperatureLength) ] double m_temperature ;
  [ db_column(6, status=m_dwvalueStatus, length=m_dwvalueLength) ] double m_value ;

	DBSTATUS m_dwserialNumberStatus ;
	DBSTATUS m_dwdauSerialNumberStatus ;
	DBSTATUS m_dwoperatorStatus ;
  DBSTATUS m_dwtimeStatus ;
  DBSTATUS m_dwtemperatureStatus ;
  DBSTATUS m_dwvalueStatus ;

	DBLENGTH m_dwserialNumberLength ;
	DBLENGTH m_dwdauSerialNumberLength ;
	DBLENGTH m_dwoperatorLength ;
  DBLENGTH m_dwtimeLength ;
  DBLENGTH m_dwtemperatureLength ;
  DBLENGTH m_dwvalueLength ;

	void GetRowsetProperties(CDBPropSet* pPropSet)
	{
		pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
	}

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 528 ".\\CalibrationSheet.cpp"

				DEFINE_COMMAND_EX(_SensorDataDBAccessor, L"")
			
#injected_line 528 ".\\CalibrationSheet.cpp"

						BEGIN_ACCESSOR_MAP(_SensorDataDBAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_dwserialNumberLength), offsetbuf(m_dwserialNumberStatus))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_dwdauSerialNumberLength), offsetbuf(m_dwdauSerialNumberStatus))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_dwoperatorLength), offsetbuf(m_dwoperatorStatus))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), offsetbuf(m_dwtimeLength), offsetbuf(m_dwtimeStatus))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), offsetbuf(m_dwtemperatureLength), offsetbuf(m_dwtemperatureStatus))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), offsetbuf(m_dwvalueLength), offsetbuf(m_dwvalueStatus))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 ".\\CalibrationSheet.cpp"

class SensorDataDB;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 ".\\CalibrationSheet.cpp"

class SensorDataDB : public CCommand<CAccessor<_SensorDataDBAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L"";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


/**
 *  Macros used to set items in a database table wrapper class.
 */
#define DB_SET_STRING(d,n,v) \
  do { \
    _tcscpy( d.m_##n, v ) ; \
    d.m_dw##n##Status = DBSTATUS_S_OK ; \
    d.m_dw##n##Length = (DBLENGTH)_tcslen(  d##.m_##n ) ; \
  } while (0) ;

#define DB_SET_BY_VALUE(d,n,v) \
  do { \
    d.m_##n = v ; \
    d.m_dw##n##Status = DBSTATUS_S_OK ; \
    d.m_dw##n##Length = sizeof(d.m_##n) ; \
  } while (0) ;

void CalibrationSheet::SaveCalibrationData( LPCWSTR pSqlCommand, CString serialNumber, double temperature, double value )
{
  SensorDataDB table ;
  HRESULT hResult = ::OpenTable( table, pSqlCommand ) ;
  DBTIMESTAMP timestamp ;
  ASSERT(COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( timestamp )) ;
  DB_SET_STRING(table, serialNumber, serialNumber) ;
  DB_SET_STRING(table, dauSerialNumber, (LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
  DB_SET_STRING(table, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
  DB_SET_BY_VALUE(table, temperature, temperature) ;
  DB_SET_BY_VALUE(table, time, timestamp) ;
  DB_SET_BY_VALUE(table, value, value) ;
  hResult = table.Insert() ;
  if (S_OK != hResult)
  {
		Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
		ASSERT(0) ;
  }
}

void CalibrationSheet::ActivateSensors( void )
{
  if (NULL != m_referenceData.m_pSource)
  {
    m_referenceData.m_pSource -> SetOverrangeDetectionActive( TRUE ) ;
  }
  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    i -> m_pSource -> SetOverrangeDetectionActive( TRUE ) ;
  }
}

BOOL CalibrationSheet::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  if (DISPLAY_INSTRUCTIONS != m_mode)
  {
    SetCursor( m_hCursor ) ;
    return TRUE ;
  }
  return __super::OnSetCursor(pWnd, nHitTest, message);
}

static BOOL IsStable( double first, double second )
{
  return 1.0 >= 1000 * abs( first - second ) ;
}

BOOL CalibrationSheet::VerifyStability( int firstMeasurement, int secondMeasurement, BOOL twist )
{
  return IsStable( m_referenceData.m_roll [firstMeasurement], twist ? m_referenceData.m_pitch [secondMeasurement] : m_referenceData.m_roll [secondMeasurement] ) &&
         IsStable( m_referenceData.m_pitch [firstMeasurement], twist ? m_referenceData.m_roll [secondMeasurement] : m_referenceData.m_pitch [secondMeasurement] ) ;
}

BOOL CalibrationSheet::ShowBarGraph( BOOL show, double axisYMin, double axisYMax )
{
  if( show == FALSE )
  {
    if( m_pBarGraph == NULL )
    {
      return( FALSE );
    }
	  if( m_pBarGraph->GetSource() != NULL )
	  {
		  m_pBarGraph->GetSource()->Stop();
      ResetAllFilters( m_pBarGraph->GetSource() );
	  }
	  m_pBarGraph->SetSource( NULL );
    m_pGraph -> InitCalibrationGraph( (int)(2 * m_sensorData.size()), 0 ) ;
    return( TRUE );
  }

  InitGlobalSensorStrings( FALSE );
  g_AlignerData.AlignMode.LoadString( IDS_GUN_ADAPTER_CALIBRATION );
  g_AlignerData.RefObjNo = GetArrayIndex( m_referenceData.m_pSource->GetName() );
  g_AlignerData.NoOfCorr = 0;
  for( vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++ )
  {
    i -> m_pSource -> ResetFilter( DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() ), THETA, m_timeConstant ) ;
    g_AlignerData.NoOfCorr++;
    g_AlignerData.ObjNo[g_AlignerData.NoOfCorr] = GetArrayIndex( i -> m_pSource -> GetName() );    
  }

  if( g_AlignerData.RefObjNo == -1 || g_AlignerData.NoOfCorr == 0 )
  {
    return( FALSE );
  }

  m_pBarGraph = static_cast<CGraphView *>(theApp.m_pGraphView) ;
  m_pBarGraph->InitDefaultBarGraph( g_AlignerData.NoOfCorr );
  CString str;
  if( GetMRad() == TRUE )
  {
    str.LoadString( IDS_MRAD_UNIT );
    m_pBarGraph->SetAxisYUnit( &str );
  }
  else
  {
    str.LoadString( IDS_ARCMIN_UNIT );
    m_pBarGraph->SetAxisYUnit( &str );
  }

  if( axisYMin == 0 && axisYMax == 0 )
  {
	  m_pBarGraph->SetAxisYMinAutoScaleOn( TRUE );
	  m_pBarGraph->SetAxisYMaxAutoScaleOn( TRUE );
  }
  else
  {
    m_pBarGraph->SetAxisYMin( axisYMin );
    m_pBarGraph->SetAxisYMax( axisYMax );
  }

  m_pBarGraph -> SetSource( &DAU::GetDAU() ) ;
  unsigned int fs = 0x11; //30.52 Hz
  ResetAllFilters( m_pBarGraph->GetSource(), fs, THETA, m_timeConstant ) ;
  m_pBarGraph->GetSource()->EnableErrorEvent( FALSE );
	m_pBarGraph -> GetSource() -> SetFrequency( fs ) ;
	m_pBarGraph -> GetSource() -> Start() ;
  SetSelectedChannels( m_pBarGraph->GetSource() );

  //Get the timestamp to be stored in the graph
  BarGraphInParam m_BarGraphParam;
  m_BarGraphParam.TimeStamp = GetTimeStr( TRUE );
  m_BarGraphParam.DateStamp = GetDateStr();
  m_pBarGraph->ShowBarGraphWithText( &m_BarGraphParam );

  theApp.SwitchView( theApp.m_pGraphView ) ;
  return( TRUE );
}

int CalibrationSheet::GetCalibrationType( void )
{
  return( m_calibrationType );
}
