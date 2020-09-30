#include "stdafx.h"
#include "MQTTLib.h"


MQTTLib::MQTTLib():m_hMod(NULL), m_timeTag(0)
{
}


MQTTLib::~MQTTLib()
{
	if (m_hMod != NULL)
	{
		FreeLibrary(m_hMod);
		m_hMod = NULL;
	}
}


BOOL MQTTLib::LoadDll()
{
	if (m_hMod != NULL)
		return TRUE;

	m_hMod = LoadLibrary("MQTTLib.dll");
	if (m_hMod == NULL)
	{
		AfxMessageBox("Error: Can't Load MQTTLib.dll");
		return FALSE;
	}
	MapFunctions();

	TRACE("Loaded MQTTLib.dll\n");
	return TRUE;	
}

BOOL MQTTLib::MapFunctions()
{
	m_pInit = (PtrToInit)GetProcAddress(m_hMod, "MQTTInit");
	if (m_pInit == NULL)
	{
		AfxMessageBox("Error: Can't Find MQTTInit");
		return FALSE;
	}

	m_pLoop = (PtrToLoop)GetProcAddress(m_hMod, "MQTTLoop");
	if (m_pLoop == NULL)
	{
		AfxMessageBox("Error: Can't Find MQTTLoop");
		return FALSE;
	}

	m_pSend = (PtrToSendMsg)GetProcAddress(m_hMod, "MQTTSendMsg");
	if (m_pSend == NULL)
	{
		AfxMessageBox("Error: Can't Find MQTTSendMsg");
		return FALSE;
	}

	m_pSubScr = (PtrToSubscr)GetProcAddress(m_hMod, "MQTTSubscribe");
	if (m_pSubScr == NULL)
	{
		AfxMessageBox("Error: Can't Find MQTTSubscribe");
		return FALSE;
	}

	m_pSetCB = (PtrToSetCB)GetProcAddress(m_hMod, "MQTTSetCB");
	if (m_pSetCB == NULL)
	{
		AfxMessageBox("Error: Can't Find MQTTSetCB");
		return FALSE;
	}
	return TRUE;
}


int MQTTLib::Init(char* clientId, char* brokerAddr, int port)
{
	if (!m_pInit)
	{
		AfxMessageBox("Init is not valid!");
		return 0;
	}

	return (*m_pInit)(clientId, brokerAddr, port);
}

void MQTTLib::MQTTLoop(int interval)
{
	int duration = (std::clock() - m_timeTag);
	if (duration < interval && interval != 0)
		return;

	m_timeTag = std::clock();

	if (!m_pLoop)
	{
		AfxMessageBox("Loop is not valid!");
		return;
	}

	(*m_pLoop)();
}

int MQTTLib::SendMsg(char* msg, char *topic)
{
	if (!m_pSend)
	{
		AfxMessageBox("Send is not valid!");
		return 0;
	}

	return (*m_pSend)(msg, topic);
}

int MQTTLib::Subscribe(char* topic, int qos)
{
	if (!m_pSubScr)
	{
		AfxMessageBox("Subscr is not valid!");
		return 0;
	}

	return (*m_pSubScr)(topic, qos);
}

void MQTTLib::SetCB(MsgCallback cb, void* pC)
{
	if (!m_pSetCB)
	{
		AfxMessageBox("SetCB is not valid!");
		return;
	}

	(*m_pSetCB)(cb, pC);
}
