// $Id: TiltFlatnessTestPage1.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SetupMeasureDlg.h"
#include "SetupAxesDlg.h"
#include "TiltFlatnessTestWizard.h"
#include "Filter.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(CTiltFlatnessTestPage1, CPropertyPage)

CTiltFlatnessTestPage1::CTiltFlatnessTestPage1()
	: CPropertyPage(CTiltFlatnessTestPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CTiltFlatnessTestPage1::~CTiltFlatnessTestPage1()
{
}

void CTiltFlatnessTestPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_TILT_FLATNESS_TEST_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
  DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CTiltFlatnessTestPage1::ShowSetup()
{
	if (IsGunPresent())
	{
		int res = AskGunAdapterTiltCheck();
		if (res == IDOK)
		{
			m_pParent->EndDialog(IDABORT);
			return;
		}
	}


	CString setup;

	if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
	{
		GetMeasurementSetup( m_pParent->m_pSource, TRUE, &setup );
	}
	else
	{
		GetMeasurementSetup( m_pParent->m_pSource, FALSE, &setup );
	}
	SetDlgItemText( IDC_CONFIGURATION_TEXT, setup );

  if( IsPlaneOrGunSelected() == TRUE )
  {
    m_pParent->SetWizardButtons( PSWIZB_NEXT );
  }
	EnableDisableSperry();
}

BOOL CTiltFlatnessTestPage1::IsPlaneOrGunSelected()
{
  BOOL planeOrGunConnected = TRUE;

  if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
  {
		if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
    {
      planeOrGunConnected = FALSE;

      for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
      {
				if( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE )
        {
          planeOrGunConnected = TRUE;
        }
      }    
    }
  }
  else
  {
    planeOrGunConnected = FALSE;
  }
  return( planeOrGunConnected );
}

void CTiltFlatnessTestPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
	setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CTiltFlatnessTestPage1::EnableDisableSperry()
{
	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		if( ( IsSelected( SPERRY_CHANNEL ) == TRUE ) && ( 0 == g_AlignerData.RX ) )
		{
			GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( TRUE );
			return;
		}
	}
	GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( FALSE );	
	m_SperryActiveBox.SetCheck( BST_UNCHECKED );
	return;
}

BEGIN_MESSAGE_MAP(CTiltFlatnessTestPage1, CPropertyPage)
  ON_EN_KILLFOCUS(IDC_TILT_FLATNESS_TEST_FILTER_TIME_CONSTANT, OnEnKillfocusTiltFlatnessTestFilterTimeConstant)
  ON_BN_CLICKED(IDC_TILT_FLATNESS_TEST_SETUP_MEASUREMENT, OnBnClickedTiltFlatnessTestSetupMeasurement)
  ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
END_MESSAGE_MAP()


// CTiltFlatnessTestPage1 message handlers

BOOL CTiltFlatnessTestPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CTiltFlatnessTestWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltFlatnessTestPage1::OnSetActive() 
{
  if( m_pParent == NULL )
  {
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }
  m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );

  if( IsPlaneOrGunConnected() == FALSE )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_CONNECT_AT_LEAST_ONE_P_G_STATION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    OnBnClickedTiltFlatnessTestSetupMeasurement();
  }
  else
  {
    ShowSetup();
  }

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    //TODO error message
    EndDialog( IDCANCEL );
    return FALSE;
  }

	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

  //restore
  m_FilterTimeConstant = g_AlignerData.TaoFlat; 
	ShowSettingsForAxes();
 
  UpdateData( FALSE );
  m_pParent->m_Status = STATUS_PAGE_READY;

  return CPropertyPage::OnSetActive();
}

BOOL CTiltFlatnessTestPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CTiltFlatnessTestPage1::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
    if( IsPlaneOrGunSelected() == FALSE )
    {
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      m_Text.LoadString( IDS_SETUP_AT_LEAST_ONE_P_G_STATION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    }
    else
    {
      //store 
      g_AlignerData.TaoFlat = m_FilterTimeConstant; 
      m_pParent->m_Status = STATUS_PAGE_NOT_READY; //next page not ready
      return CPropertyPage::OnWizardNext();
    }
		if( m_SperryActiveBox.GetCheck() ==  BST_CHECKED )
		{
			m_pParent->SetSperryActive( TRUE );
		}
		else
		{
			m_pParent->SetSperryActive( FALSE );
		}
    break;
  case STATUS_PAGE_ERROR:
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_ERROR_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    break;
  case STATUS_PAGE_NOT_READY:
  default:
    m_MsgCaption.LoadString( IDS_NOT_READY_CAPTION );
    m_Text.LoadString( IDS_TASKS_NOT_COMPLETED_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    break;
  }
  return -1;  // prevent the page from changing
}

void CTiltFlatnessTestPage1::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltFlatnessTestPage1::OnBnClickedTiltFlatnessTestSetupMeasurement()
{
  CSetupMeasureDlg setupMeasDlg( FALSE, g_AlignerData.Found, TRUE );
  setupMeasDlg.DoModal();
  ShowSetup();
}

void CTiltFlatnessTestPage1::OnBnClickedSetupAxes()
{
  CSetupAxesDlg setupAxesDlg;
  setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CTiltFlatnessTestPage1::OnEnKillfocusTiltFlatnessTestFilterTimeConstant()
{
  UpdateData( TRUE );
}

