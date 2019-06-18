#pragma once
#include <memory>
#define STATUS_MSG				WM_APP+18
enum LogType
{
	LogDebug	= 1<<0,
	LogInfo		= 1<<1,
	LogWarning	= 1<<2,
	LogError	= 1<<3,
	LogTcpIp	= 1<<4
};
#define LogAll (LogDebug|LogInfo|LogWarning|LogError|LogTcpIp)

//Struct to create a status log
//Note that the timestamp is set when instantiating
struct StatusLog
{
	StatusLog(LogType type, CString msg);

	const LogType m_type;
	CString m_strMsg;
	CString m_strTime;
};

class StatusLogger;
typedef std::unique_ptr<StatusLogger> StatusLoggerPtr;

//Using singleton pattern /BA
class StatusLogger
{
public:
	static StatusLogger* GetInstance();	//Returns the one and only instance

	void SetHWnd(HWND hWnd);
	HWND GetHWnd(){return m_hWnd;}
	void AddMessage(LogType type, CString msg);

private:
	StatusLogger();

	static StatusLoggerPtr m_pInstance;	//Storing it in a unique_ptr to make sure the destructor is called /BA

	HWND m_hWnd;
};