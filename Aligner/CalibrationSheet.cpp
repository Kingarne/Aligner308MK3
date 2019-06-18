// $Id: CalibrationSheet.cpp,v 1.11 2014-07-11 16:30:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Aligner.h"
#include "CalibrationSheet.h"
#include "SystemSetup.h"
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
  DAU::GetDAU().SetOverrangeDetectionActive( FALSE ) ;
  m_voltage = 0 ;
  SetWizardMode() ;
  m_backPressed = false;
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
    m_referenceData.m_pSource->ResetFilter( DAU::GetDAU().GetPeriod(), THETA, m_timeConstant ) ;
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
      i->m_pSource->ResetFilter( DAU::GetDAU().GetPeriod(), THETA, m_timeConstant ) ;
    }
  }
  else
  {
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
      i->m_roll [m_measurement] = 0 ;
      i->m_pitch [m_measurement] = 0 ; 
      i->m_temperature [m_measurement] = 0 ;
      i->m_pSource->ResetFilter() ;
    }
  }
}

BOOL CalibrationSheet::OnInitDialog( void )
{
  //__super::OnInitDialog() ;
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
	case TheoAdapterCalibration:
	  //    DAU::GetDAU().SetFrequency( 0x11 ) ; // 30 Hz
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);
    m_pGraph = static_cast<CGraphView *>(theApp.m_pGraphView) ;
	TRACE("size:%d\n",m_sensorData.size());
	m_pGraph->InitCalibrationGraph( (int)(2 * m_sensorData.size()), 0 ) ;
  	theApp.SwitchView( theApp.m_pGraphView ) ;
    break ;

  case DAUOffsetCalibration:
  case DAUScaleCalibration:
    //DAU::GetDAU().SetFrequency( 0x10 ) ; // 60 Hz
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);  
    DAU::GetDAU().EnableErrorEvent( FALSE ) ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
  m_hCursor = ::LoadCursor( NULL, IDC_WAIT ) ;
 // DAU::GetDAU().Start() ;
	SetSource( &(DAU::GetDAU()) ) ;
  return CPropertySheet::OnInitDialog(); //TRUE ;
}

void CalibrationSheet::OnDestroy( void )
{
	SetSource( NULL ) ;
	theApp.SwitchView( static_cast<CGraphView *>(theApp.m_pSystemConfigurationView) ) ;
	DAU::GetDAU().Stop() ;
	::DestroyCursor( m_hCursor ) ;
	ClearSelectedChannels();
	__super::OnDestroy() ;
}

BOOL CalibrationSheet::IsMeasuring( void ) const
{
  return( DISPLAY_INSTRUCTIONS != m_mode );
}

void CalibrationSheet::StopMeasurement( void )
{
   DAU::GetDAU().EnableErrorEvent( FALSE ) ;
//   DAU::GetDAU().Stop() ;
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
//  ActivateSensors() ;
  m_showAsSum = showAsSum ;
  m_pitchOffset = pitchOffset ;
  m_rollOffset = rollOffset ;
  BOOL enableErrorEvent = TRUE;
  switch (m_calibrationType)
  {
	case DayToDayScaleCalibration:
	case ScaleCalibration:
	case OffsetCalibration:
	case AdapterCalibration:
	case TheoAdapterCalibration:
		InitGraph( fixYAxis ) ;
		m_mode = PERFORM_MEASUREMENT ;
		ResetFilter( TRUE ) ;
    break ;

	case DAUScaleCalibration:
	  enableErrorEvent = FALSE;
	  //No break!;
	case DAUOffsetCalibration:  
		ResetFilter( FALSE ) ;
		m_mode = PERFORM_AVERAGE_MEASURMENT ;
	break ;

	default:
		ASSERT(0) ;
		break ;
	}
	m_sample = 0 ;
	DAU::GetDAU().EnableErrorEvent( enableErrorEvent ) ;
}

