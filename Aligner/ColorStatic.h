#pragma once


// ColorStatic

class ColorStatic : public CStatic
{
	DECLARE_DYNAMIC(ColorStatic)

public:
	ColorStatic();
	virtual ~ColorStatic();

protected:
	DECLARE_MESSAGE_MAP()

    CBrush m_brBkgnd; // Holds Brush Color for the Static Text
    COLORREF m_crBkColor; // Holds the Background Color for the Text
    COLORREF m_crTextColor; // Holds the Color for the Text
public:
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


