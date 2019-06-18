// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\gaincalibrationpage5.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:16
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: gaincalibrationpage5.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "CalibrationSheet.h"
#include "CreatePolynomial.h"
#include "GainAzimuthCalibration.h"
#include "GainCalibrationPage5.h"
#include "SensorDatabase.h"

#define MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT    2.0 //[mrad]
#define MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT   2.0 //[mrad]

IMPLEMENT_DYNAMIC(GainCalibrationPage5, CPropertyPage)

GainCalibrationPage5::GainCalibrationPage5( void ) : CPropertyPage(GainCalibrationPage5::IDD)
{
  // Empty
}

GainCalibrationPage5::~GainCalibrationPage5( void )
{
  // Empty
}

void GainCalibrationPage5::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(GainCalibrationPage5, CPropertyPage)
END_MESSAGE_MAP()

BOOL GainCalibrationPage5::OnSetActive( void )
{
  GainAzimuthCalibration *pSheet = static_cast<GainAzimuthCalibration *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(GainAzimuthCalibration) )) ;
  pSheet -> PlotConverted( 1 ) ;
  pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
  if (!pSheet -> VerifyStability( 2, 3, FALSE))
  {
    ::AfxMessageBox( _T("The calibration fixture has moved more than 1 mrad between measurements\nPress the back button to redo the last measurment") ) ;
    pSheet -> SetWizardButtons( PSWIZB_BACK ) ;
  }
  return __super::OnSetActive() ;
}

BOOL GainCalibrationPage5::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT GainCalibrationPage5::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2) ;
}

BEGIN_EVENTSINK_MAP(GainCalibrationPage5, CPropertyPage)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(GainCalibrationPage5, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than save column cells.
 */
void GainCalibrationPage5::BeforeColEdit( short index, short key, short *pCancel )
{
  (void)key ;
  if (1 != index)
  {
    *pCancel = TRUE ;
  }
}

static CString FormatDouble( double value )
{
  CString text ;
  text.Format( "%8.5f", value ) ;
  return text ;
}

void GainCalibrationPage5::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  USES_CONVERSION ;
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(VT_I4 == pBookmark -> vt) ;
  ASSERT(pBookmark ->lVal == 0) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  switch (column)
  {
  case 0:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( pSheet -> m_sensorData [0].m_pSource -> GetSerialNumber() ).copy() ;
    break ;

  case 1:
    pValue -> vt = VT_I4 ;
    pValue -> lVal = m_sensor.m_saveResult ;
    break ;

  case 2:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorPitchGain ) ).copy() ;
    break ;

  case 3:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorRollAzimuth ) ).copy() ;
    break ;

  case 4:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_pitchTemperature) ).copy() ;
    break ;

  case 5:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorRollGain ) ).copy() ;
    break ;

  case 6:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorPitchAzimuth ) ).copy() ;
    break ;

  case 7:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_rollTemperature) ).copy() ;
    break ;

  //case 0:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( pSheet -> m_sensorData [0].m_pSource -> GetSerialNumber() ).copy() ;
  //  break ;

  //case 1:
  //  pValue -> vt = VT_I4 ;
  //  pValue -> lVal = m_sensor.m_saveResult ;
  //  break ;

  //case 2:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorRollGain ) ).copy() ;
  //  break ;

  //case 3:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorPitchGain ) ).copy() ;
  //  break ;

  //case 4:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorRollAzimuth ) ).copy() ;
  //  break ;

  //case 5:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_sensorPitchAzimuth ) ).copy() ;
  //  break ;

  //case 6:
  //  pValue -> vt = VT_BSTR ;
  //  pValue -> bstrVal = _bstr_t( FormatDouble( m_sensor.m_pitchTemperature) ).copy() ;
  //  break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void GainCalibrationPage5::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
  ASSERT(0 == pBookmark -> lVal) ;
  ASSERT(1 == column) ;
  m_sensor.m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
}

void GainCalibrationPage5::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (0 != location)
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void GainCalibrationPage5::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
{
  (void)pApproximatePosition ;
  switch (pStartLocation -> vt)
  {
  case VT_NULL:
    if (0 <= offset)
    {
      SetNewLocation( pNewLocation, offset - 1 ) ;
    }
    else
    {
      SetNewLocation( pNewLocation, offset + 2 ) ;
    }
    break ;

  case VT_I4:
    SetNewLocation( pNewLocation, pStartLocation -> lVal + offset ) ;
    break ;

  default:
    ASSERT(0) ;
    pNewLocation -> vt = VT_NULL ;
    break ;
  }
}

void GainCalibrationPage5::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

