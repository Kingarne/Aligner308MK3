// $Id: TheoAdapterCalibrationPage0.cpp,v 1.1 2014-07-11 09:28:57 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TheoAdapterCalibrationPage0.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(TheoAdapterCalibrationPage0, CPropertyPage)

TheoAdapterCalibrationPage0::TheoAdapterCalibrationPage0() : CPropertyPage(TheoAdapterCalibrationPage0::IDD)
, m_timeConstant(1)
{
  // Empty
}

TheoAdapterCalibrationPage0::~TheoAdapterCalibrationPage0( void )
{
  // Empty
}

void TheoAdapterCalibrationPage0::DoDataExchange( CDataExchange *pDX )
{
  CPropertyPage::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_REFERENCE_SERIAL_NUMBER, m_reference);
  DDX_Text(pDX, IDC_EDIT1, m_timeConstant);
	DDV_MinMaxDouble(pDX, m_timeConstant, 1, 25);
  if (pDX -> m_bSaveAndValidate)
  {
    CalibrationSheet*pSheet = static_cast<CalibrationSheet*>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet -> SetTimeConstant( m_timeConstant ) ;
  }
}

BEGIN_MESSAGE_MAP(TheoAdapterCalibrationPage0, CPropertyPage)
	
END_MESSAGE_MAP()

BOOL TheoAdapterCalibrationPage0::InitReference( void )
{
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
  {
    Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
    if (UnitType::Fixed == pCurrent->GetType())
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

BOOL TheoAdapterCalibrationPage0::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  if (!InitReference())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  
  return focus ;
}

BOOL TheoAdapterCalibrationPage0::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;   
  pSheet->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
  
  MoveDlgToRightSideOfApp(pSheet);
    
  return __super::OnSetActive() ;
}

LRESULT TheoAdapterCalibrationPage0::OnWizardNext( void )
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

  ASSERT(NULL != pSheet -> m_referenceData.m_pSource) ;
  TRACE("Ref index:%d\n",pSheet->m_referenceData.m_sensorIndex);
  return __super::OnWizardNext() ;
}


