// $Id: SetupAxesDlg.cpp,v 1.2 2009/12/08 19:14:22 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "SetupAxesDlg.h"
#include "GlobalData.h"

BOOL CSetupAxesDlg::m_AxesInitiated;
int CSetupAxesDlg::m_MaxY;
int CSetupAxesDlg::m_MinY;// (m_MaxY - m_MinY) copied to g_AlignerData.RY
BOOL CSetupAxesDlg::m_DisableAutoXAxis;

IMPLEMENT_DYNAMIC(CSetupAxesDlg, CDialog)

CSetupAxesDlg::CSetupAxesDlg( BOOL disableXAxis /*= FALSE*/, BOOL disableAutoXAxis /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupAxesDlg::IDD, pParent)
  , m_RangeX(0)
	, m_continousGraph(FALSE)
{
  m_DisableXAxis = disableXAxis;
  m_DisableAutoXAxis = disableAutoXAxis;
  InitiateLiveGraphAxes();
}

CSetupAxesDlg::~CSetupAxesDlg()
{
}

void CSetupAxesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETUP_AXES_X_MANUAL_VALUE, m_RangeX);

	switch (m_XUnit)
	{
	case X_UNIT_HRS:
		DDV_MinMaxDouble(pDX, m_RangeX, MIN_RANGE_X_HOURS, MAX_RANGE_X_HOURS);
		break;
	case X_UNIT_MIN:
		DDV_MinMaxDouble(pDX, m_RangeX, MIN_RANGE_X_MIN, MAX_RANGE_X_MIN);
		break;
	case X_UNIT_S:
	default:
		DDV_MinMaxDouble(pDX, m_RangeX, MIN_RANGE_X_S, MAX_RANGE_X_S);
		break;
	}

	switch (m_YUnit)
	{
	case Y_UNIT_ARCMIN:
		DDX_Text(pDX, IDC_SETUP_AXES_Y_MIN, m_MinYDlg);
		DDV_MinMaxInt(pDX, m_MinYDlg, MIN_MIN_Y_ARCMIN, MAX_MIN_Y_ARCMIN);
		DDX_Text(pDX, IDC_SETUP_AXES_Y_MAX, m_MaxYDlg);
		DDV_MinMaxInt(pDX, m_MaxYDlg, MIN_MAX_Y_ARCMIN, MAX_MAX_Y_ARCMIN);
		break;
	case Y_UNIT_MRAD:
	default:
		DDX_Text(pDX, IDC_SETUP_AXES_Y_MIN, m_MinYDlg);
		DDV_MinMaxInt(pDX, m_MinYDlg, MIN_MIN_Y_MRAD, MAX_MIN_Y_MRAD);
		DDX_Text(pDX, IDC_SETUP_AXES_Y_MAX, m_MaxYDlg);
		DDV_MinMaxInt(pDX, m_MaxYDlg, MIN_MAX_Y_MRAD, MAX_MAX_Y_MRAD);
		break;
	}

	DDX_Check(pDX, IDC_CONTINOUS_CHECK, m_continousGraph);
}