INSERT_INTO_SENSOR_DATA(SensorPitchGainData)     :---*/ _InsertIntoSensorPitchGainDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 242 ".\\GainCalibrationPage5.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchGainDataAccessor, L" INSERT INTO SensorPitchGainData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 242 ".\\GainCalibrationPage5.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchGainDataAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 242 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorPitchGainData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 242 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorPitchGainData : public CCommand<CAccessor<_InsertIntoSensorPitchGainDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchGainData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_DATA(SensorPitchAzimuthData)  :---*/ _InsertIntoSensorPitchAzimuthDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 243 ".\\GainCalibrationPage5.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchAzimuthDataAccessor, L" INSERT INTO SensorPitchAzimuthData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 243 ".\\GainCalibrationPage5.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchAzimuthDataAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 243 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorPitchAzimuthData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 243 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorPitchAzimuthData : public CCommand<CAccessor<_InsertIntoSensorPitchAzimuthDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchAzimuthData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_DATA(SensorRollGainData)      :---*/ _InsertIntoSensorRollGainDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 244 ".\\GainCalibrationPage5.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollGainDataAccessor, L" INSERT INTO SensorRollGainData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 244 ".\\GainCalibrationPage5.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollGainDataAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 244 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorRollGainData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 244 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorRollGainData : public CCommand<CAccessor<_InsertIntoSensorRollGainDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollGainData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_DATA(SensorRollAzimuthData)   :---*/ _InsertIntoSensorRollAzimuthDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 245 ".\\GainCalibrationPage5.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollAzimuthDataAccessor, L" INSERT INTO SensorRollAzimuthData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 245 ".\\GainCalibrationPage5.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollAzimuthDataAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 245 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorRollAzimuthData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 245 ".\\GainCalibrationPage5.cpp"

class InsertIntoSensorRollAzimuthData : public CCommand<CAccessor<_InsertIntoSensorRollAzimuthDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollAzimuthData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


static void SaveCalibrationData( GainCalibrationResult &result, CString serialNumber )
{
  if (result.m_saveResult)
  {
    InsertData( InsertIntoSensorPitchGainData(), serialNumber, result.m_pitchTemperature, result.m_sensorPitchGain  ) ;
    InsertData( InsertIntoSensorRollGainData(), serialNumber, result.m_rollTemperature, result.m_sensorRollGain ) ;
    InsertData( InsertIntoSensorPitchAzimuthData(), serialNumber, result.m_rollTemperature, result.m_sensorPitchAzimuth ) ;
    InsertData( InsertIntoSensorRollAzimuthData(), serialNumber, result.m_pitchTemperature, result.m_sensorRollAzimuth ) ;
  }
}

void GainCalibrationPage5::SaveDayToDayCalibrationData( GainCalibrationResult &result, CString serialNumber )
{
  if (result.m_saveResult)
  {
    CString text, caption;
    text.LoadString( IDS_AUTOMATICALLY_ADJUST_CALIBRATION_DATA );
    caption.LoadString( IDS_QUESTION_CAPTION );

    if( IDYES == MessageBox( text, caption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
    {
      BOOL doIt = TRUE;
      if( fabs( result.m_sensorRollAzimuth ) > MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT )
      {
        caption.LoadString( IDS_WARNING_CAPTION );
        text.Format( _T("The roll azimuth correction is %.2f > %.2f [mrad]\nContinue?"), result.m_sensorRollAzimuth, MAX_ROLL_AZIMUTH_DAY_TO_DAY_ADJUSTMENT );
        if( IDNO == MessageBox( text, caption, MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON1 ) )
        {
          doIt = FALSE;
        }
      }
      if( doIt == TRUE )
      {
        if( fabs( result.m_sensorPitchAzimuth ) > MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT )
        {
          caption.LoadString( IDS_WARNING_CAPTION );
          text.Format( _T("The pitch azimuth correction is %.2f > %.2f [mrad]\nContinue?"), result.m_sensorPitchAzimuth, MAX_PITCH_AZIMUTH_DAY_TO_DAY_ADJUSTMENT );
          if( IDNO == MessageBox( text, caption, MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON1 ) )
          {
            doIt = FALSE;
          }
        }
      }

      if( doIt == TRUE )
      {
        CreatePolynomial poly;
        poly.UpdateRollAzimuthCalibrationData( serialNumber, result.m_sensorRollAzimuth );
        poly.UpdatePitchAzimuthCalibrationData( serialNumber, result.m_sensorPitchAzimuth );
      }
    }
  }
}

BOOL GainCalibrationPage5::OnWizardFinish( void )
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

void GainCalibrationPage5::OnInitGrid( void )
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
  }
  else
  {
    ASSERT(0) ; // TODO: add proper error control.
  }
}
