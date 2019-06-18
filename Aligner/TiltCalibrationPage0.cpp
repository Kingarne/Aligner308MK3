// $Id: TiltCalibrationPage0.cpp,v 1.4 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltCalibrationPage0.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TiltCalibrationPage0, CPropertyPage)

TiltCalibrationPage0::TiltCalibrationPage0() : CPropertyPage(TiltCalibrationPage0::IDD)
{
  // Empty
}

TiltCalibrationPage0::~TiltCalibrationPage0( void )
{
  // Empty
}

void TiltCalibrationPage0::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_REFERENCE_SERIAL_NUMBER, m_reference);
}

BEGIN_MESSAGE_MAP(TiltCalibrationPage0, CPropertyPage)
END_MESSAGE_MAP()

BOOL TiltCalibrationPage0::InitReference( void )
{
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
  {
    Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
    if (UnitType::Unused != pCurrent -> GetType())
    {
      int index = m_reference.AddString( pCurrent->GetSerialNumber() ) ;
      if (0 > index)
      {
        return FALSE ;
      }
	m_reference.SetItemData( index, i);   
	}
  }
  return TRUE ;
}

BOOL TiltCalibrationPage0::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  if (!InitReference())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  return focus ;
}

BOOL TiltCalibrationPage0::OnSetActive( void )
{
    CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;

      MoveDlgToRightSideOfApp(pSheet);
    return __super::OnSetActive() ;
}

LRESULT TiltCalibrationPage0::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (CB_ERR == m_reference.GetCurSel())
  {
    ::AfxMessageBox( IDS_SELECT_REFERENCE ) ;
    return -1 ;
  }
  pSheet->m_referenceData.m_sensorIndex = m_reference.GetItemData(m_reference.GetCurSel());
  pSheet->m_referenceData.m_pSource = DAU::GetDAU().GetSensor( pSheet->m_referenceData.m_sensorIndex );

  ASSERT(NULL != pSheet->m_referenceData.m_pSource) ;
  return __super::OnWizardNext() ;
}
