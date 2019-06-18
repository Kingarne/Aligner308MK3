// HDLCSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HDLCSend.h"
#include "HDLCSendDlg.h"
#include ".\hdlcsenddlg.h"
//#include "NStopWatch.h"
#include <complex>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NStopWatch sw;
// CString trace;

long DegToMsgAngle(double Angle)
{
    long		LI ;

    //	LSB shall equal 90 * 2^-18 
    Angle = Angle * pow(2, 18) ;
    Angle = Angle / 90 ;
    //	LSB shall be positioned in bit 13 in a 32b long word
    Angle = Angle * pow(2, 13) ;	// Shift left 13 positions

    if (Angle < 0.0) {
        Angle = -1.0 * Angle ;
        LI = Angle ;
        LI = -1 * LI ;
    }
    else
        LI = Angle ;

    return LI ;
}


double MsgAngleToDeg(long Angle)
{
    double	FloatAngle;
    long	LI ;

    LI = Angle ;
    //	Change LSB from bit 13 to bit 0 in the 32b long word
    LI = LI / pow(2,13) ;	// Shift right 13 positions
    //	LSB equals 90 * 2^-18 
    FloatAngle = LI ;
    FloatAngle = FloatAngle * 90 ;
    FloatAngle = FloatAngle / pow(2, 18) ;
    return FloatAngle;
}


void init_config_array(MPA_CONFIG& config)
{
    // Set up all initial values in the configuration array.
    config.set.all = 0;
    config.InternalLoopback = FALSE;
    config.set.bits.InternalLoopback = TRUE;
    config.CRCPreset = 0;
    config.set.bits.CRCPreset = TRUE;
    config.structLen = sizeof(MPA_CONFIG);
    config.TxClockSource = BAUD_RATE_GEN;
    config.set.bits.TxClockSource = TRUE;
    config.RxClockSource = BAUD_RATE_GEN;
    config.set.bits.RxClockSource = TRUE;
    config.Encoding = NRZ;
    config.set.bits.Encoding = TRUE;
    config.set.bits.BaudRate = TRUE;
    config.BaudRate = 115200;//153600; // KLABE-HACK
    config.set.bits.ClockMode = TRUE;
    config.ClockMode = 1;
    config.set.bits.ClockRate = TRUE;
    config.ClockRate = 9830400;
    config.IdleLineControl = IDLE_LINE_ONES; //IDLE_LINE_FLAGS;
    config.set.bits.IdleLineControl = TRUE;
    config.set.bits.FrameBufferSizeRx = TRUE;
    config.FrameBufferSizeRx = 0;//BufferLen;    
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHDLCSendDlg dialog



CHDLCSendDlg::CHDLCSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHDLCSendDlg::IDD, pParent)
    , m_numSentFrames(0)
    , m_info(_T(""))    
    , m_rollStr(_T("2.2"))
    , m_pitchStr(_T("3.3"))
    , m_cycle(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_timer = 0;
    m_seqNum = 0;
    m_frameBuf = NULL;
    m_ver.major = 0;
    m_ver.minor = 0;
    m_dumpData = FALSE;
    m_test = 0;
}

void CHDLCSendDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, ID_FRAME_RATE_COMB, m_frameRateCombo);
    DDX_Text(pDX, IDC_SENT_FRAMES, m_numSentFrames);
    DDX_Text(pDX, IDC_INFO, m_info);    
    DDX_Text(pDX, IDC_ROLL_EDIT, m_rollStr);
    DDX_Text(pDX, IDC_PITCH_EDIT, m_pitchStr);
    DDX_Check(pDX, IDC_CYCLE, m_cycle);
}

