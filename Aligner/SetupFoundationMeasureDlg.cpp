// $Id: SetupFoundationMeasureDlg.cpp,v 1.5 2014-07-11 09:29:33 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "SetupFoundationMeasureDlg.h"
#include "Util.h"
#include "GlobalData.h"
#include "GraphView.h"
#include ".\setupfoundationmeasuredlg.h"

// CSetupFoundationMeasureDlg dialog

IMPLEMENT_DYNAMIC(CSetupFoundationMeasureDlg, CDialog)
CSetupFoundationMeasureDlg::CSetupFoundationMeasureDlg( BOOL CommonFlatTest /*= FALSE*/, BOOL Found /*=FALSE*/, BOOL Flatness /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupFoundationMeasureDlg::IDD, pParent)
{
    m_CommonFlatTest = CommonFlatTest;
    //m_Fou = Found;
    m_Flatness = Flatness;
    m_SelectedReference = -1;
    m_testCh = -1;
    m_lengthArm1 = 0;
    m_lengthArm2 = 0;
    m_refAngle = 0;
    m_bMeasureWarping = FALSE;
}

CSetupFoundationMeasureDlg::~CSetupFoundationMeasureDlg()
{
}

void CSetupFoundationMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_LENGTH_CH_1, m_lengthArm1);
  DDV_MinMaxInt(pDX, m_lengthArm1, MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_LENGTH_ARM_2, m_lengthArm2);
  DDV_MinMaxInt(pDX, m_lengthArm2, MIN_LENGTH, MAX_LENGTH);
  DDX_Text(pDX, IDC_ANGLE_CH_1, m_refAngle);
  DDV_MinMaxInt(pDX, m_refAngle, MIN_ANGLE, MAX_ANGLE);
  DDX_Control(pDX, IDC_FOUND_TYPE_COMBO, m_typeCombo);
}

void CSetupFoundationMeasureDlg::SetObjectData()
{
    m_NoOfCorr = 0;
    memset( m_ObjNo, 0, SIZE_OF_ARRAYS * sizeof( int ) );
    if(m_testCh != -1)
    {
        m_NoOfCorr=1;
        m_ObjNo[1] = m_testCh;    
    }
    
}

int CSetupFoundationMeasureDlg::GetNoOfToBeTested()
{
    return (m_testCh == -1) ? 0 : 1;

}

void CSetupFoundationMeasureDlg::CommonFlatTestCheck( int channelNo )
{

}

BEGIN_MESSAGE_MAP(CSetupFoundationMeasureDlg, CDialog)
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
  ON_EN_KILLFOCUS(IDC_LENGTH_CH_1, OnEnKillfocus)
  ON_EN_KILLFOCUS(IDC_ANGLE_CH_1, OnEnKillfocus)
  ON_STN_CLICKED(IDC_TEXT_1, OnStnClickedText1)
	ON_BN_CLICKED(IDC_RB_TEST_CH1, OnBnClickedRbTestCh1)
	ON_BN_CLICKED(IDC_RB_TEST_CH2, OnBnClickedRbTestCh2)
	ON_BN_CLICKED(IDC_RB_TEST_CH3, OnBnClickedRbTestCh3)
	ON_BN_CLICKED(IDC_RB_TEST_CH4, OnBnClickedRbTestCh4)
	ON_BN_CLICKED(IDC_RB_TEST_CH5, OnBnClickedRbTestCh5)
	ON_BN_CLICKED(IDC_RB_TEST_CH6, OnBnClickedRbTestCh6)
	ON_BN_CLICKED(IDC_RB_TEST_CH7, OnBnClickedRbTestCh7)
	ON_BN_CLICKED(IDC_RB_TEST_CH8, OnBnClickedRbTestCh8)
	ON_BN_CLICKED(IDC_RB_TEST_CH9, OnBnClickedRbTestCh9)
	ON_BN_CLICKED(IDC_RB_TEST_CH10, OnBnClickedRbTestCh10)
	ON_BN_CLICKED(IDC_RB_TEST_CH11, OnBnClickedRbTestCh11)
    ON_BN_CLICKED(IDC_MEASURE_WARPING, OnBnClickedMeasureWarping)
  ON_CBN_SELCHANGE(IDC_FOUND_TYPE_COMBO, &CSetupFoundationMeasureDlg::OnCbnSelchangeFoundTypeCombo)
