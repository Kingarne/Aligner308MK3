// $Id: DAUCalibrationPage4.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class DAUCalibrationPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(DAUCalibrationPage4)
  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
	DAUCalibrationPage4( void ) ;
	virtual ~DAUCalibrationPage4( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_4 } ;
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
private:
  CWnd m_grid ;
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;
} ;

