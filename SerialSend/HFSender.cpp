#include "StdAfx.h"
#include ".\HFSender.h"

HFSender::HFSender(void)
{	
	m_hThread = NULL;
	m_callback = NULL;
	m_pCallbackData = NULL;
    m_sendFreq = 0.0f;
    m_continueLoop = true;
    ::QueryPerformanceFrequency(&m_frequence);
    ::QueryPerformanceCounter(&m_lastTime);
}

HFSender::~HFSender(void)
{
	Destroy();
}

int HFSender::Create()
{
  if(m_hThread != NULL)
    return 1;
  
  InitializeCriticalSection(&m_criticalSection);
  m_createEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  m_destroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  m_dataEvent = CreateEvent(NULL, FALSE, FALSE, NULL);


  m_hThread = ::CreateThread(NULL, 0, StaticThreadFunc, this, 0, 0);
  if(!m_hThread)
    return 0 ;
   
  DWORD nCreateCode = ::WaitForSingleObject((HANDLE)m_createEvent,2000);
  switch(nCreateCode)
  {
    case WAIT_OBJECT_0:
      TRACE("Created OK\n");
    break;
    case WAIT_TIMEOUT:
      TRACE("Something wrong!\n");
      return 0;      
      break;
    default: break;
  }


  return 1;
}

int HFSender::Destroy()
{
  if(!m_hThread)
    return 1;

  m_bContinue = false;
  m_continueLoop = false;
  SetEvent(m_destroyEvent);  
  if(::WaitForSingleObject(m_hThread,5000) == WAIT_TIMEOUT)
  {
    TRACE("Destroy Thread\n");
    ::TerminateThread(m_hThread,-1);
   // delete m_pThread;
    return 0;
  }
  TRACE("Destroy thread\n");
  
  
  return 1;  
}

void HFSender::SetFrequence(double f)
{
    if(f==0.0f)
        m_continueLoop = false;    
    m_sendFreq = f;
}

void HFSender::SetCallback(SENDCALLBACK callback, PVOID pData)
{
  m_callback = callback;
  m_pCallbackData = pData;
}

int HFSender::ThreadFunc()
{
	int res=0;
	TRACE("Thread started!\n");
	SetEvent(m_createEvent);
	
	m_bContinue = true;
	int nWaitCode;

    while(m_bContinue)
    {
        TRACE("Thread\n");    
        
        if(m_sendFreq > 0.0f)
        {
            SendLoop();
        }
        
        
        Sleep(500);
    }
    
    // 	HANDLE Objects[2];
// 	Objects[0] = (HANDLE)m_destroyEvent;
// 	Objects[1] = (HANDLE)m_dataEvent;
	return 1;
}

#include "NStopWatch.h"
NStopWatch sw;
CString trace;


void HFSender::SendLoop()
{
    double timeInt = 1.0f/m_sendFreq*1000.0f;    
    
    LARGE_INTEGER time1,time2;
    ::QueryPerformanceCounter(&time1);
    int count = 1;
    while(m_continueLoop)
    {
        
        ::QueryPerformanceCounter(&time2);        
        double elapsedTime = ((double) (time2.QuadPart - time1.QuadPart) * 1000.0f)  /(double) m_frequence.QuadPart;	// ms
        //Sleep(500);

        if(elapsedTime/(double)count > timeInt)
        {
//             trace.Format("t:%.5f, timeInt:%.3f",elapsedTime/(double)count,timeInt);
//             sw.TraceTime(TRUE, TRUE, trace);

            if(m_callback)
                (*m_callback)(m_pCallbackData, 0);
        
            TRACE("e:%.8f\n",elapsedTime/count);
            count++;
            //time1 = time2;
        }
        Sleep(0);
    }

    m_continueLoop = true;

}