END_MESSAGE_MAP()

void CSetupFoundationMeasureDlg::InitMaps()
{
    m_textMap[1] = IDC_TEXT_CH_1;
    m_textMap[2] = IDC_TEXT_CH_2;
    m_textMap[3] = IDC_TEXT_CH_3;
    m_textMap[4] = IDC_TEXT_CH_4;
    m_textMap[5] = IDC_TEXT_CH_5;
    m_textMap[6] = IDC_TEXT_CH_6;
    m_textMap[7] = IDC_TEXT_CH_7;
    m_textMap[8] = IDC_TEXT_CH_8;
    m_textMap[9] = IDC_TEXT_CH_9;
    m_textMap[10] = IDC_TEXT_CH_10;
    m_textMap[11] = IDC_TEXT_CH_11;

    m_refChMap[1] = IDC_REF_CH_1;
    m_refChMap[2] = IDC_REF_CH_2;
    m_refChMap[3] = IDC_REF_CH_3;
    m_refChMap[4] = IDC_REF_CH_4;
    m_refChMap[5] = IDC_REF_CH_5;
    m_refChMap[6] = IDC_REF_CH_6;
    m_refChMap[7] = IDC_REF_CH_7;
    m_refChMap[8] = IDC_REF_CH_8;
    m_refChMap[9] = IDC_REF_CH_9;
    m_refChMap[10] = IDC_REF_CH_10;
    m_refChMap[11] = IDC_REF_CH_11;

    m_rbChMap[1] = IDC_RB_TEST_CH1;
    m_rbChMap[2] = IDC_RB_TEST_CH2;
    m_rbChMap[3] = IDC_RB_TEST_CH3;
    m_rbChMap[4] = IDC_RB_TEST_CH4;
    m_rbChMap[5] = IDC_RB_TEST_CH5;
    m_rbChMap[6] = IDC_RB_TEST_CH6;
    m_rbChMap[7] = IDC_RB_TEST_CH7;
    m_rbChMap[8] = IDC_RB_TEST_CH8;
    m_rbChMap[9] = IDC_RB_TEST_CH9;
    m_rbChMap[10] = IDC_RB_TEST_CH10;
    m_rbChMap[11] = IDC_RB_TEST_CH11;
}
// CSetupFoundationMeasureDlg message handlers

BOOL CSetupFoundationMeasureDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitMaps();
    if( GetNoOfConnectedStations() < 2 )
    {
    // Msg box : Information : Please connect at least two stations
        m_Text.LoadString( IDS_CONNECT_TWO_STATIONS );
        m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        EndDialog( IDCANCEL );
        return FALSE;
    }


    int NoOfObjects, NoOfFix;


    SetDlgItemText( IDC_TEXT_1, g_AlignerData.AlignMode );


    NoOfObjects = 0;
    NoOfFix = 0;

    int startChannel = FIRST_ARRAY_INDEX_SYNCRO;

    TRACE("max:%d",LAST_ARRAY_INDEX_SENSOR);
    for( int i=startChannel; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {

			AfxFormatString2( m_ChText[i], IDS_SENSOR_NAME_TO_DEVICE, GetChannelName( i ), (GetUnitTypeDescription( i )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );


        GetDlgItem( m_textMap[i] )->ShowWindow( SW_SHOW );
        GetDlgItem( m_refChMap[i])->ShowWindow( SW_SHOW );
        GetDlgItem( m_rbChMap[i])->ShowWindow( SW_SHOW );
        
        //Show text for the connected channels
    

        //check which channels are connected
        if( IsConnected( i ) == TRUE )
        {
            NoOfObjects++;
            //Enable text for the connected channels
            GetDlgItem( m_textMap[i] )->EnableWindow( TRUE );
           
            //check which channels are fix
            if( IsFixed( i ) == TRUE )
            {
                NoOfFix++;
                GetDlgItem( m_refChMap[i] )->EnableWindow( TRUE );   

            }
        }
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

	// At least one ref and one test channel must be defined
    if( NoOfObjects <= 1 )		
    {

        m_Text.LoadString( IDS_SETUP_MEASURE_SPECIFY_AT_LEAST_TWO );

        SetDlgItemText( IDC_TEXT_5, m_Text );
        GetDlgItem( IDOK )->EnableWindow( FALSE );
        return TRUE;
    }

    if(  NoOfFix == 0  )
    {
        m_Text.LoadString(IDS_SETUP_FOUNDATION_MEASURE_DEFINE_A_FIXED_REF) ;
        SetDlgItemText( IDC_TEXT_5, m_Text );
        GetDlgItem( IDOK )->EnableWindow( FALSE );
        return TRUE;
    }
  

    m_Text.LoadString(IDS_SETUP_FOUNDATION_MEASURE_SELECT_A_FIXED_REF) ;
    SetDlgItemText( IDC_TEXT_5, m_Text );


    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        if( (IsConnected( g_AlignerData.ObjNo[i] ) == TRUE))// && ( (m_Fou == TRUE) || (IsFixed( g_AlignerData.ObjNo[i] ) == TRUE) ) )
        {
            m_testCh = g_AlignerData.ObjNo[i];
            //m_TestCh[g_AlignerData.ObjNo[i]] = TRUE;            
        }
    }
    SetObjectData();

    m_lengthArm1 = 0;
    SetIndexArmLength( 1, m_lengthArm1 ) ;
    m_refAngle = 0 ;
    SetZeroRef( 1, m_refAngle ) ;	      
    
    //restore selected reference channel
    if( ( IsConnected( g_AlignerData.RefObjNo ) == TRUE )  )
    {
        m_SelectedReference = g_AlignerData.RefObjNo;
    }
    else
    {
        m_SelectedReference = -1;
    }
  

    m_typeCombo.AddString("Circular");
    m_typeCombo.AddString("Rectangular");
    m_typeCombo.SelectString(0, "Circular");

    CheckRadioButton( IDC_REF_CH_1, IDC_REF_CH_11, m_refChMap[m_SelectedReference] );
  
    UpdateData( FALSE );

    ReferenceSelected();

    return TRUE;  // return TRUE  unless you set the focus to a control
}//OnInitDialog()

void CSetupFoundationMeasureDlg::ReferenceSelected()
{
    UpdateData( TRUE );

      
    //disable the radio buttons for selection of the channel to be tested
	GetDlgItem( IDC_RB_TEST_CH1 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH3 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH4 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH5 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH6 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH7 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH9 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH10 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RB_TEST_CH11 )->EnableWindow( FALSE );

    if( IsValidChannel( m_SelectedReference ) == TRUE )
    {
        // check which channels can be selected to be tested
        int firstArrayIndex;

        firstArrayIndex = FIRST_ARRAY_INDEX_SYNCRO;


        for( int i=firstArrayIndex; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
        {
	        // Only Platforms allowed, modified if condition
            if( ( ( IsConnected( i ) == TRUE ) && ( i != m_SelectedReference ) )&&  ( IsPlane( i ) == TRUE ) )     
            {
		        // Now tell to select channel to test
		        m_Text.LoadString( IDS_SETUP_FOUNDATION_MEASURE_SELECT_A_FIXED_CH ); 
		        SetDlgItemText( IDC_TEXT_5, m_Text );
                //enable the check box for selection of the channel to be tested
                GetDlgItem( m_rbChMap[i])->EnableWindow( TRUE );
            
            }
        }
    }

    UpdateData( FALSE );
}

void CSetupFoundationMeasureDlg::TestEnabled()
{
    UpdateData( TRUE );

    // Disable select ref and ch text
    GetDlgItem( IDC_TEXT_5 )->ShowWindow( SW_HIDE );

		// Foundation Test Adapter instructions
    m_Text.LoadString( IDS_SETUP_MEASURE_INFO );
    SetDlgItemText( IDC_TEXT_2, m_Text );
    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );		

    GetDlgItem( IDC_INDEX_ARM_LENGTH_TEXT )->ShowWindow( SW_SHOW );

    //set text above the edit box for angle
    m_Text.LoadString( IDS_SETUP_FOUNDATION_MEASURE_ANGLE );
    SetDlgItemText( IDC_AZI_REF_ANGLE_TEXT, m_Text );
    GetDlgItem( IDC_AZI_REF_ANGLE_TEXT )->ShowWindow( SW_SHOW );

    //show the edit boxes for length and angle
   
    int type;
    if ((type = m_typeCombo.GetCurSel()) == CB_ERR)
      return;

    GetDlgItem( IDC_FOUND_TYPE_COMBO)->ShowWindow(SW_SHOW);
    GetDlgItem( IDC_TYPE_STATIC)->ShowWindow(SW_SHOW);
    
    int showLength = (type == FoundationT::Circular) ? SW_SHOW : SW_HIDE;    
    
    GetDlgItem(IDC_ANGLE_CH_1)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_LENGTH_CH_1)->ShowWindow(showLength);    
    GetDlgItem(IDC_INDEX_ARM_LENGTH_TEXT)->ShowWindow(showLength);    
    GetDlgItem(IDC_MEASURE_WARPING)->ShowWindow(showLength);

    CButton* pButt = (CButton*)GetDlgItem(IDC_MEASURE_WARPING);
    m_bMeasureWarping = pButt->GetCheck() && (type == FoundationT::Circular);
    
    CWnd* pWnd = GetDlgItem(IDC_LENGTH_ARM_2);
    pWnd->ShowWindow(m_bMeasureWarping ? SW_SHOW : SW_HIDE);
    pWnd = GetDlgItem(IDC_INDEX_ARM_2_TEXT);
    pWnd->ShowWindow(m_bMeasureWarping ? SW_SHOW : SW_HIDE);


    //for all channels selected to be tested, enable the edit boxes for length and angle
    BOOL boxEnabled = FALSE;
    if (m_testCh != -1)
    {
      boxEnabled = TRUE;
      GetDlgItem(IDC_LENGTH_CH_1)->EnableWindow(TRUE);
      GetDlgItem(IDC_ANGLE_CH_1)->EnableWindow(TRUE);
    }
    
    
    UpdateData( FALSE );
}

