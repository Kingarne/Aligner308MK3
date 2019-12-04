// $Id: AlignmentWizard.h,v 1.5 2013-10-15 13:00:09 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "DataRecepient.h"
#include "Measure.h"
#include "ResultTable.h"
#include "UserMessages.h"

// m_Status
#define STATUS_PAGE_NOT_READY          0
#define STATUS_PAGE_ERROR              1
#define STATUS_PAGE_READY              2
#define STATUS_PAGE_CANCELED           3
#define STATUS_GET_DAU_TEMP_DATA       4

#include "DataRecepient.h"
#include "ResultTable.h"

class CAlignmentWizard : public CPropertySheet, public DataRecepient
{
	friend class CTiltVerificationWizard;
    friend class CAlignVerHorizonPage1;
    friend class CAlignVerHorizonPage2;
	friend class CAzimuthVerificationWizard;
    friend class CAlignVerBenchMarkPage1;
    friend class CAlignVerBenchMarkPage2;
    friend class CAzimuthAlignWizard;
    friend class CAzimuthAlignPage1;
    friend class CTiltFlatnessTestWizard;
    friend class CTiltFlatnessTestPage1;
    friend class CTiltFlatnessTestPage2;
    friend class CTiltFlatnessTestPage3;
    friend class CTiltFlatnessFoundationTestPage1;
    friend class CTiltFlatnessFoundationTestPage2;
    friend class CTiltFlatnessFoundationTestPage3;
    friend class CTiltAlignmentWizard;
    friend class CTiltAlignmentPage1;
    friend class CTiltAlignmentPage2;
    friend class CGyroPerformanceTestWizard;
    friend class CGyroPerformanceTestPage1;
    friend class CGyroPerformanceTestPage2;
    friend class CGunAdapterTiltCheckWizard;
    friend class CGunAdapterTiltCheckPage1;
    friend class CSetupFoundationMeasureDlg ;

	DECLARE_DYNAMIC(CAlignmentWizard)

public:
    CAlignmentWizard(  UINT nIDCaption, CWnd *pParentWnd = NULL );
	virtual ~CAlignmentWizard();
	void ExitResultTable();
//	BOOL AddGraphFileNameToDataBase( void );
	void SavePolarGraphFile( void );
	void SaveErrorGraphFile( void );
	void SaveResultGraphFile( void );
	BOOL GetSperryActive( void ) const;
	void SetSperryActive( BOOL sperryActive );

	CResultTable* m_pResultTable;
    CGraphView *m_pGraph;

private:
    BOOL OnInitDialog();
    afx_msg LRESULT OnNewData( WPARAM, LPARAM );
    afx_msg LRESULT OnDauError( WPARAM, LPARAM );
    afx_msg void OnDestroy();

    int m_Status;
    //DAU *m_pSource;
    CMeasure m_Measure;
  
	GraphFileManager m_GraphFileManager;
	//BOOL m_deleteReport;
	BOOL m_SperryActive;

//	double m_StraightEdgeAngle ;


public:
    AziModeT m_AziMode;
    BOOL m_EnableReference;
    BOOL m_EnableFixPlane;
    BOOL m_EnableMoveOfWizard;
	BOOL m_MeasurementInterrupted;
	CString m_PolarGraphFileName;
	CString m_ErrorGraphFileName;
	CString m_ResultGraphFileName;
	DBTIMESTAMP m_MeasurementReadyTimeStamp;

    //keep the variable names from the old pascal code
    BOOL m_UseParallaxComp;
    int m_N;
    double m_Y[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE]; // index [0][0] not used
   // double m_H[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE]; // index [0][0] not used
    double m_HBottom;
    double m_Fi[SIZE_OF_ARRAYS];
    double m_F;
    double m_Ymax;
    double m_Ymin;    
    BOOL m_Leave;
    double m_XAngle[SIZE_OF_X_ARRAY]; // index [0] not used
    double m_SineFitError[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE]; // index [0][0] not used
    double m_MaxSineFitError[SIZE_OF_ARRAYS];
    double m_XAngleForMaxSineFitError[SIZE_OF_ARRAYS];
    double m_MeanSineFit[SIZE_OF_ARRAYS];
    double m_Eave[SIZE_OF_ARRAYS];
    double m_Esquare[SIZE_OF_ARRAYS];
    double m_ERan;

    BOOL m_Platf;
    double m_Xn;
    double m_dR[SIZE_OF_ARRAYS];
    double m_dP[SIZE_OF_ARRAYS];


protected:
	DECLARE_MESSAGE_MAP()
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
};

inline
BOOL CAlignmentWizard::GetSperryActive( void ) const
{
	return( m_SperryActive );
}

inline
void CAlignmentWizard::SetSperryActive( BOOL sperryActive )
{
	m_SperryActive = sperryActive;
}
