// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\tiltcalibrationpage4.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:05
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: tiltcalibrationpage4.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
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
#include "TiltCalibrationPage4.h"
#include "SensorDatabase.h"

IMPLEMENT_DYNAMIC(TiltCalibrationPage4, CPropertyPage)

TiltCalibrationPage4::TiltCalibrationPage4( void ) : CPropertyPage(TiltCalibrationPage4::IDD)
{
  // Empty
}

TiltCalibrationPage4::~TiltCalibrationPage4( void )
{
  // Empty
}

void TiltCalibrationPage4::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage4, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage4::OnSetActive( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> PlotConverted( 3 ) ;
  pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
  return __super::OnSetActive() ;
}

BOOL TiltCalibrationPage4::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT TiltCalibrationPage4::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(TiltCalibrationPage4, CPropertyPage)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(TiltCalibrationPage4, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than save column cells.
 */
void TiltCalibrationPage4::BeforeColEdit( short index, short key, short *pCancel )
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

// TODO: Please refactor this kludge. Its far to long!
void TiltCalibrationPage4::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  USES_CONVERSION ;
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  ASSERT(VT_I4 == pBookmark -> vt) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  if (0 == pBookmark -> lVal)
  {
    switch (column)
    {
    case 0:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( pSheet -> m_referenceData.m_pSource -> GetSerialNumber() ).copy() ;
      break ;

    case 1:
      pValue -> vt = VT_I4 ;
      pValue -> lVal = m_referenceResult.m_saveResult ;
      break ;

    case 2:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_sensorRollOffset ) ).copy() ;
      break ;

    case 3:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_sensorPitchOffset ) ).copy() ;
      break ;

    case 4:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( _T("Ref") ).copy() ;
      break ;

    case 5:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( _T("Ref") ).copy() ;
      break ;

    case 6:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_temperature ) ).copy() ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
  }
  else
  {
    switch (column)
    {
    case 0:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( pSheet -> m_sensorData [pBookmark -> lVal - 1].m_pSource -> GetSerialNumber() ).copy() ;
      break ;

    case 1:
      pValue -> vt = VT_I4 ;
      pValue -> lVal = m_sensorResult [pBookmark -> lVal - 1].m_saveResult ;
      break ;

    case 2:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark -> lVal - 1].m_sensorRollOffset ) ).copy() ;
      break ;

    case 3:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark -> lVal - 1].m_sensorPitchOffset ) ).copy() ;
      break ;

    case 4:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark -> lVal - 1].m_flatRollOffset ) ).copy() ;
      break ;

    case 5:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark -> lVal - 1].m_flatPitchOffset ) ).copy() ;
      break ;

    case 6:
      pValue -> vt = VT_BSTR ;
      pValue -> bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark -> lVal - 1].m_temperature ) ).copy() ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
  }
}

