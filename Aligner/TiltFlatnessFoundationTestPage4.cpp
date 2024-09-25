// $Id: TiltFlatnessFoundationTestPage3.cpp,v 1.14 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "TiltFlatnessTestWizard.h"
#include "Util.h"
#include "AlignerDoc.h"
#include ".\tiltflatnessfoundationtestpage4.h"
#include <algorithm> 

IMPLEMENT_DYNAMIC(CTiltFlatnessFoundationTestPage4, CPropertyPage)

double GetLocalAz(double shipAz)
{
  return shipAz - FoundationStraightEdgeAngle;
}

double GetShipAz(double localAz)
{
  return localAz + FoundationStraightEdgeAngle;
}



CTiltFlatnessFoundationTestPage4::CTiltFlatnessFoundationTestPage4()
	: CPropertyPage(CTiltFlatnessFoundationTestPage4::IDD)
  , m_AzimuthAngle(0.0f)	
  , m_armLen(0.0f)
  , m_measInfo(_T(""))
  , m_topInfo(_T(""))
  , m_edgeDist(10.0f)
{
    m_measureRound = 1;
    m_measurmentNum = 0;
}

CTiltFlatnessFoundationTestPage4::~CTiltFlatnessFoundationTestPage4()
{
}

void CTiltFlatnessFoundationTestPage4::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT, m_AzimuthAngle);
  DDX_Text(pDX, IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_ARM_LEN_TEDIT, m_armLen);
  DDX_Control(pDX, IDC_MEAS_LIST, m_measList);
  DDX_Text(pDX, IDC_INFO_TEXT2, m_measInfo);
  DDX_Text(pDX, IDC_INFO_TEXT3, m_topInfo);
  DDX_Text(pDX, IDC_MARGIN_TEDIT, m_edgeDist);
}

void CTiltFlatnessFoundationTestPage4::ShowGraphButtons()
{
   
	
}

void CTiltFlatnessFoundationTestPage4::HideGraphButtons()
{
  
}

void CTiltFlatnessFoundationTestPage4::DisableAllButtons()
{   
	GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
	//GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->EnableWindow( FALSE );
}

void CTiltFlatnessFoundationTestPage4::InitResultTable( void )
{
	m_pParent->m_pResultTable->m_InParam.Version = MeasType::MT_TiltFlatnessFo;// TILT_FLATNESS_FOUNDATION_TEST;
//	m_pParent->m_pResultTable->m_InParam.Title.LoadString( IDS_TILT_FLATNESS_FOUNDATION_TEST );
	m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
    m_pParent->m_pResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
    m_pParent->m_pResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
    m_pParent->m_pResultTable->m_InParam.pACA = NULL;
	m_pParent->m_pResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pParent->m_pResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pParent->m_pResultTable->m_InParam.SavedToDataBase = FALSE;
	m_pParent->m_pResultTable->m_InParam.pTemperature = NULL;
	m_pParent->m_pResultTable->m_InParam.SamplingRate = 0;
    m_pParent->m_pResultTable->m_InParam.pFileGraphManager = &m_pParent->m_GraphFileManager;
}

