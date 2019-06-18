// $Id: AlignVerHorizonPage1.cpp,v 1.4 2012/08/22 13:09:01 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SetupAxesDlg.h"
#include "SetupTestPlaneDlg.h"
#include "Filter.h"
#include "TiltVerificationWizard.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(CAlignVerHorizonPage1, CPropertyPage)

CAlignVerHorizonPage1::CAlignVerHorizonPage1()
	: CPropertyPage(CAlignVerHorizonPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CAlignVerHorizonPage1::~CAlignVerHorizonPage1()
{
}

void CAlignVerHorizonPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ALIGN_VER_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
    DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CAlignVerHorizonPage1::ShowSetup()
{
    if( m_ParallaxComp == TRUE )
    {
        //Show Zpar
        CString zParText, tmpStr;
        tmpStr.LoadString( IDS_SETUP_PLANE_PLATFORMS_ZPAR );

        if( ( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) && ( m_pParent->m_EnableReference == TRUE ) )
        {
            zParText = tmpStr + _T("\n");
            //tmpStr.Format( _T("%.1f"), g_AlignerData.ZPar[g_AlignerData.RefObjNo] );
			tmpStr.Format( _T("%.2f"), GetZPar( g_AlignerData.RefObjNo ) );
            RAdjText( &tmpStr, 6 );   
            zParText += tmpStr + _T("\n\n\n");
        }
        else
        {
            zParText = _T("\n\n\n") + tmpStr + _T("\n");
        }

        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            //tmpStr.Format( _T("%.1f"), g_AlignerData.ZPar[g_AlignerData.ObjNo[i]] );
            tmpStr.Format( _T("%.2f"), GetZPar( g_AlignerData.ObjNo[i] ) );
            RAdjText( &tmpStr, 6 );   
            zParText += tmpStr + _T("\n");
        }
        SetDlgItemText( IDC_ZPAR_TEXT, zParText );
        GetDlgItem( IDC_ZPAR_TEXT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_SMALL_SETUP_BOUNDARY )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_LARGE_SETUP_BOUNDARY )->ShowWindow( SW_SHOW );
    }
    else
    {
        GetDlgItem( IDC_ZPAR_TEXT )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_LARGE_SETUP_BOUNDARY )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_SMALL_SETUP_BOUNDARY )->ShowWindow( SW_SHOW );
    }

	CString setup;

	if( ( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) && ( m_pParent->m_EnableReference == TRUE ) )
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

void CAlignVerHorizonPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
  setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CAlignVerHorizonPage1::EnableDisableSperry()
{
	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		if( ( ( IsSelected( SPERRY_CHANNEL ) == TRUE ) || ( g_AlignerData.Mode == Stab ) ) && ( 0 == g_AlignerData.RX ) )
		{
			GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( TRUE );
			return;
		}
	}
	GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( FALSE );	
	m_SperryActiveBox.SetCheck( BST_UNCHECKED );
	return;
}

BEGIN_MESSAGE_MAP(CAlignVerHorizonPage1, CPropertyPage)
  ON_BN_CLICKED(IDC_ALIGN_VER_PARALLAX_COMP_ON, OnBnClickedAlignVerParallaxCompOn)
  ON_BN_CLICKED(IDC_ALIGN_VER_PARALLAX_COMP_OFF, OnBnClickedAlignVerParallaxCompOff)
  ON_EN_KILLFOCUS(IDC_ALIGN_VER_FILTER_TIME_CONSTANT, OnEnKillfocusAlignVerFilterTimeConstant)
  ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
	ON_BN_CLICKED(IDC_ALIGN_VER_SETUP_TEST, OnBnClickedAlignVerSetupTest)
END_MESSAGE_MAP()

// CAlignVerHorizonPage1 message handlers

