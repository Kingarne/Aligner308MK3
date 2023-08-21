#include "StdAfx.h"
#include "aligner.h"
#include "DAOComThr.h"
#include "Registry.h"
#include <algorithm>
#include <iterator>
#include "ProgressWnd.h"
#include "SystemSetup.h"
#include "Network.h"

 NStopWatch sw;
 CString trace;
int u=0;
int f=0;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define READ_BUF_SIZE  4096
#define READ_BUF_SIZE  128
#define SETUP_BUF_SIZE 1024

ErrorHandler::ErrorHandler()
{
	m_comErr.resize(NUM_COM_ERR);	
	m_comErr[COM_ERR_INVALID_START_OFFS].description = "Invalid Offset";
	m_comErr[COM_ERR_INVALID_CRC].description = "Invalid CRC";
	m_comErr[COM_ERR_INVALID_SEQ_NUM].description = "Invalid Seq. number";
	if(theApp.IsAligner308Enabled())
    {
        m_comErr[COM_ERR_DIG_CH1_INVALID_CRC].description = "Invalid D1 CRC";
        m_comErr[COM_ERR_DIG_CH2_INVALID_CRC].description = "Invalid D2 CRC";
        m_comErr[COM_ERR_DIG_CH1_NO_DATA].description = "No D1 Data";
        m_comErr[COM_ERR_DIG_CH2_NO_DATA].description = "No D2 Data";
    }
    else
    {
        m_comErr[COM_ERR_DIG_CH1_INVALID_CRC].description = "Invalid CH3 CRC";
        m_comErr[COM_ERR_DIG_CH2_INVALID_CRC].description = "Invalid Ch4 CRC";
        m_comErr[COM_ERR_DIG_CH1_NO_DATA].description = "No CH3 Data";
        m_comErr[COM_ERR_DIG_CH2_NO_DATA].description = "No CH4 Data";
    }
   
}
void ErrorInfo::AddError()
{
	count++;
	consecutiveCount++;
	if(consecutiveCount >= 2)
		active=TRUE;
}
void ErrorHandler::AddError(int type)
{
	m_comErr[type].AddError();
}

void ErrorHandler::ResetError(int type)
{
	m_comErr[type].consecutiveCount = 0;
}

void ErrorHandler::SetActive(BOOL b)
{
	for(vector<ErrorInfo>::iterator iter = m_comErr.begin();iter!= m_comErr.end();iter++)
	{
		iter->active = FALSE;	
		//iter->alertWhenActive = TRUE;
	}
}

int ErrorHandler::CheckForError()
{
	int i=0;
	for(vector<ErrorInfo>::iterator iter = m_comErr.begin();iter!= m_comErr.end();iter++,i++)
	{
		if(iter->active && iter->alertWhenActive)
		{
			iter->active = FALSE;						
			return i;
		}
	}
	
	return COM_ERR_OK;
}

DAOComThread::DAOComThread(void)
{
    m_pThread = NULL;
    m_bContinue = TRUE;
    m_hWnd = 0;
    m_pCallback = NULL;
    m_pContext = NULL;
	m_pStatusCallback = NULL;
	m_pStatusContext = NULL;
	m_pauseRead = FALSE;
    m_dauSN = 0;
    m_DACTestActive = false;
    m_status = DAU_STATUS_NOT_INIT;
    m_pProgress = NULL;
    m_rxBytes = 0;
    m_maxAllowedDACDiff = 500;
    m_numFilterDac = 10; 
	m_totalBytesRead = 0;
	m_lastSeqNum = 0;
	
	double baseFreq = 122.0703f;
    m_sampleRateMap[0] = baseFreq;
    m_sampleRateMap[1] = m_sampleRateMap[0]/4.0f;
    m_sampleRateMap[2] = m_sampleRateMap[1]/4.0f;
    m_sampleRateMap[3] = m_sampleRateMap[2]/4.0f;

	DigDAUFrame digFrame;
	digFrame.received = FALSE;
    ::QueryPerformanceCounter(&digFrame.stamp);
    m_recentDigFrame[FRAME_TYPE_HDLC_A] = digFrame;
	m_recentDigFrame[FRAME_TYPE_HDLC_B] = digFrame;
	m_recentDigFrame[FRAME_TYPE_UART_A] = digFrame;
	m_recentDigFrame[FRAME_TYPE_UART_B] = digFrame;

    m_UARTType[FRAME_TYPE_UART_A] = DigChTypeProUnused;
    m_UARTType[FRAME_TYPE_UART_B] = DigChTypeProUnused;  
	m_ftdSerial = "";
}


DAOComThread::~DAOComThread(void)
{

    
}

int DAOComThread::ReStart()
{	
	m_status = DAU_STATUS_NO_DEVICE;
	m_pauseRead = true;
	
// 	trace.Format("Destroy");
// 	sw.TraceTime(TRUE, TRUE, trace);
	Destroy();
	
// 	trace.Format("Create");
// 	sw.TraceTime(TRUE, TRUE, trace);
	Create(m_hWnd);
	
// 	trace.Format("Done Create");
// 	sw.TraceTime(TRUE, TRUE, trace);
    m_pauseRead = false;
    
    if(m_status == DAU_STATUS_OK)
    {

    }

    return m_status;
}


int DAOComThread::Create(HWND hWnd)
{

    m_hWnd = hWnd;

    if(m_pThread != NULL)
        return 1;

    m_bContinue = TRUE;

    if((m_pThread = ::AfxBeginThread(StaticThreadFunc, this)) == NULL)
        return 1;

		
	DWORD nCreateCode = ::WaitForSingleObject((HANDLE)m_createEvent, 2000);
    switch(nCreateCode)
    {
    case WAIT_OBJECT_0:
         TRACE("DAOComThread Created OK\n");        
// 		trace.Format("Thread %p Created",m_pThread->m_hThread);
//		sw.TraceTime(TRUE, TRUE, trace);
		break;
    case WAIT_TIMEOUT:
         TRACE("Error create DAOComThread!\n");        
// 		trace.Format("Error create, Thread %p Created",m_pThread->m_hThread);
//		sw.TraceTime(TRUE, TRUE, trace);
		return 1;      
        break;
    default: break;
    }

    return 0;
}

int DAOComThread::Destroy()
{
    m_bContinue = FALSE;
    
    if(m_pThread)
    {
// 		trace.Format("before WAIT, %p, %p",m_pThread->m_hThread, m_pThread);
// 		sw.TraceTime(TRUE, TRUE, trace);		
		if(::WaitForSingleObject(m_pThread->m_hThread, 1000) == WAIT_TIMEOUT)
        {
            //Beep(1000,1000);
// 			trace.Format("WAIT_TIMEOUT, %p",m_pThread);
// 			sw.TraceTime(TRUE, TRUE, trace);

			TRACE("Destroy Thread\n");
            //::TerminateThread(m_pThread->m_hThread,-1);
            //delete m_pThread;
            m_pThread = NULL;
            return 0;
        }
    }
	
//	Sleep(100);
// 	trace.Format("Thread Destroyed");
// 	sw.TraceTime(TRUE, TRUE, trace);

	TRACE("Quit DAUComThread\n");
	m_pThread = NULL;
    return 0;
}

int DAOComThread::DAOComThreadFunc()
{    
	static int numFound=0;
	TRACE("Thread function %d\n",numFound);

    USHORT	eventType;    

    Registry reg;
    m_usbDev = atoi(reg.GetStringValue("DAUUSBDev", "-1"));
    m_maxAllowedDACDiff = reg.GetIntegerValue("DAU\\DACTest\\MaxDACDiff", 16);
	m_numFilterDac = reg.GetIntegerValue("DAU\\DACTest\\FilterLength", 16);

    if(m_usbDev >= 0)
    {
        //Use direct USB access to DAU
        m_createEvent.SetEvent();
//		Beep(1000, 50);
		if(!InitUSBHW())
        {
//			Beep(500, 50);
			m_status = DAU_STATUS_NO_DEVICE;
						
			if(m_pStatusCallback)
				(*m_pStatusCallback)(m_pStatusContext, m_status, 0);
			
// 			trace.Format("No DAU found, Quit");
// 			sw.TraceTime(TRUE, TRUE, trace);

			return 0;
        }
        m_status = DAU_STATUS_OK;                
		
		//Report that we have found DAU.
		if(m_pStatusCallback)
			(*m_pStatusCallback)(m_pStatusContext, m_status, numFound++);

		while ( m_bContinue ) 
        {
            Sleep(100);           
            ReadUSBData();        
        }
    }
    else
    {
        //Use virtual com access to DAU
        InitHW();
        m_createEvent.SetEvent();
        while ( m_bContinue ) 
        {
            Sleep(100);               
            ReadData();
        }
    }

	m_status = DAU_STATUS_NO_DEVICE;
	theApp.m_DAUPresent = FALSE;
	
	m_fifo.clear();
	m_ftd2Api.Close();
	//m_ftd2Api.FreeDLL();
    
    return 0;
}

