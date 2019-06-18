// $Id: SensorAlignmentSheet.cpp,v 1.2 2013/02/13 14:25:45 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "SensorAlignmentSheet.h"
#include ".\sensoralignmentsheet.h"

IMPLEMENT_DYNAMIC(SensorAlignmentSheet, CPropertySheet)

SensorAlignmentSheet::SensorAlignmentSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

SensorAlignmentSheet::SensorAlignmentSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// Empty
}

SensorAlignmentSheet::~SensorAlignmentSheet( void )
{
	// Empty
}

BEGIN_MESSAGE_MAP(SensorAlignmentSheet, CPropertySheet)
END_MESSAGE_MAP()

BOOL SensorAlignmentSheet::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();

    //Turn of both interfaces
    DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProA, false);
    DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProB, false);

    return bResult;
}
