// $Id: GainCalibrationPage6.cpp,v 1.4 2012/08/22 13:09:03 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "CalibrationSheet.h"
#include "CreatePolynomial.h"
#include "GainAzimuthCalibration.h"
#include "GainCalibrationPage6.h"
#include "resulttable.h"

#define MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT    2.0 //[mrad]
#define MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT   2.0 //[mrad]

IMPLEMENT_DYNAMIC(GainCalibrationPage6, CPropertyPage)

GainCalibrationPage6::GainCalibrationPage6( void ) : CPropertyPage(GainCalibrationPage6::IDD)
{
	m_calibUpdated = FALSE;
	m_pResultTable = NULL;
  // Empty
}

GainCalibrationPage6::~GainCalibrationPage6( void )
{
  // Empty
}

void GainCalibrationPage6::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;  
}

BEGIN_MESSAGE_MAP(GainCalibrationPage6, CPropertyPage)
	ON_BN_CLICKED(IDC_SHOW_RESULT_TABLE, &GainCalibrationPage6::OnBnClickedShowResultTable)
END_MESSAGE_MAP()

BOOL GainCalibrationPage6::OnSetActive( void )
{
 
	if (m_pResultTable == NULL)
	{
		m_pResultTable = new CResultTable(this);
	}

	OnInitGrid();
	InitResultTable();

	GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> PlotConverted( 1 ) ;
    pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
    if (!pSheet -> VerifyStability( 2, 3, FALSE))
    {
        ::AfxMessageBox( _T("The calibration fixture has moved more than 1 mrad between measurements\nPress the back button to redo the last measurment") ) ;
        pSheet -> SetWizardButtons( PSWIZB_BACK ) ;
    }  
    return __super::OnSetActive() ;
}

void GainCalibrationPage6::InitResultTable(void)
{
	m_pResultTable->m_InParam.Version = MeasType::MT_SensorValidation;

	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(m_pResultTable->m_InParam.Time);
	 
	m_pResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
	m_pResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
	m_pResultTable->m_InParam.pACA = NULL;
	m_pResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pResultTable->m_InParam.SavedToDataBase = FALSE;

	m_pResultTable->m_InParam.SamplingRate = 0;
	m_pResultTable->m_InParam.pFileGraphManager = NULL;// &m_pParent->m_GraphFileManager;
}

BOOL GainCalibrationPage6::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage6::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2) ;
}

/*
BEGIN_EVENTSINK_MAP(GainCalibrationPage6, CPropertyPage)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(GainCalibrationPage6, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()
*/
/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than save column cells.
 */

static CString FormatDouble( double value )
{
  CString text ;
  text.Format( "%8.5f", value ) ;
  return text ;
}



static void SaveCalibrationData( Gain6CalibrationResult &result, CString serialNumber )
{
  if (result.m_saveResult)
  {
    DBInterface::Instance()->InsertSensorData("SensorPitchGainData", DAU::GetDAU().GetSerialNumber(), serialNumber, SysSetup->GetOperatorName(), result.m_pitchTemperature, result.m_sensorPitchGain);
    DBInterface::Instance()->InsertSensorData("SensorRollGainData", DAU::GetDAU().GetSerialNumber(), serialNumber, SysSetup->GetOperatorName(), result.m_rollTemperature, result.m_sensorRollGain);
    //Note: temperature correct, shall be swapped.
    DBInterface::Instance()->InsertSensorData("SensorPitchAzimuthData", DAU::GetDAU().GetSerialNumber(), serialNumber, SysSetup->GetOperatorName(), result.m_rollTemperature, result.m_sensorPitchAzimuth);
    DBInterface::Instance()->InsertSensorData("SensorRollAzimuthData", DAU::GetDAU().GetSerialNumber(), serialNumber, SysSetup->GetOperatorName(), result.m_pitchTemperature, result.m_sensorRollAzimuth);
  }
}