void TiltCalibrationPage4::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
  switch (column)
  {
  case 1:
    if (0 == pBookmark -> lVal)
    {
      ASSERT(VT_BSTR == pValue -> vt) ;
      m_referenceResult.m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
    }
    else
    {
      m_sensorResult [pBookmark -> lVal - 1].m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
    }
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void TiltCalibrationPage4::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (location < 0 || location >= (int)(m_sensorResult.size() + 1) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void TiltCalibrationPage4::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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
      SetNewLocation( pNewLocation, (int)(m_sensorResult.size() + offset + 1 )) ;
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

void TiltCalibrationPage4::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

INSERT_INTO_SENSOR_DATA(SensorRollOffsetData)    :---*/ _InsertIntoSensorRollOffsetDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 255 ".\\TiltCalibrationPage4.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollOffsetDataAccessor, L" INSERT INTO SensorRollOffsetData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 255 ".\\TiltCalibrationPage4.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollOffsetDataAccessor)
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
#injected_line 255 ".\\TiltCalibrationPage4.cpp"

class InsertIntoSensorRollOffsetData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 255 ".\\TiltCalibrationPage4.cpp"

class InsertIntoSensorRollOffsetData : public CCommand<CAccessor<_InsertIntoSensorRollOffsetDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollOffsetData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_DATA(SensorPitchOffsetData)   :---*/ _InsertIntoSensorPitchOffsetDataAccessor { INSERT_INTO_SENSOR_DATA_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 256 ".\\TiltCalibrationPage4.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchOffsetDataAccessor, L" INSERT INTO SensorPitchOffsetData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ")
			
#injected_line 256 ".\\TiltCalibrationPage4.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchOffsetDataAccessor)
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
#injected_line 256 ".\\TiltCalibrationPage4.cpp"

class InsertIntoSensorPitchOffsetData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 256 ".\\TiltCalibrationPage4.cpp"

class InsertIntoSensorPitchOffsetData : public CCommand<CAccessor<_InsertIntoSensorPitchOffsetDataAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchOffsetData ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


static void SaveCalibrationData( TiltCalibrationResult &result, CString serialNumber )
{
  if (result.m_saveResult)
  {
    InsertData( InsertIntoSensorRollOffsetData(), serialNumber, result.m_temperature, result.m_sensorRollOffset ) ;
    InsertData( InsertIntoSensorPitchOffsetData(), serialNumber, result.m_temperature, result.m_sensorPitchOffset ) ;
  }
}

BOOL TiltCalibrationPage4::OnWizardFinish( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  SaveCalibrationData( m_referenceResult, pSheet -> m_referenceData.m_pSource -> GetSerialNumber() ) ;
  for (int i = 0 ; i < (int)(m_sensorResult.size()) ; i++)
  {
     SaveCalibrationData( m_sensorResult [i], pSheet -> m_sensorData [i].m_pSource -> GetSerialNumber() ) ;
  }
  return __super::OnWizardFinish() ;
}

void TiltCalibrationPage4::OnInitGrid( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  m_sensorResult.clear() ;
  for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
  {
    m_sensorResult.push_back( TiltCalibrationResult() ) ;
    vector<TiltCalibrationResult>::reverse_iterator m = m_sensorResult.rbegin() ;
    m -> m_sensorRollOffset  = -UnitsToMilliunits( (pSheet -> m_referenceData.m_roll [2]  - i -> m_roll [2]  - pSheet -> m_referenceData.m_roll [1]  - i -> m_roll [1]) / 2 ) ;
    m -> m_flatRollOffset    = -UnitsToMilliunits( (pSheet -> m_referenceData.m_roll [0]  - i -> m_roll [0]  - pSheet -> m_referenceData.m_roll [2]  + i -> m_roll [2]) / 2 ) ;
    m -> m_sensorPitchOffset = -UnitsToMilliunits( (pSheet -> m_referenceData.m_pitch [2] - i -> m_pitch [2] - pSheet -> m_referenceData.m_pitch [1] - i -> m_pitch [1]) / 2 ) ;
    m -> m_flatPitchOffset   = -UnitsToMilliunits( (pSheet -> m_referenceData.m_pitch [2] - i -> m_pitch [2] - pSheet -> m_referenceData.m_pitch [0] + i -> m_pitch [0]) / 2 ) ;
    m -> m_temperature = (i -> m_temperature [0] + i -> m_temperature [1] + i -> m_temperature [2]) / 3 ;
  }
  m_referenceResult.m_sensorRollOffset =  -UnitsToMilliunits( (- pSheet -> m_referenceData.m_roll [1]  - pSheet -> m_sensorData.begin() -> m_roll [1]  - pSheet -> m_referenceData.m_roll [0]  + pSheet -> m_sensorData.begin() -> m_roll [0]) / 2 ) ;
  m_referenceResult.m_sensorPitchOffset = -UnitsToMilliunits( (- pSheet -> m_referenceData.m_pitch [1] - pSheet -> m_sensorData.begin() -> m_pitch [1] - pSheet -> m_referenceData.m_pitch [0] + pSheet -> m_sensorData.begin() -> m_pitch [0]) / 2 ) ;
  m_referenceResult.m_temperature = (pSheet -> m_referenceData.m_temperature [0] + pSheet -> m_referenceData.m_temperature [1] + pSheet -> m_referenceData.m_temperature [2]) / 3 ;
  m_referenceResult.m_flatRollOffset = 0 ;
  m_referenceResult.m_flatPitchOffset = 0 ;
}
