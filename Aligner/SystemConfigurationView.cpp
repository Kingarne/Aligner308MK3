// $Id: SystemConfigurationView.cpp,v 1.10 2012/08/10 11:31:42 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SystemConfigurationView.h"
#include "Util.h"
#include "SystemSetup.h"

IMPLEMENT_DYNCREATE(SystemConfigurationView, CFormView)

SystemConfigurationView::SystemConfigurationView() : CFormView( theApp.IsAligner202Enabled()==TRUE ? SystemConfigurationView::IDD_A202 : SystemConfigurationView::IDD_A308)
{
  m_timerId = 1 ;
}

SystemConfigurationView::~SystemConfigurationView()
{
}

void SystemConfigurationView::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX) ;

    DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_PROJECT, m_GeneralProject);
	DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_SHIP, m_GeneralShip);
	DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_OPERATOR, m_GeneralOperator);
	DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_DAU_SN, m_GeneralDauSN);
	DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_DATE, m_GeneralDate);
	DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_PLACE, m_GeneralPlace);
   
    if( theApp.IsAligner202Enabled() == FALSE )
    {
        DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_UNIT, m_GeneralUnit);
        DDX_Control(pDX, IDC_SENSOR_GRID, m_sensorGrid);        
        DDX_Control(pDX, IDC_GYRO_GRID, m_gyroGrid);    
        DDX_Control(pDX, IDC_SYSTEM_CONFIG_GENERAL_LATITUDE, m_GeneralLatitude);
    }
    else
    {
        DDX_Control(pDX, IDC_SENSOR_GRID, m_channelGrid);        
    }


    if( theApp.IsAligner202Enabled() == FALSE )
    {
	   /* DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_1, m_SyncroNominalAzimuth[0]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_2, m_SyncroNominalAzimuth[1]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_3, m_SyncroNominalAzimuth[2]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_4, m_SyncroNominalAzimuth[3]);
      */
    }
    else
    {

    }
}

void SystemConfigurationView::ShowAll( int nCmdShow )
{
	if( nCmdShow == SW_SHOW )
	{
		UpdateGeneralConfiguration();
		UpdateSensorConfiguration();
		UpdateSyncroConfiguration();
	}
	ShowGeneralConfiguration( nCmdShow );
	ShowSensorConfiguration( nCmdShow );
	ShowSyncroConfiguration( nCmdShow );
  ShowInfo(nCmdShow);
}

void SystemConfigurationView::ShowGeneralConfiguration( int nCmdShow )
{
	if( nCmdShow == SW_SHOW )
	{
		UpdateGeneralConfiguration();
	}
	GetDlgItem( IDC_SYSTEM_CONFIG_TEXT )->ShowWindow( nCmdShow );
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_BOUNDARY )->ShowWindow( nCmdShow );
    GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_PROJECT_TEXT )->ShowWindow( nCmdShow );
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_SHIP_TEXT )->ShowWindow( nCmdShow );
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_OPERATOR_TEXT )->ShowWindow( nCmdShow );    
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_DAU_SN_TEXT )->ShowWindow( nCmdShow );
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_DATE_TEXT )->ShowWindow( nCmdShow );
	GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_PLACE_TEXT )->ShowWindow( nCmdShow );

	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT)
	{
		GetDlgItem(IDC_SYSTEM_CONFIG_GENERAL_SHIP_TEXT)->SetWindowText("Ship:");
	}
	else if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_CALIBRATION)
	{
		GetDlgItem(IDC_SYSTEM_CONFIG_GENERAL_SHIP_TEXT)->SetWindowText("Platform:");
	}


    if( theApp.IsAligner202Enabled() == FALSE )
    {
	    GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_LATITUDE_TEXT )->ShowWindow( nCmdShow );
        GetDlgItem( IDC_SYSTEM_CONFIG_GENERAL_UNIT_TEXT )->ShowWindow( nCmdShow );    
        m_GeneralLatitude.ShowWindow( nCmdShow );
        m_GeneralUnit.ShowWindow( nCmdShow );
    }
    m_GeneralProject.ShowWindow( nCmdShow );
	m_GeneralShip.ShowWindow( nCmdShow );
	m_GeneralOperator.ShowWindow( nCmdShow );
	m_GeneralDauSN.ShowWindow( nCmdShow );
	m_GeneralDate.ShowWindow( nCmdShow );
	m_GeneralPlace.ShowWindow( nCmdShow );
    	
	ShowSensorConfiguration( SW_HIDE );
	ShowSyncroConfiguration( SW_HIDE );
  ShowInfo(SW_HIDE);
 // GetDlgItem(IDC_SYSTEM_CONFIG_INFO_BOUNDARY)->ShowWindow(nCmdShow);
}