void DAOComThread::ReadUSBData()
{

    DWORD bytesRead;
    unsigned char* buf = (unsigned char *)malloc(READ_BUF_SIZE);
    int count=0;
    FT_STATUS status;
	int deframeRes;

    while(m_bContinue)
    {
        if(!m_pauseRead)
        {            
            status = m_ftd2Api.GetQueueStatus(&m_rxBytes);
            status = m_ftd2Api.Read(buf, READ_BUF_SIZE, &bytesRead);
            
//             trace.Format("R:%d",bytesRead);
//             sw.TraceTime(TRUE, TRUE, trace);
            
            //            TRACE("Read time %f\n",sw.GetElapsedTime());
            if (bytesRead > 0) 
            {
                //sw.Stop(); 
                //TRACE("(%d)Deframe: %d\n",count++, bytesRead);
                m_totalBytesRead += bytesRead;				
				if((deframeRes = DeFrame(buf, bytesRead)) < 0)
				{
					
				}
              
                //Sleep(rand()%2000);  
            }
            else 
            { 
//                TRACE("Sleep\n");        
                Sleep(10);        
            }
        }
    };

    free(buf);
}


void DAOComThread::ReadData()
{


    FILE* fp = fopen(m_comPort, "wb");    
    if(fp == NULL)
        return;

    int bytesRead;
    unsigned char* buf = (unsigned char *)malloc(READ_BUF_SIZE);
    int count=0;


    while(m_bContinue)
    {

        bytesRead= fread(&buf[0], 1, READ_BUF_SIZE, fp);

        //TRACE("Read time %f\n",sw.GetElapsedTime());
        if (bytesRead > 0) 
        {
            //   TRACE("(%d)Deframe: %d\n",count++, bytesRead);
            DeFrame(buf, bytesRead);           
            //Sleep(rand()%2000);  
        }
        else 
        { 
            //TRACE("Sleep\n");        
            Sleep(10);        
        }

    };

    free(buf);
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

bool CheckFrameCRC(DAUFrame& frame)
{
    unsigned char crc = 0xff, d8;
    int i, bytesRd = 0;

    d8 = frame.seq;
    d8 <<= 4;
    d8 |= frame.type;
    crc = crc_bits(crc, d8);
    crc = crc_bits(crc, frame.length);

    unsigned char* pData = (unsigned char*)&frame.HdlcMsg;
    for(int i=0;i<frame.length;i++)
    {
        crc = crc_bits(crc, pData[i]);
    }

    return (crc == frame.crc[0]);    
}

void DumpDbgFifo(std::deque<char>& fifo, int error)
{
	if(!(g_AlignerData.debugFlag & DF_DUMP_FIFO))
		return;

	Registry reg;
	CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");    

	CString dumpFile;
	SYSTEMTIME time;
	GetLocalTime(&time);
	dumpFile.Format("%s\\FifoDump%2d_%2d_%2d_%2d_%2d.txt",dir,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
	CString str;
	
	str.Format("Err:%d",error);
	dump << "Length:" << fifo.size()<< ", " << str << std::endl;

	std::deque<char>::iterator iter;;
	for(iter=fifo.begin();iter<fifo.end();iter++)
	{
		str.Format("%x",(unsigned char)*iter);	
		dump << str << " ";
	}
	dump << std::endl;

	dump.flush();
	dump.close();
}

void DumpSensorFrame2(DAUFrame& frame)
{
    CString dumpFile;
    Registry reg;
    CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");    

    static int num=0;
    int startNum=0;
    dumpFile.Format("%s\\MinsData.txt",dir);

    ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
    dump.seekp( 0, std::ios::end );
    long pos = dump.tellp();

    if(pos == 0)
    {
        //Write header
        dump << "Mins data" << std::endl;
        startNum=num;
    }

    dump << num++-startNum << ";";//std::endl;
    dump << "ffwe" << std::endl;
    dump.flush();
    dump.close();
}

void DumpSensorFrame(DAUFrame& frame)
{
	Registry reg;
	CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");    

	CString dumpFile,timeStr,seqStr;
	SYSTEMTIME time;
	GetLocalTime(&time);
	dumpFile.Format("%s\\FrameDump.txt",dir,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,frame.seq);
	timeStr.Format("(%.2d/%.2d %.2d:%.2d:%.2d:%.3d)",time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	seqStr.Format("%.2d",frame.seq);

	ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
	CString str;

	dump << timeStr << "(";
	dump << (int)frame.type << ", ";
	dump << seqStr << ", ";
	dump << (int)frame.length << ") ";
	
	for(int i=0;i<frame.length ;i++)
	{
		str.Format("%.2x ",frame.HdlcMsg[i]);	
		dump << str;
	}
	
	str.Format("(%.2x)",frame.crc);	
	dump << str << std::endl;	

	dump.flush();
	dump.close();
}


void DumpSensorFrame(BYTE* pData, int len)
{
	Registry reg;
	CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");    

	CString dumpFile;

	dumpFile.Format("%s\\FrameDump.txt",dir);

	ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
	CString str;

	for(int i=0;i<len;i++)
	{
		str.Format("%d",pData[i]);	
		dump << str << " ";
	}
	dump << std::endl;

	dump.flush();
	dump.close();
}

int DAOComThread::DeFrame(unsigned char* pData, int size)
{
    unsigned char payloadLen, frameType, seqNum, crc;
    
    m_fifo.insert(m_fifo.end(), pData, pData+size);    
    char startSeq[] = {0x55, 0xaa};
    while(1)
    {
		int error = -1;
        std::deque<char>::iterator it;
        if((it = std::search(m_fifo.begin(), m_fifo.end(), startSeq, startSeq+2)) == m_fifo.end())
        {             
            return COM_NOT_COMPLETE;
        }       
		
		if(distance(it, m_fifo.end()) < 4) // must be at least 4 bytes left 
			return COM_NOT_COMPLETE;

		int diff;
 		if((diff=distance(m_fifo.begin(), it)) != 0)// start sequence must be first
 		{						 		
//			trace.Format("Offset err:%d",diff);
//			sw.TraceTime(TRUE, TRUE, trace);
			error = COM_ERR_INVALID_START_OFFS;
		}

        it += 2;    
        frameType = *it & 0x0f;
        seqNum = (*it)>>4 & 0x0f; 

        payloadLen = *(++it);
         
        int bytesLeftInFifo = distance(++it, m_fifo.end());

        if(bytesLeftInFifo < payloadLen+1) // include crc
            return COM_NOT_COMPLETE;
        
        
		if(error == COM_ERR_INVALID_START_OFFS)
			m_errorHandler.AddError(COM_ERR_INVALID_START_OFFS); 		
		else
			m_errorHandler.ResetError(COM_ERR_INVALID_START_OFFS); 		

		static int c =0 ;       
//        TRACE("%d, bytes: %d\n",c++,payloadLen);         

        std::vector<char> payload;
        payload.reserve(payloadLen);
        copy(it, it+payloadLen, back_inserter(payload));
        crc = *(it+payloadLen);
        
		if(g_AlignerData.debugFlag & DF_DUMP_FIFO)
		{
			copy(m_fifo.begin(), it+payloadLen+1, back_inserter(m_dbgFifo));
			int maxSize = 150;
			if(m_dbgFifo.size()>maxSize)
			{
				m_dbgFifo.erase(m_dbgFifo.begin(), m_dbgFifo.begin()+(m_dbgFifo.size()-maxSize));			
			}		
		}
		
		m_fifo.erase(m_fifo.begin(), it+payloadLen+1);

        DAUFrame newMsg;        
		newMsg.type = frameType;
        newMsg.seq = seqNum;
        newMsg.length = payloadLen;
        newMsg.crc[0] = crc;
        char* pMsgData = (char*)&newMsg.HdlcMsg;
        memcpy(pMsgData, &payload[0], payload.size());
		//TRACE("seq:%d\n",seqNum);
      
		if(g_AlignerData.debugFlag & DF_DUMP_FRAME)
			DumpSensorFrame(newMsg);

		if(m_lastSeqNum+1 != seqNum)
		{
			m_errorHandler.AddError(COM_ERR_INVALID_SEQ_NUM);
			error = COM_ERR_INVALID_SEQ_NUM;
		}
		else
		{
			m_errorHandler.ResetError(COM_ERR_INVALID_SEQ_NUM);
		}
		m_lastSeqNum = (seqNum < 15) ? seqNum : -1;
		
		if(!CheckFrameCRC(newMsg))
		{
			m_errorHandler.AddError(COM_ERR_INVALID_CRC);
			error = COM_ERR_INVALID_CRC;			
		}
		else
		{
			m_errorHandler.ResetError(COM_ERR_INVALID_CRC);
		}

		if(error != COM_ERR_OK)
		{
			DumpDbgFifo(m_dbgFifo, error);		
			continue;
		}

		if(newMsg.type == FRAME_TYPE_UART_A || newMsg.type == FRAME_TYPE_UART_B)
        {
            
            HandleUARTData(newMsg, newMsg.type);         
            continue;
        }

		if(newMsg.type == FRAME_TYPE_HDLC_A || newMsg.type == FRAME_TYPE_HDLC_B)
		{            
			//sw.TraceTime(TRUE, TRUE, "HDLC");
			m_recentDigFrame[newMsg.type].received = TRUE;
			m_recentDigFrame[newMsg.type].frame = newMsg;
			::QueryPerformanceCounter(&m_recentDigFrame[newMsg.type].stamp);
            continue;
		}

//         if(newMsg.type == FRAME_TYPE_HDLC_A || newMsg.type == FRAME_TYPE_HDLC_B)
//         {            
//             TRACE("HDLC: %d, seq:%d\n",newMsg.length,newMsg.seq);
// 
//             return 1;
//         }

	    if(m_DACTestActive)
        {
            int result = DACTest(newMsg);
            if(result == 0)
            {
                //DAC test error, notify                     
                (*m_pCallback)(m_pContext, (WPARAM)1, CB_TYPE_DAC_TEST);
            }
            else if(result == 1)
            {
                (*m_pCallback)(m_pContext, (WPARAM)0, CB_TYPE_DAC_TEST);
            }
        }
        else
        {
            if(m_pCallback != NULL)
            {
                //Send received digital frames first
                LARGE_INTEGER time;                
                QueryPerformanceCounter(&time);
                map<int, DigDAUFrame>::iterator iter;
				for(iter= m_recentDigFrame.begin();iter!=m_recentDigFrame.end();iter++)
				{                                        
                    if(iter->second.received == TRUE)
					{
                        LARGE_INTEGER freq;
                        int res = QueryPerformanceFrequency(&freq);	 
                        double elapsedTime = ((double) (time.QuadPart - iter->second.stamp.QuadPart) * 1000.0f)  /(double) freq.QuadPart;	// ms
                        //TRACE("Elapsed:%f\n",elapsedTime);
                        if(elapsedTime < 1000)
                        {
                            (*m_pCallback)(m_pContext, (WPARAM)&(iter->second.frame), CB_TYPE_FRAME);
                        }
                        else
                        {
                            iter->second.received = FALSE;
                        }
					} 
				}
//                 trace.Format(" FRAME:%d",f++);
//                 sw.TraceTime(TRUE, TRUE, trace);
				//Send frame
				(*m_pCallback)(m_pContext, (WPARAM)&newMsg, CB_TYPE_FRAME);
                //             TRACE("new frame %d\n", newMsg.seq);
            }
        }
    
    
		//Temp...Induce a CRC error
// 		static int r =0;		
// 		if(r++%100 == rand()%100)
// 			m_comErr[COM_ERR_INVALID_CRC].AddError();						
	}

    return COM_OK;
}
		

int DAOComThread::HandleUARTData(DAUFrame& msg, int type)
{
    DigChTypePro protocoll = m_UARTType[type]; 
    switch(protocoll)
    {
        case DigChTypeProSigma40_03:
        case DigChTypeProSigma40_01:
		case DigChTypeProSperryMk39M3:
            return HandleSigmaUARTData(msg, type);
            break;

    case DigChTypeProSigma40_ICD:
      return HandleSigmaICDUARTDData(msg, type);
      break;


		case DigChTypeProSigma40_50:
			return HandleSigma4050UARTData(msg, type);
			break;

        case DigChTypeProSigma40_NMEA:
            return HandleSigmaNMEAUARTData(msg, type);
            break;

		case DigChTypeProMinsNMEA:
			return HandleMINSNMEAUARTData(msg, type);
			break;

        case DigChTypeProIXSEA:
            return HandleSigmaIXSEAData(msg, type);
            break;

		case DigChTypeProMSI:
            return HandleMSIData(msg, type);
            break;

		case DigChTypeProPL40:
			return HandlePL40UARTData(msg, type);
			break;

        default:break;
    }
    return 0;
}


int DAOComThread::HandleMSIData(DAUFrame& msg, int type)
{
    std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

    if(fifo.size()>1000)
        fifo.clear(); // purge if to large

    //  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
    unsigned char payloadLen, frameType, seqNum, crc;    
    unsigned char *pData = msg.HdlcMsg;
    fifo.insert(fifo.end(), pData, pData+msg.length);
    int msgLen = 30;  

	char startSeq[] = {':'};
    while(1)
    {

        BYTE* pFif = (BYTE*)&fifo[0];
        std::deque<char>::iterator startIt;
        if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+1)) == fifo.end())
        {  
            return 0;
        }       
                 
        int bytesLeftInFifo = distance(startIt, fifo.end());        

        if(bytesLeftInFifo < msgLen) 
            return 0;
     
        std::vector<char> payload;
        payload.reserve(msgLen);
        copy(startIt, startIt+msgLen, back_inserter(payload));
//      crc = *(it+payloadLen);
        fifo.erase(fifo.begin(), startIt+msgLen);
        BYTE* pData = (BYTE*)&payload[0];

        DAUFrame newMsg;
        newMsg.type = type;
        newMsg.seq = -1;//seqNum;
        newMsg.length = msgLen;
        newMsg.crc[0] = 0;
        char* pMsgData = (char*)&newMsg.HdlcMsg;        
        memcpy(pMsgData, &payload[0], payload.size());       

        m_recentDigFrame[type].received = TRUE;
        m_recentDigFrame[type].frame = newMsg;       
        ::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);        
    }        
    return 0;
}

