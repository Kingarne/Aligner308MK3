// $Id: LiveDataView.cpp,v 1.16 2015-03-30 12:07:02 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "LiveDataView.h"
#include "Util.h"
#include "Type.h"
#include "Defines.h"
#include "UserMessages.h"
#if defined(_DEBUG)
#include "GraphView.h"
#endif

#include "AlignmentWizard.h"
#include "AzimuthAlignWizard.h"
#include "AzimuthVerificationWizard.h"
#include ".\livedataview.h"


#define NO_FILTER			0
#define THIRD_ORDER		1

IMPLEMENT_DYNCREATE(CLiveDataView, CFormView)

static BOOL UseMilliRad( void )
{
  return 0 == SysSetup->GetUnits() || SYSTEM_SETUP_MODE_CALIBRATION == SysSetup->GetMode() ;
}

CLiveDataView::CLiveDataView( void ) : CFormView( CLiveDataView::IDD ), DataRecepient( this )
{
  m_viewMode = ViewCalibrated ;
  m_showOnlySelected = FALSE ;
  m_pSource = NULL ;
  m_newDataArrived = FALSE;
}

CLiveDataView::~CLiveDataView( void )
{
  // Empty
}

void CLiveDataView::DoDataExchange( CDataExchange *pDX )
{
	CFormView::DoDataExchange( pDX ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_1_ROLL, m_sensorRoll [0] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_1_PITCH, m_sensorPitch [0] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_1_TEMP, m_sensorTemp [0] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_1_DESCRIPTION, m_sensorDescription [0] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_1_HEADER, m_sensorHeader [0] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_2_ROLL, m_sensorRoll [1] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_2_PITCH, m_sensorPitch [1] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_2_TEMP, m_sensorTemp [1] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_2_DESCRIPTION, m_sensorDescription [1] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_2_HEADER, m_sensorHeader [1] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_3_ROLL, m_sensorRoll [2] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_3_PITCH, m_sensorPitch [2] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_3_TEMP, m_sensorTemp [2] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_3_DESCRIPTION, m_sensorDescription [2] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_3_HEADER, m_sensorHeader [2] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_4_ROLL, m_sensorRoll [3] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_4_PITCH, m_sensorPitch [3] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_4_TEMP, m_sensorTemp [3] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_4_DESCRIPTION, m_sensorDescription [3] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_4_HEADER, m_sensorHeader [3] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_5_ROLL, m_sensorRoll [4] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_5_PITCH, m_sensorPitch [4] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_5_TEMP, m_sensorTemp [4] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_5_DESCRIPTION, m_sensorDescription [4] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_5_HEADER, m_sensorHeader [4] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_6_ROLL, m_sensorRoll [5] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_6_PITCH, m_sensorPitch [5] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_6_TEMP, m_sensorTemp [5] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_6_DESCRIPTION, m_sensorDescription [5] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_6_HEADER, m_sensorHeader [5] ) ;

	DDX_Control( pDX, IDC_SENSOR_CHANNEL_7_ROLL, m_sensorRoll [6] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_7_PITCH, m_sensorPitch [6] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_7_TEMP, m_sensorTemp [6] );
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_7_DESCRIPTION, m_sensorDescription [6] ) ;
	DDX_Control( pDX, IDC_SENSOR_CHANNEL_7_HEADER, m_sensorHeader [6] ) ;

	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_1_ROLL, m_syncroRoll [0] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_1_PITCH, m_syncroPitch [0] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_1_DESCRIPTION, m_syncroDescription [0] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_1_HEADER, m_syncroHeader [0] ) ;

	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_2_ROLL, m_syncroRoll [1] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_2_PITCH, m_syncroPitch [1] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_2_DESCRIPTION, m_syncroDescription [1] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_2_HEADER, m_syncroHeader [1] ) ;

	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_3_ROLL, m_syncroRoll [2] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_3_PITCH, m_syncroPitch [2] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_3_DESCRIPTION, m_syncroDescription [2] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_3_HEADER, m_syncroHeader [2] ) ;

	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_4_ROLL, m_syncroRoll [3] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_4_PITCH, m_syncroPitch [3] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_4_DESCRIPTION, m_syncroDescription [3] ) ;
	DDX_Control( pDX, IDC_SYNCRO_CHANNEL_4_HEADER, m_syncroHeader [3] ) ;

	DDX_Control(pDX, IDC_SD_ERROR, m_sdError);
	DDX_Control(pDX, IDC_MAIN_BOARD_TEMP, m_mainBoardTemperature);

	DDX_Control(pDX, IDC_FILTER_TYPE, m_filterType);
	DDX_Control(pDX, IDC_FILTER_CONSTANT, m_filterConstant);
	DDX_Control(pDX, IDC_DAU_FREQUENCY, m_frequencyControl) ;
	DDX_Control(pDX, IDC_TEST_VOLTAGE, m_testVoltageControl);
	DDX_Control(pDX, IDC_TEST_VOLTAGE_ACCEPT, m_testVoltageAcceptControl);
	DDX_Control(pDX, IDC_MEASURED_TEST_VOLTAGE, m_measuredTestVoltageControl);
}

