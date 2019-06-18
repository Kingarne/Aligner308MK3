// $Id: TiltAlignmentPage2.h,v 1.2 2011/10/13 10:09:12 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

//defines for m_NextBedPlaneFunction
#define NO_FUNCTION_DEFINED      0
#define BED_PLANE_0_MEASURE_1    1
#define BED_PLANE_1_MEASURE_1    2
#define BED_PLANE_1_MEASURE_2    3
#define BED_PLANE_2_MEASURE_1    4
#define BED_PLANE_2_MEASURE_2    5
#define BED_PLANE_2_MEASURE_3    6
#define FUNCTIONS_READY          7
#define FUNCTION_INTERRUPTED     8

// CTiltAlignmentPage2 dialog

class CTiltAlignmentPage2 : public CPropertyPage
{
    friend class CTiltAlignmentWizard;
    friend class CTiltAlignmentPage1;
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CTiltAlignmentPage2)

private:
    CTiltAlignmentWizard* m_pParent;
    CString m_MsgCaption;
    CString m_Text;
    int m_NextBedPlaneFunction;
	BOOL m_CommonFlatTestTimerOn;
	UINT_PTR m_CommonFlatTestTimerEvent;
	
    //keep the variable names from the old pascal code
    double m_FiBed;
    CString m_BedPlaneDir;
    CString m_BedPlaneNegDir;
    double m_dA0R[SIZE_OF_ARRAYS];
    double m_dA0P[SIZE_OF_ARRAYS];
    double m_Arl[SIZE_OF_ARRAYS];
    double m_Apl[SIZE_OF_ARRAYS];
    double m_Drl[SIZE_OF_ARRAYS];
    double m_Dpl[SIZE_OF_ARRAYS];
	double m_Temperature[SIZE_OF_ARRAYS];

public:
	CTiltAlignmentPage2();
	virtual ~CTiltAlignmentPage2();

// Dialog Data
	enum { IDD = IDD_TILT_ALIGNMENT_PAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    BOOL OnInitDialog();
    BOOL OnSetActive();
    BOOL OnQueryCancel();
    LRESULT OnWizardBack();
    BOOL OnWizardFinish();
    void OnReset();
    void MeasureRP();
    void BedPlane0Init();
    void BedPlane0Measure1();
    void BedPlane1Init();
    void BedPlane1Measure1();
    void BedPlane1Measure2();
    void BedPlane2Init();
    void BedPlane2Measure1();
    void BedPlane2Measure2();
    void BedPlane2Measure3();
    void InitMeasureParam();
    void RestoreGlobals();
    void EnableStartButton( int textID );
    BOOL CallMeasure( double *pRoll, double *pPitch );
    void HideAll();
    void HideAllText();
    void ShowGraphButtons();
    void HideGraphButtons();
	void DisableAllButtons();
	void InitResultTable( void );
	void CheckMeasureResult();
	void StartMeasure();
	BOOL StartTimer();
	void StopTimer( BOOL measure );
    //void SetupCurrentAz();
private:
    afx_msg void OnBnClickedStartMeasure();
    afx_msg void OnBnClickedShowPolarGraph();
    afx_msg void OnBnClickedShowResultTable();
    afx_msg void OnBnClickedPrintGraph();
    afx_msg void OnBnClickedSaveGraph();
	afx_msg void OnTimer(UINT nIDEvent);
private:
    void AdjustIfAfterCommonFlatTest( void ) ;
};