void SystemConfigurationView::ShowInfo(int show)
{
 
  CWnd* pInfo = GetDlgItem(IDC_SYSTEM_INFO_BOUNDARY);
  pInfo->ShowWindow(show);
  m_descBox1.ShowWindow(show);
  m_desc1.ShowWindow(show);
  m_descBox2.ShowWindow(show);
  m_desc2.ShowWindow(show);
  m_descBox3.ShowWindow(show);
  m_desc3.ShowWindow(show);
  m_link.ShowWindow(show);
  m_img.ShowWindow(show);
}

void SystemConfigurationView::ShowSensorConfiguration( int nCmdShow )
{
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        return;
    }
    else
    {
        if( nCmdShow == SW_SHOW )
        {
            m_sensorGrid.UpdateGrid();
        }
        m_sensorGrid.ShowWindow(nCmdShow);

 	    GetDlgItem( IDC_SYSTEM_CONFIG_SENSOR_BOUNDARY )->ShowWindow( nCmdShow );
    }

}

void SystemConfigurationView::ShowSyncroConfiguration( int nCmdShow )
{    
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        if( nCmdShow == SW_SHOW )
        {
            m_channelGrid.UpdateGrid();
        }
        m_channelGrid.ShowWindow(nCmdShow);        
    }
    else
    {
        if( nCmdShow == SW_SHOW )
        {
            m_gyroGrid.UpdateGrid();
        }
        m_gyroGrid.ShowWindow(nCmdShow);
    }
        
   
    GetDlgItem( IDC_SYSTEM_CONFIG_SYNCRO_BOUNDARY )->ShowWindow( nCmdShow );    
}

void SystemConfigurationView::UpdateGeneralConfiguration( void )
{
    m_GeneralProject.SetWindowText( (LPCTSTR)SysSetup->GetProjectName() );	
	m_GeneralOperator.SetWindowText( (LPCTSTR)SysSetup->GetOperatorName() );
	m_GeneralDauSN.SetWindowText( (LPCTSTR)SysSetup->GetDAUSerialString() );
	m_GeneralDate.SetWindowText(SysSetup->GetProjectTime());// COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")) );
	m_GeneralPlace.SetWindowText( (LPCTSTR)SysSetup->GetPlace() );

	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT)
	{
		m_GeneralShip.SetWindowText((LPCTSTR)SysSetup->GetShipName());
	}
	else if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_CALIBRATION)
	{
		m_GeneralShip.SetWindowText((LPCTSTR)SysSetup->GetPlatformSN());
	}


    if( theApp.IsAligner202Enabled() == FALSE )
    {
	    CString latitude, unitStr;
	    latitude.Format( _T("%.2f°"), SysSetup->GetLatitude() );
	    m_GeneralLatitude.SetWindowText( (LPCTSTR)latitude );
                
        if( SysSetup->GetUnits() == UNIT_MRAD )
        {
            unitStr.LoadString( IDS_MRAD_UNIT );
        }
        else
        {
            unitStr.LoadString( IDS_ARCMIN_UNIT );
        }
        m_GeneralUnit.SetWindowText( (LPCTSTR)unitStr );
    }
}


void SystemConfigurationView::UpdateSensorConfiguration( void )
{
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        return;
    }
    
    m_sensorGrid.UpdateGrid();
}
  


void SystemConfigurationView::UpdateSyncroConfiguration( void )
{	
    if( theApp.IsAligner202Enabled())
    {
        m_channelGrid.UpdateGrid();
    }
}

BEGIN_MESSAGE_MAP(SystemConfigurationView, CFormView)
	ON_WM_ACTIVATE()
  ON_WM_TIMER()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

 