BEGIN_MESSAGE_MAP(CHDLCSendDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(ID_FRAME_RATE_COMB, OnCbnSelchangeFrameRateComb)
    ON_BN_CLICKED(IDC_SEND_ONE_BUTTON, OnBnClickedSendOneButton)
    ON_BN_CLICKED(IDC_DAU_BUTTON, OnBnClickedDauButton)
    ON_BN_CLICKED(IDC_DUMP_CHECK, OnBnClickedDumpCheck)    
    ON_EN_KILLFOCUS(IDC_TEST, OnEnKillfocusTest)
    ON_BN_CLICKED(IDC_CYCLE, OnBnClickedCycle)
END_MESSAGE_MAP()


void staticCallback(PVOID pData, LPARAM lp)
{
    CHDLCSendDlg* pDlg = (CHDLCSendDlg*)pData;
    pDlg->Callback(lp);
}

// CHDLCSendDlg message handlers

BOOL CHDLCSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    if(InitSyncDriver() == ERR_NO_ERROR)
    {
        //m_timer = SetTimer(1, 1000, NULL);
    }
    m_info.Format("Driver version:%d.%d",m_ver.major, m_ver.minor);

    m_frameRateCombo.AddString("0 Hz");
    m_frameRateCombo.AddString("1 Hz");
    m_frameRateCombo.AddString("2 Hz");
    m_frameRateCombo.AddString("5 Hz");
    m_frameRateCombo.AddString("10 Hz");
    m_frameRateCombo.AddString("30.5 Hz");
    m_frameRateCombo.AddString("60 Hz");
    m_frameRateCombo.AddString("100 Hz");
    m_frameRateCombo.AddString("200 Hz");

    m_frameRateCombo.SetCurSel(0);


    long r = DegToMsgAngle(2.3f);
    double roll = MsgAngleToDeg(r);
    
    m_dauDlg.m_pParent = this;
    m_dauDlg.Create(IDD_DAU_DIALOG);

    CButton* pButt = (CButton*)GetDlgItem(IDC_DUMP_CHECK);
    pButt->SetCheck(m_dumpData);

    CWnd* pWnd  = GetDlgItem(IDC_TEST);
    CString str;
    str.Format("%d",m_test);
    pWnd->SetWindowText(str);    
    
    m_HFSender.SetCallback(&staticCallback, this);
    m_HFSender.Create();       

    UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHDLCSendDlg::Callback(LPARAM lp)
{
    TRACE("Callback\n");
    m_dauDlg.UpdateValues();
    m_dauDlg.Send();
}



unsigned char crc_bits(unsigned char crc, unsigned char data)
{
    unsigned char i;
    i = (data ^ crc) & 0xff;
    crc = 0;
    if(i & 1)
        crc ^= 0x5e;
    if(i & 2)
        crc ^= 0xbc;
    if(i & 4)
        crc ^= 0x61;
    if(i & 8)
        crc ^= 0xc2;
    if(i & 0x10)
        crc ^= 0x9d;
    if(i & 0x20)
        crc ^= 0x23;
    if(i & 0x40)
        crc ^= 0x46;
    if(i & 0x80)
        crc ^= 0x8c;
    return crc;
}



int MakeFrame(unsigned char *s, int seq_numb)
{
    int len, i;
    unsigned char crc = 0xFF, data;

    len = 25;//rand();
    len &= 0x1F;
    len += 3;
    //     len = 64;
    *s = len;
    crc = crc_bits(crc, *s++);

    //if (rand() % 2) data = 0; else data = 255;

    printf("SEQ 0x");
    *s = (unsigned)((seq_numb>>24)&0xFF);
    printf("%x", *s);
    crc = crc_bits(crc, *s++);
    *s = (unsigned)((seq_numb>>16)&0xFF);
    printf("%x", *s);
    crc = crc_bits(crc, *s++);
    *s = (unsigned)((seq_numb>>8)&0xFF);
    printf("%x", *s);
    crc = crc_bits(crc, *s++);
    *s = (unsigned)((seq_numb>>0)&0xFF);
    printf("%x", *s);
    crc = crc_bits(crc, *s++);
    int counter=0;
    for (i=0;i<len;i++) {
        //*s = (unsigned char)rand();
        *s = (counter++) & 0xFF;
        //         *s = data;
        crc = crc_bits(crc, *s++);
    }
    *s++ = crc;

    printf("Payload length = %d, tot = %d\n", len, len+6);

    len += 6;

    return len;
}

void ResetFrames(PMPA_HANDLE pHandle, BOOLEAN tx, BOOLEAN rx)
{
    MPA_ERROR_CODE  result;
    MPA_RESET_FRAMES reset;

    if (tx && rx)
        printf("Resetting Tx and Rx frame buffers.\n");
    else if (tx)
        printf("Resetting Tx frame buffer.\n");
    else if (rx)
        printf("Resetting Rx frame buffer.\n");
    else
        printf("ERROR: Resetting neither frame buffer.\n");

    reset.structLen = sizeof(MPA_RESET_FRAMES);
    reset.tx = tx;
    reset.rx = rx;
    if ((result = MPA_ResetFrames(pHandle, &reset)) != ERR_NO_ERROR)
    {
        printf("ERROR: Unable to reset Tx and Rx frame buffers. Error code %d\n", result);
    }


}


int CHDLCSendDlg::InitSyncDriver()
{
    MPA_ERROR_CODE	err;
    BOOLEAN			overlapped = 0;    
    
    int n_frames_sent = 0, len, i;
    int deviceNumber = 0; //just for one device
    
    MPA_CONFIG outConfig;
    outConfig.structLen = sizeof(MPA_CONFIG);    
    // Open first device
    m_drvHandle.structLen = sizeof(MPA_HANDLE);
    err =  MPA_Open(&m_drvHandle, deviceNumber, overlapped);
    if(err != ERR_NO_ERROR)
    {
        TRACE("ERROR: Unable to open first device. Error code %d.\n", err);
        SyncDriveExit();
        return err;
    }


//     trace.Format("Device %d open",deviceNumber);
//     sw.TraceTime(TRUE, TRUE, trace);

    m_frameBuf = (unsigned char *)(malloc(256));
    if(m_frameBuf == NULL)
    {
        printf("Cannot allocate m_frameBuf!\n");
        SyncDriveExit();
    }


    // Check the SyncDrive version.

    m_ver.structLen = sizeof(MPA_DRIVER_VERSION);
    err = MPA_GetDriverVersion(&m_drvHandle, &m_ver);
    if(err != ERR_NO_ERROR)
    {
        TRACE("Syncdrive release level error!\n\n");
    }
    TRACE("Driver release level is %d.%d\n", m_ver.major, m_ver.minor);

//     trace.Format("Driver version %d.%d",m_ver.major, m_ver.minor);
//     sw.TraceTime(TRUE, TRUE, trace);

    // Now process any options that override the defaults for first device.
    init_config_array(m_config);

    MPA_CONFIG m_outConfig;
    m_outConfig.structLen = sizeof(MPA_CONFIG);    
    err = MPA_SetConfig(&m_drvHandle, &m_config, &m_outConfig);

    if(err != ERR_NO_ERROR)
    {
        TRACE("ERROR: Unable to configure sending device. Error code %d.\n", err);
        SyncDriveExit();
    }
    //You must close and reopen for configuration values to take effect
    err =  MPA_Close(&m_drvHandle);
    if(err != ERR_NO_ERROR)
    {
        TRACE("ERROR: Unable to close device. Error code %d.\n", err);
        SyncDriveExit();
    }

    err = MPA_Open(&m_drvHandle, deviceNumber, overlapped);
    if(err != ERR_NO_ERROR)
    {
        TRACE("ERROR: Unable to open device. Error code %d.\n", err);
        SyncDriveExit();
    }


    ResetFrames(&m_drvHandle, TRUE, TRUE);

    return ERR_NO_ERROR;
}



void CHDLCSendDlg::SyncDriveExit(void)
{
    // This exit routine takes care of required cleanup.
    // It's important not to exit with any locked memory lying around!

    MPA_ERROR_CODE  result;

    if ((result = MPA_Close(&m_drvHandle)) != ERR_NO_ERROR)
    {
        TRACE("ERROR: Unable to close device. Error code %d\n", result);

    }
}

long IntelStorageSwapLong(long LongIn) 
{
    long	LOut, L3, L2, L1, L0 ;

    L3 = LongIn & 0xff000000 ;
    L2 = LongIn & 0x00ff0000 ;
    L1 = LongIn & 0x0000ff00 ;
    L0 = LongIn & 0x000000ff ;
    L3 = L3 / 256 / 256 / 256 ;
    L3 = L3 & 0x000000ff ;			// mask sign
    L2 = L2 / 256 ;
    L1 = L1 * 256 ;
    L0 = L0 * 256 * 256 * 256 ;
    LOut = L3 | L2 | L1 | L0 ;
    return LOut ;
}

short IntelStorageSwapShort(short ShortIn) 
{
    short	SOut, S1, S0 ;

    S1 = ShortIn & 0xff00 ;
    S0 = ShortIn & 0x00ff ;
    S1 = S1 / 256 ;
    S1 = S1 & 0x00ff ;				// mask sign
    S0 = S0 * 256 ;
    SOut = S1 | S0 ;
    return SOut ;
}



int	CHDLCSendDlg::TransmitData(double Roll, double Pitch) 
{
    ULONG bytesWritten; // TxMsgLen
    MPA_ERROR_CODE  result, Result;
    bool			boolA ;
    FrameData frameData;
    unsigned char* pdata = (unsigned char*)&frameData;

    // Init FrameData
     frameData.Address = 3 ;
     frameData.MsgID = 1 ;
//     // Set Status bits, Status = 3
     //frameData.status = 2;
    frameData.Align = 0 ;
    frameData.Navigation = 1 ;
    frameData.AttitudeValid = 1 ;
    frameData.GPS_Available = 0 ;
    frameData.EM_Log_Available = 0 ;
    frameData.Spare = 0 ;					
    frameData.Simulation = 0 ;
    frameData.DRU_Malfunction = 0 ;
    // Set Angles
    frameData.HeadingAngle = 0x04050607 ;
//     	frameData.RollAngle = 0x08090a0b ;
//     	frameData.PitchAngle = 0x0c0d0e0f ;

//     trace.Format("r:%.2f, p:%.2f",Roll, Pitch);
//     sw.TraceTime(TRUE, TRUE, trace);
// 
     frameData.RollAngle = DegToMsgAngle(Roll);
     frameData.PitchAngle = DegToMsgAngle(Pitch);

     //double rang = MsgAngleToDeg(frameData.RollAngle);
     //double pang = MsgAngleToDeg(frameData.PitchAngle);
     //     trace.Format("r2:%df, p2:%df",frameData.RollAngle, frameData.PitchAngle);
//     sw.TraceTime(TRUE, TRUE, trace);

    //	frameData.RollAngle = 0xff00ff00;
    //	frameData.PitchAngle = 0xff00ff00;
    frameData.HeadingAngleRate = 0x1011 ;
    frameData.RollAngleRate = 0x1213 ;
    frameData.PitchAngleRate = 0x1415 ;

    // Swap words from Intel to normal storage format after receiving
    frameData.HeadingAngle = IntelStorageSwapLong(frameData.HeadingAngle) ;
    frameData.RollAngle = IntelStorageSwapLong(frameData.RollAngle) ;
    frameData.PitchAngle = IntelStorageSwapLong(frameData.PitchAngle) ;
    frameData.HeadingAngleRate = IntelStorageSwapShort(frameData.HeadingAngleRate) ;
    frameData.RollAngleRate = IntelStorageSwapShort(frameData.RollAngleRate) ;
    frameData.PitchAngleRate = IntelStorageSwapShort(frameData.PitchAngleRate) ;

    // Transmit data
    	const ULONG TxMsgLen = 21;//sizeof(FrameData) ;
//     TxMsgLen = 21 ;
//     unsigned char pBuf[50];
//     memcpy(pBuf, &frameData, 21);
//     //pBuf = (unsigned char*)&frameData;
//     for(int i=0;i< 21;i++)
//     {
//         trace.Format("0x%x",pBuf[i]);
//         sw.TraceTime(TRUE, TRUE, trace);
//     }

    if(m_dumpData)
         DumpHDLCFrame(&frameData);

    
    UCHAR data[TxMsgLen];
    memset(data, 0, TxMsgLen);
    CopyFrameData(&frameData, &data[0]);
    ResetFrames(&m_drvHandle, TRUE, TRUE);

    result = MPA_PutData(&m_drvHandle, (PUCHAR)data, TxMsgLen, &bytesWritten, NULL);

//     if(m_test == 1)
//     {
//         UCHAR buf[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15};
//         result = MPA_PutData(&m_drvHandle, buf, sizeof(buf), &bytesWritten, NULL);
//     }
//     else if(m_test == 2) 
//     {
//         UCHAR buf[] = {0x03,0x01,0x60,0x07,0x06,0x05,0x04,0xdc,0xfe,0x20,0x03,0x4b,0x7e,0xb1,0x04,0x11,0x10,0x13,0x12,0x15,0x14};
//         if(m_dumpData)
//             DumpHDLCFrame(buf, sizeof(buf));     
//         result = MPA_PutData(&m_drvHandle, buf, sizeof(buf), &bytesWritten, NULL);
//     }
//     else if(m_test == 3) 
//     {
//         //UCHAR* pBuf = new UCHAR[TxMsgLen];
//         UCHAR buf[] = {0x03,0x01,0x60,0x07,0x06,0x05,0x04,0xdc,0xfe,0x20,0x03,0x4b,0x7e,0xb1,0x04,0x11,0x10,0x13,0x12,0x15,0x14};     
//         memcpy(buf, &frameData, TxMsgLen);       
//         if(m_dumpData)
//             DumpHDLCFrame(buf, TxMsgLen);     
//         result = MPA_PutData(&m_drvHandle, buf, TxMsgLen, &bytesWritten, NULL);
//     }
//     else if(m_test == 4) 
//     {
//         result = MPA_PutData(&m_drvHandle, (UCHAR*)&frameData, TxMsgLen, &bytesWritten, NULL);
//     }
//     else
//     {
//         result = MPA_PutData(&m_drvHandle, (PUCHAR)&frameData, TxMsgLen, &bytesWritten, NULL);
//     }
    
    return ERR_NO_ERROR;
}

void CHDLCSendDlg::CopyFrameData(FrameData *frameData, UCHAR* buf)
{
    UCHAR status = 0;
    status |= ((UCHAR)frameData->DRU_Malfunction) << 0; 
    status |= ((UCHAR)frameData->Simulation) << 1; 
    status |= ((UCHAR)frameData->Spare) << 2; 
    status |= ((UCHAR)frameData->EM_Log_Available) << 3; 
    status |= ((UCHAR)frameData->GPS_Available) << 4; 
    status |= ((UCHAR)frameData->AttitudeValid) << 5; 
    status |= ((UCHAR)frameData->Navigation) << 6; 
    status |= ((UCHAR)frameData->Align) << 7; 


    UCHAR* pBuf = buf;
    memcpy((void*)buf, (void*)&frameData->Address, 1);
    buf += 1;
    memcpy((void*)buf, (void*)&frameData->MsgID, 1);
    buf += 1;
    memcpy((void*)buf, (void*)&status, 1);
    buf += 1;
    memcpy((void*)buf, (void*)&frameData->HeadingAngle, 4);
    buf += 4;
    memcpy((void*)buf, (void*)&frameData->RollAngle, 4);
    buf += 4;
    memcpy((void*)buf, (void*)&frameData->PitchAngle, 4);
    buf += 4;
    memcpy((void*)buf, (void*)&frameData->HeadingAngleRate, 2);
    buf += 2;
    memcpy((void*)buf, (void*)&frameData->RollAngleRate, 2);
    buf += 2;
    memcpy((void*)buf, (void*)&frameData->PitchAngleRate, 2);
}

void CHDLCSendDlg::DumpHDLCFrame(FrameData *frameData)
{
    CString dumpFile;   
    CString dir = ".\\";

    static int num=0;
    static int startNum=0;
    dumpFile.Format("%s\\MinsData.txt",dir);

    ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
    dump.seekp( 0, std::ios::end );
    long pos = dump.tellp();

    if(pos == 0)
    {
        //Write header
        dump << "Mins Data" << std::endl;
        dump << sizeof(FrameData);
        startNum=num;
    }

    SYSTEMTIME time;
    GetLocalTime(&time);
    CString str;
    str.Format("(%.2d:%.2d:%.2d,%.3d)",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

    TRACE("Size:%d",sizeof(FrameData));
    dump << num++-startNum << ";";//std::endl;
    dump << str << ";";
    dump << (long)(frameData->DRU_Malfunction!=0) << ";";
    dump << (long)(frameData->Simulation!=0) << ";";
    dump << (long)(frameData->Spare!=0) << ";";
    dump << (long)(frameData->EM_Log_Available!=0) << ";";
    dump << (long)(frameData->GPS_Available!=0) << ";";
    dump << (long)(frameData->AttitudeValid!=0) << ";";
    dump << (long)(frameData->Navigation!=0) << ";";
    dump << (long)(frameData->Align!=0) << ";";    
    dump << std::hex << frameData->HeadingAngle << ";";    				
    dump << std::hex << frameData->RollAngle << ";";    
    dump << std::hex << frameData->PitchAngle << ";";    
    dump << std::hex << frameData->HeadingAngleRate << ";";    			
    dump << std::hex << frameData->RollAngleRate << ";";    			
    dump << std::hex << frameData->PitchAngleRate << ";";    			

    dump << std::endl;

    dump.flush();
    dump.close();
}

void CHDLCSendDlg::DumpHDLCFrame(UCHAR *frameData, int size)
{
    CString dumpFile;   
    CString dir = ".\\";

    static int num=0;
    static int startNum=0;
    dumpFile.Format("%s\\MinsData.txt",dir);

    ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
    dump.seekp( 0, std::ios::end );
    long pos = dump.tellp();

    if(pos == 0)
    {
        //Write header
        dump << "Mins Data" << std::endl;
        dump << sizeof(FrameData);
        startNum=num;
    }

    SYSTEMTIME time;
    GetLocalTime(&time);
    CString str;
    str.Format("(%.2d:%.2d:%.2d,%.3d)",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

    TRACE("Size:%d",sizeof(FrameData));
    dump << num++-startNum << ";(" << size << ")" << ";";//std::endl;
    dump << str << ";";    
   
    CString s;
    for(int i=0;i<size;i++)
    {
        s.Format("%.2x",frameData[i]);
        dump <<  s << " ";
    }

    dump << std::endl;

    dump.flush();
    dump.close();


}


short CHDLCSendDlg::Transmit(unsigned char *frame, int len)
{
    ULONG bytesWritten;
    MPA_ERROR_CODE  result;

    // Transmit data
    TRACE("Write %d bytes...", len);
    result = MPA_PutData(&m_drvHandle, frame, len, &bytesWritten, NULL);
    
    TRACE(" %d written. Result = 0x%x\n", bytesWritten, result);
//     trace.Format(" %d written. Result = 0x%x\n", bytesWritten, result);
//     sw.TraceTime(TRUE, TRUE, trace);

    if (result != ERR_NO_ERROR)
    {
        TRACE("Error starting frame transmission %x", result);
        TRACE("Error > ");
        return(1);
    }

    if (bytesWritten != len) {
        TRACE("Error %d bytes written (%d expected) > ", bytesWritten, len);getchar();
    }
    return 0;

}

void CHDLCSendDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHDLCSendDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHDLCSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHDLCSendDlg::OnTimer(UINT nIDEvent)
{
//     int len = MakeFrame(m_frameBuf, m_seqNum++);
// 
//     ResetFrames(&m_drvHandle, TRUE, TRUE);
// 
//     UpdateData(TRUE);
// 
//     double roll = atof(m_rollStr);    
//     double pitch = atof(m_pitchStr);    
// 
//     TransmitData((double)roll, (double)pitch);  // initialize and transmit buffer  
// 
//     m_numSentFrames++;
// 
//     UpdateData(FALSE);


    if(m_dauDlg.IsWindowVisible())
    {
        m_dauDlg.Send();
    }
    else
    {
         OnBnClickedSendOneButton();
    }
    

    CDialog::OnTimer(nIDEvent);
}

void CHDLCSendDlg::OnDestroy()
{
    CDialog::OnDestroy();
    
    m_HFSender.Destroy();
    SyncDriveExit();
}

void CHDLCSendDlg::OnCbnSelchangeFrameRateComb()
{
    CString rateStr;    
    m_frameRateCombo.GetLBText(m_frameRateCombo.GetCurSel(), rateStr);
    double rate = atof(rateStr);


    m_HFSender.SetFrequence(rate);    
    //     if(m_timer != 0)
//         KillTimer(m_timer);
// 
//     if(rate > 0)
//         m_timer = SetTimer(1, (1.0f/rate)*1000, NULL);

}

void CHDLCSendDlg::OnBnClickedSendOneButton()
{
    //int len = MakeFrame(m_frameBuf, m_seqNum++);
    UpdateData(TRUE);

    ResetFrames(&m_drvHandle, TRUE, TRUE);

    TRACE("FR# %d : ", m_seqNum);

    double roll = atof(m_rollStr);    
    double pitch = atof(m_pitchStr);    

    TransmitData((double)roll, (double)pitch);  // initialize and transmit buffer  

    m_numSentFrames++;

    if(m_cycle)
    {
        double add=0.01f;
        roll += add;
        pitch += add;    
        if(roll > 5.0f)
            roll = 0.0f;
        if(pitch > 5.0f)
            pitch = 0.0f;
        
        m_rollStr.Format("%.4f",roll);
        m_pitchStr.Format("%.4f",pitch);
    }

    UpdateData(FALSE);
}

void CHDLCSendDlg::OnBnClickedDauButton()
{
       m_dauDlg.ShowWindow(m_dauDlg.IsWindowVisible()?SW_HIDE:SW_SHOW);        
}

void CHDLCSendDlg::OnBnClickedDumpCheck()
{
    CButton* pButt = (CButton*)GetDlgItem(IDC_DUMP_CHECK);
    m_dumpData = pButt->GetCheck();
}

void CHDLCSendDlg::OnEnKillfocusTest()
{
    CWnd* pWnd  = GetDlgItem(IDC_TEST);
    CString str;
    pWnd->GetWindowText(str);
    m_test = atoi(str);
}



void CHDLCSendDlg::OnBnClickedCycle()
{
    // TODO: Add your control notification handler code here
}
