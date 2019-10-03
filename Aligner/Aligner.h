// $Id: Aligner.h,v 1.6 2012/06/07 10:06:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

// #ifndef __AFXWIN_H__
// 	#error include 'stdafx.h' before including this file for PCH
// #endif


#include "ProgressWnd.h"
#include "resource.h"
#include "Singleton.h"

#define SA_AZIMUTH  1
#define SA_GUN		2

typedef enum { UndefinedMode, 
	TiltAlignmentMode,
	AzimuthAlignMode, 
	AzimuthVerBenchmarkMode, 
	AzimuthVerGyroStabMode, 
	TiltVerHorizonMode, 
	TiltVerAirTargetMode, 
	TiltFlatnesstestMode, 
	TiltFlatnesstestPlatformMode, 
	TiltFlatnesstestFoundationMode, 
	GyroPerformanceTestMode, 
	GunAdapterTiltCheckMode, 
	LiveDataGraphMode, 
	LiveDataMode, 
	CommonFlatTestMode,
	SensorValidationTest
} ALIGNMENT_MODE;

class CAlignerApp : public CWinApp
{
  DECLARE_MESSAGE_MAP()
  friend class CTiltFlatnessTestWizard ;
  friend class CAlignVerWizard ;
  friend class CAzimuthAlignWizard ;
  friend class CAlignmentWizard ;
  friend class CGunAdapterTiltCheckDlg ;
public:
  CAlignerApp( void ) ;
  virtual ~CAlignerApp( void ) ;
public:
  virtual BOOL InitInstance( void ) ;
  virtual int ExitInstance( void ) ;
public:
	void VersionDbMerge( void ) ;
  BOOL VerifyDatabase( void ) ;
public:
    int GetMode( void ) const ;
    BOOL GetCanCalibrate( void ) const ;
    BOOL GetCanAlign( void ) const ;
    BOOL GetCanAlignAzimuth( void ) const ;
    ALIGNMENT_MODE GetAlignmentMode( void ) const ;
    void SetAlignmentMode( ALIGNMENT_MODE alignmentMode ) ;
    BOOL IsAligner202Enabled( void ) const ;
    BOOL IsAligner308Enabled( void ) const ;
    BOOL IsHelpEnabled( void ) const ;
    void SetHelpEnabled( BOOL helpEnabled ) ;
    BOOL IsRangeZeroToRangeA202( void ) const ;
    void SetRangeZeroToRangeA202( BOOL rangeZeroToRangeA202 ) ;
    BOOL IsLiveGraphViewActivated( void ) const ;
    void SetLiveGraphViewActivated( BOOL liveGraphViewActivated ) ;
    BOOL IsLiveDataViewActivated( void ) const ;
    void SetLiveDataViewActivated( BOOL liveDataViewActivated ) ;
    BOOL IsTiltAndFlatnessFoundationTestEnabled( void ) const ;
    void SetTiltAndFlatnessFoundationTestEnabled( BOOL tiltAndFlatnessFoundationTestEnabled ) ;
    BOOL ReInit( void );
    void RenameDocument( CString newName );
    BOOL CheckExpired();

    BOOL m_DAUPresent;

