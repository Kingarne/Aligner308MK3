// $Id: SetupTestPlaneDlg.cpp,v 1.3 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002
 
#include "StdAfx.h"
#include "SetupTestPlaneDlg.h"
#include "Util.h"
#include "GlobalData.h"
#include "GraphView.h"

// CSetupTestPlaneDlg dialog

IMPLEMENT_DYNAMIC(CSetupTestPlaneDlg, CDialog)
CSetupTestPlaneDlg::CSetupTestPlaneDlg( BOOL EnableZPar /*= FALSE*/, BOOL EnableReference /*= TRUE*/, BOOL EnableFixPlane /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupTestPlaneDlg::IDD, pParent)
{
  m_EnableZPar = EnableZPar;
  m_EnableReference = EnableReference;
  m_EnableFixPlane = EnableFixPlane;
  m_BenchMarkTest = EnableFixPlane; //QND solution
  m_SelectedReference = -1;
}

CSetupTestPlaneDlg::~CSetupTestPlaneDlg()
{
}

void CSetupTestPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//   DDX_Text(pDX, IDC_ZPAR_CH_1, m_ZParCh[1]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[1], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_2, m_ZParCh[2]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[2], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_3, m_ZParCh[3]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[3], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_4, m_ZParCh[4]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[4], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_5, m_ZParCh[5]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[5], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_6, m_ZParCh[6]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[6], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_7, m_ZParCh[7]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[7], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_8, m_ZParCh[8]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[8], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_9, m_ZParCh[9]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[9], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_10, m_ZParCh[10]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[10], MIN_ZPAR, MAX_ZPAR);
//   DDX_Text(pDX, IDC_ZPAR_CH_11, m_ZParCh[11]);
//   DDV_MinMaxDouble(pDX, m_ZParCh[11], MIN_ZPAR, MAX_ZPAR);
  DDX_Check(pDX, IDC_TEST_CH_1, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO]);
  DDX_Check(pDX, IDC_TEST_CH_2, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 1]);
  DDX_Check(pDX, IDC_TEST_CH_3, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 2]);
  DDX_Check(pDX, IDC_TEST_CH_4, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 3]);
  DDX_Check(pDX, IDC_TEST_CH_5, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 4]);
  DDX_Check(pDX, IDC_TEST_CH_6, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 5]);
  DDX_Check(pDX, IDC_TEST_CH_7, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 6]);
  DDX_Check(pDX, IDC_TEST_CH_8, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 7]);
  DDX_Check(pDX, IDC_TEST_CH_9, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 8]);
  DDX_Check(pDX, IDC_TEST_CH_10, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 9]);
  DDX_Check(pDX, IDC_TEST_CH_10, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 10]);
}

void CSetupTestPlaneDlg::SetObjectData()
{
  m_NoOfCorr = 0;
  memset( m_ObjNo, 0, SIZE_OF_ARRAYS * sizeof( int ) );

	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			m_NoOfCorr++;
			m_ObjNo[m_NoOfCorr] = i;
		}
	}
}

BOOL CSetupTestPlaneDlg::CheckNoOfConnectedObjects()
{
  int noOfConnectedObjects = 0;
	int noOfConnectedNotFixedObjects = 0;

  if( m_EnableFixPlane == TRUE )
  {
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      if( IsConnected( i ) == TRUE )
      {
        noOfConnectedObjects++;

				if( IsFixed( i ) == FALSE )
				{
					noOfConnectedNotFixedObjects++;
				}
      }
    }

    if( noOfConnectedObjects < 2 )
    {
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      m_Text.LoadString( IDS_CONNECT_TWO_STATIONS );
			CString str;
			str.LoadString( IDS_BENCHMARK_INFO );
			m_Text += _T("\n\n") + str;
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      return( FALSE );
    }

    if( noOfConnectedNotFixedObjects < 1 )
    {
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      m_Text.LoadString( IDS_SETUP_AT_LEAST_ONE_P_G_STATION );
			CString str;
			str.LoadString( IDS_BENCHMARK_INFO );
			m_Text += _T("\n\n") + str;
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      return( FALSE );
    }
  }
  else
  {
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      if( ( IsConnected( i ) == TRUE ) && ( IsFixed( i ) == FALSE ) )
      {
        noOfConnectedObjects++;
      }
    }

		if( m_EnableReference == TRUE )
		{
			if( noOfConnectedObjects < 2 )
			{
				m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
				m_Text.LoadString( IDS_CONNECT_TWO_PLATFORMS_FOR_RELATIVE_MEASUREMENT );
				MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
				return( FALSE );
			}
		}
		else
		{
			if( noOfConnectedObjects < 1 )
			{
				m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
				m_Text.LoadString( IDS_CONNECT_ONE_PLATFORM_FOR_ABSOLUTE_MEASUREMENT );
				MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
				return( FALSE );
			}
		}
  }
  return( TRUE );
}

