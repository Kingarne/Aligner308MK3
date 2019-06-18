// $Id: DAUCalibrationPage6.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class DAUCalibrationPage6 : public CPropertyPage
{
  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
	DECLARE_DYNAMIC(DAUCalibrationPage6)
public:
	DAUCalibrationPage6( void ) ;
	virtual ~DAUCalibrationPage6( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_6 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
protected:
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long *pApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
  void StoreToDisk( void ) ;
private:
  CWnd m_grid ;
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;
} ;

