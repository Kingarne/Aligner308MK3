//----------------------------------------------------------------------------
// File: QuaDLL.H
//----------------------------------------------------------------------------
// 
// Description: 
//   Header file for the functions that comprise the API.  Also device name.
//
// Copyright(C) 2002 Quatech, Inc.
// Copyright(C) 2002 Intelligraphics, Inc.
//----------------------------------------------------------------------------

#include <windows.h>
#include <winioctl.h>
#include "QuaDef.h"

#ifdef MPADLL_EXPORTS
     #include "QuaIOCTL.h"
#endif // end of MPADLL_EXPORTS



// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MPADLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MPADLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MPADLL_EXPORTS
#define MPADLL_API __declspec(dllexport)
#else
#define MPADLL_API __declspec(dllimport)
#endif

// Extra NULL terminator is to allow space for increment number (up to Fh max).
#define DEVICE_NAME_PREFIX      "\\\\.\\MPADev\0\0"


#ifdef __cplusplus
extern "C" {
#endif


MPADLL_API MPA_ERROR_CODE WINAPI MPA_Open
(
    OUT PMPA_HANDLE pHandle, 
    IN  UCHAR       deviceNumber, 
    IN  BOOLEAN     overlapped
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_Close
(
    IN const PMPA_HANDLE pHandle
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_PutData
(
    IN  const PMPA_HANDLE pHandle, 
    IN  const PUCHAR pBuffer, 
    IN  ULONG  bufferLen, 
    OUT ULONG  *pBytesWritten, 
    IN  LPOVERLAPPED lpOverlapped
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetData
(
    IN  const PMPA_HANDLE pHandle, 
    OUT UCHAR  *pBuffer, 
    IN  ULONG  bufferLen, 
    OUT ULONG  *pBytesRead, 
    IN  LPOVERLAPPED lpOverlapped
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetTransferStatus
(
    IN  const PMPA_HANDLE  pHandle, 
    IN  LPOVERLAPPED lpOverlapped, 
    OUT ULONG        *pBytesTransferred, 
    IN  BOOL         wait
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_ResetFrames
(
    IN const PMPA_HANDLE pHandle, 
    IN const PMPA_RESET_FRAMES pReset
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_SetNodeAddr
(
    IN  const PMPA_HANDLE pHandle, 
    IN  PMPA_NODE_ADDRESS pAddrIn, 
    OUT PMPA_NODE_ADDRESS pAddrOut
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetFrameCount
(
    IN  const PMPA_HANDLE pHandle, 
    OUT PULONG pFrameCount
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetFrameSize
(
    IN  const PMPA_HANDLE pHandle, 
    OUT PMPA_FRAME_SIZE pFrameSize
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetConfig
(
    IN  const PMPA_HANDLE pHandle, 
    OUT PMPA_CONFIG pConfigOut
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_SetConfig
(
    IN  const PMPA_HANDLE pHandle, 
    IN  const PMPA_CONFIG pConfigIn, 
    OUT PMPA_CONFIG pConfigOut
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetModemControlSignals
(
    IN  const PMPA_HANDLE pHandle, 
    OUT PMPA_MODEM_CNTL_SIG pModemCntlSig
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_SetModemControlSignals
(
    IN const PMPA_HANDLE pHandle, 
    IN const PMPA_MODEM_CNTL_SIG pModemCntlSig, 
    OUT PMPA_MODEM_CNTL_SIG pModemCntlSigOut
);

MPADLL_API MPA_ERROR_CODE WINAPI MPA_GetDriverVersion
(
    IN  const PMPA_HANDLE pHandle, 
    OUT PMPA_DRIVER_VERSION pVersion
);

#ifdef __cplusplus
}
#endif
