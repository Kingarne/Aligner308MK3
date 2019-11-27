#pragma once
#include ".\gridctrl\gridctrl.h"

#define USED_COLOR	    RGB(255,255,255)
#define UNUSED_COLOR	::GetSysColor(COLOR_BTNFACE)

struct DebugColumn{
	DebugColumn(CString head, int w):header(head),width(w){}
	CString header;
	int width;
};

enum DebugColumnName{DColCh,DColOD,DColODA,DColSelect,DColRo, DColPi,DColCurrAz,DColCurrAzBack,DColEal,/*DColEalS,*/DColEalAbs,DColEac,DColZPar,DColEOn,DColMode};

class DebugGrid : public CGridCtrl
{
public:
	DebugGrid(void);
	~DebugGrid(void);

	void Init();
	void SetCell(int nRow, int nCol, int nVal);
	void SetRow(int nRow, const vector<CString> &items);
	void Clear();
	void Reset();
	
	void UpdateGrid();
//	int StoreToFile();
	
private:
	int m_cols;
	int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateHeader();
};

