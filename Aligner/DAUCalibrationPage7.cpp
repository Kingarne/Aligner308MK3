// $Id: DAUCalibrationPage7.cpp,v 1.3 2011/10/14 15:56:01 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
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
    vector<DAUScaleCalibrationResult>::reverse_iterator m = m_result.rbegin() ;
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

static BOOL SaveCalibrationData( DAUScaleCalibrationResult &result, CString &serialNumber, CString &channel )
{  
    return DBInterface::Instance()->UpdateDAUChannelGain(serialNumber, channel, result);
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
