// $Id: TiltCalibrationPage4.cpp,v 1.5 2012/08/22 13:09:04 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "CalibrationSheet.h"
#include "TiltCalibrationPage4.h"
#include "Util.h"
#include "DBInterface.h"

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
  if (!pDX->m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage4, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage4::OnSetActive( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet->PlotConverted( 3 ) ;
    pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH ) ;
    MoveDlgToRightSideOfApp(pSheet);
    return __super::OnSetActive() ;
}

BOOL TiltCalibrationPage4::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

LRESULT TiltCalibrationPage4::OnWizardBack( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
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
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  ASSERT(VT_I4 == pBookmark->vt) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  if (0 == pBookmark->lVal)
  {
    switch (column)
    {
    case 0:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( pSheet->m_referenceData.m_pSource->GetSerialNumber() ).copy() ;
      break ;

    case 1:
      pValue->vt = VT_I4 ;
      pValue->lVal = m_referenceResult.m_saveResult ;
      break ;

    case 2:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_sensorRollOffset ) ).copy() ;
      break ;

    case 3:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_sensorPitchOffset ) ).copy() ;
      break ;

    case 4:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( _T("Ref") ).copy() ;
      break ;

    case 5:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( _T("Ref") ).copy() ;
      break ;

    case 6:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_referenceResult.m_temperature ) ).copy() ;
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
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( pSheet->m_sensorData [pBookmark->lVal - 1].m_pSource->GetSerialNumber() ).copy() ;
      break ;

    case 1:
      pValue->vt = VT_I4 ;
      pValue->lVal = m_sensorResult [pBookmark->lVal - 1].m_saveResult ;
      break ;

    case 2:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark->lVal - 1].m_sensorRollOffset ) ).copy() ;
      break ;

    case 3:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark->lVal - 1].m_sensorPitchOffset ) ).copy() ;
      break ;

    case 4:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark->lVal - 1].m_flatRollOffset ) ).copy() ;
      break ;

    case 5:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark->lVal - 1].m_flatPitchOffset ) ).copy() ;
      break ;

    case 6:
      pValue->vt = VT_BSTR ;
      pValue->bstrVal = _bstr_t( FormatDouble( m_sensorResult [pBookmark->lVal - 1].m_temperature ) ).copy() ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
  }
}

void TiltCalibrationPage4::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  ASSERT(VT_I4 == pBookmark->vt) ;
  switch (column)
  {
  case 1:
    if (0 == pBookmark->lVal)
    {
      ASSERT(VT_BSTR == pValue->vt) ;
      m_referenceResult.m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
    }
    else
    {
      m_sensorResult [pBookmark->lVal - 1].m_saveResult = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
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
    pNewLocation->vt = VT_NULL ;
  }
  else
  {
    pNewLocation->vt = VT_I4 ;
    pNewLocation->lVal = location ;
  }
}

void TiltCalibrationPage4::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
{
  (void)pApproximatePosition ;
  switch (pStartLocation->vt)
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
    SetNewLocation( pNewLocation, pStartLocation->lVal + offset ) ;
    break ;

  default:
    ASSERT(0) ;
    pNewLocation->vt = VT_NULL ;
    break ;
  }
}

void TiltCalibrationPage4::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid->Update() ;
}

static void SaveCalibrationData( TiltCalibrationResult &result, CString serialNumber )
{
    if (result.m_saveResult)
    {
        DBInterface::Instance()->InsertSensorData("SensorRollOffsetData", serialNumber, result.m_temperature, result.m_sensorRollOffset); 
        DBInterface::Instance()->InsertSensorData("SensorPitchOffsetData", serialNumber, result.m_temperature, result.m_sensorPitchOffset); 
    }
}

BOOL TiltCalibrationPage4::OnWizardFinish( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;

  SaveCalibrationData( m_referenceResult, pSheet->m_referenceData.m_pSource->GetSerialNumber() ) ;
  for (int i = 0 ; i < (int)(m_sensorResult.size()) ; i++)
  {
     SaveCalibrationData( m_sensorResult [i], pSheet->m_sensorData [i].m_pSource->GetSerialNumber() ) ;
  }
  return __super::OnWizardFinish() ;
}

void TiltCalibrationPage4::OnInitGrid( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet->IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  m_sensorResult.clear() ;
  for (vector<CalibrationData>::iterator i = pSheet->m_sensorData.begin() ; i != pSheet->m_sensorData.end() ; i++)
  {
    m_sensorResult.push_back( TiltCalibrationResult() ) ;
    vector<TiltCalibrationResult>::reverse_iterator m = m_sensorResult.rbegin() ;
    m->m_sensorRollOffset  = -UnitsToMilliunits( (pSheet->m_referenceData.m_roll [2]  - i->m_roll [2]  - pSheet->m_referenceData.m_roll [1]  - i->m_roll [1]) / 2 ) ;
    m->m_flatRollOffset    = -UnitsToMilliunits( (pSheet->m_referenceData.m_roll [0]  - i->m_roll [0]  - pSheet->m_referenceData.m_roll [2]  + i->m_roll [2]) / 2 ) ;
    m->m_sensorPitchOffset = -UnitsToMilliunits( (pSheet->m_referenceData.m_pitch [2] - i->m_pitch [2] - pSheet->m_referenceData.m_pitch [1] - i->m_pitch [1]) / 2 ) ;
    m->m_flatPitchOffset   = -UnitsToMilliunits( (pSheet->m_referenceData.m_pitch [2] - i->m_pitch [2] - pSheet->m_referenceData.m_pitch [0] + i->m_pitch [0]) / 2 ) ;
    m->m_temperature = (i->m_temperature [0] + i->m_temperature [1] + i->m_temperature [2]) / 3 ;
  }
  m_referenceResult.m_sensorRollOffset =  -UnitsToMilliunits( (- pSheet->m_referenceData.m_roll [1]  - pSheet->m_sensorData.begin()->m_roll [1]  - pSheet->m_referenceData.m_roll [0]  + pSheet->m_sensorData.begin()->m_roll [0]) / 2 ) ;
  m_referenceResult.m_sensorPitchOffset = -UnitsToMilliunits( (- pSheet->m_referenceData.m_pitch [1] - pSheet->m_sensorData.begin()->m_pitch [1] - pSheet->m_referenceData.m_pitch [0] + pSheet->m_sensorData.begin()->m_pitch [0]) / 2 ) ;
  m_referenceResult.m_temperature = (pSheet->m_referenceData.m_temperature [0] + pSheet->m_referenceData.m_temperature [1] + pSheet->m_referenceData.m_temperature [2]) / 3 ;
  m_referenceResult.m_flatRollOffset = 0 ;
  m_referenceResult.m_flatPitchOffset = 0 ;
}
