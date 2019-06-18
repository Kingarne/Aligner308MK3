// CamCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialSend.h"
#include "CamCtrlDlg.h"
#include "afxdialogex.h"
#include "SerialPort.h"

// CamCtrlDlg dialog


UINT COMReadThread::ThreadFunction()
{
	if(!m_pSerialPort)	
		return -1;

	TRACE("Thread func\n");
	m_CreateEvent.SetEvent();	

	const int dataSize = 100;
	unsigned char data[dataSize];
	COMMTIMEOUTS to;
	m_pSerialPort->GetTimeout(to);
	to.ReadIntervalTimeout = 10;
	to.ReadTotalTimeoutConstant = 0;
	to.ReadTotalTimeoutMultiplier = 10;
	m_pSerialPort->SetTimeout(to);

	do 
	{

		int dataRead = m_pSerialPort->Read((char*)data, dataSize);
		TRACE("Read %d\n",dataRead);
		if(dataRead)
		{
			if(m_pCallback)
			{
				(*m_pCallback)(m_pCallbackData, (WPARAM)data, dataRead);
			}
		}
	
	} 
	while (!m_done);



	return 0;
}

void COMReadThread::Exit()
{
	m_done = true;
}

IMPLEMENT_DYNAMIC(CamCtrlDlg, CDialog)

CamCtrlDlg::CamCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CamCtrlDlg::IDD, pParent)
{

}

CamCtrlDlg::~CamCtrlDlg()
{
}

void CamCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_INFO, m_comInfo);
}


BEGIN_MESSAGE_MAP(CamCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_ACK_BUTTON, &CamCtrlDlg::OnBnClickedAckButton)
END_MESSAGE_MAP()


// CamCtrlDlg message handlers
void CamCtrlDlg::StartReadThread()
{
	m_readThread.Create();
}


CString CamCtrlDlg::Data2Str(char* pData, int dataLen)
{
	CString str,tmp;
	
	for(int i=0;i<dataLen;i++)
	{
		tmp.Format("%x, ", pData[i]);
		str += tmp;
	}

	str.TrimRight(", ");

	return str;
}


void CamCtrlDlg::OnBnClickedAckButton()
{
	BYTE data[10];
	data[0] = 48;
	data[1] = 49;
	data[2] = 50;
	data[3] = 51;
	int dataLen = 4;
	
	int wrtitten = m_pSerialPort->Write((char*)data, dataLen);
	TRACE("Bytes %d\n",wrtitten);

	
	CString dataStr = Data2Str((char*)data, dataLen);
	CString msg = "Send: " + dataStr;
	//msg.Format("Test");
	StatusLogger::GetInstance()->AddMessage(LogDebug, msg);
}

void CamCtrlDlg::ReadFunction(WPARAM wp, LPARAM lp)
{
	int dataSize = lp;
	if(dataSize > 100)
		return;

	char* pData = (char*)wp;
	CString msg = "Read: ",tmp;
	for(int i=0;i<dataSize;i++)
	{
		tmp.Format("0x%02x, ",pData[i]);
		msg += tmp;		
	}
	
	msg.TrimRight(", ");
	
	StatusLogger::GetInstance()->AddMessage(LogDebug, msg);

}

BOOL CamCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_readThread.m_pSerialPort = m_pSerialPort;
	m_readThread.SetCallback(StaticReadFunction, this);

	m_comInfo.Setup(true, LogAll);	
	StatusLogger::GetInstance()->SetHWnd(m_comInfo.m_hWnd);
	
	CString msg;
	msg.Format("Test");
	StatusLogger::GetInstance()->AddMessage(LogDebug, msg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
