// $Id: DAU.cpp,v 1.33 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "DAU.h"

#include "UserMessages.h"
#include "ProgressWnd.h"
#include "SelectDAUDialog.h"
#include "Defines.h"
#include "SensorAlignmentSheet.h"
#include "DigitalProperties.h"
#include "SyncroProperties.h"
#include "SensorAlignmentPage.h"

#include "Aligner.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include "SystemConfigurationView.h"
#include "AutoLock.h"
#include "CalibInfo.h"
#include "Util.h"
#include "SystemSetup.h"
#include "Network.h"
#include "json/json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DAU &DAU::GetDAU( void )
{
  return sm_dau ;
}

DAU DAU::sm_dau ;

DAU::DAU( void ):
m_simulationActive(FALSE),
m_digitalDataArrived(FALSE),
m_overangeDetectionActive(FALSE)
{
    m_enableErrorEvent = FALSE ;
    m_running = FALSE ;
    m_timeoutCounter = 0 ;	
    InitializeCriticalSection(&m_lock);
    m_dauFramesCounter.resize(10);
    m_maxNumRecentDacVal = 10; //sliding mean of num samples.
    m_logDACData = FALSE;

    m_temperatureChMap[0] = "J4";
    m_temperatureChMap[1] = "J5";
    m_temperatureChMap[2] = "J6";
    m_temperatureChMap[3] = "J7";
    m_temperatureChMap[4] = "J8";
    m_temperatureChMap[5] = "J9";
    m_temperatureChMap[7] = "J3";

	QueryPerformanceCounter(&m_lastTimerCheck);	
	int res = QueryPerformanceFrequency(&m_freq);	
	
	m_comThr.SetCallback(StaticFrameCallback, this);	
	m_comThr.SetStatusCallback(StaticStatusCallback, this);	
    
    m_activeHighSeaGyro = "";
	m_mqttMsg = 1;
}

DAU::~DAU( void )
{
  ClearConfig() ;
  Clear() ;
}


class Slayer
{
public:
  Slayer( void ) {} ;
  ~Slayer( void ) {
    ::OutputDebugString( m_text ) ;
  }
  CString &operator()( void ) {
    return m_text ;
  }
private:
  CString m_text ;
} ;


BOOL DAU::StopDAUComThread()
{
    return m_comThr.Destroy();
}


BOOL DAU::StartDAUComThread()
{
    Clear();

  // m_comThr.SetCallback(StaticFrameCallback, this);	
    if(m_comThr.Create(0))
    {
        Clear() ;
        //AfxGetApp()->PostThreadMessage( UM_TIMER, 0, 0 ) ;   
        return FALSE ;
    }
//    SetDACVoltage(32768); //
    return TRUE;
}


void DAU::ResetSensorCompensations()
{
    vector<Sensor*>::iterator iter;
    for ( iter = m_sensorPointers.begin() ; iter != m_sensorPointers.end() ; iter++)
    {
        (*iter)->SetCurrentAz(0.0f);
        (*iter)->m_zParCorr = 0.0f;
        (*iter)->SetRotateBackAng(0.0f, FALSE);
    }
}

BOOL DAU::Setup()
{
  Clear() ;
  ClearConfig() ;

  int res = LoadConfig();    
  
  m_stop = 0 ;
  
  return res ;
}

void DAU::StatusCallback(WPARAM wp, LPARAM lp)
{
	TRACE("StatusCallback: %d\n",wp);
	if(m_pParent)
        m_pParent->PostMessage(UM_DAU_STATUS, wp, lp);	
}

void DAU::FrameCallback(WPARAM wp, LPARAM lp)
{
    int type = (int)lp;
    switch(type)
    {
        case CB_TYPE_FRAME:
        {
            DAUFrame* pFrame = (DAUFrame*)wp;
			AutoLock lock(&m_lock);
			m_DAUFramesQue.push_back(*pFrame);  

            

//                 if(pFrame->type == FRAME_TYPE_SENSOR && m_digitalDataArrived)
//                 {
// 					 				static int c=0;
// 					 				tr.Format("Bef Sync: %d",c++);
// 					 				stw.TraceTime(TRUE, TRUE, tr);
// 
// 					m_pParent->PostMessage(UM_SYNC_DIGITAL, 0, 0);
//                     m_digitalDataArrived = FALSE;
// 				}                
            break;
        }
        case CB_TYPE_DAC_TEST:
        {
            if(wp == 1)
            {
                m_pParent->PostMessage(UM_DAC_TEST_ERR, 0, 0);
            }
            m_dauFramesCounter[FRAME_TYPE_SENSOR]++;        
            break;
        }
        default:break;
    }
    

    //TRACE("new frame:%d, %d\n", pFrame->type, m_DAUFramesQue.size());
}

int DAU::HandleDAUFrames()
{
    int numFrames=0;
    std::vector<DAUFrame> DAUFrames;
    {     
        AutoLock lock(&m_lock);
        numFrames = m_DAUFramesQue.size();
        copy(m_DAUFramesQue.begin(), m_DAUFramesQue.end(), back_inserter(DAUFrames));
        m_DAUFramesQue.clear();
    }

    //TRACE("Frames:%d\n",DAUFrames.size());

//     static int numReveived=0;
//     static int numReceivedReg=0;
	int numSensorFrames=0;
    std::vector<DAUFrame>::iterator iter;
    for(iter=DAUFrames.begin();iter!= DAUFrames.end();iter++)
    {
        m_dauFramesCounter[iter->type]++;        

        switch(iter->type)
        {
            case FRAME_TYPE_SENSOR:
            {
                HandleSensorData(*iter);                			
				numSensorFrames++;
			}
            break;
            case FRAME_TYPE_HDLC_A:
            {
                Digital* pDigital = (Digital*)GetSyncro(D2Name());                
                HandleDigitalData(*iter, pDigital);
            }
            break;
            case FRAME_TYPE_HDLC_B:
            {
                Digital* pDigital = (Digital*)GetSyncro(D1Name());
                HandleDigitalData(*iter, pDigital);
            }
            break;
            case FRAME_TYPE_UART_A:
            {
                Digital* pDigital = (Digital*)GetSyncro(D2Name());                
                HandleDigitalData(*iter, pDigital);    
            }
            break;
            case FRAME_TYPE_UART_B:
            {
                Digital* pDigital = (Digital*)GetSyncro(D1Name());                
                HandleDigitalData(*iter, pDigital);    
                //Beep(1000,100);
                //TRACE("UART B\n");
            }
            break;
            case FRAME_TYPE_REG:
            {   
                memcpy(&m_mainBoardRegsPro, &iter->m_boardRegs, sizeof(MainBoardRegsPro));
                CString str;//  = "021";
                str.Format("%d",m_mainBoardRegsPro.m_dauSerialNo);
                SetSerialNumber(str);
                UpdateResolutionsFromDAU();
                //      numReceivedReg++;
                TRACE("set val: %d\n",m_mainBoardRegsPro.m_testVoltage);
            }
            break;
            default:break;
        }           

    }
    
	if(DAU::GetDAU().GetStatus() == DAU_STATUS_OK)
	{
		//CheckDataRate(numSensorFrames);
	}


    //TRACE("received:%d, reg:%d\n",numReveived, numReceivedReg);
    return numFrames;
}

