
#include "stdafx.h"
#include "ColorStatusBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ColorStatusBar::ColorStatusBar()
{
  m_bkColor = GetSysColor(COLOR_BTNFACE);
  //m_bkColor = RGB(255,0,0);
  
	LOGFONT logFont;
  ZeroMemory (&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 16;
	logFont.lfWeight = FW_NORMAL;
	strcpy (logFont.lfFaceName, "Ariel"); 
	m_font.CreateFontIndirect(&logFont);

}

BEGIN_MESSAGE_MAP(ColorStatusBar, CStatusBar)
  //{{AFX_MSG_MAP(ColorStatusBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void ColorStatusBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
  
  CRect rect;
  GetClientRect(&rect);
  CDC memDc;
  CBitmap bitmap;  
  if(memDc.CreateCompatibleDC(&dc))
  {
    if(bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
    {
      memDc.SelectObject(&bitmap);
            
//       CBrush brush;
//       brush.CreateSolidBrush(m_bkColor);
      memDc.FillSolidRect(&rect, m_bkColor);
	    
      DrawIndicators(&memDc);
      
      dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDc, 0, 0, SRCCOPY );
    }
  }
	// Do not call CStatusBar::OnPaint() for painting messages
}


void ColorStatusBar::SetPaneTextColor(int pane, COLORREF color)
{
	std::map<int, int>::iterator iter;
	if((iter = m_indexMap.find(pane)) == m_indexMap.end())
	{
		return;
	}

	int index = iter->second;
	IndicatorData data = m_indicatorData[index];
	if(data.color == color)
		return;

	data.color = color;
	m_indicatorData[index] = data;
	Invalidate(FALSE);
}


void ColorStatusBar::SetPaneBGColor(int pane, COLORREF color)
{
	std::map<int, int>::iterator iter;
	if((iter = m_indexMap.find(pane)) == m_indexMap.end())
	{
		return;
	}

	int index = iter->second;
	IndicatorData data = m_indicatorData[index];
	if(data.bgColor == color)
		return;

	data.bgColor = color;
	m_indicatorData[index] = data;
	Invalidate(FALSE);
}

void ColorStatusBar::SetPaneText(int pane, CString& text)
{  
  std::map<int, int>::iterator iter;
  if((iter = m_indexMap.find(pane)) == m_indexMap.end())
  {
    return;
  }

  int index = iter->second;
  IndicatorData data = m_indicatorData[index];
  if(data.text == text)
    return;

  data.text = text;
  m_indicatorData[index] = data;
  Invalidate(FALSE);
}


void ColorStatusBar::AddIndicator(int id, CString text, COLORREF color, int width)
{
  IndicatorData data;
  data.bgColor = GetSysColor(COLOR_BTNFACE);
  data.id = id;
  if(id == I_EMPTY)
  {
    data.text = "";    
    data.width = 20;
  }
  else
  {    
    if(text == "")
    {
      for(int i=0;i<width;i++)
      {
        text += 'a';
      }
      width = -1;
    }
    else
    {
      data.text = text;
    }    
    data.color = color;
    if(width == -1)
    {
      CDC* pDc = GetDC();
      CFont* pOldFont = pDc->SelectObject(&m_font);
      CRect rect;
      pDc->DrawText(text, &rect, DT_CALCRECT);
    
      data.width = rect.Width() + 10;
      pDc->SelectObject(pOldFont);
    }
    else
    {
      data.width = width*10;
    }
  }
  
  if(id != I_EMPTY)
    m_indexMap[id] = m_indicatorData.size();
  m_indicatorData.push_back(data);
}


void ColorStatusBar::DrawIndicators(CDC* pDc)
{  
  CRect rect(CPoint(0,0),CSize(100,20));
  CPen darkPen(PS_SOLID, 1, RGB(100,100,100));   
  CPen whitePen(PS_SOLID, 1, RGB(255,255,255));   
  CPen* pOldPen = pDc->SelectObject(&darkPen);
  CFont* pOldFont = pDc->SelectObject(&m_font);
  int height = 16;

  CRect clientRect;
  GetClientRect(&clientRect);

  std::vector<IndicatorData>::iterator iter;
  for(iter = m_indicatorData.begin();iter != m_indicatorData.end() ; iter++)
  {
    int left = rect.left;
    int width = (iter != (m_indicatorData.end()-1)) ? iter->width : (clientRect.right - left);
    CRect paneRect = CRect(CPoint(rect.left+2,rect.top), CSize(width-2, height));

	pDc->SetTextColor(iter->color);
    pDc->SetBkColor(iter->bgColor);
    CRect paneTextRect = CRect(CPoint(rect.left+2,rect.top), CSize(iter->width-2, height));
    pDc->FillSolidRect(&paneRect, iter->bgColor);
	pDc->DrawText(iter->text, &paneTextRect, DT_CENTER);    
	    
    pDc->SelectObject(&darkPen);
    pDc->MoveTo(left,0);
    pDc->LineTo(left,height);
    pDc->MoveTo(left,0);
    pDc->LineTo(left+width,0);
    pDc->SelectObject(&whitePen);
    pDc->MoveTo(left+1,height);
    pDc->LineTo(left+width,height);
    pDc->LineTo(left+width,0);

    rect += CSize(iter->width+3,0); 
  }


  pDc->SelectObject(pOldPen);
  pDc->SelectObject(pOldFont);
}

