// $Id: SetupDriftDlg.cpp,v 1.2 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "SetupDriftDlg.h"
#include "Util.h"
#include "GlobalData.h"
#include "GraphView.h"

// CSetupDriftDlg dialog

IMPLEMENT_DYNAMIC(CSetupDriftDlg, CDialog)
CSetupDriftDlg::CSetupDriftDlg( CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDriftDlg::IDD, pParent)
{
}

CSetupDriftDlg::~CSetupDriftDlg()
{
}

void CSetupDriftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TEST_CH_1, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO]);
  DDX_Check(pDX, IDC_TEST_CH_2, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 1]);
  DDX_Check(pDX, IDC_TEST_CH_3, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 2]);
	DDX_Check(pDX, IDC_TEST_CH_4, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 3]);
}

void CSetupDriftDlg::SetGyroData()
{
  g_AlignerData.NoOfCorr = 0;
  memset( g_AlignerData.ObjNo, 0, SIZE_OF_ARRAYS * sizeof( int ) );

	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			g_AlignerData.NoOfCorr++;
			g_AlignerData.ObjNo[g_AlignerData.NoOfCorr] = i;
		}
	}
}

BEGIN_MESSAGE_MAP(CSetupDriftDlg, CDialog)
	ON_BN_CLICKED(IDC_REF_CH_1, OnBnClickedCh1)
	ON_BN_CLICKED(IDC_REF_CH_2, OnBnClickedCh2)
	ON_BN_CLICKED(IDC_REF_CH_3, OnBnClickedCh3)
  ON_BN_CLICKED(IDC_REF_CH_4, OnBnClickedCh4)
  ON_BN_CLICKED(IDC_REF_CH_5, OnBnClickedCh5)
  ON_BN_CLICKED(IDC_REF_CH_6, OnBnClickedCh6)
  ON_BN_CLICKED(IDC_REF_CH_7, OnBnClickedCh7)
  ON_BN_CLICKED(IDC_REF_CH_8, OnBnClickedCh8)
  ON_BN_CLICKED(IDC_REF_CH_9, OnBnClickedCh9)
  ON_BN_CLICKED(IDC_REF_CH_10, OnBnClickedCh10)
	ON_BN_CLICKED(IDC_REF_CH_11, OnBnClickedCh11)
  ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDC_TEST_CH_1, OnBnClickedTestCh1)
  ON_BN_CLICKED(IDC_TEST_CH_2, OnBnClickedTestCh2)
  ON_BN_CLICKED(IDC_TEST_CH_3, OnBnClickedTestCh3)
	ON_BN_CLICKED(IDC_TEST_CH_4, OnBnClickedTestCh4)
END_MESSAGE_MAP()

// CSetupDriftDlg message handlers

