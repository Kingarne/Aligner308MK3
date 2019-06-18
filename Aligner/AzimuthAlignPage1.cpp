// $Id: AzimuthAlignPage1.cpp,v 1.2 2013/09/25 11:47:39 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AzimuthAlignWizard.h"
#include "SetupAxesDlg.h"
#include "SetupMeasureDlg.h"
#include "Filter.h"
#include "Util.h"
#include "AzAlignNoteDlg.h"

// CAzimuthAlignPage1 dialog

IMPLEMENT_DYNAMIC(CAzimuthAlignPage1, CPropertyPage)
CAzimuthAlignPage1::CAzimuthAlignPage1()
	: CPropertyPage(CAzimuthAlignPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CAzimuthAlignPage1::~CAzimuthAlignPage1()
{
}

void CAzimuthAlignPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
  DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CAzimuthAlignPage1::ShowSetup()
{
	AzAlignNoteDlg dlg;
	dlg.m_isGunPresent = IsGunPresent();
	int res = dlg.DoModal();
	if (res == IDOK)
	{
		if(dlg.m_command == 1)
			m_pParent->EndDialog(RUN_SVT);
		if(dlg.m_command == 2)
			m_pParent->EndDialog(RUN_GATC);
	}

	/*if (IsGunPresent())
	{
		//AzAlign
		int res = AskGunAdapterTiltCheck();
		if (res == IDOK)
		{
			m_pParent->EndDialog(IDABORT);
			return;
		}
	}*/

	
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

void CAzimuthAlignPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
	setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CAzimuthAlignPage1::EnableDisableSperry()
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

BEGIN_MESSAGE_MAP(CAzimuthAlignPage1, CPropertyPage)
  ON_EN_KILLFOCUS(IDC_FILTER_TIME_CONSTANT, OnEnKillfocusFilterTimeConstant)
  ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
  ON_BN_CLICKED(IDC_SETUP_MEASUREMENT, OnBnClickedSetupMeasurement)
END_MESSAGE_MAP()

// CAzimuthAlignPage1 message handlers

BOOL CAzimuthAlignPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CAzimuthAlignWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAzimuthAlignPage1::OnSetActive() 
{
  if( m_pParent == NULL )
  {
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE; // prevent the page from being active
  }
  m_pParent->SetWizardButtons( PSWIZB_NEXT );

	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

  //restore 
  m_FilterTimeConstant = g_AlignerData.TaoAz; 

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    CSetupMeasureDlg setupMeasureDlg( FALSE, FALSE );
    setupMeasureDlg.DoModal();
  }

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    EndDialog( IDCANCEL );
    return FALSE; // prevent the page from being active
  }
  ShowSetup();
	ShowSettingsForAxes();
  UpdateData( FALSE );
  m_pParent->m_Status = STATUS_PAGE_READY;
  g_AlignerData.ErrorDef = ERR_NO_ERROR;

  return CPropertyPage::OnSetActive();
}

BOOL CAzimuthAlignPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CAzimuthAlignPage1::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
    //store
    g_AlignerData.TaoAz = m_FilterTimeConstant; 
    m_pParent->m_AziMode = NormAzi;
		if( m_SperryActiveBox.GetCheck() ==  BST_CHECKED )
		{
			m_pParent->SetSperryActive( TRUE );
		}
		else
		{
			m_pParent->SetSperryActive( FALSE );
		}
    return IDD_ALIGN_VER_BENCHMARK_PAGE_2; //ID of the next page to jump
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

void CAzimuthAlignPage1::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CAzimuthAlignPage1::OnEnKillfocusFilterTimeConstant()
{
  UpdateData( TRUE );
}

void CAzimuthAlignPage1::OnBnClickedSetupAxes()
{
  CSetupAxesDlg setupAxesDlg;
  setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CAzimuthAlignPage1::OnBnClickedSetupMeasurement()
{
  CSetupMeasureDlg setupMeasureDlg( FALSE, FALSE );
  if( setupMeasureDlg.DoModal() == IDOK )
  {
    g_AlignerData.LogData.ResultFromFirstMeasurementExists  = FALSE;
  }
  ShowSetup();
}
