#include "StdAfx.h"
#include ".\ftd2api.h"

FTD2Api::FTD2Api(void):m_hmodule(NULL)
{
}

FTD2Api::~FTD2Api(void)
{
}

    
BOOL FTD2Api::LoadDLL()
{
    if(m_hmodule != NULL)
        return TRUE;

    m_hmodule = LoadLibrary("Ftd2xx.dll");	
    if(m_hmodule == NULL)
    {
        AfxMessageBox("Error: Can't Load Ftd2xx.dll");
        return FALSE;
    }
    MapFunctions();
    
    TRACE("Loaded Ftd2xx.dll\n");    
    return TRUE;
}

void FTD2Api::FreeDLL()
{
    FreeLibrary(m_hmodule);
	m_hmodule = NULL;
	m_pWrite = NULL;
	m_pRead = NULL;
	m_pOpen = NULL;
	m_pOpenEx = NULL;
	m_pListDevices = NULL;
	m_pClose = NULL;
	m_pResetDevice = NULL;
	m_pPurge = NULL;
	m_pSetTimeouts = NULL;
	m_pGetQueueStatus = NULL;
	m_pSetBaudRate = NULL;
	m_pGetLatencyTimer = NULL;
	m_pSetLatencyTimer = NULL;
	TRACE("Free FTD2Api Dll\n");
}


void FTD2Api::MapFunctions()
{
    m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
    if (m_pWrite == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_Write");
        return;
    }

    m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
    if (m_pRead == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_Read");
        return;
    }

    m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
    if (m_pOpen == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_Open");
        return;
    }

    m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
    if (m_pOpenEx == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_OpenEx");
        return;
    }

    m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
    if(m_pListDevices == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_ListDevices");
        return;
    }

    m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
    if (m_pClose == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_Close");
        return;
    }

    m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
    if (m_pResetDevice == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_ResetDevice");
        return;
    }

    m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
    if (m_pPurge == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_Purge");
        return;
    }

    m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
    if (m_pSetTimeouts == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_SetTimeouts");
        return;
    }

    m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
    if (m_pGetQueueStatus == NULL)
    {
        AfxMessageBox("Error: Can't Find FT_GetQueueStatus");
        return;
    }

    m_pSetBaudRate = (PtrToSetBaudRate)GetProcAddress(m_hmodule, TEXT("FT_SetBaudRate"));
    if (m_pSetBaudRate == NULL)
    {
        AfxMessageBox(TEXT("Error: Can't Find FT_SetBaudRate"));
        return;
    }
    
    m_pGetLatencyTimer= (PtrToGetLatencyTimer)GetProcAddress(m_hmodule, TEXT("FT_GetLatencyTimer"));
    if (m_pGetLatencyTimer == NULL)
    {
        AfxMessageBox(TEXT("Error: Can't Find FT_GetLatencyTimer"));
        return;
    }

    m_pSetLatencyTimer= (PtrToSetLatencyTimer)GetProcAddress(m_hmodule, TEXT("FT_SetLatencyTimer"));
    if (m_pSetLatencyTimer == NULL)
    {
        AfxMessageBox(TEXT("Error: Can't Find FT_SetLatencyTimer"));
        return;
    }

}	

//****************************************************************************************
FT_STATUS FTD2Api::Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
    if (!m_pRead)
    {
        AfxMessageBox("FT_Read is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pRead)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	


//****************************************************************************************
FT_STATUS FTD2Api::Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
    if (!m_pWrite)
    {
        AfxMessageBox("FT_Write is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pWrite)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}	





//****************************************************************************************
FT_STATUS FTD2Api::Open(PVOID pvDevice)
{
    if (!m_pOpen)
    {
        AfxMessageBox("FT_Open is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pOpen)(pvDevice, &m_ftHandle );
}	

//****************************************************************************************
FT_STATUS FTD2Api::OpenEx(PVOID pArg1, DWORD dwFlags)
{
    if (!m_pOpenEx)
    {
        AfxMessageBox("FT_OpenEx is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pOpenEx)(pArg1, dwFlags, &m_ftHandle);
}	


//****************************************************************************************
FT_STATUS FTD2Api::ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
    if (!m_pListDevices)
    {
        AfxMessageBox("FT_ListDevices is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pListDevices)(pArg1, pArg2, dwFlags);
}	


//****************************************************************************************
FT_STATUS FTD2Api::Close()
{
    if (!m_pClose)
    {
        AfxMessageBox("FT_Close is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pClose)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS FTD2Api::ResetDevice()
{
    if (!m_pResetDevice)
    {
        AfxMessageBox("FT_ResetDevice is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pResetDevice)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS FTD2Api::Purge(ULONG dwMask)
{
    if (!m_pPurge)
    {
        AfxMessageBox("FT_Purge is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pPurge)(m_ftHandle, dwMask);
}	



//****************************************************************************************
FT_STATUS FTD2Api::SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
    if (!m_pSetTimeouts)
    {
        AfxMessageBox("FT_SetTimeouts is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pSetTimeouts)(m_ftHandle, dwReadTimeout, dwWriteTimeout);
}	


//****************************************************************************************
FT_STATUS FTD2Api::GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
    if (!m_pGetQueueStatus)
    {
        AfxMessageBox("FT_GetQueueStatus is not valid!"); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pGetQueueStatus)(m_ftHandle, lpdwAmountInRxQueue);
}	

//****************************************************************************************
FT_STATUS FTD2Api::SetBaudRate(DWORD dwBaudRate)
{
    if (!m_pSetBaudRate)
    {
        AfxMessageBox(TEXT("FT_SetBaudRate is not valid!")); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pSetBaudRate)(m_ftHandle, dwBaudRate);
}	

FT_STATUS FTD2Api::GetLatencyTimer(PUCHAR latency)
{
    if (!m_pGetLatencyTimer)
    {
        AfxMessageBox(TEXT("FT_GetLatencyTimer is not valid!")); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pGetLatencyTimer)(m_ftHandle, latency);
}	

FT_STATUS FTD2Api::SetLatencyTimer(UCHAR latency)
{
    if (!m_pSetLatencyTimer)
    {
        AfxMessageBox(TEXT("FT_SetLatencyTimer is not valid!")); 
        return FT_INVALID_HANDLE;
    }

    return (*m_pSetLatencyTimer)(m_ftHandle, latency);
}	
