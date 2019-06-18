#include "StdAfx.h"
#include "WorkerThread.h"


WorkerThread::WorkerThread(void):m_done(false),m_pThread(NULL)
{
}


WorkerThread::~WorkerThread(void)
{
	m_pThread = NULL;
}


int WorkerThread::Create()
{
	int res = 0;
	if(m_pThread)
		return 0;

	m_pThread = ::AfxBeginThread(StaticThreadFunction,this);
	//Wait for thread to start
	DWORD nRet = ::WaitForSingleObject((HANDLE)m_CreateEvent,2000);
	switch(nRet)
	{
	case WAIT_OBJECT_0:
		TRACE("Initiating OK \n");
		break;
	case WAIT_TIMEOUT:
		TRACE("Something wrong!\n");
		res = -1;
		break;
	default: break;
	}

	return res;
}

UINT WorkerThread::ThreadFunction()
{
	TRACE(" Thread started\n");  
	//Signal create event
	m_CreateEvent.SetEvent();

	const DWORD dwNumEvents = 2;
	DWORD dwEvent;
	HANDLE Events[dwNumEvents];
	Events[0] = (HANDLE)m_ExitEvent;	
	while(!m_bExit)
	{
		dwEvent = ::WaitForMultipleObjects(dwNumEvents,Events,FALSE,INFINITE);
		switch(dwEvent)
		{
		case WAIT_OBJECT_0: // Exit event
			m_bExit = true;
			TRACE("Exit event\n");
			break;
		case WAIT_OBJECT_0 + 1: // Image event
			//      TRACE("Image event\n");

			break;
		default:break;
		}
		//    TRACE("Filegrabber\n");
		//::Sleep(1000);
	}
	TRACE("Terminating  thread");  

	return 1;
}


BOOL WorkerThread::SetCallback( PCALLBACK pCallback, PVOID pData ) 
{
	m_pCallback = pCallback;
	m_pCallbackData = pData;
	return 1;
}