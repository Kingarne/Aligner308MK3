// $Id: MainFrm.cpp,v 1.8 2012/08/22 13:09:03 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "MainFrm.h"
#include "Splash.h"
#include ".\mainfrm.h"
#include "AlignerDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	//ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	//ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	//ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnHelpFinder)
    ON_WM_CLOSE()
    ON_COMMAND(ID_DEBUG_PANEL, OnDebugPanel)
    ON_MESSAGE(UM_SYNC_DIGITAL, OnSyncDigial)
    ON_MESSAGE(UM_DAC_TEST_ERR, OnDACTestErr)
	ON_MESSAGE(UM_DAU_STATUS, OnDAUStatus)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame( void )
{
  // Empty
}

CMainFrame::~CMainFrame( void )
{
  // Empty
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  if( theApp.IsAligner202Enabled() == TRUE )
  {
	  if (!m_wndToolBar.CreateEx(this) ||
		  !m_wndToolBar.LoadToolBar(IDR_MAINFRAME_A202))
	  {
		  return -1 ;
	  }
	  if (!m_wndDlgBar.Create(this, IDR_MAINFRAME_A202, 
		  CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	  {
		  return -1 ;
	  }
  }
  else
  {
	  if (!m_wndToolBar.CreateEx(this) ||
		  !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	  {
		  return -1 ;
	  }
	  if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		  CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	  {
		  return -1 ;
	  }
  }

	if (!m_wndReBar.Create( this ) ||
		!m_wndReBar.AddBar( &m_wndToolBar ) ||
		!m_wndReBar.AddBar( &m_wndDlgBar ))
	{
		return -1 ;
	}

	if (!m_wndStatusBar.Create(this))// ||	!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		return -1 ;
	}
	
	m_wndStatusBar.AddIndicator(I_INDICATOR_DAU_STATUS, "", NORMAL_COLOR, 25);	
	m_wndStatusBar.AddIndicator(I_EMPTY);	
	

	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY ) ;
	
    CSplashWnd::EnableSplashScreen(TRUE);
    CSplashWnd::ShowSplashScreen(this);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
  {
		return FALSE ;
  }
	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid( void ) const
{
	CFrameWnd::AssertValid() ;
}

void CMainFrame::Dump( CDumpContext &dc ) const
{
	CFrameWnd::Dump( dc ) ;
}

#endif //_DEBUG


void CMainFrame::OnClose( void )
{
  if (IDYES == ::AfxMessageBox( IDS_REALY_QUIT, MB_YESNO | MB_DEFBUTTON2 ))
  {
    CFrameWnd::OnClose() ;
  }
}

afx_msg void CMainFrame::OnHelpFinder()
{
	if( theApp.IsHelpEnabled() == TRUE )
	{
		CFrameWnd::OnHelpFinder();
	}
}

afx_msg void CMainFrame::OnHelp()
{
	if( theApp.IsHelpEnabled() == TRUE )
	{
		CFrameWnd::OnHelp();
	}
}

afx_msg void CMainFrame::OnContextHelp()
{
	if( theApp.IsHelpEnabled() == TRUE )
	{
		CFrameWnd::OnContextHelp();
	}
}



void CMainFrame::OnDebugPanel()
{
      
    if(m_debugDlg.m_hWnd == 0)
    {
        m_debugDlg.Create(IDD_DEBUG_DLG);
    }
 
    
    m_debugDlg.ShowWindow(m_debugDlg.IsWindowVisible()?SW_HIDE:SW_SHOW);
    

    TRACE("Debug\n");
}

LRESULT CMainFrame::OnSyncDigial( WPARAM, LPARAM )
{
    
    DAU::GetDAU().SyncDigital();
    
    return 0;
}

LRESULT CMainFrame::OnDACTestErr( WPARAM, LPARAM )
{
    ::AfxMessageBox("DAC-test error");
        
    return 0;
}

LRESULT CMainFrame::OnDAUStatus( WPARAM wp, LPARAM lp)
{
	CString str;
	int status = wp;
	int numFound = lp;
	static BOOL formerDAUStatus = TRUE;
	
	theApp.m_DAUPresent = (status == 0);

	if(theApp.m_DAUPresent)
	{		
		BOOL openResult = DAU::GetDAU().Setup();
		DAU::GetDAU().SetDACVoltage(32768);		
		if(numFound == 0)
		{
			//Do All the tests first time we find the DAU.
			Registry reg;
            if(reg.GetIntegerValue("DAU\\DoInitialTests", theApp.IsAligner308Enabled()?1:0))
			{              
				if(!DAU::GetDAU().m_comThr.LoopTest())
				{
					//Loop test failed
					TRACE("Looptest failed");
					::AfxMessageBox("Loop-test failed.");
				}
				DAU::GetDAU().m_comThr.BeginDACTest(PARTIAL_DAC_TEST);
			}
		}
	
		str.Format("%s %s (sn:%d)",GetDAUName(), openResult ? "Connected" : "Invalid key", DAU::GetDAU().GetSerial());
		m_wndStatusBar.SetPaneText(I_INDICATOR_DAU_STATUS, str);
		m_wndStatusBar.SetPaneTextColor(I_INDICATOR_DAU_STATUS, RGB(0,0,0));				
		m_wndStatusBar.SetPaneBGColor(I_INDICATOR_DAU_STATUS, openResult ? DAU_PRESENT_COLOR: DAU_INVALID_COLOR);
		
		CAlignerDoc *pDoc = static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(theApp.m_pMainWnd)->GetActiveDocument());
		pDoc->OpenConfig();
	}
	else
	{
		str.Format("%s Not Connected", GetDAUName());		
		m_wndStatusBar.SetPaneText(I_INDICATOR_DAU_STATUS, str);
		m_wndStatusBar.SetPaneTextColor(I_INDICATOR_DAU_STATUS, RGB(0,0,0));		
		m_wndStatusBar.SetPaneBGColor(I_INDICATOR_DAU_STATUS, DAU_NOT_PRESENT_COLOR);		

		if(formerDAUStatus == TRUE)
		{
			DAU::GetDAU().ClearConfig( );
			CAlignerDoc *pDoc = static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(theApp.m_pMainWnd)->GetActiveDocument());
			pDoc->OpenConfig();
		}
	}
	
	formerDAUStatus = (status == 0);
	//static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView)->Invalidate(FALSE) ;
	//Beep(800, 20);
	
	TRACE("DAU status %d\n",status);
	return 0;
}

