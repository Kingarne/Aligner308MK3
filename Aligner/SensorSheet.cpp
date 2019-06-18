// $Id: SensorSheet.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SensorSheet.h"

IMPLEMENT_DYNAMIC(SensorSheet, CPropertySheet)
SensorSheet::SensorSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

SensorSheet::SensorSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

SensorSheet::~SensorSheet()
{
}


BEGIN_MESSAGE_MAP(SensorSheet, CPropertySheet)
END_MESSAGE_MAP()


// SensorSheet message handlers