	map<int, bool> m_showAgainMap;

private:	
	BOOL m_HelpEnabled;
    ALIGNMENT_MODE m_AlignmentMode;
    BOOL m_Aligner202Enabled;
    BOOL m_RangeZeroToRangeA202;
    BOOL m_LiveGraphViewActivated;
    BOOL m_LiveDataViewActivated;
    BOOL m_TiltAndFlatnessFoundationTestEnabled;
public:
    //void AddToDocument( LONG id ) ;
   // void RemoveFromDocument( LONG id ) ;
public:
    CPropertySheet *m_pAzimuthAlignWizard ;
    CPropertySheet *m_pAzimuthVerificationWizard ;
    CView *m_pAlignerView ;
    CView *m_pLiveDataView ;
    CView *m_pGraphView ;
    CView *m_pSystemConfigurationView ;
    CView *SwitchView( CView *pNewView ) ;
    afx_msg void OnUtilitiesDataconversion();
    afx_msg void OnUpdateUtilitiesDataconversion(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentTiltalignment();
    afx_msg void OnUpdateAlignmentTiltalignment(CCmdUI *pCmdUI);
    afx_msg void OnUtilitiesTestdb();
    afx_msg void OnCalibrationSensortiltoffsetcalibration();
    afx_msg void OnAlignmentTiltFlatnessTest();
    afx_msg void OnUpdateAlignmentTiltFlatnessTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentTiltFlatnessTestPlatform();
    afx_msg void OnUpdateAlignmentTiltFlatnessTestPlatform(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentTiltFlatnessTestFoundation();
    afx_msg void OnUpdateAlignmentTiltFlatnessTestFoundation(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentAzimuthAlignment();
    afx_msg void OnUpdateAlignmentAzimuthAlignment(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentGyroPerformanceTest();
    afx_msg void OnUpdateAlignmentGyroPerformanceTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentGunAdapterTiltCheck();
    afx_msg void OnUpdateAlignmentGunAdapterTiltCheck(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentTiltVerificationHorizonTest();
    afx_msg void OnUpdateAlignmentTiltVerificationHorizonTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentTiltVerificationAirTargetTest();
    afx_msg void OnUpdateAlignmentTiltVerificationAirTargetTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentAzimuthVerificationBenchmarkTest();
    afx_msg void OnUpdateAlignmentAzimuthVerificationBenchmarkTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentAzimuthVerificationGyroStabTest();
    afx_msg void OnUpdateAlignmentAzimuthVerificationGyroStabTest(CCmdUI *pCmdUI);
    afx_msg void OnCalibrationSetup();
    afx_msg void OnUpdateCalibrationSetup(CCmdUI *pCmdUI);
    afx_msg void OnUtilitiesLivedata();
    afx_msg void OnUpdateUtilitiesLivedata(CCmdUI *pCmdUI);
    afx_msg void OnUtilitiesLivegraph();
    afx_msg void OnUpdateUtilitiesLivegraph(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentSetup();
    afx_msg void OnUpdateAlignmentSetup(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentCommonFlatTest();
    afx_msg void OnUpdateAlignmentCommonFlatTest(CCmdUI *pCmdUI);
    afx_msg void OnAlignmentDailygain();
    afx_msg void OnUpdateAlignmentDailygain(CCmdUI *pCmdUI);
    afx_msg void OnAppAbout( void ) ;
	//***  1.14
	afx_msg void OnFileOpen( void ) ;
	//***  1.14 End
private:  
  
  ProgressWnd m_progressWnd;
	Singleton m_singleton;
public:
    afx_msg void OnUtilitiesDa();
    afx_msg void OnUpdateUtilitiesDa(CCmdUI *pCmdUI);
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
} ;

extern CAlignerApp theApp ;

inline
ALIGNMENT_MODE CAlignerApp::GetAlignmentMode( void ) const {
    return m_AlignmentMode ;
}

inline
void CAlignerApp::SetAlignmentMode( ALIGNMENT_MODE alignmentMode ) {
  m_AlignmentMode = alignmentMode ;
}

inline
BOOL CAlignerApp::IsAligner308Enabled( void ) const {
# if defined(BUILD_ALIGNER_308)
    return TRUE ;
# else
# if defined(BUILD_ALIGNER_202)
    return FALSE ;
# else
#   error "Must explicitly state target!"
# endif
# endif
}

inline
BOOL CAlignerApp::IsAligner202Enabled( void ) const {
# if defined(BUILD_ALIGNER_202)
    return TRUE ;
# else
# if defined(BUILD_ALIGNER_308)
    return FALSE ;
# else
#   error "Must explicitly state target!"
# endif
# endif
}

inline
BOOL CAlignerApp::IsHelpEnabled( void ) const {
	return m_HelpEnabled;
}

inline
void CAlignerApp::SetHelpEnabled( BOOL helpEnabled ) {
	m_HelpEnabled = helpEnabled;
}

inline
BOOL CAlignerApp::IsRangeZeroToRangeA202( void ) const {
    return m_RangeZeroToRangeA202;
}

inline
void CAlignerApp::SetRangeZeroToRangeA202( BOOL rangeZeroToRangeA202 ) {
    m_RangeZeroToRangeA202 = rangeZeroToRangeA202;
}

inline
BOOL CAlignerApp::IsLiveGraphViewActivated( void ) const {
    return m_LiveGraphViewActivated;
}

inline
void CAlignerApp::SetLiveGraphViewActivated( BOOL liveGraphViewActivated ) {
    m_LiveGraphViewActivated = liveGraphViewActivated;
}

inline
BOOL CAlignerApp::IsLiveDataViewActivated( void ) const {
    return m_LiveDataViewActivated;
}

inline
void CAlignerApp::SetLiveDataViewActivated( BOOL liveDataViewActivated ) {
    m_LiveDataViewActivated = liveDataViewActivated;
}

inline
BOOL CAlignerApp::IsTiltAndFlatnessFoundationTestEnabled( void ) const {
    return m_TiltAndFlatnessFoundationTestEnabled;
}

inline
void CAlignerApp::SetTiltAndFlatnessFoundationTestEnabled( BOOL tiltAndFlatnessFoundationTestEnabled ) {
    m_TiltAndFlatnessFoundationTestEnabled = tiltAndFlatnessFoundationTestEnabled;
}