BEGIN_MESSAGE_MAP(CLiveDataView, CFormView)
  ON_BN_CLICKED(IDC_CALIBRATED, OnBnClickedCalibrated)
  ON_BN_CLICKED(IDC_UNCALIBRATED, OnBnClickedUncalibrated)
  ON_BN_CLICKED(IDC_RAW, OnBnClickedRaw)
  ON_BN_CLICKED(IDC_LIVE_CLOSE, OnBnClickedLiveClose)
  ON_BN_CLICKED(IDC_APPLY_TAU, OnBnClickedApplyTau)
  ON_CBN_SELCHANGE(IDC_FILTER_TYPE, OnCbnSelchangeFilterType)
  ON_CBN_SELCHANGE(IDC_DAU_FREQUENCY, OnCbnSelchangeDauFrequency)
  ON_EN_KILLFOCUS(IDC_FILTER_CONSTANT, OnEnKillfocusFilterConstant)
  ON_EN_CHANGE(IDC_FILTER_CONSTANT, OnEnChangeFilterConstant)
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
  ON_MESSAGE(UM_DAU_ERROR, OnDauError)
  ON_WM_DESTROY()
  ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_TEST_VOLTAGE_ACCEPT, OnBnClickedTestVoltageAccept)
  ON_WM_ACTIVATE()
  ON_BN_CLICKED(IDC_LIVE_RESTART, OnBnClickedLiveRestart)
  ON_BN_CLICKED(IDC_SYNCRO_CHANNEL_4_HEADER, OnBnClickedSyncroChannel4Header)
  ON_WM_TIMER()
END_MESSAGE_MAP()

