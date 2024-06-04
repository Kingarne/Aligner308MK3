// $Id: TiltFlatnessFoundationTestPage1.cpp,v 1.2 2013/10/01 13:25:11 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SetupFoundationMeasureDlg.h"
#include "SetupAxesDlg.h"
#include "TiltFlatnessTestWizard.h"
#include "Filter.h"
#include "Util.h"

IMPLEMENT_DYNAMIC(CTiltFlatnessFoundationTestPage1, CPropertyPage)

CTiltFlatnessFoundationTestPage1::CTiltFlatnessFoundationTestPage1()
	: CPropertyPage(CTiltFlatnessFoundationTestPage1::IDD)
  , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CTiltFlatnessFoundationTestPage1::~CTiltFlatnessFoundationTestPage1()
{
}

void CTiltFlatnessFoundationTestPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_TILT_FLATNESS_TEST_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
    DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);

    DDX_Text(pDX, IDC_LENGTH_CH_1, m_armLength1);
    DDV_MinMaxInt(pDX, m_armLength1, MIN_LENGTH, MAX_LENGTH);
    DDX_Text(pDX, IDC_ARM2_LENGTH, m_armLength2);
    DDV_MinMaxInt(pDX, m_armLength2, MIN_LENGTH, MAX_LENGTH);
    DDX_Text(pDX, IDC_ANGLE_CH_1, m_refAngle);
    DDV_MinMaxInt(pDX, m_refAngle, MIN_ANGLE, MAX_ANGLE);
}

void CTiltFlatnessFoundationTestPage1::ShowSetup()
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

    if( IsPlaneOrGunSelected() == TRUE )
    {
        m_pParent->SetWizardButtons( PSWIZB_NEXT );
    }
	EnableDisableSperry();

    m_refAngle = GetZeroRef(1);
    if (g_AlignerData.FoundationType == FoundationT::Circular)
    {
      m_armLength1 = GetIndexArmLength(1);
      m_armLength2 = GetIndexArmLength(2);

      BOOL measureWarp = (m_armLength2 > 0);
      GetDlgItem(IDC_INDEX_ARM2_LENGTH_TEXT)->ShowWindow(measureWarp ? SW_SHOW : SW_HIDE);
      GetDlgItem(IDC_ARM2_LENGTH)->ShowWindow(measureWarp ? SW_SHOW : SW_HIDE);
    }
    else
    {
      GetDlgItem(IDC_INDEX_ARM_LENGTH_TEXT)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_LENGTH_CH_1)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_INDEX_ARM2_LENGTH_TEXT)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_ARM2_LENGTH)->ShowWindow(SW_HIDE);

    }


    UpdateData( FALSE );
	UpdateData( TRUE );
}

BOOL CTiltFlatnessFoundationTestPage1::IsPlaneOrGunSelected()
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

void CTiltFlatnessFoundationTestPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
	setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CTiltFlatnessFoundationTestPage1::EnableDisableSperry()
{
	if( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE )
	{
		if( ( IsSelected( SPERRY_CHANNEL ) == TRUE ) && ( 0 == g_AlignerData.RX ) )
		{
			GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( TRUE );
			return;
		}
	}
	
    GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( FALSE );	
    GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( FALSE );	
    GetDlgItem( IDC_SPERRY_CHECK )->EnableWindow( FALSE );	
	m_SperryActiveBox.SetCheck( BST_UNCHECKED );
	return;
}

BEGIN_MESSAGE_MAP(CTiltFlatnessFoundationTestPage1, CPropertyPage)
  ON_EN_KILLFOCUS(IDC_TILT_FLATNESS_TEST_FILTER_TIME_CONSTANT, OnEnKillfocusTiltFlatnessTestFilterTimeConstant)
  ON_BN_CLICKED(IDC_TILT_FLATNESS_TEST_SETUP_MEASUREMENT, OnBnClickedTiltFlatnessTestSetupMeasurement)
  ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
END_MESSAGE_MAP()


// CTiltFlatnessFoundationTestPage1 message handlers

BOOL CTiltFlatnessFoundationTestPage1::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_pParent = (CTiltFlatnessTestWizard*) GetParent();   
    m_MsgCaption = _T("");
    m_Text = _T("");
    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltFlatnessFoundationTestPage1::OnSetActive() 
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

BOOL CTiltFlatnessFoundationTestPage1::OnQueryCancel()
{
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_REALY_ABORT );
    return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CTiltFlatnessFoundationTestPage1::OnWizardNext()
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
	    
        m_pParent->SetSperryActive( m_SperryActiveBox.GetCheck() ==  BST_CHECKED );	
        
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

void CTiltFlatnessFoundationTestPage1::OnReset()
{
    m_pParent->m_Status = STATUS_PAGE_CANCELED;
    g_AlignerData.ErrorDef = ERR_CANCEL;
    return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltFlatnessFoundationTestPage1::OnBnClickedTiltFlatnessTestSetupMeasurement()
{
    CSetupFoundationMeasureDlg setupMeasDlg( FALSE, g_AlignerData.Found, TRUE );
    setupMeasDlg.DoModal();
    ShowSetup();
}

void CTiltFlatnessFoundationTestPage1::OnBnClickedSetupAxes()
{
    CSetupAxesDlg setupAxesDlg;
    setupAxesDlg.DoModal();
	ShowSettingsForAxes();
}

void CTiltFlatnessFoundationTestPage1::OnEnKillfocusTiltFlatnessTestFilterTimeConstant()
{
    UpdateData( TRUE );
}