// SystemConfigurationView diagnostics

#ifdef _DEBUG
void SystemConfigurationView::AssertValid() const
{
	CFormView::AssertValid();
}

void SystemConfigurationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// SystemConfigurationView message handlers

void SystemConfigurationView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    
    if(theApp.IsAligner202Enabled())
    {
        m_channelGrid.Init();        
    }
    else
    {
        m_sensorGrid.Init();    
        m_gyroGrid.Init();

        CRect ref1Rect, ref2Rect, rect;
        GetDlgItem(IDC_SYSTEM_CONFIG_SENSOR_BOUNDARY)->GetWindowRect(&ref1Rect);
        GetDlgItem(IDC_SYSTEM_CONFIG_SYNCRO_BOUNDARY)->GetWindowRect(&ref2Rect);
        ScreenToClient(ref1Rect);
        ScreenToClient(ref2Rect);


        CWnd* pInfo = GetDlgItem(IDC_SYSTEM_INFO_BOUNDARY);
       // pInfo->ShowWindow(SW_SHOW);
       // pInfo->GetWindowRect(&rect);
        rect.top = ref2Rect.top;
        rect.bottom = ref2Rect.bottom;

        rect.left = ref2Rect.right + 10;
        rect.right = ref1Rect.right;
        pInfo->MoveWindow(rect);
    
        //CRect r(CPoint(rect.left+20, rect.top+20), CSize(20,20));
        CRect r1(CPoint(rect.left + 20, rect.top + 30), CSize(20, 20));
        CRect r2(CPoint(rect.left + 20, rect.top + 60), CSize(20, 20));
        CRect r3(CPoint(rect.left + 20, rect.top + 90), CSize(20, 20));

        LOGFONT logFont;
        ZeroMemory(&logFont, sizeof(LOGFONT));
        logFont.lfHeight = 14;
        logFont.lfWeight = FW_NORMAL;
        strcpy(logFont.lfFaceName, "Ariel");
        m_font.CreateFontIndirect(&logFont);

        m_descBox1.Create(WS_CHILD| WS_BORDER, r1, this, 1000);
        m_descBox1.SetBkColor(USED_COLOR);
        r1 = CRect(r1.TopLeft()+CSize(25,5), CSize(100, 20));
        m_desc1.Create("OK", WS_CHILD, r1, this);
        m_desc1.SetFont(&m_font);

        m_descBox2.Create(WS_CHILD | WS_BORDER, r2, this, 1000);
        m_descBox2.SetBkColor(UNCONNECTED_COLOR);
        r2 = CRect(r2.TopLeft() + CSize(25, 5), CSize(100, 20));
        m_desc2.Create("Unconnected", WS_CHILD, r2, this);
        m_desc2.SetFont(&m_font);

        m_descBox3.Create(WS_CHILD | WS_BORDER, r3, this, 1000);
        m_descBox3.SetBkColor(UNCALIBRATED_COLOR);
        r3 = CRect(r3.TopLeft() + CSize(25, 5), CSize(100, 20));
        m_desc3.Create("Calibration Exired", WS_CHILD, r3, this);
        m_desc3.SetFont(&m_font);

        CRect r4(CPoint(rect.right - 90, rect.bottom - 110), CSize(80, 50));
        m_link.Create("Contact:\r\https:\\\\schill.se", WS_CHILD, r4, this, 1000);
        m_link.SetFont(&m_font);

        CRect r5(CPoint(rect.right - 90, rect.bottom - 80), CSize(60, 60));
        m_img.Create("", WS_CHILD, r5, this, 1000);
        m_img.ModifyStyle(0, SS_BITMAP| SS_REALSIZECONTROL);
        m_bm.LoadBitmap(IDB_QR);
        m_img.SetBitmap(m_bm);
    }

}

void SystemConfigurationView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void SystemConfigurationView::OnTimer( UINT nIDEvent )
{
	//m_GeneralDate.SetWindowText( COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S") ) );
	m_sensorGrid.UpdateGrid();
}

void SystemConfigurationView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus) ;
  
	if (bShow)
	{
		SetTimer( m_timerId, 1000, NULL ) ;
	}
	else
	{
		KillTimer( m_timerId ) ;
	}
}
