// $Id: TheoAdapterCalibrationPage1.cpp,v 1.2 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TheoAdapterCalibrationPage1.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPage1, CPropertyPage)

TheoAdapterCalibrationPage1::TheoAdapterCalibrationPage1() : CPropertyPage(TheoAdapterCalibrationPage1::IDD)
, m_sensorAdapterString(_T(""))
{
  // Empty
}

TheoAdapterCalibrationPage1::~TheoAdapterCalibrationPage1( void )
{
  // Empty
}

void TheoAdapterCalibrationPage1::DoDataExchange( CDataExchange *pDX )
{
	CPropertyPage::DoDataExchange( pDX ) ;
	DDX_Control(pDX, IDC_SENSOR_SERIAL_NUMBER_1, m_sensorAdapterControl);
	DDX_CBString(pDX, IDC_SENSOR_SERIAL_NUMBER_1, m_sensorAdapterString);
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPage1, CPropertyPage)
END_MESSAGE_MAP()

BOOL TheoAdapterCalibrationPage1::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
	if (!InitSensorAdapter())
	{
		ASSERT(0) ;
	}
  return focus ;
}

BOOL TheoAdapterCalibrationPage1::OnSetActive( void )
{
    CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    
    MoveDlgToRightSideOfApp(pSheet);
    return __super::OnSetActive() ;
}

LRESULT TheoAdapterCalibrationPage1::OnWizardNext( void )
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
	//pSensor->SetCurrentAz(_PI/2);
	//pSheet -> m_sensorData.push_back( CalibrationData( static_cast<Sensor *>(m_sensorAdapterControl.GetItemDataPtr( index )) ) ) ;
	//reset the AdapterCalibrationData for the selected gun adapter
	CalibrationData currCalibrationData( pSensor, sensorIndex );
	DBTIMESTAMP now;
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(now);
	currCalibrationData.m_pSource->SetAdapterCalibrationData( AdapterCalibrationData( 0, 0, now) );
	pSheet->m_sensorData.push_back( currCalibrationData ) ;
   
	  return __super::OnWizardNext() ;
}

BOOL TheoAdapterCalibrationPage1::InitSensorAdapter( void )
{
	for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
	{
		Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
		if (pCurrent->GetType() == UnitType::Theo && pCurrent->GetAdapterType() == AdapterData::Type::Adj)
		{
			CString item = pCurrent->GetSerialNumber() ;
			item += "/" ;
			item += pCurrent->GetAdapterSerialNumber() ;
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
