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
	DBInterface::Instance()->GetPlatforms(m_platforms);
    
    for(auto iter= m_platforms.begin();iter!= m_platforms.end();iter++)
    {     
		CString str;
		str.Format("%03d",iter->m_sn);
		int i = m_table.AddString( str ) ;
		m_table.SetItemData(i, (DWORD_PTR)&(*iter));
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
	CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent());
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;
    MoveDlgToRightSideOfApp(pSheet);
		
	
	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_CALIBRATION)
	{
		CWnd* pWnd = GetDlgItem(IDC_TABLE_SERIAL_NUMBER);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_TABLE_STATIC);
		pWnd->ShowWindow(SW_HIDE);		
		pSheet->m_alpha = SysSetup->GetProject().m_platform.m_alpha / 1000.0f;
		g_AlignerData.PlatformSN = SysSetup->GetProject().m_platform.m_sn;
	}
	else
	{
		
	}

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
  if ( (pSheet -> m_alpha == 0))
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
	Platform pl = *((Platform*)m_table.GetItemData(m_table.GetCurSel()));

    CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
    ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
    pSheet->m_alpha = pl.m_alpha / 1000.0f ;
	g_AlignerData.PlatformSN = pl.m_sn;
}
