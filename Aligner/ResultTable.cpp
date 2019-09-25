// $Id: ResultTable.cpp,v 1.16 2015-03-30 15:06:36 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "PresentDialog.h"
#include "AlignmentWizard.h"
#include "ResultTable.h"
#include "calibInfo.h"

#ifdef BUILD_ALIGNER_308
#include "../AlignerReport/AlignerReport.h"
#endif


void CResultTable::SetReportFolder( string baseDir )
{
#ifdef BUILD_ALIGNER_308
    SetReportDirectory( baseDir );
#endif

}

CResultTable::CResultTable( CWnd* pParent /*=NULL*/)
{
	m_InParam.Version = VERSION_NOT_DEFINED;
	m_reportHistoryMainID = -1;
	m_pPresentDlg = NULL;
	m_hReportWindow = NULL;
    m_pParent = (CAlignmentWizard*)pParent;
}

CResultTable::~CResultTable()
{
	// don't delete m_pPresentDlg, it will be done by the dialog's own code ??
	if( m_pPresentDlg != NULL )
	{
		delete m_pPresentDlg;
	}
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
	    case TILT_ALIGNMENT:
		m_TiltAlignment.m_timeConstant = g_AlignerData.TaoTilt;

        if( g_AlignerData.NoOfBedPlanes != 0 )
        {
			m_TiltAlignment.m_lineOfSightDirection = g_AlignerData.AzDir;
            m_TiltAlignment.m_elevationCompensation.LoadString( ( g_AlignerData.UseElevError == TRUE ) ? IDS_ON : IDS_OFF);
        }
		else
		{
			m_TiltAlignment.m_lineOfSightDirection = DB_EMPTY_STRING;//empty
			m_TiltAlignment.m_elevationCompensation = DB_EMPTY_STRING;//empty
		}

        m_TiltAlignment.m_comment = (m_InParam.Comment.GetLength() == 0 ) ? DB_EMPTY_STRING : m_InParam.Comment;
//        m_TiltAlignment.m_measuredUnit.LoadString( ( GetMRad() == TRUE ) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);	
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

	    m_TiltAlignmentChannel[0].m_roll = DB_EMPTY_DOUBLE;//ref		
        m_TiltAlignmentChannel[0].m_pitch = DB_EMPTY_DOUBLE;//ref
	    m_TiltAlignmentChannel[0].m_tilt = DB_EMPTY_DOUBLE;//ref
	    m_TiltAlignmentChannel[0].m_angle = DB_EMPTY_DOUBLE;//ref

	    if( IsFixed( g_AlignerData.RefObjNo ) == FALSE )
        {
		    m_TiltAlignmentChannel[0].m_elevation = g_AlignerData.Kh * GetEal( g_AlignerData.RefObjNo );
		    m_TiltAlignmentChannel[0].m_bias = g_AlignerData.Kh * GetEac( g_AlignerData.RefObjNo );
        }
        else
        {
            m_TiltAlignmentChannel[0].m_elevation = DB_EMPTY_DOUBLE;//empty
            m_TiltAlignmentChannel[0].m_bias = DB_EMPTY_DOUBLE;//empty
        }

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_TiltAlignmentChannel[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_TiltAlignmentChannel[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
		    m_TiltAlignmentChannel[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;

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
                m_TiltAlignmentChannel[i].m_elevation = DB_EMPTY_DOUBLE;//empty
                m_TiltAlignmentChannel[i].m_bias = DB_EMPTY_DOUBLE;//empty
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
  case AZIMUTH_ALIGN:
		m_AzimuthAlignmentData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthAlignmentData.m_rollExcentricity = MRADIANS_TO_DEGREES( g_AlignerData.RExc );

		m_AzimuthAlignmentData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;		
	    m_AzimuthAlignmentData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_AzimuthAlignmentData.m_time = m_InParam.Time;

		m_AzimuthAlignmentItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_AzimuthAlignmentItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
		
	    m_AzimuthAlignmentItem[0].m_sensorSerialNumber = ( IsSensor( g_AlignerData.RefObjNo ) ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;
	
		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
			m_AzimuthAlignmentItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
		    m_AzimuthAlignmentItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_AzimuthAlignmentItem[0].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
		m_AzimuthAlignmentItem[0].m_nominalAzimuthdifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthAlignmentItem[0].m_measuredAzimuthDifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthAlignmentItem[0].m_measuredNominalDifference = DB_EMPTY_DOUBLE;//ref
	
	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_AzimuthAlignmentItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_AzimuthAlignmentItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			
			m_AzimuthAlignmentItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
			
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY )  )
			{
				m_AzimuthAlignmentItem[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_AzimuthAlignmentItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthAlignmentItem[i].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] );
            help1 = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] ) - GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
      		help2 =  MRADIANS_TO_DEGREES( pInParam->pACA[i] );
      		help3 = help2 - help1;
            m_AzimuthAlignmentItem[i].m_nominalAzimuthdifference = help1;
            m_AzimuthAlignmentItem[i].m_measuredAzimuthDifference = help2;
			m_AzimuthAlignmentItem[i].m_measuredNominalDifference = help3;
        }
    
        //Get calibration status
        calibInfo.SetCalibrationTime(m_AzimuthAlignmentData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_AzimuthAlignmentItem[i].m_sensorSerialNumber == "" && m_AzimuthAlignmentItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_AzimuthAlignmentItem[i].m_channel);   
            calibInfo.AddSensor(m_AzimuthAlignmentItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_AzimuthAlignmentItem[i].m_adapterSerialNumber);   
        }
        m_AzimuthAlignmentData.calibInfo = calibInfo.GetInfo();

    break;
	/***************************************************************************/
	/*												AZIMUTH_VERIFICATION_BENCHMARK									 */
	/***************************************************************************/
	case AZIMUTH_VERIFICATION_BENCHMARK:
		m_AzimuthVerificationBenchmarkData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthVerificationBenchmarkData.m_rollExcentricity = MRADIANS_TO_DEGREES( g_AlignerData.RExc );

	    m_AzimuthVerificationBenchmarkData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	    m_AzimuthVerificationBenchmarkData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_AzimuthVerificationBenchmarkData.m_time = m_InParam.Time;

		m_AzimuthVerificationBenchmarkItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_AzimuthVerificationBenchmarkItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );		
        m_AzimuthVerificationBenchmarkItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;

		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
		    m_AzimuthVerificationBenchmarkItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
		    m_AzimuthVerificationBenchmarkItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_AzimuthVerificationBenchmarkItem[0].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
		m_AzimuthVerificationBenchmarkItem[0].m_nominalAzimuthdifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationBenchmarkItem[0].m_measuredAzimuthDifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationBenchmarkItem[0].m_measuredNominalDifference = DB_EMPTY_DOUBLE;//ref
	
	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_AzimuthVerificationBenchmarkItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_AzimuthVerificationBenchmarkItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );

		    m_AzimuthVerificationBenchmarkItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
		
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY )  )
			{
				m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthVerificationBenchmarkItem[i].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] );
            help1 = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] ) - GetNominalAzimuthDegree( g_AlignerData.RefObjNo );      
			help2 =  MRADIANS_TO_DEGREES( pInParam->pACA[i] );      
			help3 = help2 - help1;
            m_AzimuthVerificationBenchmarkItem[i].m_nominalAzimuthdifference = help1;
            m_AzimuthVerificationBenchmarkItem[i].m_measuredAzimuthDifference = help2;
			m_AzimuthVerificationBenchmarkItem[i].m_measuredNominalDifference = help3;
        }
        //Get calibration status
        calibInfo.SetCalibrationTime(m_AzimuthVerificationBenchmarkData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_AzimuthVerificationBenchmarkItem[i].m_sensorSerialNumber == "" && m_AzimuthVerificationBenchmarkItem[i].m_adapterSerialNumber == "")
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
	case AZIMUTH_VERIFICATION_GYROSTAB:
		m_AzimuthVerificationGyrostabData.m_timeConstant = g_AlignerData.TaoAz;
		m_AzimuthVerificationGyrostabData.m_rollExcentricity = MRADIANS_TO_DEGREES( g_AlignerData.RExc );

        m_AzimuthVerificationGyrostabData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;

        m_AzimuthVerificationGyrostabData.m_measuredUnit.LoadString( ( GetMRad() == TRUE ) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_AzimuthVerificationGyrostabData.m_time = m_InParam.Time;

		m_AzimuthVerificationGyrostabItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_AzimuthVerificationGyrostabItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
	    m_AzimuthVerificationGyrostabItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ): DB_EMPTY_STRING;
	
		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
		    m_AzimuthVerificationGyrostabItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
		    m_AzimuthVerificationGyrostabItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_AzimuthVerificationGyrostabItem[0].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
		m_AzimuthVerificationGyrostabItem[0].m_nominalAzimuthdifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationGyrostabItem[0].m_measuredAzimuthDifference = DB_EMPTY_DOUBLE;//ref
		m_AzimuthVerificationGyrostabItem[0].m_measuredNominalDifference = DB_EMPTY_DOUBLE;//ref
	
	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_AzimuthVerificationGyrostabItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_AzimuthVerificationGyrostabItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
	        m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
	
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY )  )
			{
				m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_AzimuthVerificationGyrostabItem[i].m_nominalAzimuth = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] );
            help1 = GetNominalAzimuthDegree( g_AlignerData.ObjNo[i] ) - GetNominalAzimuthDegree( g_AlignerData.RefObjNo );
        	help2 =  MRADIANS_TO_DEGREES( pInParam->pACA[i] );
     		help3 = help2 - help1;
            m_AzimuthVerificationGyrostabItem[i].m_nominalAzimuthdifference = help1;
            m_AzimuthVerificationGyrostabItem[i].m_measuredAzimuthDifference = help2;
			m_AzimuthVerificationGyrostabItem[i].m_measuredNominalDifference = help3;
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_AzimuthVerificationGyrostabData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber == "" && m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_AzimuthVerificationGyrostabItem[i].m_channel);   
            calibInfo.AddSensor(m_AzimuthVerificationGyrostabItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_AzimuthVerificationGyrostabItem[i].m_adapterSerialNumber);   
        }
        m_AzimuthVerificationGyrostabData.calibInfo = calibInfo.GetInfo();

    break;
	/***************************************************************************/
	/*														HORIZON_ABSOLUTE_MODE 											 */
	/***************************************************************************/
  case HORIZON_ABSOLUTE_MODE:
		m_HorizonAbsoluteModeData.m_timeConstant = g_AlignerData.TaoTilt;
	    m_HorizonAbsoluteModeData.m_parallaxCompensation.LoadString( g_AlignerData.UseParallax == TRUE ? IDS_ON : IDS_OFF);
	
	    m_HorizonAbsoluteModeData.m_elevationCompensation.LoadString( g_AlignerData.UseElevError == TRUE ? IDS_ON : IDS_OFF);	
		m_HorizonAbsoluteModeData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
		
		m_HorizonAbsoluteModeData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);		
		m_HorizonAbsoluteModeData.m_time = m_InParam.Time;

		int j;
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
		{
			j = i-1; //no reference at index 0
			m_HorizonAbsoluteModeItem[j].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_HorizonAbsoluteModeItem[j].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );			
		    m_HorizonAbsoluteModeItem[j].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
		
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
			{
				m_HorizonAbsoluteModeItem[j].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_HorizonAbsoluteModeItem[j].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}
			
			m_HorizonAbsoluteModeItem[j].m_roll = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACR[i];
			m_HorizonAbsoluteModeItem[j].m_pitch = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACP[i];
			m_HorizonAbsoluteModeItem[j].m_tilt = g_AlignerData.Kh*g_AlignerData.VecAmp[i];
			m_HorizonAbsoluteModeItem[j].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
			m_HorizonAbsoluteModeItem[j].m_elevation = g_AlignerData.Kh * GetEalAbs( g_AlignerData.ObjNo[i] );
			m_HorizonAbsoluteModeItem[j].m_standardDeviation = g_AlignerData.Kh*g_AlignerData.Sigma[i];
			m_HorizonAbsoluteModeItem[j].m_maximumDeviation = g_AlignerData.Kh*m_pParent->m_MaxSineFitError[i];
			m_HorizonAbsoluteModeItem[j].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
		}

        //Get calibration status
        calibInfo.SetCalibrationTime(m_HorizonAbsoluteModeData.m_time);
        for( int i=0; i<g_AlignerData.NoOfCorr; i++ )
        {
			if(m_HorizonAbsoluteModeItem[i].m_sensorSerialNumber == "" && m_HorizonAbsoluteModeItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_HorizonAbsoluteModeItem[i].m_channel);   
            calibInfo.AddSensor(m_HorizonAbsoluteModeItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_HorizonAbsoluteModeItem[i].m_adapterSerialNumber);   
        }
        m_HorizonAbsoluteModeData.calibInfo = calibInfo.GetInfo();

    break;
	/***************************************************************************/
	/*													AIR_TARGET_RELATIVE_MODE 											 */
	/***************************************************************************/
    case AIR_TARGET_RELATIVE_MODE:
		m_HorizonRelativeModeData.m_timeConstant = g_AlignerData.TaoTilt;
	    m_HorizonRelativeModeData.m_parallaxCompensation.LoadString( g_AlignerData.UseParallax == TRUE ? IDS_ON : IDS_OFF);
	
        m_HorizonRelativeModeData.m_elevationCompensation.LoadString( g_AlignerData.UseElevError == TRUE ? IDS_ON :IDS_OFF);
	    m_HorizonRelativeModeData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	
		m_HorizonRelativeModeData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);		
		m_HorizonRelativeModeData.m_time = m_InParam.Time;

		m_HorizonRelativeModeItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_HorizonRelativeModeItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
        m_HorizonRelativeModeItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;

		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY ) )
		{
			m_HorizonRelativeModeItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
		}
		else
		{
			m_HorizonRelativeModeItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		}

		m_HorizonRelativeModeItem[0].m_roll = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_pitch = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_tilt = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_angle = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_elevation = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_standardDeviation = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_maximumDeviation = DB_EMPTY_DOUBLE;//ref
		m_HorizonRelativeModeItem[0].m_azimuth = DB_EMPTY_DOUBLE;//empty

        for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
		{
			m_HorizonRelativeModeItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_HorizonRelativeModeItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			
	        m_HorizonRelativeModeItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
	
			if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
			{
				m_HorizonRelativeModeItem[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
			}
			else
			{
				m_HorizonRelativeModeItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}
			
			m_HorizonRelativeModeItem[i].m_roll = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACR[i];
			m_HorizonRelativeModeItem[i].m_pitch = pInParam->SignDef*g_AlignerData.Kh*g_AlignerData.ACP[i];
			m_HorizonRelativeModeItem[i].m_tilt = g_AlignerData.Kh*g_AlignerData.VecAmp[i];
			m_HorizonRelativeModeItem[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
			m_HorizonRelativeModeItem[i].m_elevation = g_AlignerData.Kh * g_AlignerData.EalS[i];

			m_HorizonRelativeModeItem[i].m_standardDeviation = g_AlignerData.Kh*g_AlignerData.Sigma[i];
			m_HorizonRelativeModeItem[i].m_maximumDeviation = g_AlignerData.Kh*m_pParent->m_MaxSineFitError[i];
			m_HorizonRelativeModeItem[i].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
		}
        //Get calibration status
        calibInfo.SetCalibrationTime(m_HorizonRelativeModeData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_HorizonRelativeModeItem[i].m_sensorSerialNumber == "" && m_HorizonRelativeModeItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_HorizonRelativeModeItem[i].m_channel);   
            calibInfo.AddSensor(m_HorizonRelativeModeItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_HorizonRelativeModeItem[i].m_adapterSerialNumber);   
        }
        m_HorizonRelativeModeData.calibInfo = calibInfo.GetInfo();

        break;
	/***************************************************************************/
	/*														GYRO_PERFORMANCE_TEST  											 */
	/***************************************************************************/
	case GYRO_PERFORMANCE_TEST:
		m_GyroPerformanceTestData.m_timeConstant = g_AlignerData.TaoGyro;

	    m_GyroPerformanceTestData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;	
	    m_GyroPerformanceTestData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);	
		m_GyroPerformanceTestData.m_time = m_InParam.Time;

		m_GyroPerformanceTestItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_GyroPerformanceTestItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );		
	    m_GyroPerformanceTestItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ):DB_EMPTY_STRING;
	
		if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY )  )
	    {
			m_GyroPerformanceTestItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
        }
        else
        {
			m_GyroPerformanceTestItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
        }

		m_GyroPerformanceTestItem[0].m_roll = DB_EMPTY_DOUBLE;//ref
		m_GyroPerformanceTestItem[0].m_pitch = DB_EMPTY_DOUBLE;//ref
		m_GyroPerformanceTestItem[0].m_tilt = DB_EMPTY_DOUBLE;//ref
		m_GyroPerformanceTestItem[0].m_angle = DB_EMPTY_DOUBLE;//ref

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_GyroPerformanceTestItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_GyroPerformanceTestItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			m_GyroPerformanceTestItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		    m_GyroPerformanceTestItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
		
            m_GyroPerformanceTestItem[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
            m_GyroPerformanceTestItem[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_GyroPerformanceTestItem[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
			m_GyroPerformanceTestItem[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_GyroPerformanceTestData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_GyroPerformanceTestItem[i].m_sensorSerialNumber == "" && m_GyroPerformanceTestItem[i].m_adapterSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_GyroPerformanceTestItem[i].m_channel);   
            calibInfo.AddSensor(m_GyroPerformanceTestItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_GyroPerformanceTestItem[i].m_adapterSerialNumber);   
        }
        m_GyroPerformanceTestData.calibInfo = calibInfo.GetInfo();


        break;
	/***************************************************************************/
	/*														TILT_FLATNESS_TEST													 */
	/***************************************************************************/
  case TILT_FLATNESS_TEST:
		{
			if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
			{
				m_TiltAndFlatnessData.m_reference.LoadString( IDS_REFERENCE_FIXED_PLANE );
				m_TiltAndFlatnessItem[0].m_elevation = DB_EMPTY_DOUBLE;//Empty
			}
			else
			{
				m_TiltAndFlatnessData.m_reference.LoadString( IDS_PLATFORM_RELATIVE_ELEVATION );
				m_TiltAndFlatnessItem[0].m_elevation = DB_EMPTY_DOUBLE;//DB_ELEVATION_IS_REFERENCE;//Ref
			}
			m_TiltAndFlatnessData.m_timeConstant = g_AlignerData.TaoFlat;
			m_TiltAndFlatnessData.m_numberOfMeasurements = m_pParent->m_N;

	        m_TiltAndFlatnessData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	
	        m_TiltAndFlatnessData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
			m_TiltAndFlatnessData.m_time = m_InParam.Time;

			m_TiltAndFlatnessItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
			m_TiltAndFlatnessItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );			
            m_TiltAndFlatnessItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ):DB_EMPTY_STRING;

			if( ( IsGun( g_AlignerData.RefObjNo ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.RefObjNo ) != GUN_ADAP_EMPTY ) )
			{
				m_TiltAndFlatnessItem[0].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.RefObjNo );
			}
			else
			{
				m_TiltAndFlatnessItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
			}

			m_TiltAndFlatnessItemErr[0].m_azimuth = DB_EMPTY_DOUBLE;//empty
			m_TiltAndFlatnessItemErr[0].m_error = DB_EMPTY_DOUBLE;//empty

			m_TiltAndFlatnessItem[0].m_roll = DB_EMPTY_DOUBLE;//ref
			m_TiltAndFlatnessItem[0].m_pitch = DB_EMPTY_DOUBLE;//ref
			m_TiltAndFlatnessItem[0].m_tilt = DB_EMPTY_DOUBLE;//ref
			m_TiltAndFlatnessItem[0].m_angle = DB_EMPTY_DOUBLE;//ref
			m_TiltAndFlatnessItem[0].m_standardDeviation = DB_EMPTY_DOUBLE;//empty
			m_TiltAndFlatnessItem[0].m_maximumDeviation = DB_EMPTY_DOUBLE;//empty
			m_TiltAndFlatnessItem[0].m_azimuth = DB_EMPTY_DOUBLE;//empty
	
			for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
			{
				m_TiltAndFlatnessItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
				m_TiltAndFlatnessItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );
			    m_TiltAndFlatnessItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ): DB_EMPTY_STRING;
			
				if( ( IsGun( g_AlignerData.ObjNo[i] ) == TRUE ) && ( GetGunAdapterNumber( g_AlignerData.ObjNo[i] ) != GUN_ADAP_EMPTY ) )
				{
					m_TiltAndFlatnessItem[i].m_adapterSerialNumber = GetGunAdapterNumber( g_AlignerData.ObjNo[i] );
				}
				else
				{
					m_TiltAndFlatnessItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
				}
				
				m_TiltAndFlatnessItem[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
				m_TiltAndFlatnessItem[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
				m_TiltAndFlatnessItem[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
				m_TiltAndFlatnessItem[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );
				m_TiltAndFlatnessItem[i].m_elevation = g_AlignerData.Kh * m_pParent->m_MeanSineFit[i];

				if( g_AlignerData.Found == TRUE )
				{
					m_TiltAndFlatnessItem[i].m_standardDeviation = g_AlignerData.Sigma[i];
					m_TiltAndFlatnessItem[i].m_maximumDeviation = m_pParent->m_MaxSineFitError[i];
				}
				else
				{
					m_TiltAndFlatnessItem[i].m_standardDeviation = g_AlignerData.Kh * g_AlignerData.Sigma[i];
					m_TiltAndFlatnessItem[i].m_maximumDeviation = g_AlignerData.Kh * m_pParent->m_MaxSineFitError[i];
				}
				//***  Azimuth is always in degrees and should not be scaled by .Kh
			    m_TiltAndFlatnessItem[i].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[i];
			}

            //Get calibration status
            calibInfo.SetCalibrationTime(m_TiltAndFlatnessData.m_time);
            for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
            {
				if(m_TiltAndFlatnessItem[i].m_sensorSerialNumber == "" && m_TiltAndFlatnessItem[i].m_adapterSerialNumber == "")
					continue;

				calibInfo.AddChannel(m_TiltAndFlatnessItem[i].m_channel);   
                calibInfo.AddSensor(m_TiltAndFlatnessItem[i].m_sensorSerialNumber);   
                calibInfo.AddAdapter(m_TiltAndFlatnessItem[i].m_adapterSerialNumber);   
            }
            m_TiltAndFlatnessData.calibInfo = calibInfo.GetInfo();


			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
			{
				for( int j=1; j<=m_pParent->m_N; j++ )
				{
					m_TiltAndFlatnessItemErr[i*m_pParent->m_N + j].m_azimuth = m_pParent->m_XAngle[j] ;
					m_TiltAndFlatnessItemErr[i*m_pParent->m_N + j].m_error = m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
				}
			}

			//Extended Item
			double deviation[SIZE_OF_X_ARRAY][SIZE_OF_ARRAYS] ;//will be copied to m_deviationJ3...m_deviationJ9 !!!!!

			for( int J=1; J<=m_pParent->m_N; J++ )
			{
				m_TiltAndFlatnessExtItem[J-1].m_azimuthAngle = m_pParent->m_XAngle[J];

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
			}	
		}
		break;
	/***************************************************************************/
	/*											TILT_FLATNESS_FOUNDATION_TEST											 */
	/***************************************************************************/
	case TILT_FLATNESS_FOUNDATION_TEST:
		{
			/****************/
			/***   Data   ***/
			/****************/
			if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
			{
				m_TiltAndFlatnessFoData.m_reference.LoadString( IDS_REFERENCE_FIXED_PLANE );
				m_TiltAndFlatnessFoItem[0].m_elevation1 = DB_EMPTY_DOUBLE;//Empty
			}
			else
			{
				m_TiltAndFlatnessFoData.m_reference.LoadString( IDS_PLATFORM_RELATIVE_ELEVATION );
				m_TiltAndFlatnessFoItem[0].m_elevation1 = DB_EMPTY_DOUBLE;//DB_ELEVATION_IS_REFERENCE;//Ref
			}
			m_TiltAndFlatnessFoData.m_timeConstant = g_AlignerData.TaoFlat;
			m_TiltAndFlatnessFoData.m_numberOfMeasurements = m_pParent->m_N;

	        m_TiltAndFlatnessFoData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	
	        m_TiltAndFlatnessFoData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
			m_TiltAndFlatnessFoData.m_time = m_InParam.Time;

			m_TiltAndFlatnessFoData.m_IndexArmLength = GetIndexArmLength(1) ;
            

			/****************/
			/***   Item   ***/
			/****************/

			//	Ref and meas object in only one record
			m_TiltAndFlatnessFoItem[0].m_refstation = GetUnitTypeDescription( g_AlignerData.RefObjNo );
			m_TiltAndFlatnessFoItem[0].m_refchannel = GetChannelName( g_AlignerData.RefObjNo );
			m_TiltAndFlatnessFoItem[0].m_refsensorSerialNumber = GetUnitTypeSerialNumber( g_AlignerData.RefObjNo );
			m_TiltAndFlatnessFoItem[0].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[1] );
			m_TiltAndFlatnessFoItem[0].m_channel = GetChannelName( g_AlignerData.ObjNo[1] );
			m_TiltAndFlatnessFoItem[0].m_sensorSerialNumber = GetUnitTypeSerialNumber( g_AlignerData.ObjNo[1] );

         	calibInfo.SetCalibrationTime(m_TiltAndFlatnessFoData.m_time);
            
			if(m_TiltAndFlatnessFoItem[0].m_refsensorSerialNumber != "")
			{
				calibInfo.AddSensor(m_TiltAndFlatnessFoItem[0].m_refsensorSerialNumber);
				calibInfo.AddChannel(m_TiltAndFlatnessFoItem[0].m_refchannel);
			}
            
			if(m_TiltAndFlatnessFoItem[0].m_sensorSerialNumber != "")
			{
				calibInfo.AddSensor(m_TiltAndFlatnessFoItem[0].m_sensorSerialNumber);
				calibInfo.AddChannel(m_TiltAndFlatnessFoItem[0].m_channel);            
			}
                        
            m_TiltAndFlatnessFoData.calibInfo = calibInfo.GetInfo();
			
          	m_TiltAndFlatnessFoItemErr[0].m_azimuth = DB_EMPTY_DOUBLE;//empty
			m_TiltAndFlatnessFoItemErr[0].m_error1 = DB_EMPTY_DOUBLE;//empty
            m_TiltAndFlatnessFoItemErr[0].m_error2 = DB_EMPTY_DOUBLE;//empty

			//	Ref and meas object in only one record
			m_TiltAndFlatnessFoItem[0].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[1];
			m_TiltAndFlatnessFoItem[0].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[1];
			m_TiltAndFlatnessFoItem[0].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[1];
			m_TiltAndFlatnessFoItem[0].m_angle = AdjustDegAngle( g_AlignerData.VecArg[1], pInParam->AngleRange0ToPlusMinus180, 1 );
			m_TiltAndFlatnessFoItem[0].m_elevation1 = g_AlignerData.Kh * m_pParent->m_MeanSineFit[1];
            m_TiltAndFlatnessFoItem[0].m_elevation2 = g_AlignerData.Kh * m_pParent->m_MeanSineFit[2];

			m_TiltAndFlatnessFoItem[0].m_standardDeviation = g_AlignerData.Sigma[1];
			m_TiltAndFlatnessFoItem[0].m_bottomError = m_pParent->m_HBottom;
            m_TiltAndFlatnessFoItem[0].m_maximumDeviation = m_pParent->m_MaxSineFitError[1];
	
			//***  Azimuth is in deg and should not be scaled with arcmin factor (.Kh)
			m_TiltAndFlatnessFoItem[0].m_azimuth = m_pParent->m_XAngleForMaxSineFitError[1];

			BOOL measureWarping = (GetIndexArmLength(2) != 0);
            
            m_TiltAndFlatnessFoItem[0].m_IndexArmLength = GetIndexArmLength(1) ;
            m_TiltAndFlatnessFoItem[0].m_IndexArm2Length = GetIndexArmLength(2) ;


		    ZeroMemory(&(m_TiltAndFlatnessFoItemErr), sizeof(m_TiltAndFlatnessFoItemErr)); 
			for( int j=1; j<=m_pParent->m_N; j++ )
			{
				m_TiltAndFlatnessFoItemErr[j].m_azimuth = m_pParent->m_XAngle[j] ;
				m_TiltAndFlatnessFoItemErr[j].m_error1 = m_pParent->m_SineFitError[1][j] ;                
                if(measureWarping)
                {
                    m_TiltAndFlatnessFoItemErr[j].m_error2 = m_pParent->m_SineFitError[2][j] ;
                    m_TiltAndFlatnessFoItemErr[j].m_dh = m_TiltAndFlatnessFoItemErr[j].m_error2 - m_TiltAndFlatnessFoItemErr[j].m_error1;
                }
            }
		}
		break;
	/***************************************************************************/
	/*															COMMON_FLAT_TEST     											 */
	/***************************************************************************/
  case COMMON_FLAT_TEST:
		m_CommonFlatTestData.m_timeConstant = g_AlignerData.TaoTilt;

		m_CommonFlatTestData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	    m_CommonFlatTestData.m_measuredUnit.LoadString( ( GetMRad() == TRUE ) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_CommonFlatTestData.m_time = m_InParam.Time;

		m_CommonFlatTestItem[0].m_station = GetUnitTypeDescription( g_AlignerData.RefObjNo );
		m_CommonFlatTestItem[0].m_channel = GetChannelName( g_AlignerData.RefObjNo );
        m_CommonFlatTestItem[0].m_sensorSerialNumber = IsSensor( g_AlignerData.RefObjNo ) ? GetUnitTypeSerialNumber( g_AlignerData.RefObjNo ) : DB_EMPTY_STRING;

        m_CommonFlatTestItem[0].m_roll = DB_EMPTY_DOUBLE;
		m_CommonFlatTestItem[0].m_pitch = DB_EMPTY_DOUBLE;

		m_CommonFlatTestItem[0].m_parallellBias = g_AlignerData.Kh * GetEal( g_AlignerData.RefObjNo );
		m_CommonFlatTestItem[0].m_perpendicularBias = g_AlignerData.Kh * GetEac( g_AlignerData.RefObjNo );
		m_CommonFlatTestItem[0].m_temperature = m_InParam.pTemperature[g_AlignerData.RefObjNo];

	    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
			m_CommonFlatTestItem[i].m_station = GetUnitTypeDescription( g_AlignerData.ObjNo[i] );
			m_CommonFlatTestItem[i].m_channel = GetChannelName( g_AlignerData.ObjNo[i] );			
	        m_CommonFlatTestItem[i].m_sensorSerialNumber = IsSensor( g_AlignerData.ObjNo[i] ) ? GetUnitTypeSerialNumber( g_AlignerData.ObjNo[i] ) : DB_EMPTY_STRING;
	
            m_CommonFlatTestItem[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
            m_CommonFlatTestItem[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_CommonFlatTestItem[i].m_parallellBias = g_AlignerData.Kh * GetEal( g_AlignerData.ObjNo[i] );
			m_CommonFlatTestItem[i].m_perpendicularBias = g_AlignerData.Kh * GetEac( g_AlignerData.ObjNo[i] );
			m_CommonFlatTestItem[i].m_temperature = m_InParam.pTemperature[g_AlignerData.ObjNo[i]];
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_CommonFlatTestData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
			if(m_CommonFlatTestItem[i].m_sensorSerialNumber == "")
				continue;

			calibInfo.AddChannel(m_CommonFlatTestItem[i].m_channel);   
            calibInfo.AddSensor(m_CommonFlatTestItem[i].m_sensorSerialNumber);               
        }
        m_CommonFlatTestData.calibInfo = calibInfo.GetInfo();

        break;
		/***************************************************************************/
	/*															SENSOR_VALIDATION_TEST     											 */
	/***************************************************************************/
  case SENSOR_VALIDATION_TEST:
	  m_SensorValidationData.m_timeConstant = g_AlignerData.TaoTilt;

	  m_SensorValidationData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
	  m_SensorValidationData.m_measuredUnit.LoadString((GetMRad() == TRUE) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
	  m_SensorValidationData.m_time = m_InParam.Time;

	  m_SensorValidationItem[0].m_station = GetUnitTypeDescription(g_AlignerData.RefObjNo);
	  m_SensorValidationItem[0].m_channel = GetChannelName(g_AlignerData.RefObjNo);
	  m_SensorValidationItem[0].m_sensorSerialNumber = IsSensor(g_AlignerData.RefObjNo) ? GetUnitTypeSerialNumber(g_AlignerData.RefObjNo) : DB_EMPTY_STRING;

	  m_SensorValidationItem[0].m_rollSc = DB_EMPTY_DOUBLE;
	  m_SensorValidationItem[0].m_pitchSc = DB_EMPTY_DOUBLE;

	  m_SensorValidationItem[0].m_rollAzErr = DB_EMPTY_DOUBLE;
	  m_SensorValidationItem[0].m_pitchAzErr = DB_EMPTY_DOUBLE;
	  m_SensorValidationItem[0].m_temperature = m_InParam.pTemperature[0];// g_AlignerData.RefObjNo];

	  m_SensorValidationItem[1].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[1]);
	  m_SensorValidationItem[1].m_channel = GetChannelName(g_AlignerData.ObjNo[1]);
	  m_SensorValidationItem[1].m_sensorSerialNumber = IsSensor(g_AlignerData.ObjNo[1]) ? GetUnitTypeSerialNumber(g_AlignerData.ObjNo[1]) : DB_EMPTY_STRING;

	  m_SensorValidationItem[1].m_rollSc = g_AlignerData.ACR[1];
	  m_SensorValidationItem[1].m_pitchSc = g_AlignerData.ACP[1];
	  m_SensorValidationItem[1].m_rollAzErr = g_AlignerData.Eac[1];
	  m_SensorValidationItem[1].m_pitchAzErr = g_AlignerData.Eal[1];
	  m_SensorValidationItem[1].m_temperature = m_InParam.pTemperature[1];// g_AlignerData.ObjNo[1]];

	 /* for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
	  {
		  m_SensorValidationItem[i].m_station = GetUnitTypeDescription(g_AlignerData.ObjNo[i]);
		  m_SensorValidationItem[i].m_channel = GetChannelName(g_AlignerData.ObjNo[i]);
		  m_SensorValidationItem[i].m_sensorSerialNumber = IsSensor(g_AlignerData.ObjNo[i]) ? GetUnitTypeSerialNumber(g_AlignerData.ObjNo[i]) : DB_EMPTY_STRING;

		  m_SensorValidationItem[i].m_rollSc =  g_AlignerData.ACR[i];
		  m_SensorValidationItem[i].m_pitchSc =  g_AlignerData.ACP[i];
		  m_SensorValidationItem[i].m_rollAzErr = GetEal(g_AlignerData.ObjNo[i]);
		  m_SensorValidationItem[i].m_pitchAzErr= g_AlignerData.Kh * GetEac(g_AlignerData.ObjNo[i]);
		  m_SensorValidationItem[i].m_temperature = m_InParam.pTemperature[g_AlignerData.ObjNo[i]];
	  }*/

	  //Get calibration status
	  calibInfo.SetCalibrationTime(m_SensorValidationData.m_time);
	  for (int i = 0; i <= g_AlignerData.NoOfCorr; i++)
	  {
		  if (m_SensorValidationItem[i].m_sensorSerialNumber == "")
			  continue;

		  calibInfo.AddChannel(m_SensorValidationItem[i].m_channel);
		  calibInfo.AddSensor(m_SensorValidationItem[i].m_sensorSerialNumber);
	  }
	  m_SensorValidationData.calibInfo = calibInfo.GetInfo();

	  break;
	/***************************************************************************/
	/*																LIVE_GRAPH		     											 */
	/***************************************************************************/
	case LIVE_GRAPH:
    {
        m_LiveGraphData.m_timeConstant = pInParam->timeConstant;//g_AlignerData.TaoTilt;
		m_LiveGraphData.m_samplingRate = pInParam->SamplingRate;
		m_LiveGraphData.m_comment = m_InParam.Comment.GetLength() == 0 ? DB_EMPTY_STRING : m_InParam.Comment;
		
        m_LiveGraphData.m_measuredUnit.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		m_LiveGraphData.m_time = m_InParam.Time;

		//No reference for live graph
		m_LiveGraphItem[0].m_station = _T("");
		m_LiveGraphItem[0].m_channel = _T("");
		m_LiveGraphItem[0].m_sensorSerialNumber = DB_EMPTY_STRING;//empty
        m_LiveGraphItem[0].m_adapterSerialNumber = DB_EMPTY_STRING;//empty
		m_LiveGraphItem[0].m_roll = DB_EMPTY_DOUBLE;//empty
		m_LiveGraphItem[0].m_pitch = DB_EMPTY_DOUBLE;//empty
		m_LiveGraphItem[0].m_tilt = DB_EMPTY_DOUBLE;//empty
		m_LiveGraphItem[0].m_angle = DB_EMPTY_DOUBLE;//empty
		m_LiveGraphItem[0].m_temperature = DB_EMPTY_DOUBLE;//empty

        int i=1;
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++,i++)
        {
            int id = iter->second.id;
			m_LiveGraphItem[i].m_station = GetUnitTypeDescription( id);//g_AlignerData.ObjNo[i] );
			m_LiveGraphItem[i].m_channel = GetChannelName( id);//g_AlignerData.ObjNo[i] );
			
			m_LiveGraphItem[i].m_sensorSerialNumber = IsSensor(id) ? GetUnitTypeSerialNumber(id) : DB_EMPTY_STRING;
			
			if( ( IsGun( id) == TRUE ) && ( GetGunAdapterNumber( id) != GUN_ADAP_EMPTY )  )
			{
				m_LiveGraphItem[i].m_adapterSerialNumber = GetGunAdapterNumber( id);
			}
			else
			{
				m_LiveGraphItem[i].m_adapterSerialNumber = DB_EMPTY_STRING;
			}

            m_LiveGraphItem[i].m_roll = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACR[i];
            m_LiveGraphItem[i].m_pitch = pInParam->SignDef * g_AlignerData.Kh * g_AlignerData.ACP[i];
			m_LiveGraphItem[i].m_tilt = g_AlignerData.Kh * g_AlignerData.VecAmp[i];
			m_LiveGraphItem[i].m_angle = AdjustDegAngle( g_AlignerData.VecArg[i], pInParam->AngleRange0ToPlusMinus180, 1 );

			m_LiveGraphItem[i].m_temperature = IsSensor(id) ? pInParam->pTemperature[i] : DB_EMPTY_DOUBLE;            
        }

        //Get calibration status
        calibInfo.SetCalibrationTime(m_LiveGraphData.m_time);
        for( int i=0; i<=g_AlignerData.NoOfCorr; i++ )
        {
            if(m_LiveGraphItem[i].m_sensorSerialNumber == "" && m_LiveGraphItem[i].m_adapterSerialNumber == "")
				continue;
			
			calibInfo.AddChannel(m_LiveGraphItem[i].m_channel);   
            calibInfo.AddSensor(m_LiveGraphItem[i].m_sensorSerialNumber);   
            calibInfo.AddAdapter(m_LiveGraphItem[i].m_adapterSerialNumber);   
        }
        m_LiveGraphData.calibInfo = calibInfo.GetInfo();

    }
	break;
	/***************************************************************************/
	/*															LIVE_DATA_A202	     											 */
	/***************************************************************************/
  case LIVE_DATA_A202:
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
        break;
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
		case TILT_ALIGNMENT:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_TiltAlignment.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_TiltAlignment.m_comment = m_InParam.Comment;
			}

			if( !TiltAlignment::AddData( m_TiltAlignment ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_TiltAlignmentChannel[i].m_station.GetLength() == 0 ) 
				{
					m_TiltAlignmentChannel[i].m_station = DB_EMPTY_STRING;
				}

				if( !TiltAlignment::AddChannel( m_TiltAlignmentChannel[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case AZIMUTH_ALIGN:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_AzimuthAlignmentData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_AzimuthAlignmentData.m_comment = m_InParam.Comment;
			}

			if( !AzimuthAlignmentErrorsHistory::AddData( m_AzimuthAlignmentData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_AzimuthAlignmentItem[i].m_station.GetLength() == 0 ) 
				{
					m_AzimuthAlignmentItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !AzimuthAlignmentErrorsHistory::AddItem( m_AzimuthAlignmentItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case AZIMUTH_VERIFICATION_BENCHMARK:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_AzimuthVerificationBenchmarkData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_AzimuthVerificationBenchmarkData.m_comment = m_InParam.Comment;
			}

			if( !AzimuthVerificationBenchmarkHistory::AddData( m_AzimuthVerificationBenchmarkData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_AzimuthVerificationBenchmarkItem[i].m_station.GetLength() == 0 ) 
				{
					m_AzimuthVerificationBenchmarkItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !AzimuthVerificationBenchmarkHistory::AddItem( m_AzimuthVerificationBenchmarkItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case AZIMUTH_VERIFICATION_GYROSTAB:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_AzimuthVerificationGyrostabData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_AzimuthVerificationGyrostabData.m_comment = m_InParam.Comment;
			}

			if( !AzimuthVerificationGyrostabilityHistory::AddData( m_AzimuthVerificationGyrostabData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_AzimuthVerificationGyrostabItem[i].m_station.GetLength() == 0 ) 
				{
					m_AzimuthVerificationGyrostabItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !AzimuthVerificationGyrostabilityHistory::AddItem( m_AzimuthVerificationGyrostabItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case HORIZON_ABSOLUTE_MODE:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_HorizonAbsoluteModeData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_HorizonAbsoluteModeData.m_comment = m_InParam.Comment;
			}

			if( !HorizonAbsoluteModeHistory::AddData( m_HorizonAbsoluteModeData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<g_AlignerData.NoOfCorr; i++ ) // no reference, index 0 = first channel to test
			{
				if( m_HorizonAbsoluteModeItem[i].m_station.GetLength() == 0 ) 
				{
					m_HorizonAbsoluteModeItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !HorizonAbsoluteModeHistory::AddItem( m_HorizonAbsoluteModeItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case AIR_TARGET_RELATIVE_MODE:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_HorizonRelativeModeData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_HorizonRelativeModeData.m_comment = m_InParam.Comment;
			}

			if( !HorizonRelativeModeHistory::AddData( m_HorizonRelativeModeData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_HorizonRelativeModeItem[i].m_station.GetLength() == 0 ) 
				{
					m_HorizonRelativeModeItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !HorizonRelativeModeHistory::AddItem( m_HorizonRelativeModeItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case GYRO_PERFORMANCE_TEST:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_GyroPerformanceTestData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_GyroPerformanceTestData.m_comment = m_InParam.Comment;
			}

			if( !GyroPerformanceTestHistory::AddData( m_GyroPerformanceTestData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_GyroPerformanceTestItem[i].m_station.GetLength() == 0 ) 
				{
					m_GyroPerformanceTestItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !GyroPerformanceTestHistory::AddItem( m_GyroPerformanceTestItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case TILT_FLATNESS_TEST:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_TiltAndFlatnessData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_TiltAndFlatnessData.m_comment = m_InParam.Comment;
			}

			if( !TiltAndFlatnessHistory::AddData( m_TiltAndFlatnessData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_TiltAndFlatnessItem[i].m_station.GetLength() == 0 ) 
				{
					m_TiltAndFlatnessItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !TiltAndFlatnessHistory::AddItem( m_TiltAndFlatnessItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
                for( int j=1; j<=m_pParent->m_N; j++ )
                {				
                    if( !TiltAndFlatnessHistory::AddItemErr( m_TiltAndFlatnessItemErr[m_pParent->m_N*i + j] ) )
                    {
                        ASSERT(0); // This is a "badass" error.
                    }			
                }
            }
			
			

			for( int extI=0; extI<m_TiltAndFlatnessData.m_numberOfMeasurements; extI++ )
			{
				if( !TiltAndFlatnessHistory::AddExtItem( m_TiltAndFlatnessExtItem[extI] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}

			break;
		case TILT_FLATNESS_FOUNDATION_TEST:
			/****************/
			/***   Data   ***/
			/****************/
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_TiltAndFlatnessFoData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_TiltAndFlatnessFoData.m_comment = m_InParam.Comment;
			}

			if( !TiltAndFlatnessFoHistory::AddData( m_TiltAndFlatnessFoData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			/****************/
			/***   Item   ***/
			/****************/
			if( m_TiltAndFlatnessFoItem[0].m_station.GetLength() == 0 ) 
			{
				m_TiltAndFlatnessFoItem[0].m_station = DB_EMPTY_STRING;
			}
			if( m_TiltAndFlatnessFoItem[0].m_refstation.GetLength() == 0 ) 
			{
				m_TiltAndFlatnessFoItem[0].m_refstation = DB_EMPTY_STRING;
			}

			if( !TiltAndFlatnessFoHistory::AddItem( m_TiltAndFlatnessFoItem[0] ) )
			{
				ASSERT(0); // This is a "badass" error.
			}

			/*******************/
			/***   ItemErr   ***/
			/*******************/
			//for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			//{
				//if( m_TiltAndFlatnessFoItem[i].m_station.GetLength() == 0 ) 
				//{
				//	m_TiltAndFlatnessFoItem[i].m_station = DB_EMPTY_STRING;
				//}

				//if( !TiltAndFlatnessFoHistory::AddItem( m_TiltAndFlatnessFoItem[i] ) )
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
					//LONG oldId = TiltAndFlatnessFoHistory::GetLastID();
					//LastCounter lastID ;
					//GUARD(::OpenTable( lastID )) ;
					//GUARD(lastID.MoveFirst()) ;
					//TiltAndFlatnessFoHistory::SetLastID( lastID.m_id);
				//if (i == 0)
				//{
				//	if( !TiltAndFlatnessFoHistory::AddItemErr( m_TiltAndFlatnessFoItemErr[0] ) )	// index 0 = reference
				//	{
				//		ASSERT(0); // This is a "badass" error.
				//	}
				//}
				//else
				//{
					//LONG oldId = TiltAndFlatnessFoHistory::GetLastID();
					//LastCounter lastID ;
					//GUARD(::OpenTable( lastID )) ;
					//GUARD(lastID.MoveFirst()) ;
					//TiltAndFlatnessFoHistory::SetLastID( lastID.m_id);

					for( int j=1; j<=m_pParent->m_N; j++ )
					{
//						if( !TiltAndFlatnessFoHistory::AddItemErr( m_TiltAndFlatnessFoItemErr[(i-1)*m_pParent->m_N + j] ) )
						if( !TiltAndFlatnessFoHistory::AddItemErr( m_TiltAndFlatnessFoItemErr[j] ) )
						{
							ASSERT(0); // This is a "badass" error.
						}
					//}
					//TiltAndFlatnessFoHistory::SetLastID( oldId);
					}
				//}	// else
				//}
				//if( !TiltAndFlatnessFoHistory::AddItemErr( m_TiltAndFlatnessFoItemErr[i] ) )
				//{
				//	ASSERT(0); // This is a "badass" error.
				//}
			//}	// for( int i=0; ...

			for( int extI=0; extI<m_TiltAndFlatnessFoData.m_numberOfMeasurements; extI++ )
			{
				if( !TiltAndFlatnessFoHistory::AddExtItem( m_TiltAndFlatnessFoExtItem[extI] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
				//if( !TiltAndFlatnessFoHistory::AddItemErr( m_TiltAndFlatnessFoItemErr[extI] ) )
				//{
				//	ASSERT(0); // This is a "badass" error.
				//}
			}

			break;
		case COMMON_FLAT_TEST:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_CommonFlatTestData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_CommonFlatTestData.m_comment = m_InParam.Comment;
			}

			if( !CommonFlatTiltHistory::AddData( m_CommonFlatTestData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=0; i<=g_AlignerData.NoOfCorr; i++ ) // index 0 = reference
			{
				if( m_CommonFlatTestItem[i].m_station.GetLength() == 0 ) 
				{
					m_CommonFlatTestItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !CommonFlatTiltHistory::AddItem( m_CommonFlatTestItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;
		case SENSOR_VALIDATION_TEST:
			if (m_InParam.Comment.GetLength() == 0)
			{
				m_SensorValidationData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_SensorValidationData.m_comment = m_InParam.Comment;
			}

			if (!SensorValidationHistory::AddData(m_SensorValidationData))
			{
				ASSERT(0); // This is a "badass" error.
			}

			for (int i = 0; i <= g_AlignerData.NoOfCorr; i++) // index 0 = reference
			{
				if (m_SensorValidationItem[i].m_station.GetLength() == 0)
				{
					m_SensorValidationItem[i].m_station = DB_EMPTY_STRING;
				}

				if (!SensorValidationHistory::AddItem(m_SensorValidationItem[i]))
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
			break;

		case LIVE_GRAPH:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_LiveGraphData.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_LiveGraphData.m_comment = m_InParam.Comment;
			}

			if( !LiveGraphErrorsHistory::AddData( m_LiveGraphData ) )
			{
				ASSERT(0) ; // This is a "badass" error.
			}

			for( int i=1; i<=g_AlignerData.liveGraphMap.size(); i++ ) // index 0 = reference, but live graph has no reference
			{
				if( m_LiveGraphItem[i].m_station.GetLength() == 0 ) 
				{
					m_LiveGraphItem[i].m_station = DB_EMPTY_STRING;
				}

				if( !LiveGraphErrorsHistory::AddItem( m_LiveGraphItem[i] ) )
				{
					ASSERT(0); // This is a "badass" error.
				}
			}
            //AddGraph(m_InParam.graphFileName, m_InParam.includeGraph);

      break;
    case LIVE_DATA_A202:
			if( m_InParam.Comment.GetLength() == 0 )
			{
				m_LiveDataA202Data.m_comment = DB_EMPTY_STRING;//empty
			}
			else
			{
				m_LiveDataA202Data.m_comment = m_InParam.Comment;
			}

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
      break;
		default:
			m_InParam.SavedToDataBase = FALSE;
			break;
		}
        
        //Store graph entries in DB.
        if(m_InParam.pFileGraphManager != NULL)
        {            
            for( int i = 0; i < m_InParam.pFileGraphManager->GetNoOfSavedFiles(); i++ )
            {                        
                if( TRUE != AddGraph( m_InParam.pFileGraphManager->GetFileName( i ), TRUE)) //m_InParam->pFileGraphManager.GetIncludeToResultTable( i ) ) )
                {
                    //result = FALSE;
                }           
            }
        }
    }

   

  return TRUE ;
}

BOOL CResultTable::DeleteLast( void )
{
	BOOL result = TRUE;
#ifdef BUILD_ALIGNER_308
	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		result = TiltAlignment::DeleteLast();
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
		result = TiltAndFlatnessFoHistory::DeleteLast();
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
		m_reportHistoryMainID = -1;
	}
	return( result );
}


BOOL CResultTable::OpenReport( BOOL SaveToDB )
{
	BOOL result = TRUE;
#ifdef BUILD_ALIGNER_308
	if( SaveToDB == TRUE )
	{
		result = SaveToDataBase();
		Sleep(1000);
	}

	if( CreateEmptyReport() == FALSE )
	{
		return( FALSE );
	}
	
	result =  ShowPresentDialog( m_InParam.ShowPresenetDialog );
#endif
	return( result );
}

BOOL CResultTable::CreateEmptyReport( void )
{
#ifdef BUILD_ALIGNER_308
    switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		m_reportHistoryMainID = TiltAlignment::GetMainID();
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
		m_reportHistoryMainID = TiltAndFlatnessFoHistory::GetMainID();
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
		result = TiltAlignment::SetComment( m_InParam.Comment );
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
		result = TiltAndFlatnessFoHistory::SetComment( m_InParam.Comment );
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
		TiltAlignment::ResetMainID();
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
		TiltAndFlatnessFoHistory::ResetMainID();
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

long CResultTable::GetMainID( void )
{
    long id=0;
#ifdef BUILD_ALIGNER_308    
	switch( m_InParam.Version )
	{
	case TILT_ALIGNMENT:
		id = TiltAlignment::GetMainID();
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
		id = TiltAndFlatnessFoHistory::GetMainID();
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

BOOL CResultTable::AddGraph( CString fileName, BOOL includeToResultTable )
{
BOOL result=TRUE;
#ifdef BUILD_ALIGNER_308    
	CloseReport();

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
		result = TiltAndFlatnessFoHistory::AddGraph( fileName, includeToResultTable );
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

BOOL CResultTable::ShowReport( BOOL CloseFirst )
{
	BOOL result = FALSE;
#ifdef BUILD_ALIGNER_308

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
#endif
	return result;
}

BOOL CResultTable::OpenCalibrationReport()
{
#ifdef BUILD_ALIGNER_308
    CalibrationReportReport::Create() ;
#endif
    return TRUE;
}

BOOL CResultTable::OpenMainReport()
{
#ifdef BUILD_ALIGNER_308
    if (theApp.IsAligner202Enabled())
    {
        MainReportA202Report::Create() ;
    }
    else
    {
        MainReportReport::Create() ;
    }
#endif    
    return TRUE;
}

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

	m_hReportWindow = NULL;
#endif
	return( TRUE );
}
