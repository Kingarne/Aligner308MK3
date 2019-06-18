// $Id: AdapterCalibrationPage3.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class AdapterCalibrationPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage3)
	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
	AdapterCalibrationPage3( int measurmentToCheck ) ;
	virtual ~AdapterCalibrationPage3( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_3 };
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long* ApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
  void OnInitGrid( void ) ;
private:
  struct InclinationResult {
    double m_roll ;
    double m_pitch ;
    BOOL m_saveResult ;
  public:
    InclinationResult( void ) {
      m_saveResult = TRUE ;
    }
  } ;
  vector<InclinationResult> m_inclination ;
private:
  int m_measurmentToCheck ;
  CWnd m_grid ;
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;
} ;
