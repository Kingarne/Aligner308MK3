// $Id: AdapterCalibrationPage7.h,v 1.2 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class AdapterCalibrationPage7 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage7)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage7( void ) ;
	virtual ~AdapterCalibrationPage7( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_7 };
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