BOOL CTiltFlatnessFoundationTestPage4::IsAlreadyMeasured( double newAngle )
{
	for( int i=1; i<=m_pParent->m_N; i++ )
	{
        if( m_pParent->m_XAngle[i] == GetShipAz(newAngle))
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BEGIN_MESSAGE_MAP(CTiltFlatnessFoundationTestPage4, CPropertyPage)
    ON_BN_CLICKED(IDC_START_MEASURE, OnBnClickedStartMeasure)    
    ON_BN_CLICKED(IDC_FINISH_MEASURE, OnBnClickedFinishMeasure)	
  ON_BN_CLICKED(IDC_BACK, &CTiltFlatnessFoundationTestPage4::OnBnClickedBack)
  ON_BN_CLICKED(IDC_FWD, &CTiltFlatnessFoundationTestPage4::OnBnClickedFwd)
    ON_EN_CHANGE(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_ARM_LEN_TEDIT, &CTiltFlatnessFoundationTestPage4::OnEnChangeTiltFlatnessFoundationTestPage3ArmLenTedit)
  ON_EN_KILLFOCUS(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT, &CTiltFlatnessFoundationTestPage4::OnEnKillfocusTiltFlatnessFoundationTestPage3AzimuthAngleTedit)
END_MESSAGE_MAP()


// CTiltFlatnessFoundationTestPage4 message handlers

BOOL CTiltFlatnessFoundationTestPage4::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    CString tmpStr;
	
	m_pParent = (CTiltFlatnessTestWizard*) GetParent();   
    m_MsgCaption = _T("");
    m_Text = _T("");

    m_Text.LoadString( IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE4_SET_INDEX_ARM );
    SetDlgItemText( IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_SET_INDEX_ARM, m_Text );
    m_Text.LoadString( IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE4_AZIMUTH_ANGLE );
    SetDlgItemText( IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE, m_Text );
    // Set default 0.0 in index arm field
	//tmpStr.Format( _T("%.1f"), 0.0f);
	//GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT)->SetWindowText(tmpStr);

    m_AzimuthAngle = 0.0f;
    m_armLen = GetArmLength(m_AzimuthAngle);

  m_measureWarping = (GetIndexArmLength(2) != 0);
	
  m_measList.SetExtendedStyle(m_measList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

  m_measList.InsertColumn(0, "#", LVCFMT_LEFT, 30);
  m_measList.InsertColumn(1, "Az.", LVCFMT_LEFT, 50);
  m_measList.InsertColumn(2, "Arm L.", LVCFMT_LEFT, 50);
  m_measList.InsertColumn(3, "Tilt", LVCFMT_LEFT, 50);
	
  CWnd* pWnd = GetDlgItem(IDC_INFO_TEXT2);
  pWnd->ShowWindow(SW_SHOW);

  m_Text.Format("Rectangular Foundation: (%d,%d) [mm]\r\nStraight edge angle offset: %.1f [deg]",g_AlignerData.FoundDim.cx, g_AlignerData.FoundDim.cy, FoundationStraightEdgeAngle);
  SetDlgItemText(IDC_INFO_TEXT2, m_Text);
  
  pWnd = GetDlgItem(IDC_INFO_TEXT3);
  pWnd->ShowWindow(SW_SHOW);
  m_topInfo = m_Text;

  UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CTiltFlatnessFoundationTestPage4::OnSetActive() 
{
  if( m_pParent == NULL )
  {
      m_MsgCaption.LoadString( IDS_ERROR_CAPTION );
      m_Text.LoadString( IDS_INTERNAL_MEASURE_PAGE_FAULT_CANT_CONTINUE );
      MessageBox( m_Text, m_MsgCaption, MB_ICONERROR|MB_OK );
      EndDialog( IDCANCEL );
      return( FALSE ); // prevent the page from being active
  }
	
  //GetDlgItem(IDC_ROUND_NUM_TEXT)->ShowWindow(m_measureWarping ? SW_SHOW: SW_HIDE);
    
  if( m_pParent->m_pResultTable == NULL )
	{
		m_pParent->m_pResultTable = new CResultTable( m_pParent );
	}
	InitResultTable();

  m_pParent->m_EnableMoveOfWizard = TRUE;
  m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
  HideAll();
	m_Repeat = FALSE;
  MeasureRollPathInit();

  m_pParent->m_Measure.m_InParam.Break = FALSE;

  ZeroMemory(m_pParent->m_XAngle, sizeof(m_pParent->m_XAngle));
  ZeroMemory(m_pParent->m_armLen, sizeof(m_pParent->m_armLen));  

  UpdateGUIStates();
    
  return CPropertyPage::OnSetActive();
}

BOOL CTiltFlatnessFoundationTestPage4::OnQueryCancel()
{
	if( m_pParent->m_MeasurementInterrupted == TRUE )
	{
		return( TRUE );
	}
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text.LoadString( IDS_EXIT_WITHOUT_SAVE );

    if( IDYES == MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) )
	  {
		  return( TRUE );
	  }
	  else
	  {
		  m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
		  CString tmpStr;
		  GetDlgItemText( IDC_INFO_TEXT, tmpStr );
		  if( tmpStr == m_Text )
		  {
			  //HideAllText();
			  m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
			  GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
			  GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
			  m_pParent->m_Measure.m_InParam.Break = TRUE; 
		  }
		  return( FALSE );
	  }
}

LRESULT CTiltFlatnessFoundationTestPage4::OnWizardBack() 
{
   return CPropertyPage::OnWizardBack();
}

BOOL CTiltFlatnessFoundationTestPage4::OnWizardFinish()
{
	switch( m_pParent->m_Status )
	{
		case STATUS_PAGE_READY:
		{			
			if (m_pParent->m_N > MIN_NO_OF_FLATNESS_MEASUREMENTS)
			{
				ShowErrorGraph();
			}

			ShowResultGraph();
			ShowPolarGraph();

			m_pParent->m_pResultTable->m_InParam.Time = m_pParent->m_MeasurementReadyTimeStamp;
			m_pParent->m_pResultTable->ShowReport();			
		
			return CPropertyPage::OnWizardFinish();
		}
    
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
    return FALSE;  // prevent the page from changing
}

void CTiltFlatnessFoundationTestPage4::OnReset()
{
    m_pParent->m_Status = STATUS_PAGE_CANCELED;
    g_AlignerData.ErrorDef = ERR_CANCEL;
   
    return CPropertyPage::OnReset(); //Calls OnCancel()
}

void CTiltFlatnessFoundationTestPage4::MeasureRollPathInitRound2()
{
    CString tmpStr;
    HideAllText();

		GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT)->ShowWindow(SW_HIDE);
	  tmpStr.LoadString(IDS_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_SET_INDEX_ARM_R2);
	  SetDlgItemText(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_SET_INDEX_ARM, tmpStr);

    int num = m_measurmentNum+1;

    tmpStr.Format( _T("Measurment no. % d(% d)"), num, m_pParent->m_N);
    SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, tmpStr );
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );

    tmpStr.Format( _T("%.1f"), m_pParent->m_XAngle[num] );
  //  SetDlgItemText(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_REAL_TEDIT, tmpStr );
  
    double arcAngle = m_pParent->m_XAngle[num] - FoundationStraightEdgeAngle;
    if(arcAngle < 0)
    {
        arcAngle += 360;
    }

    
    EnableStartButton();

	::PostMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_START_MEASURE)->m_hWnd, TRUE);
	GetDlgItem(IDC_START_MEASURE)->SetFocus();
}


void CTiltFlatnessFoundationTestPage4::MeasureRollPathInit()
{
    CString tmpStr;
    HideAllText();

    tmpStr.Format( _T("%d"), m_pParent->m_N+1 );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_NEXT_MEAS_NO, tmpStr );
    SetDlgItemText( IDC_NEXT_MEAS_NO_TEXT, m_Text );
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );
    
    EnableStartButton();
   
    ::PostMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT)->m_hWnd, TRUE); 
   
}

void CTiltFlatnessFoundationTestPage4::EnableStartButton()
{	
    m_Text.LoadString( IDS_START_MEASURE );
    SetDlgItemText( IDC_START_MEASURE, m_Text );
    GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_SHOW );
    GetDlgItem( IDC_START_MEASURE )->EnableWindow( TRUE );

	
	//::PostMessage(, WM_SETFOCUS, 0, 0);
	
}

