// $Id: SetupFoundationMeasureDlg.h,v 1.3 2013/09/25 11:47:40 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "Defines.h"

#define MIN_ANGLE   0
#define MAX_ANGLE   360     //[deg]
#define MIN_LENGTH  0
#define MAX_LENGTH  100000  //[mm]


class CSetupFoundationMeasureDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupFoundationMeasureDlg)

public:
	CSetupFoundationMeasureDlg( BOOL CommonFlatTest = FALSE, BOOL Found = FALSE, BOOL Flatness = FALSE, CWnd* pParent = NULL );   // standard constructor
	virtual ~CSetupFoundationMeasureDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_FOUNDATION_MEASURE };

private:
    CString m_Text, m_MsgCaption;
    CString m_ChText[SIZE_OF_ARRAYS];
    int m_testCh;
    int m_lengthArm1;
    int m_lengthArm2;
    double m_refAngle;
    BOOL m_bMeasureWarping;
    int m_SelectedReference;
    BOOL m_Flatness;
    int m_ObjNo[SIZE_OF_ARRAYS];
    int m_NoOfCorr;
    BOOL m_CommonFlatTest;  
    BOOL m_Fou;

    map<int, int> m_textMap;
    map<int, int> m_refChMap;
    map<int, int> m_rbChMap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    BOOL OnInitDialog();
    void ReferenceSelected();
    void TestEnabled();
    void SetObjectData();
    int GetNoOfToBeTested();
    void CommonFlatTestCheck( int channelNo );
    void InitMaps();
	bool CheckSensorsConnected(int index);
	bool CheckAllSensorsConnected();

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedCh1();
    afx_msg void OnBnClickedCh2();
    afx_msg void OnBnClickedCh3();
    afx_msg void OnBnClickedCh4();
    afx_msg void OnBnClickedCh5();
    afx_msg void OnBnClickedCh6();
    afx_msg void OnBnClickedCh7();
    afx_msg void OnBnClickedCh8();
    afx_msg void OnBnClickedCh9();
    afx_msg void OnBnClickedCh10();
    afx_msg void OnBnClickedCh11();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();
    afx_msg void OnEnKillfocus();
public:
	afx_msg void OnStnClickedText1();
	void OnBnClickedRbTestCh(int ChNo) ;
	afx_msg void OnBnClickedRbTestCh1();
	afx_msg void OnBnClickedRbTestCh2();
	afx_msg void OnBnClickedRbTestCh3();
	afx_msg void OnBnClickedRbTestCh4();
	afx_msg void OnBnClickedRbTestCh5();
	afx_msg void OnBnClickedRbTestCh6();
	afx_msg void OnBnClickedRbTestCh7();
	afx_msg void OnBnClickedRbTestCh8();
	afx_msg void OnBnClickedRbTestCh9();
	afx_msg void OnBnClickedRbTestCh10();
	afx_msg void OnBnClickedRbTestCh11();

    afx_msg void OnBnClickedMeasureWarping();
    CComboBox m_typeCombo;
    afx_msg void OnCbnSelchangeFoundTypeCombo();
    int m_PFDimW;
    int m_PFDimL;
};