BOOL CSetupDriftDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  int NoOfConnectedRefPlanes, NoOfConnectedGyros;
  CString tmpStr;

	m_SelectedReference = -1;
	memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  NoOfConnectedGyros = 0;

  //check syncros
	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
  {
    tmpStr = GetChannelName( i );
		AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, tmpStr, (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );

    //Show text for the connected gyros and 
    //the check box for selection of the gyros to be tested
    switch( i )
    {
    case 1:
			GetDlgItem( IDC_TEXT_CH_1 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_1 )->ShowWindow( SW_SHOW );
      break;
    case 2:
      GetDlgItem( IDC_TEXT_CH_2 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_2 )->ShowWindow( SW_SHOW );
      break;
    case 3:
      GetDlgItem( IDC_TEXT_CH_3 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_3 )->ShowWindow( SW_SHOW );
      break;
    case 4:
      GetDlgItem( IDC_TEXT_CH_4 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_4 )->ShowWindow( SW_SHOW );
      break;
    default:
      break;
    }

    //check which channels are connected
    if( IsConnected( tmpStr ) == TRUE )
    {
      NoOfConnectedGyros++;
      //Enable text for the connected gyros and 
      //the check box for selection of the gyros to be tested
      switch( i )
      {
      case 1:
        GetDlgItem( IDC_TEXT_CH_1 )->EnableWindow( TRUE );
        GetDlgItem( IDC_TEST_CH_1 )->EnableWindow( TRUE );
        break;
      case 2:
        GetDlgItem( IDC_TEXT_CH_2 )->EnableWindow( TRUE );
        GetDlgItem( IDC_TEST_CH_2 )->EnableWindow( TRUE );
        break;
      case 3:
        GetDlgItem( IDC_TEXT_CH_3 )->EnableWindow( TRUE );
        GetDlgItem( IDC_TEST_CH_3 )->EnableWindow( TRUE );
        break;
      case 4:
        GetDlgItem( IDC_TEXT_CH_4 )->EnableWindow( TRUE );
        GetDlgItem( IDC_TEST_CH_4 )->EnableWindow( TRUE );
        break;
      default:
        break;
      }
    }
  }

  if( NoOfConnectedGyros == 0 )
  {
    m_Text.LoadString( IDS_CONNECT_A_GYRO_REFERENCE_UNIT );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  NoOfConnectedRefPlanes = 0;

  //check reference planes
	for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    tmpStr = GetChannelName( i );
		AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, tmpStr, (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );

    //Show text for the connected channels and 
    //the radio buttons for selection of the reference channel
    switch( i )
    {
    case 1:
      GetDlgItem( IDC_TEXT_CH_1 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_1 )->ShowWindow( SW_SHOW );
      break;
    case 2:
      GetDlgItem( IDC_TEXT_CH_2 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_2 )->ShowWindow( SW_SHOW );
      break;
    case 3:
      GetDlgItem( IDC_TEXT_CH_3 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_3 )->ShowWindow( SW_SHOW );
      break;
    case 4:
      GetDlgItem( IDC_TEXT_CH_4 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_4 )->ShowWindow( SW_SHOW );
      break;
    case 5:
      GetDlgItem( IDC_TEXT_CH_5 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_5 )->ShowWindow( SW_SHOW );
      break;
    case 6:
      GetDlgItem( IDC_TEXT_CH_6 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_6 )->ShowWindow( SW_SHOW );
      break;
    case 7:
      GetDlgItem( IDC_TEXT_CH_7 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_7 )->ShowWindow( SW_SHOW );
      break;
    case 8:
      GetDlgItem( IDC_TEXT_CH_8 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_8 )->ShowWindow( SW_SHOW );
      break;
    case 9:
      GetDlgItem( IDC_TEXT_CH_9 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_9 )->ShowWindow( SW_SHOW );
      break;
    case 10:
      GetDlgItem( IDC_TEXT_CH_10 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_10 )->ShowWindow( SW_SHOW );
      break;
    case 11:
      GetDlgItem( IDC_TEXT_CH_11 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_REF_CH_11 )->ShowWindow( SW_SHOW );
      break;
    default:
      break;
    }

    //check which channels are connected
    if( IsConnected( tmpStr ) == TRUE )
    {
      NoOfConnectedRefPlanes++;
      //Enable text for the connected channels and 
      //the radio buttons for selection of the reference channel
      switch( i )
      {
      case 1:
        GetDlgItem( IDC_TEXT_CH_1 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_1 )->EnableWindow( TRUE );
        break;
      case 2:
        GetDlgItem( IDC_TEXT_CH_2 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_2 )->EnableWindow( TRUE );
        break;
      case 3:
        GetDlgItem( IDC_TEXT_CH_3 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_3 )->EnableWindow( TRUE );
        break;
      case 4:
        GetDlgItem( IDC_TEXT_CH_4 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_4 )->EnableWindow( TRUE );
        break;
      case 5:
        GetDlgItem( IDC_TEXT_CH_5 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_5 )->EnableWindow( TRUE );
        break;
      case 6:
        GetDlgItem( IDC_TEXT_CH_6 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_6 )->EnableWindow( TRUE );
        break;
      case 7:
        GetDlgItem( IDC_TEXT_CH_7 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_7 )->EnableWindow( TRUE );
        break;
      case 8:
        GetDlgItem( IDC_TEXT_CH_8 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_8 )->EnableWindow( TRUE );
        break;
      case 9:
        GetDlgItem( IDC_TEXT_CH_9 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_9 )->EnableWindow( TRUE );
        break;
      case 10:
        GetDlgItem( IDC_TEXT_CH_10 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_10 )->EnableWindow( TRUE );
        break;
      case 11:
        GetDlgItem( IDC_TEXT_CH_11 )->EnableWindow( TRUE );
        GetDlgItem( IDC_REF_CH_11 )->EnableWindow( TRUE );
        break;
      default:
        break;
      }
    }//if( connected )  
  }//for( all sensors )

  if( NoOfConnectedRefPlanes == 0 )
  {
    m_Text.LoadString( IDS_SPECIFY_AT_LEAST_ONE_STATION_BESIDES_A_GYRO );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  //show text for all channels
  SetDlgItemText( IDC_TEXT_CH_1, m_ChText[1] );
  SetDlgItemText( IDC_TEXT_CH_2, m_ChText[2] );
  SetDlgItemText( IDC_TEXT_CH_3, m_ChText[3] );
  SetDlgItemText( IDC_TEXT_CH_4, m_ChText[4] );
  SetDlgItemText( IDC_TEXT_CH_5, m_ChText[5] );
  SetDlgItemText( IDC_TEXT_CH_6, m_ChText[6] );
  SetDlgItemText( IDC_TEXT_CH_7, m_ChText[7] );
  SetDlgItemText( IDC_TEXT_CH_8, m_ChText[8] );
  SetDlgItemText( IDC_TEXT_CH_9, m_ChText[9] );
  SetDlgItemText( IDC_TEXT_CH_10, m_ChText[10] );
	SetDlgItemText( IDC_TEXT_CH_11, m_ChText[11] );

  SetDlgItemText( IDC_TEXT_1, g_AlignerData.AlignMode );

  m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION_AND_GYROS_TO_BE_TESTED );
  SetDlgItemText( IDC_TEXT_5, m_Text );

  //restore the gyros to be tested
	memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );

  for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
		if( IsConnected( g_AlignerData.ObjNo[i] ) == TRUE )
    {
			m_TestCh[g_AlignerData.ObjNo[i]] = TRUE;
    }
  }
  SetGyroData();

  //restore selected reference channel
	if( IsSensor( g_AlignerData.RefObjNo  ) == TRUE )
  {
    if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
    {
      m_SelectedReference = g_AlignerData.RefObjNo;
    }
    else
    {
      m_SelectedReference = -1;
    }
  }
  else
  {
    m_SelectedReference = -1;
  }
  
  switch( m_SelectedReference )
  {
  case 1:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_1 );
    break;
  case 2:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_2 );
    break;
  case 3:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_3 );
    break;
  case 4:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_4 );
    break;
  case 5:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_5 );
    break;
  case 6:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_6 );
    break;
  case 7:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_7 );
    break;
  case 8:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_8 );
    break;
  case 9:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_9 );
    break;
  case 10:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_10 );
    break;
  case 11:
    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, IDC_REF_CH_11 );
    break;
  default: //no reference selected
    m_SelectedReference = -1;
    break;
  }

  UpdateData( FALSE );

  return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupDriftDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CSetupDriftDlg::OnBnClickedOk()
{
  if( ( IsSensor( m_SelectedReference  ) == FALSE ) || 
			( IsConnected( m_SelectedReference ) == FALSE ) )
  {
    m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION_AND_GYROS_TO_BE_TESTED );
    SetDlgItemText( IDC_TEXT_5, m_Text );
    m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

	BOOL gyroSelected = FALSE;
	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			gyroSelected = TRUE;
		}
	}

  if( gyroSelected == FALSE )
  {
    m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION_AND_GYROS_TO_BE_TESTED );
    SetDlgItemText( IDC_TEXT_5, m_Text );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_SPECIFY_AT_LEAST_ONE_GYRO_TO_BE_TESTED );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

  g_AlignerData.RefObjNo = m_SelectedReference;
  SetGyroData();

  OnOK();
}

