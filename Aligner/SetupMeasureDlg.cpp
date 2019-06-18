// $Id: SetupMeasureDlg.cpp,v 1.4 2014-10-16 08:46:56 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "SetupMeasureDlg.h"
#include "Util.h"
#include "GlobalData.h"
#include "GraphView.h"

// CSetupMeasureDlg dialog

IMPLEMENT_DYNAMIC(CSetupMeasureDlg, CDialog)
CSetupMeasureDlg::CSetupMeasureDlg( BOOL CommonFlatTest /*= FALSE*/, BOOL Found /*=FALSE*/, BOOL Flatness /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupMeasureDlg::IDD, pParent)
{
  m_CommonFlatTest = CommonFlatTest;
  m_Fou = Found; // May be removed, always false!
  m_Flatness = Flatness;
  m_SelectedReference = -1;
}

CSetupMeasureDlg::~CSetupMeasureDlg()
{
}

void CSetupMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
 /* DDX_Text(pDX, IDC_LENGTH_CH_1, m_LengthCh[1]);
  DDV_MinMaxInt(pDX, m_LengthCh[1], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_2, m_LengthCh[2]);
  DDV_MinMaxInt(pDX, m_LengthCh[2], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_3, m_LengthCh[3]);
  DDV_MinMaxInt(pDX, m_LengthCh[3], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_4, m_LengthCh[4]);
  DDV_MinMaxInt(pDX, m_LengthCh[4], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_5, m_LengthCh[5]);
  DDV_MinMaxInt(pDX, m_LengthCh[5], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_6, m_LengthCh[6]);
  DDV_MinMaxInt(pDX, m_LengthCh[6], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_7, m_LengthCh[7]);
  DDV_MinMaxInt(pDX, m_LengthCh[7], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_8, m_LengthCh[8]);
  DDV_MinMaxInt(pDX, m_LengthCh[8], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_9, m_LengthCh[9]);
  DDV_MinMaxInt(pDX, m_LengthCh[9], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_10, m_LengthCh[10]);
  DDV_MinMaxInt(pDX, m_LengthCh[10], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_CH_11, m_LengthCh[11]);
  DDV_MinMaxInt(pDX, m_LengthCh[11], MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_ANGLE_CH_1, m_AngleCh[1]);
  DDV_MinMaxInt(pDX, m_AngleCh[1], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_2, m_AngleCh[2]);
  DDV_MinMaxInt(pDX, m_AngleCh[2], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_3, m_AngleCh[3]);
  DDV_MinMaxInt(pDX, m_AngleCh[3], MIN_ANGLE, MAX_ANGLE); 
  DDX_Text(pDX, IDC_ANGLE_CH_4, m_AngleCh[4]);
  DDV_MinMaxInt(pDX, m_AngleCh[4], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_5, m_AngleCh[5]);
  DDV_MinMaxInt(pDX, m_AngleCh[5], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_6, m_AngleCh[6]);
  DDV_MinMaxInt(pDX, m_AngleCh[6], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_7, m_AngleCh[7]);
  DDV_MinMaxInt(pDX, m_AngleCh[7], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_8, m_AngleCh[8]);
  DDV_MinMaxInt(pDX, m_AngleCh[8], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_9, m_AngleCh[9]);
  DDV_MinMaxInt(pDX, m_AngleCh[9], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_10, m_AngleCh[10]);
  DDV_MinMaxInt(pDX, m_AngleCh[10], MIN_ANGLE, MAX_ANGLE);
  DDX_Text(pDX, IDC_ANGLE_CH_11, m_AngleCh[11]);
  DDV_MinMaxInt(pDX, m_AngleCh[11], MIN_ANGLE, MAX_ANGLE);*/

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

void CSetupMeasureDlg::SetObjectData()
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

int CSetupMeasureDlg::GetNoOfToBeTested()
{
  int noOfToBeTested = 0;
    
	for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
	{
		if( m_TestCh[i] == TRUE )
		{
			noOfToBeTested++;
    }
  }
  return( noOfToBeTested );
}

void CSetupMeasureDlg::CommonFlatTestCheck( int channelNo )
{
  if( m_CommonFlatTest == TRUE )
  {
    if( GetNoOfToBeTested() > MAX_NO_OF_TO_BE_TESTED_IN_COMMON_FLAT_TEST )
    {
      m_TestCh[ channelNo ] = FALSE;
      m_Text.LoadString( IDS_TO_MANY_CHANNELS_SELECTED_FOR_COMMON_FLAT_TEST );
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    }
    UpdateData( FALSE );
  }
}

