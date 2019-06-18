#pragma once

#include "ftd2xx.h"

class FTD2Api
{
public:
    FTD2Api(void);
    ~FTD2Api(void);
    
    BOOL LoadDLL();
    void FreeDLL();
    BOOL Open();

    FT_STATUS Open(PVOID);
    FT_STATUS OpenEx(PVOID, DWORD);
    FT_STATUS ListDevices(PVOID, PVOID, DWORD);
    FT_STATUS Close();
    FT_STATUS Read(LPVOID, DWORD, LPDWORD);
    FT_STATUS Write(LPVOID, DWORD, LPDWORD);
    FT_STATUS ResetDevice();
    FT_STATUS Purge(ULONG);
    FT_STATUS SetTimeouts(ULONG, ULONG);
    FT_STATUS GetQueueStatus(LPDWORD);
    FT_STATUS SetBaudRate(DWORD);
    FT_STATUS GetLatencyTimer(PUCHAR latency);
    FT_STATUS SetLatencyTimer(UCHAR latency);

protected:
    HMODULE m_hmodule;
    FT_HANDLE m_ftHandle;
        
    void MapFunctions();

    typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
    PtrToOpen m_pOpen;     

    typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
    PtrToOpenEx m_pOpenEx; 
   
    typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
    PtrToListDevices m_pListDevices; 
   
    typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
    PtrToClose m_pClose;

    typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
    PtrToRead m_pRead;

    typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
    PtrToWrite m_pWrite;

    typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
    PtrToResetDevice m_pResetDevice;
   
    typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
    PtrToPurge m_pPurge;
    
    typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
    PtrToSetTimeouts m_pSetTimeouts;
  
    typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
    PtrToGetQueueStatus m_pGetQueueStatus;
 
    typedef FT_STATUS (WINAPI *PtrToSetBaudRate)(FT_HANDLE, DWORD);
    PtrToSetBaudRate m_pSetBaudRate; 

    typedef FT_STATUS (WINAPI *PtrToGetLatencyTimer)(FT_HANDLE ftHandle, PUCHAR pucTimer);
    PtrToGetLatencyTimer m_pGetLatencyTimer;
    
    typedef FT_STATUS (WINAPI *PtrToSetLatencyTimer)(FT_HANDLE ftHandle, UCHAR ucTimer);
    PtrToSetLatencyTimer m_pSetLatencyTimer;
};
