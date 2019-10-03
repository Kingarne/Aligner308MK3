// $Id: AlignmentWizard.cpp,v 1.5 2011/10/11 08:58:41 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Aligner.h"
#include "AlignmentWizard.h"
#include "UserMessages.h"
#include "Util.h"

// CAlignmentWizard

IMPLEMENT_DYNAMIC(CAlignmentWizard, CPropertySheet)

CAlignmentWizard::CAlignmentWizard( UINT nIDCaption,  CWnd* pWndParent )
  :CPropertySheet( nIDCaption, pWndParent ), DataRecepient( this )
{
    SetWizardMode() ;
	m_pResultTable = NULL ;
    m_pSource = NULL ;
    m_pGraph = static_cast<CGraphView *>(theApp.m_pGraphView) ;
	m_pGraph ->CleanUp() ;
    m_EnableReference = FALSE ;
    m_EnableFixPlane = TRUE ;
    m_EnableMoveOfWizard = TRUE;
	m_MeasurementInterrupted = FALSE;
	m_psh.dwFlags &= ~PSH_HASHELP;
  //m_psh.dwFlags |= PSH_MODELESS;
	m_deleteReport = FALSE;
	m_SperryActive = FALSE;
}

CAlignmentWizard::~CAlignmentWizard()
{
  // Empty
}

void CAlignmentWizard::ExitResultTable( BOOL DeleteReport, BOOL b)
{
	if( m_pResultTable != NULL )
	{
     //   m_pResultTable->CloseReport();

// 		if( SaveGraphFileNameWithoutReport == TRUE )
// 		{
// 			m_GraphFileManager.ClearAllIncludeToResultTable();
// 			AddGraphFileNameToDataBase();
// 		}

        if( DeleteReport == TRUE )
        {
           // theApp.RemoveFromDocument( m_pResultTable -> m_reportHistoryMainID ) ;
            m_pResultTable->DeleteLast();
        }
        else
        {
            //theApp.AddToDocument( m_pResultTable->m_reportHistoryMainID ) ;
        }

		delete m_pResultTable;
		m_pResultTable = NULL;
	}
}
/*
BOOL CAlignmentWizard::AddGraphFileNameToDataBase( void )
{
	BOOL result = TRUE;
	for( int i = 0; i < m_GraphFileManager.GetNoOfSavedFiles(); i++ )
	{
        if( m_GraphFileManager.GetIncludeToResultTable( i ) == TRUE )
        {
		    if( TRUE != m_pResultTable->AddGraph( m_GraphFileManager.GetFileName( i ), m_GraphFileManager.GetIncludeToResultTable( i ) ) )
		    {
			    result = FALSE;
		    }
        }
        else
        {
            MoveFileToTemporaryDirectory( m_GraphFileManager.GetFileName( i ) ) ;
        }
	}
	return( result );
}
*/
BEGIN_MESSAGE_MAP(CAlignmentWizard, CPropertySheet)
	ON_WM_DESTROY()
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
  ON_MESSAGE(UM_DAU_ERROR, OnDauError)
  ON_WM_NCLBUTTONDOWN()
  ON_WM_NCRBUTTONDOWN()
END_MESSAGE_MAP()

// CAlignmentWizard message handlers

BOOL CAlignmentWizard::OnInitDialog()
{
	if( CheckSystemSetupData() == FALSE )
    {
        EndDialog( IDCANCEL );
        return FALSE;
    }
  
    DAU::GetDAU().EnableErrorEvent( FALSE ) ;
    SetSource( &(DAU::GetDAU() ) );

    if( m_pSource == NULL )
    {
        //TODO error message
        EndDialog( IDCANCEL );
        return FALSE;
    }

    if( m_pGraph == NULL )
    {
        //TODO error message
        EndDialog( IDCANCEL );
        return FALSE;
    }
    m_Measure.SetGraphPointer( m_pGraph );  
    m_Measure.m_pSource = m_pSource;
    m_Status = STATUS_PAGE_NOT_READY;
    m_UseParallaxComp = g_AlignerData.UseParallax;
	m_PolarGraphFileName = _T("");
	m_ErrorGraphFileName = _T("");
	m_ResultGraphFileName = _T("");
	COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_MeasurementReadyTimeStamp );

  return CPropertySheet::OnInitDialog();
}