CString CTiltFlatnessFoundationTestPage4::GetFinishButtonText()
{
    if(m_measureWarping && m_measureRound == 1)
    {
        return "Done Round";
    }

    return "Done";
}

void CTiltFlatnessFoundationTestPage4::OnBnClickedStartMeasure()
{
    if(m_measureWarping && m_measureRound == 2)
    {
        StartMeasureRound2();
    }
    else
    {
        StartMeasureRound1();
    }
}

void CTiltFlatnessFoundationTestPage4::SetWindowProp(int ctrl, BOOL show, CString text)
{
    GetDlgItem(ctrl)->ShowWindow( show ? SW_SHOW : SW_HIDE);
    if(text != "")
        SetDlgItemText( ctrl, m_Text );
}

void CTiltFlatnessFoundationTestPage4::SetWindowProp(int ctrl, BOOL show, int strId)
{
    GetDlgItem(ctrl)->ShowWindow( show ? SW_SHOW : SW_HIDE);
    if(strId != -1)
    {
        CString t;
        t.LoadString( strId );
        SetDlgItemText( ctrl, t);
    }
}


void CTiltFlatnessFoundationTestPage4::StartMeasureRound2()
{
    CString tmpStr, tmpStr1;
    m_pParent->SetWizardButtons( PSWIZB_DISABLEDFINISH );
    m_Text.LoadString( IDS_BREAK );
    GetDlgItemText( IDC_START_MEASURE, tmpStr );   

    if( tmpStr == m_Text )
    {
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
        m_pParent->m_Measure.m_InParam.Break = TRUE; 
        return;
    }

    GetDlgItem( IDC_FINISH_MEASURE )->ShowWindow( SW_HIDE );

    MoveDlgToRightSideOfApp(m_pParent );    
    HideGraphButtons();
    //GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->EnableWindow( FALSE );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
    SetDlgItemText( IDC_START_MEASURE, m_Text );
    m_Text.LoadString( IDS_MEASUREMENT_RUNNING );
    
    SetWindowProp(IDC_INFO_TEXT, true, m_Text);    
    SetWindowProp(IDC_INFO_TEXT, true, IDS_MEASUREMENT_RUNNING);

    tmpStr.Format( _T("%d"), m_measurmentNum+1 );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_MEAS_NO, tmpStr );
    SetWindowProp(IDC_NEXT_MEAS_NO_TEXT, true, m_Text);    
    
    m_pParent->m_EnableMoveOfWizard = FALSE;

    m_AzimuthAngle = m_pParent->m_XAngle[m_measurmentNum+1];
    g_AlignerData.Bear = m_AzimuthAngle;
    m_pParent->m_Xn = DEGREES_TO_RADIANS( m_AzimuthAngle );

    m_pParent->m_Measure.m_InParam.FiRef = IsFixed( g_AlignerData.RefObjNo ) ?  0 : m_pParent->m_Xn;
    m_pParent->m_Measure.m_InParam.rotateBackRef = m_pParent->m_Xn;

    //Only one object in foundation measurement
    m_pParent->m_Measure.m_InParam.FiObj[1] = IsFixed( g_AlignerData.ObjNo[1] ) ? 0 : m_pParent->m_Xn;
    m_pParent->m_Measure.m_InParam.rotateBack[1] = m_pParent->m_Xn;
    
    if( CallMeasure( m_pParent->m_dR, m_pParent->m_dP ) == FALSE )
    {
        if( m_Repeat == TRUE )
        {
            MeasureRollPathInitRound2();
        }
        else
        {
            RestoreGlobals();
            GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );

            SetWindowProp(IDC_INFO_TEXT, true, IDS_MEASUREMENT_INTERRUPTED);
            m_pParent->m_EnableMoveOfWizard = TRUE;
        }
        return; 
    }
    m_measurmentNum++;
    
	CString graphFileName;
	m_pParent->m_pGraph->SaveGraphToUniqueFileName(graphFileName);

	SetWindowProp(IDC_INFO_TEXT, true, IDS_MEASUREMENT_READY);
    m_pParent->m_EnableMoveOfWizard = TRUE;

    m_pParent->m_Y[2][m_measurmentNum] = m_pParent->m_dP[1];
    
    tmpStr.Format( _T("%d"), m_pParent->m_N );
    AfxFormatString1( m_Text, IDS_TILT_FLATNESS_TEST_N_MEAS_COMPLETED, tmpStr );

    if(m_measurmentNum < m_pParent->m_N)
    {
        MeasureRollPathInitRound2();
    }
    else
    {
        TRACE("Done!");
        RestoreGlobals();
        GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
        MeasureRollPathContinue();
    }

}


void CTiltFlatnessFoundationTestPage4::RestoreValues()
{
  m_AzimuthAngle = GetLocalAz(m_pParent->m_XAngle[m_measurmentNum + 1]);
  m_armLen = m_pParent->m_armLen[m_measurmentNum + 1];
  
      
  UpdateData(FALSE);
}


void CTiltFlatnessFoundationTestPage4::UpdateGUIStates()
{
  UpdateData(TRUE);

  CWnd* pWnd = GetDlgItem(IDC_BACK);
  pWnd->EnableWindow(m_measurmentNum > 0);

  pWnd = GetDlgItem(IDC_FWD);
  pWnd->EnableWindow(m_measurmentNum < m_pParent->m_N);  
  
  CString str;
  str.Format(_T("%d"), m_measurmentNum + 1);
  AfxFormatString1(m_Text, IDS_TILT_FLATNESS_TEST_MEAS_NO, str);
  SetDlgItemText(IDC_NEXT_MEAS_NO_TEXT, m_Text);
  GetDlgItem(IDC_NEXT_MEAS_NO_TEXT)->ShowWindow(SW_SHOW);

  bool opposite = m_measurmentNum % 2 != 0;
  GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_ARM_LEN_TEDIT)->EnableWindow(!opposite);
  GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT)->EnableWindow(!opposite);  

  m_measInfo = "";
  if (opposite)
  {
    m_measInfo.Format("Measure opposite side from Az: %.1f", GetLocalAz(m_pParent->m_XAngle[m_measurmentNum]));
    m_armLen = m_pParent->m_armLen[m_measurmentNum];
    m_AzimuthAngle = GetLocalAz(m_pParent->m_XAngle[m_measurmentNum])+180.0f;
    //SetDlgItemText(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_ARM_LEN_TEDIT, m_Text);
  }


  SetStartButtState();   
  UpdateMeasureList();
  
  UpdateData(FALSE);
}


