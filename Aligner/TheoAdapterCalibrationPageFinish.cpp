// $Id: TheoAdapterCalibrationPageFinish.cpp,v 1.2 2014-07-11 16:30:59 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TheoAdapterCalibrationPageFinish.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPageFinish, CPropertyPage)

TheoAdapterCalibrationPageFinish::TheoAdapterCalibrationPageFinish( void ) : CPropertyPage(TheoAdapterCalibrationPageFinish::IDD)
{
  // Empty
}

TheoAdapterCalibrationPageFinish::~TheoAdapterCalibrationPageFinish( void )
{
  // Empty
}

void TheoAdapterCalibrationPageFinish::DoDataExchange( CDataExchange *pDX )
{
    __super::DoDataExchange( pDX ) ;
    DDX_Control(pDX, IDC_GRID, m_grid) ;
    if (!pDX -> m_bSaveAndValidate)
    {
        m_pGrid = m_grid.GetControlUnknown() ;
    }
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPageFinish, CPropertyPage)
END_MESSAGE_MAP()

BOOL TheoAdapterCalibrationPageFinish::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  return focus ;
}

BOOL TheoAdapterCalibrationPageFinish::OnSetActive( void )
{
    CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
	pSheet -> SetWizardButtons( PSWIZB_FINISH | PSWIZB_BACK ) ;
    
    MoveDlgToRightSideOfApp(pSheet);

    return __super::OnSetActive() ;
}

LRESULT TheoAdapterCalibrationPageFinish::OnWizardBack( void )
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

BOOL TheoAdapterCalibrationPageFinish::OnQueryCancel( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet -> StopMeasurement() ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT | PSWIZB_BACK ) ;
  return IDYES == ::AfxMessageBox( IDS_REALY_ABORT, MB_YESNO | MB_DEFBUTTON2 ) ;
}

BEGIN_EVENTSINK_MAP(TheoAdapterCalibrationPageFinish, CPropertyPage)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 1, AfterColUpdate, VTS_I2)
  ON_EVENT(TheoAdapterCalibrationPageFinish, IDC_GRID, 62, OnInitGrid, VTS_NONE)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than save column cells.
 */
void TheoAdapterCalibrationPageFinish::BeforeColEdit( short index, short key, short *pCancel )
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
void TheoAdapterCalibrationPageFinish::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
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

void TheoAdapterCalibrationPageFinish::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
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

void TheoAdapterCalibrationPageFinish::SetNewLocation( VARIANT *pNewLocation, int location )
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

void TheoAdapterCalibrationPageFinish::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
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

void TheoAdapterCalibrationPageFinish::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

BOOL TheoAdapterCalibrationPageFinish::OnWizardFinish( void )
{
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    DBTIMESTAMP now;
    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(now);

    for (int i = 0 ; i < (int)(m_adapter.size()) ; i++)
    {
		pSheet->m_sensorData[i].m_pSource->SetAdapterCalibrationData(AdapterCalibrationData(m_adapter[i].m_elevation, m_adapter[i].m_azimuth, now));
		DBInterface::Instance()->UpdateTheoAdapterCalibration(pSheet->m_sensorData[i].m_pSource->GetAdapterSerialNumber(), m_adapter[i].m_elevation, m_adapter[i].m_azimuth);      

	}
    return __super::OnWizardFinish() ;
}

void TheoAdapterCalibrationPageFinish::OnInitGrid( void )
{
	  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
	  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
	  m_adapter.clear() ;
	  for (vector<CalibrationData>::iterator i = pSheet->m_sensorData.begin() ; i != pSheet->m_sensorData.end() ; i++)
	  {
			m_adapter.push_back( TheoAdapterCalibrationResult() ) ;
			vector<TheoAdapterCalibrationResult>::reverse_iterator m = m_adapter.rbegin() ;
			m->m_elevation = 0;
			
			double tmpDiv = (i->m_pitch[1] - pSheet->m_referenceData.m_pitch[1]) - (i->m_pitch[0] - pSheet->m_referenceData.m_pitch[0]) ;			
			
			if( fabs(tmpDiv) > DBL_EPSILON )
			{
				 m->m_azimuth = -UnitsToMilliunits( ((i->m_roll[1] - pSheet->m_referenceData.m_roll[1]) - (i->m_roll[0] - pSheet->m_referenceData.m_roll[0])) / tmpDiv );
			}
			else
			{
				m->m_azimuth = DBL_MAX;
			}
	  }
}