LRESULT CAlignmentWizard::OnNewData( WPARAM, LPARAM )
{
	if( m_pSource == NULL )
	{
		return( FALSE );
	}

  	if(UpdateCurrentValues() )
	{
        if(m_Measure.m_mode == MEASURE_MODE_ON)
            m_Measure.AddData(m_currentValues);
	}

    return FALSE ;
}

LRESULT CAlignmentWizard::OnDauError( WPARAM, LPARAM )
{
    switch( m_Measure.m_mode )
    {
        case MEASURE_MODE_ON:  
            //DAU::GetDAU().Restart( TRUE );
            m_Measure.m_InParam.Break = TRUE;     
            break;        
        default: break;
  }
	return TRUE;
}

void CAlignmentWizard::OnDestroy() 
{  
	ExitResultTable( FALSE, FALSE );

    if( m_pSource )
    {
		SetSource( NULL );
    }
	ClearSelectedChannels();
	theApp.SwitchView( theApp.m_pSystemConfigurationView );
	CPropertySheet::OnDestroy();
}

void CAlignmentWizard::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  if( m_EnableMoveOfWizard == TRUE )
  {
    CPropertySheet::OnNcLButtonDown(nHitTest, point);
  }
}

void CAlignmentWizard::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
  if( m_EnableMoveOfWizard == TRUE )
  {
    CPropertySheet::OnNcRButtonDown(nHitTest, point);
  }
}

void CAlignmentWizard::SavePolarGraphFile( void )
{
	if( m_PolarGraphFileName.GetLength() == 0 )
	{
		CString graphFileName;
		if( m_pGraph->SaveGraphToUniqueFileName( graphFileName, TRUE ) == TRUE )
		{
			m_GraphFileManager.SaveFileName( graphFileName, TRUE );
			m_PolarGraphFileName = graphFileName;
		}
	}
    else 
    {
        _bstr_t footer = m_pGraph -> m_pChart -> GetFooter() -> GetText() -> GetText() ;
        CString footerText( footer.GetBSTR() ) ;
        footerText += _T("\n") ;
        footerText += GetFileName( m_PolarGraphFileName ) ;
        m_pGraph -> m_pChart -> GetFooter() -> GetText() -> PutText( footerText.AllocSysString() ) ;
    }
}

void CAlignmentWizard::SaveErrorGraphFile( void )
{
	if( m_ErrorGraphFileName.GetLength() == 0 )
	{
		CString graphFileName;
		if( m_pGraph->SaveGraphToUniqueFileName( graphFileName, FALSE ) == TRUE )
		{
			//m_GraphFileManager.SaveFileName( graphFileName, TRUE );
			m_ErrorGraphFileName = graphFileName;
		}
	}
    else 
    {
        _bstr_t footer = m_pGraph -> m_pChart -> GetFooter() -> GetText() -> GetText() ;
        CString footerText( footer.GetBSTR() ) ;
        footerText += _T("\n") ;
        footerText += GetFileName( m_ErrorGraphFileName ) ;
        m_pGraph -> m_pChart -> GetFooter() -> GetText() -> PutText( footerText.AllocSysString() ) ;
    }
}

void CAlignmentWizard::SaveResultGraphFile( void )
{
	if( m_ResultGraphFileName.GetLength() == 0 )
	{
		CString graphFileName;
		if( m_pGraph->SaveGraphToUniqueFileName( graphFileName, TRUE ) == TRUE )
		{
			m_GraphFileManager.SaveFileName( graphFileName, TRUE );
			m_ResultGraphFileName = graphFileName;
		}
	}
    else 
    {
        _bstr_t footer = m_pGraph -> m_pChart -> GetFooter() -> GetText() -> GetText() ;
        CString footerText( footer.GetBSTR() ) ;
        footerText += _T("\n") ;
        footerText += GetFileName( m_ResultGraphFileName ) ;
        m_pGraph -> m_pChart -> GetFooter() -> GetText() -> PutText( footerText.AllocSysString() ) ;
    }
}

