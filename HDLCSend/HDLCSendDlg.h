// HDLCSendDlg.h : header file
//

#pragma once

#include "QuaDef.h"
#include "QuaDLL.h"
#include "afxwin.h"
#include "DAUDlg.h"
#include "HFSender.h"

struct FrameData 
{
    //struct
 //   {
        unsigned char Address;					// Don't care
        unsigned char MsgID;
//        unsigned char status;
        
        unsigned char DRU_Malfunction	:1;		// => Error : DRU malfunction
        unsigned char Simulation		:1;		// => Warning : Simulation data
        unsigned char Spare				:1;		// Don't care
        unsigned char EM_Log_Available	:1;		// Don't care
        unsigned char GPS_Available		:1;		// Don't care
        unsigned char AttitudeValid		:1;		// Don't care
        unsigned char Navigation		:1;		// Don't care
        unsigned char Align				:1;		// Don't care	OBS! MSb !!
        long HeadingAngle;				// Don't care
        long RollAngle;
        long PitchAngle;
        short HeadingAngleRate;			// Don't care
        short RollAngleRate;			// Don't care
        short PitchAngleRate;			// Don't care
 //   };
    //char FrameDataPad [44] ;		// Pad
};


// CHDLCSendDlg dialog
class CHDLCSendDlg : public CDialog
{
// Construction
public:
	CHDLCSendDlg(CWnd* pParent = NULL);	// standard constructor
    
// Dialog Data
	enum { IDD = IDD_HDLCSEND_DIALOG };

    int	TransmitData(double Roll, double Pitch);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    short Transmit(unsigned char *frame, int len);
   
    void SyncDriveExit(void);
    int InitSyncDriver();
    void DumpHDLCFrame(FrameData *frameData);
    void DumpHDLCFrame(UCHAR *frameData, int size);
    void CopyFrameData(FrameData *frameData, UCHAR* buf);

    MPA_CONFIG m_config;
    MPA_HANDLE m_drvHandle;
    MPA_DRIVER_VERSION m_ver;

    int m_timer;
    int m_seqNum;
    unsigned char *m_frameBuf;
    BOOL m_dumpData;
    BYTE m_test;

    DAUDlg m_dauDlg;
    HFSender m_HFSender;

public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    CComboBox m_frameRateCombo;
    int m_numSentFrames;
    CString m_info;
    afx_msg void OnCbnSelchangeFrameRateComb();
    afx_msg void OnBnClickedSendOneButton();
    
    void Callback(LPARAM lp);
    CString m_rollStr;
    CString m_pitchStr;
    BOOL m_cycle;
    afx_msg void OnBnClickedDauButton();
    afx_msg void OnBnClickedDumpCheck();    
    afx_msg void OnEnKillfocusTest();
    afx_msg void OnBnClickedCycle();

};
