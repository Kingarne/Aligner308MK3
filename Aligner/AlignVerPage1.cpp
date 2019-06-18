// $Id: AlignVerPage1.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Include.h"
#include "AlignVerWizard.h"
#include "SetupTestDlg.h"

// CAlignVerPage1 dialog
IMPLEMENT_DYNAMIC(CAlignVerPage1, CPropertyPage)
CAlignVerPage1::CAlignVerPage1()
	: CPropertyPage(1)
{
}

CAlignVerPage1::~CAlignVerPage1()
{
}

void CAlignVerPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

void CAlignVerPage1::ShowPlatformTestSetup()
{
  if( g_AlignerData.NoOfCorr > 0 )
  {
    CString tmpStr, tmpStr1, tmpStr2, text;
    tmpStr.LoadString( IDS_SETUP_PLANE_PLATFORMS_ZPAR );
    SetDlgItemText( IDC_TEXT_4, tmpStr );
    GetDlgItem( IDC_TEXT_4 )->ShowWindow( SW_SHOW );
    m_Text = _T("");
    text = _T("");

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
      tmpStr1.Format( _T("%d"), g_AlignerData.ObjNo[i] );
      tmpStr2 = g_AlignerData.Device[g_AlignerData.ObjNo[i]].Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
      AfxFormatString2( tmpStr, IDS_SETUP_PLANE_PLATFORMS, tmpStr1, tmpStr2 );
      m_Text += tmpStr + _T("\n");
      tmpStr.Format( _T("%.1f"), g_AlignerData.ZPar[g_AlignerData.ObjNo[i]] );
      RAdjText( &tmpStr, 4 );   
      text += tmpStr + _T("\n");
    }
    SetDlgItemText( IDC_TEXT_2, m_Text );
    GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_SHOW );
    SetDlgItemText( IDC_TEXT_3, text );
    GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_SHOW );
  }
}

void CAlignVerPage1::HideAllText()
{
  GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_TEXT_2 )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_TEXT_3 )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_TEXT_4 )->ShowWindow( SW_HIDE );
}

BEGIN_MESSAGE_MAP(CAlignVerPage1, CPropertyPage)
  ON_BN_CLICKED(IDC_ALIGN_VER_SETUP_TEST_PAGE_1, OnBnClickedAlignVerSetupTest)
  ON_BN_CLICKED(IDC_ALIGN_VER_HORIZON_RADIO, OnBnClickedAlignVerHorizonRadio)
  ON_BN_CLICKED(IDC_ALIGN_VER_BENCHMARK_RADIO, OnBnClickedAlignVerBenchmarkRadio)
END_MESSAGE_MAP()

// CAlignVerPage1 message handlers

