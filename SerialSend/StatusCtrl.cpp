#include "StdAfx.h"
#include "StatusCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

StatusCtrl::StatusCtrl(void)
	: m_bShowTime(true)
	, m_nMaxItems(500)
	, m_nLogFilter(0xffffffff)
	, m_bInitialized(false)
{
	
}

void StatusCtrl::AddStatusMessage( const StatusLog &log )
{
	if( (log.m_type & m_nLogFilter) == 0 )
		return;

	const int nI = GetItemCount();
	
	//Add the item
	InsertItem(nI, log.m_strTime);
	SetItemText(nI, 1, log.m_strMsg);
	SetItemData(nI, (DWORD_PTR)log.m_type);

	if(nI < 100){	//trying not to overdo this
		SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	}
	
	SetItemState(nI,LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	EnsureVisible(nI, false);	//scroll it into view if it is not

	if(nI > m_nMaxItems)
	{
		DeleteItem(0);
	}

}

void StatusCtrl::Clear()
{
	DeleteAllItems();
}

void StatusCtrl::Setup( bool bShowTime, int logFilter /*= LogAll*/ )
{
	m_bShowTime = bShowTime;
	m_nLogFilter = logFilter;

	if(m_bInitialized)
		return;

	// Change so one click activates an item in the list
	SetExtendedStyle(LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT);

	InsertColumn(0, _T("Time"));
	InsertColumn(1, _T("Message"));
	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	m_bInitialized = true;
}

BEGIN_MESSAGE_MAP(StatusCtrl, CMFCListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(STATUS_MSG, OnMessage)
END_MESSAGE_MAP()

void StatusCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int index = GetSelectedItem();
	if(index >= 0)
	{
		CString str = GetItemText(index, 1);
		//AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
// 		EditDlg dlg;
// 		dlg.Init(true /*read only*/, "Message", str);
// 		dlg.DoModal();
	}

	CMFCListCtrl::OnLButtonDblClk(nFlags, point);
}

LRESULT StatusCtrl::OnMessage( WPARAM wp, LPARAM lp )
{
	StatusLog *pLog = (StatusLog*) wp;
	if(!pLog)
		return S_FALSE;

	AddStatusMessage(*pLog);

	delete pLog;
	return S_OK;
}

COLORREF StatusCtrl::OnGetCellTextColor( int nRow, int nColumn )
{
	if(nColumn == 0)
		return CMFCListCtrl::OnGetCellTextColor(nRow,nColumn);
	else
	{
		LogType type = (LogType)GetItemData(nRow);
		switch(type)
		{
		case LogDebug:		return RGB(0,200,0);	//green
		case LogInfo:		return RGB(0,0,0);		//black
		case LogWarning:	return RGB(0,255,255);	//yellow
		case LogError:		return RGB(255,0,0);	//red
		case LogTcpIp:		return RGB(0,0,255);	//blue
		default:			
			return CMFCListCtrl::OnGetCellTextColor(nRow,nColumn);
		}
	}
}

int StatusCtrl::GetSelectedItem()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int selected=-1;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			selected = nItem;
		}
	}
	//returns -1 if not selected;
	return selected;
}