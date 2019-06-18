// $Id: TiltCalibrationPage1.cpp,v 1.5 2014-07-15 13:22:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "tiltcalibrationpage1.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TiltCalibrationPage1, CPropertyPage)

TiltCalibrationPage1::TiltCalibrationPage1( void ) : CPropertyPage(TiltCalibrationPage1::IDD)
{
  // Empty
}

TiltCalibrationPage1::~TiltCalibrationPage1( void )
{
  // Empty
}

void TiltCalibrationPage1::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange(pDX) ;
  DDX_Control(pDX, IDC_CALIBRATED_SERIAL_NUMBER_0, m_sensor [0]);
  DDX_Control(pDX, IDC_CALIBRATED_SERIAL_NUMBER_1, m_sensor [1]);
  DDX_Control(pDX, IDC_CALIBRATED_SERIAL_NUMBER_2, m_sensor [2]);
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage1, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage1::InitSensor( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  ASSERT(NULL != pSheet->m_referenceData.m_pSource) ;

  int oldIndex [3] = { -1, -1, -1 } ;
  CString oldSerialNumber [3] ;
  for (int i = 0 ; i < 3 ; i++)
  {
    m_sensor [i].GetWindowText( oldSerialNumber [i] ) ;
    m_sensor [i].ResetContent() ;
  }
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
  {
    Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
    if (UnitType::Fixed == pCurrent->GetType())
    {
      if (pSheet->m_referenceData.m_pSource->GetSerialNumber() != pCurrent->GetSerialNumber())
      {
        for (int j = 0 ; j < 3 ; j++)
        {
          int index = m_sensor[j].AddString( pCurrent->GetSerialNumber() ) ;
          if (0 > index)
          {
            return FALSE ;
          }
          m_sensor[j].SetItemData( index, i);
		  if (pCurrent->GetSerialNumber() == oldSerialNumber [j])
          {
            oldIndex [j] = index ;
          }
        }
      }
    }
  }
  for (int i = 0 ; i < 3 ; i++)
  {
    m_sensor [i].SetCurSel( oldIndex [i] ) ;
  }
  return TRUE ;
}

BOOL TiltCalibrationPage1::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet->SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    if (!InitSensor())
    {
        // TODO: General Failure should handle this!
        ASSERT(0) ;
    }
    pSheet->m_measurement = 0 ;
    
    MoveDlgToRightSideOfApp(pSheet);
	inProgress = false;

    return __super::OnSetActive() ;
}

LRESULT TiltCalibrationPage1::OnWizardNext( void )
{
	if(inProgress)
	{
		inProgress = false;
		return CPropertyPage::OnWizardNext();
	}
	
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	pSheet->m_sensorData.clear() ;

	BOOL sensorSelected = FALSE ;
	for (int i = 0 ; i < 3 ; i++)
	{
		int index ;
		if (( CB_ERR != (index = m_sensor[i].GetCurSel()) ))
		{
			int index = m_sensor[i].GetItemData( m_sensor[i].GetCurSel() );
			pSheet->m_sensorData.push_back( CalibrationData( DAU::GetDAU().GetSensor( index ), index))  ;  	  
			sensorSelected = TRUE ;
		}
	}
	if (!sensorSelected)
	{
		::AfxMessageBox( IDS_SELECT_SENSOR ) ;
		return -1 ;
	}
	inProgress = true;
	ASSERT(NULL != pSheet->m_sensorData.begin()->m_pSource) ;
	pSheet->SetWizardButtons( PSWIZB_BACK ) ;
	pSheet->StartMeasurement() ;
	return -1 ;
}

BOOL TiltCalibrationPage1::OnQueryCancel( void )
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

LRESULT TiltCalibrationPage1::OnWizardBack( void )
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
