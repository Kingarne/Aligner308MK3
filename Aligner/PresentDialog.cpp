// $Id: PresentDialog.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "PresentDialog.h"
#include "AlignmentWizard.h"

#define MAX_COMMENT_LENGTH  255 //The database limits it to 255

IMPLEMENT_DYNAMIC(CPresentDialog, CDialog)

CPresentDialog::CPresentDialog( CResultTable* pResultTable /*=NULL*/, CWnd* pReportWindow /*=NULL*/ )
	: CDialog(CPresentDialog::IDD, pReportWindow)
  , m_Comment(_T(""))
{
	ASSERT( pResultTable );
	m_pResultTable = pResultTable;
}

CPresentDialog::~CPresentDialog()
{
}

void CPresentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text( pDX, IDC_COMMENT_BOX, m_Comment ) ;
}

BEGIN_MESSAGE_MAP(CPresentDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_EN_KILLFOCUS(IDC_COMMENT_BOX, OnEnKillfocusCommentBox)
END_MESSAGE_MAP()

// CPresentDialog message handlers

BOOL CPresentDialog::OnInitDialog()
{
	//if( m_pResultTable->m_InParam.DeleteReport == FALSE )
	//{
	//	GetDlgItem( IDOK )->ShowWindow( SW_HIDE );
	//	GetDlgItem( IDCANCEL )->ShowWindow( SW_SHOW );
	//	GetDlgItem( IDC_SAVE )->ShowWindow( SW_SHOW );
	//}
	//else
	//{
	//	GetDlgItem( IDOK )->ShowWindow( SW_SHOW );
	//	GetDlgItem( IDCANCEL )->ShowWindow( SW_HIDE );
	//	GetDlgItem( IDC_SAVE )->ShowWindow( SW_HIDE );
	//}
  CDialog::OnInitDialog();
	m_Comment = m_pResultTable->m_InParam.Comment;
  UpdateData( FALSE );

  return TRUE;
}

void CPresentDialog::OnBnClickedOk()
{
	if( m_pResultTable->m_InParam.Comment != m_Comment )
	{
		m_pResultTable->m_InParam.Comment = m_Comment.Left( MAX_COMMENT_LENGTH );
		//m_pResultTable->UpdateReport();
		m_pResultTable->UpdateComment();
	}
	OnOK();
}

void CPresentDialog::OnBnClickedCancel()
{
	OnCancel();
}

void CPresentDialog::OnBnClickedSave()
{
	m_pResultTable->m_InParam.Comment = m_Comment.Left( MAX_COMMENT_LENGTH );
	m_pResultTable->CloseReport();
	OnOK();
}

void CPresentDialog::OnEnKillfocusCommentBox()
{
  UpdateData( TRUE );
}

