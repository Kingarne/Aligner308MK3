// $Id: TheoAdapterCalibrationPage2.h,v 1.2 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class TheoAdapterCalibrationPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPage2)
	DECLARE_MESSAGE_MAP()
public:
	TheoAdapterCalibrationPage2( void ) ;
	virtual ~TheoAdapterCalibrationPage2( void ) ;
private:
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_2 };
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
