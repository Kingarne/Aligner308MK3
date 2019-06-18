// $Id: AlignerView.cpp,v 1.7 2013/04/02 08:28:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AlignerView.h"
#include "SystemSetup.h"
#include "DAU.h"
#include ".\alignerview.h"

NStopWatch stw;
CString tr;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAlignerView, CView)

BEGIN_MESSAGE_MAP(CAlignerView, CView)
  ON_WM_DESTROY()
  ON_WM_TIMER()
END_MESSAGE_MAP()

CAlignerView::CAlignerView( void ):m_timer(0)
{
	// Empty
}

CAlignerView::~CAlignerView( void )
{
  // Empty
}

BOOL CAlignerView::PreCreateWindow( CREATESTRUCT &cs )
{
	return __super::PreCreateWindow( cs ) ;
}

void CAlignerView::OnInitialUpdate( void )
{
	m_timer = SetTimer(1, 100, NULL);
    
    CView::OnInitialUpdate() ;
}

void CAlignerView::OnDraw( CDC *pDC )
{
  /*
  CAlignerDoc *pDocument = GetDocument() ;
  ASSERT_VALID(pDocument) ;
  CString text ;
  pDC -> DrawText( _T("Operator:"), CRect(10,10,300,25), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( _T("Place   :"), CRect(10,30,300,45), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( _T("Ship:    "), CRect(10,50,300,65), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( (LPCTSTR)SysSetup->GetOperatorName(),  CRect(90,10,300,25), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( (LPCTSTR)SysSetup->GetPlace(),         CRect(90,30,300,45), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( (LPCTSTR)SysSetup->GetShipName(),      CRect(90,50,300,65), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( _T("DAU S/N:"),  CRect(320,10,600,25), DT_BOTTOM | DT_LEFT ) ;
  pDC -> DrawText( (LPCTSTR)DAU::GetDAU().GetSerialNumber(), CRect(400,10,800,65), DT_BOTTOM | DT_LEFT ) ;

  const int startHeight = 90 ;
  const int heightStep = 20 ;
  const int heightShift = 15 ;
  const int channelPosition = 10 ;
  const int sensorPosition = channelPosition + 80 ;
  const int commentPosition = sensorPosition + 100 ;
  const int typePosition = commentPosition + 140 ;
  const int nominalPosition = typePosition + 45 ;
  const int adapterPosition = nominalPosition + 120 ;

  int height = startHeight ;
  if (0 < DAU::GetDAU().GetSensorCount() + DAU::GetDAU().GetSyncroCount())
  {
    pDC -> DrawText( _T("Channel"),        CRect(channelPosition, height, channelPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    pDC -> DrawText( _T("Sensor S/N"),     CRect(sensorPosition,  height, sensorPosition+100,  height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    pDC -> DrawText( _T("Comment"),        CRect(commentPosition, height, commentPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    pDC -> DrawText( _T("Type"),           CRect(typePosition,    height, typePosition+100,    height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    pDC -> DrawText( _T("Nomial Azimuth"), CRect(nominalPosition, height, nominalPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    pDC -> DrawText( _T("Adapter S/N"),    CRect(adapterPosition, height, adapterPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
    for (int i = 0 ; i < DAU::GetDAU().GetSyncroCount() ; i++)
    {
      height += heightStep ;
      Syncro *pSyncro= DAU::GetDAU().GetSyncro( i ) ;
      if (Unit::Unused != pSyncro -> GetType())
      {
        text.Format( _T("%+6.1f°"), pSyncro -> GetNominalAzimuth() ) ;
        pDC -> DrawText( pSyncro -> GetName(),                      CRect(channelPosition, height, channelPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( pSyncro -> GetSerialNumber(),              CRect(sensorPosition,  height, sensorPosition+100,  height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( pSyncro -> m_description,                  CRect(commentPosition, height, commentPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( Unit::GetUnitText( pSyncro -> GetType() ), CRect(typePosition,    height, typePosition+100,    height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( text,                                      CRect(nominalPosition, height, nominalPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
      }
    }
    for (i = DAU::GetDAU().GetSensorCount() - 1 ; 0 <= i ; i--)
    {
      height += heightStep ;
      Sensor *pSensor = DAU::GetDAU().GetSensor( i ) ;
      if (Unit::Unused != pSensor -> GetType())
      {
        text.Format( _T("%+6.1f°"), pSensor -> GetNominalAzimuth() ) ;
        pDC -> DrawText( pSensor -> GetName(),                      CRect(channelPosition, height, channelPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( pSensor -> GetSerialNumber(),              CRect(sensorPosition,  height, sensorPosition+100,  height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( pSensor -> m_description,                  CRect(commentPosition, height, commentPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( Unit::GetUnitText( pSensor -> GetType() ), CRect(typePosition,    height, typePosition+100,    height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        pDC -> DrawText( text,                                      CRect(nominalPosition, height, nominalPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        if (Unit::Gun == pSensor -> GetType())
        {
          pDC -> DrawText( pSensor -> GetAdapterSerialNumber(),       CRect(adapterPosition, height, adapterPosition+100, height+heightShift), DT_BOTTOM | DT_LEFT ) ;
        }
      }
    }
  }
*/
}