int DAOComThread::HandleSigmaIXSEAData(DAUFrame& msg, int type)
{
    std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

    if(fifo.size()>1000) 
        fifo.clear(); // purge if to large

    //  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
    unsigned char payloadLen, frameType, seqNum, crc;    
    unsigned char *pData = msg.HdlcMsg;
    fifo.insert(fifo.end(), pData, pData+msg.length);
    int msgLen = 18;

    char startSeq[] = {0x02};
    while(1)
    {

        BYTE* pFif = (BYTE*)&fifo[0];
        std::deque<char>::iterator startIt;
        if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+1)) == fifo.end())
        {  
            return 0;
        }       

        if(distance(startIt, fifo.end()) < msgLen) // must be at least 18 bytes left 
            return 0;

        int msgOffs = distance(fifo.begin(), startIt);
               
        int bytesLeftInFifo = distance(startIt, fifo.end());        

        if(bytesLeftInFifo < msgLen) 
            return 0;
     
        std::vector<char> payload;
        payload.reserve(msgLen);
        copy(startIt, startIt+msgLen, back_inserter(payload));
//         crc = *(it+payloadLen);
        fifo.erase(fifo.begin(), startIt+msgLen);
        BYTE* pData = (BYTE*)&payload[0];

        DAUFrame newMsg;
        newMsg.type = type;
        newMsg.seq = -1;//seqNum;
        newMsg.length = msgLen;
        newMsg.crc[0] = payload[17];
        char* pMsgData = (char*)&newMsg.HdlcMsg;        
        memcpy(pMsgData, &payload[0], payload.size());       

        m_recentDigFrame[type].received = TRUE;
        m_recentDigFrame[type].frame = newMsg;       
        ::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);        
    }        
    return 0;
}

int DAOComThread::HandleSigmaNMEAUARTData(DAUFrame& msg, int type)
{
    std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

    if(fifo.size()>1000)
        fifo.clear(); // purge if to large

    //  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
    unsigned char payloadLen, frameType, seqNum, crc;    
    unsigned char *pData = msg.HdlcMsg;
    fifo.insert(fifo.end(), pData, pData+msg.length);

    char startSeq[] = "$PFEC";
    char endSeq[] = "\r\n";        
    int ssLen = strlen(startSeq);
    int esLen = strlen(endSeq);
    while(1)
    {

        char* pFif = &fifo[0];
        std::deque<char>::iterator startIt;
        if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+ssLen)) == fifo.end())
        {  
            return 0;
        }       
        //TRACE("Start found\n");
        
        std::deque<char>::iterator endIt;
        if((endIt = std::search(fifo.begin(), fifo.end(), endSeq, endSeq+esLen)) == fifo.end())
        {  
            return 0;
        }               
        
        int n = distance(startIt, endIt);
        if(n<0)
        {
            advance(endIt, 2);
            fifo.erase(fifo.begin(), endIt);
            return 0;
        }
        vector<char> frame;//(n);//(startIt, endIt);
        std::deque<char>::iterator it;
        for(it = startIt;it!=endIt;it++)
        {
            frame.push_back(*it);
            //TRACE("frame:%c\n",*it);       
        }
        
        //ParseNMEA(frame);
        //copy(fifo.begin(), fifo.end(), frame.begin());

