//----------------------------------------------------------------------------
// File: QuaDef.H
//----------------------------------------------------------------------------
// 
// Description: 
//   Contains structures, enumerations, error codes and defines used by 
//   SYS, DLL, and applications.
// 
// Copyright(C) 2002 Quatech, Inc.
// Copyright(C) 2002 Intelligraphics, Inc.
//----------------------------------------------------------------------------

#if !defined(__QUADEF_H__)
#define __QUADEF_H__

#define MAX_DEVICES_SUPPORTED       0x10        // Number of instances we support.

typedef struct tagMPA_HANDLE
{
    ULONG   structLen;
    HANDLE  hndDevice;
    BOOLEAN overlapped;
} MPA_HANDLE, *PMPA_HANDLE;

typedef ULONG MPA_FRAME_COUNT, *PMPA_FRAME_COUNT;   // For Rx only

typedef ULONG MPA_FRAME_SIZE, *PMPA_FRAME_SIZE;     // For Rx only.

typedef struct tagMPA_RESET_FRAMES
{
    ULONG   structLen;
    BOOLEAN tx;
    BOOLEAN rx;
} MPA_RESET_FRAMES, *PMPA_RESET_FRAMES;

typedef struct tagMPA_NODE_ADDRESS
{
    ULONG   structLen;
    union
    {
        struct
        {
            // Set the following bits to indicate which parameter(s) are to be 
            // set.  Parameter value(s) with corresponding bits set to zero are 
            // not changed.
            UCHAR addr              : 1;
            UCHAR enable            : 1;
            UCHAR pad               : (8*sizeof(UCHAR) - 2);
        } bits;
        ULONG all;  // Used to clear/set all bits at once.
    } set;

    UCHAR addr;
    BOOLEAN enable;
} MPA_NODE_ADDRESS, *PMPA_NODE_ADDRESS;

// Same bit ordering as in WRB so we can use shift operator to set.
typedef enum tagCLOCK_SOURCE
{
    RTXC, 
    TRXC, 
    BAUD_RATE_GEN, 
    DPLL
} CLOCK_SOURCE;

typedef enum tagTRXC_OUTPUT
{
    CRYSTAL_OUT, 
    TX_CLOCK, 
    BAUD_RATE_GEN_OUT, 
    DPLL_OUT
} TRXC_OUTPUT;

// Same bit ordering as in WRA so we can use shift operator to set.
typedef enum tagENCODING
{
    NRZ, 
    NRZI, 
    FM1, 
    FM0
} ENCODING;

typedef enum tagDPLL_CLOCK_SOURCE
{
    DPLL_SRC_BAUD_RATE_GEN, 
    DPLL_SRC_RTXC
} DPLL_CLOCK_SOURCE;

typedef enum tagDPLL_MODE
{
    DPLL_MODE_NRZI, 
    DPLL_MODE_FM
} DPLL_MODE;

typedef enum tagIDLE_LINE_CONTROL
{
    IDLE_LINE_FLAGS, 
    IDLE_LINE_ONES
    // ESCC doesn't support sending continuous 0s.
} IDLE_LINE_CONTROL;

typedef struct tagMPA_CONFIG
{
    ULONG   structLen;
    union
    {
        struct
        {
            // Set the following bits to indicate which parameter(s) are to be 
            // set.  Parameter value(s) with corresponding bits set to zero are 
            // not changed.
            // Tx does not require buffers since pending IRPs provide buffers.
            UCHAR FrameBufferSizeRx : 1;
            UCHAR NumFrameBuffersRx : 1;
            UCHAR BaudRate          : 1;
            UCHAR ClockRate         : 1;
            UCHAR ClockMode         : 1;
            UCHAR InternalLoopback  : 1;
            UCHAR RxClockSource     : 1;
            UCHAR TxClockSource     : 1;
            UCHAR TRXCOutput        : 1;
            UCHAR Encoding          : 1;
            UCHAR DPLLClockSource   : 1;
            UCHAR DPLLMode          : 1;
            UCHAR AutoRTSDeact      : 1;
            UCHAR CRCPreset         : 1;
            UCHAR IdleLineControl   : 1;
            ULONG pad               : (8*sizeof(ULONG) - 13);
        } bits;
        ULONG all;  // Used to clear/set all bits at once.
    } set;

    ULONG             FrameBufferSizeRx;      // In bytes.
    ULONG             NumFrameBuffersRx;
    ULONG             BaudRate;
    ULONG             ClockRate;
    ULONG             ClockMode;
    BOOLEAN           InternalLoopback;
    CLOCK_SOURCE      RxClockSource;
    CLOCK_SOURCE      TxClockSource;
    TRXC_OUTPUT       TRXCOutput;
    ENCODING          Encoding;
    DPLL_CLOCK_SOURCE DPLLClockSource;
    DPLL_MODE         DPLLMode;
    BOOLEAN           AutoRTSDeact;
    BOOLEAN           CRCPreset;
    IDLE_LINE_CONTROL IdleLineControl;
} MPA_CONFIG, *PMPA_CONFIG;

