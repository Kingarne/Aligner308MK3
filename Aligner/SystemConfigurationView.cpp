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
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_1, m_SyncroNominalAzimuth[0]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_2, m_SyncroNominalAzimuth[1]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_3, m_SyncroNominalAzimuth[2]);
	    DDX_Control(pDX, IDC_SYSTEM_CONFIG_SYNCRO_NOMINAL_AZIMUTH_4, m_SyncroNominalAzimuth[3]);
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
	m_GeneralShip.SetWindowText( (LPCTSTR)SysSetup->GetShipName() );
	m_GeneralOperator.SetWindowText( (LPCTSTR)SysSetup->GetOperatorName() );
	m_GeneralDauSN.SetWindowText( (LPCTSTR)DAU::GetDAU().GetSerialNumber() );
	m_GeneralDate.SetWindowText( COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S") ) );
	m_GeneralPlace.SetWindowText( (LPCTSTR)SysSetup->GetPlace() );

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
    }

}

void SystemConfigurationView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void SystemConfigurationView::OnTimer( UINT nIDEvent )
{
	m_GeneralDate.SetWindowText( COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S") ) );
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
