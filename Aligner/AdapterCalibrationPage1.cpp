// $Id: AdapterCalibrationPage1.cpp,v 1.4 2014-07-11 09:29:30 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AdapterCalibrationPage1.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage1, CPropertyPage)

AdapterCalibrationPage1::AdapterCalibrationPage1() : CPropertyPage(AdapterCalibrationPage1::IDD)
, m_sensorAdapterString(_T(""))
{
  // Empty
}

AdapterCalibrationPage1::~AdapterCalibrationPage1( void )
{
  // Empty
}

void AdapterCalibrationPage1::DoDataExchange( CDataExchange *pDX )
{
	CPropertyPage::DoDataExchange( pDX ) ;
	DDX_Control(pDX, IDC_SENSOR_SERIAL_NUMBER_1, m_sensorAdapterControl);
	DDX_CBString(pDX, IDC_SENSOR_SERIAL_NUMBER_1, m_sensorAdapterString);
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage1, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage1::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
	if (!InitSensorAdapter())
	{
		ASSERT(0) ;
	}
  return focus ;
}

BOOL AdapterCalibrationPage1::OnSetActive( void )
{
    CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    
    MoveDlgToRightSideOfApp(pSheet);
    return __super::OnSetActive() ;
}

LRESULT AdapterCalibrationPage1::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> m_sensorData.clear() ;
	int index ;
	if (( CB_ERR == (index = m_sensorAdapterControl.GetCurSel()) ))
  {
    ::AfxMessageBox( IDS_SELECT_SENSOR ) ;
    return -1 ;
  }
  int sensorIndex = m_sensorAdapterControl.GetItemData(m_sensorAdapterControl.GetCurSel());
  Sensor* pSensor = DAU::GetDAU().GetSensor( sensorIndex );
	//pSheet -> m_sensorData.push_back( CalibrationData( static_cast<Sensor *>(m_sensorAdapterControl.GetItemDataPtr( index )) ) ) ;
  //reset the AdapterCalibrationData for the selected gun adapter
  CalibrationData currCalibrationData( pSensor, sensorIndex );
	DBTIMESTAMP now;
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(now);
  currCalibrationData.m_pSource->SetAdapterCalibrationData( AdapterCalibrationData( 0, 0, now ) );
  pSheet->m_sensorData.push_back( currCalibrationData ) ;
   
  return __super::OnWizardNext() ;
}

BOOL AdapterCalibrationPage1::InitSensorAdapter( void )
{
	for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
	{
		Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
		if (UnitType::Gun == pCurrent -> GetType())
		{
			CString item = pCurrent -> GetSerialNumber() ;
			item += "/" ;
			item += pCurrent -> GetAdapterSerialNumber() ;
			int index = m_sensorAdapterControl.AddString( item ) ;
			if (0 > index)// || CB_ERR == m_sensorAdapterControl.SetItemDataPtr( index, static_cast<void *>(pCurrent) ))
			{
				return FALSE ;
			}
			m_sensorAdapterControl.SetItemData( index, i);
		}
  }
  return TRUE ;
}
