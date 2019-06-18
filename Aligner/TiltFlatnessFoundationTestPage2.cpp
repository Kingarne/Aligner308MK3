// $Id: TiltFlatnessFoundationTestPage2.cpp,v 1.3 2013/04/26 14:41:36 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltFlatnessTestWizard.h"
#include "Util.h"
#include ".\tiltflatnessfoundationtestpage2.h"

IMPLEMENT_DYNAMIC(CTiltFlatnessFoundationTestPage2, CPropertyPage)

CTiltFlatnessFoundationTestPage2::CTiltFlatnessFoundationTestPage2()
	: CPropertyPage(CTiltFlatnessFoundationTestPage2::IDD)
{
}

CTiltFlatnessFoundationTestPage2::~CTiltFlatnessFoundationTestPage2()
{
}

void CTiltFlatnessFoundationTestPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTiltFlatnessFoundationTestPage2, CPropertyPage)
	ON_STN_CLICKED(IDC_INTRO_TEXT, OnStnClickedIntroText)
	ON_STN_CLICKED(IDC_TEXT_3, OnStnClickedText3)
END_MESSAGE_MAP()

// CTiltFlatnessFoundationTestPage2 message handlers

BOOL CTiltFlatnessFoundationTestPage2::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CTiltFlatnessTestWizard*) GetParent();   
  m_MsgCaption = _T("");
  m_Text = _T("");
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltFlatnessFoundationTestPage2::OnSetActive() 
{
  if( m_pParent == NULL )
  {
    m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
    m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
    MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
    EndDialog( IDCANCEL );
    return( FALSE ); // prevent the page from being active
  }
  m_pParent->SetWizardButtons( PSWIZB_NEXT );
  //GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
  //GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
  //GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );

  InitMeasureParam();
  m_pParent->m_Platf = FALSE;
  m_pParent->m_Leave = FALSE;

  //if( g_AlignerData.Found == TRUE )
  //{
    //m_Text.LoadString( IDS_TILT_FLATNESS_TEST_FOUNDATION_TEST );
    //SetDlgItemText( IDC_INTRO_TEXT, m_Text );
    //GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );
  //}
  //else
  //{
  //  m_Text.LoadString( IDS_TILT_FLATNESS_TEST_PLEASE_NOTICE );
  //  SetDlgItemText( IDC_INTRO_TEXT, m_Text );
  //  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_SHOW );
  //}
  //m_Text.LoadString( IDS_TILT_FLATNESS_TEST_TOT_NO_OF );
  //SetDlgItemText( IDC_TEXT_2, m_Text );
  //GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );

	if( IsFixed( g_AlignerData.RefObjNo ) == TRUE )
  {
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
			if( IsFixed( g_AlignerData.ObjNo[i] ) == FALSE )
      {
        m_pParent->m_Platf = TRUE;
      }
    }

    if( m_pParent->m_Platf == FALSE )
    { // should never happen
      m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
      m_Text.LoadString( IDS_SETUP_AT_LEAST_ONE_P_G_STATION );
      MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
      //GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
      //GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
      //GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );
      return CPropertyPage::OnSetActive();
    }
    
    //m_Text.LoadString( IDS_TILT_FLATNESS_TEST_SENSORS_REMAIN_IN_POS );
    //SetDlgItemText( IDC_TEXT_3, m_Text );
    //GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_SHOW );
  }
  m_pParent->m_N = 0;
  m_pParent->m_ERan = 0;
  memset( m_pParent->m_MaxSineFitError, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Esquare, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Eave, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  UpdateData( FALSE );
  m_pParent->m_Status = STATUS_PAGE_READY;
  g_AlignerData.ErrorDef = ERR_NO_ERROR;

  return CPropertyPage::OnSetActive();
}

BOOL CTiltFlatnessFoundationTestPage2::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CTiltFlatnessFoundationTestPage2::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

LRESULT CTiltFlatnessFoundationTestPage2::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:
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

void CTiltFlatnessFoundationTestPage2::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltFlatnessFoundationTestPage2::InitMeasureParam()
{
  //set inparameters for measure
  //this params already initiated:
  //m_pParent->m_Measure.m_InParam.pSource = NULL;
  //m_pParent->m_Measure.m_InParam.SourceStatus = ACQU_IDLE;
  
  DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			
  
  m_pParent->m_Measure.m_InParam.Break = FALSE;
  memcpy( m_pParent->m_Measure.m_InParam.ObjNo, g_AlignerData.ObjNo, SIZE_OF_ARRAYS * sizeof( int ) );
  m_pParent->m_Measure.m_InParam.RefObjNo = g_AlignerData.RefObjNo;
  m_pParent->m_Measure.m_InParam.Mode1 = RPath;
  m_pParent->m_Measure.m_InParam.Tao = g_AlignerData.TaoFlat;
  m_pParent->m_Measure.m_InParam.FiRef = 0;
  memset( m_pParent->m_Measure.m_InParam.FiObj, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Measure.m_InParam.ZParCorr, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_pParent->m_Measure.m_InParam.rotateBack, 0, SIZE_OF_ARRAYS * sizeof( BOOL ) );
  m_pParent->m_Measure.m_InParam.rotateBackRef = 0.0f;
  m_pParent->m_Measure.m_InParam.CountObj = g_AlignerData.NoOfCorr;
  m_pParent->m_Measure.m_InParam.RangeX = g_AlignerData.RX;
  m_pParent->m_Measure.m_InParam.RangeY = g_AlignerData.RY;
  m_pParent->m_Measure.m_InParam.UseElev = FALSE; 
  m_pParent->m_Measure.m_InParam.Fs = m_pParent->m_pSource->GetFrequency();
	m_pParent->m_Measure.m_InParam.SperryActive = m_pParent->GetSperryActive();
  m_pParent->m_Measure.m_InParam.commonFlatTest = FALSE;
	
	m_pParent->m_pSource -> Start() ;
}

void CTiltFlatnessFoundationTestPage2::OnStnClickedIntroText()
{
	// TODO: Add your control notification handler code here
}

void CTiltFlatnessFoundationTestPage2::OnStnClickedText3()
{
	// TODO: Add your control notification handler code here
}
