// $Id: AlignVerHorizonPage1.h,v 1.2 2012/03/01 10:00:18 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"

class CAlignmentWizard ;

class CAlignVerHorizonPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignVerHorizonPage1)

private:
    CAlignmentWizard *m_pParent;
    CString m_MsgCaption;
    CString m_Text;
    BOOL m_ParallaxComp;
    double m_FilterTimeConstant;
    BOOL m_EnableFixPlane;
	CButton m_SperryActiveBox;

public:
	CAlignVerHorizonPage1();
	virtual ~CAlignVerHorizonPage1();

// Dialog Data
	enum { IDD = IDD_ALIGN_VER_HORIZON_PAGE_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    BOOL OnInitDialog();
    BOOL OnSetActive();
    BOOL OnQueryCancel();
    LRESULT OnWizardBack();
    LRESULT OnWizardNext();
    void OnReset();
    void ShowSetup();
	void ShowSettingsForAxes();
	void EnableDisableSperry();
    
	DECLARE_MESSAGE_MAP()
private:
    afx_msg void OnBnClickedAlignVerParallaxCompOn();
    afx_msg void OnBnClickedAlignVerParallaxCompOff();
    afx_msg void OnEnKillfocusAlignVerFilterTimeConstant();
    afx_msg void OnBnClickedSetupAxes();
public:
	afx_msg void OnBnClickedAlignVerSetupTest();
};
