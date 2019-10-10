// $Id: ResultTable.h,v 1.8 2013-10-31 13:24:47 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "Measure.h"
#include "PresentDialog.h"
#include "DataRecepient.h"

class CAlignmentWizard ;

#if defined(DAU_SIMULATION) // added by arto 060508
	#define R2_NO_NULL_FIX
#endif

#ifdef R2_NO_NULL_FIX
	#define DB_ELEVATION_IS_REFERENCE						0
	#define	DB_EMPTY_STRING											_T("")  // stlo, 070125:replaces: "---"
	#define DB_EMPTY_DOUBLE											HUGE_VAL  // stlo, 070131:replaces: 0
#else
	#define DB_ELEVATION_IS_REFERENCE						10.0
	#define	DB_EMPTY_STRING											_T("")   // stlo, 070125:replaces "---"
	#define DB_EMPTY_DOUBLE											HUGE_VAL
#endif

typedef struct
{
    int Version;
    //CString Title;
	DBTIMESTAMP Time;
    BOOL AngleRange0ToPlusMinus180;
    int SignDef;
    double* pACA;
	CString Comment;
	BOOL ShowPresenetDialog;
	BOOL SavedToDataBase;
	double* pTemperature;
	double SamplingRate;
    double timeConstant;
    CString graphFileName;
    //BOOL includeGraph;
    GraphFileManager* pFileGraphManager;
}InParam;

typedef struct
{
    CString m_station ;
	CString m_channel ;
    double m_value ;
    double m_gearing ;
    double m_refVoltage ;
} LiveDataA202Param;

class CResultTable
{

public:
  
	CResultTable( CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultTable();

    BOOL DeleteLast( void );
	//BOOL CloseReport( void );
	BOOL ShowReport( BOOL CloseFirst );	
	//BOOL UpdateComment( void );
	BOOL AddGraph( CString fileName, BOOL includeToResultTable );
//	BOOL ResetMainID( void );
   // long GetMainID( void );
    
    BOOL OpenMainReport();
    BOOL OpenCalibrationReport();

	void SetReportFolder( string baseDir ) ;
  
    InParam m_InParam;
    LiveDataA202Param* m_pLiveDataA202Param;
    long m_reportMeasID;

protected:

	//BOOL CreateEmptyReport( void );
    BOOL InitiateReport( InParam* pInParam );
    BOOL SaveToDataBase( void );
	BOOL OpenReport( BOOL SaveToDB );
	BOOL IsReportOpen( void );
	//BOOL ShowPresentDialog( BOOL Show );

    
    CAlignmentWizard* m_pParent;
    //CPresentDialog* m_pPresentDlg;
	//HANDLE m_hReportWindow;

	TiltAlignment::Data m_TiltAlignment;
	TiltAlignment::ChannelData m_TiltAlignmentChannel[SIZE_OF_ARRAYS];

	TiltAndFlatnessHistory::Data m_TiltAndFlatnessData;
	TiltAndFlatnessHistory::ItemData m_TiltAndFlatnessItem[SIZE_OF_ARRAYS];
	TiltAndFlatnessHistory::ItemErrData m_TiltAndFlatnessItemErr[SIZE_OF_X_ARRAY];
	TiltAndFlatnessHistory::ExtItemData m_TiltAndFlatnessExtItem[SIZE_OF_X_ARRAY];
	
	TiltAndFlatnessFo::Data m_TiltAndFlatnessFo;
	TiltAndFlatnessFo::ChannelData m_TiltAndFlatnessFoChannel[SIZE_OF_ARRAYS];
	TiltAndFlatnessFo::ChannelErrData m_TiltAndFlatnessFoChannelErr[SIZE_OF_X_ARRAY];
	TiltAndFlatnessFo::ExtChannelData m_TiltAndFlatnessFoExtChannel[SIZE_OF_X_ARRAY];
	
	GyroPerformanceTestHistory::Data m_GyroPerformanceTestData;
	GyroPerformanceTestHistory::ItemData m_GyroPerformanceTestItem[SIZE_OF_ARRAYS];
	AzimuthAlignmentErrorsHistory::Data m_AzimuthAlignmentData;
	AzimuthAlignmentErrorsHistory::ItemData m_AzimuthAlignmentItem[SIZE_OF_ARRAYS];
	AzimuthVerificationBenchmarkHistory::Data m_AzimuthVerificationBenchmarkData;
	AzimuthVerificationBenchmarkHistory::ItemData m_AzimuthVerificationBenchmarkItem[SIZE_OF_ARRAYS];
	AzimuthVerificationGyrostabilityHistory::Data m_AzimuthVerificationGyrostabData;
	AzimuthVerificationGyrostabilityHistory::ItemData m_AzimuthVerificationGyrostabItem[SIZE_OF_ARRAYS];
	HorizonAbsoluteModeHistory::Data m_HorizonAbsoluteModeData;
	HorizonAbsoluteModeHistory::ItemData m_HorizonAbsoluteModeItem[SIZE_OF_ARRAYS];
	HorizonRelativeModeHistory::Data m_HorizonRelativeModeData;
	HorizonRelativeModeHistory::ItemData m_HorizonRelativeModeItem[SIZE_OF_ARRAYS];
	CommonFlatTiltHistory::Data m_CommonFlatTestData;
	CommonFlatTiltHistory::ItemData m_CommonFlatTestItem[SIZE_OF_ARRAYS];
	SensorValidationHistory::Data m_SensorValidationData;
	SensorValidationHistory::ItemData m_SensorValidationItem[SIZE_OF_ARRAYS];
	LiveGraphErrorsHistory::Data m_LiveGraphData;
	LiveGraphErrorsHistory::ItemData m_LiveGraphItem[SIZE_OF_ARRAYS];
	LiveDataA202ErrorsHistory::Data m_LiveDataA202Data;
	LiveDataA202ErrorsHistory::ItemData m_LiveDataA202Item[SIZE_OF_ARRAYS];
};
