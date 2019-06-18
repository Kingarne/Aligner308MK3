// $Id: TiltAlignmentPage1.cpp,v 1.4 2013/02/13 14:25:46 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "setupMeasureDlg.h"
#include "SetupAxesDlg.h"
#include "TiltAlignmentWizard.h"
#include "Filter.h"
#include "Util.h"


// CTiltAlignmentPage1 dialog

IMPLEMENT_DYNAMIC(CTiltAlignmentPage1, CPropertyPage)

CTiltAlignmentPage1::CTiltAlignmentPage1() : CPropertyPage(CTiltAlignmentPage1::IDD) , m_FilterTimeConstant(MIN_FILTER_TIME_CONSTANT)
{
}

CTiltAlignmentPage1::~CTiltAlignmentPage1()
{
}

void CTiltAlignmentPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TILT1_FILTER_TIME_CONSTANT, m_FilterTimeConstant);
	DDV_MinMaxDouble(pDX, m_FilterTimeConstant, MIN_FILTER_TIME_CONSTANT, MAX_FILTER_TIME_CONSTANT);
	DDX_Control(pDX, IDC_SPERRY_CHECK, m_SperryActiveBox);
}

void CTiltAlignmentPage1::ShowSetup()
{
	if (IsGunPresent() && !m_pParent->m_CommonFlatTest)
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
		GetMeasurementSetup( m_pParent->m_pSource, TRUE, &setup, m_pParent->m_CommonFlatTest );
	}
	else
	{
		GetMeasurementSetup( m_pParent->m_pSource, FALSE, &setup, m_pParent->m_CommonFlatTest );
	}
	SetDlgItemText( IDC_CONFIGURATION_TEXT, setup );

    if( ( IsOnlyFixedSelected() == TRUE ) || ( m_pParent->m_CommonFlatTest == TRUE ) )
    {
        HideLOSDirectionSelection();
    }
    else
    {
        ShowLOSDirectionSelection();
    }

    if( m_pParent->m_CommonFlatTest == TRUE )
    {
        HideSperry();
    }
    else
    {
	    EnableDisableSperry();
    }
}
/*
BOOL CTiltAlignmentPage1::IsGunPresent()
{	
	if ((g_AlignerData.NoOfCorr > 0) && (IsConnected(g_AlignerData.RefObjNo) == TRUE))
	{
		if (IsGun(g_AlignerData.RefObjNo) == TRUE)
		{
			return TRUE;			
		}

		for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
		{
			if (IsGun(g_AlignerData.ObjNo[i]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
*/

BOOL CTiltAlignmentPage1::IsOnlyFixedSelected()
{
    BOOL onlyFixedSelected = TRUE;

    if( (g_AlignerData.NoOfCorr > 0) && ( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) )
    {
		if( IsFixed( g_AlignerData.RefObjNo ) == FALSE )
        {
            onlyFixedSelected = FALSE;
        }

        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
		    if( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE )
            {
                onlyFixedSelected = FALSE;
            }
        }
    }
    return( onlyFixedSelected );
}

void CTiltAlignmentPage1::ShowLOSDirectionSelection()
{
    GetDlgItem( IDC_TILT1_LOS_DIR_0_180 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_TILT1_LOS_DIR_90_90 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_TILT1_LOS_DIRECTION_TEXT )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_TILT1_LOS_DIRECTION_GROUP )->ShowWindow( SW_SHOW );
}

void CTiltAlignmentPage1::HideLOSDirectionSelection()
{
    GetDlgItem( IDC_TILT1_LOS_DIR_0_180 )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_TILT1_LOS_DIR_90_90 )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_TILT1_LOS_DIRECTION_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_TILT1_LOS_DIRECTION_GROUP )->ShowWindow( SW_HIDE );
}

void CTiltAlignmentPage1::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
    setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
	EnableDisableSperry();
}

void CTiltAlignmentPage1::EnableDisableSperry()
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

void CTiltAlignmentPage1::HideSperry()
{
    GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_HIDE );
}

BEGIN_MESSAGE_MAP(CTiltAlignmentPage1, CPropertyPage)
    ON_BN_CLICKED(IDC_TILT1_SETUP_MEASUREMENT, OnBnClickedTilt1SetupMeasurement)
    ON_EN_KILLFOCUS(IDC_TILT1_FILTER_TIME_CONSTANT, OnEnKillfocusTilt1FilterTimeConstant)
    ON_BN_CLICKED(IDC_TILT1_LOS_DIR_0_180, OnBnClickedTilt1LosDir0180)
    ON_BN_CLICKED(IDC_TILT1_LOS_DIR_90_90, OnBnClickedTilt1LosDir9090)
    ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
