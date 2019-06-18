// $Id: AlignerReport.h,v 1.1 2009/10/14 08:10:46 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#ifdef ALIGNERREPORT_EXPORTS
#define ALIGNERREPORT_API __declspec(dllexport)
#else
#define ALIGNERREPORT_API __declspec(dllimport)
#endif

#define DECLARE_REPORT(name) \
class ALIGNERREPORT_API name##Report { \
private: name##Report( void ) ; \
public: static void Create( LONG id, HANDLE &handle ) ; \
public: static void Create( LONG id ) ; \
public: static void Create( void ) ; \
public: static void Close( LONG id ) ; \
public: static void Update( LONG id ) ; \
public: static BOOL IsOpen( LONG id ) ; \
} ;
//public: static String* GetExeFileName( void ) ; \

DECLARE_REPORT(AzimuthAlignmentErrors) ;
DECLARE_REPORT(AzimuthVerificationBenchmark) ;
DECLARE_REPORT(AzimuthVerificationGyrostab) ;
DECLARE_REPORT(CommonFlatAzimuth) ;
DECLARE_REPORT(CommonFlatTilt) ;
DECLARE_REPORT(SensorValidationTest);
DECLARE_REPORT(GyroPerformanceTest) ;
DECLARE_REPORT(HorizonAbsoluteMode) ;
DECLARE_REPORT(HorizonRelativeMode) ;
DECLARE_REPORT(TiltAndFlatness) ;
DECLARE_REPORT(TiltAndFlatnessFo) ;
DECLARE_REPORT(TiltAlignmentErrors) ;
DECLARE_REPORT(LiveGraphErrors) ;
DECLARE_REPORT(LiveDataA202Errors) ;
DECLARE_REPORT(MainReportA202) ;
DECLARE_REPORT(MainReport) ;
DECLARE_REPORT(CalibrationReport) ;

BOOL ALIGNERREPORT_API CreateDirectoryPath( std::string &directory ) ;
void ALIGNERREPORT_API SetReportDirectory( std::string &directory ) ;