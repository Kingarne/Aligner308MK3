// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\sensoralignmentpage.mrg.cpp
// compiler-generated file created 05/10/06 at 15:48:40
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: sensoralignmentpage.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "SensorAlignmentPage.h"
#include "SensorDatabase.h"
#include "Database.h"

IMPLEMENT_DYNAMIC(SensorAlignmentPage, CPropertyPage)

static const char *s_pTypeText [] = {
  _T("Unused"), _T("Fixed"), _T("Platform"), _T("Gun")
} ;

CString SensorAlignmentPage::TypetextFromInt( int type )
{
  return type > 0 && type < sizeof(s_pTypeText) / sizeof(char *) ? CString( s_pTypeText [type] ) : CString( _T("") ) ;
}

SensorAlignmentPage::SensorAlignmentPage( BOOL calibrationConfig ) : CPropertyPage( calibrationConfig ? SensorAlignmentPage::IDD2 : SensorAlignmentPage::IDD1)
, m_type(0)
, m_nominalAzimuth(0)
{
	m_calibrationSetup = calibrationConfig ;
}

SensorAlignmentPage::~SensorAlignmentPage( void )
{
	// Empty
}

void SensorAlignmentPage::DoDataExchange( CDataExchange *pDX )
{
	__super::DoDataExchange( pDX ) ;
	DDX_Control(pDX, IDC_SENSOR_DESCRIPTION, m_unitDescriptionControl) ;
	DDX_Text(pDX, IDC_SENSOR_DESCRIPTION, m_description) ;
	DDX_Control(pDX, IDC_SENSOR_SERIAL_NUMBER, m_serialNumberControl) ;
	DDX_Text(pDX, IDC_SENSOR_SERIAL_NUMBER, m_serialNumber) ;
	DDX_Control(pDX, IDC_SENSOR_NOMINAL_AZIMUTH, m_nominalAzimuthControl);
	DDX_Text(pDX, IDC_SENSOR_NOMINAL_AZIMUTH, m_nominalAzimuth);
	DDV_MinMaxDouble(pDX, m_nominalAzimuth, -180, 180);
	DDX_Control(pDX, IDC_ADAPTER_SERIAL_NUMBER, m_adapterSerialNumber);
	DDX_Text(pDX, IDC_ADAPTER_SERIAL_NUMBER, m_adapterNumberString) ;
	DDX_Control(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetectionControl ) ;
	DDX_Check(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetection) ;

	DDX_CBIndex(pDX, IDC_SENSOR_TYPE, m_type);
	if (m_calibrationSetup && pDX -> m_bSaveAndValidate && 2 == m_type)
	{
		m_type = 3 ;
	}
	DDX_Control(pDX, IDC_SENSOR_TYPE, m_typeControl) ;
	DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzimuthTitleControl);

	if (pDX -> m_bSaveAndValidate && m_type && m_serialNumber.GetLength() == 0)
	{
		AfxMessageBox( IDS_MUSTSETSERIAL ) ;
		pDX -> Fail() ;
	}
	if (pDX -> m_bSaveAndValidate && 3 == m_type && m_adapterNumberString.GetLength() == 0)
	{
		AfxMessageBox( IDS_MUSTSETADAPTER ) ;
		pDX -> Fail() ;
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
END_MESSAGE_MAP()

UNIQUE_SN(CalibratedSensors)              :---*/ _UniqueSNFromCalibratedSensorsAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 79 ".\\SensorAlignmentPage.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromCalibratedSensorsAccessor, L" SELECT DISTINCT serialNumber FROM CalibratedSensors ORDER BY serialNumber ")
			
#injected_line 79 ".\\SensorAlignmentPage.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromCalibratedSensorsAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 79 ".\\SensorAlignmentPage.cpp"

class UniqueSNFromCalibratedSensors;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 79 ".\\SensorAlignmentPage.cpp"

class UniqueSNFromCalibratedSensors : public CCommand<CAccessor<_UniqueSNFromCalibratedSensorsAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM CalibratedSensors ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void SensorAlignmentPage::SetupSerialNumber( int type )
{
	m_serialNumberControl.ResetContent() ;
  UniqueSNFromCalibratedSensors table ;
  HRESULT hResult = ::OpenTable( table) ;
  if (S_OK == hResult)
  {
    HRESULT hResult = table.MoveFirst() ;
    while( S_OK == hResult)
    {
      CString text ;
      text.Format( "%03d", atoi(table.m_serialNumber) ) ;
			//if (text != "000" || m_calibrationSetup) //Always allow serial number 000, R2 050413
			//{
				int index = m_serialNumberControl.AddString( text ) ;
				if (m_serialNumber == table.m_serialNumber)
				{
					m_serialNumberControl.SetCurSel( index ) ;
				}
			//}
      hResult = table.MoveNext() ;
    }
  }
  else
  {
    ASSERT(0) ; // TODO: Add proper error handling.
    return ;
  }
}

UNIQUE_SN(GunAdapterCalibration)   :---*/ _UniqueSNFromGunAdapterCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 111 ".\\SensorAlignmentPage.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromGunAdapterCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM GunAdapterCalibration ORDER BY serialNumber ")
			
