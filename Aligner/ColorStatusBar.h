#pragma once

#include <vector>

#define NORMAL_COLOR			RGB(90,90,0)
#define DAU_PRESENT_COLOR		RGB(100,255,100)
#define DAU_INVALID_COLOR		RGB(255,180,0)
#define DAU_NOT_PRESENT_COLOR	RGB(200,50,50)


enum Indicators
{
	I_EMPTY,
	I_INDICATOR_DAU_STATUS,
};


struct IndicatorData
{
  int id;  
  int width;
  CString text;
  COLORREF color;
  COLORREF bgColor;
};



class ColorStatusBar : public CStatusBar
{
  
public:
  ColorStatusBar();
  
  //{{AFX_VIRTUAL(ColorStatusBar)
  //}}AFX_VIRTUAL
	

	void SetPaneTextColor(int pane, COLORREF color);
	void SetPaneBGColor(int pane, COLORREF color);
	void SetPaneText(int pane, CString& text);
	void AddIndicator(int id, CString title="", COLORREF color=RGB(0,0,0), int width=-1);

protected:
  //{{AFX_MSG(ColorStatusBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  void DrawIndicators(CDC* pDc);
  std::vector<IndicatorData> m_indicatorData;

  std::map<int, int> m_indexMap;
  COLORREF m_bkColor;
  CFont m_font;
};



