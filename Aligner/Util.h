// $Id: Util.h,v 1.24 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "Aligner.h"
#include "DAU.h"
#include "Defines.h"

#define NOT_CONNECTED_COLOR RGB(230,230,230)

//typedef enum { UndefinedMode, TiltAlignmentMode, AzimuthAlignMode, AzimuthVerBenchmarkMode, AzimuthVerGyroStabMode, TiltVerHorizonMode, TiltVerAirTargetMode, TiltFlatnesstestMode, TiltFlatnesstestPlatformMode, TiltFlatnesstestFoundationMode, GyroPerformanceTestMode, GunAdapterTiltCheckMode, LiveDataGraphMode, CommonFlatTestMode } ALIGNMENT_MODE;
struct DAUScaleCalibrationResult
{
    double m_rollGain ;
    double m_pitchGain ;
} ;

struct DAUOffsetCalibrationResult
{
    int m_rollOffset ;
    int m_pitchOffset ;
public:
    DAUOffsetCalibrationResult( void ) {
    }
} ;

struct SelectedData
{
    BOOL m_use ;
    CString m_operatorName ;
    double m_value ;
    DBTIMESTAMP m_time ;
    double m_temperature ;
    CString m_dauSerialNumber ;
public:
    SelectedData( BOOL use, const CString &operatorName, double value, const DBTIMESTAMP &time, double temperature, const CString dauSerialNumber ) {
        m_use = use ;
        m_operatorName = operatorName ;
        m_value = value ;
        m_time = time ;
        m_temperature = temperature ; 
        m_dauSerialNumber = dauSerialNumber ;
    }

    SelectedData( void ) {
        m_use = FALSE ;
        m_operatorName = _T("") ;
        m_value = 0 ;
        //m_time =  ;
        m_temperature = 0 ; 
        m_dauSerialNumber = _T("") ;
    }
} ;

// struct ObjItem
// {
//     int num;
//     double Fi;
//     double ZParCorr;
// };

typedef struct
{
    BOOL Break;    
    ModeT Mode1;
    double Tao;
    int RefObjNo;
    double FiRef;                         //Fi1
    double rotateBackRef;
    double FiObj[SIZE_OF_ARRAYS];         //Fi2    
    double ZParCorr[SIZE_OF_ARRAYS];
    int ObjNo[SIZE_OF_ARRAYS];
    double rotateBack[SIZE_OF_ARRAYS];
    //ObjItem objItem[SIZE_OF_ARRAYS];
    int CountObj;
    long int RangeX;
    int RangeY;
    BOOL UseElev; 
    double Fs;
    BOOL SperryActive;
    BOOL commonFlatTest;
}MeasureInParam;

typedef struct
{
    double DeltaRoll[SIZE_OF_ARRAYS];
    double DeltaPitch[SIZE_OF_ARRAYS];
    double TempT[SIZE_OF_ARRAYS];
}MeasureOutParam;

int RoundToInt( double (v) );
int RoundToInt( float (v) );
long RoundToLong( double (v) );
long RoundToLong( float (v) );

//keep the function names from the old pascal code
void FreqToTs( double freq, double* pTs);
void FreqToTs( unsigned int FreqvDiv, double* pTs);
void E1( double* pAc, double* pAl, double R, double P, double Alfa );
void DoEvents();
void CartToVec( double RL, double PL, double *pAL, double *pFiL );
CString GetDateStr( void );
CString GetTimeStr( BOOL showSeconds );
CString GetDateStampStr( DBTIMESTAMP *timeStamp );
CString GetTimeStampStr( DBTIMESTAMP *timeStamp, BOOL showSeconds );
double GetRange0ToPlusMinus180( double angleIn );
double GetRange0ToPlus360( double angleIn );
void SineFit( double* pAzim, double pTilt[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE], int JC, int N, double* pC2, double* pC1, double* pC0 );
void SineFitFo( double* pAzim, int *pIndexArmLen, double pTilt[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE], int JC, int N, double* pC2, double* pC1, double* pC0 );
void CAdjText( CString* pDum, int Inc );
void RAdjText( CString* pDum, int Inc );
void LAdjText( CString* pDum, int Inc );
BOOL InitGlobalSensorStrings( BOOL reset = FALSE );
void InitSystemSetupData();
BOOL CheckSystemSetupData( void );
void CheckAndResetMeasurementSetups( ALIGNMENT_MODE nextAlignmentMode );
void ResetAllFilters( DAU *pSource, double Ts, double Teta, double Tao );
void ResetAllFilters( DAU *pSource );
void ResetAllNomAz();
BOOL IsPlaneOrGunConnected();
BOOL IsFixedConnected();
int GetNoOfConnectedStations();
void GetMeasurementSetup( DAU *pSource, BOOL enableReference, CString* pSetup, BOOL commonFlatTest = FALSE );
CString GetChannelName( int arrayIndex);
CString GetTypeName( int arrayIndex );
CString GetSensorSerialNumber( int arrayIndex);
int GetChannelNumber( int arrayIndex );
void SaveResultToDatabase( void );
void MoveDlgToRightSideOfTheGraph( CWnd *pWnd, CFormView* pGraph = NULL );
void MoveDlgToRightSideOfApp(CWnd *pWnd);
void MoveDlgToTheCenterOfMainWnd( CWnd *pWnd );
double AdjustDegAngle( double angle, BOOL range0ToPlusMinus180, int signDef );
int GetArrayIndex( CString channelName );
BOOL GetCompositeAngle( double coarseAngle, double fineAngle, double fineAngleGearing, double* pCompositeAngle );
void AdjustSyncroValueToRangeA202( double* pSyncroValue, double gearing );
int AskGunAdapterTiltCheck();
BOOL IsGunPresent();
UnitType::Types GetUnitType(int arrayIndex);


