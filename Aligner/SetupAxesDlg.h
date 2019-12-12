// $Id: SetupAxesDlg.h,v 1.2 2009/10/30 15:06:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

#define DEFAULT_RANGE_X_S       200 // g_AlignerData.RX always in [s]
#define MIN_RANGE_X_S           2 // FIX set back to 10.
#define MAX_RANGE_X_S           172800
#define MIN_RANGE_X_MIN         ( SECONDS_TO_MINUTES( MIN_RANGE_X_S ) )
#define MAX_RANGE_X_MIN         ( SECONDS_TO_MINUTES( MAX_RANGE_X_S ) )
#define MIN_RANGE_X_HOURS       ( SECONDS_TO_HOURS( MIN_RANGE_X_S ) )
#define MAX_RANGE_X_HOURS       ( SECONDS_TO_HOURS( MAX_RANGE_X_S ) )

#define DEFAULT_RANGE_Y_MRAD    20
#define DEFAULT_RANGE_Y_ARCMIN  RoundToInt( MRADIANS_TO_ARCMIN( DEFAULT_RANGE_Y_MRAD ) )

#define DEFAULT_MAX_Y_MRAD      10
#define DEFAULT_MIN_Y_MRAD      -10
#define DEFAULT_MAX_Y_ARCMIN    RoundToInt( MRADIANS_TO_ARCMIN( DEFAULT_MAX_Y_MRAD ) )
#define DEFAULT_MIN_Y_ARCMIN    RoundToInt( MRADIANS_TO_ARCMIN( DEFAULT_MIN_Y_MRAD ) )


#define MIN_MAX_Y_MRAD          -250
#define MAX_MAX_Y_MRAD          250
#define MIN_MAX_Y_ARCMIN        RoundToInt( ceil( MRADIANS_TO_ARCMIN( MIN_MAX_Y_MRAD ) ) )
#define MAX_MAX_Y_ARCMIN        RoundToInt( floor( MRADIANS_TO_ARCMIN( MAX_MAX_Y_MRAD ) ) )

#define MIN_MIN_Y_MRAD          -250
#define MAX_MIN_Y_MRAD          250
#define MIN_MIN_Y_ARCMIN        RoundToInt( ceil( MRADIANS_TO_ARCMIN( MIN_MIN_Y_MRAD ) ) )
#define MAX_MIN_Y_ARCMIN        RoundToInt( floor( MRADIANS_TO_ARCMIN( MAX_MIN_Y_MRAD ) ) )


// m_XUnit
#define X_UNIT_S      0
#define X_UNIT_MIN    1
#define X_UNIT_HRS    2

// m_YUnit
#define Y_UNIT_MRAD   0
#define Y_UNIT_ARCMIN 1

class CSetupAxesDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupAxesDlg)

public:
	CSetupAxesDlg( BOOL disableXAxis = FALSE, BOOL disableAutoXAxis = FALSE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupAxesDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_AXES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();
public:
    static void InitiateLiveGraphAxes( void );
	static void GetSettingsForAxes( CString* pXAxisSettings, CString* pYAxisMinSettings, CString* pYAxisMaxSettings );
	static int GetMaxY( void );
	static int GetMinY( void );
    static void SetMinMaxY( int minY, int maxY );

	DECLARE_MESSAGE_MAP()
protected:
  static BOOL m_AxesInitiated;
  static int m_MaxY;
  static int m_MinY;
  static BOOL m_DisableAutoXAxis;

  int m_MaxYDlg;
  int m_MinYDlg;

  afx_msg void OnEnKillfocusSetupAxesYMin();
  afx_msg void OnEnKillfocusSetupAxesYMax();

public:
  double m_RangeX;
  BOOL m_RangeXAuto;
  BOOL m_RangeYAuto;
  int m_XUnit;
  int m_YUnit;
	BOOL m_DisableXAxis;
  

  afx_msg void OnBnClickedSetupAxisXAuto();
  afx_msg void OnBnClickedSetupAxisXManual();
  afx_msg void OnBnClickedSetupAxisXSUnit();
  afx_msg void OnBnClickedSetupAxisXMinUnit();
  afx_msg void OnBnClickedSetupAxisXHrsUnit();
  afx_msg void OnBnClickedSetupAxisYAuto();
  afx_msg void OnBnClickedSetupAxisYManual();
  afx_msg void OnEnKillfocusSetupAxisXManualValue();
  afx_msg void OnEnKillfocusSetupAxisYManualValue();
  afx_msg void OnBnClickedOk();
  
  BOOL m_continousGraph;
};
