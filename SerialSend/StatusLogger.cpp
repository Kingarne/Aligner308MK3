#include "StdAfx.h"
#include "StatusLogger.h"
#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

StatusLog::StatusLog( LogType type, CString msg )
	: m_type(type)
{
	SYSTEMTIME stime;
	GetSystemTime(&stime);

	m_strTime.Format("[%.2d:%.2d:%.2d:%.3d]", stime.wHour,stime.wMinute,stime.wSecond, stime.wMilliseconds);
	m_strMsg = msg;
}

StatusLoggerPtr StatusLogger::m_pInstance = StatusLoggerPtr();

StatusLogger::StatusLogger()
	: m_hWnd(NULL)
{

}

StatusLogger* StatusLogger::GetInstance()
{
	if(!m_pInstance.get())
		m_pInstance = StatusLoggerPtr( new StatusLogger() );

	return m_pInstance.get();
}

void StatusLogger::AddMessage( LogType type, CString msg )
{
	if(m_hWnd == NULL)
		return;

	StatusLog* pLog = new StatusLog(type, msg);
	::PostMessage(m_hWnd, STATUS_MSG, (WPARAM)pLog, 0);
}

void StatusLogger::SetHWnd( HWND hWnd )
{
	m_hWnd = hWnd;
}