__declspec(selectany) double FoundationStraightEdgeAngle ;

//Setup functions
BOOL IsSelected( CString channelName, BOOL asReference = TRUE, BOOL asToBeTested = TRUE );
BOOL IsConnected( CString channelName );
BOOL IsConnected( int arrayIndex );
BOOL IsFixed( CString channelName );
BOOL IsFixed( int arrayIndex );
BOOL IsPlane( CString channelName );
BOOL IsPlane( int arrayIndex );
BOOL IsGun( CString channelName );
BOOL IsGun( int arrayIndex );
BOOL IsTheo(CString channelName);
BOOL IsTheo(int arrayIndex);
BOOL IsSensor( int arrayIndex );
BOOL IsSyncro( int arrayIndex );
BOOL IsSyncro( int arrayIndex );
BOOL IsDigital( int arrayIndex );
BOOL IsValidChannel( int arrayIndex );
void SetNotConnected( CString channelName );
CString GetUnitTypeDescription( CString channelName );
CString GetUnitTypeDescription( int arrayIndex );
void SetUnitTypeDescription( int arrayIndex, CString unitDescription );
CString GetUnitTypeSerialNumber( CString channelName );
CString GetUnitTypeSerialNumber( int arrayIndex );
void SetUnitTypeSerialNumber( int arrayIndex, CString unitSerialNumber );
double GetZPar( int arrayIndex );
void SetZPar( int arrayIndex, double zPar );
double GetZeroRef( int arrayIndex );
//int GetZeroRef( int arrayIndex );
void SetZeroRef( int arrayIndex, double zeroRef );
//void SetZeroRef( int arrayIndex, int zeroRef );
int GetIndexArmLength( int arrayIndex );
void SetIndexArmLength( int arrayIndex, int indexArmLength );
//int GetIndexArm2Length( int arrayIndex );
//void SetIndexArm2Length( int arrayIndex, int indexArmLength );
CString GetGunAdapterNumber( int arrayIndex );
void SetGunAdapterNumber( int arrayIndex, CString gunAdapterNumber );
double GetEal( int arrayIndex );
void SetEal( int arrayIndex, double eal );
double GetEac( int arrayIndex );
void SetEac( int arrayIndex, double eac );
double GetEalAbs( int arrayIndex );
void SetEalAbs( int arrayIndex, double ealAbs );
double ReadTemperature( int arrayIndex );
void SetTemperature( int arrayIndex, double temperature );
void CopyTemperature( double* tempToCopy );
double GetTemperature( int arrayIndex, const DAUValues &values ) ;
double GetRoll( int arrayIndex, const DAUValues &values ) ;
double GetUncalibratedRoll( int arrayIndex, const DAUValues &values ) ;
double GetUncalibratedPitch( int arrayIndex, const DAUValues &values ) ;
double GetUncalibratedHeading( int arrayIndex, const DAUValues &values ) ;
double GetPitch( int arrayIndex, const DAUValues &values ) ;
double GetNominalAzimuthDegree( int arrayIndex, DAU *pSource = NULL ) ;
double GetNominalAzimuthMRad( int arrayIndex, DAU *pSource = NULL ) ;
double GetGearing( int arrayIndex, DAU *pSource = NULL ) ;
void SetMRad( BOOL mRad );
BOOL GetMRad();
void SetSelectedChannels( DAU *pSource = NULL, BOOL fromLiveGraphData=FALSE );
void ClearSelectedChannels( DAU *pSource = NULL );
void SetUsedAsSelectedChannels( DAU *pSource = NULL);
//BOOL MoveFileToTemporaryDirectory( CString &fileNameWithPath );
CString GetFileName( CString &fileNameWithPath );
void SetupSensors(MeasureInParam& inParam);
Digital* GetDigital(CString name);
CString D1Name();
CString D2Name();
int ResolutionToBits(int res);
CString GetDAUName();

class CDelay
{
public:
	CDelay();
	virtual ~CDelay();

	void StartDelay( DWORD delaySeconds );
	void ResetDelay( void );
	BOOL IsReady( void );

private:
  BOOL m_ready;
	BOOL m_break;
	DWORD m_delayTimeSeconds;
};

