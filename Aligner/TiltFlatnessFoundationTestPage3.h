// $Id: TiltFlatnessFoundationTestPage3.h,v 1.2 2013/10/01 13:25:11 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CTiltFlatnessTestWizard ;

class CTiltFlatnessFoundationTestPage3 : public CPropertyPage
{
    friend class CTiltFlatnessTestWizard;
    friend class CTiltFlatnessTestPage1;
    friend class CTiltFlatnessTestPage2;

    DECLARE_DYNAMIC(CTiltFlatnessFoundationTestPage3)

private:
    CTiltFlatnessTestWizard* m_pParent;
    CString m_MsgCaption;
    CString m_Text;
    double m_AzimuthAngle;
    double m_ArcAngle;
	double m_ArcAngleReal;
	double m_AzimuthAngleReal;

    CString m_TimeStamp;
    CString m_DateStamp;
	BOOL m_Repeat;
    BOOL m_measureWarping;
    int m_measureRound;
    int m_measurmentNum;

public:
	CTiltFlatnessFoundationTestPage3();
	virtual ~CTiltFlatnessFoundationTestPage3();

// Dialog Data
	enum { IDD = IDD_TILT_FLATNESS_FOUNDATION_TEST_PAGE_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    BOOL OnInitDialog();
    BOOL OnSetActive();
    BOOL OnQueryCancel();
    LRESULT OnWizardBack();
    BOOL OnWizardFinish();
    void OnReset();
    void MeasureRollPathInit();
    void MeasureRollPathInitRound2();
    void MeasureRollPathContinue();
    void RestoreGlobals();
    //void EnableStartButton( int textID );
    void EnableStartButton();
    BOOL CallMeasure( double *pRoll, double *pPitch );
    void HideAllText();
    void HideAll();
    void ShowGraphButtons();
    void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );
	BOOL IsAlreadyMeasured( double newAngle );
    CString GetFinishButtonText();
    void StartMeasureRound1();
    void StartMeasureRound2();
    void SetWindowProp(int ctrl, BOOL show, CString text="");
    void SetWindowProp(int ctrl, BOOL show, int strId=-1);

	DECLARE_MESSAGE_MAP()
private:
    afx_msg void OnBnClickedStartMeasure();
    afx_msg void OnEnKillfocusTiltFlatnessFoundationTestPage3AzimuthAngle();
    afx_msg void OnBnClickedShowPolarGraph();
    afx_msg void OnBnClickedShowErrorGraph();
    afx_msg void OnBnClickedShowResultGraph();
    afx_msg void OnBnClickedShowResultTable();
    afx_msg void OnBnClickedPrintGraph();
    afx_msg void OnBnClickedSaveGraph();
public:
	afx_msg void OnEnChangeTiltFlatnessFoundationTestPage3ArcAngleTedit();
    afx_msg void OnBnClickedFinishMeasure();
	afx_msg void OnEnChangeTiltFlatnessFoundationTestPage3AzimuthAngleTedit();
	afx_msg void OnEnChangeTiltFlatnessFoundationTestPage3ArcAngleRealTedit();	
};