//         frame.resize(n);
//         std::copy(startIt, endIt, frame.begin());

        advance(endIt, 2);
        fifo.erase(fifo.begin(), endIt);
        
        
        /*        std::deque<char>::iterator it = startIt;
        int pos = ssLen;
        fifo.erase(fifo.begin(), startIt+pos);
*/
        DAUFrame newMsg;
        newMsg.type = type; 
        newMsg.seq = -1;//seqNum;
        newMsg.length = frame.size();
        newMsg.crc[0] = 57;//crc;
        char* pMsgData = (char*)&newMsg.HdlcMsg;
        memcpy(pMsgData, &frame[0], frame.size());

        m_recentDigFrame[type].received = TRUE;
        m_recentDigFrame[type].frame = newMsg;	
        ::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);
    }
    return 1;
}

int DAOComThread::HandleMINSNMEAUARTData(DAUFrame& msg, int type)
{
	std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

	if (fifo.size() > 1000)
	{
		fifo.clear(); // purge if to large
	}
	//  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
	unsigned char payloadLen, frameType, seqNum, crc;
	unsigned char *pData = msg.HdlcMsg;
	fifo.insert(fifo.end(), pData, pData + msg.length);

	if (fifo.size() == 0)
		return 0;

	char startSeq[] = "$PANZHRP";
	char endSeq[] = "\r\n";
	int ssLen = strlen(startSeq);
	int esLen = strlen(endSeq);
	while (1)
	{

		std::deque<char>::iterator startIt;
		if ((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq + ssLen)) == fifo.end())
		{
			return 0;
		}
		//TRACE("Start found\n");

		std::deque<char>::iterator endIt;
		if ((endIt = std::search(fifo.begin(), fifo.end(), endSeq, endSeq + esLen)) == fifo.end())
		{
			return 0;
		}

		int n = distance(startIt, endIt);
		if (n < 0)
		{
			advance(endIt, 2);
			fifo.erase(fifo.begin(), endIt);
			return 0;
		}
		vector<char> frame;//(n);//(startIt, endIt);
		std::deque<char>::iterator it;
		for (it = startIt; it != endIt; it++)
		{
			frame.push_back(*it);
			//TRACE("frame:%c\n",*it);       
		}

		//ParseNMEA(frame);
		//copy(fifo.begin(), fifo.end(), frame.begin());

//         frame.resize(n);
//         std::copy(startIt, endIt, frame.begin());

		advance(endIt, 2);
		fifo.erase(fifo.begin(), endIt);


		/*        std::deque<char>::iterator it = startIt;
		int pos = ssLen;
		fifo.erase(fifo.begin(), startIt+pos);
*/
		DAUFrame newMsg;
		newMsg.type = type;
		newMsg.seq = -1;//seqNum;
		newMsg.length = frame.size();
		newMsg.crc[0] = 57;//crc;
		char* pMsgData = (char*)&newMsg.HdlcMsg;
		memcpy(pMsgData, &frame[0], frame.size());

		m_recentDigFrame[type].received = TRUE;
		m_recentDigFrame[type].frame = newMsg;
		::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);
	}
	return 1;
}


BOOL DAOComThread::ParseNMEA(vector<char>& frame)
{
    std::vector<char>::iterator pos = find(frame.begin(), frame.end(), '*');
    if(pos == frame.end())
        return FALSE;

    CString checkSum;
    checkSum += *(pos+1);
    checkSum += *(pos+2);

    std::vector<char>::iterator p1 = pos+1;
    std::vector<char>::iterator p2 = pos+2;
    frame.erase(pos, pos+3);
    
    vector<CString> field;
    CString str;
    std::vector<char>::iterator start = frame.begin();
    pos = find(frame.begin(), frame.end(), ',');
    while(pos != frame.end())
    {    
        str = "";
        std::vector<char>::iterator it;
        for(it = start;it!=pos;it++)
        {
            str += *it;
        }
        
        start = pos+1;
        pos = find(start, frame.end(), ',');               
        field.push_back(str);
    }

    str = "";
    std::vector<char>::iterator it;
    for(it = start;it!=frame.end();it++)
    {
        str += *it;
    }
    field.push_back(str);

//     vector<CString>::iterator fIt;
//     for(fIt = field.begin();fIt!=field.end();fIt++)
//     {
//         TRACE("field:%s\n",*fIt);
//     }
    
    if(field.size() != 5)
        return FALSE;

    return TRUE;
}

int DAOComThread::HandlePL40UARTData(DAUFrame& msg, int type)
{
	std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

	if(fifo.size()>1000)
		fifo.clear(); // purge if to large

	//  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
	unsigned char payloadLen, frameType, seqNum, crc;    
	unsigned char *pData = msg.HdlcMsg;
	fifo.insert(fifo.end(), pData, pData+msg.length);
	int msgLen = 22;  

	char startSeq[] = {0xAA, 0x55};
	while(1)
	{

		BYTE* pFif = (BYTE*)&fifo[0];
		std::deque<char>::iterator startIt;
		if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+1)) == fifo.end())
		{  
			return 0;
		}       

		int bytesLeftInFifo = distance(startIt, fifo.end());        

		if(bytesLeftInFifo < msgLen) 
			return 0;

		std::vector<char> payload;
		payload.reserve(msgLen);
		copy(startIt, startIt+msgLen, back_inserter(payload));
		//      crc = *(it+payloadLen);
		fifo.erase(fifo.begin(), startIt+msgLen);
		BYTE* pData = (BYTE*)&payload[0];

		DAUFrame newMsg;
		newMsg.type = type;
		newMsg.seq = -1;//seqNum;
		newMsg.length = msgLen;
		newMsg.crc[0] = 0;
		char* pMsgData = (char*)&newMsg.HdlcMsg;        
		memcpy(pMsgData, &payload[0], payload.size());       

		m_recentDigFrame[type].received = TRUE;
		m_recentDigFrame[type].frame = newMsg;       
		::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);        
	}        
	return 0;
}

int DAOComThread::HandleSigma4050UARTData(DAUFrame& msg, int type)
{
	std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

	if(fifo.size()>1000)
		fifo.clear(); // purge if to large

	//  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
	unsigned char payloadLen, frameType, seqNum, crc;    
	unsigned char *pData = msg.HdlcMsg;
	fifo.insert(fifo.end(), pData, pData+msg.length);

	char startSeq[] = {0x5A, 0xA5};
	while(1)
	{

		char* pFif = &fifo[0];
		std::deque<char>::iterator startIt;
		if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+2)) == fifo.end())
		{  
			return 0;
		}       

		if(distance(startIt, fifo.end()) < 10) // must be at least 10 bytes left 
			return 0;

		std::deque<char>::iterator it = startIt;
		it += 4;            
		payloadLen = *(it);

		int bytesLeftInFifo = distance(it, fifo.end());

		int msgLen = payloadLen+6;

		if(bytesLeftInFifo < msgLen) 
			return 0;

		std::vector<char> payload;
		payload.reserve(msgLen);
		copy(startIt, startIt+msgLen, back_inserter(payload));
		crc = *(it+payloadLen);
		fifo.erase(fifo.begin(), startIt+msgLen);

		DAUFrame newMsg;
		newMsg.type = type;
		newMsg.seq = -1;//seqNum;
		newMsg.length = msgLen;
		newMsg.crc[0] = crc;
		char* pMsgData = (char*)&newMsg.HdlcMsg;
		char* pP = &payload[0];
		memcpy(pMsgData, &payload[0], payload.size());

		m_recentDigFrame[type].received = TRUE;
		m_recentDigFrame[type].frame = newMsg;	
		::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);
		//         trace.Format("uart:%d",u++);
		//         sw.TraceTime(TRUE, TRUE, trace);
	}
	return 1;
}