void CSetupAxesDlg::GetSettingsForAxes( CString* pXAxisSettings, CString* pYAxisMinSettings, CString* pYAxisMaxSettings )
{
  CString unitStr;
  *pXAxisSettings = _T("");
  *pYAxisMinSettings = _T("");
  *pYAxisMaxSettings = _T("");

  if( ( g_AlignerData.RX >= MIN_RANGE_X_S ) && ( g_AlignerData.RX <= MAX_RANGE_X_S ) )
  {
    pXAxisSettings->Format( _T("%d "), g_AlignerData.RX );
    unitStr.LoadString( IDS_S_UNIT );
    *pXAxisSettings += unitStr;
  }
  else
  {
    if( m_DisableAutoXAxis == TRUE )
    {
      g_AlignerData.RX = DEFAULT_RANGE_X_S;
      pXAxisSettings->Format( _T("%d "), g_AlignerData.RX );
      unitStr.LoadString( IDS_S_UNIT );
      *pXAxisSettings += unitStr;
    }
    else
    {
      pXAxisSettings->LoadString( IDS_AUTO );
    }    
  }

  if( GetMRad() == TRUE )
  {
    unitStr.LoadString( IDS_MRAD_UNIT );
  }
  else
  {
    unitStr.LoadString( IDS_ARCMIN_UNIT );
  }

  if( g_AlignerData.RY == 0 )
  {
      pYAxisMinSettings->LoadString( IDS_AUTO );
	  *pYAxisMinSettings += _T(" ");
      pYAxisMaxSettings->LoadString( IDS_AUTO );
	  *pYAxisMaxSettings += _T(" ");
  }
  else
  {
    if( GetMRad() == TRUE )
    {
        if( m_MinY < MIN_MIN_Y_MRAD )
        {
        m_MinY = MIN_MIN_Y_MRAD;
        }
        else if( m_MinY > MAX_MIN_Y_MRAD )
        {
        m_MinY = MAX_MIN_Y_MRAD;
        }

        if( m_MaxY < MIN_MAX_Y_MRAD )
        {
        m_MaxY = MIN_MAX_Y_MRAD;
        }
        else if( m_MaxY > MAX_MAX_Y_MRAD )
        {
        m_MaxY = MAX_MAX_Y_MRAD;
        }
        pYAxisMinSettings->Format( _T("%+d "), m_MinY );  
        pYAxisMaxSettings->Format( _T("%+d "), m_MaxY );  
    }
    else
    {
        if( m_MinY < MIN_MIN_Y_ARCMIN )
        {
        m_MinY = MIN_MIN_Y_ARCMIN;
        }
        else if( m_MinY > MAX_MIN_Y_ARCMIN )
        {
        m_MinY = MAX_MIN_Y_ARCMIN;
        }

        if( m_MaxY < MIN_MAX_Y_ARCMIN )
        {
        m_MaxY = MIN_MAX_Y_ARCMIN;
        }
        else if( m_MaxY > MAX_MAX_Y_ARCMIN )
        {
        m_MaxY = MAX_MAX_Y_ARCMIN;
        }
        pYAxisMinSettings->Format( _T("%+d "), m_MinY );  
        pYAxisMaxSettings->Format( _T("%+d "), m_MaxY );    
    }
  }
  RAdjText( pYAxisMinSettings, 5 );
  RAdjText( pYAxisMaxSettings, 5 );
  *pYAxisMinSettings += unitStr;
  *pYAxisMaxSettings += unitStr;
  //g_AlignerData.RY = (long int)( m_MaxY - m_MinY );
  return; 
}

BEGIN_MESSAGE_MAP(CSetupAxesDlg, CDialog)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_AUTO, OnBnClickedSetupAxisXAuto)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_MANUAL, OnBnClickedSetupAxisXManual)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_S_UNIT, OnBnClickedSetupAxisXSUnit)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_MIN_UNIT, OnBnClickedSetupAxisXMinUnit)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_HRS_UNIT, OnBnClickedSetupAxisXHrsUnit)
  ON_BN_CLICKED(IDC_SETUP_AXES_Y_AUTO, OnBnClickedSetupAxisYAuto)
  ON_BN_CLICKED(IDC_SETUP_AXES_Y_MANUAL, OnBnClickedSetupAxisYManual)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_X_MANUAL_VALUE, OnEnKillfocusSetupAxisXManualValue)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y_MIN, OnEnKillfocusSetupAxesYMin)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y_MAX, OnEnKillfocusSetupAxesYMax)

END_MESSAGE_MAP()


