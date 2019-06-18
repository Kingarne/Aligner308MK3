#pragma once
#include "StatusLogger.h"
#include "afxlistctrl.h"

class StatusCtrl : public CMFCListCtrl
{
public:
	StatusCtrl(void);

	void Setup(bool bShowTime, int logFilter = LogAll);
	void Clear();
	void AddStatusMessage(const StatusLog &log);

private:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual COLORREF OnGetCellTextColor(int nRow, int nColumn);
	LRESULT OnMessage(WPARAM wp, LPARAM lp);
	
	int GetSelectedItem(); 

	bool m_bInitialized;

	bool m_bShowTime;
	int m_nMaxItems;	//Maximum rows/items in the list
	int m_nLogFilter;	//Which log types to display
};