int DAOComThread::HandleSigmaICDUARTDData(DAUFrame& msg, int type)
{
  std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;

  if (fifo.size() > 1000)
    fifo.clear(); // purge if to large

//  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
  unsigned char payloadLen, frameType, seqNum, crc;
  unsigned char* pData = msg.HdlcMsg;
  fifo.insert(fifo.end(), pData, pData + msg.length);

  char startSeq[] = { 0x01, 0xAA };
  while (1)
  {

    char* pFif = &fifo[0];
    std::deque<char>::iterator startIt;
    if ((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq + 2)) == fifo.end())
    {
      return 0;
    }

    if (distance(startIt, fifo.end()) < 10) // must be at least 10 bytes left 
      return 0;

    std::deque<char>::iterator it = startIt;
    it += 2;
    payloadLen = 16;

    int bytesLeftInFifo = distance(it, fifo.end());

    int msgLen = payloadLen + 2;

    if (bytesLeftInFifo < payloadLen)
      return 0;

    std::vector<char> payload;
    payload.reserve(msgLen);
    copy(startIt, startIt + msgLen, back_inserter(payload));
   
    fifo.erase(fifo.begin(), startIt + msgLen);

    DAUFrame newMsg;
    newMsg.type = type;
    newMsg.seq = -1;//seqNum;
    newMsg.length = msgLen;
    newMsg.crc[0] = payload[16];
    newMsg.crc[1] = payload[17];
    char* pMsgData = (char*)&newMsg.HdlcMsg;    
    memcpy(pMsgData, &payload[0], payload.size());

    m_recentDigFrame[type].received = TRUE;
    m_recentDigFrame[type].frame = newMsg;
    ::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);
    //         trace.Format("uart:%d",u++);
//         sw.TraceTime(TRUE, TRUE, trace);
  }
  return 1;
}

int DAOComThread::HandleSigmaUARTData(DAUFrame& msg, int type)
{
    std::deque<char>& fifo = (type == FRAME_TYPE_UART_A) ? m_UARTAFifo : m_UARTBFifo;
    
    if(fifo.size()>1000)
        fifo.clear(); // purge if to large
    
    //  TRACE("Len:%d, seq:%d\n", msg.length, msg.seq);
    unsigned char payloadLen, frameType, seqNum, crc;    
    unsigned char *pData = msg.HdlcMsg;
    fifo.insert(fifo.end(), pData, pData+msg.length);

    char startSeq[] = {0x5A, 0xA5};
    while(1)
    {

		char* pFif = &fifo[0];
		std::deque<char>::iterator startIt;
        if((startIt = std::search(fifo.begin(), fifo.end(), startSeq, startSeq+2)) == fifo.end())
        {  
            return 0;
        }       

        if(distance(startIt, fifo.end()) < 10) // must be at least 10 bytes left 
            return 0;

        std::deque<char>::iterator it = startIt;
        it += 2;            
        payloadLen = *(it);

        int bytesLeftInFifo = distance(++it, fifo.end());

		int msgLen = payloadLen+6;
		
		if(bytesLeftInFifo < msgLen) 
            return 0;
        
        std::vector<char> payload;
        payload.reserve(msgLen);
        copy(startIt, startIt+msgLen, back_inserter(payload));
        crc = *(it+payloadLen);
        fifo.erase(fifo.begin(), startIt+msgLen);

        DAUFrame newMsg;
        newMsg.type = type;
        newMsg.seq = -1;//seqNum;
        newMsg.length = msgLen;
        newMsg.crc[0] = crc;
        char* pMsgData = (char*)&newMsg.HdlcMsg;
        char* pP = &payload[0];
		memcpy(pMsgData, &payload[0], payload.size());
    
		m_recentDigFrame[type].received = TRUE;
		m_recentDigFrame[type].frame = newMsg;	
        ::QueryPerformanceCounter(&m_recentDigFrame[type].stamp);
        //         trace.Format("uart:%d",u++);
//         sw.TraceTime(TRUE, TRUE, trace);
    }
    return 1;
}

void DAOComThread::FillDACValues(int type)
{
    m_dacValues.clear();

    switch(type)
    {
        case PARTIAL_DAC_TEST:
            for(unsigned int i=0x01;i<=0x8000;i=i<<1)
            {
                m_dacValues.push_back(i);
            }
        break;

        case FULL_DAC_TEST:
        {
            //unsigned int highVal=0x7fff, lowVal=0x0;  
            unsigned int highVal=0x1;  
            while(highVal <= 0xffff)
           // while(lowVal < 0x00ff)
            {
                m_dacValues.push_back(highVal);
                //m_dacValues.push_back(lowVal);                
                highVal++;                                
            };

        }
    }
   
//     for(int i=0;i<1000;i++)
//     {
//         m_dacValues.push_back(rand()%65000);
//     }
}


void DAOComThread::SetDACVoltage( int voltage )
{
    m_boardRegs.m_testVoltage = voltage;
    SendDAUConf(m_boardRegs);
}

void DAOComThread::BeginDACTest(int type)
{
    FillDACValues(type);
        
    if(m_pProgress)
        m_pProgress->Init("Performing DAC Test", m_dacValues.size());
    
    SetSamplingMode(FQ_MODE_1220);

    m_DACTestType = type;
    m_lastSentVal = 0;       
    m_boardRegs.m_testVoltage = m_lastSentVal;
    SendDAUConf(m_boardRegs);
    Sleep(100);
    m_DACTestActive = true;   
    m_status = DAU_PERFORMING_DAC_TEST;
    
    m_dacResults.clear();
    m_dacResults.reserve(m_dacValues.size());
    m_recentRecDACVal.clear();

    m_DACTestResult = DAC_TEST_OK;
    m_maxDACDiff = 0;

    m_DACTestStartTime = sw.GetCurrentTime();
}

short DAOComThread::GetFilteredMeasuredDACVoltage()
{
    if(m_recentRecDACVal.size()==0)
        return 0;

	if(m_recentRecDACVal.size() > 1)
		m_recentRecDACVal.pop_front();

    int total =0;
    deque<short>::iterator iter; 
    for(iter = m_recentRecDACVal.begin();iter!=m_recentRecDACVal.end();iter++)
    {  
        total += *iter;
    }        
    return total/(int)m_recentRecDACVal.size();
}


bool DAOComThread::AddToReceivedDACVal(short val)
{
    m_recentRecDACVal.push_back(val);
    return (m_recentRecDACVal.size() == m_numFilterDac);    
}

void DAOComThread::DumpDACData()
{
    Registry reg;
    CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");    

    CString dumpFile;
    dumpFile.Format("%s\\%sDACDump.txt",dir, (m_DACTestType == FULL_DAC_TEST) ? "Full" : "Diagnostic");

    //sort(m_dacResults.begin(), m_dacResults.end());
    ofstream dump(dumpFile, std::ios_base::out);
    
    vector<DACResult>::iterator iter;
    for(iter=m_dacResults.begin();iter!=m_dacResults.end();iter++)
    {
        dump << iter->setVal << "\t" << iter->measVal << "\t" << iter->diff << "\t" << iter->result << std::endl; 
    }
    dump.flush();
    dump.close();
}

int DAOComThread::DACTest(DAUFrame& newMsg)
{
    if(newMsg.type != FRAME_TYPE_SENSOR)
        return 2;

    if(m_pProgress && m_pProgress->IsCanceled())
    {
        m_dacValues.clear();  //Means we are done.      
    }
    
    if(m_dacValues.size() == 0)
    {
        if(m_pProgress)
            m_pProgress->Done();
        
        if(m_DACTestResult == DAC_TEST_FAIL || m_DACTestType == FULL_DAC_TEST)
            DumpDACData();

        m_DACTestActive = false;        
        m_status = DAU_STATUS_OK;
        SetSamplingMode(FQ_MODE_305);
        SetDACVoltage(32768);
        
        return (m_DACTestResult == DAC_TEST_FAIL) ? 0 : 1;
    }
    
    if(m_lastSentVal != newMsg.sdcAdcMsgData.m_testSetVoltage)
        return 1;

    if(!AddToReceivedDACVal(newMsg.sdcAdcMsgData.m_testVoltage))
        return 1;

    short filterdDACVal = GetFilteredMeasuredDACVoltage();
    int setVolt = newMsg.sdcAdcMsgData.m_testSetVoltage;
    int diff = filterdDACVal+32768-setVolt;
   // TRACE("(%d) %u, diff:%d\n",m_dacValues.size(), newMsg.sdcAdcMsgData.m_testSetVoltage, diff);
    BOOL DACError = (abs(diff) > m_maxAllowedDACDiff);
    m_recentRecDACVal.clear();
    m_dacResults.push_back(DACResult(setVolt, filterdDACVal-32768, diff, DACError));    
    
    if(DACError)
    {        
        m_DACTestResult = DAC_TEST_FAIL;
    }
   
    if(abs(diff) > abs(m_maxDACDiff))
        m_maxDACDiff = diff;

    m_lastSentVal = m_dacValues.front();    
    m_boardRegs.m_testVoltage = m_lastSentVal;
    m_dacValues.pop_front();
  
    //Send next DAC value
    SendDAUConf(m_boardRegs);

    if(m_pProgress)
    {
        double timeDiff = sw.GetCurrentTime() - m_DACTestStartTime;
        //TRACE("Diff:%.2f\n",diff);
        m_pProgress->SetElapsedTime(timeDiff);
        m_pProgress->Step();
        CString str;
        str.Format("Max diff:%d",m_maxDACDiff);
        m_pProgress->SetInfoText(str);
        if(DACError)
            m_pProgress->SetWindowText("Performing DAC Test (FAILED)");
    }

    return 1;
}



