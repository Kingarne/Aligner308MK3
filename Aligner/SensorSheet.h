// $Id: SensorSheet.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class SensorSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(SensorSheet)
	DECLARE_MESSAGE_MAP()
public:
	SensorSheet( UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 ) ;
	SensorSheet( LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 ) ;
	virtual ~SensorSheet( void ) ;
} ;