#ifdef _DEBUG
void CAlignerView::AssertValid() const
{
	__super::AssertValid() ;
}

void CAlignerView::Dump( CDumpContext &dc ) const
{
	__super::Dump( dc ) ;
}

CAlignerDoc* CAlignerView::GetDocument( void ) const
{
	ASSERT(m_pDocument -> IsKindOf( RUNTIME_CLASS(CAlignerDoc)) ) ;
	return (CAlignerDoc*)m_pDocument ;
}
#endif //_DEBUG


void CAlignerView::OnDestroy()
{
  CView::OnDestroy();
}

BOOL CAlignerView::ShouldReveiveData()
{
    if(DAU::GetDAU().m_mainBoardRegsPro.m_dataAcqEnable)// && DataRecepient::GetNumRecepient() > 0)
    {
        return TRUE;
    }
    return FALSE;
}



void CAlignerView::OnTimer(UINT nIDEvent)
{
    static int zeroHandledCount=0;
    int numframesHandled = DAU::GetDAU().HandleDAUFrames();
	
//     stw.Stop();
//     tr.Format("num:%d",numframesHandled);
//     stw.TraceTime(TRUE, TRUE, tr);
//     stw.Start();
    
    zeroHandledCount += (numframesHandled == 0) ? 1 : -zeroHandledCount;
	//TRACE("frames:%d, %d\n",numframesHandled, zeroHandledCount);

    if(DAU::GetDAU().GetStatus() == DAU_STATUS_NOT_INIT)
	{
		TRACE("First restart\n");
		DAU::GetDAU().Restart();
	}
	
	if(zeroHandledCount > 80 /*&& theApp.m_DAUPresent*/ )
    {
        //Dau dead...
        TRACE("dau dead\n");
        zeroHandledCount = 0;
        
        if(DAU::GetDAU().GetStatus() == DAU_STATUS_NO_DEVICE)
        {
			TRACE("DAU_STATUS_NO_DEVICE restart\n");
			DAU::GetDAU().SendDAUDeadMsg();
			DAU::GetDAU().Restart();
        }
        else if(DAU::GetDAU().GetStatus() == DAU_STATUS_OK && DAU::GetDAU().m_mainBoardRegsPro.m_dataAcqEnable)
        {
			TRACE("DAU_STATUS_OK restart\n");
			DAU::GetDAU().SendDAUDeadMsg();
			DAU::GetDAU().Restart();
        }
    }
// 	else if(!theApp.m_DAUPresent)
// 	{
// 		DAU::GetDAU().Restart();
// 	}


	static int timerCount=0;
	
	if(timerCount++%20==0)
		DAU::GetDAU().CheckDigitalDataReception();

    //TRACE("Num Handled:%d\n", numframesHandled);


    CView::OnTimer(nIDEvent);
}