void DAOComThread::InitRegs(struct conf_struct *c, unsigned char *regs)
{
    int tmp;

    regs[0] = 0x01;  // Control
    regs[0] |= c->sample_speed << 4;

    regs[1] = c->test;  // Testbyte

    // SCTRL_A
    regs[2] = c->hdlcA_en;
    if (c->uartA_en || c->hdlcA_en)
        regs[2] |= 0x02;
    regs[2] |= c->serA_inv << 2;
    regs[2] |= c->hdlcA_speed << 4;

    // UARTA_CTRL
    switch (c->uartA.n_bits) 
    {
    case 5: regs[3] = 0;break;
    case 6: regs[3] = 1;break;
    case 7: regs[3] = 2;break;
    case 8: regs[3] = 3;break;
    default: regs[3] = 0;
    }

    regs[3] |= c->uartA.rx_filter << 4;
    regs[3] |= c->uartA.parity << 3;
    regs[3] |= c->uartA.par_even << 2;

    tmp = 19660800;
    tmp /= c->uartA.baud;
    //printf("UART A Div-factor = %d (0x%x)\n", tmp, tmp);
    tmp -= 1;
    regs[4] = tmp >> 8;   // UARTA_DIV 13..8
    regs[5] = tmp&0xff;   // UARTA_DIV 7..0


    // SCTRL_B
    regs[6] = c->hdlcB_en;
    if (c->uartB_en || c->hdlcB_en)
        regs[6] |= 0x02;
    regs[6] |= c->serB_inv << 2;
    regs[6] |= c->hdlcB_speed << 4;

    // UARTB_CTRL
    switch (c->uartB.n_bits) 
    {
    case 5: regs[7] = 0;break;
    case 6: regs[7] = 1;break;
    case 7: regs[7] = 2;break;
    case 8: regs[7] = 3;break;
    default: regs[7] = 0;
    }
    regs[7] |= c->uartB.rx_filter << 4;
    regs[7] |= c->uartB.parity << 3;
    regs[7] |= c->uartB.par_even << 2;

    tmp = 19660800;
    tmp /= c->uartB.baud;
    //printf("UART B Div-factor = %d (0x%x)\n", tmp, tmp);
    tmp -= 1;
    regs[8] = tmp >> 8;   // UARTB_DIV 13..8
    regs[9] = tmp&0xff;   // UARTB_DIV 7..0

    tmp = c->dac;
    tmp += 0x8000;
    tmp &= 0xffff;
    regs[11] = tmp & 0xff;   // DAC 7..0
    regs[10] = tmp >> 8;     // DAC 15..8
    regs[12] = c->sdc_res;   // SDC
    regs[12] |= c->sdc_res << 2;   // SDC
    regs[12] |= c->sdc_res << 4;   // SDC
    regs[12] |= c->sdc_res << 6;   // SDC
}


void DAOComThread::InitFPGARegs(MainBoardRegsPro &m_boardRegs)
{
    Registry reg;
    CString base = "DAU\\";
    memset(&m_boardRegs, 0x0, sizeof(MainBoardRegsPro));

    m_boardRegs.m_dataAcqEnable = 1;

    //DAU CTRL    
    m_boardRegs.m_SamplingSpeed = reg.GetIntegerValue(base + "SampleSpeed", 0);
    m_boardRegs.m_dauReset = 0;
    m_boardRegs.m_usbTxOn = 0;

    //Loop test data
    m_boardRegs.m_testByte = reg.GetIntegerValue(base + "Test", 0x38);

    //SCTRL_A
    m_boardRegs.m_serialCtrlA_HdlcUartSel = reg.GetIntegerValue(base + "UartOrHDLC_A", 0);
    m_boardRegs.m_serialCtrlA_Enable = reg.GetIntegerValue(base + "Enable_A", 0);
    m_boardRegs.m_serialCtrlA_HdlcBitrate = reg.GetIntegerValue(base + "hdlcASpeed", 0);

    //UART_A_ctrl
    int nBits = reg.GetIntegerValue(base + "UARTA\\Bits", 8);
    if(nBits < 5 || nBits > 8)
        nBits = 5;
    m_boardRegs.m_uartCtrlA_m_noOfDataBits = nBits - 5;

    m_boardRegs.m_uartCtrlA_m_parityEven = reg.GetIntegerValue(base + "UARTA\\ParEven", 0);
    m_boardRegs.m_uartCtrlA_m_parityOn = reg.GetIntegerValue(base + "UARTA\\Parity", 1);
    m_boardRegs.m_uartCtrlA_m_rxTransientFilter = reg.GetIntegerValue(base + "UARTA\\rxFilter", 1);
    m_boardRegs.m_uartCtrlA_m_fifoThreshold = reg.GetIntegerValue(base + "UARTA\\fifoThreshold", 2); //	00 = 1, 01 = 8, 10 = 16, 11 = 64 bytes ; DAU send threshold

    //UART_A_BAUD
    int baud = reg.GetIntegerValue(base + "UARTA\\Baud", 115200);
    int tmp = 19660800;
    tmp /= baud ;
    //printf("UART A Div-factor = %d (0x%x)\n", tmp, tmp);
    tmp -= 1;
    m_boardRegs.m_uartCtrlA_BaudRate = tmp;
    //swap(m_boardRegs.m_uartBaudRateA);

    //SCTRL_B
    m_boardRegs.m_serialCtrlB_HdlcUartSel = reg.GetIntegerValue(base + "UartOrHDLC_B", 0);
    m_boardRegs.m_serialCtrlB_Enable = reg.GetIntegerValue(base + "Enable_B", 0);
    m_boardRegs.m_serialCtrlB_HdlcBitrate = reg.GetIntegerValue(base + "hdlcBSpeed", 0);

    //UART_B_ctrl
    nBits = reg.GetIntegerValue(base + "UARTB\\Bits", 8);
    if(nBits < 5 || nBits > 8)
        nBits = 5;
    m_boardRegs.m_uartCtrlB_m_noOfDataBits = nBits - 5;

    m_boardRegs.m_uartCtrlB_m_parityEven = reg.GetIntegerValue(base + "UARTB\\ParEven", 0);
    m_boardRegs.m_uartCtrlB_m_parityOn = reg.GetIntegerValue(base + "UARTB\\Parity", 1);
    m_boardRegs.m_uartCtrlB_m_rxTransientFilter = reg.GetIntegerValue(base + "UARTB\\rxFilter", 1);
    m_boardRegs.m_uartCtrlB_m_fifoThreshold = reg.GetIntegerValue(base + "UARTB\\fifoThreshold", 2); //	00 = 1, 01 = 8, 10 = 16, 11 = 64 bytes ; DAU send threshold

    //UART_B_BAUD
    baud = reg.GetIntegerValue(base + "UARTB\\Baud", 115200);
    tmp = 19660800;
    tmp /= baud ;
    //printf("UART A Div-factor = %d (0x%x)\n", tmp, tmp);
    tmp -= 1;
    m_boardRegs.m_uartCtrlB_BaudRate = tmp;
   // swap(m_boardRegs.m_uartBaudRateB);

    // DAC test
    m_boardRegs.m_testVoltage = reg.GetIntegerValue(base + "Dac", 0x4000);

    //SDC
    int sdcRes = reg.GetIntegerValue(base + "sdcRes", 3);
    m_boardRegs.m_syncroResolutionD2J2Pitch = sdcRes ;				//	00 = 10, 01 = 12, 10 = 14, 11 = 16 bits
    m_boardRegs.m_syncroResolutionD2J2Roll = sdcRes ;
    m_boardRegs.m_syncroResolutionD1J1Pitch = sdcRes ;
    m_boardRegs.m_syncroResolutionD1J1Roll = sdcRes ;
}