void CSetupTestPlaneDlg::ShowAllZPar( BOOL Show )
{
	if( Show == TRUE )
	{
		GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_ZPAR_TEXT )->ShowWindow( SW_SHOW );

    if( m_BenchMarkTest == TRUE )
    {
      for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
      {
        switch( i )
        {
        case 1:
          GetDlgItem( IDC_ZPAR_CH_1 )->ShowWindow( SW_SHOW );
          break;
        case 2:
          GetDlgItem( IDC_ZPAR_CH_2 )->ShowWindow( SW_SHOW );
          break;
        case 3:
          GetDlgItem( IDC_ZPAR_CH_3 )->ShowWindow( SW_SHOW );
          break;
        case 4:
          GetDlgItem( IDC_ZPAR_CH_4 )->ShowWindow( SW_SHOW );
          break;
        } 
      }
    }

    for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      switch( i )
      {
      case 1:
        GetDlgItem( IDC_ZPAR_CH_1 )->ShowWindow( SW_SHOW );
        break;
      case 2:
        GetDlgItem( IDC_ZPAR_CH_2 )->ShowWindow( SW_SHOW );
        break;
      case 3:
        GetDlgItem( IDC_ZPAR_CH_3 )->ShowWindow( SW_SHOW );
        break;
      case 4:
        GetDlgItem( IDC_ZPAR_CH_4 )->ShowWindow( SW_SHOW );
        break;
      case 5:
        GetDlgItem( IDC_ZPAR_CH_5 )->ShowWindow( SW_SHOW );
        break;
      case 6:
        GetDlgItem( IDC_ZPAR_CH_6 )->ShowWindow( SW_SHOW );
        break;
      case 7:
        GetDlgItem( IDC_ZPAR_CH_7 )->ShowWindow( SW_SHOW );
        break;
      case 8:
        GetDlgItem( IDC_ZPAR_CH_8 )->ShowWindow( SW_SHOW );
        break;
      case 9:
        GetDlgItem( IDC_ZPAR_CH_9 )->ShowWindow( SW_SHOW );
        break;
      case 10:
        GetDlgItem( IDC_ZPAR_CH_10 )->ShowWindow( SW_SHOW );
        break;
      case 11:
        GetDlgItem( IDC_ZPAR_CH_11 )->ShowWindow( SW_SHOW );
        break;
      }
    }
	}
}

void CSetupTestPlaneDlg::DisableAllZPar( BOOL Disable )
{
	if( Disable == TRUE )
	{
		GetDlgItem( IDC_ZPAR_CH_1 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ZPAR_CH_2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ZPAR_CH_3 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ZPAR_CH_4 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_5 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_6 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_7 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_8 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_9 )->EnableWindow( FALSE );
		GetDlgItem( IDC_ZPAR_CH_10 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ZPAR_CH_11 )->EnableWindow( FALSE );
	}
}

void CSetupTestPlaneDlg::EnableDisableZPar( BOOL DoIt, BOOL enable, int iD )
{
	if( DoIt == TRUE )
	{
		if( (iD == IDC_ZPAR_CH_1) || (iD == IDC_ZPAR_CH_2) || (iD == IDC_ZPAR_CH_3) ||
        (iD == IDC_ZPAR_CH_4) || (iD == IDC_ZPAR_CH_5) || (iD == IDC_ZPAR_CH_6) ||
				(iD == IDC_ZPAR_CH_7) || (iD == IDC_ZPAR_CH_8) || (iD == IDC_ZPAR_CH_9) || 
        (iD == IDC_ZPAR_CH_10) || (iD == IDC_ZPAR_CH_11) )
		{
			GetDlgItem( iD )->EnableWindow( enable );
		}
	}
}

void CSetupTestPlaneDlg::HideAllReferenceItems()
{
	GetDlgItem( IDC_REF_TEXT )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_1 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_2 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_3 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_4 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_5 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_6 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_7 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_8 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_9 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_REF_CH_10 )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_REF_CH_11 )->ShowWindow( SW_HIDE );
}