BOOL CAlignVerHorizonPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CAlignmentWizard*)GetParent();
  ASSERT(m_pParent -> IsKindOf( RUNTIME_CLASS(CAlignmentWizard) ));
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAlignVerHorizonPage1::OnSetActive() 
{
    if( m_pParent == NULL )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
        EndDialog( IDCANCEL );
        return( FALSE ); // prevent the page from being active
    }

    m_pParent->SetWizardButtons( PSWIZB_NEXT );
    m_EnableFixPlane = FALSE;

	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

    //restore
    m_ParallaxComp = m_pParent->m_UseParallaxComp;
    m_FilterTimeConstant = g_AlignerData.TaoTilt; 

    if( m_pParent->m_EnableReference == FALSE )
    { // Horizon/Absolute test
        g_AlignerData.Mode = Stab;
        g_AlignerData.AlignMode.LoadString( IDS_TILT_VERIFICATION_HORIZON_TEST );
    }
    else
    {	// AirTarget/Relative test
        g_AlignerData.Mode = StabDiff;
        g_AlignerData.AlignMode.LoadString( IDS_TILT_VERIFICATION_AIR_TARGET_TEST );
    }

    if( g_AlignerData.TestNotSetup == TRUE )
    {
        CSetupTestPlaneDlg setupTestPlaneDlg( TRUE, m_pParent->m_EnableReference, m_EnableFixPlane );
        if( setupTestPlaneDlg.DoModal() == IDCANCEL )
		    {
			    EndDialog( IDCANCEL );
			    return( FALSE ); // prevent the page from being active
		    }
    }
    g_AlignerData.TestNotSetup = FALSE;
    ShowSetup();

    if( m_ParallaxComp == TRUE )
    {
        CheckRadioButton( IDC_ALIGN_VER_PARALLAX_COMP_ON, IDC_ALIGN_VER_PARALLAX_COMP_OFF, IDC_ALIGN_VER_PARALLAX_COMP_ON );
    }
    else
    {
        CheckRadioButton( IDC_ALIGN_VER_PARALLAX_COMP_ON, IDC_ALIGN_VER_PARALLAX_COMP_OFF, IDC_ALIGN_VER_PARALLAX_COMP_OFF );
    }

	ShowSettingsForAxes();
    UpdateData( FALSE );
    m_pParent->m_Status = STATUS_PAGE_READY;
    g_AlignerData.ErrorDef = ERR_NO_ERROR;

    return CPropertyPage::OnSetActive();
}

BOOL CAlignVerHorizonPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CAlignVerHorizonPage1::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

LRESULT CAlignVerHorizonPage1::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
    //store
    m_pParent->m_UseParallaxComp = m_ParallaxComp;
    g_AlignerData.TaoTilt = m_FilterTimeConstant; 
		if( m_SperryActiveBox.GetCheck() ==  BST_CHECKED )
		{
			m_pParent->SetSperryActive( TRUE );
		}
		else
		{
			m_pParent->SetSperryActive( FALSE );
		}
    return IDD_ALIGN_VER_HORIZON_PAGE_2; //ID of the next page to jump
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

void CAlignVerHorizonPage1::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CAlignVerHorizonPage1::OnBnClickedAlignVerParallaxCompOn()
{
  m_ParallaxComp = TRUE;
  ShowSetup();
}

void CAlignVerHorizonPage1::OnBnClickedAlignVerParallaxCompOff()
{
  m_ParallaxComp = FALSE;
  ShowSetup();
}

void CAlignVerHorizonPage1::OnEnKillfocusAlignVerFilterTimeConstant()
{
  UpdateData( TRUE );
}

void CAlignVerHorizonPage1::OnBnClickedSetupAxes()
{
  CSetupAxesDlg setupAxesDlg;
  setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CAlignVerHorizonPage1::OnBnClickedAlignVerSetupTest()
{
  CSetupTestPlaneDlg setupTestPlaneDlg( m_ParallaxComp, m_pParent->m_EnableReference, m_EnableFixPlane );
  setupTestPlaneDlg.DoModal();
  ShowSetup();
}