BEGIN_MESSAGE_MAP(CSetupMeasureDlg, CDialog)
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
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_1, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_2, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_3, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_4, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_5, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_6, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_7, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_8, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_9, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_10, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_11, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_1, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_2, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_3, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_4, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_5, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_6, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_7, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_8, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_9, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_10, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_11, OnEnKillfocus)
END_MESSAGE_MAP()

// CSetupMeasureDlg message handlers

BOOL CSetupMeasureDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if( GetNoOfConnectedStations() < 2 )
  {
    m_Text.LoadString( IDS_CONNECT_TWO_STATIONS );
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }


  int NoOfObjects, NoOfFix;

  memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  memset( m_LengthCh, 0, SIZE_OF_ARRAYS * sizeof( int ) );
  memset( m_AngleCh, 0, SIZE_OF_ARRAYS * sizeof( int ) );

  SetDlgItemText( IDC_TEXT_1, g_AlignerData.AlignMode );

	if( m_CommonFlatTest == TRUE )
	{
    m_Text.LoadString( IDC_CONNECTED_SENSORS_TEXT );
    SetDlgItemText( IDC_CONNECTED_TEXT, m_Text );
	}

  if( m_Fou == TRUE )
  {
    m_Text.LoadString( IDS_SETUP_MEASURE_INFO );
    SetDlgItemText( IDC_TEXT_2, m_Text );
    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );
  }

  if( ( m_Flatness == FALSE ) && ( m_CommonFlatTest == FALSE ) )
  {
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SYNCRO; i++ )
    {
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
      }
    }// for( all syncros )
  }

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
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
    }
  }// for( all sensors )

  NoOfObjects = 0;
  NoOfFix = 0;

  int startChannel = FIRST_ARRAY_INDEX_SYNCRO;
  if( m_CommonFlatTest == TRUE )
  {
    startChannel = FIRST_ARRAY_INDEX_SENSOR;
  }

  for( int i=startChannel; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
		if( m_CommonFlatTest == TRUE )
		{
			CString sensorSN;
			AfxFormatString1( sensorSN, IDS_SN_XXX, GetSensorSerialNumber( i ) );
			AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, sensorSN, (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
		}
		else
		{
			AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, GetChannelName( i ), (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
		}

    //Show text for the connected channels
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
      NoOfObjects++;
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

      if( m_Fou == FALSE )
      { 
				if( ( m_CommonFlatTest == FALSE ) || ( IsSyncro( i ) == FALSE ) )
				{
					//enable the radio buttons for selection of the reference channel
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

      //check which channels are fix
			if( IsFixed( i ) == TRUE )
      {
        NoOfFix++;

        if( m_Fou == TRUE )
        {
          //enable the radio button for selection of the fix reference channel
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
          }//switch( i )
        }//if( m_Fou == TRUE )
      }//if( FIXED )
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

  if( NoOfObjects <= 1 )
  {
    if( m_CommonFlatTest == TRUE )
    {
      m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_AT_LEAST_TWO_SENSORS );
    }
    else
    {
      m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_AT_LEAST_TWO );
    }
    SetDlgItemText( IDC_TEXT_5, m_Text );
    GetDlgItem( IDOK )->EnableWindow( FALSE );
    return TRUE;
  }

  if( ( m_Fou == TRUE ) && ( NoOfFix == 0 ) )
  {
    m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_AT_LEAST_ONE_F_PLANE );
    SetDlgItemText( IDC_TEXT_5, m_Text );
    GetDlgItem( IDOK )->EnableWindow( FALSE );
    return TRUE;
  }
  
  if( m_Fou == TRUE )
  {
    m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_FIX_REFERENCE );
    SetDlgItemText( IDC_TEXT_5, m_Text );
  }
  else
  {
    if( m_CommonFlatTest == TRUE )
    {
      m_Text.LoadString( IDS_SPECIFY_REFERENCE_SENSOR );
    }
    else
    {
      m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
    }
    SetDlgItemText( IDC_TEXT_5, m_Text );
  }

  //restore the channels to be tested
  memset( m_TestCh, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );

  for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
  {
    if( (IsConnected( g_AlignerData.ObjNo[i] ) == TRUE) &&
      ( (m_Fou == FALSE) || (IsFixed( g_AlignerData.ObjNo[i] ) == TRUE) ) )
    {
      switch( g_AlignerData.ObjNo[i] )
      {
      case 1:
        m_TestCh[1] = TRUE;
        break;
      case 2:
        m_TestCh[2] = TRUE;
        break;
      case 3:
        m_TestCh[3] = TRUE;
        break;
      case 4:
        m_TestCh[4] = TRUE;
        break;
      case 5:
        m_TestCh[5] = TRUE;
        break;
      case 6:
        m_TestCh[6] = TRUE;
        break;
      case 7:
        m_TestCh[7] = TRUE;
        break;
      case 8:
        m_TestCh[8] = TRUE;
        break;
      case 9:
        m_TestCh[9] = TRUE;
        break;
      case 10:
        m_TestCh[10] = TRUE;
        break;
      case 11:
        m_TestCh[11] = TRUE;
        break;
      }
    }
  }
  SetObjectData();

  if( m_Fou == TRUE )
  {
    //restore length and angle
    for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      m_LengthCh[i] = GetIndexArmLength( i );
      m_AngleCh[i] = GetZeroRef( i );
    }
  }

  //restore selected reference channel
  if( ( IsConnected( g_AlignerData.RefObjNo ) == TRUE ) &&
      ( (m_Fou == FALSE) || (IsFixed( g_AlignerData.RefObjNo ) == TRUE) ) )
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

  UpdateData( FALSE );

  ReferenceSelected();

  return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupMeasureDlg::ReferenceSelected()
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
    int firstArrayIndex;
    if( m_CommonFlatTest == TRUE )
    {
      firstArrayIndex = FIRST_ARRAY_INDEX_SENSOR;
    }
    else
    {
      firstArrayIndex = FIRST_ARRAY_INDEX_SYNCRO;
    }

    for( int i=firstArrayIndex; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
      if( ( ( IsConnected( i ) == TRUE ) && ( i != m_SelectedReference ) )&&
					( ( m_Fou == FALSE ) || ( IsFixed( i ) == FALSE ) ) )
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
    }//for( all channels )
  }

  TestEnabled();

  UpdateData( FALSE );
}