void ScrollToBottom(CListCtrl& listCtrl)
{
  // Get the number of items in the list control
  int itemCount = listCtrl.GetItemCount();

  // Check if there are items in the list
  if (itemCount > 0)
  {
    // Scroll to the last item (itemCount - 1 is the index of the last item)
    listCtrl.EnsureVisible(itemCount - 1, FALSE);
  }
}

void CTiltFlatnessFoundationTestPage4::UpdateMeasureList()
{
  m_measList.DeleteAllItems();

  int i = 0;
  for (int i= 1; i<= m_pParent->m_N; i++) 
  {
    CString str;
    int item = i - 1;
    str.Format("%d", i);
    m_measList.InsertItem(item, str);
    str.Format("%.1f", GetLocalAz(m_pParent->m_XAngle[i]));
    m_measList.SetItemText(item, 1, str);
    str.Format("%.1f", m_pParent->m_armLen[i]);
    m_measList.SetItemText(item, 2, str);
    str.Format("%.4f", m_pParent->m_Y[1][i]);
    m_measList.SetItemText(item, 3, str);   
  }
  ScrollToBottom(m_measList);
}

void CTiltFlatnessFoundationTestPage4::StartMeasureRound1()
{
  UpdateData(TRUE);

  CString tmpStr, tmpStr1;
  m_pParent->SetWizardButtons(PSWIZB_DISABLEDFINISH);
  m_Text.LoadString(IDS_BREAK);
  GetDlgItemText(IDC_START_MEASURE, tmpStr);

  if (tmpStr == m_Text)
  {
    GetDlgItem(IDC_START_MEASURE)->EnableWindow(FALSE);
    GetDlgItem(IDC_INTRO_TEXT)->ShowWindow(SW_HIDE);
    m_pParent->m_Measure.m_InParam.Break = TRUE;
    return;
  }

  if (IsAlreadyMeasured(m_AzimuthAngle) == TRUE)
  {
    m_MsgCaption.LoadString(IDS_WARNING_CAPTION);
    tmpStr.Format(_T("%.1f"), m_AzimuthAngle);
    AfxFormatString1(tmpStr1, IDS_ANGLE_ALREADY_MEASURED_AGAIN, tmpStr);

    if (IDCANCEL == MessageBox(tmpStr1, m_MsgCaption, MB_ICONEXCLAMATION | MB_OKCANCEL))
      return;
  }

   GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_HIDE);

  MoveDlgToRightSideOfApp(m_pParent);
  HideGraphButtons();
  //GetDlgItem(IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE)->EnableWindow(FALSE);
  GetDlgItem(IDC_INTRO_TEXT)->ShowWindow(SW_HIDE);
  SetDlgItemText(IDC_START_MEASURE, m_Text);
  SetWindowProp(IDC_INFO_TEXT, true, IDS_MEASUREMENT_RUNNING);
  m_pParent->m_EnableMoveOfWizard = FALSE;

  
  g_AlignerData.Bear = m_AzimuthAngle;
  m_pParent->m_Xn = DEGREES_TO_RADIANS(m_AzimuthAngle);

  m_pParent->m_Measure.m_InParam.FiRef = IsFixed(g_AlignerData.RefObjNo) ? 0 : m_pParent->m_Xn;
  m_pParent->m_Measure.m_InParam.rotateBackRef = m_pParent->m_Xn;

  //Only one object in foundation measurement
  m_pParent->m_Measure.m_InParam.FiObj[1] = IsFixed(g_AlignerData.ObjNo[1]) ? 0 : m_pParent->m_Xn;
  m_pParent->m_Measure.m_InParam.rotateBack[1] = m_pParent->m_Xn;

  if (CallMeasure(m_pParent->m_dR, m_pParent->m_dP) == FALSE)
  {
    if (m_Repeat == TRUE)
    {
      MeasureRollPathInit();
    }
    else
    {
      RestoreGlobals();
      GetDlgItem(IDC_START_MEASURE)->EnableWindow(FALSE);

      m_Text.LoadString(IDS_MEASUREMENT_INTERRUPTED);
      SetDlgItemText(IDC_INFO_TEXT, m_Text);
      GetDlgItem(IDC_INFO_TEXT)->ShowWindow(SW_SHOW);
      m_pParent->m_EnableMoveOfWizard = TRUE;
    }
    return;
  }
 
  m_measurmentNum++;
  if (m_measurmentNum > m_pParent->m_N)
    m_pParent->m_N = m_measurmentNum;

  m_pParent->m_XAngle[m_measurmentNum] = GetShipAz(m_AzimuthAngle);
  m_pParent->m_armLen[m_measurmentNum] = m_armLen;

 
  CString graphFileName;
  m_pParent->m_pGraph->SaveGraphToUniqueFileName(graphFileName);

  m_Text.LoadString(IDS_MEASUREMENT_READY);
  SetDlgItemText(IDC_INFO_TEXT, m_Text);
  GetDlgItem(IDC_INFO_TEXT)->ShowWindow(SW_SHOW);
  m_pParent->m_EnableMoveOfWizard = TRUE;

  m_pParent->m_Y[1][m_measurmentNum] = m_pParent->m_dP[1];

  tmpStr.Format(_T("%d"), m_measurmentNum);
  AfxFormatString1(m_Text, IDS_TILT_FLATNESS_TEST_N_MEAS_COMPLETED, tmpStr);

  if ((m_measurmentNum >= MIN_NO_OF_FLATNESS_MEASUREMENTS) && (m_measurmentNum < MAX_NO_OF_FLATNESS_MEASUREMENTS) && m_measurmentNum%2==0)
  {
    SetDlgItemText(IDC_FINISH_MEASURE, GetFinishButtonText());
    GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow(SW_SHOW);
  }
  else if (m_measurmentNum >= MAX_NO_OF_FLATNESS_MEASUREMENTS)
  {
    m_MsgCaption.LoadString(IDS_INFORMATION_CAPTION);
    tmpStr.LoadString(IDS_TILT_FLATNESS_TEST_NO_MORE_MEASUREMENTS);
    m_Text += tmpStr;
    MessageBox(m_Text, m_MsgCaption, MB_ICONINFORMATION | MB_OK);
    m_pParent->m_Leave = TRUE;
  }

  if (m_pParent->m_Leave == FALSE)
  {
    MeasureRollPathInit();
  }
  else
  {
    RestoreGlobals();
    GetDlgItem(IDC_START_MEASURE)->EnableWindow(FALSE);
    MeasureRollPathContinue();
  }

  UpdateGUIStates();
}

