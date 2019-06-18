// $Id: GyroPerformanceTestPage1.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GyroPerformanceTestWizard.h"
#include "SetupAxesDlg.h"
#include "SetupDriftDlg.h"
#include "Filter.h"
#include "gyroperformancetestpage1.h"
#include "Util.h"

// CGyroPerformanceTestPage1 dialog

IMPLEMENT_DYNAMIC(CGyroPerformanceTestPage1, CPropertyPage)
CGyroPerformanceTestPage1::CGyroPerformanceTestPage1()
	: CPropertyPage(CGyroPerformanceTestPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CGyroPerformanceTestPage1::~CGyroPerformanceTestPage1()
{
}

void CGyroPerformanceTestPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
  DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CGyroPerformanceTestPage1::ShowSetup()
{
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
	EnableDisableSperry();
}

void CGyroPerformanceTestPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
	setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CGyroPerformanceTestPage1::EnableDisableSperry()
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

BEGIN_MESSAGE_MAP(CGyroPerformanceTestPage1, CPropertyPage)
  ON_EN_KILLFOCUS(IDC_FILTER_TIME_CONSTANT, OnEnKillfocusFilterTimeConstant)
  ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
  ON_BN_CLICKED(IDC_SETUP_MEASUREMENT, OnBnClickedSetupDrift)
END_MESSAGE_MAP()

// CGyroPerformanceTestPage1 message handlers

BOOL CGyroPerformanceTestPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CGyroPerformanceTestWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CGyroPerformanceTestPage1::OnSetActive() 
{
  if( m_pParent == NULL )
  {
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    EndDialog( IDCANCEL );
    return( FALSE ); // prevent the page from being active
  }
 
  BOOL syncroConnected = FALSE;

	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
  {
    if( IsConnected( i ) == TRUE )
    {
      syncroConnected = TRUE;
    }
  }

  if( syncroConnected == FALSE )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_CONNECT_AT_LEAST_ONE_GYRO );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  if( IsFixedConnected() == FALSE )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_CONNECT_AT_LEAST_ONE_FIXED_STATION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

  //restore
  m_FilterTimeConstant = g_AlignerData.TaoGyro; 

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    CSetupDriftDlg setupDriftDlg;

    if( setupDriftDlg.DoModal() == IDCANCEL )
    {
      EndDialog( IDCANCEL );
      return FALSE;
    }
  }

  m_Text.LoadString( IDS_THIS_FUNCTION_PLOTS_THE_PROVIDED_THAT_THE_AZIMUTH );
  SetDlgItemText( IDC_INFO_TEXT, m_Text );

  ShowSetup();
	ShowSettingsForAxes();
  UpdateData( FALSE );
  g_AlignerData.ErrorDef = ERR_NO_ERROR;
  m_pParent->m_Status = STATUS_PAGE_READY;
  m_pParent->SetWizardButtons( PSWIZB_NEXT );

  return CPropertyPage::OnSetActive();
}

BOOL CGyroPerformanceTestPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CGyroPerformanceTestPage1::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
    //store
    g_AlignerData.TaoGyro = m_FilterTimeConstant; 
		if( m_SperryActiveBox.GetCheck() ==  BST_CHECKED )
		{
			m_pParent->SetSperryActive( TRUE );
		}
		else
		{
			m_pParent->SetSperryActive( FALSE );
		}
    return CPropertyPage::OnWizardNext();
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

void CGyroPerformanceTestPage1::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CGyroPerformanceTestPage1::OnEnKillfocusFilterTimeConstant()
{
  UpdateData( TRUE );
}

void CGyroPerformanceTestPage1::OnBnClickedSetupAxes()
{
  CSetupAxesDlg setupAxesDlg;
  setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CGyroPerformanceTestPage1::OnBnClickedSetupDrift()
{
  CSetupDriftDlg setupDriftDlg;
  setupDriftDlg.DoModal();
  ShowSetup();
}
