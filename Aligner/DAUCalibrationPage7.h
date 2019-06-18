// $Id: DAUCalibrationPage7.h,v 1.2 2011/10/14 15:56:01 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class DAUCalibrationPage7 : public CPropertyPage
{
  DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
	DECLARE_DYNAMIC(DAUCalibrationPage7)
public:
	DAUCalibrationPage7( void ) ;
	virtual ~DAUCalibrationPage7( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_7 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnSetActive( void ) ;
  virtual BOOL OnQueryCancel( void ) ;
  virtual LRESULT OnWizardBack( void ) ;
  virtual BOOL OnWizardFinish( void ) ;
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
  std::vector<DAUScaleCalibrationResult> m_result ;
} ;

