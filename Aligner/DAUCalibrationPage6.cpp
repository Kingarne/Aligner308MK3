// $Id: DAUCalibrationPage6.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "DAUCalibrationPage6.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage6, CPropertyPage)

DAUCalibrationPage6::DAUCalibrationPage6( void ) : CPropertyPage(DAUCalibrationPage6::IDD)
{
}

DAUCalibrationPage6::~DAUCalibrationPage6( void )
{
}

void DAUCalibrationPage6::DoDataExchange( CDataExchange *pDX )
{
	__super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage6, CPropertyPage)
END_MESSAGE_MAP()

BOOL DAUCalibrationPage6::OnSetActive( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT ) ;
  m_pGrid -> ReBind() ;
  return __super::OnSetActive() ;
}

BOOL DAUCalibrationPage6::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT DAUCalibrationPage6::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES != ::AfxMessageBox( IDS_REALY_BACK, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(DAUCalibrationPage6, CPropertyPage)
  ON_EVENT(DAUCalibrationPage6, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(DAUCalibrationPage6, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage6, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(DAUCalibrationPage6, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(DAUCalibrationPage6, IDC_GRID, 1, AfterColUpdate, VTS_I2)
END_EVENTSINK_MAP()

/**
*  This event handler is called by the grid before a cell is edited and is
*  implemented here to prevent editing of cells other than save column cells.
*/
void DAUCalibrationPage6::BeforeColEdit( short index, short key, short *pCancel )
{
  *pCancel = TRUE ;
}

static CString FormatDouble( double value )
{
  CString text ;
  text.Format( "%+8.1f", value ) ;
  return text ;
}

void DAUCalibrationPage6::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
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
    pValue -> bstrVal = _bstr_t( FormatDouble( pSheet -> m_sensorData [pBookmark -> lVal].m_roll [1] ) ).copy() ;
    break ;

  case 2:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( FormatDouble( pSheet -> m_sensorData [pBookmark -> lVal].m_pitch [1] ) ).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void DAUCalibrationPage6::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark -> vt) ;
}

void DAUCalibrationPage6::SetNewLocation( VARIANT *pNewLocation, int location )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (location < 0 || location >= (int)(pSheet -> m_sensorData.size()) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void DAUCalibrationPage6::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
{
  (void)pApproximatePosition ;
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  switch (pStartLocation -> vt)
  {
  case VT_NULL:
    if (0 <= offset)
    {
      SetNewLocation( pNewLocation, offset - 1 ) ;
    }
    else
    {
      SetNewLocation( pNewLocation, (int)(pSheet -> m_sensorData.size() + offset)) ;
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

void DAUCalibrationPage6::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}