void DAOComThread::SendDAUConf(MainBoardRegsPro &m_boardRegs)
{
    FT_STATUS status;
    DWORD numWrite;
    unsigned char buf[DAU_MSG_LEN+1];
    memset(buf, 0x55, HEADER_LEN);
    buf[HEADER_LEN-1] = 0xaa;

    memcpy(buf+HEADER_LEN, &m_boardRegs, sizeof(m_boardRegs));//NUM_REGS);
    memset(buf+HEADER_LEN+NUM_REGS, 0x55, TAIL_LEN);    
    status = m_ftd2Api.Write(&buf[0], DAU_MSG_LEN, &numWrite);  
}

int DAOComThread::InitUSBHW()
{
    Registry reg;

	if(!m_ftd2Api.LoadDLL())
        return FALSE;

    int numDevs, devIndex = -1;
    FT_STATUS status;
    status = m_ftd2Api.ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
        
    char *BufPtrs[64]; // pointer to array of 64 pointers    
    for(DWORD d=0; d<numDevs; d++)
      BufPtrs[d] = new char[64];
    BufPtrs[d] = NULL;

    
    CString str;    
    status = m_ftd2Api.ListDevices(BufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION);
    if (FT_SUCCESS(status)) 
    {
        for(DWORD u=0; u<numDevs; u++)
        {            
            str.Format("%s", BufPtrs[u]);
            if(str.Find("FT245R") != -1)
            {
                devIndex = u;
                break;
            }
        }
    }
    
 
    if(devIndex < 0)
	{
		for(d=0; d<numDevs; d++)
		{
			delete BufPtrs[d];  
		}
		return FALSE; //No device
	}
	status = m_ftd2Api.ListDevices(BufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_SERIAL_NUMBER);
	if (FT_SUCCESS(status) && devIndex<numDevs) 
	{
		m_ftdSerial = BufPtrs[devIndex];	
	}

	for(d=0; d<numDevs; d++)
	{
		delete BufPtrs[d];  
	}

	/*if (!Features::CheckString(m_ftdSerial))
	{
		::AfxMessageBox( _T("Invalid application Key!") ) ;
		return FALSE ;
	}*/


    status = m_ftd2Api.Open((PVOID)devIndex);//load default device 0
    if(status != FT_OK)
    {
        ::AfxMessageBox("Could not open DAU (USB) device.");
        return FALSE;
    }

    
    
    m_ftd2Api.SetBaudRate(115200);
    m_ftd2Api.SetTimeouts(100, 100);
    UCHAR latency;
    m_ftd2Api.GetLatencyTimer(&latency);
	latency = reg.GetIntegerValue("DAU\\ReceiveLatencyTimer", 16);
	m_ftd2Api.SetLatencyTimer(latency);

     
    InitFPGARegs(m_boardRegs);

    //Stop DAU
    m_boardRegs.m_dataAcqEnable = 0;
    m_boardRegs.m_dauReset = 0;    
    SendDAUConf(m_boardRegs);

    Sleep(100);
    TRACE("Reading old data ... ");
 
    DWORD RxBytes;
    status = m_ftd2Api.GetQueueStatus(&RxBytes);
    TRACE("Clear %d bytes in Rx\n", RxBytes);    
    if(RxBytes > 0)
        status = m_ftd2Api.Purge(FT_PURGE_RX);  
    

    //Reset DAU
    m_boardRegs.m_dauReset = 1;
    SendDAUConf(m_boardRegs);
  
    DWORD tries = 0, numRead=0;   
    do
    {
        Sleep(10);    
        status = m_ftd2Api.GetQueueStatus(&RxBytes);            
    }
    while(RxBytes < 21 && tries++ < 100);
    
    TRACE("Read reset-frame ... \n");   
    //unsigned char buf[21];
    m_ftd2Api.Read(m_regs, 21, &numRead);
    m_dauSN = m_regs[5];
    //     do
//     {
//         Read(&m_regs[0], 1, &numRead);
//         TRACE("%02x ", m_regs[0]);
//     }while(numRead > 0);
//     
    //Start DAU
     m_boardRegs.m_dataAcqEnable = 1;
     m_boardRegs.m_dauReset = 0;    
     SendDAUConf(m_boardRegs);  
     
//      unsigned char buf[DAU_MSG_LEN+1];
//      memset(buf, 0x0, DAU_MSG_LEN);
//      memset(buf, 0xaa, HEADER_LEN);
//      buf[HEADER_LEN-1] = 0x55;
// 
//      memcpy(buf+HEADER_LEN, &m_boardRegs, sizeof(m_boardRegs));//NUM_REGS);
     //status = Write(&buf[0], DAU_MSG_LEN, &numWrite); 
   

    return TRUE;
}

double DAOComThread::GetSamplingRate() 
{
    return m_sampleRateMap[m_boardRegs.m_SamplingSpeed];
}

int DAOComThread::GetSamplingMode()
{
    return m_boardRegs.m_SamplingSpeed;        
}


void DAOComThread::SetSamplingMode(int mode) 
{
    m_boardRegs.m_SamplingSpeed = mode;            
    SendDAUConf(m_boardRegs);
}

void DAOComThread::SendDACData(short data)
{
//     WORD d =data;
//     swap(d);
    m_boardRegs.m_testVoltage = data;    
    SendDAUConf(m_boardRegs);
}

void DAOComThread::SendTestData(unsigned char data)
{
    m_boardRegs.m_testByte = data;    
    SendDAUConf(m_boardRegs);
}

void DAOComThread::SetDAUAcqState(BOOL b)
{
    m_boardRegs.m_dataAcqEnable = b;    
    SendDAUConf(m_boardRegs);
}

void DAOComThread::DAUReset()
{
    m_boardRegs.m_dauReset = 1;    
    SendDAUConf(m_boardRegs);
    m_boardRegs.m_dauReset = 0;    
    SendDAUConf(m_boardRegs);
}

int DAOComThread::LoopTest()
{
    if(m_pProgress)
        m_pProgress->Init("Performing Loop Test", 256);
    
    BOOL testOK = TRUE;
    DWORD RxBytes;
    FT_STATUS status;

    m_pauseRead = TRUE;
    int oldAcqState = m_boardRegs.m_dataAcqEnable;
    int oldCtrlAState = m_boardRegs.m_serialCtrlA_Enable;
    int oldCtrlBState = m_boardRegs.m_serialCtrlB_Enable;

    m_boardRegs.m_dauReset = 1;    
    m_boardRegs.m_dataAcqEnable = 0;    
    m_boardRegs.m_serialCtrlA_Enable = 0;
    m_boardRegs.m_serialCtrlB_Enable = 0;
    SendDAUConf(m_boardRegs);

    Sleep(10);
    
    status = m_ftd2Api.GetQueueStatus(&RxBytes);
    if(RxBytes > 0)
        status = m_ftd2Api.Purge(FT_PURGE_RX);  

    status = m_ftd2Api.GetQueueStatus(&RxBytes);

    m_boardRegs.m_dauReset = 0;    
    SendDAUConf(m_boardRegs);    

    Sleep(10);


    status = m_ftd2Api.GetQueueStatus(&RxBytes);
    if(RxBytes > 0)
        status = m_ftd2Api.Purge(FT_PURGE_RX);  

     DWORD tries = 0, numRead=0;   
    do
    {
        Sleep(1);    
        status = m_ftd2Api.GetQueueStatus(&RxBytes);            
    }
    while(RxBytes < 21 && tries++ < 100);

    TRACE("Read reset-frame ... \n");   
    do
    {
        m_ftd2Api.Read(&m_regs[0], 1, &numRead);
        //TRACE("%02x ", m_regs[0]);
    }while(numRead > 0);    

    
    //Start test
    unsigned char buf[21];    
    for(int testVal = 0;testVal <256;testVal++)
    {
        if(m_pProgress)
            m_pProgress->Step();
        m_boardRegs.m_testByte = testVal;
        SendDAUConf(m_boardRegs);    
        do
        {
            Sleep(1);    
            status = m_ftd2Api.GetQueueStatus(&RxBytes);            
        }
        while(RxBytes < 21 && tries++ < 100);
        TRACE("tries%d,%d\n",tries, testVal);                        
        m_ftd2Api.Read(buf, 21, &numRead);   

        if(buf[7] != testVal || tries == 100)
        {
            testOK = FALSE;
            break;
        }
        tries = 0;
    }

    m_pauseRead = FALSE;
    if(oldAcqState == 1 || oldCtrlAState == 1 || oldCtrlBState == 1)
    {
        m_boardRegs.m_dataAcqEnable = oldAcqState;    
        m_boardRegs.m_serialCtrlA_Enable = oldCtrlAState;
        m_boardRegs.m_serialCtrlB_Enable = oldCtrlBState;
        SendDAUConf(m_boardRegs);
    }

    if(m_pProgress)
        m_pProgress->Done();
    

    return testOK;
}


