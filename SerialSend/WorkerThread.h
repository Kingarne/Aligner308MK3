#pragma once

#include "afxmt.h"

typedef void (*PCALLBACK)(PVOID pData, WPARAM wp, LPARAM lp);

class WorkerThread
{
public:
	WorkerThread();
	virtual ~WorkerThread(void);

	int Create();	
	BOOL SetCallback( PCALLBACK pCallback, PVOID pData );
	virtual void Exit(){}

protected:
	bool m_done;	

	PCALLBACK m_pCallback;
	PVOID m_pCallbackData;  
	CWinThread *m_pThread;
	static UINT StaticThreadFunction(LPVOID lParam)
	{
		return ((WorkerThread*)lParam)->ThreadFunction();
	}
	virtual UINT ThreadFunction();
	BOOL m_bExit;
	CEvent m_ExitEvent;
	CEvent m_CreateEvent;

};

