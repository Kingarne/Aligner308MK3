// $Id: AdapterCalibrationPage9.cpp,v 1.5 2014-07-11 09:29:30 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AdapterCalibrationPage9.h"
#include "CalibrationSheet.h"

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
    
    MoveDlgToRightSideOfApp(pSheet);

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

BOOL AdapterCalibrationPage9::OnWizardFinish( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

    for (int i = 0 ; i < (int)(m_adapter.size()) ; i++)
    {
        DBInterface::Instance()->UpdateAdapterCalibration(pSheet->m_sensorData[i].m_pSource->GetAdapterSerialNumber(), m_adapter[i].m_elevation, m_adapter[i].m_azimuth);      
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
