// $Id: AlignVerBenchMarkPage1.cpp,v 1.2 2012/07/20 14:32:52 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AlignVerBenchMarkPage1.h"
#include "AlignmentWizard.h"
#include "Util.h"
#include "SetupAxesDlg.h"
#include "SetupTestPlaneDlg.h"

IMPLEMENT_DYNAMIC(CAlignVerBenchMarkPage1, CPropertyPage)

CAlignVerBenchMarkPage1::CAlignVerBenchMarkPage1()
	: CPropertyPage(CAlignVerBenchMarkPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CAlignVerBenchMarkPage1::~CAlignVerBenchMarkPage1()
{
}

void CAlignVerBenchMarkPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ALIGN_VER_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
    DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CAlignVerBenchMarkPage1::ShowSetup()
{
	CString setup;

    if( ( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) && 
      ( m_EnableReference == TRUE ) )
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

void CAlignVerBenchMarkPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
    setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CAlignVerBenchMarkPage1::EnableDisableSperry()
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

BEGIN_MESSAGE_MAP(CAlignVerBenchMarkPage1, CPropertyPage)
    ON_EN_KILLFOCUS(IDC_ALIGN_VER_FILTER_TIME_CONSTANT, OnEnKillfocusAlignVerFilterTimeConstant)
    ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
	ON_BN_CLICKED(IDC_ALIGN_VER_SETUP_TEST, OnBnClickedAlignVerSetupTest)
END_MESSAGE_MAP()

// CAlignVerBenchMarkPage1 message handlers

BOOL CAlignVerBenchMarkPage1::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_pParent = (CAlignmentWizard*)GetParent();
    ASSERT(m_pParent -> IsKindOf( RUNTIME_CLASS(CAlignmentWizard) ));
    m_MsgCaption = _T("");
    m_Text = _T("");
    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAlignVerBenchMarkPage1::OnSetActive() 
{
    if( m_pParent == NULL )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
        EndDialog( IDCANCEL );
        return( FALSE ); // prevent the page from being active
    }

    if( m_pParent->m_EnableFixPlane == TRUE )
    {
        g_AlignerData.AlignMode.LoadString( IDS_AZIMUTH_VERIFICATION_BENCHMARK_TEST );
    }
    else
    {
        g_AlignerData.AlignMode.LoadString( IDS_AZIMUTH_VERIFICATION_GYROSTAB_TEST );
    }

    m_pParent->SetWizardButtons( PSWIZB_NEXT );
    g_AlignerData.Mode = Azim;
    m_EnableZPar = FALSE;
    m_EnableReference = TRUE;

	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

  //restore
    m_FilterTimeConstant = g_AlignerData.TaoAz; 

    if( ( g_AlignerData.TestNotSetup == TRUE ) || ( IsConnected( g_AlignerData.RefObjNo ) == FALSE ) )
    {
	    CSetupTestPlaneDlg setupTestPlaneDlg( m_EnableZPar, m_EnableReference, m_pParent->m_EnableFixPlane );

        if( setupTestPlaneDlg.DoModal() == IDCANCEL )
		{
			EndDialog( IDCANCEL );
			return( FALSE ); // prevent the page from being active
		}
    }
    g_AlignerData.TestNotSetup = FALSE;
    ShowSetup();
	ShowSettingsForAxes();
    UpdateData( FALSE );
    m_pParent->m_Status = STATUS_PAGE_READY;
    g_AlignerData.ErrorDef = ERR_NO_ERROR;

    return CPropertyPage::OnSetActive();
}

BOOL CAlignVerBenchMarkPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CAlignVerBenchMarkPage1::OnWizardNext()
{
    switch( m_pParent->m_Status )
    {
    case STATUS_PAGE_READY:
        //store
        g_AlignerData.TaoAz = m_FilterTimeConstant; 
        if( m_pParent->m_EnableFixPlane == TRUE )
        {
        m_pParent->m_AziMode = BenchAzi;
        }
        else
        {
        m_pParent->m_AziMode = StabAzi;
        }
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

void CAlignVerBenchMarkPage1::OnReset()
{
    m_pParent->m_Status = STATUS_PAGE_CANCELED;
    g_AlignerData.ErrorDef = ERR_CANCEL;
    return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CAlignVerBenchMarkPage1::OnEnKillfocusAlignVerFilterTimeConstant()
{
     UpdateData( TRUE );
}

void CAlignVerBenchMarkPage1::OnBnClickedSetupAxes()
{
    CSetupAxesDlg setupAxesDlg;
    setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CAlignVerBenchMarkPage1::OnBnClickedAlignVerSetupTest()
{
    CSetupTestPlaneDlg setupTestPlaneDlg( m_EnableZPar, m_EnableReference, m_pParent->m_EnableFixPlane );

	if( setupTestPlaneDlg.DoModal() == IDOK )
    {
        g_AlignerData.LogData.ResultFromFirstMeasurementExists  = FALSE;
    }
    ShowSetup();
}
