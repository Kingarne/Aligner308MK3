// SetupAxesDlgA202.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SetupAxesDlgA202.h"
#include "Util.h"
#include "GlobalData.h"

BOOL CSetupAxesDlgA202::m_AxesInitiated;
double CSetupAxesDlgA202::m_MaxY1Deg;
double CSetupAxesDlgA202::m_MinY1Deg;// (m_MaxY1Deg - m_MinY1Deg) copied to g_AlignerData.RY
double CSetupAxesDlgA202::m_MaxY2Deg;
double CSetupAxesDlgA202::m_MinY2Deg;
double CSetupAxesDlgA202::m_RangeX; //copied to g_AlignerData.RX

// CSetupAxesDlgA202 dialog

IMPLEMENT_DYNCREATE(CSetupAxesDlgA202, CDialog)

CSetupAxesDlgA202::CSetupAxesDlgA202(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupAxesDlgA202::IDD, pParent)
{
}

CSetupAxesDlgA202::~CSetupAxesDlgA202()
{
}

void CSetupAxesDlgA202::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_SETUP_AXES_X_RANGE, m_RangeXDlg);

  switch( m_XUnit )
  {
  case X_UNIT_HRS:
    DDV_MinMaxDouble(pDX, m_RangeXDlg, MIN_RANGE_X_HOURS, MAX_RANGE_X_HOURS);
    break;
  case X_UNIT_MIN:
    DDV_MinMaxDouble(pDX, m_RangeXDlg, MIN_RANGE_X_MIN, MAX_RANGE_X_MIN);
    break;
  case X_UNIT_S:
  default:
    DDV_MinMaxDouble(pDX, m_RangeXDlg, MIN_RANGE_X_S, MAX_RANGE_X_S);
    break;
  }    
  DDX_Text(pDX, IDC_SETUP_AXES_Y1_MIN, m_MinY1DegDlg);
  DDV_MinMaxDouble(pDX, m_MinY1DegDlg, MIN_MIN_Y_DEG, MAX_MIN_Y_DEG);
  DDX_Text(pDX, IDC_SETUP_AXES_Y1_MAX, m_MaxY1DegDlg);
  DDV_MinMaxDouble(pDX, m_MaxY1DegDlg, MIN_MAX_Y_DEG, MAX_MAX_Y_DEG);
  DDX_Text(pDX, IDC_SETUP_AXES_Y2_MIN, m_MinY2DegDlg);
  DDV_MinMaxDouble(pDX, m_MinY2DegDlg, MIN_MIN_Y_DEG, MAX_MIN_Y_DEG);
  DDX_Text(pDX, IDC_SETUP_AXES_Y2_MAX, m_MaxY2DegDlg);
  DDV_MinMaxDouble(pDX, m_MaxY2DegDlg, MIN_MAX_Y_DEG, MAX_MAX_Y_DEG);
}

BOOL CSetupAxesDlgA202::OnInitDialog()
{
	CDialog::OnInitDialog();
  InitiateLiveGraphAxes();
  m_MaxY1DegDlg = m_MaxY1Deg;
  m_MinY1DegDlg = m_MinY1Deg;
  m_MaxY2DegDlg = m_MaxY2Deg;
  m_MinY2DegDlg = m_MinY2Deg;
  m_RangeXDlg = m_RangeX;
  m_XUnit = X_UNIT_S;
  OnBnClickedSetupAxesXSUnit();

  UpdateData( FALSE );
  
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetupAxesDlgA202::GetSettingsForAxes( CString* pXAxisSettings, CString* pY1AxisSettings, CString* pY2AxisSettings )
{
    CString tmpStr;
    *pXAxisSettings = _T("");
	*pY1AxisSettings = _T("");
    *pY2AxisSettings = _T("");
    InitiateLiveGraphAxes();

    if( ( m_RangeX < MIN_RANGE_X_S ) || ( m_RangeX > MAX_RANGE_X_S ) )
    {
		    m_RangeX = DEFAULT_RANGE_X_S;
    }
    g_AlignerData.RX = (long int)m_RangeX;
    pXAxisSettings->Format( _T("%.1f "), m_RangeX );
    tmpStr.LoadString( IDS_S_UNIT );
    *pXAxisSettings += tmpStr;

	tmpStr.LoadString( IDS_DEG_UNIT );

    if( ( m_MinY1Deg < MIN_MIN_Y_DEG ) || ( m_MinY1Deg > MAX_MIN_Y_DEG ) )
    {
		    m_MinY1Deg = DEFAULT_MIN_Y_DEG;
    }

	if( ( m_MaxY1Deg < MIN_MAX_Y_DEG ) || ( m_MaxY1Deg > MAX_MAX_Y_DEG ) )
	{
		m_MaxY1Deg = DEFAULT_MAX_Y_DEG;
	}
	pY1AxisSettings->Format( _T("%+.1f to %+.1f "), m_MinY1Deg, m_MaxY1Deg );  
	*pY1AxisSettings += tmpStr;

    if( ( m_MinY2Deg < MIN_MIN_Y_DEG ) || ( m_MinY2Deg > MAX_MIN_Y_DEG ) )
    {
		    m_MinY2Deg = DEFAULT_MIN_Y_DEG;
    }

	if( ( m_MaxY2Deg < MIN_MAX_Y_DEG ) || ( m_MaxY2Deg > MAX_MAX_Y_DEG ) )
	{
		m_MaxY2Deg = DEFAULT_MAX_Y_DEG;
	}
	pY2AxisSettings->Format( _T("%+.1f to %+.1f "), m_MinY2Deg, m_MaxY2Deg );  
	*pY2AxisSettings += tmpStr;

    g_AlignerData.RY = (long int)( m_MaxY1Deg - m_MinY1Deg );
    return; 
}

BEGIN_MESSAGE_MAP(CSetupAxesDlgA202, CDialog)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_X_RANGE, OnEnKillfocusSetupAxesXRange)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_S_UNIT, OnBnClickedSetupAxesXSUnit)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_MIN_UNIT, OnBnClickedSetupAxesXMinUnit)
  ON_BN_CLICKED(IDC_SETUP_AXES_X_HRS_UNIT, OnBnClickedSetupAxesXHrsUnit)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y1_MAX, OnEnKillfocusSetupAxesY1Max)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y1_MIN, OnEnKillfocusSetupAxesY1Min)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y2_MAX, OnEnKillfocusSetupAxesY2Max)
  ON_EN_KILLFOCUS(IDC_SETUP_AXES_Y2_MIN, OnEnKillfocusSetupAxesY2Min)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CSetupAxesDlgA202 message handlers