void CSetupMeasureDlg::TestEnabled()
{
  UpdateData( TRUE );

  if( IsValidChannel( m_SelectedReference ) == TRUE )
  {
    //the selected reference channel cannot be selected to be tested
    m_TestCh[m_SelectedReference] = FALSE;
  }

  if( m_Fou == TRUE )
  {
    //set text above the edit box for length
    m_Text.LoadString( IDS_SETUP_MEASURE_LENGTH );
    SetDlgItemText( IDC_INDEX_ARM_LENGTH_TEXT, m_Text );
    GetDlgItem( IDC_INDEX_ARM_LENGTH_TEXT )->ShowWindow( SW_SHOW );

    //set text above the edit box for angle
    m_Text.LoadString( IDS_SETUP_MEASURE_ANGLE );
    SetDlgItemText( IDC_AZI_REF_ANGLE_TEXT, m_Text );
    GetDlgItem( IDC_AZI_REF_ANGLE_TEXT )->ShowWindow( SW_SHOW );

    //show the edit boxes for length and angle
    GetDlgItem( IDC_LENGTH_CH_1 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_2 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_3 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_4 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_5 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_6 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_7 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_8 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_9 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_10 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_LENGTH_CH_11 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_1 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_2 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_3 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_4 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_5 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_6 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_7 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_8 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_9 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_10 )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_ANGLE_CH_11 )->ShowWindow( SW_SHOW );

    //disable the edit boxes for length and angle
    GetDlgItem( IDC_LENGTH_CH_1 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_3 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_4 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_5 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_6 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_7 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_9 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_10 )->EnableWindow( FALSE );
    GetDlgItem( IDC_LENGTH_CH_11 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_1 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_3 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_4 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_5 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_6 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_7 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_9 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_10 )->EnableWindow( FALSE );
    GetDlgItem( IDC_ANGLE_CH_11 )->EnableWindow( FALSE );

    //for all channels selected to be tested, enable the edit boxes for length and angle
    BOOL boxEnabled = FALSE;
    if( m_TestCh[1] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_1 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_1 )->EnableWindow( TRUE );
    }
    if( m_TestCh[2] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_2 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_2 )->EnableWindow( TRUE );
    }
    if( m_TestCh[3] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_3 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_3 )->EnableWindow( TRUE );
    }
    if( m_TestCh[4] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_4 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_4 )->EnableWindow( TRUE );
    }
    if( m_TestCh[5] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_5 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_5 )->EnableWindow( TRUE );
    }
    if( m_TestCh[6] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_6 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_6 )->EnableWindow( TRUE );
    }
    if( m_TestCh[7] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_7 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_7 )->EnableWindow( TRUE );
    }
    if( m_TestCh[8] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_8 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_8 )->EnableWindow( TRUE );
    }
    if( m_TestCh[9] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_9 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_9 )->EnableWindow( TRUE );
    }
    if( m_TestCh[10] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_10 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_10 )->EnableWindow( TRUE );
    }
    if( m_TestCh[11] == TRUE )
    {
      boxEnabled = TRUE;
      GetDlgItem( IDC_LENGTH_CH_11 )->EnableWindow( TRUE );
      GetDlgItem( IDC_ANGLE_CH_11 )->EnableWindow( TRUE );
    }

    if( boxEnabled == TRUE )
    {
      //some channel(s) selected to be tested, ask for length and angle
      m_Text.LoadString( IDS_SETUP_MEASURE_ENTER_LENGTH ); 
      SetDlgItemText( IDC_TEXT_5, m_Text );
    }
    else
    {
      //no channel(s) selected to be tested, ask for stations to be tested
      m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST ); 
      SetDlgItemText( IDC_TEXT_5, m_Text );
    }
  }//if( m_Fou == TRUE )
  else
  {
    if( IsValidChannel( m_SelectedReference ) == TRUE )
    {
      //ask for channels to be tested
      if( m_CommonFlatTest == TRUE )
      {
        m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST_SENSOR ); 
      }
      else
      {
        m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_CH_TO_TEST ); 
      }
      SetDlgItemText( IDC_TEXT_5, m_Text );
    }
  }

  UpdateData( FALSE );
}

void CSetupMeasureDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CSetupMeasureDlg::OnBnClickedOk()
{
  //validate m_SelectedReference
  if( m_Fou == TRUE )
  {
    if( ( IsConnected( m_SelectedReference ) == FALSE ) ||
        ( IsFixed( m_SelectedReference ) == FALSE ) )
    {
      m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_FIX_REFERENCE );
      SetDlgItemText( IDC_TEXT_5, m_Text );
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      return;
    }
  }
  else
  {
    if( IsConnected( m_SelectedReference ) == FALSE )
    {
      if( m_CommonFlatTest == TRUE )
      {
        m_Text.LoadString( IDS_SPECIFY_REFERENCE_SENSOR );
      }
      else
      {
        m_Text.LoadString( IDS_SPECIFY_REFERENCE_STATION );
      }
      SetDlgItemText( IDC_TEXT_5, m_Text );
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      return;
    }
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

  if( m_Flatness == TRUE )
  {
    BOOL planeOrGunConnected = TRUE;

		if( IsFixed( m_SelectedReference ) == TRUE )
    {
      planeOrGunConnected = FALSE;

      for( int i=1; i<=m_NoOfCorr; i++ )
      {
				if( IsFixed( m_ObjNo[i] ) == FALSE )
        {
          planeOrGunConnected = TRUE;
        }
      }    
    }

    if( planeOrGunConnected == FALSE )
    {
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      m_Text.LoadString( IDS_SETUP_AT_LEAST_ONE_P_G_STATION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      return;
    }
  }

    //store the channels to be tested
    g_AlignerData.RefObjNo = m_SelectedReference;
    g_AlignerData.NoOfCorr = m_NoOfCorr;
    memcpy( g_AlignerData.ObjNo, m_ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
      
    if( m_Fou == TRUE )
    {
        //store length and angle
        for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
        {
		    SetIndexArmLength( i, m_LengthCh[i] );
            SetZeroRef( i, m_AngleCh[i] );
        }
    }

    OnOK();
}

void CSetupMeasureDlg::OnBnClickedTestCh1()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 1 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh2()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 2 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh3()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 3 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh4()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 4 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh5()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 5 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh6()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 6 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh7()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 7 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh8()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 8 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh9()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 9 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh10()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 10 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedTestCh11()
{
  UpdateData( TRUE );
  CommonFlatTestCheck( 11 );
  TestEnabled();
}

void CSetupMeasureDlg::OnBnClickedCh1()
{
  m_SelectedReference = 1;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh2()
{
  m_SelectedReference = 2;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh3()
{
  m_SelectedReference = 3;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh4()
{
  m_SelectedReference = 4;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh5()
{
  m_SelectedReference = 5;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh6()
{
  m_SelectedReference = 6;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh7()
{
  m_SelectedReference = 7;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh8()
{
  m_SelectedReference = 8;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh9()
{
  m_SelectedReference = 9;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh10()
{
  m_SelectedReference = 10;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnBnClickedCh11()
{
  m_SelectedReference = 11;
  ReferenceSelected();
}

void CSetupMeasureDlg::OnEnKillfocus()
{
  UpdateData( TRUE );
}
