// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\adaptercalibrationpage9.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:33
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: adaptercalibrationpage9.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "AdapterCalibrationPage9.h"
#include "CalibrationSheet.h"
#include "SensorDatabase.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage9, CPropertyPage)

AdapterCalibrationPage9::AdapterCalibrationPage9( void ) : CPropertyPage(AdapterCalibrationPage9::IDD)
{
  // Empty
}

AdapterCalibrationPage9::~AdapterCalibrationPage9( void )
{
  // Empty
}

void AdapterCalibrationPage9::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage9, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage9::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage9::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_FINISH | PSWIZB_BACK ) ;
  return __super::OnSetActive() ;
}

LRESULT AdapterCalibrationPage9::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (pSheet -> IsMeasuring())
  {
    pSheet -> StopMeasurement() ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
    return -1 ;
  }
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BOOL AdapterCalibrationPage9::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(AdapterCalibrationPage9, CPropertyPage)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(AdapterCalibrationPage9, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than save column cells.
 */
void AdapterCalibrationPage9::BeforeColEdit( short index, short key, short *pCancel )
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
void AdapterCalibrationPage9::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  USES_CONVERSION ;
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  ASSERT(VT_I4 == pBookmark -> vt) ;

  switch (column)
  {
  case 0:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( pSheet -> m_sensorData [pBookmark -> lVal].m_pSource -> GetAdapterSerialNumber() ).copy() ;
    break ;

  case 1:
    pValue -> vt = VT_I4 ;
    pValue -> lVal = m_adapter [pBookmark -> lVal].m_saveResult ;
    break ;

  case 2:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_adapter [pBookmark -> lVal].m_elevation ) ).copy() ;
    break ;

  case 3:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_adapter[pBookmark -> lVal].m_azimuth ) ).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
	}
}

void AdapterCalibrationPage9::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
  switch (column)
  {
  case 1:
    m_adapter [pBookmark -> lVal].m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void AdapterCalibrationPage9::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (location < 0 || location >= (int)(m_adapter.size()) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void AdapterCalibrationPage9::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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
      SetNewLocation( pNewLocation, (int)(m_adapter.size() + offset )) ;
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

void AdapterCalibrationPage9::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

template <class T, class S>
void UpdateData( T &insert, S &remove, CString &serialNumber, double elevation, double azimuth )
{
  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_VALUE(insert, elevation, elevation ) ;
    SET_DB_VALUE(insert, azimuth, azimuth ) ;
    hResult = ::OpenTable( insert ) ;
    if (FAILED(hResult))
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    else
    {
      CString text ;
      text.Format( "Succesfully stored model parameters for gun adapter S/N %s.", serialNumber ) ;
      ::AfxMessageBox( text ) ;
      insert.Close() ;
    }
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

[
  db_command { INSERT INTO GunAdapterCalibration (serialNumber, azimuth, elevation) VALUES (?,?,?) }
]
class /*+++ symbol was renamed:  InsertIntoGunAdapterCalibration
 :---*/ _InsertIntoGunAdapterCalibrationAccessor {
public:
  PARM_DB_L(1, TCHAR, serialNumber, 4) ;
	PARM_DB_V(2, double, azimuth) ;
  PARM_DB_V(3, double, elevation) ;
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 231 ".\\AdapterCalibrationPage9.cpp"

				DEFINE_COMMAND_EX(_InsertIntoGunAdapterCalibrationAccessor, L" INSERT INTO GunAdapterCalibration (serialNumber, azimuth, elevation) VALUES (?,?,?) ")
			
#injected_line 231 ".\\AdapterCalibrationPage9.cpp"

					BEGIN_PARAM_MAP(_InsertIntoGunAdapterCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 231 ".\\AdapterCalibrationPage9.cpp"

class InsertIntoGunAdapterCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 231 ".\\AdapterCalibrationPage9.cpp"

class InsertIntoGunAdapterCalibration : public CCommand<CAccessor<_InsertIntoGunAdapterCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO GunAdapterCalibration (serialNumber, azimuth, elevation) VALUES (?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { DELETE FROM GunAdapterCalibration WHERE serialNumber = ? }
]
class /*+++ symbol was renamed:  DeleteGunAdapterCalibration
 :---*/ _DeleteGunAdapterCalibrationAccessor {
  public: PARM_DB_L(1, TCHAR, serialNumber, 4)

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 249 ".\\AdapterCalibrationPage9.cpp"

				DEFINE_COMMAND_EX(_DeleteGunAdapterCalibrationAccessor, L" DELETE FROM GunAdapterCalibration WHERE serialNumber = ? ")
			
#injected_line 249 ".\\AdapterCalibrationPage9.cpp"

					BEGIN_PARAM_MAP(_DeleteGunAdapterCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 249 ".\\AdapterCalibrationPage9.cpp"

class DeleteGunAdapterCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 249 ".\\AdapterCalibrationPage9.cpp"

class DeleteGunAdapterCalibration : public CCommand<CAccessor<_DeleteGunAdapterCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM GunAdapterCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


BOOL AdapterCalibrationPage9::OnWizardFinish( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  for (int i = 0 ; i < (int)(m_adapter.size()) ; i++)
  {
    ::UpdateData( InsertIntoGunAdapterCalibration(), DeleteGunAdapterCalibration(), pSheet -> m_sensorData [i].m_pSource -> GetAdapterSerialNumber(), m_adapter [i].m_elevation, m_adapter [i].m_azimuth ) ;
  }
  return __super::OnWizardFinish() ;
}

void AdapterCalibrationPage9::OnInitGrid( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  m_adapter.clear() ;
  for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
  {
    m_adapter.push_back( AdapterCalibrationResult() ) ;
    vector<AdapterCalibrationResult>::reverse_iterator m = m_adapter.rbegin() ;
    m -> m_elevation = UnitsToMilliunits( ((i -> m_pitch [1] - pSheet -> m_referenceData.m_pitch [1]) - (i -> m_pitch [0] - pSheet -> m_referenceData.m_pitch [0])) / 2.0 ) ;
    double tmpDiv = ((i -> m_roll [3] - pSheet -> m_referenceData.m_roll [3]) - (i -> m_roll [2] - pSheet -> m_referenceData.m_roll [2]));
    if( fabs(tmpDiv) > DBL_EPSILON )
    {
      m -> m_azimuth   = - UnitsToMilliunits( ((i -> m_pitch [2] - pSheet -> m_referenceData.m_pitch [2]) - (i -> m_pitch [3] - pSheet -> m_referenceData.m_pitch [3])) / tmpDiv ) ;
    }
    else
    {
      m -> m_azimuth = DBL_MAX;
    }
  }
}
