// $Id: GainCalibrationPage0.cpp,v 1.6 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002-2003

#include "stdafx.h"
#include "GainCalibrationPage0.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage0, CPropertyPage)

GainCalibrationPage0::GainCalibrationPage0( void ) : CPropertyPage(GainCalibrationPage0::IDD)
{
  // Empty
}

GainCalibrationPage0::~GainCalibrationPage0( void )
{
  // Empty
}

void GainCalibrationPage0::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_REFERENCE_SERIAL_NUMBER, m_reference) ;
  DDX_Control(pDX, IDC_TABLE_SERIAL_NUMBER, m_table);
}

BEGIN_MESSAGE_MAP(GainCalibrationPage0, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_TABLE_SERIAL_NUMBER, OnCbnSelchangeTableSerialNumber)
END_MESSAGE_MAP()

BOOL GainCalibrationPage0::InitReference( void )
{
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
  {
    Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
    if (UnitType::Unused != pCurrent -> GetType())
    {
      int index = m_reference.AddString( pCurrent -> GetSerialNumber() ) ;
      if (0 > index)// || CB_ERR == m_reference.SetItemDataPtr( index, static_cast<void *>(pCurrent) ))
      {
        return FALSE ;
      }
	  m_reference.SetItemData( index, i);
	}
  }
  return TRUE ;
}

//UNIQUE_SN(PlatformCorrection)     { UNIQUE_SN_BODY } ;

BOOL GainCalibrationPage0::InitTable( void )
{
    vector<CString> serialNumbers;
    DBInterface::Instance()->GetUniqueSensorSN("PlatformCorrection", serialNumbers); 
    vector<CString>::iterator iter;
    for(iter=serialNumbers.begin();iter!=serialNumbers.end();iter++)
    {
        TRACE("SN:%s\n",*iter);
        m_table.AddString( *iter ) ;
    }
  
    switch (m_table.GetCount())
    {
        case 0:
            return FALSE ;
        break;

        case 1:
            m_table.SetCurSel( 0 ) ;
            m_table.EnableWindow( FALSE ) ;
            OnCbnSelchangeTableSerialNumber() ;
        break ;

        default:
            m_table.EnableWindow( TRUE ) ;
        break ;
    }
    return TRUE ;
}

BOOL GainCalibrationPage0::OnInitDialog( void )
{
	BOOL focus = CDialog::OnInitDialog();
	//BOOL focus = __super::OnInitDialog() ;
  if (!InitReference())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  if (!InitTable())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  
  CWnd* pWnd = GetDlgItem(IDC_TIME_CONST_EDIT);
  CString tcStr; tcStr.Format("%.1f", g_AlignerData.TaoTilt);
  pWnd->SetWindowText(tcStr);

  return focus ;
}

BOOL GainCalibrationPage0::OnSetActive( void )
{
    CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;
    MoveDlgToRightSideOfApp(pSheet);
    return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage0::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (CB_ERR == m_reference.GetCurSel())
  {
    ::AfxMessageBox( IDS_SELECT_REFERENCE ) ;
    return -1 ;
  }
  if ( (pSheet -> m_alpha == 0) || ( CB_ERR == m_table.GetCurSel() ) )
  {
    ::AfxMessageBox( IDS_SELECT_TABLE ) ;
    return -1 ;
  }

  CWnd* pWnd = GetDlgItem(IDC_TIME_CONST_EDIT);
  CString tcStr; pWnd->GetWindowText(tcStr);
  double tc = atof(tcStr);
  if (tc < 0.5f || tc > 25)
  {
	  ::AfxMessageBox(_T("Enter a number between 0.5 and 25.0"), MB_ICONERROR);
	  return -1;
  }

  g_AlignerData.TaoTilt = tc; //TEMP
  pSheet->SetTimeConstant(g_AlignerData.TaoTilt);
  pSheet->m_referenceData.m_sensorIndex = m_reference.GetItemData(m_reference.GetCurSel());
  pSheet->m_referenceData.m_pSource = DAU::GetDAU().GetSensor( pSheet->m_referenceData.m_sensorIndex );
  
  g_AlignerData.RefObjNo = CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX(pSheet->m_referenceData.m_sensorIndex);

  ASSERT(NULL != pSheet->m_referenceData.m_pSource) ;
  TRACE("Ref index:%d\n",pSheet->m_referenceData.m_sensorIndex); 
  return __super::OnWizardNext() ;
}

void GainCalibrationPage0::OnCbnSelchangeTableSerialNumber( void )
{
    double alpha = 0.0f;
    CString serialNumber ;
    m_table.GetWindowText( serialNumber ) ;
    DBInterface::Instance()->GetPlatformCorrectionAngle(serialNumber, alpha);
    
    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet->m_alpha = alpha / 1000.0f ;
}
