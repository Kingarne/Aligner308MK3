// $Id: CalibrationSheet.h,v 1.4 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002-2003

#pragma once

#define MAX_NB_CALIBRATION_MEASUREMENTS 4

#include "DataRecepient.h"
#include "GraphView.h"

struct CalibrationData {
  Sensor *m_pSource ;
  int m_sensorIndex;
  double m_roll [MAX_NB_CALIBRATION_MEASUREMENTS]  ;
  double m_pitch [MAX_NB_CALIBRATION_MEASUREMENTS] ;
  double m_temperature [MAX_NB_CALIBRATION_MEASUREMENTS] ;
public:
  CalibrationData( Sensor *pSource = NULL, int index = -1) {
    m_pSource = pSource ;
	m_sensorIndex = index;
  }
} ;

class CalibrationSheet : public CPropertySheet, public DataRecepient
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CalibrationSheet)
public:
  enum CalibrationType { OffsetCalibration, ScaleCalibration, AdapterCalibration, TheoAdapterCalibration, DAUOffsetCalibration, DAUScaleCalibration, DayToDayScaleCalibration } ;
protected:
	CalibrationSheet( UINT nIDCaption, CalibrationType calibrationType ) ;
public:
	virtual ~CalibrationSheet( void ) ;
public:
	void StartMeasurement( BOOL showAsSum = FALSE, double pitchOffset = 0.0, double rollOffset = 0.0, BOOL fixYAxis = FALSE ) ;
	void StartMeasurement( double voltage ) ;
	void InitVoltage( double voltage ) ;
	void StopMeasurement( void ) ;
	void SetTimeConstant( double timeConstant ) ;
	BOOL IsMeasuring( void ) const ;
	BOOL GetIsDaily( void ) const ;
	void ActivateSensors( void ) ;
	BOOL VerifyStability( int firstMeasurement, int secondMeasurement, BOOL twist = FALSE ) ;
	void PlotConverted( int mode = 0 ) ;
	BOOL ShowBarGraph( BOOL show, double axisYMin = 0, double axisYMax = 0, BOOL useRoll=TRUE );
	int GetCalibrationType( void );
	CString GetCalibrationName(CalibrationType type);
public:
  static void SaveCalibrationData( LPCWSTR pSqlCommand, CString serialNumber, double temperature, double value ) ;
public:

  CalibrationData m_referenceData ;
  vector<CalibrationData> m_sensorData ;
  double m_alpha ;
  CString m_tableSerialNumber ;
  int m_measurement ;
  double m_voltage_ref ;
  double m_voltage_0 ;
  double m_voltage_1 ;
  bool m_backPressed;
protected:
  virtual BOOL OnInitDialog( void ) ;
private:
  afx_msg LRESULT OnNewData( WPARAM, LPARAM ) ;
  afx_msg LRESULT OnDauError( WPARAM, LPARAM ) ;
  afx_msg void OnDestroy( void ) ;
private:
  void InitGraph( int fixAxis ) ;
  void ResetFilter( BOOL useFilter = TRUE ) ;
  void UpdateGraph( BOOL useCalibrated ) ;
  void UpdateAverageData( void ) ;
  void FinalizeMeasurmentData( BOOL useCalibrated ) ;
  void FinalizeAverageData( void ) ;
  int PointToSample( void ) ;
protected:
  DAU *m_pSource ;
  int m_mode ;
  int m_sample ;
  BOOL m_showAsSum ;
  double m_pitchOffset ;
  double m_rollOffset ;
  double m_timeConstant ;
  double m_voltage ;
private:
  CGraphView *m_pGraph ;
  CGraphView *m_pBarGraph;
  CalibrationType m_calibrationType ;
  HCURSOR m_hCursor ;
  int m_position ;
public:
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
} ;

inline
void CalibrationSheet::SetTimeConstant( double timeConstant )
{
  m_timeConstant = timeConstant ;
}

inline
int CalibrationSheet::PointToSample( void )
{
  switch (m_calibrationType)
  {
    case DAUOffsetCalibration:
    case DAUScaleCalibration:
      return 240 ;
  }
  return static_cast<int>(31 * 15 * m_timeConstant) ;
}