void CSetupAxesDlgA202::OnBnClickedOk()
{
  CString msgCaption, text;
  msgCaption.LoadString( IDS_ERROR_CAPTION );

  if( m_MinY1DegDlg >= m_MaxY1DegDlg )
  {
    if( m_MaxY1DegDlg > MIN_MIN_Y_DEG )
    {
      text.LoadString( IDS_MIN_MUST_BE_SMALLER_THAN_MAX );
      GetDlgItem( IDC_SETUP_AXES_Y1_MIN )->SetFocus();
    }
    else
    {
      text.LoadString( IDS_MAX_MUST_BE_BIGGER_THAN_MIN );
      GetDlgItem( IDC_SETUP_AXES_Y1_MAX )->SetFocus();
    }
    MessageBox( text, msgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

  if( m_MinY2DegDlg >= m_MaxY2DegDlg )
  {
    if( m_MaxY2DegDlg > MIN_MIN_Y_DEG )
    {
      text.LoadString( IDS_MIN_MUST_BE_SMALLER_THAN_MAX );
      GetDlgItem( IDC_SETUP_AXES_Y2_MIN )->SetFocus();
    }
    else
    {
      text.LoadString( IDS_MAX_MUST_BE_BIGGER_THAN_MIN );
      GetDlgItem( IDC_SETUP_AXES_Y2_MAX )->SetFocus();
    }
    MessageBox( text, msgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

  m_MaxY1Deg = m_MaxY1DegDlg;
  m_MinY1Deg = m_MinY1DegDlg;
  m_MaxY2Deg = m_MaxY2DegDlg;
  m_MinY2Deg = m_MinY2DegDlg;

	if( m_MaxY1Deg < MIN_MAX_Y_DEG )
	{
		m_MaxY1Deg = MIN_MAX_Y_DEG;
	}
	else if( m_MaxY1Deg > MAX_MAX_Y_DEG )
	{
		m_MaxY1Deg = MAX_MAX_Y_DEG;
	}

	if( m_MinY1Deg < MIN_MIN_Y_DEG )
	{
		m_MinY1Deg = MIN_MIN_Y_DEG;
	}
	else if( m_MinY1Deg > MAX_MIN_Y_DEG )
	{
		m_MinY1Deg = MAX_MIN_Y_DEG;
	}

	if( m_MaxY2Deg < MIN_MAX_Y_DEG )
	{
		m_MaxY2Deg = MIN_MAX_Y_DEG;
	}
	else if( m_MaxY2Deg > MAX_MAX_Y_DEG )
	{
		m_MaxY2Deg = MAX_MAX_Y_DEG;
	}

	if( m_MinY2Deg < MIN_MIN_Y_DEG )
	{
		m_MinY2Deg = MIN_MIN_Y_DEG;
	}
	else if( m_MinY2Deg > MAX_MIN_Y_DEG )
	{
		m_MinY2Deg = MAX_MIN_Y_DEG;
	}

  g_AlignerData.RY = (long int)( m_MaxY1Deg - m_MinY1Deg );

  if( m_XUnit == X_UNIT_HRS )
  {
    m_RangeX = HOURS_TO_SECONDS( m_RangeXDlg );
  }
  else if( m_XUnit == X_UNIT_MIN )
  {
    m_RangeX = MINUTES_TO_SECONDS( m_RangeXDlg );
  }
  else
  {
    m_RangeX = m_RangeXDlg;
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
  
  OnOK();
}

void CSetupAxesDlgA202::OnEnKillfocusSetupAxesXRange()
{
  UpdateData( TRUE );
}

void CSetupAxesDlgA202::OnBnClickedSetupAxesXSUnit()
{
  if( m_XUnit == X_UNIT_HRS )
  {
    m_RangeXDlg = HOURS_TO_SECONDS( m_RangeXDlg );
  }
  else if( m_XUnit == X_UNIT_MIN )
  {
    m_RangeXDlg = MINUTES_TO_SECONDS( m_RangeXDlg );
  }

  if( m_RangeXDlg < MIN_RANGE_X_S )
  {
    m_RangeXDlg = MIN_RANGE_X_S;
  }
  else if( m_RangeXDlg > MAX_RANGE_X_S )
  {
    m_RangeXDlg = MAX_RANGE_X_S;
  }
  m_XUnit = X_UNIT_S;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_S_UNIT );
  UpdateData( FALSE );
}

void CSetupAxesDlgA202::OnBnClickedSetupAxesXMinUnit()
{
  if( m_XUnit == X_UNIT_HRS )
  {
    m_RangeXDlg = HOURS_TO_MINUTES( m_RangeXDlg );
  }
  else if( m_XUnit == X_UNIT_S )
  {
    m_RangeXDlg = SECONDS_TO_MINUTES( m_RangeXDlg );
  }

  if( m_RangeXDlg < MIN_RANGE_X_MIN )
  {
    m_RangeXDlg = MIN_RANGE_X_MIN;
  }
  else if( m_RangeXDlg > MAX_RANGE_X_MIN )
  {
    m_RangeXDlg = MAX_RANGE_X_MIN;
  }
  m_XUnit = X_UNIT_MIN;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_MIN_UNIT );
  UpdateData( FALSE );
}

void CSetupAxesDlgA202::OnBnClickedSetupAxesXHrsUnit()
{
  if( m_XUnit == X_UNIT_MIN )
  {
    m_RangeXDlg = MINUTES_TO_HOURS( m_RangeXDlg );
  }
  else if( m_XUnit == X_UNIT_S )
  {
    m_RangeXDlg = SECONDS_TO_HOURS( m_RangeXDlg );
  }

  if( m_RangeXDlg < MIN_RANGE_X_HOURS )
  {
    m_RangeXDlg = MIN_RANGE_X_HOURS;
  }
  else if( m_RangeXDlg > MAX_RANGE_X_HOURS )
  {
    m_RangeXDlg = MAX_RANGE_X_HOURS;
  }
  m_XUnit = X_UNIT_HRS;
  CheckRadioButton( IDC_SETUP_AXES_X_S_UNIT, IDC_SETUP_AXES_X_HRS_UNIT, IDC_SETUP_AXES_X_HRS_UNIT );
  UpdateData( FALSE );
}

void CSetupAxesDlgA202::OnEnKillfocusSetupAxesY1Max()
{
  UpdateData( TRUE );
}

void CSetupAxesDlgA202::OnEnKillfocusSetupAxesY1Min()
{
  UpdateData( TRUE );
}

void CSetupAxesDlgA202::OnEnKillfocusSetupAxesY2Max()
{
  UpdateData( TRUE );
}

void CSetupAxesDlgA202::OnEnKillfocusSetupAxesY2Min()
{
  UpdateData( TRUE );
}


double CSetupAxesDlgA202::GetMaxY1Deg( void )
{
	return( m_MaxY1Deg );
}


double CSetupAxesDlgA202::GetMinY1Deg( void )
{
	return( m_MinY1Deg );
}

double CSetupAxesDlgA202::GetMaxY2Deg( void )
{
	return( m_MaxY2Deg );
}

double CSetupAxesDlgA202::GetMinY2Deg( void )
{
	return( m_MinY2Deg );
}


double CSetupAxesDlgA202::GetRangeXS( void )
{
	return( m_RangeX );
}

void CSetupAxesDlgA202::InitiateLiveGraphAxes( void )
{
  if( m_AxesInitiated == FALSE )
  {
    m_MaxY1Deg = DEFAULT_MAX_Y_DEG;
    m_MinY1Deg = DEFAULT_MIN_Y_DEG;
    g_AlignerData.RY = (long int)( m_MaxY1Deg - m_MinY1Deg );
    m_MaxY2Deg = DEFAULT_MAX_Y_DEG;
    m_MinY2Deg = DEFAULT_MIN_Y_DEG;
    m_RangeX = DEFAULT_RANGE_X_S;
    g_AlignerData.RX = (long int)m_RangeX;
    m_AxesInitiated = TRUE;
  }
}