// CSetupAxesDlg message handlers
BOOL CSetupAxesDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  //InitiateLiveGraphAxes();
  m_MaxYDlg = m_MaxY;
  m_MinYDlg = m_MinY;

  CString str;
  str.LoadString( IDS_S_UNIT );
  SetDlgItemText( IDC_SETUP_AXES_X_S_UNIT, str );
  str.LoadString( IDS_MIN_UNIT );
  SetDlgItemText( IDC_SETUP_AXES_X_MIN_UNIT, str );
  str.LoadString( IDS_HRS_UNIT );
  SetDlgItemText( IDC_SETUP_AXES_X_HRS_UNIT, str );

  if( ( g_AlignerData.RX >= MIN_RANGE_X_S ) && ( g_AlignerData.RX <= MAX_RANGE_X_S ) )
  {
    m_RangeX = (double)g_AlignerData.RX;
    OnBnClickedSetupAxisXManual();
  }
  else
  {
    m_RangeX = DEFAULT_RANGE_X_S;

    if( m_DisableAutoXAxis == TRUE )
    {
      OnBnClickedSetupAxisXManual();
    }
    else
    {
      OnBnClickedSetupAxisXAuto();
    }
  }
  m_XUnit = X_UNIT_S;
  OnBnClickedSetupAxisXSUnit();

  if( g_AlignerData.RY == 0 )
  {
    OnBnClickedSetupAxisYAuto();
  }
  else
  {
    OnBnClickedSetupAxisYManual();
  }
  if( GetMRad() == TRUE )
  {
    m_YUnit = Y_UNIT_MRAD;
	str.LoadString( IDS_MRAD_UNIT );
  }
  else
  {
    m_YUnit = Y_UNIT_ARCMIN;
    str.LoadString( IDS_ARCMIN_UNIT );
  }
  SetDlgItemText( IDC_Y_AXIS_UNIT, str );

	if( m_DisableXAxis == TRUE )
	{
		GetDlgItem( IDC_SETUP_AXES_X_AUTO )->EnableWindow( FALSE );
		GetDlgItem( IDC_SETUP_AXES_X_MANUAL )->EnableWindow( FALSE );
		GetDlgItem( IDC_SETUP_AXES_X_MANUAL_VALUE )->EnableWindow( FALSE );
		GetDlgItem( IDC_SETUP_AXES_X_S_UNIT )->EnableWindow( FALSE );
		GetDlgItem( IDC_SETUP_AXES_X_MIN_UNIT )->EnableWindow( FALSE );
		GetDlgItem( IDC_SETUP_AXES_X_HRS_UNIT )->EnableWindow( FALSE );
		GetDlgItem( IDC_X_TEXT )->EnableWindow( FALSE );
		GetDlgItem(IDC_CONTINOUS_CHECK)->EnableWindow(FALSE);
		
	}
  else if( m_DisableAutoXAxis == TRUE )
  {
    GetDlgItem( IDC_SETUP_AXES_X_AUTO )->EnableWindow( FALSE );
  }

	m_continousGraph = g_AlignerData.ContinousGraph;

  UpdateData( FALSE );
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetupAxesDlg::OnBnClickedSetupAxisXAuto()
{
	m_RangeXAuto = TRUE;
	CheckRadioButton( IDC_SETUP_AXES_X_AUTO, IDC_SETUP_AXES_X_MANUAL, IDC_SETUP_AXES_X_AUTO );
	GetDlgItem( IDC_SETUP_AXES_X_MANUAL_VALUE )->EnableWindow( FALSE );
	GetDlgItem( IDC_SETUP_AXES_X_S_UNIT )->EnableWindow( FALSE );
	GetDlgItem( IDC_SETUP_AXES_X_MIN_UNIT )->EnableWindow( FALSE );
	GetDlgItem( IDC_SETUP_AXES_X_HRS_UNIT )->EnableWindow( FALSE );
	GetDlgItem(IDC_CONTINOUS_CHECK)->EnableWindow(FALSE);
}

void CSetupAxesDlg::OnBnClickedSetupAxisXManual()
{
	m_RangeXAuto = FALSE;
	CheckRadioButton( IDC_SETUP_AXES_X_AUTO, IDC_SETUP_AXES_X_MANUAL, IDC_SETUP_AXES_X_MANUAL );
	GetDlgItem( IDC_SETUP_AXES_X_MANUAL_VALUE )->EnableWindow( TRUE );
	GetDlgItem( IDC_SETUP_AXES_X_S_UNIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_SETUP_AXES_X_MIN_UNIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_SETUP_AXES_X_HRS_UNIT )->EnableWindow( TRUE );
	GetDlgItem(IDC_CONTINOUS_CHECK)->EnableWindow(TRUE);
}

void CSetupAxesDlg::OnBnClickedSetupAxisXSUnit()
{
  if( m_XUnit == X_UNIT_HRS )
  {
    m_RangeX = HOURS_TO_SECONDS( m_RangeX );
  }
  else if( m_XUnit == X_UNIT_MIN )
  {
    m_RangeX = MINUTES_TO_SECONDS( m_RangeX );
  }

  if( m_RangeX < MIN_RANGE_X_S )
  {
    m_RangeX = MIN_RANGE_X_S;
  }
  else if( m_RangeX > MAX_RANGE_X_S )
  {
    m_RangeX = MAX_RANGE_X_S;
  }
  m_XUnit = X_UNIT_S;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_S_UNIT );
  UpdateData( FALSE );
}

void CSetupAxesDlg::OnBnClickedSetupAxisXMinUnit()
{
  if( m_XUnit == X_UNIT_HRS )
  {
    m_RangeX = HOURS_TO_MINUTES( m_RangeX );
  }
  else if( m_XUnit == X_UNIT_S )
  {
    m_RangeX = SECONDS_TO_MINUTES( m_RangeX );
  }

  if( m_RangeX < MIN_RANGE_X_MIN )
  {
    m_RangeX = MIN_RANGE_X_MIN;
  }
  else if( m_RangeX > MAX_RANGE_X_MIN )
  {
    m_RangeX = MAX_RANGE_X_MIN;
  }
  m_XUnit = X_UNIT_MIN;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_MIN_UNIT );
  UpdateData( FALSE );
}