END_MESSAGE_MAP()


// CTiltAlignmentPage1 message handlers

BOOL CTiltAlignmentPage1::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_pParent = (CTiltAlignmentWizard*) GetParent();   
    m_MsgCaption = _T("");
    m_Text = _T("");
    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltAlignmentPage1::OnSetActive() 
{
    if( m_pParent == NULL )
    {
        m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
        m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
        MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
        EndDialog( IDCANCEL );
        return FALSE;
    }
    m_pParent->SetWizardButtons( PSWIZB_NEXT );

    if( ( DAU::GetDAU().GetEnableSperryMeasurement() == TRUE ) && ( m_pParent->m_CommonFlatTest == FALSE ) )
	{
		GetDlgItem( IDC_SPERRY_BOUNDARY )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_SPERRY_CHECK )->ShowWindow( SW_SHOW );
	}

    if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
    {
        g_AlignerData.Along = TRUE;
        g_AlignerData.AzDir = PLUS_0_TO_PLUS_180;

        CSetupMeasureDlg setupMeasDlg( m_pParent->m_CommonFlatTest, FALSE );
        setupMeasDlg.DoModal();

        if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
        {
            //TODO error message
		    EndDialog( IDCANCEL );
            return FALSE;
        }
    }

    DAU::GetDAU().InitDataCompensator();
    ShowSetup();

    //restore
    m_LOSDirection = g_AlignerData.AzDir;
    m_FilterTimeConstant = g_AlignerData.TaoTilt; 
      
    if( m_LOSDirection == PLUS_0_TO_PLUS_180 )
    {
        CheckRadioButton( IDC_TILT1_LOS_DIR_0_180, IDC_TILT1_LOS_DIR_90_90, IDC_TILT1_LOS_DIR_0_180 );
    }
    else
    {
        CheckRadioButton( IDC_TILT1_LOS_DIR_0_180, IDC_TILT1_LOS_DIR_90_90, IDC_TILT1_LOS_DIR_90_90 );
    }

    ShowSettingsForAxes();

    UpdateData( FALSE );
    m_pParent->m_Status = STATUS_PAGE_READY;

    return CPropertyPage::OnSetActive();
}

BOOL CTiltAlignmentPage1::OnQueryCancel()
{
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_REALY_ABORT );
    return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CTiltAlignmentPage1::OnWizardNext()
{
    switch( m_pParent->m_Status )
    {
    case STATUS_PAGE_READY:
        //store 
        g_AlignerData.AzDir = m_LOSDirection;
        g_AlignerData.TaoTilt = m_FilterTimeConstant; 
        if( g_AlignerData.AzDir == PLUS_0_TO_PLUS_180 )
        {
            g_AlignerData.Along = TRUE;
        }
        else
        {
            g_AlignerData.Along = FALSE;
        }
		if( m_SperryActiveBox.GetCheck() ==  BST_CHECKED )
		{
			m_pParent->SetSperryActive( TRUE );
		}
		else
		{
			m_pParent->SetSperryActive( FALSE );
		}
        m_pParent->m_Status = STATUS_PAGE_NOT_READY; //next page not ready
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

void CTiltAlignmentPage1::OnReset()
{
    m_pParent->m_Status = STATUS_PAGE_CANCELED;
    g_AlignerData.ErrorDef = ERR_CANCEL;
    return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltAlignmentPage1::OnBnClickedTilt1SetupMeasurement()
{
    CSetupMeasureDlg setupMeasDlg( m_pParent->m_CommonFlatTest, FALSE );
    setupMeasDlg.DoModal();
    DAU::GetDAU().InitDataCompensator();
    ShowSetup();
}

void CTiltAlignmentPage1::OnBnClickedSetupAxes()
{
    CSetupAxesDlg setupAxesDlg;
    setupAxesDlg.DoModal();
    ShowSettingsForAxes();
}

void CTiltAlignmentPage1::OnEnKillfocusTilt1FilterTimeConstant()
{
    UpdateData( TRUE );
}

void CTiltAlignmentPage1::OnBnClickedTilt1LosDir0180()
{
    m_LOSDirection = PLUS_0_TO_PLUS_180;
}

void CTiltAlignmentPage1::OnBnClickedTilt1LosDir9090()
{
    m_LOSDirection = MINUS_90_TO_PLUS_90;
}


