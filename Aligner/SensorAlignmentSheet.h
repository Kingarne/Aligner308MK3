// $Id: SensorAlignmentSheet.h,v 1.2 2013/02/13 14:25:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class SensorAlignmentSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(SensorAlignmentSheet)
	DECLARE_MESSAGE_MAP()
public:
	SensorAlignmentSheet( UINT caption, CWnd *pParent = NULL, UINT selectPage = 0 ) ;
	SensorAlignmentSheet( LPCTSTR pCaption, CWnd *pParent = NULL, UINT selectPage = 0 ) ;
	virtual ~SensorAlignmentSheet( void ) ;
    virtual BOOL OnInitDialog();
} ;
    