void CTiltFlatnessFoundationTestPage4::RestoreGlobals()
{
  g_AlignerData.UseElevError = m_pParent->m_Measure.m_InParam.UseElev;
}

BOOL CTiltFlatnessFoundationTestPage4::CallMeasure( double *pRoll, double *pPitch )
{
	m_Repeat = FALSE;
 
    SetupSensors(m_pParent->m_Measure.m_InParam);
    
    do
    {
        m_pParent->m_Measure.Measure();

		if( g_AlignerData.ErrorDef == ERR_CANCEL )
		{
			return( FALSE );
		}
          
        if( ( g_AlignerData.ErrorFlag == TRUE ) || ( g_AlignerData.Reject == TRUE ) )
        {
            m_pParent->m_Measure.m_InParam.Break = FALSE;
            m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
            m_Text.LoadString( IDS_MEASUREMENT_INTERRUPTED_REPEAT_IT );

            if( DAU::GetDAU().IsOpen() == FALSE || MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDYES )
            {
                if( m_pParent->m_pSource != NULL )
                {
                    m_pParent->m_pSource->Start();
                }
                else
                {
                    DAU::GetDAU().Start();
                }
                m_Repeat = TRUE;
            }
			else
			{
				m_pParent->m_MeasurementInterrupted = TRUE;
			}
			return FALSE;
        }
    }while( ( g_AlignerData.ErrorFlag == TRUE ) || ( g_AlignerData.Reject == TRUE ) );

    //Now the measurement is ready, get the timestamp to be stored in the graph and table
    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_pParent->m_MeasurementReadyTimeStamp );

    memcpy( pRoll, m_pParent->m_Measure.m_OutParam.DeltaRoll, SIZE_OF_ARRAYS * sizeof( double ) );
    memcpy( pPitch, m_pParent->m_Measure.m_OutParam.DeltaPitch, SIZE_OF_ARRAYS * sizeof( double ) );
	  CopyTemperature( m_pParent->m_Measure.m_OutParam.TempT );

    return TRUE;
}

void CTiltFlatnessFoundationTestPage4::SimulData()
{
  m_pParent->m_XAngle[0] = 0; //Not used
  m_pParent->m_XAngle[1] = 0;
  m_pParent->m_XAngle[2] = 30;
  m_pParent->m_XAngle[3] = 60;
  m_pParent->m_XAngle[4] = 90;
  m_pParent->m_XAngle[5] = 120;
  m_pParent->m_XAngle[6] = 150;
  m_pParent->m_XAngle[7] = 180;
  m_pParent->m_XAngle[8] = 210;
  m_pParent->m_XAngle[9] = 240;
  m_pParent->m_XAngle[10] = 270;
  m_pParent->m_XAngle[11] = 300;
  m_pParent->m_XAngle[12] = 330;

  m_pParent->m_armLen[0] = 0;
  m_pParent->m_armLen[1] = 500.00;
  m_pParent->m_armLen[2] = 577.35;
  m_pParent->m_armLen[3] = 1154.70;
  m_pParent->m_armLen[4] = 1000.00;
  m_pParent->m_armLen[5] = 1154.70;
  m_pParent->m_armLen[6] = 577.35;
  m_pParent->m_armLen[7] = 500.00;
  m_pParent->m_armLen[8] = 577.35;
  m_pParent->m_armLen[9] = 1154.70;
  m_pParent->m_armLen[10] = 1000.00;
  m_pParent->m_armLen[11] = 1154.70;
  m_pParent->m_armLen[12] = 577.35;
     
  m_pParent->m_Y[1][0] = 0;
  m_pParent->m_Y[1][1] = -0.00800000;
  m_pParent->m_Y[1][2] = -0.00366025;
  m_pParent->m_Y[1][3] = 0.00366025;
  m_pParent->m_Y[1][4] = 0.01100000;
  m_pParent->m_Y[1][5] = 0.01366025;
  m_pParent->m_Y[1][6] = 0.01366025;
  m_pParent->m_Y[1][7] = 0.01200000;
  m_pParent->m_Y[1][8] = 0.00366025;
  m_pParent->m_Y[1][9] = -0.00366025;
  m_pParent->m_Y[1][10] = -0.00900000;
  m_pParent->m_Y[1][11] = -0.01366025;
  m_pParent->m_Y[1][12] = -0.01366025;

  m_pParent->m_N = 12;
}