LRESULT CalibrationSheet::OnNewData( WPARAM, LPARAM )
{
    static int c=0;
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
      
		    //TRACE("New data:%d\n",c++);
		if( m_calibrationType == AdapterCalibration || m_calibrationType == TheoAdapterCalibration || m_calibrationType == DayToDayScaleCalibration )
        {
            UpdateGraph( TRUE ) ;
        }
        else
        {
            UpdateGraph( FALSE ) ;
        }

        if (++m_sample >= PointToSample())
        {
            DAU::GetDAU().EnableErrorEvent( FALSE ) ;
            if( m_calibrationType == AdapterCalibration || m_calibrationType == TheoAdapterCalibration || m_calibrationType == DayToDayScaleCalibration )
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
    //DAU::GetDAU().Restart( TRUE );
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
    double xPosition = m_sample * DAU::GetDAU().GetPeriod() ;
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
        if (m_showAsSum)
        {
            m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits((m_pitchOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetRoll(useCalibrated) + m_currentValues.GetSensor(i->m_sensorIndex).GetRoll(useCalibrated)) ) ) ;
		    m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits((m_rollOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetPitch(useCalibrated) + m_currentValues.GetSensor(i->m_sensorIndex).GetPitch(useCalibrated)) ) ) ;
	    }
        else
        {
            if (0 == m_pitchOffset)
            {
			    //TRACE("1, x:%f, Ref:%f, data:%f\n",xPosition, m_referenceData.m_pSource->GetRoll( useCalibrated ), i->m_pSource->GetRoll( useCalibrated ));
    // 			for(int c=0;c<m_currentValues.GetSensorCount();c++)
    // 				TRACE("2, x:%f, y:%f\n",xPosition, m_currentValues.GetSensor(c).GetUncalibratedRoll());			
			    double pitch = m_pitchOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetRoll(useCalibrated) - m_currentValues.GetSensor(i->m_sensorIndex).GetRoll(useCalibrated);
			    double roll = m_rollOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetPitch(useCalibrated) - m_currentValues.GetSensor(i->m_sensorIndex).GetPitch(useCalibrated);	
			    static double former[10];
    		

			    //TRACE("%d, ref:%d, i:%d, x:%f, p:%f, r:%f, diff:%f\n",serie, m_referenceData.m_sensorIndex,i->m_sensorIndex, xPosition, UnitsToMilliunits(pitch), UnitsToMilliunits(roll), UnitsToMilliunits(former[i->m_sensorIndex])-UnitsToMilliunits(roll));
			    m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits(pitch)) ;
			    m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits(roll)) ;			
			    former[i->m_sensorIndex] = roll;
		    }
            else
            {
            m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits((m_pitchOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetPitch(useCalibrated) + m_currentValues.GetSensor(i->m_sensorIndex).GetRoll(useCalibrated)) ) ) ;
		    m_pGraph->SetXYData( serie++, xPosition, UnitsToMilliunits((m_rollOffset + m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetRoll(useCalibrated) - m_currentValues.GetSensor(i->m_sensorIndex).GetPitch(useCalibrated)) ) ) ;

		    }
        }
        //m_pGraph->UpdateGraphData() ;
		m_pGraph->UpdateData() ;
    }
}

void CalibrationSheet::FinalizeMeasurmentData( BOOL useCalibrated )
{
  m_referenceData.m_roll[m_measurement] = m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetRoll(useCalibrated);
  TRACE("Ref Roll:%.6f\n", m_referenceData.m_roll[m_measurement]);
  m_referenceData.m_pitch[m_measurement] = m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetPitch(useCalibrated);
  m_referenceData.m_temperature[m_measurement] = m_currentValues.GetSensor(m_referenceData.m_sensorIndex).GetTemperature();

  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    i->m_roll[m_measurement] = m_currentValues.GetSensor(i->m_sensorIndex).GetRoll(useCalibrated); ;
	TRACE("Roll:%.2f\n", i->m_roll[m_measurement]);
	i->m_pitch[m_measurement] = m_currentValues.GetSensor(i->m_sensorIndex).GetPitch(useCalibrated); ;
    i->m_temperature[m_measurement] = m_currentValues.GetSensor(i->m_sensorIndex).GetTemperature();
  }
}

