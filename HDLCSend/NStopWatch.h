
#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

enum Nmode{NmodeNormal,NmodeAuto};

#define PerfCountLowPart (NStopWatch::GetPerformanceCounter().LowPart) 
#define PerfCountHiPart (NStopWatch::GetPerformanceCounter().HighPart) 
#define PerfCount PerfCountHiPart<<"-"<<PerfCountLowPart

//std::ofstream traceOut("trace.txt", std::ios_base::out | std::ios_base::app);

class NStopWatch
{
public:
    
  NStopWatch(Nmode mode=NmodeNormal)
  {
    m_mode=mode;
    ::QueryPerformanceFrequency(&m_frequence);
    Start();
  }
  
  ~NStopWatch(void)
  {
    if (m_mode==NmodeAuto)
    {
	    Stop();
	    TraceTime();
    }
  }

  void Start()
  {
    ::QueryPerformanceCounter(&m_startCount);
    m_tagCount=m_startCount;
  }
  
  void Stop()
  {
    ::QueryPerformanceCounter(&m_stopCount);
    if (m_frequence.QuadPart == 0) 
    {
	    m_elapsedTime = (double) (m_stopCount.QuadPart - m_startCount.QuadPart);
    }
    else 
    {
	    m_elapsedTime = ((double) (m_stopCount.QuadPart - m_startCount.QuadPart) * 1000.0) / (double) m_frequence.QuadPart;	// ms		
    }
  }

  static std::ofstream traceOut;
  static void OpenTrace()
  {
    traceOut.open("trace.txt", std::ios_base::out | std::ios_base::app);
  }
  static void CloseTrace()
  {    
    traceOut.flush();
    traceOut.close();
  }
  
  
  static LARGE_INTEGER GetPerformanceCounter()
  {
    LARGE_INTEGER tempCount;
    ::QueryPerformanceCounter(&tempCount);
	  return tempCount;
  }

  void TimeTag()
  { 
    LARGE_INTEGER tempCount = m_tagCount;
    ::QueryPerformanceCounter(&m_tagCount);
	  m_intermediateTime = ((double) (m_tagCount.QuadPart - tempCount.QuadPart) * 1000.0) / (double) m_frequence.QuadPart;	// ms     
  }

  double GetElapsedTime(){return m_elapsedTime;}
  double GetIntermediateTime(){return m_intermediateTime;}
  
  void TraceTime(BOOL bToFile=FALSE, BOOL timeStamp=FALSE, CString tag="", BOOL showElapsed=TRUE)
  {
    if(bToFile)
    {
      std::ofstream out("time.txt", std::ios_base::out | std::ios_base::app);
      if(timeStamp)
      {
        SYSTEMTIME time;
        GetLocalTime(&time);
        out << "(" <<  std::setfill('0') << std::setw(2) << time.wDay << "/" << std::setfill('0') << std::setw(2) << time.wMonth << " " << std::setfill('0') << std::setw(2) << time.wHour << ":" << std::setfill('0') << std::setw(2) << time.wMinute << ":"  << std::setfill('0') << std::setw(2) << time.wSecond << ":" << std::setfill('0') << std::setw(3) << time.wMilliseconds << ") "; 
        
      }
      if(showElapsed)
      {
        out << "Time: " << GetElapsedTime() << " ms " << (LPCTSTR)tag << std::endl;
      }
      else
      {
        out << (LPCTSTR)tag << std::endl;
      }
      out.flush();
      out.close();
    }else{  
    }
  }
  void TraceIntermediateTime(BOOL bToFile=FALSE, BOOL timeStamp=FALSE, CString tag="")
  {
    if(bToFile)
    {
      std::ofstream out("time.txt", std::ios_base::out | std::ios_base::app);
      if(timeStamp)
      {
        SYSTEMTIME time;
        GetLocalTime(&time);        
        out << "(" <<  std::setfill('0') << std::setw(2) << time.wHour << ":" << std::setfill('0') << std::setw(2) << time.wMinute << ":"  << std::setfill('0') << std::setw(2) << time.wSecond << ":" << std::setfill('0') << std::setw(3) << time.wMilliseconds << ") "; 
        
      }
      out << (LPCTSTR)tag << ": " << m_intermediateTime << " ms" << std::endl;
      out.close();
    }else{     
    }
  }
  
  void StopAndTraceTime(BOOL bToFile=FALSE)
  {
    Stop();
    TraceTime(bToFile);
  }
  void Trace()
  {
    TraceTime(TRUE);
  }

  double GetCurrentTime()
  {
    LARGE_INTEGER current;   
    ::QueryPerformanceCounter(&current);
    return (double)current.QuadPart / (double)m_frequence.QuadPart;
  }
protected:

  Nmode m_mode;
  LARGE_INTEGER m_startCount;
  LARGE_INTEGER m_stopCount;
  LARGE_INTEGER m_tagCount;
  LARGE_INTEGER m_frequence;

  double m_intermediateTime;
  double m_elapsedTime;
};

  