void DAU::SetResolutionsToDAU()
{
    if(!theApp.IsAligner202Enabled())
        return;

    Syncro* pSA1 = GetSyncro(A1);
    Syncro* pSB1 = GetSyncro(B1);
    Syncro* pSA2 = GetSyncro(A2);
    Syncro* pSB2 = GetSyncro(B2);

    if(!pSA1 || !pSA2 || !pSB1 || !pSB2)
    {
        return;
    }

    m_comThr.SetResolutions(pSA1->GetResolution(), pSA2->GetResolution(), pSB1->GetResolution(), pSB2->GetResolution());
}


void DAU::UpdateResolutionsFromDAU()
{
    if(!theApp.IsAligner202Enabled())
        return;

    Syncro* pSA1 = GetSyncro(A1);
    Syncro* pSB1 = GetSyncro(B1);
    Syncro* pSA2 = GetSyncro(A2);
    Syncro* pSB2 = GetSyncro(B2);

    if(!pSA1 || !pSA2 || !pSB1 || !pSB2)
    {
        return;
    }

    pSA1->SetResolution(m_mainBoardRegsPro.m_syncroResolutionD1J1Roll);
    pSA2->SetResolution(m_mainBoardRegsPro.m_syncroResolutionD1J1Pitch);
    pSB1->SetResolution(m_mainBoardRegsPro.m_syncroResolutionD2J2Roll);
    pSB2->SetResolution(m_mainBoardRegsPro.m_syncroResolutionD2J2Pitch);

}

BOOL DAU::CheckDataRate(int numFramesHandled)
{		
	LARGE_INTEGER perfCounter;
	QueryPerformanceCounter(&perfCounter);
	static int framesSinceLastCheck =0;
	framesSinceLastCheck += numFramesHandled;
	static int numBelowAccept=0;

	double d = (double)(perfCounter.QuadPart-m_lastTimerCheck.QuadPart)/(double)m_freq.QuadPart;
	double timeIntervall=1.0f;
	double timePast = (double)(perfCounter.QuadPart-m_lastTimerCheck.QuadPart)/(double)m_freq.QuadPart;
	// 	TRACE("d:%f\n",d);
	if(timePast > timeIntervall && timePast < (timeIntervall*2))
	{		
		m_lastTimerCheck = perfCounter;
		double framesPerSec = (double)framesSinceLastCheck/timePast;
		double acceptableRate = m_comThr.GetSamplingRate()*0.7f;
		//TRACE("Check rate: %f, %d, %f, %f(%f)\n",d, framesSinceLastCheck, framesPerSec, m_comThr.GetSamplingRate(),acceptableRate);
		framesSinceLastCheck = 0;

		numBelowAccept = (framesPerSec < acceptableRate) ? numBelowAccept+1 : 0;
		
		if(numBelowAccept >= 3)
		{
			//Something wrong with the rate!
 			DAU::GetDAU().Restart();
		}
	}
	else if(timePast >= (timeIntervall*2))
	{
		m_lastTimerCheck = perfCounter;
	}

	return TRUE;
}