void CSetupAxesDlg::OnBnClickedSetupAxisXHrsUnit()
{
  if( m_XUnit == X_UNIT_MIN )
  {
    m_RangeX = MINUTES_TO_HOURS( m_RangeX );
  }
  else if( m_XUnit == X_UNIT_S )
  {
    m_RangeX = SECONDS_TO_HOURS( m_RangeX );
  }

  if( m_RangeX < MIN_RANGE_X_HOURS )
  {
    m_RangeX = MIN_RANGE_X_HOURS;
  }
  else if( m_RangeX > MAX_RANGE_X_HOURS )
  {
    m_RangeX = MAX_RANGE_X_HOURS;
  }
  m_XUnit = X_UNIT_HRS;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_HRS_UNIT );


  UpdateData( FALSE );
}

void CSetupAxesDlg::OnBnClickedSetupAxisYAuto()
{
  m_RangeYAuto = TRUE;
  CheckRadioButton( IDC_SETUP_AXES_Y_AUTO, IDC_SETUP_AXES_Y_MANUAL, IDC_SETUP_AXES_Y_AUTO );
  GetDlgItem( IDC_SETUP_AXES_Y_MAX )->EnableWindow( FALSE );
  GetDlgItem( IDC_SETUP_AXES_Y_MIN )->EnableWindow( FALSE );
}

void CSetupAxesDlg::OnBnClickedSetupAxisYManual()
{
  m_RangeYAuto = FALSE;
  CheckRadioButton( IDC_SETUP_AXES_Y_AUTO, IDC_SETUP_AXES_Y_MANUAL, IDC_SETUP_AXES_Y_MANUAL );
  GetDlgItem( IDC_SETUP_AXES_Y_MAX )->EnableWindow( TRUE );
  GetDlgItem( IDC_SETUP_AXES_Y_MIN )->EnableWindow( TRUE );
}

void CSetupAxesDlg::OnEnKillfocusSetupAxisXManualValue()
{
  UpdateData( TRUE );
}

void CSetupAxesDlg::OnEnKillfocusSetupAxisYManualValue()
{
  UpdateData( TRUE );
}

void CSetupAxesDlg::OnEnKillfocusSetupAxesYMax()
{
  UpdateData( TRUE );
}

void CSetupAxesDlg::OnEnKillfocusSetupAxesYMin()
{
  UpdateData( TRUE );
}