BOOL CAlignVerPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  m_pParent = (CAlignmentWizard*)GetParent();
  ASSERT(m_pParent -> IsKindOf( RUNTIME_CLASS(CAlignmentWizard) ));
  m_MsgCaption = _T("");
  m_Text = _T("");
  m_SelectedTest = IDC_ALIGN_VER_HORIZON_RADIO;
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAlignVerPage1::OnSetActive() 
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
  HideAllText();
  BOOL okToContinue = FALSE;

  for( int i=0; i<SIZE_OF_ARRAYS; i++ )
  {
    if( (g_AlignerData.Incl[i] != INCL_EMPTY) && 
        (g_AlignerData.PlaneType[i] != PLANE_TYPE_FIXED) )
    {
      okToContinue = TRUE;
    }
  }

  if( okToContinue == FALSE )
  {
    g_AlignerData.TestNotSetup = TRUE;
    m_MsgCaption.LoadString( IDS_INFORMATION_CAPTION );
    m_Text.LoadString( IDS_SETUP_AT_LEAST_ONE_P_G_STATION );
    MessageBox( m_Text, m_MsgCaption, MB_ICONINFORMATION|MB_OK );
    EndDialog( IDCANCEL );
    return FALSE;
  }

  //if( g_AlignerData.TestNotSetup == TRUE )
  //{
  //  CSetupTestDlg SetupTestDlg;
  //  if( SetupTestDlg.DoModal() == IDCANCEL )
		//{
		//	EndDialog( IDCANCEL );
		//	return FALSE;
		//}
  //}

  //g_AlignerData.TestNotSetup = FALSE;

  //m_Text.LoadString( IDS_PLATFORM_TEST_SETUP );
  //SetDlgItemText( IDC_TEXT_1, m_Text );
  //GetDlgItem( IDC_TEXT_1 )->ShowWindow( SW_SHOW );

  //ShowPlatformTestSetup();

  #ifdef HPSW
    GetDlgItem( IDC_ALIGN_VER_BENCHMARK_RADIO )->EnableWindow( TRUE );
  #endif

  switch( m_SelectedTest )
  {
  #ifdef HPSW
    case IDC_ALIGN_VER_BENCHMARK_RADIO:
      CheckRadioButton( IDC_ALIGN_VER_HORIZON_RADIO, IDC_ALIGN_VER_BENCHMARK_RADIO, IDC_ALIGN_VER_BENCHMARK_RADIO );
      break;
  #endif
  case IDC_ALIGN_VER_HORIZON_RADIO:
  default:
    m_SelectedTest = IDC_ALIGN_VER_HORIZON_RADIO;
    CheckRadioButton( IDC_ALIGN_VER_HORIZON_RADIO, IDC_ALIGN_VER_BENCHMARK_RADIO, IDC_ALIGN_VER_HORIZON_RADIO );
    break;
  }

  UpdateData( FALSE );
  m_pParent->m_Status = STATUS_PAGE_READY;

  return CPropertyPage::OnSetActive();
}

BOOL CAlignVerPage1::OnQueryCancel()
{
  m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
  m_Text.LoadString( IDS_REALY_ABORT );
  return( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) );
}

LRESULT CAlignVerPage1::OnWizardNext()
{
  switch( m_pParent->m_Status )
  {
  case STATUS_PAGE_READY:

    switch( m_SelectedTest )
    {
    #ifdef HPSW
      case IDC_ALIGN_VER_BENCHMARK_RADIO:
        HideAllText();
        m_pParent->m_Status = STATUS_PAGE_NOT_READY; //next page not ready
        return IDD_ALIGN_VER_BENCHMARK_PAGE_1; //ID of the next page to jump
        break;
    #endif
    case IDC_ALIGN_VER_HORIZON_RADIO:
      HideAllText();
      m_pParent->m_Status = STATUS_PAGE_NOT_READY; //next page not ready
      return IDD_ALIGN_VER_HORIZON_PAGE_1; //ID of the next page to jump
      break;
    default: //should never happen
      m_SelectedTest = IDC_ALIGN_VER_HORIZON_RADIO;
      CheckRadioButton( IDC_ALIGN_VER_HORIZON_RADIO, IDC_ALIGN_VER_BENCHMARK_RADIO, IDC_ALIGN_VER_HORIZON_RADIO );
      break;
    }
    //return CPropertyPage::OnWizardNext();
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

void CAlignVerPage1::OnReset()
{
  m_pParent->m_Status = STATUS_PAGE_CANCELED;
  g_AlignerData.ErrorDef = ERR_CANCEL;
  return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CAlignVerPage1::OnBnClickedAlignVerSetupTest()
{
  CSetupTestDlg SetupTestDlg;
  SetupTestDlg.DoModal();
  ShowPlatformTestSetup();
}

void CAlignVerPage1::OnBnClickedAlignVerHorizonRadio()
{
  m_SelectedTest = IDC_ALIGN_VER_HORIZON_RADIO;
}

void CAlignVerPage1::OnBnClickedAlignVerBenchmarkRadio()
{
  m_SelectedTest = IDC_ALIGN_VER_BENCHMARK_RADIO;
}
