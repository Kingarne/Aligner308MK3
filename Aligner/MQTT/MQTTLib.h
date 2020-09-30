#pragma once
#include <ctime>
#include "MQTTSignals.h"

typedef void(*MsgCallback)(void* pC, char* msg, char* top);

class MQTTLib
{
public:
	MQTTLib();
	~MQTTLib();

	BOOL LoadDll();

	int Init(char* clientId, char* brokerAddr, int port);
	void MQTTLoop(int interval);
	int SendMsg(char* msg, char* topic);
	int Subscribe(char*, int qos);
	void SetCB(MsgCallback cb, void* pC);
	bool IsOpen() { return m_hMod != NULL; }

protected:

	std::clock_t m_timeTag;

	BOOL MapFunctions();
	HMODULE m_hMod;

	typedef int (WINAPIV *PtrToInit)(char* clientId, char* brokerAddr, int port);
	PtrToInit m_pInit;

	typedef void (WINAPIV *PtrToLoop)();
	PtrToLoop m_pLoop;

	typedef int (WINAPIV *PtrToSendMsg)(char* msg, char* topic);
	PtrToSendMsg m_pSend;

	typedef int (WINAPIV *PtrToSubscr)(char* topic, int qos);
	PtrToSubscr m_pSubScr;

	typedef void (WINAPIV *PtrToSetCB)(MsgCallback cb, void* pC);
	PtrToSetCB m_pSetCB;

};