void CSetupDriftDlg::OnBnClickedTestCh1()
{
	if( m_TestCh[FIRST_ARRAY_INDEX_SYNCRO] == TRUE )
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO] = FALSE;
  }
  else
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO] = TRUE;
  }
}

void CSetupDriftDlg::OnBnClickedTestCh2()
{
  if( m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 1] == TRUE )
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 1] = FALSE;
  }
  else
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 1] = TRUE;
  }
}

void CSetupDriftDlg::OnBnClickedTestCh3()
{
  if( m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 2] == TRUE )
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 2] = FALSE;
  }
  else
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 2] = TRUE;
  }
}

void CSetupDriftDlg::OnBnClickedTestCh4()
{
  if( m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 3] == TRUE )
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 3] = FALSE;
  }
  else
  {
    m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 3] = TRUE;
  }
}

void CSetupDriftDlg::OnBnClickedCh1()
{
  m_SelectedReference = 1;
}

void CSetupDriftDlg::OnBnClickedCh2()
{
  m_SelectedReference = 2;
}

void CSetupDriftDlg::OnBnClickedCh3()
{
  m_SelectedReference = 3;
}

void CSetupDriftDlg::OnBnClickedCh4()
{
  m_SelectedReference = 4;
}

void CSetupDriftDlg::OnBnClickedCh5()
{
  m_SelectedReference = 5;
}

void CSetupDriftDlg::OnBnClickedCh6()
{
  m_SelectedReference = 6;
}

void CSetupDriftDlg::OnBnClickedCh7()
{
  m_SelectedReference = 7;
}

void CSetupDriftDlg::OnBnClickedCh8()
{
  m_SelectedReference = 8;
}

void CSetupDriftDlg::OnBnClickedCh9()
{
  m_SelectedReference = 9;
}

void CSetupDriftDlg::OnBnClickedCh10()
{
  m_SelectedReference = 10;
}

void CSetupDriftDlg::OnBnClickedCh11()
{
  m_SelectedReference = 11;
}
