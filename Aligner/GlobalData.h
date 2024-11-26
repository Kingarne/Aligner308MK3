// $Id: GlobalData.h,v 1.12 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

//#define HPSW						//Enables Azimuth verification
//#define FOUNDATION_TEST	//Enables Foundation test
//#define INVSIGN

//Global data

#pragma once

///#include "resource.h"
#include "Type.h"
#include "Defines.h"

typedef enum { Rol, Pit } AzCalDirT;
typedef enum { All, Onegyro, Nogyro, ThreeToSix, OneToSeven } ChannelConfT;
//typedef enum { Diff, GB, Com, Azim, AB, Stab, StabDiff, Gyr, RPath } ModeT;
typedef enum { Diff, Azim, Stab, StabDiff, Gyr, RPath } ModeT;
typedef enum { a, b} PpxT;
typedef enum { plus0ToPlus180, minus90ToPlus90 } AzDirT;
typedef enum { mr, ur, arcmin, arcsec, mm, um } UnitModeT;
typedef enum { None, OnlyRange, Both, OnlyBear } RBTypeT;
typedef enum { Sine, Linear, Normal } PLModeT;
typedef enum { NormAzi, BenchAzi, StabAzi, FlatAzi } AziModeT;
typedef enum { Circular, Rectangular} FoundationT;

struct LogDataT
{
  BOOL ResultFromFirstMeasurementExists;
  double RPrim[SIZE_OF_ARRAYS];
  double dPPrim[SIZE_OF_ARRAYS];
} ;

#define Axis_None   0
#define Axis_Y1     1
#define Axis_Y2     2

struct LiveGraphInfo
{
    LiveGraphInfo(){name="";id=0;roll=Axis_None;pitch=Axis_None;heading=Axis_None;dig=FALSE;type="";lastTempr=TEMPERATURE_NOT_AVAILABLE;}
    LiveGraphInfo(CString n, int i, CString t=""){LiveGraphInfo();name=n;id=i;roll=Axis_None;pitch=Axis_None;heading=Axis_None;dig=FALSE;type=t;lastTempr=TEMPERATURE_NOT_AVAILABLE;}
    LiveGraphInfo(CString n, int r, int p, int c){name=n;roll=r;pitch=p;heading=c;dig=FALSE;type="";lastTempr=TEMPERATURE_NOT_AVAILABLE;}
    CString name;
    CString type;
    BOOL dig;
    int id;
    int roll;
    int pitch;
    int heading;
    double lastTempr;
};


//keep the constant names from the old pascal code
//namespace g_AlignerConst
//{
  //const double MTA                = 3.437747; // 1 [mrad] = MTA [arcmin]
  //const double KConv              = 0.01745329; // 1 [rad] = 1 / KConv [deg]
  //const CString Title1                  = "TILT ALIGNMENT ERRORS";
  //const CString Title2                  = "AZIMUTH ALIGNMENT ERRORS";
  //const CString Title3                  = "TILT VERIFICATION - AIR TARGET TEST";
  //const CString Title4                  = "TILT VERIFICATION - HORIZON TEST";
  //const CString Title5                  = "AZIMUTH VERIFICATION - BENCHMARK TEST";
  //const CString Title6                  = "GYRO PERFORMANCE TEST";
  //const CString Title7                  = "TILT & FLATNESS TEST";
  //const CString Title8                  = "COMMON FLAT TEST (TILT)";
  //const CString Title9                  = "AZIMUTH VERIFICATION - GYROSTAB TEST";
  //const CString Title10                 = "ABSOLUTE TILT";
  //const CString Title11                 = "COMMON FLAT TEST (AZIMUTH)";
//}

#define DF_DUMP_FIFO	0x01
#define DF_DUMP_FRAME	0x02
#define DF_DUMP_MINS	0x04

