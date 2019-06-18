// $Id: SyncroSheet.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class SyncroSheet : public CPropertySheet
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(SyncroSheet)
public:
	SyncroSheet( UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 ) ;
	SyncroSheet( LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 ) ;
	virtual ~SyncroSheet( void ) ;
} ;
