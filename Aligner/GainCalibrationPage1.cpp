// $Id: GainCalibrationPage1.cpp,v 1.5 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GainCalibrationPage1.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage1, CPropertyPage)

GainCalibrationPage1::GainCalibrationPage1( void ) : CPropertyPage(GainCalibrationPage1::IDD)
{
  // Empty
}

GainCalibrationPage1::~GainCalibrationPage1( void )
{
  // Empty
}

void GainCalibrationPage1::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_CALIBRATED_SERIAL_NUMBER, m_sensor) ;
}

BEGIN_MESSAGE_MAP(GainCalibrationPage1, CPropertyPage)
END_MESSAGE_MAP()

BOOL GainCalibrationPage1::InitSensor( void )
{
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	ASSERT(NULL != pSheet->m_referenceData.m_pSource) ;

	int oldIndex = -1 ;
	CString oldSerialNumber ;
	m_sensor.GetWindowText( oldSerialNumber ) ;
	m_sensor.ResetContent() ;

	for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
	{
		Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
		if (UnitType::Unused != pCurrent->GetType())
		{
		if (pSheet->m_referenceData.m_pSource->GetSerialNumber() != pCurrent->GetSerialNumber())
		{
			int index = m_sensor.AddString( pCurrent->GetSerialNumber() ) ;
			if (0 > index) 
			{
				return FALSE ;
			}
			m_sensor.SetItemData( index, i);
			if (pCurrent->GetSerialNumber() == oldSerialNumber)
			{
				oldIndex = index ;
			}
		}
		}
	}
	m_sensor.SetCurSel( oldIndex ) ;
	return TRUE ;
}

BOOL GainCalibrationPage1::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    MoveDlgToRightSideOfApp(pSheet);
    pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    if (!InitSensor())
    {
        // TODO: General Failure should handle this!
        ASSERT(0) ;
    }
    pSheet->m_measurement = 0 ;    
	inProgress = false;
    return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage1::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

	if (CB_ERR == m_sensor.GetCurSel())
	{
		::AfxMessageBox( IDS_SELECT_SENSOR ) ;
		return -1 ;
	}
  
	int index = m_sensor.GetItemData( m_sensor.GetCurSel() );
	pSheet->m_sensorData.clear() ;  
	pSheet->m_sensorData.push_back( CalibrationData( DAU::GetDAU().GetSensor( index ), index))  ;  
	inProgress = true;
	
	g_AlignerData.ObjNo[1] = CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX(index);
	g_AlignerData.NoOfCorr = 1;

	pSheet->SetWizardButtons( PSWIZB_BACK ) ;
	pSheet->StartMeasurement( FALSE, 0.0, pSheet->m_alpha / 2.0, TRUE ) ;
  
	return -1 ;
}

BOOL GainCalibrationPage1::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet->IsMeasuring())
  {
    pSheet->StopMeasurement() ;
    pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
  }
  return TRUE ;
}

LRESULT GainCalibrationPage1::OnWizardBack()
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet->IsMeasuring())
  {
    pSheet->StopMeasurement() ;
    pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }
  return __super::OnWizardBack() ;
}
