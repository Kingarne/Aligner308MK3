// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "ColorStatic.h"
#include ".\colorstatic.h"


// ColorStatic

IMPLEMENT_DYNAMIC(ColorStatic, CStatic)

ColorStatic::ColorStatic()
{
    m_crTextColor = RGB(0,70,213);
    m_brBkgnd.CreateSolidBrush(m_crBkColor); // Create the Brush Color for the Background.
}

ColorStatic::~ColorStatic()
{
}


BEGIN_MESSAGE_MAP(ColorStatic, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// ColorStatic message handlers



HBRUSH ColorStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
    HBRUSH hbr;
    
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(m_crTextColor);
    return NULL;
}