void CTiltFlatnessFoundationTestPage4::MeasureRollPathContinue()
{
	double Y, MeanSineFit, VecAmp, VecArg, CosXAngleMinFi, VecAmp_Mul_CosXAngleMinFi, XAngleDeg, XAngleRad, Fi, XAngleMinFi, SineFitError ;
  double m_U[SIZE_OF_YT_MATRIX_Y_SIZE];
  m_pParent->m_Ymax = -10000;
  m_pParent->m_Ymin = 10000;
  ZeroMemory(&(m_pParent->m_MeanSineFit), sizeof(m_pParent->m_MeanSineFit)); 

  //SimulData();

  double meanSinFit;
  SineFitFo2(m_pParent->m_XAngle, m_pParent->m_armLen, m_pParent->m_Y, 0, 1, m_pParent->m_N, &(g_AlignerData.ACR[1]), &(g_AlignerData.ACP[1]), &meanSinFit, &(m_U[0]));
  SineFitFo2(m_pParent->m_XAngle, m_pParent->m_armLen, m_pParent->m_Y, meanSinFit, 1, m_pParent->m_N, &(g_AlignerData.ACR[1]), &(g_AlignerData.ACP[1]), &meanSinFit, &(m_U[0]));  
  CartToVec( g_AlignerData.ACR[1], g_AlignerData.ACP[1], &(g_AlignerData.VecAmp[1]), &(g_AlignerData.VecArg[1]) );
  m_pParent->m_Fi[1] = DEGREES_TO_RADIANS( g_AlignerData.VecArg[1] );
    
  VecAmp = g_AlignerData.VecAmp[1];
  VecArg = g_AlignerData.VecArg[1];
  Fi = m_pParent->m_Fi[1] ;

  for( int i=1; i<=m_measureRound; i++ )
  {
        m_pParent->m_MeanSineFit[i] = meanSinFit;// / (GetIndexArmLength(i) / 1000.0f);
        for( int j=1; j<=m_pParent->m_N; j++ )
        {   
         
            double G = VecAmp * cos(M_PI / 180 * m_pParent->m_XAngle[j] - M_PI / 180 * VecArg);
            m_pParent->m_SineFitError[i][j] = (m_pParent->m_armLen[j] * (m_U[j] - G))/1000.0f;

            if( fabs( m_pParent->m_SineFitError[i][j] ) > fabs( m_pParent->m_MaxSineFitError[i] ) )
            {
                m_pParent->m_MaxSineFitError[i] = m_pParent->m_SineFitError[i][j];
                m_pParent->m_XAngleForMaxSineFitError[i] = m_pParent->m_XAngle[j];
            }
            m_pParent->m_Eave[1] += m_pParent->m_SineFitError[i][j];
            m_pParent->m_Esquare[1] += SQUARE( m_pParent->m_SineFitError[i][j] );

            //	Set max and min
            if( fabs( m_pParent->m_MaxSineFitError[i]) > m_pParent->m_ERan )
            {
                m_pParent->m_ERan = fabs( m_pParent->m_MaxSineFitError[i] );
            }
        }     
    }
        
    g_AlignerData.Sigma[1] = sqrt( m_pParent->m_Esquare[1] / ( (m_pParent->m_N*m_measureRound) - 1 ) );
    
    double minR1 = *min_element(&m_pParent->m_SineFitError[1][1], &m_pParent->m_SineFitError[1][m_pParent->m_N+1]);        
    m_pParent->m_HBottom = minR1;
    
    GetDlgItem( IDC_FINISH_MEASURE )->ShowWindow( SW_HIDE );
    
    m_pParent->SetWizardButtons( PSWIZB_FINISH );
}


void CTiltFlatnessFoundationTestPage4::HideAllText()
{
    GetDlgItem( IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INTRO_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_ENTER_ANGLE_TEXT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_INFO_TEXT )->ShowWindow( SW_HIDE );
}

void CTiltFlatnessFoundationTestPage4::HideAll()
{
    HideAllText();
    //GetDlgItem( IDC_TILT_FLATNESS_TEST_PAGE3_AZIMUTH_ANGLE )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_START_MEASURE )->ShowWindow( SW_HIDE );
   
}
 
	/***************************************************************************/
	/*													ShowResultGraph											 */
	/***************************************************************************/