void DAU::SyncDigital()
{
    static int c=0;
    //TRACE("Sync:%d\n",c++);
    for (vector<Syncro*>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
    {
        //TRACE("Name:%s", (*i)->GetName());
        Digital* pDig = dynamic_cast<Digital*>(*i);
        if(pDig != NULL)
        {
            //Set only digital data, not syncro
            if(pDig->GetType() != UnitType::Unused)
            {
                pDig->SetCandidate();                

// 				tr.Format("Sync");
//  				stw.TraceTime(TRUE, TRUE, tr);
			}
        }
    }    
}

void swap(short& data)
{
    unsigned char* pData = (unsigned char*)&data;
    unsigned char tmp=pData[0];
    pData[0] = pData[1];
    pData[1] = tmp;
}


void DAU::InvertSensorData(DAUFrame& frame)
{
    for(int i=0;i<2;i++)
    {
        swap(frame.sdcAdcMsgData.m_syncro[i].m_pitch);
        //frame.sdcAdcMsgData.m_syncro[i].m_pitch = -frame.sdcAdcMsgData.m_syncro[i].m_pitch;
        
        swap(frame.sdcAdcMsgData.m_syncro[i].m_roll);
        //frame.sdcAdcMsgData.m_syncro[i].m_roll  = -frame.sdcAdcMsgData.m_syncro[i].m_roll;
    }
    for(int i=0;i<7;i++)
    {
        frame.sdcAdcMsgData.m_sensor[i].m_pitch = -frame.sdcAdcMsgData.m_sensor[i].m_pitch;
        frame.sdcAdcMsgData.m_sensor[i].m_roll = -frame.sdcAdcMsgData.m_sensor[i].m_roll;
    }
    frame.sdcAdcMsgData.m_temperature.m_temperature = -frame.sdcAdcMsgData.m_temperature.m_temperature;
}

short DAU::GetFilteredMeasuredDACVoltage( void )
{
    if(m_recentRecDACVal.size()==0)
        return 0;

    int total =0;
    deque<short>::iterator iter; 
    for(iter = m_recentRecDACVal.begin();iter!=m_recentRecDACVal.end();iter++)
    {
        total += *iter;
    }        
    return total/(int)m_recentRecDACVal.size();
}


void DAU::AddToReceivedDACVal(short val)
{
    m_recentRecDACVal.push_back(val);
    if(m_recentRecDACVal.size() > m_maxNumRecentDacVal)
    {
        m_recentRecDACVal.pop_front();
    }
}

void DAU::ExportMQTT()
{
	if (!m_exportMQTT || !m_mqttLib.IsOpen())
		return;

	m_mqttLib.MQTTLoop(1000);
	
	if (m_mqttMsg == 0)
		return;

	BOOL calibrated = (m_mqttMsg != 2);
	BOOL unfilter = (m_mqttMsg == 3);

	CString msg;
	for (auto i = m_sensorPointers.begin(); i != m_sensorPointers.end(); i++)
	{
		CString tmp;		
		if (unfilter)
		{
			tmp.Format("%f,%f,", (*i)->GetUnfilteredRoll(), (*i)->GetUnfilteredPitch());
		}
		else
		{
			tmp.Format("%f,%f,", (*i)->GetRoll(calibrated), (*i)->GetPitch(calibrated));
		}
		msg += tmp;
	}
	msg.TrimRight(',');

	m_mqttLib.SendMsg((char*)(LPCTSTR)msg, MQTT_A308MODULE_LIVE_DATA);
}


int DAU::HandleSensorData(DAUFrame& frame)
{
    InvertSensorData(frame);

    if(m_simulationActive)
        memcpy(&frame.sdcAdcMsgData.m_syncro, &m_simulatedData.m_syncro, 9*2*sizeof(__int16));
    
    if(m_logDACData)
        Log2File(m_recDACVal, "DAC.txt");
    m_recDACVal = frame.sdcAdcMsgData.m_testVoltage;
    AddToReceivedDACVal(m_recDACVal);     
    m_recSetDACVal = frame.sdcAdcMsgData.m_testSetVoltage;            

    BOOL outOfRange = FALSE;
    CString outOfRangeSensor ;

    //TRACE("temp channel:%d\n",frame.sdcAdcMsgData.m_statusTempChannel);
    for (vector<Sensor*>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
    {
        //TRACE("offset:%d, %s\n",(*i)->GetOffset(),(*i)->GetName());
        if(!(*i)->SetData( frame.sdcAdcMsgData.m_sensor[(*i)->GetOffset()]))
        {
            outOfRangeSensor = (*i)->GetName() ;
            outOfRange = TRUE ;
        }
    }

	ExportMQTT();

    if(!outOfRange)
    {
        outOfRangeSensor = theApp.IsAligner202Enabled() ? HandleSyncroData202(frame) : HandleSyncroData308(frame);   
        outOfRange = (outOfRangeSensor != "");
    }

    int tempChannel = frame.sdcAdcMsgData.m_statusTempChannel;
    switch(tempChannel)
    {
        case 6:
        m_mainBoard.SetTemperatureData( frame.sdcAdcMsgData.m_temperature) ;
        break ;
       
        default:
        unsigned int i = (tempChannel + 1) % 8;                
        if(i < m_sensorPointers.size())
        {
            Sensor* pSens = GetSensor(m_temperatureChMap[tempChannel]);
            if(pSens)
            {
                pSens->SetTemperatureData( frame.sdcAdcMsgData.m_temperature ) ;
            }            
        }
        break ;
    }   

    NotifyListeners();
    
        
    if (IsErrorEventEnabled()  )
    {

		if(outOfRange)
		{
			EnableErrorEvent( FALSE ) ;    
			TRACE("Out of range!!\n");    
			//CSingleLock _(&m_listenersLock, TRUE) ;        		
			ErrorEventInfo info(ET_OUT_OF_RANGE, 0, outOfRangeSensor);			
			__raise ErrorEvent(info ) ;    
		}
		CheckComErrors();
    }
    return 0;
}

CString DAU::HandleSyncroData308(DAUFrame& frame)
{    
    CString outOfRangeSensor="" ;
    for (vector<Syncro*>::iterator syncI = m_syncroPointers.begin() ; syncI != m_syncroPointers.end() ; syncI++)
    {
        //TRACE("Name:%s", (*i)->GetName());
        Digital* pDig = dynamic_cast<Digital*>(*syncI);
        if(pDig != NULL)
            continue;

        //Set only syncro data, not digital
        if((*syncI)->GetType() != UnitType::Unused)
        {
            //            TRACE("roll:%d, %x\n",frame.sdcAdcMsgData.m_syncro[1].m_roll,frame.sdcAdcMsgData.m_syncro[1].m_roll);
            int offset = ((*syncI)->GetName() == "J1") ? 1 : 0; // Because syncros are swapped from DAU...first J2 then J1.

            if(!(*syncI)->SetData( frame.sdcAdcMsgData.m_syncro[offset]))
            {
                outOfRangeSensor = (*syncI)->GetName() ;
             }
        }

    }
    return outOfRangeSensor;
}

CString DAU::HandleSyncroData202(DAUFrame& frame)
{    
    CString outOfRangeSensor="" ;

    Syncro *pA1 = GetSyncro(A1) ;
    Syncro *pB1 = GetSyncro(B1) ;
    Syncro *pC1 = GetSyncro(C1) ;
    Syncro *pA2 = GetSyncro(A2) ;
    Syncro *pB2 = GetSyncro(B2) ;
    Syncro *pC2 = GetSyncro(C2) ;

    vector<Syncro *> channel;
    channel.push_back(pA1);
    channel.push_back(pB1);
    channel.push_back(pA2);
    channel.push_back(pB2);

    vector<Syncro*>::iterator iter;
    for(iter = channel.begin();iter!=channel.end(); iter++)
    {
        Syncro* pSync = *iter;
        if(pSync && (pSync->GetType() != UnitType::Unused))
        {
            SyncroDataPro data ;
            data = frame.sdcAdcMsgData.m_syncro[pSync->GetOffset()/2] ;
            if(pSync->GetOffset()%2==0)
                data.m_pitch = data.m_roll; 
            else
                data.m_roll = data.m_pitch; 
            
            pSync->SetData(data) ;
            //outOfRangeSensor = pSyncI->GetName() ;
        }
    }
   
    if(pC1 && pC1->GetType() == UnitType::Composite)
    {
        double composite ;
        double coarse = (pA1->GetType() == UnitType::Coarse) ? pA1->GetRoll() : pB1->GetRoll();
        double fine = (pA1->GetType() == UnitType::Fine) ? pA1->GetRoll() : pB1->GetRoll();
        double gearing = (pA1->GetType() == UnitType::Fine) ? pA1->GetGearing() : pB1->GetGearing();
        GetCompositeAngle( coarse, fine, gearing, &composite ) ;
        //TRACE("coarse:%.2f, fine:%.2f, comp:%.2f\n",coarse, fine, composite);
        pC1->SetData(composite) ;
    }
    
    if(pC2 && pC2->GetType() == UnitType::Composite)
    {
        double composite ;
        double coarse = (pA2->GetType() == UnitType::Coarse) ? pA2->GetRoll() : pB2->GetRoll();
        double fine = (pA2->GetType() == UnitType::Fine) ? pA2->GetRoll() : pB2->GetRoll();
        double gearing = (pA2->GetType() == UnitType::Fine) ? pA2->GetGearing() : pB2->GetGearing();
        GetCompositeAngle( coarse, fine, gearing, &composite ) ;
        pC2->SetData(composite) ;
    }

    return outOfRangeSensor;
}


void DAU::SendInvalidGyroDataMsg()
{
    if(IsErrorEventEnabled()  )
    {
        EnableErrorEvent( FALSE ) ;    				
        ErrorEventInfo info(ET_COM_ERROR, 0, "Invalid Gyro data");			
        __raise ErrorEvent(info ) ;    			
    }
}

void DAU::SendDAUDeadMsg()
{
	if(IsErrorEventEnabled()  )
	{
		EnableErrorEvent( FALSE ) ;    				
		ErrorEventInfo info(ET_COM_ERROR, 0, "DAU stopped receiving data");			
		__raise ErrorEvent(info ) ;    			
	}
}

void DAU::CheckComErrors()
{
	int error=COM_ERR_OK;
	if((error=m_comThr.m_errorHandler.CheckForError()) != COM_ERR_OK)
	{
		EnableErrorEvent( FALSE ) ;    			
		ErrorEventInfo info(ET_COM_ERROR, error, m_comThr.m_errorHandler.GetErrorDisc(error));			
		__raise ErrorEvent(info ) ;    					
	}	
}

int DAU::HandleDigitalData(DAUFrame& frame, Digital* pDigital)
{
    if(pDigital == NULL)
        return -1;

    pDigital->HandleDataFrame(frame);
	
	if(pDigital->m_CRCError && pDigital->IsSelected())
	{
		int err = (pDigital->GetName() == D1Name()) ? COM_ERR_DIG_CH1_INVALID_CRC : COM_ERR_DIG_CH2_INVALID_CRC;
		m_comThr.m_errorHandler.AddError(err);
		pDigital->m_CRCError = FALSE;
	}

    m_digitalDataArrived = TRUE;
	pDigital->SetCandidate();

    return 0;
}


BOOL DAU::Clear( void )
{
  
  m_DAUFramesQue.clear();   
  return TRUE ;
}

BOOL DAU::IsOpen( void ) const
{
    return m_comThr.IsRunning();    
}

int DAU::GetStatus()
{
    return m_comThr.GetStatus();
}

void DAU::SetDACVoltage( int voltage )
{
    m_comThr.SetDACVoltage(voltage );
}

void DAU::ClearConfig( void )
{
  for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
  {
    delete *i ;
  }
  for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
  {
    delete *i ;
  }
  m_sensorMap.clear() ;
  m_syncroMap.clear() ;
  m_sensorPointers.clear() ;
  m_syncroPointers.clear() ;
}


static const char DAU_REGISTER_SECTION []          = "" ;
static const char DAU_REGISTER_PORTNAME []         = "port" ;
static const char DAU_REGISTER_PORTNAME_DEFAULT [] = "COM1" ;
static const char DAU_REGISTER_SERIAL []           = "serial" ;
static const char DAU_REGISTER_SERIAL_DEFAULT []   = "" ;
static const char DAU_REGISTER_ID []               = "id" ;
static UINT       DAU_REGISTER_ID_DEFAULT          = 0 ;

BOOL DAU::LoadSensorConfig( LONG id )
{
    DBInterface::Instance()->GetSensors(m_sensorPointers, id);
    
    vector<Sensor*>::iterator iter;
    for(iter = m_sensorPointers.begin();iter!=m_sensorPointers.end();iter++)
    {
        m_sensorMap[(*iter)->GetName()] = *iter;
        SysSetup->AddSensor((*iter)->GetOffset()) ;
    }
 
    return TRUE ;
}

BOOL DAU::LoadSyncroConfig( LONG id )
{
	DBInterface::Instance()->GetSyncros(m_syncroPointers, id);

	vector<Syncro*>::iterator iter;
	for(iter = m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
	{
		m_syncroMap[(*iter)->GetName()] = *iter;
	}
	
	if(theApp.IsAligner308Enabled())
	{
		for(iter = m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
		{
			SysSetup->AddSyncro((*iter)->GetOffset()) ;
		}
	}
	else
	{
		//Always first 6 bits enabled
		for(int i=0;i<6;i++)
		{
			SysSetup->AddSyncro(i) ;
		}
	}

    return TRUE ;
}

void MsgCB(void* pC, char* msg, char* top)
{
	TRACE("topic:%s, msg:%s\n", top, msg);
	DAU* pObj = (DAU*)pC;
	pObj->m_mqttMsg = atoi(msg);
}

BOOL DAU::CheckDAUKey(CString DAUSerial, DAUSetupData& dauData)
{
  CString s = dauData.meta;
  
  Json::Value root;
  Json::Reader read;
  std::string meta(dauData.meta);
  read.parse(meta, root);

  Json::Value key = root["key"];
  CString keyStr = "";

  try
  {
    keyStr = key.asCString();
 
  }
  catch (Json::Exception e)
  {
    TRACE(e.what());
    return FALSE;
  }
  
  if (!Features::CheckString(DAUSerial, keyStr))
  {
    ::AfxMessageBox(_T("Invalid DAU Key!"));
    return FALSE;
  }

  //std::ifstream f("test.json");
  //json data = json::parse(f);
  //json data;
  //json data = json::parse((LPCTSTR)dauData.meta);

  //std::string key = data["key"];


  return TRUE;
}

BOOL DAU::LoadConfig()
{
  m_mask = 255 ;
  
#if defined(DAU_SIMULATION)
  //m_serial = 3 ;
#endif
  
  m_serial = m_comThr.m_dauSN ;
  m_dauDBData.Clear();

  DBInterface::Instance()->GetDAUData(m_serial, m_dauDBData);
  if (!CheckDAUKey(m_comThr.GetFTDISerial(), m_dauDBData))
    return FALSE;

  SysSetup->SetDAUSerial(m_serial);
  //Check

  Registry reg;
  m_exportMQTT = (reg.GetIntegerValue("DAU\\MQTTExport", 0) == 1);
  if (m_exportMQTT)
  {
	  if (m_mqttLib.LoadDll())
	  {		  
		  m_mqttLib.Init(A308MODULE_CLIENT_ID, BROKER_ADDRESS, MQTT_PORT);
		  m_mqttLib.Subscribe(MQTT_A308_SET_MODULE_ACTION, 2);
		  m_mqttLib.SetCB(MsgCB, this);
	  }
	  else
		  m_exportMQTT = false; 
  }
  

  //test
    /*CalibrationInfo info;
    DBTIMESTAMP ts;
    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP(ts);
    info.SetCalibrationTime(ts);
    info.AddChannel("J4");
    info.AddChannel("J6");
    info.AddAdapter("51");
    info.AddAdapter("50");    
    info.AddSensor("006");
    info.AddSensor("091");
    CString inf = info.GetInfo();
    TRACE("Info: %s\n",inf);*/


  CString str;
  str.Format("%03d",m_serial);
  SetSerialNumber(str) ;
  m_protocolVersion = 0;// m_dauDBData.protocolVersion;
  m_sampleAndHold = 0;// m_dauDBData.sampleAndHold;
 
  SysSetup->ClearUnits() ;
  if (!LoadSyncroConfig(m_dauDBData.DBId))
  {
      return FALSE ;
  }
  if (!LoadSensorConfig(m_dauDBData.DBId))
  {
      return FALSE ;
  }

  int digitalSetup = reg.GetIntegerValue("DAU\\DigitalSetup", DIG_D1 | DIG_D2); 

    if((digitalSetup & DIG_D1) != 0)
    {
        CString name = D1Name();
        Digital *pDigital = new Digital( name, theApp.IsAligner202Enabled() ) ;
        m_syncroPointers.push_back( pDigital ) ;
        m_syncroMap[name] = pDigital ;    
    }

    if((digitalSetup & DIG_D2) != 0)
    {
        CString name = D2Name();
        Digital *pDigital = new Digital( name, theApp.IsAligner202Enabled() ) ;
        m_syncroPointers.push_back( pDigital ) ;
        m_syncroMap[name] = pDigital ;    
    }  

   
  //SortSensors();
  SortSyncros();
 
  return TRUE ;
}
 
void DAU::SortSensors()
{
    reverse(m_sensorPointers.begin(), m_sensorPointers.end());

}

void DAU::SortSyncros()
{
    vector<Syncro *>::iterator iter;  
    TRACE("Before:\n");
    for(iter=m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
    {
        TRACE("%s\n",(*iter)->GetName());
    }
    
    m_syncroPointers.clear();
    vector<Syncro *> syncros;  
    //Try to sort syncros in this order: D1 J1 D2 J2
    syncros.push_back(GetSyncro(D1));
    syncros.push_back(GetSyncro(J1));
    syncros.push_back(GetSyncro(D2));
    syncros.push_back(GetSyncro(J2));
    syncros.push_back(GetSyncro(A1));
    syncros.push_back(GetSyncro(B1));
    syncros.push_back(GetSyncro(C1));
    syncros.push_back(GetSyncro(A2));
    syncros.push_back(GetSyncro(B2));
    syncros.push_back(GetSyncro(C2));
    syncros.push_back(GetSyncro(CH3));
    syncros.push_back(GetSyncro(CH4));
    for(iter=syncros.begin();iter!=syncros.end();iter++)
    {        
        if(*iter != NULL)
        {
            TRACE("%s\n",(*iter)->GetName());       
            m_syncroPointers.push_back(*iter);
        }
    }

    TRACE("After:\n");
    for(iter=m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
    {
        TRACE("%s\n",(*iter)->GetName());
    }


}

void DAU::SetDefaultCalibration( void )
{
  for (unsigned int i = 0 ; i < m_sensorPointers.size() ; i++)
  {
    m_sensorPointers [i]->SetChannelCalibrationData( ChannelCalibrationData(), ChannelCalibrationData() ) ;
  }
}

BOOL DAU::Restart(BOOL showMsg)
{
    //Restart DAU    
    SetSerialNumber("000");
	int status = m_comThr.ReStart();
    if(status == DAU_STATUS_OK)
    {

    }
   
    return TRUE ;
}

BOOL DAU::Stop( void )
{
  
  return TRUE ;
}



Sensor *DAU::GetSensor( const CString &name )
{
  map<CString,Sensor *>::iterator position = m_sensorMap.find( name ) ;
  return position != m_sensorMap.end() ? (*position).second : NULL ;
}

Syncro *DAU::GetSyncro( const CString &name )
{
  map<CString,Syncro *>::iterator position = m_syncroMap.find( name ) ;
  return position != m_syncroMap.end() ? (*position).second : NULL ;
}

void DAU::UpdateSyncroSettings()
{
    vector<Syncro *>::iterator iter;
    for(iter=m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
    {             
        Digital* pDig = dynamic_cast<Digital*>(*iter);           
        if(pDig != NULL)
        {            
            pDig->SetSyncroType(pDig->m_syncroType, true);   
        }

    }
}

BOOL DAU::CheckDigitalDataReception()
{
	vector<Syncro *>::iterator iter;
	for(iter=m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
	{             
		Digital* pDig = dynamic_cast<Digital*>(*iter);           
		if(pDig != NULL)
		{            
//			pDig->m_isSelected
			if(pDig->GetType() != UnitType::Unused && pDig->IsSelected())
			{
				if(!pDig->CheckIfDataReceived())
				{
					int err = (pDig->GetName() == D1Name()) ? COM_ERR_DIG_CH1_NO_DATA : COM_ERR_DIG_CH2_NO_DATA;
					m_comThr.m_errorHandler.AddError(err);
					//m_comThr.m_comErr[err].alertWhenActive = pDig->IsSelected();
				}
			}
		}
	}
	return true;
}

void DAU::NotifyListeners( void )
{
    //TRACE("Recepients:%d\n", DataRecepient::GetNumRecepient());    
    if(DataRecepient::GetNumRecepient() > 0)
    {
        DAUValues newValues = m_dataCompensator.GetDAUData(this);
		//DAUValues newValues( *this ) ;
        try
        {
            __raise DataEvent( newValues ) ;
			//TRACE("After raise\n");
		}
        catch (...)
        {
        }
    }
}

Syncro* DAU::GetHighSeaCompGyro()
{    
    for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
    {
        if((*i)->GetHighSeaComp())
        {
            return *i;
        }
    }
    return NULL;
}

void DAU::InitDataCompensator(BOOL activeate)
{    
    if(activeate)
        m_dataCompensator.Init();
    else
        m_dataCompensator.Deactivate();
}

static double DegreesToRadians( double degrees )
{
  return M_PI * degrees / 180.0 ;
}

static double RadiansToDegrees( double radians )
{
  return 180.0 * radians / M_PI ;
}

void DAU::RemoveBuddyUsedChannel(CString str)
{
    m_usedBuddyChannelSet.erase(str);
}

void DAU::AddBuddyUsedChannel(CString str)
{    
    m_usedBuddyChannelSet.insert(str);
}

bool DAU::IsBuddyChannelUsed(CString str)
{
    set<CString>::iterator iter = m_usedBuddyChannelSet.find(str);
    return (iter != m_usedBuddyChannelSet.end());        
}

void DAU::SetPropertyPageBuddies(map<CString, CPropertyPage*>& syncMap)
{
    map<CString, CPropertyPage*>::iterator digIter, syncIter;
    if((digIter = syncMap.find(D1Name())) != syncMap.end() && (syncIter = syncMap.find("J1")) != syncMap.end())
    {
        ((DigitalProperties*)digIter->second)->m_buddy = J1;
        ((SyncroProperties*)syncIter->second)->m_buddy = D1Name();
    }
    
    if((digIter = syncMap.find("D2")) != syncMap.end() && (syncIter = syncMap.find("J2")) != syncMap.end())
    {
        ((DigitalProperties*)digIter->second)->m_buddy = J2;
        ((SyncroProperties*)syncIter->second)->m_buddy = D2Name();
    }
}

Sensor *DAU::GetSensorFromSN(const CString &sn)
{
	for (auto it = m_sensorMap.begin(); it != m_sensorMap.end(); it++)
	{
		if (sn == it->second->GetSerialNumber())
			return it->second;
	}

	return NULL;
}

//*******************************************************************************//
//***															ConfigSensors																***//
//*** ----------------------------------------------------------------------- ***//
//***	Prepares the ch setup Property Page, opens it and handles the set data on BnOK		***//
//*******************************************************************************//
BOOL DAU::ConfigSensors( BOOL sensorsOnly )
{
  if (0 == m_sensorPointers.size() && (0 == m_syncroPointers.size() || sensorsOnly))
  {
    return( FALSE ) ;
  }

  SensorAlignmentSheet sheet( IDS_SENSORALIGNMENT_CONFIGURATION_TITLE ) ;  
  map<CString, CPropertyPage*> syncMap;

  if (!sensorsOnly)
  {
    for (UINT i = 0 ; i < m_syncroPointers.size() ; i++)
    {
      Digital *pDigital ;
      if (( pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ))
      {
        DigitalProperties *pProperties = new DigitalProperties ;
        pProperties->GetPSP().pszTitle = m_syncroPointers [i]->GetName() ;
        pProperties->GetPSP().dwFlags |= PSP_USETITLE ;
        
        //pProperties->m_highSeaComp = pDigital->GetHighSeaComp();
        pProperties->m_DigChTypePro = pDigital->GetSyncroType();
		pProperties->m_description = pDigital->GetUnitTypeDescription() ;
		pProperties->m_nominalAzimuth = pDigital->GetNominalAzimuth() ;
		pProperties->m_pDigital = pDigital;
        sheet.AddPage( pProperties ) ;        
        syncMap[m_syncroPointers[i]->GetName()] = pProperties;  
      }
      else
      {
        SyncroProperties *pProperties = new SyncroProperties ;
        pProperties->GetPSP().pszTitle = m_syncroPointers [i]->GetName() ;
        pProperties->GetPSP().dwFlags |= PSP_USETITLE ;
        pProperties->m_gearing = m_syncroPointers [i]->GetGearing() ;
        pProperties->m_description = m_syncroPointers [i]->GetUnitTypeDescription() ;
        pProperties->m_type = static_cast<int>(m_syncroPointers [i]->GetType()) ;
        pProperties->m_nominalAzimuth = m_syncroPointers [i]->GetNominalAzimuth() ;
        pProperties->m_overrangeDetection = m_syncroPointers [i]->GetOverrangeDetection() ;
        pProperties->m_resolution = m_syncroPointers[i]->GetResolution();                               
        sheet.AddPage( pProperties ) ;        
        syncMap[m_syncroPointers[i]->GetName()] = pProperties;  
      }
    }
  }

    SetPropertyPageBuddies(syncMap);   

    for (int i = 0 ; i<m_sensorPointers.size() ; i++)
    //for (int i = (int)m_sensorPointers.size() - 1 ; 0 <= i ; i--)
    {
        SensorAlignmentPage *pProperties = new SensorAlignmentPage( sensorsOnly ) ;
        pProperties->GetPSP().pszTitle = m_sensorPointers [i]->GetName() ;
        pProperties->GetPSP().dwFlags |= PSP_USETITLE ;
        CString serialNumber = m_sensorPointers [i]->GetSerialNumber() ;
        pProperties->m_serialNumber = 0 == m_sensorPointers [i]->GetType() ? CString(_T("")) : serialNumber ; //Allow serialnumber 000, R2 050413
        //pProperties->m_serialNumber = "000" == serialNumber ? CString(_T("")) : serialNumber ;        
        pProperties->m_adapterNumberString = m_sensorPointers[i]->GetAdapterSerialNumber();
        pProperties->m_description = m_sensorPointers [i]->m_description ;
        pProperties->m_overrangeDetection = m_sensorPointers [i]->GetOverrangeDetection() ;
        pProperties->m_type = m_sensorPointers [i]->GetType() ;
        pProperties->m_nominalAzimuth = m_sensorPointers [i]->GetNominalAzimuth() ;
        sheet.AddPage( pProperties ) ;
    }

    BOOL chAEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlA_Enable;
    BOOL chBEnable = DAU::GetDAU().m_mainBoardRegsPro.m_serialCtrlB_Enable;

    int result = IDOK;

    if (IDOK == (result = sheet.DoModal()))
    {
        if (!sensorsOnly)
        {
            for (UINT i = 0 ; i < m_syncroPointers.size() ; i++)
            {
                Digital *pDigital ;
                if (( pDigital = dynamic_cast<Digital *>(m_syncroPointers [i]) ))
                {
                    DigitalProperties *pProperties = static_cast<DigitalProperties *>(sheet.GetPage( i )) ;
                    m_syncroPointers[i]->SetType( static_cast<UnitType::Types>((pProperties->m_DigChTypePro != DigChTypeProUnused) ? UnitType::Fixed : UnitType::Unused) ) ;
                    m_syncroPointers[i]->m_description = pProperties->m_description ;
                    m_syncroPointers[i]->SetNominalAzimuth( pProperties->m_nominalAzimuth ) ;
                    m_syncroPointers[i]->SetHighSeaComp(DAU::GetDAU().m_activeHighSeaGyro == pDigital->GetName()) ;
                }
                else
                {    
                    SyncroProperties *pProperties = static_cast<SyncroProperties *>(sheet.GetPage( i )) ;
                    m_syncroPointers[i]->SetType( static_cast<UnitType::Types>(pProperties->m_type) ) ;
                    m_syncroPointers[i]->SetGearing( pProperties->m_gearing ) ;
                    m_syncroPointers[i]->m_description = pProperties->m_description ;
                    m_syncroPointers[i]->SetNominalAzimuth( pProperties->m_nominalAzimuth ) ;
                    m_syncroPointers[i]->SetOverrangeDetection( pProperties->m_overrangeDetection ) ;
                    m_syncroPointers[i]->SetResolution(pProperties->m_resolution);                               
                    m_syncroPointers[i]->SetIsSelected( FALSE );
                    m_syncroPointers[i]->SetHighSeaComp(DAU::GetDAU().m_activeHighSeaGyro == m_syncroPointers[i]->GetName()) ;
                }
            }     
        }
        for (int i = 0; i<m_sensorPointers.size() ;i++)
        {
            SensorAlignmentPage *pProperties = static_cast<SensorAlignmentPage *>(sheet.GetPage( static_cast<int>((sensorsOnly ? i : m_syncroPointers.size()+i)))) ;
            m_sensorPointers[i]->SetSerialNumber( pProperties->m_serialNumber ) ;
            m_sensorPointers[i]->SetAdapterSerialNumber( pProperties->m_adapterNumberString ) ;
            m_sensorPointers[i]->m_description = pProperties->m_description ;
            m_sensorPointers[i]->SetOverrangeDetection(pProperties->m_overrangeDetection) ;
            m_sensorPointers[i]->SetType( static_cast<UnitType::Types>(pProperties->m_type) ) ;
            m_sensorPointers[i]->SetNominalAzimuth( pProperties->m_nominalAzimuth ) ;
            m_sensorPointers[i]->LoadCalibration() ;
            m_sensorPointers[i]->SetIsSelected( FALSE );
        }
    }
    else
    {
        //Restore interface status
         DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProA, chAEnable);
         DAU::GetDAU().m_comThr.EnableDigInterface(DigDauChProB, chBEnable);
    }

    while (0 < sheet.GetPageCount())
    {
        CPropertyPage *pProperties = sheet.GetPage( 0 ) ;
        sheet.RemovePage( pProperties ) ;
        delete pProperties ;
    }

    if( IDOK == result )
    {
        SetResolutionsToDAU();
        UpdateUARTProtocoll();   
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

BOOL DAU::StartWithTest( int testVoltage )
{
    SetDACVoltage(testVoltage);
    return TRUE;
}



BOOL DAU::Start()
{
    return TRUE;
}

//*******************************************************************************//
//***																	ProbePro																***//
//*** ----------------------------------------------------------------------- ***//
//***	- Called when changing project, AlignerDoc.cpp:CAlignerDoc::OnSetupSystem(), Dau.cpp:DAU::Open(), Probe() ***//
//***	- Probes the DAU for serial number																			***//
//***	- Makes an initial loop test																						***//
//*******************************************************************************//
BOOL DAU::ProbePro( void )
{

  return TRUE ;
}



//*******************************************************************************//
//***																	Probe	(old)															***//
//*** ----------------------------------------------------------------------- ***//
//***	- Called when changing project, AlignerDoc.cpp:CAlignerDoc::OnSetupSystem(), Dau.cpp:DAU::Open(), Probe() ***//
//***	- Probes the DAU for serial number																			***//
//***	- Makes an initial loop test																						***//
//*******************************************************************************//
BOOL DAU::Probe( void )
{
  
  return TRUE ;
}

SingleSensor::SingleSensor( double roll, double pitch, double unfilteredRoll, double unfilteredPitch, double uncalibratedRoll, double uncalibratedPitch, __int16 rawRoll, __int16 rawPitch, double temperature, __int16 rawTemperature, int stat) :
    m_roll(roll, unfilteredRoll, uncalibratedRoll, rawRoll), m_pitch(pitch, unfilteredPitch, uncalibratedPitch, rawPitch), m_temperature(temperature, rawTemperature, rawTemperature, rawTemperature), m_status(stat)    

//m_roll(roll), m_pitch(pitch), m_uncalibratedRoll(uncalibratedRoll), m_uncalibratedPitch(uncalibratedPitch), m_rawRoll(rawRoll), m_rawPitch(rawPitch), m_temperature(temperature), m_rawTemperature(rawTemperature)
{

} ;

SingleSensor::SingleSensor(SingleSensorData roll, SingleSensorData pitch, SingleSensorData heading, int stat):
    m_roll(roll), m_pitch(pitch), m_heading(heading), m_temperature(0.0f,0.0f,0.0f,0), m_status(stat)
{

}


DAUValues::DAUValues( DAU &source )
{
    for (int i = 0 ; i < source.GetSensorCount() ; i++)
    {
        Sensor *pSensor = source.GetSensor( i ) ;
        m_sensors.push_back( SingleSensor(
            pSensor->GetRoll(),
            pSensor->GetPitch(),
            pSensor->GetUnfilteredRoll(),
            pSensor->GetUnfilteredPitch(),
            pSensor->GetRoll( FALSE ),
            pSensor->GetPitch( FALSE ),
            pSensor->GetData().m_roll,
            pSensor->GetData().m_pitch,
            pSensor->GetTemperature(),
            pSensor->GetTemperatureData().m_temperature ) ) ;
    }
    for (int i = 0 ; i < source.GetSyncroCount() ; i++)
    {
        Syncro *pSyncro = source.GetSyncro( i ) ;
        m_syncros.push_back(SingleSensor(SingleSensorData(pSyncro->GetRoll(),pSyncro->GetUnfilteredRoll(),pSyncro->GetRoll( FALSE ), pSyncro->GetData().m_roll),
                                     SingleSensorData(pSyncro->GetPitch(),pSyncro->GetUnfilteredPitch(),pSyncro->GetPitch( FALSE ), pSyncro->GetData().m_pitch),
                                     SingleSensorData(pSyncro->GetHeading(),pSyncro->GetUnfilteredHeading(),pSyncro->GetHeading( FALSE ), pSyncro->GetData().m_heading),
                                                        pSyncro->GetStatus()));
//     m_syncros.push_back( SingleSensor(
//       pSyncro->GetRoll(),
//       pSyncro->GetPitch(),
//       pSyncro->GetRoll( FALSE ),
//       pSyncro->GetPitch( FALSE ),
//       pSyncro->GetData().m_roll,
//       pSyncro->GetData().m_pitch ) ) ;
    }
}

DAUValues::DAUValues( void )
{
  // Empty.
}

DAUValues::DAUValues( const DAUValues &source )
{
  m_sensors.insert( m_sensors.begin(), source.m_sensors.begin(), source.m_sensors.end() ) ;
  m_syncros.insert( m_syncros.begin(), source.m_syncros.begin(), source.m_syncros.end() ) ;
}

void DAUValues::Set( const DAUValues &source )
{
  m_sensors.clear() ;
  m_syncros.clear() ;
  m_sensors.insert( m_sensors.begin(), source.m_sensors.begin(), source.m_sensors.end() ) ;
  m_syncros.insert( m_syncros.begin(), source.m_syncros.begin(), source.m_syncros.end() ) ;
}

class Compensate
{
	double m_roll;
	double m_pitch;
public:    
	Compensate(double r, double p) : m_roll(r),m_pitch(p){}
	void operator()(SingleSensor& elem )
	{        
		elem.Compensate(m_roll, m_pitch);
	}
};

void DAUValues::CompensateSensorData(double roll, double pitch)
{
	for_each(m_sensors.begin(), m_sensors.end(), Compensate(roll, pitch));
}

void DAU::SetSelected( BOOL selected )
{	
	for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
	{
		(*i)->SetIsSelected(selected) ;
		(*i)->SetType(UnitType::Unused) ;
	}
	for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
	{
		(*i)->SetIsSelected(selected) ;
		Digital *pDigital ;
		if(pDigital = dynamic_cast<Digital *>(*i) )
		{
			pDigital->SetSyncroType(DigChTypeProUnused) ;
		}
		(*i)->SetType(UnitType::Unused) ;
	}
}

void DAU::SetOverrangeDetection(BOOL flag)
{	
	for (vector<Sensor *>::iterator i = m_sensorPointers.begin(); i != m_sensorPointers.end(); i++)
	{
		(*i)->SetOverrangeDetection(flag);
	}
	for (vector<Syncro *>::iterator i = m_syncroPointers.begin(); i != m_syncroPointers.end(); i++)
	{
		(*i)->SetOverrangeDetection(flag);
	}
}


void DAU::SetOverrangeDetectionActive( BOOL flag )
{
    BOOL overangeDetectionActive = flag;
    for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
    {
        (*i)->SetOverrangeDetectionActive( flag ) ;
    }
    for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
    {
        (*i)->SetOverrangeDetectionActive( flag ) ;
    }
}

int DAU::GetProtocolVersion( void )
{
  return m_protocolVersion ;
}

void DAU::SetIgnoreElevation( BOOL ignoreElevation )
{
    for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
    {
        (*i)->SetIgnoreElevation( ignoreElevation ) ;
    }
}

void DAU::Log2File(short val, CString file)
{
    std::ofstream out(file, std::ios_base::out | std::ios_base::app);
    out << val << std::endl;
    out.flush();
    out.close();

}

Syncro *DAU::GetFirstSelectedSyncro()
{
    vector<Syncro *>::iterator iter;
    for(iter = m_syncroPointers.begin();iter!=m_syncroPointers.end();iter++)
    {
        if((*iter)->IsSelected())
            return *iter;
    }

    return NULL;
}


void DAU::UpdateUARTProtocoll()
{
    Digital* pD1 = (Digital*)GetSyncro(D1);
    if(!pD1)
        pD1 = (Digital*)GetSyncro(CH3);
    
    Digital* pD2 = (Digital*)GetSyncro(D2);
    if(!pD2)
        pD2 = (Digital*)GetSyncro(CH4);


    DigChTypePro UART1Type = (pD1!=NULL) ? pD1->m_syncroType : DigChTypeProUnused;
    DigChTypePro UART2Type = (pD2!=NULL) ? pD2->m_syncroType : DigChTypeProUnused;
    
    //Note: UARTA -> D2, UARTB -> D1
    m_comThr.SetUARTType(FRAME_TYPE_UART_A, UART2Type);
    m_comThr.SetUARTType(FRAME_TYPE_UART_B, UART1Type);

}

void DAU::DumpSensorStates()
{
	Registry reg;
	CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");

	CString dumpFile;
	dumpFile.Format("%s\\SensorDump.txt", dir);

	CString timeStr;
	SYSTEMTIME time;
	GetLocalTime(&time);
	timeStr.Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);


	//sort(m_dacResults.begin(), m_dacResults.end());
	ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);

	dump << timeStr << endl;

	for (auto iter = m_sensorPointers.begin(); iter != m_sensorPointers.end(); iter++)
	{
		(*iter)->Dump(dump);
	}
	dump << endl << endl;

	dump.flush();
	dump.close();

	


	//m_sensorPointers


}


//*******************************************************************************//
//***																	NewData   															***//
//*** ----------------------------------------------------------------------- ***//
//***																																					***//
//*******************************************************************************//
/*void DAU::NewData( void )
{
    BOOL outOfRange = FALSE ;
    CString offendingSensor ;

    if (theApp.IsAligner202Enabled())
    {
        for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
        {
            Digital *pDigital ;
            if (( pDigital = dynamic_cast<Digital *>(*i) ))
            {
                //(*i)->GetName()
                // TODO: This will assume that a 'Digital' gyro has an offset!
                (*i)->SetData( m_data.m_syncro [(*i)->GetOffset()] ) ;
            }
        }

        Syncro *pA1 = GetSyncro( CString(_T("A1")) ) ;
        Syncro *pB1 = GetSyncro( CString(_T("B1")) ) ;
        Syncro *pC1 = GetSyncro( CString(_T("C1")) ) ;
        Syncro *pA2 = GetSyncro( CString(_T("A2")) ) ;
        Syncro *pB2 = GetSyncro( CString(_T("B2")) ) ;
        Syncro *pC2 = GetSyncro( CString(_T("C2")) ) ;

        if( (pA1 != NULL) && (pB1 != NULL) && (pC1 != NULL) && (pA2 != NULL) && (pB2 != NULL) && (pC2 != NULL) )
        {
            SyncroData data ;
            data = m_data.m_syncro [0] ;
            data.m_pitch = data.m_roll ; // J1 roll = A1
            pA1->SetData( data ) ;

            data = m_data.m_syncro [0] ;
            data.m_roll = data.m_pitch ; // B1 = J1 pitch
            pB1->SetData( data ) ;

            data = m_data.m_syncro [1] ;
            data.m_pitch = data.m_roll ;
            pA2->SetData( data ) ;

            data = m_data.m_syncro [1] ;
            data.m_roll = data.m_pitch ;
            pB2->SetData( data ) ;

            if (1 == pA1->GetType() && 2 == pB1->GetType())
            {
                double composite ;
                GetCompositeAngle( pA1->GetRoll(), pB1->GetRoll(), pB1->GetGearing(), &composite ) ;
                pC1->SetData( composite ) ;
            }
            else if (1 == pB1->GetType() && 2 == pA1->GetType())
            {
                double composite ;
                GetCompositeAngle( pB1->GetRoll(), pA1->GetRoll(), pA1->GetGearing(), &composite ) ;
                pC1->SetData( composite ) ;
            }
            else 
            {
                pC1->SetData( 0 ) ;
            }

            if (1 == pA2->GetType() && 2 == pB2->GetType())
            {
                double composite ;
                GetCompositeAngle( pA2->GetRoll(), pB2->GetRoll(), pB2->GetGearing(), &composite ) ;
                pC2->SetData( composite ) ;
            }
            else if (1 == pB2->GetType() && 2 == pA2->GetType())
            {
                double composite ;
                GetCompositeAngle( pB2->GetRoll(), pA2->GetRoll(), pA2->GetGearing(), &composite ) ;
                pC2->SetData( composite ) ;
            }
            else 
            {
                pC2->SetData( 0 ) ;
            }
        }
    }
    else // this is 308
    {
        for (vector<Syncro *>::iterator i = m_syncroPointers.begin() ; i != m_syncroPointers.end() ; i++)
        {
            // TODO: This will assume that a 'Digital' gyro has an offset!
            if (!((*i)->SetData( m_data.m_syncro [(*i)->GetOffset()] )) && Sensor::Unused != (*i)->GetType())
            {
                //if( (*i)->IsSelected() || (*i)->GetOverrangeDetectionActive() )
                if( (*i)->GetOverrangeDetectionActive() )
                {
                    offendingSensor = (*i)->GetName() ;
                    outOfRange = TRUE ;
                }
            }
        }

        for (vector<Sensor *>::iterator i = m_sensorPointers.begin() ; i != m_sensorPointers.end() ; i++)
        {
            if (!((*i)->SetData( m_data.m_sensor [(*i)->GetOffset()] )) && Sensor::Unused != (*i) ->GetType())
            {
                //if( (*i)->IsSelected() || (*i)->GetOverrangeDetectionActive() )
                if( (*i)->GetOverrangeDetectionActive() )
                {
                    offendingSensor = (*i)->GetName() ;
                    outOfRange = TRUE ;
                }
            }
        }
    }

    if (0 != m_protocolVersion)
    {
        switch (m_data.m_mainBoard.m_temperatureChannel)
        {
        case 6:

            m_mainBoard.SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
            break ;

        case 7:
            if (0 < m_sensorPointers.size())
            {
                m_sensorPointers [0]->SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
            }
            break ;
        default:
            if ((unsigned int)(1 + m_data.m_mainBoard.m_temperatureChannel) < m_sensorPointers.size())
            {
                m_sensorPointers [1 + m_data.m_mainBoard.m_temperatureChannel]->SetTemperatureData( TemperatureData( m_data.m_temperature ) ) ;
            }
            break ;
        }
    }
    else
    {
        int sensor = m_data.m_temperature.m_old [1] & 7 ;
        if (sensor < m_sensorPointers.size())
        {,
            m_sensorPointers [sensor]->SetTemperatureData( TemperatureData( static_cast<__int16>(m_data.m_temperature.m_old [0] * 263.9358 ) ) ) ;,
        },
    },
    m_mainBoard.SetData( m_data.m_mainBoard ) ;,
    NotifyListeners() ;,

    if ( (m_errorEvent || outOfRange) && ( TRUE == IsErrorEventEnabled() ) )
    {
        m_errorEventFlag = m_errorEvent;
        m_errorEvent = FALSE ;
        CSingleLock _(&m_listenersLock, TRUE) ;
        try
        {
            Stop();
            __raise ErrorEvent( outOfRange, offendingSensor ) ;
        }
        catch (...)
        {
        }
        EnableErrorEvent( FALSE ) ;
    }
}

*/
