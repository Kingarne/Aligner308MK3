// $Id: LiveGraphView.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Include.h"

IMPLEMENT_DYNCREATE(CLiveGraphView, CFormView)

CLiveGraphView::CLiveGraphView( void ) : CFormView(CLiveGraphView::IDD)
{
  m_pSource = NULL ;
}

CLiveGraphView::~CLiveGraphView( void )
{
  // Empty.
}

void CLiveGraphView::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_CHART_CONTROL, m_chart) ;
  m_pChart = m_chart.GetControlUnknown() ;
}

BEGIN_MESSAGE_MAP(CLiveGraphView, CFormView)
  ON_BN_CLICKED(IDC_LIVE_GRAPH_QUIT, OnBnClickedLiveGraphQuit)
  ON_BN_CLICKED(IDC_LIVE_GRAPH_PRINT, OnBnClickedLiveGraphPrint)
  ON_BN_CLICKED(IDC_LIVE_GRAPH_SAVE, OnBnClickedLiveGraphSave)
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
  ON_WM_DESTROY()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CLiveGraphView::AssertValid( void ) const
{
	__super::AssertValid();
}

void CLiveGraphView::Dump( CDumpContext &dc ) const
{
	__super::Dump(dc);
}
#endif //_DEBUG

void CLiveGraphView::SetSource( DAU *pDAU )
{
  if (m_pSource)
  {
    m_pSource -> RemoveListener( this ) ;
  }
  if (( NULL != (m_pSource = pDAU) ))
  {
    m_pSource -> AddListener( this ) ;
  }
}

afx_msg LRESULT CLiveGraphView::OnNewData( WPARAM, LPARAM )
{
  UpdateData() ;
  return TRUE ;
}

void CLiveGraphView::UpdateData( void )
{
  if (m_pSource)
  {
    try
    {
      IChartGroupCollection_DualPtr pGroups = m_pChart -> GetChartGroups() ;
      IChartGroup_DualPtr pGroup = pGroups -> GetItem( 1L ) ;
      IChart2DData_DualPtr pData = pGroup -> GetData() ;
      pData -> PutIsBatched( TRUE ) ;
      long pointCount = pData -> GetNumPoints( 1 ) ;
      pData -> PutNumPoints( 1, pointCount + 1 ) ;
      pData -> Put_Y( 1, pointCount + 1 , 180.0 * m_pSource -> GetSyncro( 0 ) -> GetRoll() / M_PI ) ;
      pData -> PutNumPoints( 2, pointCount + 1 ) ;
      pData -> Put_Y( 2, pointCount + 1 , 180.0 * m_pSource -> GetSyncro( 0 ) -> GetPitch() / M_PI ) ;
    }
    catch (_com_error e)
    {
      TRACE("Error :%s\n", (char *)e.Description()) ;
    }
  }
}

void CLiveGraphView::OnBnClickedLiveGraphQuit( void )
{
  theApp.SwitchView( theApp.m_pAlignerView );
}

void CLiveGraphView::OnInitialUpdate( void )
{

	//CSetupLiveDataGraphDlg setupLiveDataGraphDlg( m_ChannelNo, &m_NoOfChannels );
	//setupLiveDataGraphDlg.DoModal();
  //m_ChannelNo[0];
  //m_NoOfChannels;

  __super::OnInitialUpdate() ;
  try
  {
    IChartGroup_DualPtr pGroup = m_pChart -> GetChartGroups() -> GetItem( 1L ) ;
    IChart2DData_DualPtr pData = pGroup -> GetData() ;
    pData -> PutNumSeries( 4 ) ;
    pData -> PutNumPoints( 1, 0 ) ;
    pData -> PutNumPoints( 2, 0 ) ;
    pData -> PutNumPoints( 3, 0 ) ;
    pData -> PutNumPoints( 4, 0 ) ;
    IChartStyleCollection_DualPtr pStyleCollection= pGroup -> GetStyles() ;
    pStyleCollection -> GetItem( 1L ) -> GetSymbol() -> PutShape( oc2dShapeNone );
    pStyleCollection -> GetItem( 2L ) -> GetSymbol() -> PutShape( oc2dShapeNone );
    pStyleCollection -> GetItem( 3L ) -> GetSymbol() -> PutShape( oc2dShapeNone );
    pStyleCollection -> GetItem( 4L ) -> GetSymbol() -> PutShape( oc2dShapeNone );
  }
  catch (_com_error e)
  {
    TRACE("Error: %s\n", (char *)e.Description()) ;
  }

  if (m_pSource)
  {
    m_pSource -> AddListener( this ) ;
  }
}

void CLiveGraphView::OnBnClickedLiveGraphPrint( void )
{
  try
  {
    m_pChart -> PrintChart( oc2dFormatBitmap, oc2dScaleToWidth, 0, 0, 0, 0 ) ;
  }
  catch (_com_error e)
  {
    TRACE("Error: %s\n", (char *)e.Description()) ;
  }
}

void CLiveGraphView::OnBnClickedLiveGraphSave( void )
{
  try
  {
    IChartGroupCollection_DualPtr pGroups = m_pChart -> GetChartGroups() ;
    IChart2DData_DualPtr pData = pGroups -> GetItem( "1" ) -> GetData() ;
    pData -> PutNumPoints( 1, 0 ) ;
    pData -> PutNumPoints( 2, 0 ) ;
    pData -> PutNumPoints( 3, 0 ) ;
    pData -> PutNumPoints( 4, 0 ) ;
  }
  catch (_com_error e)
  {
    TRACE("Error: %s\n", (char *)e.Description()) ;
  }
}

void CLiveGraphView::OnDestroy() 
{
  if (m_pSource)
  {
    SetSource( NULL ) ;
  }
	CFormView::OnDestroy();
}