void CLiveDataView::InitFrequencyControl( void )
{
    m_frequencyControl.ResetContent();
    CString str;
    double rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_1220];    
    str.Format("0 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_305];
    str.Format("1 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_76];
    str.Format("2 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_19];
    str.Format("3 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    m_frequencyControl.SetCurSel(DAU::GetDAU().m_comThr.GetSamplingMode());

}


LRESULT CLiveDataView::OnNewData( WPARAM, LPARAM )
{
  static int count=0;  
  //TRACE("CLiveDataView: New data %d\n",count++);
  if (UpdateCurrentValues( TRUE ))
  { 
      m_newDataArrived = TRUE;
      //UpdateData() ;
  }
  return TRUE ;
}

LRESULT CLiveDataView::OnDauError( WPARAM, LPARAM )
{
  //OnBnClickedLiveClose() ;
  if( theApp.m_pAzimuthAlignWizard != NULL )
  {
    static_cast<CAzimuthAlignWizard *>(theApp.m_pAzimuthAlignWizard) -> m_AlignVerBenchMarkPage2.PostMessage( UM_DAU_ERROR ) ;
  }
  else if( theApp.m_pAzimuthVerificationWizard != NULL )
  {
    static_cast<CAzimuthVerificationWizard *>(theApp.m_pAzimuthVerificationWizard) -> m_AlignVerBenchMarkPage2.PostMessage( UM_DAU_ERROR ) ;
  }
  else
  {
    LiveRestart( TRUE );
    //GetDlgItem( IDC_LIVE_RESTART ) -> ShowWindow( SW_SHOW );
  }
  return TRUE ;
}

void CLiveDataView::UpdateData( void )
{
  if (m_pSource)
  {
    UpdateSensorData() ;
    UpdateSyncroData() ;
    UpdateMainboardData() ;
	UpdateMeasuredTestVoltage() ;
  }
}


void CLiveDataView::SetSensorChannelState(int ch, BOOL connected)
{
    COLORREF col = connected ? RGB(255, 255, 255) : GetSysColor(COLOR_BTNFACE);
    m_sensorDescription[ch].SetBkColor(col);
    m_sensorRoll[ch].SetBkColor(col);
    m_sensorPitch[ch].SetBkColor(col);
    m_sensorTemp[ch].SetBkColor(col);
}

void CLiveDataView::SetSyncroChannelState(int ch, BOOL connected)
{
    COLORREF col = connected ? RGB(255, 255, 255) : GetSysColor(COLOR_BTNFACE);
    m_syncroDescription[ch].SetBkColor(col);
    m_syncroRoll[ch].SetBkColor(col);
    m_syncroPitch[ch].SetBkColor(col);    
}

void CLiveDataView::UpdateText( void )
{
    if (m_pSource)
    {
        for (int i = 0 ; i < m_pSource -> GetSensorCount() ; i++)
        {
            Sensor *pCurrent = m_pSource -> GetSensor( i ) ;
            if (UnitType::Unused == pCurrent -> GetType() || (m_showOnlySelected && !pCurrent -> IsSelected()))
            {
                m_sensorDescription[i].SetWindowText( _T("") ) ;
            }
            else
            {
                m_sensorDescription[i].SetWindowText( pCurrent ->  m_description ) ;
            }
            CString header = pCurrent -> GetName() ;
            if (UnitType::Unused != pCurrent -> GetType())
            {
                header += _T(" (S/N ") ;
                header += pCurrent->GetSerialNumber() ;
                header += _T(")") ;
            }
            m_sensorHeader[i].SetWindowText( header ) ;
            SetSensorChannelState(i, pCurrent->GetType() != UnitType::Unused); 
        
        }

        map<CString, int> syncroMap;
        syncroMap[D1] = 3; 
        syncroMap[D2] = 2; 
        syncroMap[J1] = 1; 
        syncroMap[J2] = 0; 
        for (int i = 0 ; i < m_pSource -> GetSyncroCount() ; i++)
        {
            CString windowText;
            Syncro *pCurrent = m_pSource -> GetSyncro( i ) ;
            CString syncroName = pCurrent -> GetName();
            int index = syncroMap[syncroName];    
            SetSyncroChannelState(index, pCurrent->GetType() != UnitType::Unused); 
    
            windowText = (UnitType::Unused == pCurrent -> GetType() || (m_showOnlySelected && !pCurrent -> IsSelected())) ? _T("") : pCurrent->m_description;
           
            m_syncroDescription[index].SetWindowText( windowText ) ;
            m_syncroHeader[index].SetWindowText( syncroName ) ;            
        }
    }

  if (ViewRaw == m_viewMode)
  {
    CString text ;
    text.Format( IDS_PITCH_RAW_TEXT ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_PITCH_TEXT )) -> SetWindowText( text ) ;

    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;

    text.Format( IDS_ROLL_RAW_TEXT ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_ROLL_TEXT )) -> SetWindowText( text ) ;

    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;

    text.Format( IDS_TEMPERATURE_RAW_TEXT ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
  }
  else
  {
    CString text ;
    if (!UseMilliRad())
    {
      text.Format( IDS_PITCH_ARCMIN_TEXT ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_PITCH_TEXT )) -> SetWindowText( text ) ;

      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;

      text.Format( IDS_ROLL_ARCMIN_TEXT ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_ROLL_TEXT )) -> SetWindowText( text ) ;

      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;
    }
    else
    {
      text.Format( IDS_PITCH_MILLIRAD_TEXT ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_PITCH_TEXT )) -> SetWindowText( text ) ;

      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_PITCH_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_PITCH_TEXT )) -> SetWindowText( text ) ;

      text.Format( IDS_ROLL_MILLIRAD_TEXT ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_ROLL_TEXT )) -> SetWindowText( text ) ;

      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_1_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_2_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_3_ROLL_TEXT )) -> SetWindowText( text ) ;
      static_cast<CStatic *>(GetDlgItem( IDC_SYNCRO_CHANNEL_4_ROLL_TEXT )) -> SetWindowText( text ) ;
    }
    text.Format( IDS_TEMPERATURE_CENTIGRADES_TEXT ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_1_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_2_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_3_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_4_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_5_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_6_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
    static_cast<CStatic *>(GetDlgItem( IDC_SENSOR_CHANNEL_7_TEMPERATURE_TEXT )) -> SetWindowText( text ) ;
  }


}