void CTiltFlatnessFoundationTestPage4::ShowResultGraph()
{
	if(m_measureWarping)
        return;
    
    //GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
    UnitModeT UMode;
    CString str;
    TiltFlatnessFoTestGraphInParam graphParams;
    graphParams.PlMode = Sine;
    graphParams.mode = RPath;
    graphParams.RefNo = g_AlignerData.RefObjNo;
    graphParams.timeConstant = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.Points = m_pParent->m_N;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
    graphParams.IndexArmLength = GetIndexArmLength(1) ;

    m_pParent->m_pGraph->InitDefaultPlotGraph( 2 * g_AlignerData.NoOfCorr );
    //modify the default plot graph for tilt&flatness
    m_pParent->m_pGraph->SetUseCommonXValues( FALSE );
    m_pParent->m_pGraph->SetAxisXMin( 0 );
    m_pParent->m_pGraph->SetAxisXMax( 360 );
    str.LoadString( IDS_DEG_UNIT );
    m_pParent->m_pGraph->SetAxisXUnit( &str );

    if( GetMRad() == TRUE )
    {
        UMode = mr;
        str.LoadString( IDS_MRAD_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
    }
    else
    {
        UMode = arcmin;
        str.LoadString( IDS_ARCMIN_UNIT );
        m_pParent->m_pGraph->SetAxisYUnit( &str );
    }
  

    int k = 1;
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_pGraph->SetLinePattern( k, oc2dLineNone );
        m_pParent->m_pGraph->SetPointShape( k,  m_pParent->m_pGraph->GetPointShape( i ) );
        m_pParent->m_pGraph->SetPointColor( k, m_pParent->m_pGraph->GetColor( i ) );
		    m_pParent->m_pGraph->SetSerieLabel( k, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
        k++;
        m_pParent->m_pGraph->SetLinePattern( k, oc2dLineSolid );//sineFit
        m_pParent->m_pGraph->SetLineColor( k, m_pParent->m_pGraph->GetColor( i ) );//sineFit
        m_pParent->m_pGraph->SetPointShape( k, oc2dShapeNone );//sineFit
        m_pParent->m_pGraph->SetSerieLabel( k, _T(" ") );//sineFit
        k++;
    }

    double yPlot;
    //set tilt&flatness data for the graph
    for( int j=1; j<=m_pParent->m_N; j++ )
    {
        k = 1;
//         for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
//         {
			    /***  The code below replaces the out-commented code below  ***/
			/***  Y[i,j] already scaled with arcmin factor .Kh  ***/
         yPlot = m_pParent->m_Y[1][j];
			/***  Initially, g_AlignerData.Kh is set = 1 or = MTA (3,4..) if mrad or arcmin is selected, respectively  ***/
			
		//if( UMode == mr )
   //   {
   //     yPlot = m_pParent->m_Y[i][j];
   //   }
   //   else
   //   { //UMode == arcmin
   //     yPlot = m_pParent->m_Y[i][j] * g_AlignerData.Kh;
   //   }
        m_pParent->m_pGraph->SetXYData( k, m_pParent->m_XAngle[j], yPlot );
        k += 2;
//        }
        m_pParent->m_pGraph->UpdateData(FALSE);
    }

    //set the sineFit data for the graph
    for( int deg=0; deg<=360; deg++ )
    {
        k = 2;
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {
            m_pParent->m_F = m_pParent->m_MeanSineFit[i] + g_AlignerData.VecAmp[i] * cos( DEGREES_TO_RADIANS( deg ) - m_pParent->m_Fi[i] );
            yPlot = m_pParent->m_F;
            m_pParent->m_pGraph->SetXYData( k, deg, yPlot );
            k += 2;
        }
        m_pParent->m_pGraph->UpdateData(FALSE);
    }
        
    m_pParent->m_pGraph->ShowTiltFlatnessFoTestGraphWithText( &graphParams );
	m_pParent->SaveResultGraphFile();
}

	/***************************************************************************/
	/*													ShowErrorGraph											 */
	/***************************************************************************/
void CTiltFlatnessFoundationTestPage4::ShowErrorGraph()
{
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );
    UnitModeT UMode;
    int YRan;
    CString str;
    TiltFlatnessFoTestGraphInParam graphParams;
    graphParams.PlMode = Linear;
    graphParams.mode = RPath;
    graphParams.RefNo = g_AlignerData.RefObjNo;
    graphParams.timeConstant = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.Points = m_pParent->m_N;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );

    m_pParent->m_pGraph->InitDefaultPlotGraph( m_measureRound );
    //modify the default plot graph for tilt&flatness
    m_pParent->m_pGraph->SetUseCommonXValues( TRUE );
    m_pParent->m_pGraph->SetAxisXMin( 0 );
    m_pParent->m_pGraph->SetAxisXMax( 360 );  
    str.LoadString( IDS_DEG_UNIT );
    m_pParent->m_pGraph->SetAxisXUnit( &str );

    if( ( GetMRad() == TRUE ) || ( g_AlignerData.Found == TRUE ) )
    {
        YRan = RoundToInt( 2000 * m_pParent->m_ERan + 1.5 );

        if( YRan > 2000 )
        {
            YRan = RoundToInt( YRan / 1000 +1.5 );

            if( g_AlignerData.Found == TRUE )
            {
                UMode = mm;
                str.LoadString( IDS_MM_UNIT );
                m_pParent->m_pGraph->SetAxisYUnit( &str );
            }
            else
            {
                UMode = mr;
                str.LoadString( IDS_MRAD_UNIT );
                m_pParent->m_pGraph->SetAxisYUnit( &str );
            }
        }
        else
        {
        if( g_AlignerData.Found == TRUE )
        {
            UMode = um;
            str.LoadString( IDS_UM_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        else
        {
            UMode = ur;
            str.LoadString( IDS_URAD_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        }
    }
    else
    {
        YRan = RoundToInt( 120 * g_AlignerData.Kh * m_pParent->m_ERan + 1.5 );
        if( YRan > 500 )
        {
            YRan = RoundToInt( YRan / 60 + 1.5 );
            UMode = arcmin;
            str.LoadString( IDS_ARCMIN_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
        else
        {
            UMode = arcsec;
            str.LoadString( IDS_ARCSEC_UNIT );
            m_pParent->m_pGraph->SetAxisYUnit( &str );
        }
    }

    CString round;
    for( int i=1; i<=m_measureRound; i++ )
    {
        m_pParent->m_pGraph->SetLinePattern( i, oc2dLineNone );
        m_pParent->m_pGraph->SetPointShape( i,  m_pParent->m_pGraph->GetPointShape( i ) );
        m_pParent->m_pGraph->SetPointColor( i, m_pParent->m_pGraph->GetColor( i ) );
		round.Format("-R%d",i);
        m_pParent->m_pGraph->SetSerieLabel( i, GetUnitTypeDescription( g_AlignerData.ObjNo[1] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL-3 ) + round);
    }

    double yPlot;

    //set tilt&flatness data for the graph
  
        for( int j=1; j<=m_pParent->m_N; j++ )
        {
            for( int i=1; i<=m_measureRound; i++ )
            {         
			/***  1.14 ; The code below replaces the out-commented code below  ***/
			/***  SineFitError already scaled with arcmin factor .Kh  ***/
//			yPlot = m_pParent->m_SineFitError[i][j] * g_AlignerData.Kh;
			yPlot = m_pParent->m_SineFitError[i][j] ;
			yPlot = yPlot / g_AlignerData.Kh ;
			if ((UMode == ur) || (UMode == um))
				yPlot = yPlot * 1000 ;
			if (UMode == arcsec)
				yPlot = yPlot * 60 ;
			/***  Initially, g_AlignerData.Kh is set = 1 or = MTA (3,4..) if mrad or arcmin is selected, respectively  ***/
            m_pParent->m_pGraph->SetXYData( i, m_pParent->m_XAngle[j], yPlot );
        }
        m_pParent->m_pGraph->UpdateData(FALSE);
    }

    m_pParent->m_pGraph->ShowTiltFlatnessFoTestGraphWithText( &graphParams );
    m_pParent->SaveErrorGraphFile();
}

	/***************************************************************************/
	/*													ShowPolarGraph											 */
	/***************************************************************************/
void CTiltFlatnessFoundationTestPage4::ShowPolarGraph()
{
	//GetDlgItem( IDC_SAVE_GRAPH )->ShowWindow( SW_HIDE );

    //init the graph
    m_pParent->m_pGraph->InitDefaultPolarGraph( g_AlignerData.NoOfCorr );

    //set data and labels
    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        m_pParent->m_pGraph->SetXYData( i, g_AlignerData.VecArg[i], g_AlignerData.VecAmp[i] );
		    m_pParent->m_pGraph->SetSerieLabel( i, GetUnitTypeDescription( g_AlignerData.ObjNo[i] ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
    }
    m_pParent->m_pGraph->UpdateData(FALSE);
    PolarGraphInParam graphParams;
    graphParams.mode = m_pParent->m_Measure.m_InParam.Mode1;
    graphParams.tao = m_pParent->m_Measure.m_InParam.Tao;
    graphParams.RefNo = m_pParent->m_Measure.m_InParam.RefObjNo;
    graphParams.TimeStamp = GetTimeStampStr( &(m_pParent->m_MeasurementReadyTimeStamp), TRUE );
    graphParams.DateStamp = GetDateStampStr( &(m_pParent->m_MeasurementReadyTimeStamp) );
    graphParams.commonFlatTest = FALSE;
    m_pParent->m_pGraph->ShowPolarGraphWithText( &graphParams );
	m_pParent->SavePolarGraphFile();
}


void CTiltFlatnessFoundationTestPage4::SetStartButtState()
{
  //UpdateData(TRUE);

	bool enable = true;
	CString tmpStr;

	GetDlgItem(IDC_TILT_FLATNESS_FOUNDATION_TEST_PAGE3_AZIMUTH_ANGLE_TEDIT)->GetWindowText(tmpStr);
	if (tmpStr == "")
		enable = false;

	if(m_armLen <= 0 && g_AlignerData.FoundationType == FoundationT::Rectangular)
    enable = false;


	GetDlgItem(IDC_START_MEASURE)->EnableWindow(enable);
}

void CTiltFlatnessFoundationTestPage4::OnEnChangeTiltFlatnessFoundationTestPage3ArmLenTedit()
{
  UpdateData(TRUE);
  SetStartButtState();
}


double CTiltFlatnessFoundationTestPage4::GetArmLength(double az)
{
  double lengthY = g_AlignerData.FoundDim.cy / 2 - m_edgeDist;
  double lengthX = g_AlignerData.FoundDim.cx / 2 - m_edgeDist;

  double armY = fabs(lengthY / cos(DEGREES_TO_RADIANS(az)));
  double armX = fabs(lengthX / cos(DEGREES_TO_RADIANS(90.0f-az)));

  TRACE("az: %.1f, armY:%.2f, armX:%.2f ->",az,  armY, armX);

  return min(armY, armX);
}

void CTiltFlatnessFoundationTestPage4::OnEnKillfocusTiltFlatnessFoundationTestPage3AzimuthAngleTedit()
{
  UpdateData(TRUE);
  SetStartButtState();
  
  m_armLen = std::round(GetArmLength(m_AzimuthAngle));
  TRACE("(%d) Real arm Length:%.1f\n", m_AzimuthAngle, m_armLen);

     
  UpdateData(FALSE);
}


void CTiltFlatnessFoundationTestPage4::OnBnClickedFinishMeasure()
{
    m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
    m_Text = "Are you sure you want to finish measurement?";
    if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 ) == IDYES )
    {
        if(m_measureWarping && m_measureRound == 1)
        {                        
            m_measureRound = 2;
            m_measurmentNum = 0;
            MeasureRollPathInitRound2();
                   
            //GetDlgItem(IDC_ROUND_NUM_TEXT)->SetWindowText("Round 2 of 2");
            GetDlgItem(IDC_NEXT_MEAS_NO_TEXT )->ShowWindow( SW_SHOW );
            GetDlgItem(IDC_FINISH_MEASURE)->ShowWindow( SW_HIDE );
            
        }
        else
        {
            RestoreGlobals();
            GetDlgItem( IDC_START_MEASURE )->EnableWindow( FALSE );
            MeasureRollPathContinue();
        }               
    }
}


void CTiltFlatnessFoundationTestPage4::OnBnClickedBack()
{
  if (m_measurmentNum <= 0)
    return;
  
  m_measurmentNum--;

  RestoreValues();
  UpdateGUIStates();
}


void CTiltFlatnessFoundationTestPage4::OnBnClickedFwd()
{
  if (m_measurmentNum >= m_pParent->m_N)
    return;

  m_measurmentNum++;
  
  RestoreValues();
  UpdateGUIStates();
}





