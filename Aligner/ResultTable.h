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
	BOOL ShowReport();	
	BOOL AddGraph( CString fileName, BOOL includeToResultTable );
    
    BOOL OpenMainReport();
    BOOL OpenCalibrationReport();

	void SetReportFolder( string baseDir ) ;
  
    InParam m_InParam;
    LiveDataA202Param* m_pLiveDataA202Param;
    long m_reportMeasID;

protected:

    BOOL InitiateReport( InParam* pInParam );
    BOOL SaveToDataBase( void );
	BOOL OpenReport( BOOL SaveToDB );
	BOOL IsReportOpen( void );
    
    CAlignmentWizard* m_pParent;

	TiltAlignment::Data m_TiltAlignment;
	TiltAlignment::ChannelData m_TiltAlignmentChannel[SIZE_OF_ARRAYS];

	TiltAndFlatness::Data m_TiltAndFlatness;
	TiltAndFlatness::ChannelData m_TiltAndFlatnessChannel[SIZE_OF_ARRAYS];
	TiltAndFlatness::ChannelErrData m_TiltAndFlatnessChannelErr[SIZE_OF_X_ARRAY];
	TiltAndFlatness::ExtChannelData m_TiltAndFlatnessExtChannel[SIZE_OF_X_ARRAY];
	
	TiltAndFlatnessFo::Data m_TiltAndFlatnessFo;
	TiltAndFlatnessFo::ChannelData m_TiltAndFlatnessFoChannel[SIZE_OF_ARRAYS];
	TiltAndFlatnessFo::ChannelErrData m_TiltAndFlatnessFoChannelErr[SIZE_OF_X_ARRAY];
	TiltAndFlatnessFo::ExtChannelData m_TiltAndFlatnessFoExtChannel[SIZE_OF_X_ARRAY];
	
	GyroPerformance::Data m_GyroPerformance;
	GyroPerformance::ChannelData m_GyroPerformanceChannel[SIZE_OF_ARRAYS];

	AzimuthAlignment::Data m_AzimuthAlignmentData;
	AzimuthAlignment::ChannelData m_AzimuthAlignmentChannel[SIZE_OF_ARRAYS];

	CommonFlatTilt::Data m_CommonFlat;
	CommonFlatTilt::ChannelData m_CommonFlatChannel[SIZE_OF_ARRAYS];

	HorizonAbsoluteMode::Data m_HorizonAbsoluteMode;
	HorizonAbsoluteMode::ChannelData m_HorizonAbsoluteModeChannel[SIZE_OF_ARRAYS];
	HorizonRelativeMode::Data m_HorizonRelativeMode;
	HorizonRelativeMode::ChannelData m_HorizonRelativeModeChannel[SIZE_OF_ARRAYS];

	SensorValidation::Data m_SensorValidation;
	SensorValidation::ChannelData m_SensorValidationChannel[SIZE_OF_ARRAYS];

	LiveGraph::Data m_LiveGraph;
	LiveGraph::ChannelData m_LiveGraphChannel[SIZE_OF_ARRAYS];
};