BEGIN_MESSAGE_MAP(CSetupTestPlaneDlg, CDialog)
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
  ON_BN_CLICKED(IDC_TEST_CH_5, OnBnClickedTestCh5)
  ON_BN_CLICKED(IDC_TEST_CH_6, OnBnClickedTestCh6)
  ON_BN_CLICKED(IDC_TEST_CH_7, OnBnClickedTestCh7)
  ON_BN_CLICKED(IDC_TEST_CH_8, OnBnClickedTestCh8)
  ON_BN_CLICKED(IDC_TEST_CH_9, OnBnClickedTestCh9)
  ON_BN_CLICKED(IDC_TEST_CH_10, OnBnClickedTestCh10)
  ON_BN_CLICKED(IDC_TEST_CH_11, OnBnClickedTestCh11)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_1, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_2, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_3, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_4, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_5, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_6, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_7, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_8, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_9, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_10, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ZPAR_CH_11, OnEnKillfocus)
END_MESSAGE_MAP()

// CSetupTestPlaneDlg message handlers

BOOL CSetupTestPlaneDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  //memset( m_ZParCh, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  
  if( CheckNoOfConnectedObjects() == FALSE )
  {
    EndDialog( IDCANCEL );
    return FALSE;
  }

  if( m_BenchMarkTest == TRUE )
  { 
    m_Text.LoadString( IDS_BENCHMARK_INFO );
    SetDlgItemText( IDC_TEXT_2, m_Text );
		GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );

		//BenchMarkTest, connected syncro can be reference and can be tested
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
    {
      switch( i )
      {
      case 1:
        GetDlgItem( IDC_REF_CH_1 )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_TEST_CH_1 )->ShowWindow( SW_SHOW );
        break;
      case 2:
        GetDlgItem( IDC_REF_CH_2 )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_TEST_CH_2 )->ShowWindow( SW_SHOW );
        break;
      case 3:
        GetDlgItem( IDC_REF_CH_3 )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_TEST_CH_3 )->ShowWindow( SW_SHOW );
        break;
      case 4:
        GetDlgItem( IDC_REF_CH_4 )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_TEST_CH_4 )->ShowWindow( SW_SHOW );
        break;
      } 
    }
  }

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    switch( i )
    {
    case 1:
      GetDlgItem( IDC_REF_CH_1 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_1 )->ShowWindow( SW_SHOW );
      break;
    case 2:
      GetDlgItem( IDC_REF_CH_2 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_2 )->ShowWindow( SW_SHOW );
      break;
    case 3:
      GetDlgItem( IDC_REF_CH_3 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_3 )->ShowWindow( SW_SHOW );
      break;
    case 4:
      GetDlgItem( IDC_REF_CH_4 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_4 )->ShowWindow( SW_SHOW );
      break;
    case 5:
      GetDlgItem( IDC_REF_CH_5 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_5 )->ShowWindow( SW_SHOW );
      break;
    case 6:
      GetDlgItem( IDC_REF_CH_6 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_6 )->ShowWindow( SW_SHOW );
      break;
    case 7:
      GetDlgItem( IDC_REF_CH_7 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_7 )->ShowWindow( SW_SHOW );
      break;
    case 8:
      GetDlgItem( IDC_REF_CH_8 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_8 )->ShowWindow( SW_SHOW );
      break;
    case 9:
      GetDlgItem( IDC_REF_CH_9 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_9 )->ShowWindow( SW_SHOW );
      break;
    case 10:
      GetDlgItem( IDC_REF_CH_10 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_10 )->ShowWindow( SW_SHOW );
      break;
    case 11:
      GetDlgItem( IDC_REF_CH_11 )->ShowWindow( SW_SHOW );
      GetDlgItem( IDC_TEST_CH_11 )->ShowWindow( SW_SHOW );
      break;
    } 
  }//for( all sensors )

  SetDlgItemText( IDC_TEXT_1, g_AlignerData.AlignMode );

	if( m_EnableReference == FALSE )
	{
		HideAllReferenceItems();
	}

  for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, GetChannelName( i ), (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );	

    //Show text for all channels
    switch( i )
    {
    case 1:
      GetDlgItem( IDC_TEXT_CH_1 )->ShowWindow( SW_SHOW );
      break;
    case 2:
      GetDlgItem( IDC_TEXT_CH_2 )->ShowWindow( SW_SHOW );
      break;
    case 3:
      GetDlgItem( IDC_TEXT_CH_3 )->ShowWindow( SW_SHOW );
      break;
    case 4:
      GetDlgItem( IDC_TEXT_CH_4 )->ShowWindow( SW_SHOW );
      break;
    case 5:
      GetDlgItem( IDC_TEXT_CH_5 )->ShowWindow( SW_SHOW );
      break;
    case 6:
      GetDlgItem( IDC_TEXT_CH_6 )->ShowWindow( SW_SHOW );
      break;
    case 7:
      GetDlgItem( IDC_TEXT_CH_7 )->ShowWindow( SW_SHOW );
      break;
    case 8:
      GetDlgItem( IDC_TEXT_CH_8 )->ShowWindow( SW_SHOW );
      break;
    case 9:
      GetDlgItem( IDC_TEXT_CH_9 )->ShowWindow( SW_SHOW );
      break;
    case 10:
      GetDlgItem( IDC_TEXT_CH_10 )->ShowWindow( SW_SHOW );
      break;
    case 11:
      GetDlgItem( IDC_TEXT_CH_11 )->ShowWindow( SW_SHOW );
      break;
    default:
      break;
    }

    //check which channels are connected
    if( IsConnected( i ) == TRUE )
    {
      //Enable text for the connected channels
      switch( i )
      {
      case 1:
        GetDlgItem( IDC_TEXT_CH_1 )->EnableWindow( TRUE );
        break;
      case 2:
        GetDlgItem( IDC_TEXT_CH_2 )->EnableWindow( TRUE );
        break;
      case 3:
        GetDlgItem( IDC_TEXT_CH_3 )->EnableWindow( TRUE );
        break;
      case 4:
        GetDlgItem( IDC_TEXT_CH_4 )->EnableWindow( TRUE );
        break;
      case 5:
        GetDlgItem( IDC_TEXT_CH_5 )->EnableWindow( TRUE );
        break;
      case 6:
        GetDlgItem( IDC_TEXT_CH_6 )->EnableWindow( TRUE );
        break;
      case 7:
        GetDlgItem( IDC_TEXT_CH_7 )->EnableWindow( TRUE );
        break;
      case 8:
        GetDlgItem( IDC_TEXT_CH_8 )->EnableWindow( TRUE );
        break;
      case 9:
        GetDlgItem( IDC_TEXT_CH_9 )->EnableWindow( TRUE );
        break;
      case 10:
        GetDlgItem( IDC_TEXT_CH_10 )->EnableWindow( TRUE );
        break;
      case 11:
        GetDlgItem( IDC_TEXT_CH_11 )->EnableWindow( TRUE );
        break;
      default:
        break;
      }

			if( m_EnableReference == TRUE )
			{
				if( ( m_EnableFixPlane == TRUE ) || ( IsFixed( i ) == FALSE ) )
        {
				  switch( i )
				  {
				  case 1:
					  GetDlgItem( IDC_REF_CH_1 )->EnableWindow( TRUE );
					  break;
				  case 2:
					  GetDlgItem( IDC_REF_CH_2 )->EnableWindow( TRUE );
					  break;
				  case 3:
					  GetDlgItem( IDC_REF_CH_3 )->EnableWindow( TRUE );
					  break;
				  case 4:
					  GetDlgItem( IDC_REF_CH_4 )->EnableWindow( TRUE );
					  break;
				  case 5:
					  GetDlgItem( IDC_REF_CH_5 )->EnableWindow( TRUE );
					  break;
				  case 6:
					  GetDlgItem( IDC_REF_CH_6 )->EnableWindow( TRUE );
					  break;
				  case 7:
					  GetDlgItem( IDC_REF_CH_7 )->EnableWindow( TRUE );
					  break;
				  case 8:
					  GetDlgItem( IDC_REF_CH_8 )->EnableWindow( TRUE );
					  break;
				  case 9:
					  GetDlgItem( IDC_REF_CH_9 )->EnableWindow( TRUE );
					  break;
				  case 10:
					  GetDlgItem( IDC_REF_CH_10 )->EnableWindow( TRUE );
					  break;
				  case 11:
					  GetDlgItem( IDC_REF_CH_11 )->EnableWindow( TRUE );
					  break;
				  default:
					  break;
				  }
        }
			}
    }//if( connected )  
  }//for( all channels )

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
  
	if( m_EnableReference == TRUE )
	{
		m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
		SetDlgItemText( IDC_TEXT_5, m_Text );
	}
	else
	{
		m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST );
		SetDlgItemText( IDC_TEXT_5, m_Text );
	}

  //restore the channels to be tested
  memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );

  for( i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    if( (IsConnected( g_AlignerData.ObjNo[i] ) == TRUE) &&
        (IsFixed( g_AlignerData.ObjNo[i] ) == FALSE) )
    {
      m_TestCh[g_AlignerData.ObjNo[i]] = TRUE;
    }
  }

