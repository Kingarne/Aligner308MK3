// $Id: DAUCalibrationPage2.cpp,v 1.4 2012/08/22 13:09:02 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
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
    MoveDlgToRightSideOfApp(pSheet);
    pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
    m_result.clear() ;
    for (vector<CalibrationData>::iterator i = pSheet -> m_sensorData.begin() ; i != pSheet -> m_sensorData.end() ; i++)
    {
        m_result.push_back( DAUOffsetCalibrationResult() ) ;
        vector<DAUOffsetCalibrationResult>::reverse_iterator m = m_result.rbegin() ;
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

static BOOL SaveCalibrationData( DAUOffsetCalibrationResult &result, CString &serialNumber, CString &channel )
{
    return DBInterface::Instance()->UpdateDAUChannelOffset(serialNumber, channel, result);
}

BOOL DAUCalibrationPage2::OnWizardFinish( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  BOOL ok = TRUE ;
  for (int i = 0 ; i < (int)(m_result.size()) ; i++)
  {
    if (!SaveCalibrationData( m_result[i], DAU::GetDAU().GetSerialNumber(), pSheet->m_sensorData[i].m_pSource->GetName() ))
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