void CLiveDataView::UpdateSensorData( void )
{
  for (int i = 0 ; i < m_currentValues.GetSensorCount() ; i++)
  {
    CString rollText ;
    CString pitchText ;
    CString temperatureText ;
    Sensor *pCurrent = m_pSource -> GetSensor( i ) ;
    if ((ViewRaw == m_viewMode || UnitType::Unused != pCurrent -> GetType()) && (pCurrent -> IsSelected() || !m_showOnlySelected))
    {
      double roll, pitch;
      switch (m_viewMode)
      {
      case ViewRaw:
        rollText.Format( IDS_RAW_DATA_FORMAT, m_currentValues.GetSensor( i ).GetRawRoll() ) ;
        pitchText.Format( IDS_RAW_DATA_FORMAT, m_currentValues.GetSensor( i ).GetRawPitch() ) ;
        temperatureText.Format( IDS_RAW_DATA_FORMAT, m_currentValues.GetSensor( i ).GetRawTemperature() ) ;
        DrawValue(&m_sensorRoll[i], rollText, true);
		DrawValue(&m_sensorPitch[i], pitchText, true);
		break ;

      case ViewUncalibrated:
        roll = m_currentValues.GetSensor( i ).GetUncalibratedRoll();
        pitch = m_currentValues.GetSensor( i ).GetUncalibratedPitch();
        if( g_AlignerData.SignDef == -1 )
        {
          roll = -roll;
          pitch = -pitch;
        }
        if (UseMilliRad())
        {
          rollText.Format( IDS_MILLIRAD_FORMAT, 1000 * roll ) ;
          pitchText.Format( IDS_MILLIRAD_FORMAT, 1000 * pitch ) ;
        }
        else
        {
          rollText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * roll) ) ;
          pitchText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * pitch) ) ;
        }
        temperatureText.Format( IDS_TEMPERATURE_FORMAT, m_currentValues.GetSensor( i ).GetTemperature() ) ;
        DrawValue(&m_sensorRoll[i], rollText);
		DrawValue(&m_sensorPitch[i], pitchText);

		break ;

      default: // ViewCalibrated
        roll = -g_AlignerData.SignDef * m_currentValues.GetSensor( i ).GetRoll();
        pitch = -g_AlignerData.SignDef * m_currentValues.GetSensor( i ).GetPitch();
        if (UseMilliRad())
        {
          rollText.Format( IDS_MILLIRAD_FORMAT, 1000 * roll ) ;
          pitchText.Format( IDS_MILLIRAD_FORMAT, 1000 * pitch ) ;
        }
        else
        {
          rollText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * roll) ) ;
          pitchText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * pitch) ) ;
        }
        temperatureText.Format( IDS_TEMPERATURE_FORMAT, m_currentValues.GetSensor( i ).GetTemperature() ) ;
		DrawValue(&m_sensorRoll[i], rollText);
		DrawValue(&m_sensorPitch[i], pitchText);

        break ;
      }
    }
    m_sensorRoll [i].SetWindowText( rollText ) ;
    m_sensorPitch [i].SetWindowText( pitchText ) ;
    m_sensorTemp [i].SetWindowText( temperatureText ) ;
	  
	
	

  }
}

void CLiveDataView::DrawValue(CWnd* pWnd, CString valtxt, bool clearOnly)
{
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	//TRACE("l:%d, t=%d, r:%d, b=%d\n", rect.left, rect.top, rect.right, rect.bottom);
	CClientDC dc(this);
	//CDC *pdc = GetDC();
	COLORREF bgcolour = ::GetSysColor(COLOR_3DFACE);
	CPen bgpen(PS_SOLID, 3, bgcolour);
	CPen* pOldPen = dc.SelectObject(&bgpen);
	dc.MoveTo(rect.left-1,rect.top-2);
	dc.LineTo(rect.right+1,rect.top-2);
	
	if(clearOnly)
	{
		dc.SelectObject(pOldPen);
		//ReleaseDC(pdc);
		return;
	}

	double val = atof(valtxt);

	int w = rect.Width();
	int centerX = rect.CenterPoint().x;
	//int ofs = rand()%w-w/2;

	double fac = (double)w/210.0f;
	double len = fac*(val);  //Negative to the left
	int ofs = (int)(len+0.5f);

	CPen pen(PS_SOLID, 2, RGB(80,80,80));
	dc.SelectObject(&pen);		
	
	dc.MoveTo(centerX, rect.top-1);
	dc.LineTo(centerX+len, rect.top-1);

	CPen cpen(PS_SOLID, 1, RGB(0,0,0));
	dc.SelectObject(&cpen);
	dc.MoveTo(centerX, rect.top-1);
	dc.LineTo(centerX, rect.top-4);
	dc.SelectObject(pOldPen);
	//ReleaseDC(pdc);
}

