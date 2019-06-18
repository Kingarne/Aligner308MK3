// $Id: SetupSpecGBDlg.cpp,v 1.2 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SetupSpecGBDlg.h"
#include "Util.h"
#include "GlobalData.h"
#include "GraphView.h"

// CSetupSpecGBDlg dialog

IMPLEMENT_DYNAMIC(CSetupSpecGBDlg, CDialog)
CSetupSpecGBDlg::CSetupSpecGBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSpecGBDlg::IDD, pParent)
{
}

CSetupSpecGBDlg::~CSetupSpecGBDlg()
{
}

void CSetupSpecGBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
	DDX_Check(pDX, IDC_TEST_CH_11, m_TestCh[FIRST_ARRAY_INDEX_SYNCRO + 10]);
}

void CSetupSpecGBDlg::SetObjectData()
{
  m_NoOfCorr = 0;
  memset( m_ObjNo, 0, SIZE_OF_ARRAYS * sizeof( int ) );

	for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			m_NoOfCorr++;
			m_ObjNo[m_NoOfCorr] = i;
		}
	}
}

BEGIN_MESSAGE_MAP(CSetupSpecGBDlg, CDialog)
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
	ON_BN_CLICKED(IDC_TEST_CH_1, OnBnClickedTestCh)
	ON_BN_CLICKED(IDC_TEST_CH_2, OnBnClickedTestCh)
	ON_BN_CLICKED(IDC_TEST_CH_3, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_4, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_5, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_6, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_7, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_8, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_9, OnBnClickedTestCh)
  ON_BN_CLICKED(IDC_TEST_CH_10, OnBnClickedTestCh)
	ON_BN_CLICKED(IDC_TEST_CH_11, OnBnClickedTestCh)
END_MESSAGE_MAP()

// CSetupSpecGBDlg message handlers

BOOL CSetupSpecGBDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if( GetNoOfConnectedStations() < 2 )
  {
    m_Text.LoadString( IDS_AT_LEAST_TWO_STATIONS_MUST_BE_CONNECTED );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  SetDlgItemText( IDC_TEXT_1, g_AlignerData.AlignMode );

  //restore selected reference channel
	if( (IsConnected( g_AlignerData.RefObjNo ) == TRUE)  &&
			(IsGun( g_AlignerData.RefObjNo ) == FALSE) )
  {
    m_SelectedReference = g_AlignerData.RefObjNo;
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
      break;
  }

  //restore the channels to be tested
	memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  
  for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    if( (IsGun( g_AlignerData.ObjNo[i] ) == TRUE) &&
        (g_AlignerData.ObjNo[i] != m_SelectedReference) )
    {
			m_TestCh[g_AlignerData.ObjNo[i]] = TRUE;
    }
  }

	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, GetChannelName( i ), (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );

    //Show text for the connected channels and for
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

			//check which channels are not of type GUN
			if( IsGun( i ) == FALSE )
			{
				//Enable the radio buttons for selection of the reference channel
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
			}//if( GUN )
    }//if( connected )  
	}//for()

	for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    //Show the check box for selection of the channel to be tested
    switch( i )
    {
    case 1:
      GetDlgItem( IDC_TEST_CH_1 )->ShowWindow( SW_SHOW );
      break;
    case 2:
      GetDlgItem( IDC_TEST_CH_2 )->ShowWindow( SW_SHOW );
      break;
    case 3:
      GetDlgItem( IDC_TEST_CH_3 )->ShowWindow( SW_SHOW );
      break;
    case 4:
      GetDlgItem( IDC_TEST_CH_4 )->ShowWindow( SW_SHOW );
      break;
    case 5:
      GetDlgItem( IDC_TEST_CH_5 )->ShowWindow( SW_SHOW );
      break;
    case 6:
      GetDlgItem( IDC_TEST_CH_6 )->ShowWindow( SW_SHOW );
      break;
    case 7:
      GetDlgItem( IDC_TEST_CH_7 )->ShowWindow( SW_SHOW );
      break;
    case 8:
      GetDlgItem( IDC_TEST_CH_8 )->ShowWindow( SW_SHOW );
      break;
    case 9:
      GetDlgItem( IDC_TEST_CH_9 )->ShowWindow( SW_SHOW );
      break;
    case 10:
      GetDlgItem( IDC_TEST_CH_10 )->ShowWindow( SW_SHOW );
      break;
    case 11:
      GetDlgItem( IDC_TEST_CH_11 )->ShowWindow( SW_SHOW );
      break;
    default:
      break;
    }
  }//for( all sensors )

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
  
  m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
  SetDlgItemText( IDC_TEXT_5, m_Text );

  UpdateData( FALSE );

  ReferenceSelected();

  return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupSpecGBDlg::ReferenceSelected()
{
  UpdateData( TRUE );

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

  if( IsValidChannel( m_SelectedReference ) == TRUE )
  {
    // check which channels can be selected to be tested
		for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
			if( ( IsGun( i ) == TRUE ) && ( i != m_SelectedReference ) )
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

void CSetupSpecGBDlg::TestEnabled()
{
  UpdateData( TRUE );

  if( m_SelectedReference != -1 )
  {
		//the selected reference channel cannot be selected to be tested
		m_TestCh[m_SelectedReference] = FALSE;
    //ask for channels to be tested
    m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST ); 
    SetDlgItemText( IDC_TEXT_5, m_Text );
  }

  UpdateData( FALSE );
}

void CSetupSpecGBDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CSetupSpecGBDlg::OnBnClickedOk()
{
  if( IsValidChannel( m_SelectedReference ) == FALSE )
  {
    m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
    SetDlgItemText( IDC_TEXT_5, m_Text );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    return;
  }

	BOOL testChannelSelected = FALSE;
	for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
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

  //store the reference channel and stations to be tested
  g_AlignerData.RefObjNo = m_SelectedReference;
  g_AlignerData.NoOfCorr = m_NoOfCorr;
  memcpy( g_AlignerData.ObjNo, m_ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );

  OnOK();
}

void CSetupSpecGBDlg::OnBnClickedTestCh()
{
  TestEnabled();
}

void CSetupSpecGBDlg::OnBnClickedCh1()
{
  m_SelectedReference = 1;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh2()
{
  m_SelectedReference = 2;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh3()
{
  m_SelectedReference = 3;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh4()
{
  m_SelectedReference = 4;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh5()
{
  m_SelectedReference = 5;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh6()
{
  m_SelectedReference = 6;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh7()
{
  m_SelectedReference = 7;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh8()
{
  m_SelectedReference = 8;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh9()
{
  m_SelectedReference = 9;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh10()
{
  m_SelectedReference = 10;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnBnClickedCh11()
{
  m_SelectedReference = 11;
  ReferenceSelected();
}

void CSetupSpecGBDlg::OnEnKillfocus()
{
  UpdateData( TRUE );
}
