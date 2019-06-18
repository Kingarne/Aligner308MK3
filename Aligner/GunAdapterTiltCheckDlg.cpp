// $Id: GunAdapterTiltCheckDlg.cpp,v 1.3 2012/07/17 09:33:07 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "GunAdapterTiltCheckDlg.h"
#include "SetupSpecGBDlg.h"
#include "SetupAxesDlg.h"
#include "Util.h"
#include "Measure.h"

// CGunAdapterTiltCheckDlg dialog

IMPLEMENT_DYNAMIC(CGunAdapterTiltCheckDlg, CDialog)
CGunAdapterTiltCheckDlg::CGunAdapterTiltCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGunAdapterTiltCheckDlg::IDD, pParent)
{
}

CGunAdapterTiltCheckDlg::~CGunAdapterTiltCheckDlg()
{
}

void CGunAdapterTiltCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CGunAdapterTiltCheckDlg::ShowSettingsForAxes()
{
	CString XAxisSetting, YAxisMinSetting, YAxisMaxSetting;
	CSetupAxesDlg setupAxesDlg;
	setupAxesDlg.GetSettingsForAxes( &XAxisSetting, &YAxisMinSetting, &YAxisMaxSetting );
	XAxisSetting.LoadString( IDS_FIXED );
	//SetDlgItemText( IDC_AXIS_X_VALUE, XAxisSetting );
	SetDlgItemText( IDC_AXIS_Y_MIN_VALUE, YAxisMinSetting );
    SetDlgItemText( IDC_AXIS_Y_MAX_VALUE, YAxisMaxSetting );
}

void CGunAdapterTiltCheckDlg::ShowSetup()
{
	CString setup;

	if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
	{
		GetMeasurementSetup( m_pGraph->GetSource(), TRUE, &setup );
	}
	else
	{
		GetMeasurementSetup( m_pGraph->GetSource(), FALSE, &setup );
	}
	SetDlgItemText( IDC_CONFIGURATION_TEXT, setup );
}

BEGIN_MESSAGE_MAP(CGunAdapterTiltCheckDlg, CDialog)
  ON_BN_CLICKED(IDC_SHOW_GRAPH, OnBnClickedShowGraph)
  ON_BN_CLICKED(IDC_SETUP_MEASUREMENT, OnBnClickedSetupMeasurement)
	ON_BN_CLICKED(IDC_SETUP_AXES, OnBnClickedSetupAxes)
  ON_WM_MOVE()
  ON_WM_MOVING()
  ON_WM_WINDOWPOSCHANGING()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_SETFOCUS()
  ON_WM_KILLFOCUS()
  ON_WM_NCACTIVATE()
  ON_WM_NCLBUTTONDOWN()
  ON_WM_NCRBUTTONDOWN()
  ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CGunAdapterTiltCheckDlg message handlers

BOOL CGunAdapterTiltCheckDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

	m_freqMode = FQ_MODE_305;
	m_Tao = 5.0;
  BOOL gunExists = FALSE;
  GetDlgItem( IDOK )->EnableWindow( FALSE );

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		if( IsGun( i ) == TRUE )
    {
      gunExists = TRUE;
    }
  }

  if( gunExists == FALSE )
  {
    CString msgCaption;
    msgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_CONNECT_AT_LEAST_ONE_G_STATION );
    MessageBox( m_Text, msgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  m_pGraph = static_cast<CGraphView *>(theApp.m_pGraphView);

  m_Text = _T("");
  m_ShowGraphPressed = FALSE;

  g_AlignerData.AlignMode.LoadString( IDS_GUN_ADAPTER_TILT_CHECK );

  if( IsConnected( g_AlignerData.RefObjNo ) == FALSE )
  {
    CSetupSpecGBDlg SetupSpecGBDlg( this );

    if( SetupSpecGBDlg.DoModal() == IDCANCEL )
    {
      EndDialog( IDCANCEL );
      return FALSE;
    }
  }

  if( InitGraph() == FALSE )
  {
    //TODO error message
		EndDialog( IDCANCEL );
    return( FALSE );
  }

	ShowSetup();
	ShowSettingsForAxes();
  UpdateData( FALSE );

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGunAdapterTiltCheckDlg::Destroy() 
{
	theApp.SwitchView( theApp.m_pSystemConfigurationView );

	if( m_pGraph->GetSource() != NULL )
	{
    m_pGraph->GetSource()->EnableErrorEvent( FALSE );
		m_pGraph->GetSource()->Stop();
    ResetAllFilters( m_pGraph->GetSource() );
	}
  else
  {
    DAU::GetDAU().EnableErrorEvent( FALSE );
  }
	m_pGraph->SetSource( NULL );
}

void CGunAdapterTiltCheckDlg::OnBnClickedShowGraph()
{
  m_ShowGraphPressed = TRUE;
  MoveDlgToRightSideOfApp(this );
  //MoveDlgToRightSideOfTheGraph( this, m_pGraph );

  if( m_pGraph->GetSource() != NULL )
  {
    SetSelectedChannels( m_pGraph->GetSource() );
    m_pGraph->GetSource()->EnableErrorEvent( FALSE );
  }
  else
  {
    SetSelectedChannels();
    DAU::GetDAU().EnableErrorEvent( FALSE );
  }

  //Get the timestamp to be stored in the graph and table
  m_BarGraphParam.TimeStamp = GetTimeStr( TRUE );
  m_BarGraphParam.DateStamp = GetDateStr();
  m_pGraph->SetNoOfSeries( g_AlignerData.NoOfCorr );
  m_pGraph->ShowBarGraphWithText( &m_BarGraphParam );

  //Show live data
  GetDlgItem( IDC_SHOW_GRAPH )->EnableWindow( FALSE );
  GetDlgItem( IDOK )->EnableWindow( TRUE );
}

void CGunAdapterTiltCheckDlg::OnBnClickedSetupMeasurement()
{
  CSetupSpecGBDlg SetupSpecGBDlg;

  if( (SetupSpecGBDlg.DoModal() == IDOK) &&  (m_ShowGraphPressed == TRUE) )
  {
    m_pGraph->HideGraph();
    OnBnClickedShowGraph();
  }
	ShowSetup();
}

BOOL CGunAdapterTiltCheckDlg::InitGraph()
{
	m_pGraph->InitDefaultBarGraph( g_AlignerData.NoOfCorr );

  CString str;

  if( GetMRad() == TRUE )
  {
    str.LoadString( IDS_MRAD_UNIT );
    m_pGraph->SetAxisYUnit( &str );
  }
  else
  {
    str.LoadString( IDS_ARCMIN_UNIT );
    m_pGraph->SetAxisYUnit( &str );
  }

	if( g_AlignerData.RY == 0 )
	{
		m_pGraph->SetAxisYMinAutoScaleOn( TRUE );
		m_pGraph->SetAxisYMaxAutoScaleOn( TRUE );
	}
    else 
    {
		m_pGraph->SetAxisYMinAutoScaleOn( FALSE );
		m_pGraph->SetAxisYMaxAutoScaleOn( FALSE );
        CSetupAxesDlg setupAxesDlg;
		m_pGraph->SetAxisYMin( (double)setupAxesDlg.GetMinY() );
		m_pGraph->SetAxisYMax( (double)setupAxesDlg.GetMaxY() );
	}
  
    m_pGraph -> SetSource( &DAU::GetDAU() ) ;
    ResetAllFilters( m_pGraph->GetSource(), DAU::GetDAU().GetPeriod(), TETA, m_Tao ) ;
	
    DAU::GetDAU().m_comThr.SetSamplingMode(m_freqMode);		

  return( TRUE );
}

void CGunAdapterTiltCheckDlg::OnOK()
{
  //CString caption, text;
  //caption.LoadString( IDS_QUESTION_CAPTION );
  //text.LoadString( IDS_EXIT_WITH_SAVE );

  //if( IDYES == MessageBox( text, caption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
  //{
	  g_AlignerData.GunAdapterTiltCheckOK = TRUE;
    Destroy();
    CDialog::OnOK();
  //}
  //else
  //{
  //  return;
  //}
}

void CGunAdapterTiltCheckDlg::OnCancel()
{
  //CString caption, text;
  //caption.LoadString( IDS_QUESTION_CAPTION );
  //text.LoadString( IDS_EXIT_WITHOUT_SAVE );

  //if( MessageBox( text, caption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDNO )
  //{
  //  return;
  //}
  Destroy();
  CDialog::OnCancel();
}

void CGunAdapterTiltCheckDlg::OnBnClickedSetupAxes()
{
  CSetupAxesDlg setupAxesDlg( TRUE );

  if( setupAxesDlg.DoModal() == IDOK )
	{
		m_pGraph->HideGraph();

		if( g_AlignerData.RY == 0 )
		{
			m_pGraph->SetAxisYMinAutoScaleOn( TRUE );
			m_pGraph->SetAxisYMaxAutoScaleOn( TRUE );
		}
        else
		{
			m_pGraph->SetAxisYMinAutoScaleOn( FALSE );
			m_pGraph->SetAxisYMaxAutoScaleOn( FALSE );
            CSetupAxesDlg setupAxesDlg;
		    m_pGraph->SetAxisYMin( (double)setupAxesDlg.GetMinY() );
		    m_pGraph->SetAxisYMax( (double)setupAxesDlg.GetMaxY() );
		}

    if( m_ShowGraphPressed == TRUE )
    {
		  m_pGraph->ShowGraph();
    }
	}
  ShowSettingsForAxes();
}

void CGunAdapterTiltCheckDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  if( m_ShowGraphPressed == FALSE )
  {
    CDialog::OnNcLButtonDown(nHitTest, point);
  }
}

void CGunAdapterTiltCheckDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
  if( m_ShowGraphPressed == FALSE )
  {
    CDialog::OnNcRButtonDown(nHitTest, point);
  }
}