void CLiveDataView::UpdateSyncroData( void )
{
    for (int i = 0 ; i < m_currentValues.GetSyncroCount() ; i++)
    {
        CString rollText ;
        CString pitchText ;
        Syncro *pCurrent = m_pSource -> GetSyncro( i ) ;
        if ((ViewRaw == m_viewMode || UnitType::Unused != m_pSource -> GetSyncro( i ) -> GetType()) && (pCurrent -> IsSelected() || !m_showOnlySelected))
        {
            double roll, pitch;
            switch (m_viewMode)
            {
            case ViewRaw:
                rollText.Format( IDS_RAW_DATA_FORMAT, m_currentValues.GetSyncro( i ).GetRawRoll() ) ;
                pitchText.Format( IDS_RAW_DATA_FORMAT, m_currentValues.GetSyncro( i ).GetRawPitch() ) ;
                break ;

            case ViewUncalibrated:
                roll = -m_currentValues.GetSyncro( i ).GetUncalibratedRoll();
                pitch = -m_currentValues.GetSyncro( i ).GetUncalibratedPitch();
                if( g_AlignerData.SignDef == 1 )
                {
                    roll = -roll;
                    pitch = -pitch;
                }
                rollText.Format( IDS_MILLIRAD_FORMAT, 1000 * roll ) ;
                pitchText.Format( IDS_MILLIRAD_FORMAT, 1000 * pitch ) ;
                break ;

            default: // ViewCalibrated
                roll = m_currentValues.GetSyncro( i ).GetRoll();
                pitch = m_currentValues.GetSyncro( i ).GetPitch();
                if( g_AlignerData.SignDef == 1 )
                {
                    roll = -roll;
                    pitch = -pitch;
                    }
                    if (UseMilliRad())
                    {
                        rollText.Format( IDS_MILLIRAD_FORMAT, 1000 * roll ) ;
                        pitchText.Format( IDS_MILLIRAD_FORMAT, 1000 * pitch ) ;
                    }
                    else
                    {
                        rollText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * roll) ) ;
                        pitchText.Format( IDS_ARCMIN_FORMAT, MRADIANS_TO_ARCMIN(1000 * pitch) ) ;
                    }
                break ;
            }
        }
        CString syncroName = pCurrent -> GetName();

        Digital* pDig = dynamic_cast<Digital*>(pCurrent);                
	    if(pDig)
	    {
		    if(!pDig->m_dataReceptionOK && pDig->GetType() != UnitType::Unused)
		    {
			    rollText.Format("No Data");
			    pitchText.Format("No Data");
		    }
	    }
	    if( D1 == syncroName )
        {
            m_syncroRoll [3].SetWindowText( rollText ) ;
            m_syncroPitch [3].SetWindowText( pitchText ) ;
        }
        else if( D2 == syncroName )
        {
            m_syncroRoll [2].SetWindowText( rollText ) ;
            m_syncroPitch [2].SetWindowText( pitchText ) ;
        }
        else if( J1 == syncroName )
        {
            m_syncroRoll [1].SetWindowText( rollText ) ;
            m_syncroPitch [1].SetWindowText( pitchText ) ;
        }
        else if( J2 == syncroName )
        {
            m_syncroRoll [0].SetWindowText( rollText ) ;
            m_syncroPitch [0].SetWindowText( pitchText ) ;
        }
    }
}

void CLiveDataView::UpdateMainboardData( void )
{
  ASSERT(m_pSource) ;
  m_sdError.SetCheck( m_pSource -> GetMainboard() -> IsSDError() ? BST_CHECKED : BST_UNCHECKED ) ;
  CString text ;
  text.Format( IDS_TEMPERATURE_FORMAT, m_pSource -> GetMainboard() -> GetTemperature() ) ;
  m_mainBoardTemperature.SetWindowText( text ) ;
}

void CLiveDataView::UpdateMeasuredTestVoltage( void )
{
  CString measuredVoltage;
  measuredVoltage.Format( "%d", m_pSource->GetFilteredMeasuredDACVoltage() );//change polarity, don't know why
	m_measuredTestVoltageControl.SetWindowText( measuredVoltage ) ;
}

