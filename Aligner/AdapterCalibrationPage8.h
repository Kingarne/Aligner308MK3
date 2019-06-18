// $Id: AdapterCalibrationPage8.h,v 1.2 2014-07-11 09:28:24 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class AdapterCalibrationPage8 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage8)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage8( void ) ;
	virtual ~AdapterCalibrationPage8( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_8 };
  void ShowGUI( int cmdShow );
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnInitDialog( void ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardBack( void ) ;
	virtual LRESULT OnWizardNext( void ) ;

	bool inProgress;
} ;
