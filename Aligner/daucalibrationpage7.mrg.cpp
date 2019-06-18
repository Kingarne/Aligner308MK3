// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\daucalibrationpage7.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:22
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: daucalibrationpage7.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
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
#include "DAUCalibrationPage7.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage7, CPropertyPage)

DAUCalibrationPage7::DAUCalibrationPage7( void ) : CPropertyPage(DAUCalibrationPage7::IDD)
{
}

DAUCalibrationPage7::~DAUCalibrationPage7( void )
{
}

void DAUCalibrationPage7::DoDataExchange( CDataExchange *pDX )
{
	__super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage7, CPropertyPage)
END_MESSAGE_MAP()

BOOL DAUCalibrationPage7::OnSetActive( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
  m_result.clear() ;
  double voltageDifference = pSheet -> m_voltage_0  - pSheet -> m_voltage_1 ;
  double voltPerLSB = (2.0 *  ( pSheet -> m_voltage_ref + 0.2 )) / 65536  ;
  
  for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
  {
    m_result.push_back( DAUScaleCalibrationResult() ) ;
    vector<DAUScaleCalibrationResult>::reverse_iterator m = /*+++ symbol was renamed:  m_result.rbegin() ;
    double scale = -voltPerLSB / voltageDifference ;
    m -> m_pitchGain = scale * (i -> m_pitch [0] - i -> m_pitch [1]) ;
    m -> m_rollGain = scale * (i -> m_roll [0] - i -> m_roll [1]) ;
  }
  m_pGrid -> ReBind() ;
  return __super::OnSetActive() ;
}

BOOL DAUCalibrationPage7::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT DAUCalibrationPage7::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(DAUCalibrationPage7, CPropertyPage)
  ON_EVENT(DAUCalibrationPage7, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(DAUCalibrationPage7, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage7, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage7, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(DAUCalibrationPage7, IDC_GRID, 1, AfterColUpdate, VTS_I2)
END_EVENTSINK_MAP()

/**
*  This event handler is called by the grid before a cell is edited and is
*  implemented here to prevent editing of cells other than save column cells.
*/
void DAUCalibrationPage7::BeforeColEdit( short index, short key, short *pCancel )
{
  *pCancel = TRUE ;
}

static CString FormatDouble( double value )
{
  CString text ;
  text.Format( "%7.5f", value ) ;
  return text ;
}

void DAUCalibrationPage7::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
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
    pValue -> bstrVal = _bstr_t( FormatDouble( m_result [pBookmark -> lVal].m_rollGain ) ).copy() ;
    break ;

  case 2:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_result [pBookmark -> lVal].m_pitchGain ) ).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void DAUCalibrationPage7::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
}

void DAUCalibrationPage7::SetNewLocation( VARIANT *pNewLocation, int location )
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

void DAUCalibrationPage7::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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

void DAUCalibrationPage7::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

[
  db_command { UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = ?, pitchGain = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? } 
]
static class UpdateChannelGain
 :---*/ _UpdateChannelGainAccessor {
public:
  PARM_DB_V(1, double, rollGain) ;
  PARM_DB_V(2, double, pitchGain) ;
  PARM_DB_L(3, TCHAR, serialNumber, 4) ;
  PARM_DB_L(4, TCHAR, channel, 4) ;
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 174 ".\\DAUCalibrationPage7.cpp"

				DEFINE_COMMAND_EX(_UpdateChannelGainAccessor, L" UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = ?, pitchGain = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? ")
			
#injected_line 174 ".\\DAUCalibrationPage7.cpp"

					BEGIN_PARAM_MAP(_UpdateChannelGainAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_rollGain),  _SIZE_TYPE(m_rollGain), 0, 0, offsetbuf(m_rollGain), 0, offsetbuf(m_status_rollGain))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_pitchGain),  _SIZE_TYPE(m_pitchGain), 0, 0, offsetbuf(m_pitchGain), 0, offsetbuf(m_status_pitchGain))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 174 ".\\DAUCalibrationPage7.cpp"

class UpdateChannelGain;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 174 ".\\DAUCalibrationPage7.cpp"

class UpdateChannelGain : public CCommand<CAccessor<_UpdateChannelGainAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = ?, pitchGain = ? WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=? And [DAUSetupSensorChannel].[name]=? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


static BOOL SaveCalibrationData( DAUScaleCalibrationResult &result, CString &serialNumber, CString &channel )
{
  UpdateChannelGain table ;
  SET_DB_VALUE_NOCHECK(table,rollGain,result.m_rollGain) ;
  SET_DB_VALUE_NOCHECK(table,pitchGain,result.m_pitchGain) ;
  SET_DB_STRING(table,serialNumber,serialNumber) ;
  SET_DB_STRING(table,channel,channel) ;
  HRESULT hResult = ::OpenTable( table ) ;
  return SUCCEEDED(hResult) ;
}

BOOL DAUCalibrationPage7::OnWizardFinish( void )
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
