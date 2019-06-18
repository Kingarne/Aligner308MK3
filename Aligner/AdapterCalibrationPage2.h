// $Id: AdapterCalibrationPage2.h,v 1.2 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class AdapterCalibrationPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage2)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage2( void ) ;
	virtual ~AdapterCalibrationPage2( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_2 };
  void ShowGUI( int cmdShow );
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnInitDialog( void ) ;
	virtual BOOL OnSetActive( void ) ;
	virtual BOOL OnQueryCancel( void ) ;
	virtual LRESULT OnWizardNext( void ) ;
	virtual LRESULT OnWizardBack( void ) ;

	bool inProgress;
} ;
