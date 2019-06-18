// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\daucalibrationpage2.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:25
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: daucalibrationpage2.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "Resource.h"
#include "DAUCalibrationPage2.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage2, CPropertyPage)

DAUCalibrationPage2::DAUCalibrationPage2( void ) : CPropertyPage(DAUCalibrationPage2::IDD)
{
}

DAUCalibrationPage2::~DAUCalibrationPage2( void )
{
}

void DAUCalibrationPage2::DoDataExchange( CDataExchange *pDX )
{
	__super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage2, CPropertyPage)
END_MESSAGE_MAP()

BOOL DAUCalibrationPage2::OnSetActive( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
  m_result.clear() ;
  for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
  {
    m_result.push_back( DAUOffsetCalibrationResult() ) ;
    vector<DAUOffsetCalibrationResult>::reverse_iterator m = /*+++ symbol was renamed:  m_result.rbegin() ;
    m -> m_pitchOffset = i -> m_pitch [0] ;
    m -> m_rollOffset = i -> m_roll [0] ;
  }
  m_pGrid -> ReBind() ;
  return __super::OnSetActive() ;
}

BOOL DAUCalibrationPage2::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT DAUCalibrationPage2::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(DAUCalibrationPage2, CPropertyPage)
  ON_EVENT(DAUCalibrationPage2, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(DAUCalibrationPage2, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage2, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage2, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(DAUCalibrationPage2, IDC_GRID, 1, AfterColUpdate, VTS_I2)
END_EVENTSINK_MAP()

/**
*  This event handler is called by the grid before a cell is edited and is
*  implemented here to prevent editing of cells other than save column cells.
*/
void DAUCalibrationPage2::BeforeColEdit( short index, short key, short *pCancel )
{
  *pCancel = TRUE ;
}

static CString FormatInt( int value )
{
  CString text ;
  text.Format( "%5d", value ) ;
  return text ;
}

void DAUCalibrationPage2::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  USES_CONVERSION ;
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  ASSERT(VT_I4 == pBookmark -> vt) ;

  switch (column)
  {
  case 0:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( pSheet -> m_sensorData [pBookmark -> lVal].m_pSource -> GetName() ).copy() ;
    break ;

  case 1:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatInt( m_result [pBookmark -> lVal].m_rollOffset ) ).copy() ;
    break ;

  case 2:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatInt( m_result [pBookmark -> lVal].m_pitchOffset ) ).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void DAUCalibrationPage2::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
}

void DAUCalibrationPage2::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (location < 0 || location >= (int)(m_result.size()) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void DAUCalibrationPage2::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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
      SetNewLocation( pNewLocation, (int)(m_result.size() + offset)) ;
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

void DAUCalibrationPage2::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

[
  db_command { UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = ?, pitchOffset = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? } 
]
static class UpdateChannelOffset
 :---*/ _UpdateChannelOffsetAccessor {
public:
  PARM_DB_V(1, double, rollOffset) ;
  PARM_DB_V(2, double, pitchOffset) ;
  PARM_DB_L(3, TCHAR, serialNumber, 4) ;
  PARM_DB_L(4, TCHAR, channel, 4) ;
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 170 ".\\DAUCalibrationPage2.cpp"

				DEFINE_COMMAND_EX(_UpdateChannelOffsetAccessor, L" UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = ?, pitchOffset = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? ")
			
#injected_line 170 ".\\DAUCalibrationPage2.cpp"

					BEGIN_PARAM_MAP(_UpdateChannelOffsetAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_rollOffset),  _SIZE_TYPE(m_rollOffset), 0, 0, offsetbuf(m_rollOffset), 0, offsetbuf(m_status_rollOffset))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_pitchOffset),  _SIZE_TYPE(m_pitchOffset), 0, 0, offsetbuf(m_pitchOffset), 0, offsetbuf(m_status_pitchOffset))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 170 ".\\DAUCalibrationPage2.cpp"

class UpdateChannelOffset;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 170 ".\\DAUCalibrationPage2.cpp"

class UpdateChannelOffset : public CCommand<CAccessor<_UpdateChannelOffsetAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = ?, pitchOffset = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


static BOOL SaveCalibrationData( DAUOffsetCalibrationResult &result, CString &serialNumber, CString &channel )
{
  UpdateChannelOffset table ;
  SET_DB_VALUE_NOCHECK(table,rollOffset,result.m_rollOffset) ;
  SET_DB_VALUE_NOCHECK(table,pitchOffset,result.m_pitchOffset) ;
  SET_DB_STRING(table,serialNumber,serialNumber) ;
  SET_DB_STRING(table,channel,channel) ;
  return SUCCEEDED(::OpenTable( table )) ;
}

BOOL DAUCalibrationPage2::OnWizardFinish( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  BOOL ok = TRUE ;
  for (int i = 0 ; i < (int)(m_result.size()) ; i++)
  {
    if (!SaveCalibrationData( m_result [i], DAU::GetDAU().GetSerialNumber(), pSheet -> m_sensorData [i].m_pSource -> GetName() ))
    {
      ok =FALSE ;
    }
  }
  if (ok)
  {
    ::AfxMessageBox( _T("Succesfully stored calibration parameters.") ) ;
  }
  else
  {
    ::AfxMessageBox( _T("Error storing calibration parameters."), MB_ICONERROR ) ;
  }
  return __super::OnWizardFinish() ;
}