//   if( m_EnableZPar == TRUE )
//   {
//     //restore length and angle
//     for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//     {
//       m_ZParCh[i] = GetZPar( i );
//     }
//   }

  //restore selected reference channel
	if( m_EnableReference == FALSE )
	{
		m_SelectedReference = -1;
	}
	else
	{
		if( m_EnableFixPlane == TRUE )
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
			if( ( IsSensor( g_AlignerData.RefObjNo  ) == TRUE ) &&
					( IsFixed( g_AlignerData.RefObjNo ) == FALSE ) )
			{
				m_SelectedReference = g_AlignerData.RefObjNo;
			}
			else
			{
				m_SelectedReference = -1;
			}
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
				break;
		}
	}

  UpdateData( FALSE );

  ReferenceSelected();

  return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupTestPlaneDlg::ReferenceSelected()
{
  UpdateData( TRUE );

	ShowAllZPar( FALSE);//m_EnableZPar );

  //disable the check boxes for selection of the channel to be tested
  GetDlgItem( IDC_TEST_CH_1 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_2 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_3 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_4 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_5 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_6 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_7 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_8 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_9 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_10 )->EnableWindow( FALSE );
  GetDlgItem( IDC_TEST_CH_11 )->EnableWindow( FALSE );
	DisableAllZPar( m_EnableZPar );

  if( ( m_EnableReference == FALSE ) || ( m_SelectedReference != -1 ) )
  {
    // check which channels can be selected to be tested
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      if( ( IsConnected( i ) == TRUE ) &&  
          ( IsFixed( i ) == FALSE ) && 
          ( i != m_SelectedReference ) )
      {
        //enable the check box for selection of the channel to be tested
        switch( i )
        {
        case 1:
          GetDlgItem( IDC_TEST_CH_1 )->EnableWindow( TRUE );
          break;
        case 2:
          GetDlgItem( IDC_TEST_CH_2 )->EnableWindow( TRUE );
          break;
        case 3:
          GetDlgItem( IDC_TEST_CH_3 )->EnableWindow( TRUE );
          break;
        case 4:
          GetDlgItem( IDC_TEST_CH_4 )->EnableWindow( TRUE );
          break;
        case 5:
          GetDlgItem( IDC_TEST_CH_5 )->EnableWindow( TRUE );
          break;
        case 6:
          GetDlgItem( IDC_TEST_CH_6 )->EnableWindow( TRUE );
          break;
        case 7:
          GetDlgItem( IDC_TEST_CH_7 )->EnableWindow( TRUE );
          break;
        case 8:
          GetDlgItem( IDC_TEST_CH_8 )->EnableWindow( TRUE );
          break;
        case 9:
          GetDlgItem( IDC_TEST_CH_9 )->EnableWindow( TRUE );
          break;
        case 10:
          GetDlgItem( IDC_TEST_CH_10 )->EnableWindow( TRUE );
          break;
        case 11:
          GetDlgItem( IDC_TEST_CH_11 )->EnableWindow( TRUE );
          break;
        default:
          break;
        }
      }
    }//for( all sensors )
  }

  TestEnabled();

  UpdateData( FALSE );
}