//keep the variable names from the old pascal code
typedef struct GLO
{
    double ACR[SIZE_OF_ARRAYS];
    double ACP[SIZE_OF_ARRAYS];
    CString AlignMode;
    BOOL Along;
    CString AzDir;
    double Bear;
    CString DauNo;
    CString Device[SIZE_OF_ARRAYS];
    double EalAbs[SIZE_OF_ARRAYS];
    double Eac[SIZE_OF_ARRAYS];
    double Eal[SIZE_OF_ARRAYS];
    double EalS[SIZE_OF_ARRAYS];
    BOOL ElevErrorExist;
    int ErrorDef;
    BOOL ErrorFlag;
    CString ErrorType[37]; // index [0] not used
	BOOL GunAdapterTiltCheckOK;
    BOOL Found;
    CString GunAdap[SIZE_OF_ARRAYS];
    int IA_Len[SIZE_OF_ARRAYS]; //(IndexArmLength, check if index gets correct sensor/gyro
    int IA2_Len[SIZE_OF_ARRAYS];
    CString Incl[SIZE_OF_ARRAYS];
    double Kh;
    LogDataT LogData;
    ModeT Mode;
    int NoOfBedPlanes;
    int NoOfCorr;
    int ObjNo[SIZE_OF_ARRAYS]; //index [0] not used
    CString Operator;
    CString Place;
    UnitType::Types PlaneType[SIZE_OF_ARRAYS];
    double Range;
    int RefObjNo;
    BOOL Reject;
    double RExc;
    long int RX;
	  BOOL ContinousGraph;
    int RY;
    double Sigma[SIZE_OF_ARRAYS];
    int SignDef;
    CString Ship;
    double TaoGyro;
    double TaoTilt;
    double TaoAz;
    double TaoFlat;
    BOOL TestNotSetup;
    double Tsens[SIZE_OF_ARRAYS];    
    BOOL UseElevError;
    BOOL UseParallax;
    double VecAmp[SIZE_OF_ARRAYS];
    double VecArg[SIZE_OF_ARRAYS];    
    double ZeroRef[SIZE_OF_ARRAYS];
    double ZPar[SIZE_OF_ARRAYS];
	  int debugFlag;
    map<int, LiveGraphInfo> liveGraphMap;
	  int PlatformSN;
    FoundationT FoundationType;
    CSize FoundDim;

public:

  void Init( void )
  {
        int i;
        debugFlag = 0;
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            ObjNo[i] = 0;
        }

        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            ACR[i] = 0.0;
            ACP[i] = 0.0;
        }
		ContinousGraph = FALSE;
		AlignMode = _T("");
        Along = TRUE;
        AzDir = PLUS_0_TO_PLUS_180;
        Bear = 0.0;
        DauNo = _T("") ;
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            Device[i] = _T("");
        }
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            EalAbs[i] = INVALID_VALUE;
            Eac[i] = INVALID_VALUE;
            Eal[i] = INVALID_VALUE;
            EalS[i] = INVALID_VALUE;
        }
        ElevErrorExist = FALSE;
        ErrorDef = ERR_NO_ERROR;
        ErrorFlag = FALSE;
        //TODO move strings to the string table
        ErrorType[0] = "Internal error"; // index [0] not used
        ErrorType[1] = "Data Acq interrupted or not started";
        ErrorType[2] = "Acq stopped: Oflow Ch1 roll";
        ErrorType[3] = "Acq stopped: Oflow Ch1 pitch";
        ErrorType[4] = "Acq stopped: Oflow Ch2 roll";
        ErrorType[5] = "Acq stopped: Oflow Ch2 pitch";
        ErrorType[6] = "Acq stopped: Oflow Ch9 roll";
        ErrorType[7] = "Acq stopped: Oflow Ch9 pitch";
        ErrorType[8] = "Acq stopped: Oflow Ch8 roll";
        ErrorType[9] = "Acq stopped: Oflow Ch8 pitch";
        ErrorType[10] = "Acq stopped: Oflow Ch7 roll";
        ErrorType[11] = "Acq stopped: Oflow Ch7 pitch";
        ErrorType[12] = "Acq stopped: Oflow Ch6 roll";
        ErrorType[13] = "Acq stopped: Oflow Ch6 pitch";
        ErrorType[14] = "Acq stopped: Oflow Ch5 roll";
        ErrorType[15] = "Acq stopped: Oflow Ch5 pitch";
        ErrorType[16] = "Acq stopped: Oflow Ch4 roll";
        ErrorType[17] = "Acq stopped: Oflow Ch4 pitch";
        ErrorType[18] = "Acq stopped: Oflow Ch3 roll";
        ErrorType[19] = "Acq stopped: Oflow Ch3 pitch";
        ErrorType[20] = "Cannot send.Parallel port busy";
        ErrorType[21] = "Acq stopped: Timeout reading DAU";
        ErrorType[22] = "Sync error (wrong data).Please reset HW";
        ErrorType[23] = "Sync error (too many data). Please reset HW";
        ErrorType[24] = "Access denied. Diag. test not perf. (Restart!)";
        ErrorType[25] = "Computer running with too low speed";
        ErrorType[26] = "Opto interf. not connected, or printer pow. fail";
        ErrorType[27] = "DAU reset failed";
        ErrorType[28] = "Opto fibre or DAU not conn. or printer pow. fail";
        ErrorType[29] = "Opto interf. not strapped for 16 bit data";
        ErrorType[30] = "SW will not run on this system";
        ErrorType[31] = "Access denied. SW will not run on this system";
        ErrorType[32] = "Disk write error";
        ErrorType[33] = "Cannot find PCcard for gyro.Insert card and reboot";
        ErrorType[34] = "Read error Ch0 (digital gyro)";
        ErrorType[35] = "Roll data Ch0 not valid (digital gyro)";
        ErrorType[36] = "Pitch data Ch0 not valid (digital gyro)";
        RefObjNo = -1;
	        GunAdapterTiltCheckOK = FALSE;
        #ifdef FOUNDATION_TEST
        Found = TRUE;
        #else
        Found = FALSE;
        #endif
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            GunAdap[i] = GUN_ADAP_EMPTY;
            IA_Len[i] = 0;
            IA2_Len[i] = 0;
            Incl[i] = INCL_EMPTY;
            LogData.dPPrim[i] = 0.0;
            LogData.RPrim[i] = 0.0;
        }
        
        LogData.ResultFromFirstMeasurementExists = FALSE;
        
        Mode = Diff;
	    Kh = 1;
        NoOfCorr = 0;
        NoOfBedPlanes = 0;
        Operator = _T("");
        Place = _T("");
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            PlaneType[i] = PLANE_TYPE_EMPTY ;
        }
        Range = 0.0;
        Reject = FALSE;
        RExc = 0.0;
        RX = 0; // 0->autoscale
        RY = 0; // 0->autoscale
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            Sigma[i] = 0.0;
        }

        Ship = _T("");
        TaoGyro = 12.0;
        TaoTilt = 12.0;
        TaoAz = 18.0;
        TaoFlat = 12.0;
        TestNotSetup = TRUE;
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            Tsens[i] = 0.0;
        }
        //Ts = 0.0;
        UseElevError = FALSE;
        UseParallax = FALSE;
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            VecAmp[i] = 0;
            VecArg[i] = 0;
        }
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            ZeroRef[i] = 0;
        }
        for( i=0; i<SIZE_OF_ARRAYS; i++ )
        {
            ZPar[i] = 0.0;
        }
        FoundationType = FoundationT::Circular;

    }
	
}ALIGNER_DATA;

//Global variables
extern ALIGNER_DATA g_AlignerData;
