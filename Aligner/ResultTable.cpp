// $Id: ResultTable.cpp,v 1.16 2015-03-30 15:06:36 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "PresentDialog.h"
#include "AlignmentWizard.h"
#include "ResultTable.h"
#include "calibInfo.h"
#include "ReportManager.h"

//#ifdef BUILD_ALIGNER_308
//#include "../AlignerReport/AlignerReport.h"
//#endif


void CResultTable::SetReportFolder( string baseDir )
{
//#ifdef BUILD_ALIGNER_308
//    SetReportDirectory( baseDir );
//#endif

}

CResultTable::CResultTable( CWnd* pParent /*=NULL*/)
{
	m_InParam.Version = MeasType::MT_Undefined;
	m_reportMeasID = -1;
    m_pParent = (CAlignmentWizard*)pParent;
}

CResultTable::~CResultTable()
{
}

BOOL CResultTable::InitiateReport( InParam* pInParam )
{
	if( pInParam == NULL )
	{
		pInParam = &m_InParam;
	}
    double help1, help2, help3;
    CalibrationInfo calibInfo;
    
    switch( pInParam->Version )
    {
	    /***************************************************************************/
	    /*																TILT_ALIGNMENT													 */
	    /***************************************************************************/
	case MeasType::MT_TiltAlignment:
		m_TiltAlignment.m_timeConstant = g_AlignerData.TaoTilt;

        if( g_AlignerData.NoOfBedPlanes != 0 )
        {
			m_TiltAlignment.m_lineOfSightDirection = g_AlignerData.AzDir;
			m_TiltAlignment.m_elevationCompensation = g_AlignerData.UseElevError;
        }
		else
		{
			m_TiltAlignment.m_lineOfSightDirection = DB_EMPTY_STRING;//empty
			m_TiltAlignment.m_elevationCompensation = false;//empty
		}

        m_TiltAlignment.m_comment = (m_InParam.Comment.GetLength() == 0 ) ? DB_EMPTY_STRING : m_InParam.Comment;
		m_TiltAlignment.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
		m_TiltAlignment.m_time = m_InParam.Time;

		m_TiltAlignmentChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_TiltAlignmentChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );

	    m_TiltAlignmentChannel[0].m_sensorSerialNumber = (IsSensor( g_AlignerData.RefObjNo )) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;
	
		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
			m_TiltAlignmentChannel[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
            m_TiltAlignmentChannel[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_TiltAlignmentChannel[0].m_roll = 0.0f;
        m_TiltAlignmentChannel[0].m_pitch = 0.0f;
	    m_TiltAlignmentChannel[0].m_tilt = 0.0f;
	    m_TiltAlignmentChannel[0].m_angle = 0.0f;
		m_TiltAlignmentChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);

	    if( IsFixed( g_AlignerData.RefObjNo ) == FALSE )
        {
		    m_TiltAlignmentChannel[0].m_elevation = g_AlignerData.Kh * GetEal( g_AlignerData.RefObjNo );
		    m_TiltAlignmentChannel[0].m_bias = g_AlignerData.Kh * GetEac( g_AlignerData.RefObjNo );
        }
        else
        {
            m_TiltAlignmentChannel[0].m_elevation = 0.0f;
            m_TiltAlignmentChannel[0].m_bias = 0.0f;
        }

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_TiltAlignmentChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_TiltAlignmentChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
		    m_TiltAlignmentChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
			m_TiltAlignmentChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);

			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY )  )
			{
				m_TiltAlignmentChannel[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_TiltAlignmentChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

            m_TiltAlignmentChannel[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
            m_TiltAlignmentChannel[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_TiltAlignmentChannel[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
			m_TiltAlignmentChannel[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );

			if( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE )
            {
				m_TiltAlignmentChannel[i].m_elevation = g_AlignerData.Kh * GetEal( g_AlignerData.ObjNo[i] );
				m_TiltAlignmentChannel[i].m_bias = g_AlignerData.Kh * GetEac( g_AlignerData.ObjNo[i] );
            }
            else
            {
                m_TiltAlignmentChannel[i].m_elevation = 0.0f;
                m_TiltAlignmentChannel[i].m_bias = 0.0f;
            }
        }//for( i=1; i<=g_AlignerData.NoOfCorr; i++ )

        //Get calibration status
        calibInfo.SetCalibrationTime(m_TiltAlignment.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if(m_TiltAlignmentChannel[i].m_sensorSerialNumber == "" && m_TiltAlignmentChannel[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_TiltAlignmentChannel[i].m_channel);   
            calibInfo.AddSensor(m_TiltAlignmentChannel[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_TiltAlignmentChannel[i].m_adapterSerialNumber);   
        }
        m_TiltAlignment.calibInfo = calibInfo.GetInfo();

	break;
	/***************************************************************************/
	/*																AZIMUTH_ALIGN														 */
	/***************************************************************************/
  case MeasType::MT_AzimuthAlign:
  case MeasType::MT_AZVerBenchmark:
  case MeasType::MT_AZVerGyrostab:

		m_AzimuthAlignmentData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthAlignmentData.m_rollExcentricity = MRADIANS_TO_DEGREES( g_AlignerData.RExc );
		m_AzimuthAlignmentData.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
		m_AzimuthAlignmentData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;			    
		m_AzimuthAlignmentData.m_time = m_InParam.Time;

		m_AzimuthAlignmentChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_AzimuthAlignmentChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
		
		m_AzimuthAlignmentChannel[0].m_sensorSerialNumber = ( IsSensor( g_AlignerData.RefObjNo ) ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;
	
		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
			m_AzimuthAlignmentChannel[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
			m_AzimuthAlignmentChannel[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_AzimuthAlignmentChannel[0].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
		m_AzimuthAlignmentChannel[0].m_nominalAzimuthdifference = 0.0f;
		m_AzimuthAlignmentChannel[0].m_measuredAzimuthDifference = 0.0f;
		m_AzimuthAlignmentChannel[0].m_measuredNominalDifference = 0.0f;
		m_AzimuthAlignmentChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_AzimuthAlignmentChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);
			m_AzimuthAlignmentChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_AzimuthAlignmentChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			
			m_AzimuthAlignmentChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
			
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY )  )
			{
				m_AzimuthAlignmentChannel[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_AzimuthAlignmentChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthAlignmentChannel[i].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] );
            help1 = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] ) - GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
      		help2 =  MRADIANS_TO_DEGREES( pInParam->pACA[i] );
      		help3 = help2 - help1;
			m_AzimuthAlignmentChannel[i].m_nominalAzimuthdifference = help1;
			m_AzimuthAlignmentChannel[i].m_measuredAzimuthDifference = help2;
			m_AzimuthAlignmentChannel[i].m_measuredNominalDifference = help3;
        }
    
        //Get calibration status
        calibInfo.SetCalibrationTime(m_AzimuthAlignmentData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_AzimuthAlignmentChannel[i].m_sensorSerialNumber == "" && m_AzimuthAlignmentChannel[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_AzimuthAlignmentChannel[i].m_channel);
            calibInfo.AddSensor(m_AzimuthAlignmentChannel[i].m_sensorSerialNumber);
            calibInfo.AddAdapter(m_AzimuthAlignmentChannel[i].m_adapterSerialNumber);
        }
        m_AzimuthAlignmentData.calibInfo = calibInfo.GetInfo();

    break;
	
  case MeasType::MT_VerifAbsolute:

	  m_HorizonAbsoluteMode.m_timeConstant = g_AlignerData.TaoTilt;
	  m_HorizonAbsoluteMode.m_parallaxCompensation.LoadString( g_AlignerData.UseParallax == TRUE ? IDS_ON : IDS_OFF);
	
	  m_HorizonAbsoluteMode.m_elevationCompensation.LoadString( g_AlignerData.UseElevError == TRUE ? IDS_ON : IDS_OFF);
	  m_HorizonAbsoluteMode.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	  m_HorizonAbsoluteMode.m_refChannel = "";// GetChannelName(g_AlignerData.RefObjNo);
	  m_HorizonAbsoluteMode.m_time = m_InParam.Time;

		int j;
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
		{
			j = i-1; //no reference at index 0
			m_HorizonAbsoluteModeChannel[j].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_HorizonAbsoluteModeChannel[j].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			m_HorizonAbsoluteModeChannel[j].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
			m_HorizonAbsoluteModeChannel[j].m_type= GetUnitType(g_AlignerData.ObjNo[i]);

			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
			{
				m_HorizonAbsoluteModeChannel[j].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_HorizonAbsoluteModeChannel[j].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}
			
			m_HorizonAbsoluteModeChannel[j].m_roll = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACR[i];
			m_HorizonAbsoluteModeChannel[j].m_pitch = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACP[i];
			m_HorizonAbsoluteModeChannel[j].m_tilt = g_AlignerData.Kh*g_AlignerData.VecAmp[i];
			m_HorizonAbsoluteModeChannel[j].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
			m_HorizonAbsoluteModeChannel[j].m_elevation = g_AlignerData.Kh * GetEalAbs( g_AlignerData.ObjNo[i] );
			m_HorizonAbsoluteModeChannel[j].m_standardDeviation = g_AlignerData.Kh*g_AlignerData.Sigma[i];
			m_HorizonAbsoluteModeChannel[j].m_maximumDeviation = g_AlignerData.Kh*m_pParent->m_MaxSineFitError[i];
			m_HorizonAbsoluteModeChannel[j].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
		}

        //Get calibration status
        calibInfo.SetCalibrationTime(m_HorizonAbsoluteMode.m_time);
        for( int i=0; i<g_AlignerData.NoOfCorr; i++ )
        {
			if(m_HorizonAbsoluteModeChannel[i].m_sensorSerialNumber == "" && m_HorizonAbsoluteModeChannel[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_HorizonAbsoluteModeChannel[i].m_channel);   
            calibInfo.AddSensor(m_HorizonAbsoluteModeChannel[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_HorizonAbsoluteModeChannel[i].m_adapterSerialNumber);   
        }
        m_HorizonAbsoluteMode.calibInfo = calibInfo.GetInfo();

    break;
	/***************************************************************************/
	/*													AIR_TARGET_RELATIVE_MODE 											 */
	/***************************************************************************/
    case MeasType::MT_VerifRelative:

		m_HorizonRelativeMode.m_timeConstant = g_AlignerData.TaoTilt;
		m_HorizonRelativeMode.m_parallaxCompensation.LoadString( g_AlignerData.UseParallax == TRUE ? IDS_ON : IDS_OFF);
		m_HorizonRelativeMode.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
		m_HorizonRelativeMode.m_elevationCompensation.LoadString( g_AlignerData.UseElevError == TRUE ? IDS_ON :IDS_OFF);
		m_HorizonRelativeMode.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
		
		m_HorizonRelativeMode.m_time = m_InParam.Time;

		m_HorizonRelativeModeChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_HorizonRelativeModeChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
		m_HorizonRelativeModeChannel[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;

		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY ) )
		{
			m_HorizonRelativeModeChannel[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
		}
		else
		{
			m_HorizonRelativeModeChannel[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		}

		m_HorizonRelativeModeChannel[0].m_roll = 0.0f;
		m_HorizonRelativeModeChannel[0].m_pitch = 0.0f;
		m_HorizonRelativeModeChannel[0].m_tilt = 0.0f;
		m_HorizonRelativeModeChannel[0].m_angle = 0.0f;
		m_HorizonRelativeModeChannel[0].m_elevation = 0.0f;
		m_HorizonRelativeModeChannel[0].m_standardDeviation = 0.0f;
		m_HorizonRelativeModeChannel[0].m_maximumDeviation = 0.0f;
		m_HorizonRelativeModeChannel[0].m_azimuth = 0.0f;
		m_HorizonRelativeModeChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);

        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
		{
			m_HorizonRelativeModeChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);
			m_HorizonRelativeModeChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_HorizonRelativeModeChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			
			m_HorizonRelativeModeChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
	
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
			{
				m_HorizonRelativeModeChannel[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_HorizonRelativeModeChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}
			
			m_HorizonRelativeModeChannel[i].m_roll = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACR[i];
			m_HorizonRelativeModeChannel[i].m_pitch = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACP[i];
			m_HorizonRelativeModeChannel[i].m_tilt = g_AlignerData.Kh*g_AlignerData.VecAmp[i];
			m_HorizonRelativeModeChannel[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
			m_HorizonRelativeModeChannel[i].m_elevation = g_AlignerData.Kh * g_AlignerData.EalS[i];

			m_HorizonRelativeModeChannel[i].m_standardDeviation = g_AlignerData.Kh*g_AlignerData.Sigma[i];
			m_HorizonRelativeModeChannel[i].m_maximumDeviation = g_AlignerData.Kh*m_pParent->m_MaxSineFitError[i];
			m_HorizonRelativeModeChannel[i].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
		}
        //Get calibration status
        calibInfo.SetCalibrationTime(m_HorizonRelativeMode.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_HorizonRelativeModeChannel[i].m_sensorSerialNumber == "" && m_HorizonRelativeModeChannel[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_HorizonRelativeModeChannel[i].m_channel);
            calibInfo.AddSensor(m_HorizonRelativeModeChannel[i].m_sensorSerialNumber);
            calibInfo.AddAdapter(m_HorizonRelativeModeChannel[i].m_adapterSerialNumber);
        }
        m_HorizonRelativeMode.calibInfo = calibInfo.GetInfo();

        break;
	/***************************************************************************/
	/*														GYRO_PERFORMANCE_TEST  											 */
	/***************************************************************************/
	case MeasType::MT_GyroPerf:

		m_GyroPerformance.m_timeConstant = g_AlignerData.TaoGyro;
		m_GyroPerformance.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
		m_GyroPerformance.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;		
		m_GyroPerformance.m_time = m_InParam.Time;

		m_GyroPerformanceChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_GyroPerformanceChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
		m_GyroPerformanceChannel[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ):DB_EMPTY_STRING;
	

		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
			m_GyroPerformanceChannel[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
			m_GyroPerformanceChannel[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_GyroPerformanceChannel[0].m_roll = 0.0f;
		m_GyroPerformanceChannel[0].m_pitch = 0.0f;
		m_GyroPerformanceChannel[0].m_tilt = 0.0f;
		m_GyroPerformanceChannel[0].m_angle = 0.0f;
		m_GyroPerformanceChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_GyroPerformanceChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);
			m_GyroPerformanceChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_GyroPerformanceChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			m_GyroPerformanceChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			m_GyroPerformanceChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
		
			m_GyroPerformanceChannel[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
			m_GyroPerformanceChannel[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_GyroPerformanceChannel[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
			m_GyroPerformanceChannel[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_GyroPerformance.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_GyroPerformanceChannel[i].m_sensorSerialNumber == "" && m_GyroPerformanceChannel[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_GyroPerformanceChannel[i].m_channel);
            calibInfo.AddSensor(m_GyroPerformanceChannel[i].m_sensorSerialNumber);
            calibInfo.AddAdapter(m_GyroPerformanceChannel[i].m_adapterSerialNumber);
        }
		m_GyroPerformance.calibInfo = calibInfo.GetInfo();


        break;
	/***************************************************************************/
	/*														TILT_FLATNESS_TEST													 */
	/***************************************************************************/
  case MeasType::MT_TiltFlatnessPl:
		{
		
			m_TiltAndFlatness.m_timeConstant = g_AlignerData.TaoFlat;
			m_TiltAndFlatness.m_numberOfMeasurements = m_pParent->m_N;
			m_TiltAndFlatness.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
	        m_TiltAndFlatness.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
			m_TiltAndFlatness.m_time = m_InParam.Time;

			m_TiltAndFlatnessChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
			m_TiltAndFlatnessChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
            m_TiltAndFlatnessChannel[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ):DB_EMPTY_STRING;

			if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY ) )
			{
				m_TiltAndFlatnessChannel[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
			}
			else
			{
				m_TiltAndFlatnessChannel[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}
			
			m_TiltAndFlatnessChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessChannel[0].m_roll = 0.0f;
			m_TiltAndFlatnessChannel[0].m_pitch = 0.0f;
			m_TiltAndFlatnessChannel[0].m_tilt = 0.0f;
			m_TiltAndFlatnessChannel[0].m_angle = 0.0f;
			m_TiltAndFlatnessChannel[0].m_standardDeviation = 0.0f;
			m_TiltAndFlatnessChannel[0].m_maximumDeviation = 0.0f;
			m_TiltAndFlatnessChannel[0].m_azimuth = 0.0f;
			m_TiltAndFlatnessChannel[0].m_elevation= 0.0f;
	
			for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
			{
				m_TiltAndFlatnessChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);
				m_TiltAndFlatnessChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
				m_TiltAndFlatnessChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
				m_TiltAndFlatnessChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
			
				if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
				{
					m_TiltAndFlatnessChannel[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
				}
				else
				{
					m_TiltAndFlatnessChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
				}
				
				m_TiltAndFlatnessChannel[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
				m_TiltAndFlatnessChannel[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
				m_TiltAndFlatnessChannel[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
				m_TiltAndFlatnessChannel[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
				m_TiltAndFlatnessChannel[i].m_elevation = g_AlignerData.Kh * m_pParent->m_MeanSineFit[i];

				if( g_AlignerData.Found == TRUE )
				{
					m_TiltAndFlatnessChannel[i].m_standardDeviation = g_AlignerData.Sigma[i];
					m_TiltAndFlatnessChannel[i].m_maximumDeviation = m_pParent->m_MaxSineFitError[i];
				}
				else
				{
					m_TiltAndFlatnessChannel[i].m_standardDeviation = g_AlignerData.Kh * g_AlignerData.Sigma[i];
					m_TiltAndFlatnessChannel[i].m_maximumDeviation = g_AlignerData.Kh * m_pParent->m_MaxSineFitError[i];
				}
				//***  Azimuth is always in degrees and should not be scaled by .Kh
				m_TiltAndFlatnessChannel[i].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
			}

            //Get calibration status
            calibInfo.SetCalibrationTime(m_TiltAndFlatness.m_time);
            for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
            {
				if(m_TiltAndFlatnessChannel[i].m_sensorSerialNumber == "" && m_TiltAndFlatnessChannel[i].m_adapterSerialNumber == "")
					continue;

				calibInfo.AddChannel(m_TiltAndFlatnessChannel[i].m_channel);
                calibInfo.AddSensor(m_TiltAndFlatnessChannel[i].m_sensorSerialNumber);
                calibInfo.AddAdapter(m_TiltAndFlatnessChannel[i].m_adapterSerialNumber);
            }
            m_TiltAndFlatness.calibInfo = calibInfo.GetInfo();


			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
			{
				for( int j=1; j<=m_pParent->m_N; j++ )
				{
					m_TiltAndFlatnessChannelErr[i*m_pParent->m_N + j].m_azimuth = m_pParent->m_XAngle[j] ;
					m_TiltAndFlatnessChannelErr[i*m_pParent->m_N + j].m_error = m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
				}
			}

			//Extended Item
		/*	double deviation[SIZE_OF_X_ARRAY][SIZE_OF_ARRAYS] ;//will be copied to m_deviationJ3...m_deviationJ9 !!!!!

			for( int J=1; J<=m_pParent->m_N; J++ )
			{
				m_TiltAndFlatnessExtChannelItem[J-1].m_azimuthAngle = m_pParent->m_XAngle[J];

				for( int k=1; k<=g_AlignerData.NoOfCorr; k++ )
				{
					if( g_AlignerData.Found == TRUE )
					{
						deviation[J-1][k-1] = m_pParent->m_SineFitError[k][J];
					}
					else
					{
						deviation[J-1][k-1] = g_AlignerData.Kh * m_pParent->m_SineFitError[k][J];
//						deviation[J-1][k-1] = m_pParent->m_SineFitError[k][J];
					}
				}
			}

			for( int J=1; J<=m_pParent->m_N; J++ )
			{
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ3 = deviation[J-1][0];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ4 = deviation[J-1][1];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ5 = deviation[J-1][2];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ6 = deviation[J-1][3];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ7 = deviation[J-1][4];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ8 = deviation[J-1][5];
				m_TiltAndFlatnessExtItem[J-1].m_deviationJ9 = deviation[J-1][6];
			}*/	
		}
		break;
	/***************************************************************************/
	/*											TILT_FLATNESS_FOUNDATION_TEST											 */
	/***************************************************************************/
	case MeasType::MT_TiltFlatnessFo:
		{
			/****************/
			/***   Data   ***/
			/****************/
	/*		if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
			{
				m_TiltAndFlatnessFo.m_reference.LoadString( IDS_REFERENCE_FIXED_PLANE );
			}
			else
			{
				m_TiltAndFlatnessFo.m_reference.LoadString( IDS_PLATFORM_RELATIVE_ELEVATION );				
			}
		*/	
			
			m_TiltAndFlatnessFo.m_timeConstant = g_AlignerData.TaoFlat;
			m_TiltAndFlatnessFo.m_numberOfMeasurements = m_pParent->m_N;
			m_TiltAndFlatnessFo.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessFo.m_time = m_InParam.Time;

	        m_TiltAndFlatnessFo.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
		        
			m_TiltAndFlatnessFo.m_time = m_InParam.Time;

			//m_TiltAndFlatnessFo.m_IndexArmLength = GetIndexArmLength(1) ;
            

			/****************/
			/***   Item   ***/
			/****************/

			//	Ref and meas object in only one record
			//m_TiltAndFlatnessFoItem[0].m_refstation = GetUnitTypeDescription( g_AlignerData.RefObjNo );
			//m_TiltAndFlatnessFoItem[0].m_refchannel = GetChannelName( g_AlignerData.RefObjNo );
			//m_TiltAndFlatnessFoItem[0].m_refsensorSerialNumber = GetUnitTypeSerialNumber( g_AlignerData.RefObjNo );
			
			//Reference
			m_TiltAndFlatnessFoChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessFoChannel[0].m_station = GetUnitTypeDescription(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessFoChannel[0].m_channel = GetChannelName(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessFoChannel[0].m_sensorSerialNumber = GetUnitTypeSerialNumber(g_AlignerData.RefObjNo);
			m_TiltAndFlatnessFoChannel[0].m_elevation = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_roll = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_pitch = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_tilt = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_angle = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_elevation = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_elevation2 = 0.0f;

			m_TiltAndFlatnessFoChannel[0].m_standardDeviation = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_bottomError = 0.0f;
			m_TiltAndFlatnessFoChannel[0].m_maximumDeviation = 0.0f;

       		
			//Measured
			m_TiltAndFlatnessFoChannel[1].m_type = GetUnitType(g_AlignerData.ObjNo[1]);
			m_TiltAndFlatnessFoChannel[1].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[1]);
			m_TiltAndFlatnessFoChannel[1].m_channel = GetChannelName(g_AlignerData.ObjNo[1]);
			m_TiltAndFlatnessFoChannel[1].m_sensorSerialNumber = GetUnitTypeSerialNumber(g_AlignerData.ObjNo[1]);				
				
			//	Ref and meas object in only one record
			m_TiltAndFlatnessFoChannel[1].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[1];
			m_TiltAndFlatnessFoChannel[1].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[1];
			m_TiltAndFlatnessFoChannel[1].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[1];
			m_TiltAndFlatnessFoChannel[1].m_angle = AdjustDegAngle(g_AlignerData.VecArg[1], pInParam->AngleRange0ToPlusMinus180, 1);
			m_TiltAndFlatnessFoChannel[1].m_elevation = g_AlignerData.Kh * m_pParent->m_MeanSineFit[1];
			m_TiltAndFlatnessFoChannel[1].m_elevation2 = g_AlignerData.Kh * m_pParent->m_MeanSineFit[2];

			m_TiltAndFlatnessFoChannel[1].m_standardDeviation = g_AlignerData.Sigma[1];
			m_TiltAndFlatnessFoChannel[1].m_bottomError = m_pParent->m_HBottom;
			m_TiltAndFlatnessFoChannel[1].m_maximumDeviation = m_pParent->m_MaxSineFitError[1];

			//***  Azimuth is in deg and should not be scaled with arcmin factor (.Kh)
			m_TiltAndFlatnessFoChannel[1].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[1];
			
			BOOL measureWarping = (GetIndexArmLength(2) != 0);
            
          //  m_TiltAndFlatnessFoItem[0].m_IndexArmLength = GetIndexArmLength(1) ;
          //  m_TiltAndFlatnessFoItem[0].m_IndexArm2Length = GetIndexArmLength(2) ;


		    ZeroMemory(&(m_TiltAndFlatnessFoChannelErr), sizeof(m_TiltAndFlatnessFoChannelErr)); 
			for( int j=1; j<=m_pParent->m_N; j++ )
			{
				m_TiltAndFlatnessFoChannelErr[j].m_azimuth = m_pParent->m_XAngle[j] ;
				m_TiltAndFlatnessFoChannelErr[j].m_error = m_pParent->m_SineFitError[1][j] ;
				m_TiltAndFlatnessFoChannelErr[j].m_indexArmLength1 = GetIndexArmLength(1);
				
				if(measureWarping)
                {
					m_TiltAndFlatnessFoChannelErr[j].m_error2 = m_pParent->m_SineFitError[2][j] ;
					m_TiltAndFlatnessFoChannelErr[j].m_dh = m_TiltAndFlatnessFoChannelErr[j].m_error2 - m_TiltAndFlatnessFoChannelErr[j].m_error;
					m_TiltAndFlatnessFoChannelErr[j].m_indexArmLength2 = GetIndexArmLength(2) ;
				}
            }
			//Get calibration status
			calibInfo.SetCalibrationTime(m_TiltAndFlatnessFo.m_time);
			for (int i = 0; i <= g_AlignerData.NoOfCorr; i++)
			{
				if (m_TiltAndFlatnessFoChannel[i].m_sensorSerialNumber == "" && m_TiltAndFlatnessFoChannel[i].m_adapterSerialNumber == "")
					continue;

				calibInfo.AddChannel(m_TiltAndFlatnessFoChannel[i].m_channel);
				calibInfo.AddSensor(m_TiltAndFlatnessFoChannel[i].m_sensorSerialNumber);
				calibInfo.AddAdapter(m_TiltAndFlatnessFoChannel[i].m_adapterSerialNumber);
			}
			m_TiltAndFlatnessFo.calibInfo = calibInfo.GetInfo();
		}
		break;
	/***************************************************************************/
	/*															COMMON_FLAT_TEST     											 */
	/***************************************************************************/
  case MeasType::MT_CommonFlatTilt:

	  m_CommonFlat.m_timeConstant = g_AlignerData.TaoTilt;
	  m_CommonFlat.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	  m_CommonFlat.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
	  m_CommonFlat.m_time = m_InParam.Time;

	  m_CommonFlatChannel[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
	  m_CommonFlatChannel[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
	  m_CommonFlatChannel[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;

	  m_CommonFlatChannel[0].m_roll = 0.0f;
	  m_CommonFlatChannel[0].m_pitch = 0.0f;

	  m_CommonFlatChannel[0].m_parallellBias = g_AlignerData.Kh * GetEal( g_AlignerData.RefObjNo );
	  m_CommonFlatChannel[0].m_perpendicularBias = g_AlignerData.Kh * GetEac( g_AlignerData.RefObjNo );
	  m_CommonFlatChannel[0].m_temperature = m_InParam.pTemperature[g_AlignerData.RefObjNo];
	  m_CommonFlatChannel[0].m_type = GetUnitType(g_AlignerData.RefObjNo);

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_CommonFlatChannel[i].m_type = GetUnitType(g_AlignerData.ObjNo[i]);
			m_CommonFlatChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_CommonFlatChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			m_CommonFlatChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
	
			m_CommonFlatChannel[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
			m_CommonFlatChannel[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_CommonFlatChannel[i].m_parallellBias = g_AlignerData.Kh * GetEal( g_AlignerData.ObjNo[i] );
			m_CommonFlatChannel[i].m_perpendicularBias = g_AlignerData.Kh * GetEac( g_AlignerData.ObjNo[i] );
			m_CommonFlatChannel[i].m_temperature = m_InParam.pTemperature[g_AlignerData.ObjNo[i]];
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_CommonFlat.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_CommonFlatChannel[i].m_sensorSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_CommonFlatChannel[i].m_channel);   
            calibInfo.AddSensor(m_CommonFlatChannel[i].m_sensorSerialNumber);               
        }
        m_CommonFlat.calibInfo = calibInfo.GetInfo();

        break;
		/***************************************************************************/
	/*															SENSOR_VALIDATION_TEST     											 */
	/***************************************************************************/
  case MeasType::MT_SensorValidation:

	  m_SensorValidation.m_timeConstant = g_AlignerData.TaoTilt;
	  m_SensorValidation.m_refChannel = GetChannelName(g_AlignerData.RefObjNo);
	  m_SensorValidation.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;	  
	  m_SensorValidation.m_time = m_InParam.Time;
	  m_SensorValidation.platformSN = g_AlignerData.PlatformSN;

	  m_SensorValidationChannel[0].m_station = GetUnitTypeDescription(g_AlignerData.RefObjNo);
	  m_SensorValidationChannel[0].m_channel = GetChannelName(g_AlignerData.RefObjNo);
	  m_SensorValidationChannel[0].m_sensorSerialNumber = IsSensor(g_AlignerData.RefObjNo) ? GetUnitTypeSerialNumber(g_AlignerData.RefObjNo) : DB_EMPTY_STRING;

	  m_SensorValidationChannel[0].m_rollSc = 0.0f;
	  m_SensorValidationChannel[0].m_pitchSc = 0.0f;
	  m_SensorValidationChannel[0].m_rollAzErr = 0.0f;
	  m_SensorValidationChannel[0].m_pitchAzErr = 0.0f;
	  m_SensorValidationChannel[0].m_temperature = m_InParam.pTemperature[0];// g_AlignerData.RefObjNo];

	  m_SensorValidationChannel[1].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[1]);
	  m_SensorValidationChannel[1].m_channel = GetChannelName(g_AlignerData.ObjNo[1]);
	  m_SensorValidationChannel[1].m_sensorSerialNumber = IsSensor(g_AlignerData.ObjNo[1]) ? GetUnitTypeSerialNumber(g_AlignerData.ObjNo[1]) : DB_EMPTY_STRING;

	  m_SensorValidationChannel[1].m_rollSc = g_AlignerData.ACR[1];
	  m_SensorValidationChannel[1].m_pitchSc = g_AlignerData.ACP[1];
	  m_SensorValidationChannel[1].m_rollAzErr = g_AlignerData.Eac[1];
	  m_SensorValidationChannel[1].m_pitchAzErr = g_AlignerData.Eal[1];
	  m_SensorValidationChannel[1].m_temperature = m_InParam.pTemperature[1];// g_AlignerData.ObjNo[1]];


	  //Get calibration status
	  calibInfo.SetCalibrationTime(m_SensorValidation.m_time);
	  for (int i = 0; i <= g_AlignerData.NoOfCorr; i++)
	  {
		  if (m_SensorValidationChannel[i].m_sensorSerialNumber == "")
			  continue;

		  calibInfo.AddChannel(m_SensorValidationChannel[i].m_channel);
		  calibInfo.AddSensor(m_SensorValidationChannel[i].m_sensorSerialNumber);
	  }
	  m_SensorValidation.calibInfo = calibInfo.GetInfo();

	  break;
	/***************************************************************************/
	/*																LIVE_GRAPH		     											 */
	/***************************************************************************/
	case MeasType::MT_LiveGraph:
    {
        m_LiveGraph.m_timeConstant = pInParam->timeConstant;//g_AlignerData.TaoTilt;
		m_LiveGraph.m_samplingRate = pInParam->SamplingRate;
		m_LiveGraph.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;			
		m_LiveGraph.m_time = m_InParam.Time;

		//No reference for live graph
		m_LiveGraphChannel[0].m_station = _T("");
		m_LiveGraphChannel[0].m_channel = _T("");
		m_LiveGraphChannel[0].m_sensorSerialNumber = _T("");
		m_LiveGraphChannel[0].m_adapterSerialNumber = _T("");
		m_LiveGraphChannel[0].m_roll = 0.0f;
		m_LiveGraphChannel[0].m_pitch = 0.0f;
		m_LiveGraphChannel[0].m_tilt = 0.0f;
		m_LiveGraphChannel[0].m_angle = 0.0f;
		m_LiveGraphChannel[0].m_temperature = 0.0f;

        int i=1;
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++,i++)
        {
            int id = iter->second.id;
			m_LiveGraphChannel[i].m_type = GetUnitType(id);
			m_LiveGraphChannel[i].m_station = GetUnitTypeDescription( id);//g_AlignerData.ObjNo[i] );
			m_LiveGraphChannel[i].m_channel = GetChannelName( id);//g_AlignerData.ObjNo[i] );
			
			m_LiveGraphChannel[i].m_sensorSerialNumber = IsSensor(id) ? GetUnitTypeSerialNumber(id) : DB_EMPTY_STRING;
			
			if( ( IsGun( id) == TRUE ) && ( GetGunAdapterNumber( id) != GUN_ADAP_EMPTY )  )
			{
				m_LiveGraphChannel[i].m_adapterSerialNumber = GetGunAdapterNumber( id);
			}
			else
			{
				m_LiveGraphChannel[i].m_adapterSerialNumber = DB_EMPTY_STRING;
			}

			m_LiveGraphChannel[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
			m_LiveGraphChannel[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_LiveGraphChannel[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
			m_LiveGraphChannel[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );

			m_LiveGraphChannel[i].m_temperature = IsSensor(id) ? pInParam->pTemperature[i] : DB_EMPTY_DOUBLE;
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_LiveGraph.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if(m_LiveGraphChannel[i].m_sensorSerialNumber == "" && m_LiveGraphChannel[i].m_adapterSerialNumber == "")
				continue;
			
			calibInfo.AddChannel(m_LiveGraphChannel[i].m_channel);
            calibInfo.AddSensor(m_LiveGraphChannel[i].m_sensorSerialNumber);
            calibInfo.AddAdapter(m_LiveGraphChannel[i].m_adapterSerialNumber);
        }
        m_LiveGraph.calibInfo = calibInfo.GetInfo();

    }
	break;
	/***************************************************************************/
	/*															LIVE_DATA_A202	     											 */
	/***************************************************************************/
 /* case LIVE_DATA_A202:
		m_LiveDataA202Data.m_samplingRate = pInParam->SamplingRate;

		if( m_InParam.Comment.GetLength() == 0 )
		{
			m_LiveDataA202Data.m_comment = DB_EMPTY_STRING;//empty
		}
		else
		{
			m_LiveDataA202Data.m_comment = m_InParam.Comment;
		}

		m_LiveDataA202Data.m_time = m_InParam.Time;

		//No reference for live graph
	    for( int i=0; i<g_AlignerData.NoOfCorr; i++ )
        {
			m_LiveDataA202Item[i].m_station = m_pLiveDataA202Param[i].m_station;
			m_LiveDataA202Item[i].m_channel = m_pLiveDataA202Param[i].m_channel;
            m_LiveDataA202Item[i].m_value = m_pLiveDataA202Param[i].m_value;
			m_LiveDataA202Item[i].m_gearing = m_pLiveDataA202Param[i].m_gearing;
			m_LiveDataA202Item[i].m_refVoltage = m_pLiveDataA202Param[i].m_refVoltage;
        }
        break;*/
  default:
		return( FALSE );
    break;
  }
	return( TRUE );
}//InitiateReport()

	/***************************************************************************/
	/*															SaveToDataBase	     											 */
	/***************************************************************************/
BOOL CResultTable::SaveToDataBase( void )
{
	if( m_InParam.SavedToDataBase == FALSE )
	{
		m_InParam.SavedToDataBase = TRUE;

		switch( m_InParam.Version )
		{
		case MeasType::MT_TiltAlignment:
	
			if( !TiltAlignment::AddData( m_TiltAlignment ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			m_reportMeasID = TiltAlignment::GetMeasID();

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{		
				if( !TiltAlignment::AddChannel( m_TiltAlignmentChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
		

			break;

		case MeasType::MT_AzimuthAlign:
		case MeasType::MT_AZVerGyrostab:
		case MeasType::MT_AZVerBenchmark:

			m_AzimuthAlignmentData.m_comment = m_InParam.Comment;
			m_AzimuthAlignmentData.type = m_InParam.Version;

			if( !AzimuthAlignment::AddData( m_AzimuthAlignmentData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			m_reportMeasID = AzimuthAlignment::GetMeasID();

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{			
				if( !AzimuthAlignment::AddChannel( m_AzimuthAlignmentChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		
		case MeasType::MT_VerifAbsolute:
			
			m_HorizonAbsoluteMode.m_comment = m_InParam.Comment;
			

			if( !HorizonAbsoluteMode::AddData( m_HorizonAbsoluteMode ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			m_reportMeasID = HorizonAbsoluteMode::GetMeasID();
			for( int i=0; i<g_AlignerData.NoOfCorr; i++ ) // no reference, index 0 = first channel to test
			{				

				if( !HorizonAbsoluteMode::AddChannel( m_HorizonAbsoluteModeChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case MeasType::MT_VerifRelative:
			
			m_HorizonRelativeMode.m_comment = m_InParam.Comment;
			

			if( !HorizonRelativeMode::AddData( m_HorizonRelativeMode ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			m_reportMeasID = HorizonRelativeMode::GetMeasID();
			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{				

				if( !HorizonRelativeMode::AddChannel( m_HorizonRelativeModeChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case MeasType::MT_GyroPerf:
		
			
			m_GyroPerformance.m_comment = m_InParam.Comment;			

			if( !GyroPerformance::AddData( m_GyroPerformance ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			
			m_reportMeasID = GyroPerformance::GetMeasID();

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_GyroPerformanceChannel[i].m_station.GetLength() == 0 ) 
				{
					m_GyroPerformanceChannel[i].m_station = DB_EMPTY_STRING;
				}

				if( !GyroPerformance::AddChannel( m_GyroPerformanceChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case MeasType::MT_TiltFlatnessPl:
			
			m_TiltAndFlatness.m_comment = m_InParam.Comment;
			
			if( !TiltAndFlatness::AddData( m_TiltAndFlatness ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
			m_reportMeasID = TiltAndFlatness::GetMeasID();

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{			
				if( !TiltAndFlatness::AddChannel( m_TiltAndFlatnessChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
                for( int j=1; j<=m_pParent->m_N; j++ )
                {				
                    if( !TiltAndFlatness::AddChannelErr( m_TiltAndFlatnessChannelErr[m_pParent->m_N*i + j] ) )
                    {
                        ASSERT(0); // This is a "badass" error.
                    }			
                }
            }

			break;
		case MeasType::MT_TiltFlatnessFo:
						
			m_TiltAndFlatnessFo.m_comment = m_InParam.Comment;
			

			if( !TiltAndFlatnessFo::AddData( m_TiltAndFlatnessFo ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			m_reportMeasID = TiltAndFlatnessFo::GetMeasID();
	
			for (int i = 0; i <= g_AlignerData.NoOfCorr; i++) // index 0 = reference
			{
				if (!TiltAndFlatnessFo::AddChannel(m_TiltAndFlatnessFoChannel[i]))
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			
			for (int j = 1; j <= m_pParent->m_N; j++)
			{
				if (!TiltAndFlatnessFo::AddChannelErr(m_TiltAndFlatnessFoChannelErr[j]))
				{
					ASSERT(0); // This is a "badass" error.
				}
			}

		/*	if( m_TiltAndFlatnessFoChannel[0].m_refstation.GetLength() == 0 )
			{
				m_TiltAndFlatnessFoChannel[0].m_refstation = DB_EMPTY_STRING;
			}
			*/
			/*******************/
			/***   ItemErr   ***/
			/*******************/
			//for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			//{
				//if( m_TiltAndFlatnessFoItem[i].m_station.GetLength() == 0 ) 
				//{
				//	m_TiltAndFlatnessFoItem[i].m_station = DB_EMPTY_STRING;
				//}

				//if( !TiltAndFlatnessFo::AddItem( m_TiltAndFlatnessFoItem[i] ) )
				//{
				//	ASSERT(0); // This is a "badass" error.
				//}

//#define GUARD(s) do { HRESULT hResult ; if (( S_OK != (hResult = s) )) { Database::DisplayError( hResult, __FILE__, __LINE__ ) ; return FALSE ; } } while (0) ;
//#if defined(_DEBUG)
//#define GUARD_DEBUG(s,text) do { HRESULT hResult ; if (( S_OK != (hResult = s) )) { Database::DisplayError( hResult, text ) ; return FALSE ; } } while (0) ;
//#else
//#define GUARD_DEBUG(s,text) GUARD(s)
//#endif

				//if (i != 0)
				//{
					//LONG oldId = TiltAndFlatnessFo::GetLastID();
					//LastCounter lastID ;
					//GUARD(::OpenTable( lastID )) ;
					//GUARD(lastID.MoveFirst()) ;
					//TiltAndFlatnessFo::SetLastID( lastID.m_id);
				//if (i == 0)
				//{
				//	if( !TiltAndFlatnessFo::AddItemErr( m_TiltAndFlatnessFoItemErr[0] ) )	// index 0 = reference
				//	{
				//		ASSERT(0); // This is a "badass" error.
				//	}
				//}
				//else
				//{
					//LONG oldId = TiltAndFlatnessFo::GetLastID();
					//LastCounter lastID ;
					//GUARD(::OpenTable( lastID )) ;
					//GUARD(lastID.MoveFirst()) ;
					//TiltAndFlatnessFo::SetLastID( lastID.m_id);

					
				//}	// else
				//}
				//if( !TiltAndFlatnessFo::AddItemErr( m_TiltAndFlatnessFoItemErr[i] ) )
				//{
				//	ASSERT(0); // This is a "badass" error.
				//}
			//}	// for( int i=0; ...

			for( int extI=0; extI<m_TiltAndFlatnessFo.m_numberOfMeasurements; extI++ )
			{
				if( !TiltAndFlatnessFo::AddExtData( m_TiltAndFlatnessFoExtChannel[extI] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
				//if( !TiltAndFlatnessFo::AddItemErr( m_TiltAndFlatnessFoItemErr[extI] ) )
				//{
				//	ASSERT(0); // This is a "badass" error.
				//}
			}

			break;
		case MeasType::MT_CommonFlatTilt:
				
			m_CommonFlat.m_comment = m_InParam.Comment;
			
			if( !CommonFlatTilt::AddData( m_CommonFlat ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			m_reportMeasID = CommonFlatTilt::GetMeasID();
			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{			
				if( !CommonFlatTilt::AddChannel( m_CommonFlatChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case MeasType::MT_SensorValidation:
			
			m_SensorValidation.m_comment = m_InParam.Comment;			

			if (!SensorValidation::AddData(m_SensorValidation))
			{
				ASSERT(0); // This is a "badass" error.
			}
			m_reportMeasID = SensorValidation::GetMeasID();

			for (int i = 0; i <= g_AlignerData.NoOfCorr; i++) // index 0 = reference
			{
				if (m_SensorValidationChannel[i].m_station.GetLength() == 0)
				{
					m_SensorValidationChannel[i].m_station = DB_EMPTY_STRING;
				}

				if (!SensorValidation::AddChannel(m_SensorValidationChannel[i]))
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;

		case MeasType::MT_LiveGraph:

			m_LiveGraph.m_comment = m_InParam.Comment;			

			if( !LiveGraph::AddData( m_LiveGraph ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			m_reportMeasID = LiveGraph::GetMeasID();
			
			for( int i=1; i<=g_AlignerData.liveGraphMap.size(); i++ ) // index 0 = reference, but live graph has no reference
			{
				if( m_LiveGraphChannel[i].m_station.GetLength() == 0 ) 
				{
					m_LiveGraphChannel[i].m_station = DB_EMPTY_STRING;
				}

				if( !LiveGraph::AddChannel( m_LiveGraphChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}            

      break;
   // case LIVE_DATA_A202:
		
			/*m_LiveDataA202Data.m_comment = m_InParam.Comment;
			
			if( !LiveDataA202ErrorsHistory::AddData( m_LiveDataA202Data ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}
            {
            CString tm ;
            tm.Format( "corr=%d\n", g_AlignerData.NoOfCorr ) ;
            }
			for( int i=0; i<g_AlignerData.NoOfCorr; i++ ) 
			{
				if( m_LiveDataA202Item[i].m_station.GetLength() == 0 ) 
				{
					m_LiveDataA202Item[i].m_station = DB_EMPTY_STRING;
				}

				if( !LiveDataA202ErrorsHistory::AddItem( m_LiveDataA202Item[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
      break;*/
		default:
			m_InParam.SavedToDataBase = FALSE;
			break;
		}
        
        //Store graph entries in DB.
        if(m_InParam.pFileGraphManager != NULL)
        {            
			m_InParam.pFileGraphManager->Sort();
			for( int i = 0; i < m_InParam.pFileGraphManager->GetNoOfSavedFiles(); i++ )
            {                        
				CString file = m_InParam.pFileGraphManager->GetFileName(i);
				BOOL include = m_InParam.pFileGraphManager->GetIncludeToResultTable(i);

                if( TRUE != AddGraph( file, include))
                {
                    //result = FALSE;
                }           
            }
        }
    }

   

  return TRUE ;
}

BOOL CResultTable::DeleteLast(void)
{
	BOOL res =  DBInterface::Instance()->DeleteRecord("Measurement", m_reportMeasID);
	m_reportMeasID = -1;

	return res;
}


/*	BOOL result = TRUE;
#ifdef BUILD_ALIGNER_308
	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		result = TiltAlignment::DeleteMeas();
		break;
	case AZIMUTH_ALIGN:
		result = AzimuthAlignmentErrorsHistory::DeleteLast();
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		result = AzimuthVerificationBenchmarkHistory::DeleteLast();
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		result = AzimuthVerificationGyrostabilityHistory::DeleteLast();
		break;
	case HORIZON_ABSOLUTE_MODE:
		result = HorizonAbsoluteModeHistory::DeleteLast();
		break;
	case AIR_TARGET_RELATIVE_MODE:
		result = HorizonRelativeModeHistory::DeleteLast();
		break;
	case GYRO_PERFORMANCE_TEST:
		result = GyroPerformanceTestHistory::DeleteLast();
		break;
	case TILT_FLATNESS_TEST:
		result = TiltAndFlatnessHistory::DeleteLast();
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		result = TiltAndFlatnessFo::DeleteLast();
		break;
	case COMMON_FLAT_TEST:
		result = CommonFlatTiltHistory::DeleteLast();
		break;
	case SENSOR_VALIDATION_TEST:
		result = SensorValidationHistory::DeleteLast();
		break;
	case LIVE_GRAPH:
		result = LiveGraphErrorsHistory::DeleteLast();
		break;
  case LIVE_DATA_A202:
    result = LiveDataA202ErrorsHistory::DeleteLast();
    break;
	default:
		result = FALSE;
		break;
	}
#endif
	if( result == TRUE )
	{
		m_reportMeasID = -1;
	}
	return( result );
}
*/

BOOL CResultTable::OpenReport( BOOL SaveToDB )
{
	BOOL result = TRUE;

	if( SaveToDB == TRUE )
	{
		result = SaveToDataBase();
	}

	ReportManager rm;
	rm.OpenReport(SysSetup->GetProjectID(), m_reportMeasID);

	/*if( CreateEmptyReport() == FALSE )
	{
		return( FALSE );
	}
	
	result =  ShowPresentDialog( m_InParam.ShowPresenetDialog );
#endif*/
	return( result );
}
/*
BOOL CResultTable::CreateEmptyReport( void )
{
#ifdef BUILD_ALIGNER_308
    switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		m_reportHistoryMainID = TiltAlignment::GetMeasID();
		TiltAlignmentErrorsReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case AZIMUTH_ALIGN:
		m_reportHistoryMainID = AzimuthAlignmentErrorsHistory::GetMainID();
		AzimuthAlignmentErrorsReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		m_reportHistoryMainID = AzimuthVerificationBenchmarkHistory::GetMainID();
		AzimuthVerificationBenchmarkReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		m_reportHistoryMainID = AzimuthVerificationGyrostabilityHistory::GetMainID();
		AzimuthVerificationGyrostabReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case HORIZON_ABSOLUTE_MODE:
		m_reportHistoryMainID = HorizonAbsoluteModeHistory::GetMainID();
		HorizonAbsoluteModeReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case AIR_TARGET_RELATIVE_MODE:
		m_reportHistoryMainID = HorizonRelativeModeHistory::GetMainID();
		HorizonRelativeModeReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case GYRO_PERFORMANCE_TEST:
		m_reportHistoryMainID = GyroPerformanceTestHistory::GetMainID();
		GyroPerformanceTestReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case TILT_FLATNESS_TEST:
		m_reportHistoryMainID = TiltAndFlatnessHistory::GetMainID();
		TiltAndFlatnessReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		m_reportHistoryMainID = TiltAndFlatnessFo::GetMainID();
		TiltAndFlatnessFoReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case COMMON_FLAT_TEST:
		m_reportHistoryMainID = CommonFlatTiltHistory::GetMainID();
		CommonFlatTiltReport::Create( m_reportHistoryMainID, m_hReportWindow );
		break;
	case SENSOR_VALIDATION_TEST:
		m_reportHistoryMainID = SensorValidationHistory::GetMainID();
		SensorValidationTestReport::Create(m_reportHistoryMainID, m_hReportWindow);
		break;
	case LIVE_GRAPH:
        try{
            m_reportHistoryMainID = LiveGraphErrorsHistory::GetMainID();
		    LiveGraphErrorsReport::Create( m_reportHistoryMainID, m_hReportWindow );
        }catch(_com_error &e)
        {
            TRACE("Com Error");
        }catch(...)
        {
           TRACE("Error");
        }
		break;
  case LIVE_DATA_A202:
		m_reportHistoryMainID = LiveDataA202ErrorsHistory::GetMainID();
		LiveDataA202ErrorsReport::Create( m_reportHistoryMainID, m_hReportWindow );
    break;
	default:
		return( FALSE );
		break;
	}
#endif
	return( TRUE );
}

BOOL CResultTable::ShowPresentDialog( BOOL Show )
{
#ifdef BUILD_ALIGNER_308
    if( m_hReportWindow == NULL )
	{
		return( FALSE );
	}

	CWnd *pReportWindow = CWnd::FromHandle( (HWND)m_hReportWindow );
	if( pReportWindow == NULL )
	{
		return( FALSE );
	}

	if( m_pPresentDlg == NULL )
	{
		m_pPresentDlg = new CPresentDialog( this, pReportWindow );
	}
		
	if( m_pPresentDlg->Create(CPresentDialog::IDD, pReportWindow) != TRUE ) 
	{
		return( FALSE );
	}

    CRect rect;

    m_pPresentDlg->GetWindowRect(&rect);
    m_pPresentDlg->MoveWindow(5, 650, rect.Width(), rect.Height());

	if( Show == TRUE )
	{
		m_pPresentDlg->ShowWindow( SW_SHOW );
	}
#endif
	return( TRUE );
}

//BOOL CResultTable::UpdateReport( void )
//{
//	BOOL result;
//	CloseReport();
//	m_InParam.ShowPresenetDialog = FALSE;
//	result = OpenReport( TRUE );
//	m_InParam.ShowPresenetDialog = TRUE;
//	return( result );
//}

BOOL CResultTable::UpdateComment( void )
{
	BOOL result;
#ifdef BUILD_ALIGNER_308
    CloseReport();

	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
//		result = TiltAlignment::SetComment( m_InParam.Comment );
		break;
	case AZIMUTH_ALIGN:
		result = AzimuthAlignmentErrorsHistory::SetComment( m_InParam.Comment );
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		result = AzimuthVerificationBenchmarkHistory::SetComment( m_InParam.Comment );
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		result = AzimuthVerificationGyrostabilityHistory::SetComment( m_InParam.Comment );
		break;
	case HORIZON_ABSOLUTE_MODE:
		result = HorizonAbsoluteModeHistory::SetComment( m_InParam.Comment );
		break;
	case AIR_TARGET_RELATIVE_MODE:
		result = HorizonRelativeModeHistory::SetComment( m_InParam.Comment );
		break;
	case GYRO_PERFORMANCE_TEST:
		result = GyroPerformanceTestHistory::SetComment( m_InParam.Comment );
		break;
	case TILT_FLATNESS_TEST:
		result = TiltAndFlatnessHistory::SetComment( m_InParam.Comment );
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		result = TiltAndFlatnessFo::SetComment( m_InParam.Comment );
		break;
	case COMMON_FLAT_TEST:
		result = CommonFlatTiltHistory::SetComment( m_InParam.Comment );
		break;
	case SENSOR_VALIDATION_TEST:
		result = SensorValidationHistory::SetComment(m_InParam.Comment);
		break;
	case LIVE_GRAPH:
		result = LiveGraphErrorsHistory::SetComment( m_InParam.Comment );
		break;
  case LIVE_DATA_A202:
    result = LiveDataA202ErrorsHistory::SetComment( m_InParam.Comment );
    break;
	default:
		result = FALSE;
		break;
	}

	if( result == TRUE )
	{
		m_InParam.ShowPresenetDialog = FALSE;
		Sleep(400);
        result = OpenReport( FALSE );
		m_InParam.ShowPresenetDialog = TRUE;
	}
	
#endif
    return result ;
}


BOOL CResultTable::ResetMainID( void )
{
#ifdef BUILD_ALIGNER_308
    switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		TiltAlignment::ResetMeasID();
		break;
	case AZIMUTH_ALIGN:
		AzimuthAlignmentErrorsHistory::ResetMainID();
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		AzimuthVerificationBenchmarkHistory::ResetMainID();
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		AzimuthVerificationGyrostabilityHistory::ResetMainID();
		break;
	case HORIZON_ABSOLUTE_MODE:
		HorizonAbsoluteModeHistory::ResetMainID();
		break;
	case AIR_TARGET_RELATIVE_MODE:
		HorizonRelativeModeHistory::ResetMainID();
		break;
	case GYRO_PERFORMANCE_TEST:
		GyroPerformanceTestHistory::ResetMainID();
		break;
	case TILT_FLATNESS_TEST:
		TiltAndFlatnessHistory::ResetMainID();
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		TiltAndFlatnessFo::ResetMainID();
		break;
	case COMMON_FLAT_TEST:
		CommonFlatTiltHistory::ResetMainID();
		break;
	case SENSOR_VALIDATION_TEST:
		SensorValidationHistory::ResetMainID();
		break;
	case LIVE_GRAPH:
		LiveGraphErrorsHistory::ResetMainID();
		break;
  case LIVE_DATA_A202:
    LiveDataA202ErrorsHistory::ResetMainID();
    break;
	default:
		return( FALSE );
		break;
	}
#endif
    return TRUE ;
}
/*
long CResultTable::GetMainID( void )
{
    long id=0;
#ifdef BUILD_ALIGNER_308    
	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		id = TiltAlignment::GetMeasID();
		break;
	case AZIMUTH_ALIGN:
		id = AzimuthAlignmentErrorsHistory::GetMainID();
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		id = AzimuthVerificationBenchmarkHistory::GetMainID();
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		id = AzimuthVerificationGyrostabilityHistory::GetMainID();
		break;
	case HORIZON_ABSOLUTE_MODE:
		id = HorizonAbsoluteModeHistory::GetMainID();
		break;
	case AIR_TARGET_RELATIVE_MODE:
		id = HorizonRelativeModeHistory::GetMainID();
		break;
	case GYRO_PERFORMANCE_TEST:
		id = GyroPerformanceTestHistory::GetMainID();
		break;
	case TILT_FLATNESS_TEST:
		id = TiltAndFlatnessHistory::GetMainID();
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		id = TiltAndFlatnessFo::GetMainID();
		break;
	case COMMON_FLAT_TEST:
		id = CommonFlatTiltHistory::GetMainID();
		break;
	case SENSOR_VALIDATION_TEST:
		id = SensorValidationHistory::GetMainID();
		break;
	case LIVE_GRAPH:
		id = LiveGraphErrorsHistory::GetMainID();
		break;
  case LIVE_DATA_A202:
        id = LiveDataA202ErrorsHistory::GetMainID();
    break;
	default:
        id = -1;
		break;
	}
#endif
	return id ;
}
*/
BOOL CResultTable::AddGraph(CString fileName, BOOL includeToResultTable)
{
	return DBInterface::Instance()->InsertGraph(m_reportMeasID, fileName, includeToResultTable);	
}


/*BOOL result=TRUE;
#ifdef BUILD_ALIGNER_308    
//	CloseReport();

	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		result = TiltAlignment::AddGraph( fileName, includeToResultTable );
		break;
	case AZIMUTH_ALIGN:
		result = AzimuthAlignmentErrorsHistory::AddGraph( fileName, includeToResultTable );
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		result = AzimuthVerificationBenchmarkHistory::AddGraph( fileName, includeToResultTable );
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		result = AzimuthVerificationGyrostabilityHistory::AddGraph( fileName, includeToResultTable );
		break;
	case HORIZON_ABSOLUTE_MODE:
		result = HorizonAbsoluteModeHistory::AddGraph( fileName, includeToResultTable );
		break;
	case AIR_TARGET_RELATIVE_MODE:
		result = HorizonRelativeModeHistory::AddGraph( fileName, includeToResultTable );
		break;
	case GYRO_PERFORMANCE_TEST:
		result = GyroPerformanceTestHistory::AddGraph( fileName, includeToResultTable );
		break;
	case TILT_FLATNESS_TEST:
		result = TiltAndFlatnessHistory::AddGraph( fileName, includeToResultTable );
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		result = TiltAndFlatnessFo::AddGraph( fileName, includeToResultTable );
		break;
	case COMMON_FLAT_TEST:
		result = CommonFlatTiltHistory::AddGraph( fileName, includeToResultTable );
		break;
	case SENSOR_VALIDATION_TEST:
		result = SensorValidationHistory::AddGraph(fileName, includeToResultTable);
		break;
	case LIVE_GRAPH:
		result = LiveGraphErrorsHistory::AddGraph( fileName, includeToResultTable );
		break;
  case LIVE_DATA_A202:
    result = LiveDataA202ErrorsHistory::AddGraph( fileName, includeToResultTable );
    break;
	default:
		result = FALSE;
		break;
	}
#endif
	return result ;
}
/*
BOOL CResultTable::IsReportOpen( void )
{
	BOOL result = FALSE;
#ifdef BUILD_ALIGNER_308
	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		result = TiltAlignmentErrorsReport::IsOpen( m_reportHistoryMainID );
		break;
	case AZIMUTH_ALIGN:
		result = AzimuthAlignmentErrorsReport::IsOpen( m_reportHistoryMainID );
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		result = AzimuthVerificationBenchmarkReport::IsOpen( m_reportHistoryMainID );
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		result = AzimuthVerificationGyrostabReport::IsOpen( m_reportHistoryMainID );
		break;
	case HORIZON_ABSOLUTE_MODE:
		result = HorizonAbsoluteModeReport::IsOpen( m_reportHistoryMainID );
		break;
	case AIR_TARGET_RELATIVE_MODE:
		result = HorizonRelativeModeReport::IsOpen( m_reportHistoryMainID );
		break;
	case GYRO_PERFORMANCE_TEST:
		result = GyroPerformanceTestReport::IsOpen( m_reportHistoryMainID );
		break;
	case TILT_FLATNESS_TEST:
		result = TiltAndFlatnessReport::IsOpen( m_reportHistoryMainID );
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		result = TiltAndFlatnessFoReport::IsOpen( m_reportHistoryMainID );
		break;
	case COMMON_FLAT_TEST:
		result = CommonFlatTiltReport::IsOpen( m_reportHistoryMainID );
		break;
	case SENSOR_VALIDATION_TEST:
		result = SensorValidationTestReport::IsOpen(m_reportHistoryMainID);
		break;
	case LIVE_GRAPH:
		result = LiveGraphErrorsReport::IsOpen( m_reportHistoryMainID );
		break;
  case LIVE_DATA_A202:
    result = LiveDataA202ErrorsReport::IsOpen( m_reportHistoryMainID );
    break;
	default:
		result = FALSE;
		break;
	}
#endif
	return result;
}
*/
BOOL CResultTable::ShowReport(BOOL CloseFirst)
{
	BOOL result = InitiateReport(NULL);
	if(result )
	{
		result = OpenReport(TRUE);
	}

	
	return result;
}
/*
	if( CloseFirst == TRUE )
	{
		CloseReport();
	}

	if( IsReportOpen() == TRUE )
	{
		if( m_hReportWindow == NULL )
		{
			return( FALSE );
		}
		::SetActiveWindow( (HWND)m_hReportWindow );
		result = TRUE;
	}
	else
	{
		if( (result = InitiateReport( NULL ) ) == FALSE )
		{
			return( result );
		}
		result = OpenReport( TRUE );
	}
	return result;
}*/

BOOL CResultTable::OpenCalibrationReport()
{
    return TRUE;
}

BOOL CResultTable::OpenMainReport()
{
    return TRUE;
}
/*
BOOL CResultTable::CloseReport( void )
{
#ifdef BUILD_ALIGNER_308
    switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		TiltAlignmentErrorsReport::Close( m_reportHistoryMainID );
		break;
	case AZIMUTH_ALIGN:
		AzimuthAlignmentErrorsReport::Close( m_reportHistoryMainID );
		break;
	case AZIMUTH_VERIFICATION_BENCHMARK:
		AzimuthVerificationBenchmarkReport::Close( m_reportHistoryMainID );
		break;
	case AZIMUTH_VERIFICATION_GYROSTAB:
		AzimuthVerificationGyrostabReport::Close( m_reportHistoryMainID );
		break;
	case HORIZON_ABSOLUTE_MODE:
		HorizonAbsoluteModeReport::Close( m_reportHistoryMainID );
		break;
	case AIR_TARGET_RELATIVE_MODE:
		HorizonRelativeModeReport::Close( m_reportHistoryMainID );
		break;
	case GYRO_PERFORMANCE_TEST:
		GyroPerformanceTestReport::Close( m_reportHistoryMainID );
		break;
	case TILT_FLATNESS_TEST:
		TiltAndFlatnessReport::Close( m_reportHistoryMainID );
		break;
	case TILT_FLATNESS_FOUNDATION_TEST:
		TiltAndFlatnessFoReport::Close( m_reportHistoryMainID );
		break;
	case COMMON_FLAT_TEST:
		CommonFlatTiltReport::Close( m_reportHistoryMainID );
		break;
	case SENSOR_VALIDATION_TEST:
		SensorValidationTestReport::Close(m_reportHistoryMainID);
		break;
	case LIVE_GRAPH:
		LiveGraphErrorsReport::Close( m_reportHistoryMainID );
		break;
  case LIVE_DATA_A202:
    LiveDataA202ErrorsReport::Close( m_reportHistoryMainID );
    break;
	default:
		return( FALSE );
		break;
	}

//	m_hReportWindow = NULL;
#endif
	return( TRUE );
}*/


/***************************************************************************/
	/*												AZIMUTH_VERIFICATION_BENCHMARK									 */
	/***************************************************************************/
/*	case AZIMUTH_VERIFICATION_BENCHMARK:
		m_AzimuthVerificationBenchmarkData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthVerificationBenchmarkData.m_rollExcentricity = MRADIANS_TO_DEGREES(g_AlignerData.RExc);

		m_AzimuthVerificationBenchmarkData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
		m_AzimuthVerificationBenchmarkData.m_measuredUnit.LoadString(GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_AzimuthVerificationBenchmarkData.m_time = m_InParam.Time;

		m_AzimuthVerificationBenchmarkItem[0].m_station = GetUnitTypeDescription(g_AlignerData.RefObjNo);
		m_AzimuthVerificationBenchmarkItem[0].m_channel = GetChannelName(g_AlignerData.RefObjNo);
		m_AzimuthVerificationBenchmarkItem[0].m_sensorSerialNumber = IsSensor(g_AlignerData.RefObjNo) ? GetUnitTypeSerialNumber(g_AlignerData.RefObjNo) : DB_EMPTY_STRING;

		if ((IsGun(g_AlignerData.RefObjNo) == TRUE) && (GetGunAdapterNumber(g_AlignerData.RefObjNo) != GUN_ADAP_EMPTY))
		{
			m_AzimuthVerificationBenchmarkItem[0].m_adapterSerialNumber = GetGunAdapterNumber(g_AlignerData.RefObjNo);
		}
		else
		{
			m_AzimuthVerificationBenchmarkItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		}

		m_AzimuthVerificationBenchmarkItem[0].m_nominalAzimuth = GetNominalAzimuthDegree(g_AlignerData.RefObjNo);
		m_AzimuthVerificationBenchmarkItem[0].m_nominalAzimuthdifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationBenchmarkItem[0].m_measuredAzimuthDifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationBenchmarkItem[0].m_measuredNominalDifference = DB_EMPTY_DOUBLE;//ref

		for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
		{
			m_AzimuthVerificationBenchmarkItem[i].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[i]);
			m_AzimuthVerificationBenchmarkItem[i].m_channel = GetChannelName(g_AlignerData.ObjNo[i]);

			m_AzimuthVerificationBenchmarkItem[i].m_sensorSerialNumber = IsSensor(g_AlignerData.ObjNo[i]) ? GetUnitTypeSerialNumber(g_AlignerData.ObjNo[i]) : DB_EMPTY_STRING;

			if ((IsGun(g_AlignerData.ObjNo[i]) == TRUE) && (GetGunAdapterNumber(g_AlignerData.ObjNo[i]) != GUN_ADAP_EMPTY))
			{
				m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber = GetGunAdapterNumber(g_AlignerData.ObjNo[i]);
			}
			else
			{
				m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthVerificationBenchmarkItem[i].m_nominalAzimuth = GetNominalAzimuthDegree(g_AlignerData.ObjNo[i]);
			help1 = GetNominalAzimuthDegree(g_AlignerData.ObjNo[i]) - GetNominalAzimuthDegree(g_AlignerData.RefObjNo);
			help2 = MRADIANS_TO_DEGREES(pInParam->pACA[i]);
			help3 = help2 - help1;
			m_AzimuthVerificationBenchmarkItem[i].m_nominalAzimuthdifference = help1;
			m_AzimuthVerificationBenchmarkItem[i].m_measuredAzimuthDifference = help2;
			m_AzimuthVerificationBenchmarkItem[i].m_measuredNominalDifference = help3;
		}
		//Get calibration status
		calibInfo.SetCalibrationTime(m_AzimuthVerificationBenchmarkData.m_time);
		for (int i = 0; i <= g_AlignerData.NoOfCorr; i++)
		{
			if (m_AzimuthVerificationBenchmarkItem[i].m_sensorSerialNumber == "" && m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_AzimuthVerificationBenchmarkItem[i].m_channel);
			calibInfo.AddSensor(m_AzimuthVerificationBenchmarkItem[i].m_sensorSerialNumber);
			calibInfo.AddAdapter(m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber);
		}
		m_AzimuthVerificationBenchmarkData.calibInfo = calibInfo.GetInfo();

		break;
		/***************************************************************************/
		/*												AZIMUTH_VERIFICATION_GYROSTAB										 */
		/***************************************************************************/
	/*case AZIMUTH_VERIFICATION_GYROSTAB:
		m_AzimuthVerificationGyrostabData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthVerificationGyrostabData.m_rollExcentricity = MRADIANS_TO_DEGREES(g_AlignerData.RExc);

		m_AzimuthVerificationGyrostabData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;

		m_AzimuthVerificationGyrostabData.m_measuredUnit.LoadString((GetMRad() == TRUE) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_AzimuthVerificationGyrostabData.m_time = m_InParam.Time;

		m_AzimuthVerificationGyrostabItem[0].m_station = GetUnitTypeDescription(g_AlignerData.RefObjNo);
		m_AzimuthVerificationGyrostabItem[0].m_channel = GetChannelName(g_AlignerData.RefObjNo);
		m_AzimuthVerificationGyrostabItem[0].m_sensorSerialNumber = IsSensor(g_AlignerData.RefObjNo) ? GetUnitTypeSerialNumber(g_AlignerData.RefObjNo) : DB_EMPTY_STRING;

		if ((IsGun(g_AlignerData.RefObjNo) == TRUE) && (GetGunAdapterNumber(g_AlignerData.RefObjNo) != GUN_ADAP_EMPTY))
		{
			m_AzimuthVerificationGyrostabItem[0].m_adapterSerialNumber = GetGunAdapterNumber(g_AlignerData.RefObjNo);
		}
		else
		{
			m_AzimuthVerificationGyrostabItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		}

		m_AzimuthVerificationGyrostabItem[0].m_nominalAzimuth = GetNominalAzimuthDegree(g_AlignerData.RefObjNo);
		m_AzimuthVerificationGyrostabItem[0].m_nominalAzimuthdifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationGyrostabItem[0].m_measuredAzimuthDifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationGyrostabItem[0].m_measuredNominalDifference = DB_EMPTY_DOUBLE;//ref

		for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
		{
			m_AzimuthVerificationGyrostabItem[i].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[i]);
			m_AzimuthVerificationGyrostabItem[i].m_channel = GetChannelName(g_AlignerData.ObjNo[i]);
			m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber = IsSensor(g_AlignerData.ObjNo[i]) ? GetUnitTypeSerialNumber(g_AlignerData.ObjNo[i]) : DB_EMPTY_STRING;

			if ((IsGun(g_AlignerData.ObjNo[i]) == TRUE) && (GetGunAdapterNumber(g_AlignerData.ObjNo[i]) != GUN_ADAP_EMPTY))
			{
				m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber = GetGunAdapterNumber(g_AlignerData.ObjNo[i]);
			}
			else
			{
				m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthVerificationGyrostabItem[i].m_nominalAzimuth = GetNominalAzimuthDegree(g_AlignerData.ObjNo[i]);
			help1 = GetNominalAzimuthDegree(g_AlignerData.ObjNo[i]) - GetNominalAzimuthDegree(g_AlignerData.RefObjNo);
			help2 = MRADIANS_TO_DEGREES(pInParam->pACA[i]);
			help3 = help2 - help1;
			m_AzimuthVerificationGyrostabItem[i].m_nominalAzimuthdifference = help1;
			m_AzimuthVerificationGyrostabItem[i].m_measuredAzimuthDifference = help2;
			m_AzimuthVerificationGyrostabItem[i].m_measuredNominalDifference = help3;
		}

		//Get calibration status
		calibInfo.SetCalibrationTime(m_AzimuthVerificationGyrostabData.m_time);
		for (int i = 0; i <= g_AlignerData.NoOfCorr; i++)
		{
			if (m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber == "" && m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_AzimuthVerificationGyrostabItem[i].m_channel);
			calibInfo.AddSensor(m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber);
			calibInfo.AddAdapter(m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber);
		}
		m_AzimuthVerificationGyrostabData.calibInfo = calibInfo.GetInfo();

		break;*/
		/***************************************************************************/
		/*														HORIZON_ABSOLUTE_MODE 											 */
		/***************************************************************************/