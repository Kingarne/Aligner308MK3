// $Id: AlignVerBenchMarkPage2.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define FREQUENCY_FOR_LIVE_DATA		0x15
#define TAU_FOR_LIVE_DATA					4

#include "resource.h"
#include "Defines.h"
#include "GlobalData.h"

class CAlignmentWizard ;

class CAlignVerBenchMarkPage2 : public CPropertyPage
{
  friend class CAlignVerBenchMarkPage1;
  friend class CAzimuthAlignPage2;
  friend class CAlignmentWizard;

	DECLARE_DYNAMIC(CAlignVerBenchMarkPage2)

private:
  CAlignmentWizard *m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  CString m_WhatToDoText;
  //CString m_TimeStamp;
  //CString m_DateStamp;
  BOOL m_ShowLiveData;
	BOOL m_UseResultFromFirstMeasurement;

  double m_dPError[SIZE_OF_ARRAYS];
  double m_dP[SIZE_OF_ARRAYS];
  double m_dPBis[SIZE_OF_ARRAYS];
  CString m_TitleAzi;
  double m_TempL[SIZE_OF_ARRAYS];
  ModeT m_ModeA;
  double m_C0AziCorrY, m_C0AziCorrX;
  BOOL m_Er;
  double m_RBis[SIZE_OF_ARRAYS];
  double m_ACA[SIZE_OF_ARRAYS];
  double m_MAT[SIZE_OF_ARRAYS];
  int m_MeasNo;

public:
	CAlignVerBenchMarkPage2();
	virtual ~CAlignVerBenchMarkPage2();

// Dialog Data
	enum { IDD = IDD_ALIGN_VER_BENCHMARK_PAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardBack();
  BOOL OnWizardFinish();
  void OnReset();
  void RestoreGlobals();
  void InitMeasureParam();
  BOOL CallMeasure( double *pRoll, double *pPitch );
  void HideAllText();
  void HideAll();
  BOOL InitMeasureAz();
  BOOL MeasureAzFirst();
  BOOL MeasureAzSecond();
  void MeasFC( int Lineversion );
  void StoreGraph();
  void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );
	void ShowLiveData( void );

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedStartMeasure();
  afx_msg void OnBnClickedContinueMeasure();
  afx_msg LRESULT OnDauError( WPARAM, LPARAM );
};
