// $Id: ProgressWnd.cpp,v 1.4 2009/12/08 19:14:22 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "ProgressWnd.h"
#include ".\progresswnd.h"

IMPLEMENT_DYNAMIC(ProgressWnd, CDialog)

ProgressWnd::ProgressWnd(CWnd *pParent /*=NULL*/) : CDialog( ProgressWnd::IDD, pParent )
{

}

ProgressWnd::ProgressWnd(int steps, CString title, CWnd *pParent /*=NULL*/) : CDialog( ProgressWnd::IDD, pParent )
, m_info(_T(""))
, m_info2Text(_T(""))
{
  m_canceled = FALSE;
    m_steps = steps ;
  Create( ProgressWnd::IDD, pParent ) ;
  ShowWindow(SW_SHOW);
  SetWindowText(title);
  Step();
}

  ProgressWnd::~ProgressWnd( void )
{
  // Empty
}

void ProgressWnd::DoDataExchange( CDataExchange *pDX )
{
    CDialog::DoDataExchange( pDX ) ;
    DDX_Control( pDX, IDC_PROBE_PROGRESS, m_progressControl ) ;
    DDX_Text(pDX, IDC_INFO_TEXT, m_info);
    DDX_Text(pDX, IDC_INFO2_TEXT, m_info2Text);
}

BEGIN_MESSAGE_MAP(ProgressWnd, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL ProgressWnd::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  m_progressControl.SetRange32( 0, m_steps ) ;
  m_progressControl.SetStep( 1 ) ;
  
  return TRUE ;
}

void ProgressWnd::Init(CString title, int steps)
{
    SetSteps(steps);
    SetPos(0);
    SetWindowText(title);
    ShowWindow(SW_SHOW);
    Step();
    m_canceled = FALSE;
    m_elapsedTime = 0;
	CButton* pButt = (CButton*)GetDlgItem(IDC_AUTO_CLOSE_CHECK);
	pButt->SetCheck(true);
    m_timer = SetTimer(1, 1000, NULL);
}

void ProgressWnd::Done()
{
    if(m_timer)
    {
        KillTimer(m_timer);
        m_timer = 0;
    }
	
	CButton* pButt = (CButton*)GetDlgItem(IDC_AUTO_CLOSE_CHECK);
	if(pButt->GetCheck())
		ShowWindow(SW_HIDE);    
}

void ProgressWnd::SetElapsedTime(double elapsed)
{
    m_elapsedTime = elapsed;       
}

void ProgressWnd::SetSteps( int steps )
{
  m_steps = steps ;
  m_progressControl.SetRange32( 0, m_steps ) ;
}

void ProgressWnd::SetPos( int pos )
{
  m_progressControl.SetPos( pos ) ;
}
int ProgressWnd::Step( void )
{
  return m_progressControl.StepIt() ;
}

void ProgressWnd::OnBnClickedButton1()
{
    m_canceled = TRUE;
	EndDialog(IDCANCEL);
}

void ProgressWnd::SetInfoText(CString& str)
{
    m_info2Text = str;    
}

void ProgressWnd::OnTimer(UINT nIDEvent)
{
    int currPos = m_progressControl.GetPos();
    if(currPos==0)
        return;
    double factor = (double)m_steps/(double)currPos;
    double totalTime = (m_elapsedTime*factor);
    //int estimate = (int)totalTime-(int)(totalTime/factor);
    int estimate = (int)totalTime-(int)(m_elapsedTime);

    m_info.Format("Time remaining:%02d:%02d:%02d",estimate/3600, (estimate/60)%60, estimate%60);   
    TRACE("estimate:%d, total:%.2f, elapses:%.2f\n",estimate, totalTime,m_elapsedTime);
    UpdateData(FALSE);

    CDialog::OnTimer(nIDEvent);
}