void CSetupFoundationMeasureDlg::OnBnClickedCancel()
{
  OnCancel();
}

void CSetupFoundationMeasureDlg::OnBnClickedOk()
{
    UpdateData( TRUE );
    //validate m_SelectedReference
    if( ( IsConnected( m_SelectedReference ) == FALSE ) || ( IsFixed( m_SelectedReference ) == FALSE ) )
    {
        m_Text.LoadString( IDS_SETUP_FOUNDATION_MEASURE_DEFINE_A_FIXED_REF );
        SetDlgItemText( IDC_TEXT_5, m_Text );
        m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
        MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
        return;
    }

	
	BOOL testChannelSelected = (m_testCh != -1);

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

    int type;
    if ((type = m_typeCombo.GetCurSel()) == CB_ERR)
      return;

    if (type == FoundationT::Circular)
    {
      SetIndexArmLength(1, m_lengthArm1);

      if (m_lengthArm1 <= 0)
      {
        m_MsgCaption.LoadString(IDS_ERROR_CAPTION);
        m_Text.LoadString(IDS_SETUP_MEASURE_ENTER_LENGTH_ERROR);
        MessageBox(m_Text, m_MsgCaption, MB_ICONERROR | MB_OK);
        return;
      }

      if (m_bMeasureWarping)
      {
        if (m_lengthArm2 <= 0)
        {
          m_MsgCaption.LoadString(IDS_ERROR_CAPTION);
          m_Text.LoadString(IDS_SETUP_MEASURE_ENTER_LENGTH2_ERROR);
          MessageBox(m_Text, m_MsgCaption, MB_ICONERROR | MB_OK);
          return;
        }
        else
        {
          SetIndexArmLength(2, m_lengthArm2);
        }
      }     
    }

    //store the channels to be tested
    g_AlignerData.RefObjNo = m_SelectedReference;
    g_AlignerData.NoOfCorr = m_NoOfCorr;
    memcpy( g_AlignerData.ObjNo, m_ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
    
    SetZeroRef( 1, m_refAngle );
    FoundationStraightEdgeAngle = m_refAngle;
    g_AlignerData.FoundationType = (FoundationT)type;

	if (!CheckAllSensorsConnected())
	{
		m_Text.Format("Unconnected sensor detected! Continue anyway?");
		if (MessageBox(m_Text, m_MsgCaption, MB_ICONWARNING | MB_YESNO) != IDYES)
			return;
	}


	OnOK();
}

bool CSetupFoundationMeasureDlg::CheckSensorsConnected(int index)
{
	if (IsSensor(index))
	{
		int i = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(index);

		double tempr = DAU::GetDAU().GetSensor(i)->GetTemperature();
		if (tempr > 68.3f)
			return false;
	}

	return true;
}

bool CSetupFoundationMeasureDlg::CheckAllSensorsConnected()
{
	if (!CheckSensorsConnected(g_AlignerData.RefObjNo))
		return false;

	for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
	{
		if (!CheckSensorsConnected(g_AlignerData.ObjNo[i]))
			return false;

	}

	return true;
}

void CSetupFoundationMeasureDlg::OnBnClickedCh1()
{
  m_SelectedReference = 1;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh2()
{
  m_SelectedReference = 2;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh3()
{
  m_SelectedReference = 3;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh4()
{
  m_SelectedReference = 4;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh5()
{
  m_SelectedReference = 5;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh6()
{
  m_SelectedReference = 6;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh7()
{
  m_SelectedReference = 7;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh8()
{
  m_SelectedReference = 8;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh9()
{
  m_SelectedReference = 9;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh10()
{
  m_SelectedReference = 10;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnBnClickedCh11()
{
  m_SelectedReference = 11;
  ReferenceSelected();
}

void CSetupFoundationMeasureDlg::OnEnKillfocus()
{
  UpdateData( TRUE );
}

void CSetupFoundationMeasureDlg::OnStnClickedText1()
{
	// TODO: Add your control notification handler code here
}

void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh(int ChNo)
{
    UpdateData( TRUE );
    CommonFlatTestCheck( ChNo );
    m_testCh = ChNo;
    UpdateData( FALSE );
    TestEnabled();
}

void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh1()
{ OnBnClickedRbTestCh(1) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh2()
{ OnBnClickedRbTestCh(2) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh3()
{ OnBnClickedRbTestCh(3) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh4()
{ OnBnClickedRbTestCh(4) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh5()
{ OnBnClickedRbTestCh(5) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh6()
{ OnBnClickedRbTestCh(6) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh7()
{ OnBnClickedRbTestCh(7) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh8()
{ OnBnClickedRbTestCh(8) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh9()
{ OnBnClickedRbTestCh(9) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh10()
{ OnBnClickedRbTestCh(10) ; }
void CSetupFoundationMeasureDlg::OnBnClickedRbTestCh11()
{ OnBnClickedRbTestCh(11) ; }

void CSetupFoundationMeasureDlg::OnBnClickedMeasureWarping()
{
    TestEnabled();

}


void CSetupFoundationMeasureDlg::OnCbnSelchangeFoundTypeCombo()
{
  TestEnabled();

}
