// $Id: GainCalibrationPage6.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

struct Gain6CalibrationResult
{
  double m_sensorRollGain ;
  double m_sensorPitchGain ;
  double m_sensorRollAzimuth ;
  double m_sensorPitchAzimuth ;
  double m_pitchTemperature ;
  double m_rollTemperature ;
  BOOL m_saveResult ;
public:
  Gain6CalibrationResult( void ) {
    m_saveResult = TRUE ;
  }
} ;

class GainCalibrationPage6 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage6)
	DECLARE_MESSAGE_MAP()
 
public:
	GainCalibrationPage6( void ) ;
	virtual ~GainCalibrationPage6( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_6} ;
protected:
  virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  virtual BOOL OnWizardFinish( void ) ;
  //void SetNewLocation( VARIANT *pNewLocation, int location ) ;  
  void OnInitGrid( void ) ;
  void SaveDayToDayCalibrationData( Gain6CalibrationResult &result, CString serialNumber );
public:
  Gain6CalibrationResult m_sensor ;
private:
  BOOL m_backOk ;
  BOOL m_calibUpdated;
  CResultTable* m_pResultTable;
  double m_Temperature[SIZE_OF_ARRAYS];
  void InitResultTable(void);

public:
	
} ;
