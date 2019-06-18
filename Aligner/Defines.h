// $Id: Defines.h,v 1.7 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define TEMPERATURE_NOT_AVAILABLE (-1.0)

#define MTA			3.437747; // 1 [mrad] = MTA [arcmin]
#define INVALID_VALUE (1E10)

#define SECONDS_TO_MINUTES( second )  ( (double)(second) / 60.0 )
#define SECONDS_TO_HOURS( second )    ( (double)(second) / 3600.0 )
#define MINUTES_TO_SECONDS( min )     ( (double)(min) * 60.0 )
#define MINUTES_TO_HOURS( min )       ( (double)(min) / 60.0 )
#define HOURS_TO_SECONDS( hour )      ( (double)(hour) * 3600.0 )
#define HOURS_TO_MINUTES( hour )      ( (double)(hour) * 60.0 )
#define ARCMIN_TO_MRADIANS( arcmin )  ( (double)(arcmin) * ( (1000*M_PI) / (60*180) ) )
#define ARCMIN_TO_RADIANS( arcmin )		( (double)(arcmin) * ( M_PI / (60*180) ) )
#define MRADIANS_TO_ARCMIN( mRad )    ( (double)(mRad) * ( (60*180) / (1000*M_PI) ) )
#define MRADIANS_TO_RADIANS( mRad )		( (double)(mRad) / 1000.0 )
#define MRADIANS_TO_DEGREES( mRad )		( (double)(mRad) * ( 180 / (1000*M_PI) ) )
#define DEGREES_TO_RADIANS( deg )     ( ( M_PI * (deg) ) / 180.0 )
#define DEGREES_TO_MRADIANS( deg )		( ( 1000 * M_PI * (deg) ) / 180.0 )
#define RADIANS_TO_MRADIANS( rad )		( (double)(rad) * 1000.0 )

#define SQUARE( value )               ( (value) * (value) )

#define DAU_MAX_NB_SYNCRO_CHANNELS  10 //4(2+2) for Aligner 308 10(4+6) for Aligner 202
#define DAU_MAX_NB_SENSOR_CHANNELS	7
#define DAU_NB_SYNCRO_CHANNELS			(DAU::GetDAU().GetSyncroCount())
#define DAU_NB_SENSOR_CHANNELS			(DAU::GetDAU().GetSensorCount())
#define DAU_NB_CHANNELS             (DAU_NB_SYNCRO_CHANNELS + DAU_NB_SENSOR_CHANNELS)
#define SIZE_OF_ARRAYS              (DAU_MAX_NB_SYNCRO_CHANNELS + DAU_MAX_NB_SENSOR_CHANNELS + 1)
#define FIRST_ARRAY_INDEX_SYNCRO    (1)
#define LAST_ARRAY_INDEX_SYNCRO     (FIRST_ARRAY_INDEX_SYNCRO + DAU_NB_SYNCRO_CHANNELS - 1)
#define FIRST_ARRAY_INDEX_SENSOR    (LAST_ARRAY_INDEX_SYNCRO + 1)
#define LAST_ARRAY_INDEX_SENSOR     (FIRST_ARRAY_INDEX_SENSOR + DAU_NB_SENSOR_CHANNELS - 1)

#define CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( index ) ((index) - FIRST_ARRAY_INDEX_SYNCRO)
#define CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( index ) (index - FIRST_ARRAY_INDEX_SENSOR) //(LAST_ARRAY_INDEX_SENSOR - (index))//
#define CONVERT_SYNCRO_CHANNEL_TO_ARRAY_INDEX( channel ) ( (channel) + FIRST_ARRAY_INDEX_SYNCRO )
#define CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX( channel ) (channel + FIRST_ARRAY_INDEX_SENSOR) //( LAST_ARRAY_INDEX_SENSOR - (channel) )

#define MIN_NO_OF_FLATNESS_MEASUREMENTS	3
#define MAX_NO_OF_FLATNESS_MEASUREMENTS	108

#define MAX_NO_OF_GRAPHS_ON_ONE_MEASUREMENT	(MAX_NO_OF_FLATNESS_MEASUREMENTS + 3)

#define MAX_NO_OF_TO_BE_TESTED_IN_COMMON_FLAT_TEST 3

//defines for PlaneType
#define PLANE_TYPE_FIXED     UnitType::Fixed
#define PLANE_TYPE_GUN       UnitType::Gun
#define PLANE_TYPE_PLANE     UnitType::Platform
#define PLANE_TYPE_THEO      UnitType::Theo
#define PLANE_TYPE_EMPTY     UnitType::Unused

