#pragma once
#include "WorkerThread.h"
#include "afxcmn.h"
#include "StatusCtrl.h"

class SerialPort;
// CamCtrlDlg dialog

class COMReadThread : public WorkerThread
{
public:
	COMReadThread():WorkerThread(), m_pSerialPort(NULL){}
	~COMReadThread(){}

	SerialPort* m_pSerialPort;

	virtual void Exit();
protected:

	virtual UINT ThreadFunction();

};


class CamCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CamCtrlDlg)

public:
	CamCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CamCtrlDlg();

// Dialog Data
	enum { IDD = IDD_CAM_CTRL_DIALOG };

	SerialPort* m_pSerialPort ;
	void StartReadThread();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	COMReadThread m_readThread;

	static void StaticReadFunction(PVOID pData, WPARAM wp, LPARAM lp)
	{
		return ((CamCtrlDlg*)pData)->ReadFunction(wp, lp);
	}
	void ReadFunction(WPARAM wp, LPARAM lp);
	CString Data2Str(char* pData, int dataLen);

public:
	afx_msg void OnBnClickedAckButton();
	virtual BOOL OnInitDialog();
	StatusCtrl m_comInfo;
};
