// $Id: AlignVerHorizonPage2.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "GlobalData.h"

class CAlignmentWizard ;

class CAlignVerHorizonPage2 : public CPropertyPage
{
  friend class CAlignVerHorizonPage1;

	DECLARE_DYNAMIC(CAlignVerHorizonPage2)

private:
  CAlignmentWizard *m_pParent;
  CString m_MsgCaption;
  CString m_Text;
  double m_Bear;
  double m_Range;
  int m_YRan;
  int m_YBias;
  CString m_TimeStamp;
  CString m_DateStamp;
	RBTypeT m_RBType;
	BOOL m_Repeat;
	BOOL m_UseParallaxValueToBeRestoredOnCancel;

public:
	CAlignVerHorizonPage2();
	virtual ~CAlignVerHorizonPage2();

// Dialog Data
	enum { IDD = IDD_ALIGN_VER_HORIZON_PAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  BOOL OnSetActive();
  BOOL OnQueryCancel();
  LRESULT OnWizardBack();
  BOOL OnWizardFinish();
  void OnReset();
  BOOL HorizonTestInit();
  void HorizonTestLoopInit();
  void HorizonTestContinue();
  void RestoreGlobals();
  //void EnableStartButton( int textID );
  void EnableStartButton();
  void InitMeasureParam();
  BOOL CallMeasure( double *pRoll, double *pPitch );
  void HideAllText();
  void HideAll();
  void RangeBearing( RBTypeT RBType );
  void StoreGraph();
  void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );
  void EnableBearingAndRangeBox();
  void DisableBearingAndRangeBox();
	BOOL IsAlreadyMeasured( double newAngle );

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedStartMeasure();
  afx_msg void OnEnKillfocusAlignVerBear();
  afx_msg void OnEnKillfocusAlignVerRange();
  void ShowPolarGraph();
  void ShowErrorGraph();
  void ShowResultGraph();
 public:
	afx_msg void OnBnClickedFinishMeasure();
};
