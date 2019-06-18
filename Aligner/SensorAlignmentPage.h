// $Id: SensorAlignmentPage.h,v 1.4 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "ColorStatic.h"
#include <map>
using namespace std;

class SensorAlignmentPage : public CPropertyPage
{
public:
	SensorAlignmentPage( BOOL calibrationConfig = FALSE ) ;
	virtual ~SensorAlignmentPage( void ) ;

	UnitType::Types m_type ;
    CString m_description ;    
    CString m_adapterNumberString ;
    CString m_serialNumber ;
    double m_nominalAzimuth ;
    BOOL m_overrangeDetection ;

protected:
	enum { IDD1 = IDD_SENSORALIGNMENT_PROPERTYPAGE } ;
	enum { IDD2 = IDD_SENSORCALIBRATION_PROPERTYPAGE } ;

	virtual void DoDataExchange( CDataExchange *pDX ) ;
    virtual BOOL OnInitDialog( void ) ;
    afx_msg void OnCbnSelchangeSensorType( void ) ;
    afx_msg void OnCbnSelchangeStation( void ) ;    
    void SetupSerialNumber( int type ) ;
    void SetupAdapterNumber( UnitType::Types type ) ;
    void SetupStations();

    CComboBox m_serialNumberControl ;
    CComboBox m_typeControl ;
    CComboBox m_adapterSerialNumber;
    CComboBox m_stationControl;
	map<UnitType::Types, string> m_typeTextmap;

    CEdit m_UnitTypeDescriptionControl ;
    CButton m_overrangeDetectionControl ;    
    CEdit m_nominalAzimuthControl ;
    
	BOOL m_calibrationSetup ;
	CRect m_sensorControlRegion ;

    ColorStatic m_typeTitle;
    ColorStatic m_stationTitle;
    ColorStatic m_nominalAzTitle;
    ColorStatic m_sensorSNTitle;
    ColorStatic m_adaptorSNTitle;    	

	

    DECLARE_DYNAMIC(SensorAlignmentPage)
    DECLARE_MESSAGE_MAP()
} ;