#ifdef _DEBUG
void CLiveDataView::AssertValid( void ) const
{
  CFormView::AssertValid() ;
}

void CLiveDataView::Dump( CDumpContext &dc ) const
{
  CFormView::Dump( dc ) ;
}
#endif //_DEBUG

void CLiveDataView::OnInitialUpdate( void )
{
    CFormView::OnInitialUpdate() ;
#ifdef _DEBUG
  GetDlgItem( IDC_DAU_FREQUENCY ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_DAU_FREQUENCY_HEADER ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_FIFO_HALF_FULL ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_ACQUSITION_IN_PROGRESS ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_FIFO_FULL ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_FLAGS_HEADER ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_SD_ERROR ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_LIVE_CLOSE2 ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_LIVE_STOP ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_DUMP_HEADER ) -> ShowWindow( SW_SHOW );
  GetDlgItem( IDC_UNCALIBRATED ) -> ShowWindow( SW_SHOW );
#endif
    CString str;
    if( g_AlignerData.SignDef == -1 )
    {
        str.LoadString( IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_UP );
    }
    else
    {
        str.LoadString( IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_DOWN );          
    }
    static_cast<CStatic *>(GetDlgItem( IDC_ROLL_PITCH_SIGN_DEF )) -> SetWindowText( str ) ;
    GetDlgItem( IDC_LIVE_RESTART ) -> ShowWindow( SW_HIDE );

    //m_frequency = DAU::GetDAU().GetFrequency() ;
    m_dataFreqMode = DAU::GetDAU().m_comThr.GetSamplingMode();			
    InitFrequencyControl() ;
    SetViewMode( m_viewMode );
    m_filterConstant.SetWindowText( _T("10") ) ; // Kludge
    m_filterType.SetCurSel( NO_FILTER ) ;
    GetDlgItem( IDC_FILTER_CONSTANT ) -> EnableWindow( FALSE ) ;
    GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( FALSE ) ;
    SetTauLimits();

    for(int i=0;i<4;i++)
    {
        SetSyncroChannelState(i, FALSE);
    }
    
 
    m_timer = SetTimer(1, 1000, NULL);
}

void CLiveDataView::OnBnClickedCalibrated( void )
{
  m_viewMode = ViewCalibrated ;
  UpdateText() ;
  UpdateData() ;
}

void CLiveDataView::OnBnClickedUncalibrated( void )
{
  m_viewMode = ViewUncalibrated ;
  UpdateText() ;
  UpdateData() ;
}

void CLiveDataView::OnBnClickedRaw( void )
{
  m_viewMode = ViewRaw ;
  UpdateText() ;
  UpdateData() ;
}

void CLiveDataView::OnDestroy( void ) 
{
  SetSource( NULL ) ;
  __super::OnDestroy() ;
}

void CLiveDataView::OnActivateView( BOOL activate, CView *pActivateView, CView *pDeactiveView)
{
  __super::OnActivateView( activate, pActivateView, pDeactiveView ) ;
}

