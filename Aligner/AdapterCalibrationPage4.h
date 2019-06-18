// $Id: AdapterCalibrationPage4.h,v 1.2 2014-07-11 09:28:23 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class AdapterCalibrationPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage4)
	DECLARE_MESSAGE_MAP()
public:
	AdapterCalibrationPage4( void ) ;
	virtual ~AdapterCalibrationPage4( void ) ;
private:
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_4 };
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