void CSetupTestPlaneDlg::TestEnabled()
{
  UpdateData( TRUE );
// 
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[1], IDC_ZPAR_CH_1 );
//   EnableDisableZPar( m_EnableZPar, m_TestCh[2], IDC_ZPAR_CH_2 );
//   EnableDisableZPar( m_EnableZPar, m_TestCh[3], IDC_ZPAR_CH_3 );
//   EnableDisableZPar( m_EnableZPar, m_TestCh[4], IDC_ZPAR_CH_4 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[5], IDC_ZPAR_CH_5 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[6], IDC_ZPAR_CH_6 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[7], IDC_ZPAR_CH_7 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[8], IDC_ZPAR_CH_8 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[9], IDC_ZPAR_CH_9 );
// 	EnableDisableZPar( m_EnableZPar, m_TestCh[10], IDC_ZPAR_CH_10 );
//   EnableDisableZPar( m_EnableZPar, m_TestCh[11], IDC_ZPAR_CH_11 );
// 
//   //the selected reference channel cannot be selected to be tested
//   switch( m_SelectedReference )
//   {
//   case 1:
//     m_TestCh[1] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_1 );
//     break;
//   case 2:
//     m_TestCh[2] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_2 );
//     break;
//   case 3:
//     m_TestCh[3] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_3 );
//     break;
//   case 4:
//     m_TestCh[4] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_4 );
//     break;
//   case 5:
//     m_TestCh[5] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_5 );
//     break;
//   case 6:
//     m_TestCh[6] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_6 );
//     break;
//   case 7:
//     m_TestCh[7] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_7 );
//     break;
//   case 8:
//     m_TestCh[8] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_8 );
//     break;
//   case 9:
//     m_TestCh[9] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_9 );
//     break;
//   case 10:
//     m_TestCh[10] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_10 );
//     break;
//   case 11:
//     m_TestCh[11] = FALSE;
//     EnableDisableZPar( m_EnableZPar, TRUE, IDC_ZPAR_CH_11 );
//     break;
//   default:
//     break;
//   }

  if( IsValidChannel( m_SelectedReference ) == TRUE )
  {
    //ask for channels to be tested
    m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST ); 
    SetDlgItemText( IDC_TEXT_5, m_Text );
  }

  UpdateData( FALSE );
}

void CSetupTestPlaneDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CSetupTestPlaneDlg::OnBnClickedOk()
{
  //validate m_SelectedReference

  if( ( m_EnableReference == TRUE ) && 
      ( IsValidChannel( m_SelectedReference ) == FALSE ) )
  {
    m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
    SetDlgItemText( IDC_TEXT_5, m_Text );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

	BOOL testChannelSelected = FALSE;
	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			testChannelSelected = TRUE;
		}
	}

  if( testChannelSelected == FALSE )
  {
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_SETUP_MEASURE_SETUP_ONE_TO_TEST );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

  SetObjectData();

  //store the channels to be tested
  g_AlignerData.RefObjNo = m_SelectedReference;
  g_AlignerData.NoOfCorr = m_NoOfCorr;
  memcpy( g_AlignerData.ObjNo, m_ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
  
//   if( m_EnableZPar == TRUE )
//   {
//     //store Z-par
//     SetZPar( 1, m_ZParCh[1] );
//     SetZPar( 2, m_ZParCh[2] );
//     SetZPar( 3, m_ZParCh[3] );
//     SetZPar( 4, m_ZParCh[4] );
//     SetZPar( 5, m_ZParCh[5] );
//     SetZPar( 6, m_ZParCh[6] );
//     SetZPar( 7, m_ZParCh[7] );
//     SetZPar( 8, m_ZParCh[8] );
//     SetZPar( 9, m_ZParCh[9] );
//     SetZPar( 10, m_ZParCh[10] );
//     SetZPar( 11, m_ZParCh[11] );
//   }

  OnOK();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh1()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[1], IDC_ZPAR_CH_1 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh2()
{
	EnableDisableZPar( m_EnableZPar, !m_TestCh[2], IDC_ZPAR_CH_2 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh3()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[3], IDC_ZPAR_CH_3 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh4()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[4], IDC_ZPAR_CH_4 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh5()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[5], IDC_ZPAR_CH_5 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh6()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[6], IDC_ZPAR_CH_6 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh7()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[7], IDC_ZPAR_CH_7 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh8()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[8], IDC_ZPAR_CH_8 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh9()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[9], IDC_ZPAR_CH_9 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh10()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[10], IDC_ZPAR_CH_10 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedTestCh11()
{
	//EnableDisableZPar( m_EnableZPar, !m_TestCh[11], IDC_ZPAR_CH_11 );
    TestEnabled();
}

void CSetupTestPlaneDlg::OnBnClickedCh1()
{
  m_SelectedReference = 1;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh2()
{
  m_SelectedReference = 2;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh3()
{
  m_SelectedReference = 3;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh4()
{
  m_SelectedReference = 4;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh5()
{
  m_SelectedReference = 5;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh6()
{
  m_SelectedReference = 6;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh7()
{
  m_SelectedReference = 7;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh8()
{
  m_SelectedReference = 8;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh9()
{
  m_SelectedReference = 9;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh10()
{
  m_SelectedReference = 10;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnBnClickedCh11()
{
  m_SelectedReference = 11;
  ReferenceSelected();
}

void CSetupTestPlaneDlg::OnEnKillfocus()
{
  UpdateData( TRUE );
}
