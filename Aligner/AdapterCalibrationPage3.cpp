// $Id: AdapterCalibrationPage3.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AdapterCalibrationPage3.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(AdapterCalibrationPage3, CPropertyPage)

AdapterCalibrationPage3::AdapterCalibrationPage3( int measurmentToCheck ) : CPropertyPage(AdapterCalibrationPage3::IDD)
{
  m_measurmentToCheck = measurmentToCheck ;
}

AdapterCalibrationPage3::~AdapterCalibrationPage3( void )
{
  // Empty
}

void AdapterCalibrationPage3::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(AdapterCalibrationPage3, CPropertyPage)
END_MESSAGE_MAP()

BOOL AdapterCalibrationPage3::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL AdapterCalibrationPage3::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return __super::OnSetActive() ;
}

LRESULT AdapterCalibrationPage3::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return __super::OnWizardNext() ;
}

LRESULT AdapterCalibrationPage3::OnWizardBack( void )
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

BOOL AdapterCalibrationPage3::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(AdapterCalibrationPage3, CPropertyPage)
  ON_EVENT(AdapterCalibrationPage3, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(AdapterCalibrationPage3, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(AdapterCalibrationPage3, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(AdapterCalibrationPage3, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(AdapterCalibrationPage3, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells.
 */
void AdapterCalibrationPage3::BeforeColEdit( short, short, short *pCancel )
{
  *pCancel = TRUE ;
}

static CString FormatDouble( double value )
{
  CString text ;
  text.Format( "%8.5f", value ) ;
  return text ;
}

// TODO: Please refactor this kludge. Its far to long!
void AdapterCalibrationPage3::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
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
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( m_inclination [pBookmark -> lVal].m_roll ) ).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
	}
}

void AdapterCalibrationPage3::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (location < 0 || location >= (int)(m_inclination.size()) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void AdapterCalibrationPage3::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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
      SetNewLocation( pNewLocation, (int)(m_inclination.size() + offset )) ;
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

void AdapterCalibrationPage3::AfterColUpdate( short index )
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
    if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

void AdapterCalibrationPage3::OnInitGrid( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  m_inclination.clear() ;
  for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
  {
    m_inclination.push_back( InclinationResult() ) ;
    vector<InclinationResult>::reverse_iterator m = m_inclination.rbegin() ;
    m -> m_roll = (i -> m_roll [m_measurmentToCheck] - pSheet -> m_referenceData.m_roll [m_measurmentToCheck]) ;
  }
}
