// $Id: TiltCalibrationPage4.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

struct TiltCalibrationResult
{
  double m_sensorRollOffset ;
  double m_sensorPitchOffset ;
  double m_flatRollOffset ;
  double m_flatPitchOffset ;
  double m_temperature ;
  BOOL m_saveResult ;
public:
  TiltCalibrationResult( void ) {
    m_saveResult = TRUE ;
  }
} ;

class TiltCalibrationPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(TiltCalibrationPage4)
public:
	TiltCalibrationPage4( void ) ;
	virtual ~TiltCalibrationPage4( void ) ;
public:
	enum { IDD = IDD_TILT_CALIBRATION_PAGE_4 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  virtual BOOL OnWizardFinish( void ) ;
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long *pApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
  void OnInitGrid( void ) ;
	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
  TiltCalibrationResult m_referenceResult ;
  vector<TiltCalibrationResult> m_sensorResult ;
private:
  CWnd m_grid ;
//  TrueDBGrid70::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : commented
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : replacing the above
} ;