static void AddText( CGraphView *pGraph, CString &text, int &position )
{
  IChartLabel_DualPtr pLabel = pGraph->m_pChart->GetChartLabels()->Add() ;
  pLabel->GetText()->PutText( _bstr_t( text ) ) ;
  pLabel->PutAnchor( oc2dAnchorEast ) ;
  pLabel->PutAttachMethod( oc2dAttachCoord ) ;
  pLabel->GetAttachCoord()->PutX( pGraph->GetControlWidth() - 200 ) ;
  pLabel->GetAttachCoord()->PutY( position ) ;
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
    text.Format(  _T("Reference %3s"), m_referenceData.m_pSource->GetSerialNumber() ) ;
    AddText( m_pGraph, text, position ) ;
  }
  else
  {
    m_position += 5 ;
    text.Format(  _T("Reference %3s %+5.1f [°C]"), m_referenceData.m_pSource->GetSerialNumber(), m_referenceData.m_temperature [m_measurement] ) ;
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
      roll =  RADIANS_TO_MRADIANS(  i->m_roll  [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(  i->m_pitch [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 1:
      roll = RADIANS_TO_MRADIANS(  i->m_pitch [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i->m_roll  [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      //roll = RADIANS_TO_MRADIANS(  i->m_pitch [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      //roll = RADIANS_TO_MRADIANS(- i->m_roll  [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 2:
      roll  = RADIANS_TO_MRADIANS(- i->m_roll  [m_measurement] - m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i->m_pitch [m_measurement] - m_referenceData.m_pitch [m_measurement] ) ;
      break ;

    case 3:
      roll  = RADIANS_TO_MRADIANS(- i->m_roll  [m_measurement] + m_referenceData.m_roll  [m_measurement] ) ;
      pitch = RADIANS_TO_MRADIANS(- i->m_pitch [m_measurement] + m_referenceData.m_pitch [m_measurement] ) ;
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
      label.Format(  _T("Roll  %3s"), i->m_pSource->GetSerialNumber() ) ;
    }
    else
    {
      label.Format(  _T("Roll  %3s %+7.3f [mrad] %+5.1f [°C]"), i->m_pSource->GetSerialNumber(), roll,  i->m_temperature [m_measurement] ) ;
    }
    m_pGraph->SetSerieLabel( serie, label ) ;
    serie = ( (ScaleCalibration == m_calibrationType || DayToDayScaleCalibration == m_calibrationType) && 0 == m_rollOffset ) ? 2 - j : j + 1 ; j++ ;
    if (4 == mode)
    {
      label.Format(  _T("Pitch %3s"), i->m_pSource->GetSerialNumber() ) ;
    }
    else
    {
      label.Format(  _T("Pitch %3s %+7.3f [mrad] %+5.1f [°C]"), i->m_pSource->GetSerialNumber(), pitch, i->m_temperature [m_measurement] ) ;
    }
    m_pGraph->SetSerieLabel( serie, label ) ;
  }
}

void CalibrationSheet::UpdateAverageData( void )
{
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {    
	    i->m_roll[m_measurement] += m_currentValues.GetSensor(i->m_sensorIndex).GetRawRoll();//i->m_pSource->GetData().m_roll ;
        i->m_pitch[m_measurement] += m_currentValues.GetSensor(i->m_sensorIndex).GetRawPitch();
        i->m_temperature[m_measurement] += m_currentValues.GetSensor(i->m_sensorIndex).GetRawTemperature();
	
	//TRACE("i:%d, Roll:%f, raw:%f, r:%f\n",i->m_sensorIndex, i->m_roll[m_measurement], m_currentValues.GetSensor(i->m_sensorIndex).GetRawRoll(), m_currentValues.GetSensor(i->m_sensorIndex).GetRoll());
    }
}

void CalibrationSheet::InitGraph( BOOL fixYAxis )
{
	TRACE("Num data:%d\n",m_sensorData.size());
	m_pGraph->SetNoOfSeries( (int)(2 * m_sensorData.size()), FALSE ) ;
    m_pGraph->m_pChart->PutIsBatched( TRUE ) ;
    m_pGraph->m_pChart->GetChartLabels()->RemoveAll() ;
    m_pGraph->SetAxisXMin( 0, FALSE ) ;
    m_pGraph->SetAxisXMax( m_timeConstant * 15, FALSE ) ;
    m_pGraph->SetAxisYMin( -5, FALSE ) ;
    m_pGraph->SetAxisYMax( +5, FALSE ) ;
    if (fixYAxis)
    {
        m_pGraph->SetAxisYMinAutoScaleOn( FALSE, FALSE ) ;
        m_pGraph->SetAxisYMaxAutoScaleOn( FALSE, FALSE ) ;
        m_pGraph->SetAxisY2MinAutoScaleOn( FALSE, FALSE ) ;
        m_pGraph->SetAxisY2MaxAutoScaleOn( FALSE, FALSE ) ;
    }
    else
    {
        m_pGraph->SetAxisYMinAutoScaleOn( TRUE, FALSE ) ;
        m_pGraph->SetAxisYMaxAutoScaleOn( TRUE, FALSE ) ;
        m_pGraph->SetAxisY2MinAutoScaleOn( TRUE, FALSE ) ;
        m_pGraph->SetAxisY2MaxAutoScaleOn( TRUE, FALSE ) ;
    }
    m_pGraph->SetAxisXMinAutoScaleOn( FALSE, FALSE ) ;
    m_pGraph->SetAxisXMaxAutoScaleOn( FALSE, FALSE ) ;
    m_pGraph->SetAxisXUnit( &CString(_T("time [s]")), FALSE ) ;
    m_pGraph->SetAxisYUnit(  &CString(_T("[mrad]")), FALSE ) ;

    int position = 13 ;
    switch (m_calibrationType)
    {
    case DayToDayScaleCalibration:
        m_pGraph->SetAxisY2Enabled( TRUE ) ;
        m_pGraph->SetAxisY2Min( m_pitchOffset == 0 ? m_rollOffset - 5 : m_pitchOffset - 5, FALSE ) ;
        m_pGraph->SetAxisY2Max( m_pitchOffset == 0 ? m_rollOffset + 5 : m_pitchOffset + 5, FALSE ) ;
        m_pGraph->SetAxisY2Unit(  &CString(_T("[mrad]")), FALSE ) ;
        AddText( m_pGraph, GetCalibrationName(m_calibrationType), position ) ;
        break ;

    case ScaleCalibration:
        m_pGraph->SetAxisY2Enabled( TRUE ) ;
        m_pGraph->SetAxisY2Min( m_pitchOffset == 0 ? m_rollOffset - 5 : m_pitchOffset - 5, FALSE ) ;
        m_pGraph->SetAxisY2Max( m_pitchOffset == 0 ? m_rollOffset + 5 : m_pitchOffset + 5, FALSE ) ;
        m_pGraph->SetAxisY2Unit(  &CString(_T("[mrad]")), FALSE ) ;
        AddText( m_pGraph, CString( _T("Sensor Scale Factor and") ), position ) ;
        AddText( m_pGraph, CString( _T("Azimuth Calibration") ), position ) ;
        break ;

    case OffsetCalibration:
        m_pGraph->SetAxisY2Enabled( FALSE ) ;
        AddText( m_pGraph, GetCalibrationName(m_calibrationType), position ) ;
        break ;

    case AdapterCalibration:
        m_pGraph->SetAxisY2Enabled( FALSE ) ;
        AddText( m_pGraph, GetCalibrationName(m_calibrationType), position ) ;
        break ;
	
	case TheoAdapterCalibration:
        m_pGraph->SetAxisY2Enabled( FALSE ) ;
        AddText( m_pGraph, GetCalibrationName(m_calibrationType), position ) ;
        break ;
    }
    AddText( m_pGraph, CString(_T("")), position ) ;
    AddText( m_pGraph, COleDateTime::GetCurrentTime().Format( _T("%x %X") ), position ) ;
    AddText( m_pGraph, CString( _T("Operator: ") ), SysSetup->GetOperatorName(), position ) ;
    AddText( m_pGraph, CString( _T("Place: ") ), SysSetup->GetPlace(), position ) ;
    CString text ;
    text.Format( _T("%5.2f"), SysSetup->GetLatitude() ) ;
    AddText( m_pGraph, CString( _T("Latitude: ") ), text, position ) ;
    AddText( m_pGraph, CString( _T("DAU S/N: ") ), DAU::GetDAU().GetSerialNumber(), position ) ;
    text.Format( _T("Time constant: %3.1f s"), m_timeConstant) ;
    AddText( m_pGraph, text, position ) ;
    for (int i = 0 ; i < (int)(2 * m_sensorData.size()) ; i++)
    {
        CString label ;
        //label += _T("S/N ") ;
        //label += m_sensorData [i/2].m_pSource->GetSerialNumber() ;
        //label += _T(" - ") ;
        //label += _T("S/N ") ;
        //label += m_referenceData.m_pSource->GetSerialNumber() ;
        //label += i % 2 ? _T(" P") : _T(" R") ;
        if ( (ScaleCalibration == m_calibrationType || DayToDayScaleCalibration == m_calibrationType) && 0 == m_rollOffset )
        {
        m_pGraph->SetLinePattern( 2 - i, oc2dLineSolid, FALSE ) ;
        m_pGraph->SetLineColor( 2 - i, m_pGraph->GetColor( i ), FALSE ) ;
        //m_pGraph->SetSerieLabel( 2 - i, label ) ;
        }
        else
        {
        m_pGraph->SetLinePattern( i + 1, oc2dLineSolid, FALSE ) ;
        m_pGraph->SetLineColor( i + 1, m_pGraph->GetColor( i ), FALSE ) ;
        //m_pGraph->SetSerieLabel( i + 1, label ) ;
        }
    }
    CString footerText ;
    footerText.LoadString( IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_UP ) ;
    m_pGraph->m_pChart->GetFooter()->GetText()->PutText( footerText.AllocSysString() ) ;
    m_pGraph->m_pChart->PutIsBatched( FALSE ) ;

    m_pGraph->UpdateGraph() ;
    m_position = position ;
    PlotConverted( 4 ) ;
}

void CalibrationSheet::FinalizeAverageData( void )
{
    for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
    {
        double debug ;
        debug = i->m_roll [m_measurement] /= PointToSample() ;
        debug = i->m_pitch [m_measurement] /= PointToSample() ;
        debug = i->m_temperature [m_measurement] /= PointToSample() ;
    }
}


void CalibrationSheet::ActivateSensors( void )
{
  if (NULL != m_referenceData.m_pSource)
  {
    m_referenceData.m_pSource->SetOverrangeDetectionActive( TRUE ) ;
  }
  for (vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++)
  {
    i->m_pSource->SetOverrangeDetectionActive( TRUE ) ;
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

CString CalibrationSheet::GetCalibrationName(CalibrationType type)
{
	CString calibName = "";
	int id = 0;
	switch (type)
    {
		case DayToDayScaleCalibration:
			id = IDS_DAILY_AZIMUTH_ERROR_TEST_SETUP_TITLE;
        break ;

    case ScaleCalibration:
       
        break ;

    case OffsetCalibration:
        id = IDS_TILT_OFFSET_CALIBRATION_TITLE;
        break ;

    case AdapterCalibration:
        id = IDS_GUN_ADAPTER_CALIBRATION;
        break ;
	
	case TheoAdapterCalibration:
        id = IDS_THEO_ADAPTER_CALIBRATION;
        break ;
    }

	if(id != 0)
		calibName.LoadString( id );

	return calibName;
}

BOOL CalibrationSheet::ShowBarGraph( BOOL show, double axisYMin, double axisYMax, BOOL useRoll )
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
        m_pGraph->InitCalibrationGraph( (int)(2 * m_sensorData.size()), 0 ) ;
        return( TRUE );
    }

    InitGlobalSensorStrings( FALSE );
    g_AlignerData.AlignMode = GetCalibrationName(m_calibrationType);
    g_AlignerData.RefObjNo = GetArrayIndex( m_referenceData.m_pSource->GetName() );
    g_AlignerData.NoOfCorr = 0;
    for( vector<CalibrationData>::iterator i = m_sensorData.begin() ; i != m_sensorData.end() ; i++ )
    {
        i->m_pSource->ResetFilter( DAU::GetDAU().GetPeriod(), THETA, m_timeConstant ) ;
        g_AlignerData.NoOfCorr++;
        g_AlignerData.ObjNo[g_AlignerData.NoOfCorr] = GetArrayIndex( i->m_pSource->GetName() );    
    }

    if( g_AlignerData.RefObjNo == -1 || g_AlignerData.NoOfCorr == 0 )
    {
        return( FALSE );
    }

    m_pBarGraph = static_cast<CGraphView *>(theApp.m_pGraphView) ;
    m_pBarGraph->InitDefaultBarGraph( g_AlignerData.NoOfCorr, useRoll );
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

    m_pBarGraph->SetSource( &DAU::GetDAU() ) ;
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);	
      
    ResetAllFilters( m_pBarGraph->GetSource(), DAU::GetDAU().GetPeriod(), THETA, m_timeConstant ) ;

    //m_pBarGraph->GetSource()->EnableErrorEvent( FALSE );    
    // m_pBarGraph->GetSource()->SetFrequency( fs ) ;
    //	m_pBarGraph->GetSource()->Start() ;
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