//defines for Incl, GunAdap, Device
#define INCL_EMPTY           _T("")
#define GUN_ADAP_EMPTY       _T("")
#define DEVICE_EMPTY         _T("")

//defines for AzDir
#define PLUS_0_TO_PLUS_180    _T("0°/180°")
#define MINUS_90_TO_PLUS_90   _T("-90°/90°")

//#define SIZE_OF_GYRO_ARRAY        (DAU_NB_SYNCRO_CHANNELS+1)
#define SIZE_OF_XFIT_ARRAY        (MAX_NO_OF_FLATNESS_MEASUREMENTS + 1) 
#define SIZE_OF_X_ARRAY           (MAX_NO_OF_FLATNESS_MEASUREMENTS + 1)
#define SIZE_OF_YT_MATRIX_X_SIZE  9
#define SIZE_OF_YT_MATRIX_Y_SIZE  (MAX_NO_OF_FLATNESS_MEASUREMENTS + 1)

//Error defines
#define ERR_INVALID_INPARAM             -3
#define ERR_NO_SOURCE                   -2
#define ERR_CANCEL                      -1
#define ERR_NO_ERROR                    0
#define ERR_DATA_ACQ_INTERRUPTED        1
#define ERR_ACQ_STOP_OVERFLOW           2 //2 - 19
#define ERR_PARALLEL_PORT_BUSY          20
#define ERR_ACQ_STOP_TIMEOUT            21
#define ERR_SYNC_WRONG_DATA             22
#define ERR_SYNC_TOO_MANY_DATA          23
#define ERR_ACCESS_DENIED               24
#define ERR_TOO_LOW_SPEED               25
#define ERR_OPTO_OR_PRINTER_ERROR       26
#define ERR_DAU_RESET_FAULT             27
#define ERR_OPTO_OR_DAU_ERROR           28
#define ERR_OPTO_NOT_16_BIT             29
#define ERR_SW_CANT_RUN                 30
#define ERR_ACCESS_DENIED_SW_CANT_RUN   31
#define ERR_DISK_WRITE                  32
#define ERR_PC_CARD_FOR_GYRO            33
#define ERR_READ_ERROR_DIG_GYRO         34
#define ERR_INVALID_DIG_GYRO_ROLL       35
#define ERR_INVALID_DIG_GYRO_PITCH      36

//Channel name
#define SPERRY_CHANNEL	_T("J1")
#define D1	_T("D1")
#define D2	_T("D2")
#define D3	_T("D3")
#define D4	_T("D4")
#define J1	_T("J1")
#define J2	_T("J2")
#define A1	_T("1A")
#define B1	_T("1B")
#define C1	_T("1C")
#define A2	_T("2A")
#define B2	_T("2B")
#define C2	_T("2C")
#define J3	_T("J3")
#define J4	_T("J4")
#define J5	_T("J5")
#define J6	_T("J6")
#define J7	_T("J7")
#define J8	_T("J8")
#define J9	_T("J9")
#define CH3	_T("CH3")
#define CH4	_T("CH4")

// Measurement version
#define VERSION_NOT_DEFINED							0
#define TILT_ALIGNMENT									1
#define AZIMUTH_ALIGN										2
#define AZIMUTH_VERIFICATION_BENCHMARK	3
#define AZIMUTH_VERIFICATION_GYROSTAB		4
#define HORIZON_ABSOLUTE_MODE						5
#define AIR_TARGET_RELATIVE_MODE				6
#define GYRO_PERFORMANCE_TEST						7
#define TILT_FLATNESS_TEST							8
#define COMMON_FLAT_TEST                9
#define LIVE_GRAPH											10
#define LIVE_DATA_A202                  11
#define TILT_FLATNESS_FOUNDATION_TEST		12
#define SENSOR_VALIDATION_TEST                13

#define TAU_FREQUENCY_FACTOR								1.5915
#define MIN_FILTER_TIME_CONSTANT_LIVE_DATA	0.05

//for syncroProperties m_resolution
#define RESOLUTION_10_BIT     0x00
#define RESOLUTION_12_BIT     0x01
#define RESOLUTION_14_BIT     0x02
#define RESOLUTION_16_BIT     0x03
#define RESOLUTION_NOT_USED   0xFF

#define DEFAULT_FILE_DIR								_T(".\\")
