// $Id: SensorAlignmentPage.cpp,v 1.7 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SensorAlignmentPage.h"
#include <algorithm>
using namespace std;

IMPLEMENT_DYNAMIC(SensorAlignmentPage, CPropertyPage)


	


SensorAlignmentPage::SensorAlignmentPage( BOOL calibrationConfig ) : CPropertyPage( calibrationConfig ? SensorAlignmentPage::IDD2 : SensorAlignmentPage::IDD1)
	, m_type(UnitType::Unused)
, m_nominalAzimuth(0)
{
	m_calibrationSetup = calibrationConfig ;

	if(m_typeTextmap.size()==0)
	{
		m_typeTextmap[UnitType::Unused] = string(UnitType::GetUnitText(UnitType::Unused));
		m_typeTextmap[UnitType::Fixed] = string(UnitType::GetUnitText(UnitType::Fixed));
		m_typeTextmap[UnitType::Gun] = string(UnitType::GetUnitText(UnitType::Gun));
		m_typeTextmap[UnitType::Platform] = string(UnitType::GetUnitText(UnitType::Platform));
		if(!m_calibrationSetup)
			m_typeTextmap[UnitType::Theo] = string(UnitType::GetUnitText(UnitType::Theo));
	}

}

SensorAlignmentPage::~SensorAlignmentPage( void )
{
	// Empty
}

void SensorAlignmentPage::DoDataExchange( CDataExchange *pDX )
{
	__super::DoDataExchange( pDX ) ;
	DDX_Control(pDX, IDC_TYPE_TITLE, m_typeTitle);
    DDX_Control(pDX, IDC_STATION_TITLE, m_stationTitle);
    DDX_Control(pDX, IDC_SENS_SN_TITLE, m_sensorSNTitle);
    DDX_Control(pDX, IDC_ADAPT_SN_TITLE, m_adaptorSNTitle);
    DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzTitle);
    
    DDX_Control(pDX, IDC_STATION_COMBO, m_stationControl) ;    
    //DDX_Control(pDX, IDC_SENSOR_DESCRIPTION, m_UnitTypeDescriptionControl) ;
	//DDX_Text(pDX, IDC_SENSOR_DESCRIPTION, m_description) ;
	DDX_Control(pDX, IDC_SENSOR_SERIAL_NUMBER, m_serialNumberControl) ;
	DDX_Text(pDX, IDC_SENSOR_SERIAL_NUMBER, m_serialNumber) ;
	DDX_Control(pDX, IDC_SENSOR_NOMINAL_AZIMUTH, m_nominalAzimuthControl);
	DDX_Text(pDX, IDC_SENSOR_NOMINAL_AZIMUTH, m_nominalAzimuth);
	DDV_MinMaxDouble(pDX, m_nominalAzimuth, -180, 180);
	DDX_Control(pDX, IDC_ADAPTER_SERIAL_NUMBER, m_adapterSerialNumber);
	DDX_Text(pDX, IDC_ADAPTER_SERIAL_NUMBER, m_adapterNumberString) ;
	DDX_Control(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetectionControl ) ;
	DDX_Check(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetection) ;
	
	
	DDX_Control(pDX, IDC_SENSOR_TYPE, m_typeControl) ;
	//DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzimuthTitleControl);

	if (pDX->m_bSaveAndValidate && m_type && m_serialNumber.GetLength() == 0)
	{
		AfxMessageBox( IDS_MUSTSETSERIAL ) ;
		pDX -> Fail() ;
	}
    if (pDX->m_bSaveAndValidate && m_type && m_stationControl.GetCurSel() == CB_ERR && !m_calibrationSetup)
    {
        AfxMessageBox( IDS_MUSTSETSTATION ) ;
        pDX -> Fail() ;
    }
	if (pDX->m_bSaveAndValidate && (m_type == UnitType::Gun || m_type == UnitType::Theo) && m_adapterNumberString.GetLength() == 0)
	{
		AfxMessageBox( IDS_MUSTSETADAPTER ) ;
		pDX->Fail() ;
	}
	//DDX_CBIndex(pDX, IDC_SENSOR_TYPE, m_type);
	//if (m_calibrationSetup && pDX -> m_bSaveAndValidate && 2 == m_type)
	//{
	//	m_type = 3 ;
	//}
	//DDX_Control(pDX, IDC_SENSOR_TYPE, m_typeControl) ;
	//DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzimuthTitleControl);
}

BEGIN_MESSAGE_MAP(SensorAlignmentPage, CPropertyPage)
    ON_CBN_SELCHANGE(IDC_SENSOR_TYPE, OnCbnSelchangeSensorType)
    ON_CBN_SELCHANGE(IDC_STATION_COMBO, OnCbnSelchangeStation)
END_MESSAGE_MAP()

void SensorAlignmentPage::SetupStations()
{
    m_stationControl.ResetContent();
    
    vector<CString> stations;
    DBInterface::Instance()->GetStationsOnShip(stations, SysSetup->GetShipName());
    vector<CString>::iterator iter;
    for(iter=stations.begin();iter!=stations.end();iter++)
    {
        int index = m_stationControl.AddString(*iter) ;
    }

    m_stationControl.SelectString(0, m_description);    
}

void SensorAlignmentPage::SetupSerialNumber( int type )
{
    m_serialNumberControl.ResetContent() ;
    
    vector<CString> sensors;
    DBInterface::Instance()->GetCalibratedSensors(sensors);
    vector<CString>::iterator iter;
    for(iter=sensors.begin();iter!=sensors.end();iter++)
    {
        int index = m_serialNumberControl.AddString(*iter) ;    
    }
    
    m_serialNumberControl.SelectString(0, m_serialNumber);
}