void CLiveDataView::ResetFilters( double factor )
{
    if (0 == factor)
    {
        m_filterType.SetCurSel( 0 ) ;
    }
    for (int i = 0 ; i < DAU_NB_SENSOR_CHANNELS ; i++)
    {
        switch (m_filterType.GetCurSel())
        {
        case NO_FILTER:
        DAU::GetDAU().GetSensor( i )->ResetFilter() ;
        break ;

        case THIRD_ORDER:
        DAU::GetDAU().GetSensor( i )->ResetFilter( DAU::GetDAU().GetPeriod(), 0.9, factor ) ;
        break ;

        default:
        ASSERT(0) ;
        break ;
        }
    }

  for (int i = 0 ; i < DAU_NB_SYNCRO_CHANNELS ; i++)
  {
    switch (m_filterType.GetCurSel())
    {
    case NO_FILTER:
      DAU::GetDAU().GetSyncro( i ) -> ResetFilter() ;
      break ;

    case THIRD_ORDER: // 3:rd order filter
      DAU::GetDAU().GetSyncro( i ) -> ResetFilter( DAU::GetDAU().GetPeriod(), 0.9, DAU::GetDAU().GetPeriod() * factor ) ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
  }
}

void CLiveDataView::OnEnKillfocusFilterConstant()
{
}

void CLiveDataView::OnEnChangeFilterConstant()
{
  GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( TRUE ) ;
}

void CLiveDataView::OnBnClickedApplyTau()
{
  if( IsTauOK() == FALSE )
  {
    return;
  }
  CString filterConstantText ;
  m_filterConstant.GetWindowText( filterConstantText ) ;
  double factor = atof( filterConstantText ) ;	
  ResetFilters( factor ) ;
  GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( FALSE ) ;
}

void CLiveDataView::OnCbnSelchangeFilterType( void )
{
  if( m_filterType.GetCurSel() == NO_FILTER )
  { // No filter
    ResetFilters( 0 ) ;
    GetDlgItem( IDC_FILTER_CONSTANT ) -> EnableWindow( FALSE ) ;
    GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( FALSE ) ;
  }
  else
  {
    if( IsTauOK() == TRUE )
    {
      CString filterConstantText ;
      m_filterConstant.GetWindowText( filterConstantText ) ;
      double factor = atof( filterConstantText ) ;
      ResetFilters( factor ) ;
    }
    else
    {
      GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( TRUE ) ;
    }
    GetDlgItem( IDC_FILTER_CONSTANT ) -> EnableWindow( TRUE ) ;
  }
}

void CLiveDataView::OnCbnSelchangeDauFrequency( void )
{
  m_dataFreqMode = m_frequencyControl.GetCurSel();
    DAU::GetDAU().m_comThr.SetSamplingMode(m_dataFreqMode);			
  //   DAU::GetDAU().SetFrequency( m_frequency ) ;
//   DAU::GetDAU().Start() ;
  SetTauLimits();
  IsTauOK();
}

void CLiveDataView::LiveClose( void )
{
  DAU::GetDAU().EnableErrorEvent( FALSE );
  ClearSelectedChannels();
  theApp.SetLiveDataViewActivated( FALSE );
  theApp.SwitchView( theApp.m_pSystemConfigurationView ) ;
}

void CLiveDataView::OnBnClickedLiveClose( void )
{
  LiveClose();
  DAU::GetDAU().SetOverrangeDetectionActive( TRUE );
}

void CLiveDataView::LiveRestart( BOOL showMessages )
{
//   if( DAU::GetDAU().Restart( showMessages ) == TRUE )
//   {
//     //DAU::GetDAU().ClearErrorEventFlag();
//     //GetDlgItem( IDC_LIVE_RESTART ) -> ShowWindow( SW_HIDE );
//   }
  TestVoltageAccept();
  DAU::GetDAU().EnableErrorEvent( TRUE ); 
}

void CLiveDataView::OnBnClickedLiveRestart()
{
  LiveRestart( TRUE );
}

void CLiveDataView::SetViewMode( ViewMode viewMode )
{
  switch (viewMode)
  {
  case ViewRaw:
    m_viewMode = viewMode ;
    CheckRadioButton( IDC_CALIBRATED, IDC_RAW, IDC_RAW ) ;
    break ;

  case ViewUncalibrated:
    m_viewMode = viewMode ;
    CheckRadioButton( IDC_CALIBRATED, IDC_RAW, IDC_UNCALIBRATED ) ;
    break ;

  default:
    m_viewMode = ViewCalibrated ;
    CheckRadioButton( IDC_CALIBRATED, IDC_RAW, IDC_CALIBRATED ) ;
    break ;
  }
  UpdateText() ;
  UpdateData() ;
}

void CLiveDataView::SetTauLimits( void )
{
    //double samplePeriod = DAU::BitfieldToPeriod( DAU::GetDAU().GetFrequency() );
    double samplePeriod = 1.0f / DAU::GetDAU().m_comThr.GetSamplingRate();

    m_minTau = TAU_FREQUENCY_FACTOR * samplePeriod;

    if( m_minTau < MIN_FILTER_TIME_CONSTANT_LIVE_DATA )
    {
        m_minTau = MIN_FILTER_TIME_CONSTANT_LIVE_DATA;
    }
    m_maxTau = MAX_FILTER_TIME_CONSTANT ;
}

BOOL CLiveDataView::IsTauOK( void )
{
  if( m_filterType.GetCurSel() == NO_FILTER )
  {
    return TRUE ;
  }
  CString filterConstantText ;
  m_filterConstant.GetWindowText( filterConstantText ) ;
  double factor = atof( filterConstantText ) ;

  if( (factor < m_minTau) || (factor > m_maxTau) )
  {
    CString MsgCaption, MsgText, Max, Min;
    MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    Max.Format( "%.2f", m_maxTau ) ;
    Min.Format( "%.2f", m_minTau ) ;
    AfxFormatString2( MsgText, IDS_TAU_OUT_OF_LIMITS_MUST_BE_BETWEEN_MIN_MAX, Min, Max );
    MessageBox( MsgText, MsgCaption, MB_ICONINFORMATION|MB_OK );
    GetDlgItem( IDC_APPLY_TAU ) -> EnableWindow( TRUE ) ;
    return FALSE ;
  }
  return TRUE ;
}

void CLiveDataView::SetFilterTypeAndTau( double tau )
{
  m_filterType.SetCurSel( THIRD_ORDER ) ;
  SetTauLimits() ;
  CString filterConstantText ;
  filterConstantText.Format( "%.2f", tau ) ;
  m_filterConstant.SetWindowText( filterConstantText ) ;
  OnCbnSelchangeFilterType();
}

void CLiveDataView::SetShowOnlySelected( BOOL flag )
{
  m_showOnlySelected = flag ;
}

void CLiveDataView::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (bShow)
    {
        SetHandleError() ;
        GetDlgItem( IDC_LIVE_RESTART ) -> ShowWindow( SW_HIDE );
        m_testVoltageControl.EnableWindow( 0 < DAU::GetDAU().GetProtocolVersion()) ;
	    m_measuredTestVoltageControl.EnableWindow( 0 < DAU::GetDAU().GetProtocolVersion()) ;
        m_testVoltageAcceptControl.EnableWindow( 0 < DAU::GetDAU().GetProtocolVersion()) ;
        m_testVoltageControl.SetWindowText( _T("0") ) ;
	    m_measuredTestVoltageControl.SetWindowText( _T("0") ) ;
        theApp.SetLiveDataViewActivated( TRUE ) ;
        m_frequency = DAU::GetDAU().GetFrequency() ;
        m_frequencyControl.SetCurSel(DAU::GetDAU().m_comThr.GetSamplingMode());
        //m_frequencyControl.SetCurSel( m_frequency - HIGHEST_FREQUENCY_INDEX ) ;
        SetTauLimits() ;
        SetSource( &(DAU::GetDAU()) ) ;
        DAU::GetDAU().EnableErrorEvent( TRUE );
        DAU::GetDAU().m_comThr.m_errorHandler.m_comErr[COM_ERR_DIG_CH1_NO_DATA].alertWhenActive = FALSE;
	    DAU::GetDAU().m_comThr.m_errorHandler.m_comErr[COM_ERR_DIG_CH2_NO_DATA].alertWhenActive = FALSE;
	    TestVoltageAccept();
        UpdateText() ;                
    }
    else
    {
        DAU::GetDAU().StartWithTest( 32768 ) ;
	    DAU::GetDAU().m_comThr.m_errorHandler.m_comErr[COM_ERR_DIG_CH1_NO_DATA].alertWhenActive = TRUE;
	    DAU::GetDAU().m_comThr.m_errorHandler.m_comErr[COM_ERR_DIG_CH2_NO_DATA].alertWhenActive = TRUE;
	    SetSource( NULL ) ;
    }
    __super::OnShowWindow( bShow, nStatus ) ;
    theApp.SetLiveDataViewActivated( bShow ) ;
}

void CLiveDataView::OnBnClickedTestVoltageAccept( void )
{
  TestVoltageAccept();
}

void CLiveDataView::TestVoltageAccept( void )
{
  CString voltageText ;
  m_testVoltageControl.GetWindowText( voltageText ) ;
  int voltage = atoi( voltageText ) ;
  if ((-32768 > voltage) || (32767 < voltage))
  {
    ::AfxMessageBox( _T("Test Voltage must be in the range -32768 to 32767 inclusive.") ) ;
    return ;
  }
  voltage += 32768 ;
  DAU::GetDAU().SetDACVoltage( voltage ) ;
}

void CLiveDataView::OnBnClickedSyncroChannel4Header()
{
  // TODO: Add your control notification handler code here
}

void CLiveDataView::OnTimer(UINT nIDEvent)
{
    //TRACE("OnTimer\n");
    if(m_newDataArrived)
    {
        UpdateData();
        m_newDataArrived = FALSE;
    }

    __super::OnTimer(nIDEvent);
}
