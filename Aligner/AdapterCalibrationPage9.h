// $Id: AdapterCalibrationPage9.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

struct AdapterCalibrationResult
{
  double m_elevation ;
  double m_azimuth ;
  BOOL m_saveResult ;
public:
  AdapterCalibrationResult( void ) {
    m_saveResult = TRUE ;
  }
} ;

class AdapterCalibrationPage9 : public CPropertyPage
{
  DECLARE_DYNAMIC(AdapterCalibrationPage9)
  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
  AdapterCalibrationPage9( void ) ;
  virtual ~AdapterCalibrationPage9( void ) ;
private:
  enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_9 };
protected:
  virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnWizardFinish( void ) ;
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long* ApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
  void OnInitGrid( void ) ;
public:
  vector<AdapterCalibrationResult> m_adapter ;
private:
  CWnd m_grid ;
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;
} ;