void SensorAlignmentPage::SetupAdapterNumber( UnitType::Types type )
{
    m_adapterSerialNumber.ResetContent() ;

    vector<CString> adapters;
    if(type == UnitType::Gun || type == UnitType::Theo)
	{
		DBInterface::Instance()->GetCalibratedAdapters(type, adapters);
	}	
	else
	{
		return;
	}
    vector<CString>::iterator iter;
    for(iter=adapters.begin();iter!=adapters.end();iter++)
    {
        int index = m_adapterSerialNumber.AddString(*iter) ;
    }

    m_adapterSerialNumber.SelectString(0, m_adapterNumberString);
}

BOOL SensorAlignmentPage::OnInitDialog( void )
{
    __super::OnInitDialog() ;
	
	map<UnitType::Types, string>::iterator iter;	
	for(iter = m_typeTextmap.begin();iter!= m_typeTextmap.end();iter++)
	{
		int index = m_typeControl.AddString(iter->second.c_str());
		m_typeControl.SetItemData(index, iter->first);
		TRACE("Add str:%s",iter->second.c_str());
	}
	TRACE("type:%d, %s\n",m_type,(LPCTSTR)UnitType::GetUnitText(m_type));
	m_typeControl.SelectString(0, UnitType::GetUnitText(m_type));

	//Unused;Fixed;Platform;Gun;

	if (m_calibrationSetup)
	{
		m_nominalAzimuthControl.ShowWindow( SW_HIDE ) ;
		m_nominalAzTitle.ShowWindow( SW_HIDE ) ;
		m_typeControl.DeleteString( 2 ) ; // Kludge!
	    //Have no station
        m_description = "N/A";
        m_stationControl.ShowWindow( SW_HIDE );
        m_stationTitle.ShowWindow( SW_HIDE );
    }
	WINDOWPLACEMENT placement ;
	if (m_serialNumberControl.GetWindowPlacement( &placement ))
	{
		m_sensorControlRegion = placement.rcNormalPosition ;
	}
    
	
	SetupSerialNumber( 0 ) ;    
    SetupStations() ;

	OnCbnSelchangeSensorType() ;



    return TRUE ;
}

void SensorAlignmentPage::OnCbnSelchangeStation()
{
    CString str;
    int index;
    if((index = m_stationControl.GetCurSel()) == CB_ERR)
        return;

    m_stationControl.GetLBText(index, str);
    m_description = str;

};

void SensorAlignmentPage::OnCbnSelchangeSensorType()
{	
	m_type = (UnitType::Types)m_typeControl.GetItemData(m_typeControl.GetCurSel());
	TRACE("Curr sel:%d,%d\n",m_typeControl.GetCurSel(), m_type);
	
	if (m_calibrationSetup && m_type == 2)
	{
		m_type = UnitType::Gun;
	}

	UnitType::Types t = (UnitType::Types)m_type;
	switch (t)
    {
		case UnitType::Unused:
        //m_UnitTypeDescriptionControl.EnableWindow( FALSE ) ;
        m_stationControl.EnableWindow( FALSE ) ;
        m_serialNumberControl.EnableWindow( FALSE ) ;
        m_adapterSerialNumber.EnableWindow( FALSE ) ;
        m_nominalAzimuthControl.EnableWindow( FALSE ) ;
        m_overrangeDetectionControl.EnableWindow( FALSE ) ;
        break ;
  
        case UnitType::Fixed:
		   
			//m_serialNumberControl.Clear() ;
			//SetupSerialNumber( 1 ) ;
		    
            //m_UnitTypeDescriptionControl.EnableWindow( TRUE ) ;
            m_stationControl.EnableWindow( TRUE ) ;
            m_serialNumberControl.EnableWindow( TRUE ) ;
            m_adapterSerialNumber.EnableWindow( FALSE ) ;
            m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
            m_overrangeDetectionControl.EnableWindow( TRUE ) ;
            break ;

        case UnitType::Platform:
		     
			//m_serialNumberControl.Clear() ;
			//SetupSerialNumber( 2 ) ;
		     
            //m_UnitTypeDescriptionControl.EnableWindow( TRUE ) ;
            m_stationControl.EnableWindow( TRUE ) ;
            m_serialNumberControl.EnableWindow( TRUE ) ;
            m_adapterSerialNumber.EnableWindow( m_calibrationSetup ) ;
            m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
            m_overrangeDetectionControl.EnableWindow( TRUE ) ;
        break ;

		case UnitType::Gun:
            //m_UnitTypeDescriptionControl.EnableWindow( TRUE ) ;
            SetupAdapterNumber(t) ;
			m_stationControl.EnableWindow( TRUE ) ;
            m_serialNumberControl.EnableWindow( TRUE ) ;
            m_adapterSerialNumber.EnableWindow( TRUE ) ;
            m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
            m_overrangeDetectionControl.EnableWindow( TRUE ) ;
            break ;

		case UnitType::Theo: // Theo
						
			SetupAdapterNumber(t) ;
		    
            //m_UnitTypeDescriptionControl.EnableWindow( TRUE ) ;
            m_stationControl.EnableWindow( TRUE ) ;
            m_serialNumberControl.EnableWindow( TRUE ) ;
			m_adapterSerialNumber.EnableWindow( TRUE ) ;            
            m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
            m_overrangeDetectionControl.EnableWindow( TRUE ) ;
			
			break;

        default:
            ASSERT(0) ;
            break ;
     }
}