void CSetupAxesDlg::OnBnClickedOk()
{
  UpdateData(TRUE);
  CString msgCaption, text;
  msgCaption.LoadString( IDS_ERROR_CAPTION );

  if( m_RangeXAuto == TRUE )
  {
    g_AlignerData.RX = 0;
  }
  else
  {
    if( m_XUnit == X_UNIT_HRS )
    {
      m_RangeX = HOURS_TO_SECONDS( m_RangeX );
    }
    else if( m_XUnit == X_UNIT_MIN )
    {
      m_RangeX = MINUTES_TO_SECONDS( m_RangeX );
    }

    if( m_RangeX < MIN_RANGE_X_S )
    {
      m_RangeX = MIN_RANGE_X_S;
    }
    else if( m_RangeX > MAX_RANGE_X_S )
    {
      m_RangeX = MAX_RANGE_X_S;
    }
    g_AlignerData.RX = (long int)m_RangeX;
	g_AlignerData.ContinousGraph = m_continousGraph;
  }

  if( m_RangeYAuto == TRUE )
  {
    g_AlignerData.RY = 0;
  }
  else
  {
    if( m_YUnit == Y_UNIT_MRAD )
    {
      SetMRad( TRUE );
      if( m_MinYDlg >= m_MaxYDlg )
      {
        if( m_MaxYDlg > MIN_MIN_Y_MRAD )
        {
          text.LoadString( IDS_MIN_MUST_BE_SMALLER_THAN_MAX );
          GetDlgItem( IDC_SETUP_AXES_Y_MIN )->SetFocus();
        }
        else
        {
          text.LoadString( IDS_MAX_MUST_BE_BIGGER_THAN_MIN );
          GetDlgItem( IDC_SETUP_AXES_Y_MAX )->SetFocus();
        }
        MessageBox( text, msgCaption, MB_ICONINFORMATION|MB_OK );
        return;
      }

      m_MaxY = m_MaxYDlg;
      m_MinY = m_MinYDlg;

	  if( m_MaxY < MIN_MAX_Y_MRAD )
	  {
	    m_MaxY = MIN_MAX_Y_MRAD;
	  }
	  else if( m_MaxY > MAX_MAX_Y_MRAD )
	  {
	    m_MaxY = MAX_MAX_Y_MRAD;
	  }
	  if( m_MinY < MIN_MIN_Y_MRAD )
	  {
	    m_MinY = MIN_MIN_Y_MRAD;
	  }
	  else if( m_MinY > MAX_MIN_Y_MRAD )
	  {
	    m_MinY = MAX_MIN_Y_MRAD;
	  }
    }
    else
    {
      SetMRad( FALSE );
      if( m_MinYDlg >= m_MaxYDlg )
      {
        if( m_MaxYDlg > MIN_MIN_Y_ARCMIN )
        {
          text.LoadString( IDS_MIN_MUST_BE_SMALLER_THAN_MAX );
          GetDlgItem( IDC_SETUP_AXES_Y_MIN )->SetFocus();
        }
        else
        {
          text.LoadString( IDS_MAX_MUST_BE_BIGGER_THAN_MIN );
          GetDlgItem( IDC_SETUP_AXES_Y_MAX )->SetFocus();
        }
        MessageBox( text, msgCaption, MB_ICONINFORMATION|MB_OK );
        return;
      }

      m_MaxY = m_MaxYDlg;
      m_MinY = m_MinYDlg;

	  if( m_MaxY < MIN_MAX_Y_ARCMIN )
	  {
		m_MaxY = MIN_MAX_Y_ARCMIN;
	  }
	  else if( m_MaxY > MAX_MAX_Y_ARCMIN )
	  {
		m_MaxY = MAX_MAX_Y_ARCMIN;
	  }
	  if( m_MinY < MIN_MIN_Y_ARCMIN )
	  {
		m_MinY = MIN_MIN_Y_ARCMIN;
	  }
	  else if( m_MinY > MAX_MIN_Y_ARCMIN )
	  {
	    m_MinY = MAX_MIN_Y_ARCMIN;
	  }
    }

    g_AlignerData.RY = (long int)( m_MaxY - m_MinY );
  }

  OnOK();
}

void CSetupAxesDlg::InitiateLiveGraphAxes( void )
{
  if( m_AxesInitiated == FALSE )
  {
    if( GetMRad() == TRUE )
    {
        m_MaxY = DEFAULT_MAX_Y_MRAD;
        m_MinY = DEFAULT_MIN_Y_MRAD;
    }
    else
    {
        m_MaxY = DEFAULT_MAX_Y_ARCMIN;
        m_MinY = DEFAULT_MIN_Y_ARCMIN;
    }
    //g_AlignerData.RY = (long int)( m_MaxY - m_MinY );
    m_AxesInitiated = TRUE;
  }
}

int CSetupAxesDlg::GetMaxY( void )
{
	return( m_MaxY );
}


int CSetupAxesDlg::GetMinY( void )
{
	return( m_MinY );
}

void CSetupAxesDlg::SetMinMaxY( int minY, int maxY )
{
  if( minY > maxY )
  {
      int tmp = maxY;
      maxY = minY;
      minY = tmp;
  }

  if( GetMRad() == TRUE )
  {
      if( minY < MIN_MIN_Y_MRAD )
      {
        minY = MIN_MIN_Y_MRAD;
      }
      else if( minY > MAX_MIN_Y_MRAD )
      {
        minY = MAX_MIN_Y_MRAD;
      }

      if( maxY < MIN_MAX_Y_MRAD )
      {
        maxY = MIN_MAX_Y_MRAD;
      }
      else if( maxY > MAX_MAX_Y_MRAD )
      {
        maxY = MAX_MAX_Y_MRAD;
      }
  }
  else
  {
      if( minY < MIN_MIN_Y_ARCMIN )
      {
        minY = MIN_MIN_Y_ARCMIN;
      }
      else if( minY > MAX_MIN_Y_ARCMIN )
      {
        minY = MAX_MIN_Y_ARCMIN;
      }

      if( maxY < MIN_MAX_Y_ARCMIN )
      {
        maxY = MIN_MAX_Y_ARCMIN;
      }
      else if( maxY > MAX_MAX_Y_ARCMIN )
      {
        maxY = MAX_MAX_Y_ARCMIN;
      }
  }

  m_MaxY = maxY;
  m_MinY = minY;

  g_AlignerData.RY = (long int)( m_MaxY - m_MinY );
}
