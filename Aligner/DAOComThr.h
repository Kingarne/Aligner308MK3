#pragma once
#include "afxmt.h"
#include <deque>
#include "DAUData.h"
#include "FTD2Api.h"

typedef void (*PIOCALLBACK)(PVOID pData, WPARAM wp, LPARAM lp);
class ProgressWnd;


#define COM_ERR_OK						-1
#define COM_ERR_INVALID_START_OFFS		0
#define COM_ERR_INVALID_CRC				1
#define COM_ERR_INVALID_SEQ_NUM			2
#define COM_ERR_DIG_CH1_INVALID_CRC		3
#define COM_ERR_DIG_CH2_INVALID_CRC		4
#define COM_ERR_DIG_CH1_NO_DATA			5
#define COM_ERR_DIG_CH2_NO_DATA			6
#define NUM_COM_ERR						7

struct ErrorInfo
{
	ErrorInfo():count(0),consecutiveCount(0),active(FALSE),alertWhenActive(TRUE){}
	void AddError();
	int count;
	int consecutiveCount;
	BOOL active;	
	BOOL alertWhenActive;
	CString description;
};

class ErrorHandler
{
public:
	ErrorHandler();
	void AddError(int type);
	void ResetError(int type);
	void SetActive(BOOL b);
	int CheckForError();
	CString GetErrorDisc(int type){return m_comErr[type].description;}

	vector<ErrorInfo> m_comErr;
};


struct DACResult
{
    DACResult(short set, short meas, short df, BOOL res){setVal=set;measVal=meas;diff=df;result=res;}
    bool operator<(const DACResult &comp){return setVal < comp.setVal;}
    short setVal;
    short measVal;
    short diff;
    BOOL result;
};

struct uart_struct {
    int baud;
    int n_bits;
    int rx_filter;
    int parity;
    int par_even;
};



struct conf_struct {
    int sample_speed; 
    int  test; 
    int  uartA_en; 
    int  hdlcA_en; 
    int  serA_inv;
    struct uart_struct uartA; 
    int  uartB_en; 
    int  hdlcB_en; 
    int  serB_inv;
    struct uart_struct uartB; 
    int  hdlcA_speed; 
    int  hdlcB_speed; 
    int  dac;
    int  sdc_res;
};

struct DigDAUFrame
{
	DAUFrame frame;
	BOOL received;
    LARGE_INTEGER stamp;
};

#define HEADER_LEN 2
#define NUM_REGS 16
#define TAIL_LEN 5
#define DAU_MSG_LEN (HEADER_LEN+NUM_REGS+TAIL_LEN)

#define PARTIAL_DAC_TEST 0
#define FULL_DAC_TEST 1
#define DAC_TEST_OK   0
#define DAC_TEST_FAIL 1

#define DAU_STATUS_OK               0
#define DAU_STATUS_NO_DEVICE        1
#define DAU_STATUS_NOT_INIT         2
#define DAU_PERFORMING_DAC_TEST     3  

#define CB_TYPE_FRAME    0
#define CB_TYPE_DAC_TEST 1

#define COM_NOT_COMPLETE		0
#define COM_OK						1


class DAOComThread
{
public:
    DAOComThread(void);
    ~DAOComThread(void);

    friend class DebugDlg;

    int Create(HWND hWnd);
    int Destroy();
    void SetCallback(PIOCALLBACK cb, void* context){m_pCallback=cb;m_pContext=context;}
	void SetStatusCallback(PIOCALLBACK cb, void* context){m_pStatusCallback=cb;m_pStatusContext=context;}
	BOOL IsRunning(void) const ; 
    int ReStart();    

    int GetStatus(){return m_status;}    
	void SetProgressWnd(ProgressWnd* pWnd){m_pProgress = pWnd;}
    void SetDAUAcqState(BOOL b);
    void DAUReset();
    void SendTestData(unsigned char data);
    void SendDACData(short);
    void SetSamplingMode(int mode);
    int GetSamplingMode();
    double GetSamplingRate();
    void BeginDACTest(int type = PARTIAL_DAC_TEST);
    void SetDACVoltage( int voltage );
    void SetUARTType(int UARTNum, DigChTypePro UARTType);

    int LoopTest();
    //void SetDigInterface(DigDauChPro interfaceCh, DigChSerialTypePro interfaceType);
    void EnableDigInterface(DigDauChPro interfaceCh, bool enable);
    void SetDigInterfaceSettings(DigDauChPro interfaceCh, DigChSerialTypePro interfaceType, DigChTypePro type);
    void SetResolutions(int a1, int a2, int b1, int b2);
    void GetResolutions(int& a1, int& a2, int& b1, int& b2);

    int m_dauSN;    
    map<int, double> m_sampleRateMap;	
	ErrorHandler m_errorHandler;

protected:
        
    void SendDAUConf(MainBoardRegsPro &m_boardRegs);
    CWinThread *m_pThread;
    static UINT StaticThreadFunc(LPVOID pParam){
        return ((DAOComThread*)pParam)->DAOComThreadFunc();
    }

    int DAOComThreadFunc();
    CEvent m_createEvent;
    CEvent m_destroyEvent;

    HWND m_hWnd;

    PIOCALLBACK m_pCallback;
    void *m_pContext;
	PIOCALLBACK m_pStatusCallback;
	void *m_pStatusContext;
	bool m_bContinue;

    unsigned char m_regs[DAU_MSG_LEN];
    void InitRegs(struct conf_struct *c, unsigned char *regs);
    void InitFPGARegs(MainBoardRegsPro &boardsReg);
    int InitHW();
    int InitUSBHW();
    void ReadUSBData();
    void ReadData();
    int DACTest(DAUFrame& newMsg);
    

    FTD2Api m_ftd2Api;
	CString m_ftdSerial;
    int m_status;
    BOOL m_pauseRead;
    CString m_comPort;
    int m_usbDev;
    MainBoardRegsPro m_boardRegs;    

    //DAC test data
    bool m_DACTestActive;    
    int m_lastSentVal;
    list<unsigned int> m_dacValues;
    vector<DACResult> m_dacResults; 
    void FillDACValues(int type);
    short GetFilteredMeasuredDACVoltage();
    bool AddToReceivedDACVal(short val);
    void DumpDACData();
    int m_DACTestResult;
    int m_DACTestType;
    int m_maxAllowedDACDiff;
    int m_maxDACDiff;
    double m_DACTestStartTime;
    deque<short> m_recentRecDACVal;
    int m_numFilterDac;	
	int m_totalBytesRead;
	int m_lastSeqNum;
	
    ProgressWnd* m_pProgress;

    std::deque<char> m_dbgFifo;
	std::deque<char> m_fifo;
    std::deque<char> m_UARTAFifo;
    std::deque<char> m_UARTBFifo;

    int DeFrame(unsigned char* pData, int size);
    int HandleUARTData(DAUFrame& newMessage, int type);
    int HandleSigmaUARTData(DAUFrame& msg, int type);
	int HandleSigma4050UARTData(DAUFrame& msg, int type);
    int HandleSigmaNMEAUARTData(DAUFrame& msg, int type);
	int HandleMINSNMEAUARTData(DAUFrame& msg, int type);
	int HandleSigmaIXSEAData(DAUFrame& msg, int type);
	int HandleMSIData(DAUFrame& msg, int type);
	int HandlePL40UARTData(DAUFrame& msg, int type);

    BOOL ParseNMEA(vector<char>& frame);

	map<int, DigDAUFrame> m_recentDigFrame;
    map<int, DigChTypePro> m_UARTType;


    DWORD m_rxBytes;        
    LARGE_INTEGER m_fr;

public:

};