#injected_line 111 ".\\SensorAlignmentPage.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromGunAdapterCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 111 ".\\SensorAlignmentPage.cpp"

class UniqueSNFromGunAdapterCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 111 ".\\SensorAlignmentPage.cpp"

class UniqueSNFromGunAdapterCalibration : public CCommand<CAccessor<_UniqueSNFromGunAdapterCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM GunAdapterCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void SensorAlignmentPage::SetupAdapterNumber( void )
{
	m_adapterSerialNumber.ResetContent() ;
  UniqueSNFromGunAdapterCalibration table ;
  HRESULT hResult = ::OpenTable( table ) ;
  if (S_OK == hResult)
  {
    HRESULT hResult = table.MoveFirst() ;
    while( S_OK == hResult)
    {
      CString text ;
      text.Format( "%03d", atoi(table.m_serialNumber) ) ;
			if (text != "000")
			{
        int index = m_adapterSerialNumber.AddString( text ) ;
				if (m_adapterNumberString == table.m_serialNumber)
				{
					m_adapterSerialNumber.SetCurSel( index ) ;
				}
			}
      hResult = table.MoveNext() ;
    }
  }
  else
  {
    ASSERT(0) ; // TODO: Add proper error handling.
    return ;
  }
}

BOOL SensorAlignmentPage::OnInitDialog( void )
{
  __super::OnInitDialog() ;
	if (m_calibrationSetup)
	{
		m_nominalAzimuthControl.ShowWindow( SW_HIDE ) ;
		m_nominalAzimuthTitleControl.ShowWindow( SW_HIDE ) ;
		m_typeControl.DeleteString( 2 ) ; // Kludge!
	}
	WINDOWPLACEMENT placement ;
	if (m_serialNumberControl.GetWindowPlacement( &placement ))
	{
		m_sensorControlRegion = placement.rcNormalPosition ;
	}
  SetupSerialNumber( 0 ) ;
  SetupAdapterNumber() ;
  OnCbnSelchangeSensorType() ;
  return TRUE ;
}

void SensorAlignmentPage::OnCbnSelchangeSensorType()
{
  switch (m_typeControl.GetCurSel())
  {
  case 0:
    m_unitDescriptionControl.EnableWindow( FALSE ) ;
    m_serialNumberControl.EnableWindow( FALSE ) ;
    m_adapterSerialNumber.EnableWindow( FALSE ) ;
    m_nominalAzimuthControl.EnableWindow( FALSE ) ;
    m_overrangeDetectionControl.EnableWindow( FALSE ) ;
    break ;
  
  case 1:
		if (m_calibrationSetup)
		{
			m_serialNumberControl.Clear() ;
			SetupSerialNumber( 1 ) ;
		}
    m_unitDescriptionControl.EnableWindow( TRUE ) ;
    m_serialNumberControl.EnableWindow( TRUE ) ;
    m_adapterSerialNumber.EnableWindow( FALSE ) ;
    m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
    m_overrangeDetectionControl.EnableWindow( TRUE ) ;
    break ;

  case 2:
		if (m_calibrationSetup)
		{
			m_serialNumberControl.Clear() ;
			SetupSerialNumber( 2 ) ;
		}
    m_unitDescriptionControl.EnableWindow( TRUE ) ;
    m_serialNumberControl.EnableWindow( TRUE ) ;
    m_adapterSerialNumber.EnableWindow( m_calibrationSetup ) ;
    m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
    m_overrangeDetectionControl.EnableWindow( TRUE ) ;
    break ;

  case 3:
    m_unitDescriptionControl.EnableWindow( TRUE ) ;
    m_serialNumberControl.EnableWindow( TRUE ) ;
    m_adapterSerialNumber.EnableWindow( TRUE ) ;
    m_nominalAzimuthControl.EnableWindow( !m_calibrationSetup ) ;
    m_overrangeDetectionControl.EnableWindow( TRUE ) ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}
