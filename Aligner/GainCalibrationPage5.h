// $Id: GainCalibrationPage5.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

struct GainCalibrationResult
{
  double m_sensorRollGain ;
  double m_sensorPitchGain ;
  double m_sensorRollAzimuth ;
  double m_sensorPitchAzimuth ;
  double m_pitchTemperature ;
  double m_rollTemperature ;
  BOOL m_saveResult ;
public:
  GainCalibrationResult( void ) {
    m_saveResult = TRUE ;
  }
} ;

class GainCalibrationPage5 : public CPropertyPage
{
	DECLARE_DYNAMIC(GainCalibrationPage5)
	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
	GainCalibrationPage5( void ) ;
	virtual ~GainCalibrationPage5( void ) ;
private:
	enum { IDD = IDD_GAIN_CALIBRATION_PAGE_5 } ;
protected:
  virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  virtual BOOL OnWizardFinish( void ) ;
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long* ApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
  void OnInitGrid( void ) ;
  void SaveDayToDayCalibrationData( GainCalibrationResult &result, CString serialNumber );
public:
  GainCalibrationResult m_sensor ;
private:
  CWnd m_grid ;
//  TrueDBGrid70::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : commented
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : replacing the above
  BOOL m_backOk ;
} ;