void GainCalibrationPage6::SaveDayToDayCalibrationData( Gain6CalibrationResult &result, CString serialNumber )
{
	if (result.m_saveResult)
	{
		CString msgCaption, text;

		msgCaption.LoadString(IDS_QUESTION_CAPTION);
		
		m_pResultTable->ShowReport(TRUE);

		text.LoadString( IDS_AUTOMATICALLY_ADJUST_CALIBRATION_DATA );
		msgCaption.LoadString( IDS_QUESTION_CAPTION );

		if( IDYES == MessageBox( text, msgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
		{
		  BOOL doIt = TRUE;
		  if( fabs( result.m_sensorRollAzimuth ) > MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT )
		  {
			msgCaption.LoadString( IDS_WARNING_CAPTION );
			text.Format( _T("The roll azimuth correction is %.2f > %.2f [mrad]\nContinue?"), result.m_sensorRollAzimuth, MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT );
			if( IDNO == MessageBox( text, msgCaption, MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON1 ) )
			{
			  doIt = FALSE;
			}
		  }
		  if( doIt == TRUE )
		  {
			if( fabs( result.m_sensorPitchAzimuth ) > MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT )
			{
			   msgCaption.LoadString( IDS_WARNING_CAPTION );
			  text.Format( _T("The pitch azimuth correction is %.2f > %.2f [mrad]\nContinue?"), result.m_sensorPitchAzimuth, MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT );
			  if( IDNO == MessageBox( text, msgCaption, MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON1 ) )
			  {
				doIt = FALSE;
			  }
			}
		  }

		  if( doIt == TRUE )
		  {
			CreatePolynomial poly;
			BOOL u1 = poly.UpdateRollAzimuthCalibrationData( serialNumber, result.m_sensorRollAzimuth );
			BOOL u2 = poly.UpdatePitchAzimuthCalibrationData( serialNumber, result.m_sensorPitchAzimuth );
			m_calibUpdated = u1 & u2;	  			  
			SensorValidation::UpdateCalibrationFlag(TRUE);
		  }

		  //m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
				
		}
		//m_deleteReport = TRUE;
		text.LoadString(IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD);
		if (IDYES != MessageBox(text, msgCaption, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1))
		{
		//	m_pResultTable->CloseReport();
			return;
		}
		//m_pResultTable->CloseReport();
		//theApp.AddToDocument(m_pResultTable->m_reportHistoryMainID);
		delete m_pResultTable;
		m_pResultTable = NULL;
	  }
}

BOOL GainCalibrationPage6::OnWizardFinish( void )
{
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	if( pSheet->GetCalibrationType() == CalibrationSheet::DayToDayScaleCalibration )
	{
		SaveDayToDayCalibrationData( m_sensor, pSheet -> m_sensorData [0].m_pSource -> GetSerialNumber() ) ;
	}
	else
	{
		SaveCalibrationData( m_sensor, pSheet -> m_sensorData [0].m_pSource -> GetSerialNumber() ) ;
	}


	return CPropertyPage::OnWizardFinish() ;
}

void GainCalibrationPage6::OnInitGrid( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  double r_s_1 = pSheet -> m_sensorData [0].m_roll [0] ;
  double r_s_2 = pSheet -> m_sensorData [0].m_roll [1] ;
  double r_s_3 = pSheet -> m_sensorData [0].m_roll [2] ;
  double r_s_4 = pSheet -> m_sensorData [0].m_roll [3] ;
  double p_s_1 = pSheet -> m_sensorData [0].m_pitch [0] ;
  double p_s_2 = pSheet -> m_sensorData [0].m_pitch [1] ;
  double p_s_3 = pSheet -> m_sensorData [0].m_pitch [2] ;
  double p_s_4 = pSheet -> m_sensorData [0].m_pitch [3] ;
  double r_r_1 = pSheet -> m_referenceData.m_roll [0] ;
  double r_r_2 = pSheet -> m_referenceData.m_roll [1] ;
  double r_r_3 = pSheet -> m_referenceData.m_roll [2] ;
  double r_r_4 = pSheet -> m_referenceData.m_roll [3] ;
  double p_r_1 = pSheet -> m_referenceData.m_pitch [0] ;
  double p_r_2 = pSheet -> m_referenceData.m_pitch [1] ;
  double p_r_3 = pSheet -> m_referenceData.m_pitch [2] ;
  double p_r_4 = pSheet -> m_referenceData.m_pitch [3] ;

//  double denom = -r_s_2*p_s_4+r_s_1*p_s_4-r_s_1*p_s_3+r_s_2*p_s_3-r_s_4*p_s_1+r_s_4*p_s_2+r_s_3*p_s_1-r_s_3*p_s_2 ;
  double denom = 1.0 ;
  double alpha = pSheet -> m_alpha ;

  if (0 != denom)
  {
    m_sensor.m_sensorPitchAzimuth = UnitsToMilliunits( (p_s_3 - r_r_3 - p_s_4 + r_r_4) / alpha ) ;
    if( fabs((p_s_1 - p_r_1 - p_s_2 + p_r_2) / alpha) < DBL_EPSILON )
    {
        m_sensor.m_sensorPitchGain    = 1000;
    }
    else
    {
        m_sensor.m_sensorPitchGain    = 1.0 / ((p_s_1 - p_r_1 - p_s_2 + p_r_2) / alpha) ;
    }
    m_sensor.m_sensorRollAzimuth  = - UnitsToMilliunits( (r_s_1 - r_r_1 - r_s_2 + r_r_2) / alpha ) ;
    if( fabs((r_s_3 + p_r_3 - r_s_4 - p_r_4) / alpha) < DBL_EPSILON )
    {
        m_sensor.m_sensorRollGain     = 1000;
    }
    else
    {
        m_sensor.m_sensorRollGain     = 1.0 / ((r_s_3 + p_r_3 - r_s_4 - p_r_4) / alpha) ;
    }

//    m_sensor.m_sensorPitchAzimuth = - UnitsToMilliunits( -(p_s_4 * p_r_2 + p_s_3 * p_r_1 + p_s_3 * alpha - p_s_4 * alpha - p_s_3 * p_r_2 - p_s_1 * r_r_3 - p_s_4 * p_r_1 - p_s_2 * r_r_4 + p_s_1 * r_r_4 + p_s_2 * r_r_3) / denom ) ;
//    m_sensor.m_sensorPitchGain    = (r_s_3 * alpha - r_s_4 * p_r_1 + r_s_4 * p_r_2 + r_s_3 * p_r_1 - r_s_3 * p_r_2 - r_r_3 * r_s_1 + r_r_3 * r_s_2 + r_r_4 * r_s_1 - r_r_4 * r_s_2 - r_s_4 * alpha) / denom ;
//    m_sensor.m_sensorRollAzimuth  = UnitsToMilliunits( -(r_s_1 * p_r_3 - r_s_4 * r_r_2 - r_s_1 * p_r_4 - r_s_2 * p_r_3 + r_s_3 * r_r_2 + r_s_4 * r_r_1 + r_s_2 * p_r_4 + r_s_1 * alpha - r_s_3 * r_r_1 - r_s_2 * alpha) / denom ) ;
//    m_sensor.m_sensorRollGain     = (p_s_1 * alpha - p_s_2 * alpha + p_s_1 * p_r_3 - p_s_1 * p_r_4 - p_s_2 * p_r_3 + p_s_2 * p_r_4 + r_r_1 * p_s_4 - r_r_1 * p_s_3 - r_r_2 * p_s_4 + r_r_2 * p_s_3) / denom ;
    m_sensor.m_pitchTemperature   = (pSheet -> m_sensorData [0].m_temperature [0] + pSheet -> m_sensorData [0].m_temperature [1]) / 2 ;
    m_sensor.m_rollTemperature    = (pSheet -> m_sensorData [0].m_temperature [2] + pSheet -> m_sensorData [0].m_temperature [3]) / 2 ;
 
	g_AlignerData.ACR[1] = m_sensor.m_sensorRollGain;
	g_AlignerData.ACP[1] = m_sensor.m_sensorPitchGain;
	g_AlignerData.Eac[1] = m_sensor.m_sensorRollAzimuth;
	g_AlignerData.Eal[1] = m_sensor.m_sensorPitchAzimuth;

	m_Temperature[0] = pSheet->m_referenceData.m_temperature[0];
	m_Temperature[1] = (m_sensor.m_pitchTemperature + m_sensor.m_rollTemperature) / 2.0f;;

	m_pResultTable->m_InParam.pTemperature = m_Temperature;

  }
  else
  {
    ASSERT(0) ; // TODO: add proper error control.
  }
}


void GainCalibrationPage6::OnBnClickedShowResultTable()
{
//	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;

	//OnBnClickedShowPolarGraph();
	
	m_pResultTable->ShowReport(FALSE);
	//m_pParent->m_deleteReport = TRUE;
}