typedef struct tagMPA_MODEM_CNTL_SIG
{
    ULONG   structLen;
    union
    {
        struct
        {
            // Set the following bits to indicate which parameter(s) are to be 
            // set.  Parameter value(s) with corresponding bits set to zero are 
            // not changed.
// removed support for the following three bits of the COMM register            
//            UCHAR testModeStatus    : 1;
//            UCHAR localLoopbackEn   : 1;
//            UCHAR remoteLoopbackEn  : 1;

            UCHAR extSyncEn         : 1;
            UCHAR rxClkSource       : 1;
            UCHAR txClkSource       : 1;
            UCHAR rxBufEnable       : 1; // enable RX buffers
            UCHAR txBufEnable       : 1; // enable TX buffers

            UCHAR rts               : 1;
            UCHAR dtr               : 1;
            UCHAR cts               : 1;
            UCHAR dcd               : 1;
            ULONG pad               : (8*sizeof(USHORT) - 10);
        } bits;
        USHORT all; // Used to clear/set all bits at once.
    } set;

    union
    {
        struct
        {
            // On-board modem control signals in Communications Register.
// removed support for the following three bits of the COMM register            
//            UCHAR testModeStatus    : 1;
//            UCHAR localLoopbackEn   : 1;
//            UCHAR remoteLoopbackEn  : 1;

            UCHAR extSyncEn         : 1;
            UCHAR rxClkSource       : 1;
            UCHAR txClkSource       : 1;
            UCHAR rxBufEnable       : 1; // enable rx buffers on card
            UCHAR txBufEnable       : 1; // enable tx buffers on card

            // On-chip modem control signals
            UCHAR rts               : 1;
            UCHAR dtr               : 1;
            UCHAR cts               : 1;
            UCHAR dcd               : 1;
            UCHAR pad               : (8*sizeof(USHORT) - 10);
        } bits;
        USHORT all; // Used to clear/set all bits at once.
    } signals;
} MPA_MODEM_CNTL_SIG, *PMPA_MODEM_CNTL_SIG;

typedef struct tagMPA_DRIVER_VERSION
{
    ULONG   structLen;
    // Driver version is in format major.minor.
    UCHAR  major;
    UCHAR  minor;
} MPA_DRIVER_VERSION, *PMPA_DRIVER_VERSION;

// Error codes
typedef enum tagMPA_ERROR_CODE
{
    ERR_NO_ERROR, 
    ERR_COULDNT_OPEN_DEVICE, 
    ERR_DEVICE_NOT_OPENED, 
    ERR_COULDNT_CLOSE_DEVICE, 
    ERR_IOCTL_ERROR, 
    ERR_BAD_RETURN_SIZE, 
    ERR_NOTHING_TO_DO, 
    ERR_INCORRECT_NUM_BYTES_WRITTEN, 
    ERR_INCORRECT_NUM_BYTES_READ, 
    ERR_ERROR_WRITING_DATA, 
    ERR_ERROR_READING_DATA, 
    ERR_WRITE_PENDING, 
    ERR_READ_PENDING, 
    ERR_MEM_ALLOC_ERROR, 
    ERR_IO_STILL_PROGRESSING, 
    ERR_TRANSFER_STATUS, 
    ERR_INVALID_PARAMETERS_PASSED, 
    ERR_INVALID_FOR_NON_OVERLAPPED, 
    ERR_DATA_OVERRUN,        // more recieve packets came than buffers were available
    ERR_DATA_CRC,            // CRC error detected in data
    ERR_INVALID_STRUCT_SIZE, 
    // Add more here
    NUM_ERROR_CODES
} MPA_ERROR_CODE;

#endif // !defined(__QUADEF_H__)

