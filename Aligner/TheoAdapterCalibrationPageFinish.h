// $Id: TheoAdapterCalibrationPageFinish.h,v 1.2 2014-07-11 16:30:59 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

struct TheoAdapterCalibrationResult
{
	double m_elevation ;
	double m_azimuth ;
	BOOL m_saveResult ;
public:
	TheoAdapterCalibrationResult( void ) {
    m_saveResult = TRUE ;
  }
} ;

class TheoAdapterCalibrationPageFinish : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPageFinish)
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	TheoAdapterCalibrationPageFinish( void ) ;
	virtual ~TheoAdapterCalibrationPageFinish( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_FINISH };
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
	vector<TheoAdapterCalibrationResult> m_adapter ;
private:
	CWnd m_grid ;
	TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;
} ;