int DAOComThread::InitHW()
{    


    struct conf_struct c;
    UINT numWritten;
    Registry reg;
    int len=0;
    char *file_buf;

    CString port = reg.GetStringValue("DAUPort", "");
	m_comPort.Format("\\\\.\\COM%s",port);

    SerialPort serialPort;
    serialPort.Open(FALSE, m_comPort);
    serialPort.SetToDefault();
    serialPort.IsDefault();
    serialPort.Close();

    //FILE* fp = fopen("serial('COM8','BaudRate',38400,'StopBits',1)", "wb");
    FILE* fp = fopen(m_comPort, "wb");
    if(fp == NULL)
    {
        return 1;
    }
    file_buf = (char*)malloc(SETUP_BUF_SIZE);
    len = setvbuf(fp, file_buf, _IOFBF, SETUP_BUF_SIZE);

    CString base = "DAU\\";
    c.sample_speed = reg.GetIntegerValue(base + "SampleSpeed", 0);
    c.test = reg.GetIntegerValue(base + "Test", 0x38);

    c.uartA_en = reg.GetIntegerValue(base + "EnableUartA", 0);
    c.hdlcA_en = reg.GetIntegerValue(base + "EnableHdlcA", 0);
    c.serA_inv = reg.GetIntegerValue(base + "SerAInv", 1);
    c.uartA.baud = reg.GetIntegerValue(base + "UARTA\\Baud", 115200);
    c.uartA.n_bits = reg.GetIntegerValue(base + "UARTA\\Bits", 8);
    c.uartA.rx_filter = reg.GetIntegerValue(base + "UARTA\\rxFilter", 1);
    c.uartA.parity = reg.GetIntegerValue(base + "UARTA\\Parity", 1);
    c.uartA.par_even = reg.GetIntegerValue(base + "UARTA\\ParEven", 0);
    c.hdlcA_speed = reg.GetIntegerValue(base + "hdlcASpeed", 0);

    c.uartB_en = reg.GetIntegerValue(base + "EnableUartB", 0);
    c.hdlcB_en = reg.GetIntegerValue(base + "EnableHdlcB", 0);
    c.serB_inv = reg.GetIntegerValue(base + "SerBInv", 1);
    c.uartB.baud = reg.GetIntegerValue(base + "UARTB\\Baud", 115200);
    c.uartB.n_bits = reg.GetIntegerValue(base + "UARTB\\Bits", 8);
    c.uartB.rx_filter = reg.GetIntegerValue(base + "UARTB\\rxFilter", 1);
    c.uartB.parity = reg.GetIntegerValue(base + "UARTB\\Parity", 1);
    c.uartB.par_even = reg.GetIntegerValue(base + "UARTB\\ParEven", 0);
    c.hdlcB_speed = reg.GetIntegerValue(base + "hdlcBSpeed", 0);

    c.dac = reg.GetIntegerValue(base + "Dac", 0x4000);
    c.sdc_res = reg.GetIntegerValue(base + "sdcRes", 3);

    int i=0;
    for(i=0;i<18;i++) 
        m_regs[i] = 0xAA;
    m_regs[i++] = 0xAA;
    m_regs[i++] = 0x55;
    m_regs[i++] = 0;  // sn
    m_regs[i++] = 0;  // Version
    for (;i<DAU_MSG_LEN;i++) m_regs[i] = 0;

    TRACE("Stop communication\n");
    m_regs[22] = 0x00; // stop;
    len = fwrite(&m_regs[0], 1, DAU_MSG_LEN, fp);
    fflush(fp);

    Sleep(100);
    TRACE("Reading old data ... ");
    len = 0;
    char old;
    //    fread(&old, 1, 1, fp);

    while(fread(&old, 1, 1, fp) > 0)
    {
       // TRACE("%c\n",old);
        len++;
    }
    TRACE("%d bytes read\n", len);    

    printf("Reset...\n");
    m_regs[22] = 0x40; // reset;
    len = fwrite(&m_regs[0], 1, DAU_MSG_LEN, fp);
    fflush(fp);

    Sleep(1000);
    printf("Read reset-frame ... \n");
    len = 0;
    while(fread(&m_regs[0], 1, 1, fp) > 0)
    {
        len++;
        printf("%02x ", m_regs[0]);
    } 

    InitRegs(&c, &m_regs[22]);

    len = fwrite(&m_regs[0], 1, DAU_MSG_LEN, fp);
    fflush(fp);

    if (fp)
        fclose(fp);    

    free(file_buf);

    return 1;
}

BOOL DAOComThread::IsRunning(void) const
{ 
    if( m_pThread == NULL )
    {
        return(FALSE); 
    }
    
        
    return (m_status == S_OK);
    
}

void DAOComThread::GetResolutions(int& a1, int& a2, int& b1, int& b2)
{
    b2 = m_boardRegs.m_syncroResolutionD2J2Pitch;
    b1 = m_boardRegs.m_syncroResolutionD2J2Roll;
    a2 = m_boardRegs.m_syncroResolutionD1J1Pitch;
    a1 = m_boardRegs.m_syncroResolutionD1J1Roll;
}


void DAOComThread::SetResolutions(int a1, int a2, int b1, int b2)
{
    m_boardRegs.m_syncroResolutionD2J2Pitch = b2;
    m_boardRegs.m_syncroResolutionD2J2Roll = b1;
    m_boardRegs.m_syncroResolutionD1J1Pitch = a2;
    m_boardRegs.m_syncroResolutionD1J1Roll = a1;
    
    SendDAUConf(m_boardRegs);
}



void DAOComThread::EnableDigInterface(DigDauChPro interfaceCh, bool enable)
{
    if(interfaceCh == DigDauChProA)
    {        
        m_boardRegs.m_serialCtrlA_Enable = enable;            
    }   
    else
    {
        m_boardRegs.m_serialCtrlB_Enable = enable;            
    }
    SendDAUConf(m_boardRegs);
}

// void DAOComThread::SetDigInterface(DigDauChPro interfaceCh, DigChSerialTypePro interfaceType)
// {
//     int HDLC_UART_Sel = (interfaceType == DigChSerialTypeProHdlc) ? 1 : 0;
//     
//     if(interfaceCh == DigDauChProA)
//     {        
//         m_boardRegs.m_serialCtrlA_HdlcUartSel = HDLC_UART_Sel;            
//     }   
//     else
//     {
//         m_boardRegs.m_serialCtrlB_HdlcUartSel = HDLC_UART_Sel;            
//     }
//     
//     SendDAUConf(m_boardRegs);
// }


void DAOComThread::SetDigInterfaceSettings(DigDauChPro interfaceCh, DigChSerialTypePro interfaceType, DigChTypePro type)
{    
    map<int, ValidSyncroInfo>& syncroTypes = SysSetup->m_validSyncroTypes;
    if(syncroTypes.find(type) != syncroTypes.end())
    {
        ValidSyncroInfo info = syncroTypes[type];      
        int HDLCActive = (info.serialType == DigChSerialTypeProHdlc) ? 1 : 0;    
        if(interfaceCh == DigDauChProA)
        {        
            m_boardRegs.m_serialCtrlA_HdlcUartSel = HDLCActive;            
            m_boardRegs.m_serialCtrlA_Enable = 1;            
            if(HDLCActive)
            {
                m_boardRegs.m_serialCtrlA_HdlcBitrate = info.rate;
            }
            else
            {
                m_boardRegs.m_uartCtrlA_BaudRate = (double)19660800/info.rate - 0.5; //-1+0.5
                m_boardRegs.m_uartCtrlA_m_noOfDataBits = info.bits-5;
                m_boardRegs.m_uartCtrlA_m_parityOn = info.parity != 0;
                m_boardRegs.m_uartCtrlA_m_parityEven = info.parity == 1;
            }
        }   
        else
        {
            m_boardRegs.m_serialCtrlB_HdlcUartSel = HDLCActive;            
            m_boardRegs.m_serialCtrlB_Enable = 1;            
            if(HDLCActive)
            {
                m_boardRegs.m_serialCtrlB_HdlcBitrate = info.rate;
            }
            else
            {
                m_boardRegs.m_uartCtrlB_BaudRate = (double)19660800/info.rate - 0.5; //-1+0.5
                m_boardRegs.m_uartCtrlB_m_noOfDataBits = info.bits-5;
                m_boardRegs.m_uartCtrlB_m_parityOn = info.parity != 0;
                m_boardRegs.m_uartCtrlB_m_parityEven = info.parity == 1;
            }

        }
    }
    SendDAUConf(m_boardRegs);  
}

void DAOComThread::SetUARTType(int UARTNum, DigChTypePro UARTType)
{
    m_UARTType[UARTNum] = UARTType;
}

