// $Id: SetupAxesDlgA202.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

// m_XUnit
#define X_UNIT_S      0
#define X_UNIT_MIN    1
#define X_UNIT_HRS    2

#define DEFAULT_RANGE_X_S       200 // g_AlignerData.RX always in [s]
#define MIN_RANGE_X_S           10
#define MAX_RANGE_X_S           172800
#define MIN_RANGE_X_MIN         ( SECONDS_TO_MINUTES( MIN_RANGE_X_S ) )
#define MAX_RANGE_X_MIN         ( SECONDS_TO_MINUTES( MAX_RANGE_X_S ) )
#define MIN_RANGE_X_HOURS       ( SECONDS_TO_HOURS( MIN_RANGE_X_S ) )
#define MAX_RANGE_X_HOURS       ( SECONDS_TO_HOURS( MAX_RANGE_X_S ) )

#define DEFAULT_MAX_Y_DEG      360
#define MIN_MAX_Y_DEG          -400
#define MAX_MAX_Y_DEG          400
#define DEFAULT_MIN_Y_DEG       0
#define MIN_MIN_Y_DEG           -400
#define MAX_MIN_Y_DEG           400


// CSetupAxesDlgA202 dialog

class CSetupAxesDlgA202 : public CDialog
{
	DECLARE_DYNCREATE(CSetupAxesDlgA202)

public:
	CSetupAxesDlgA202(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupAxesDlgA202();

// Dialog Data
	enum { IDD = IDD_SETUP_AXES_A202 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
public:
	static void GetSettingsForAxes( CString* pXAxisSettings, CString* pY1AxisSettings, CString* pY2AxisSettings );
  static void InitiateLiveGraphAxes( void );
	static double GetMaxY1Deg( void );
	static double GetMinY1Deg( void );
	static double GetMaxY2Deg( void );
	static double GetMinY2Deg( void );
	static double GetRangeXS( void );

	DECLARE_MESSAGE_MAP()
protected:
  static BOOL m_AxesInitiated;
  static double m_MaxY1Deg;
  static double m_MinY1Deg;
  static double m_MaxY2Deg;
  static double m_MinY2Deg;
  static double m_RangeX;
  int m_XUnit;

  double m_MaxY1DegDlg;
  double m_MinY1DegDlg;
  double m_MaxY2DegDlg;
  double m_MinY2DegDlg;
  double m_RangeXDlg;

  afx_msg void OnEnKillfocusSetupAxesXRange();
  afx_msg void OnBnClickedSetupAxesXSUnit();
  afx_msg void OnBnClickedSetupAxesXMinUnit();
  afx_msg void OnBnClickedSetupAxesXHrsUnit();
  afx_msg void OnEnKillfocusSetupAxesY1Max();
  afx_msg void OnEnKillfocusSetupAxesY1Min();
  afx_msg void OnEnKillfocusSetupAxesY2Max();
  afx_msg void OnEnKillfocusSetupAxesY2Min();
  afx_msg void OnBnClickedOk();
};

