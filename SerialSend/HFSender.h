#pragma once


typedef void (*SENDCALLBACK)(PVOID pData, LPARAM lp);


class HFSender
{
public:
	HFSender(void);
	~HFSender(void);

	int Create();
	int Destroy();
	void SetCallback(SENDCALLBACK callback, PVOID pData);
	void SetFrequence(double f);

protected:
	
	HANDLE m_hThread;
	//CWinThread *m_pThread;
	static DWORD WINAPI StaticThreadFunc(LPVOID pParam){
		return ((HFSender*)pParam)->ThreadFunc();
	}

	int ThreadFunc();
    void SendLoop();

	HANDLE m_createEvent;
	HANDLE m_destroyEvent;
	HANDLE m_dataEvent;
	CRITICAL_SECTION m_criticalSection;
	
	PVOID m_pCallbackData;
	SENDCALLBACK m_callback;
	
	bool m_bContinue;
    bool m_continueLoop;
    double m_sendFreq;

    LARGE_INTEGER m_frequence;
    LARGE_INTEGER m_lastTime;

};

