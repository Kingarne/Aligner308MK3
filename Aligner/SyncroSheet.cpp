// $Id: SyncroSheet.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"

#include "SyncroSheet.h"
#include ".\syncrosheet.h"

IMPLEMENT_DYNAMIC(SyncroSheet, CPropertySheet)

SyncroSheet::SyncroSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

SyncroSheet::SyncroSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

SyncroSheet::~SyncroSheet()
{
}


BEGIN_MESSAGE_MAP(SyncroSheet, CPropertySheet)
END_MESSAGE_MAP()


