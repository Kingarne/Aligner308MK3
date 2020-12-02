#pragma once

#include <gdiplus.h>
#include "afxwin.h"
using namespace Gdiplus;

class SerialPort;
// DAUDlg dialog

#define TYPE_SIGMA40_ID01  0 
#define TYPE_SIGMA40_ID03  1
#define TYPE_SIGMA40_NMEA  2
#define TYPE_SPERRY_MK39M3 3

#pragma pack(push , 1)
struct Sigma40Id01Data
{
    unsigned short header;
    unsigned char numData;
    unsigned char Id;
    unsigned char status1;
    unsigned char status2;
    unsigned short heading;
    unsigned short roll;
    unsigned short pitch;
    unsigned char pad[18];
	unsigned char crc;
	unsigned char term;
};

struct Sigma40Id03Data
{
    unsigned short header;
    unsigned char numData;
    unsigned char Id;
    unsigned char status1;
    unsigned char status2;
    unsigned char GPSTime[3];
    unsigned short heading;
    unsigned short roll;
    unsigned short pitch;
    unsigned char pad[16];
	unsigned char crc;
	unsigned char term;
};

struct SperryMK39M3Data
{
	unsigned short header;
	unsigned char numData;
	unsigned char Id;
	unsigned char status1;
	unsigned char status2;
	unsigned short heading;
	unsigned short roll;
	unsigned short pitch;
	unsigned char pad[18];
	unsigned char crc;
	unsigned char term;
};

#pragma pack(pop)

class DAUDlg : public CDialog
{
	DECLARE_DYNAMIC(DAUDlg)

public:
	DAUDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DAUDlg();

// Dialog Data
	enum { IDD = IDD_DAU_DIALOG };

    SerialPort* m_pSerialPort ;
    void Send();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    Sigma40Id01Data m_sigma40id01Data;
    Sigma40Id03Data m_sigma40id03Data;
	SperryMK39M3Data m_sperryData;

    double CalcAngle(CRect& m_rollRect, CPoint& point);
    void CheckPoint(CPoint& point);
    unsigned short ToProtocolFormat(double deg);
    
    LARGE_INTEGER m_frequence;
    LARGE_INTEGER m_lastTime;

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedSendButton();
	afx_msg void OnBnClickeCRCErrButton();

        
    void UpdateValues();
    void SendDauData();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    
    void Draw(Graphics &gr);
    
    CRect m_rollRect;
    CRect m_pitchRect;
    
    short m_roll;
    short m_pitch;
    double m_rollAng;
    double m_pitchAng;
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    short m_protRoll;
    short m_protPitch;
	short m_heading;
	afx_msg void OnBnClickedPitchAuto();
    afx_msg void OnBnClickedRollAuto();
    BOOL m_autoRoll;
    BOOL m_autoPitch;
    int m_timer;
    BOOL m_bInduceCRCErr;

    afx_msg void OnTimer(UINT nIDEvent);
    
    CString m_maxRollAngStr;
    CString m_rollAngStepStr;
    double m_maxRollAng;
    double m_rollAngStep;

    CString m_maxPitchAngStr;
    CString m_pitchAngStepStr;
    double m_maxPitchAng;
    double m_pitchAngStep;

    CString m_rollCenterStr;
    CString m_pitchCenterStr;
    double m_rollCenter;
    double m_pitchCenter;

    CString m_rollNoiceStr;
    CString m_pitchNoiceStr;
    double m_pitchNoice;    
    double m_rollNoice;    

    int m_type;

    afx_msg void OnEnKillfocusMaxRollAng();
    afx_msg void OnEnKillfocusRollStep();
    afx_msg void OnEnKillfocusMaxPitchAng();
    afx_msg void OnEnKillfocusPitchStep();
    afx_msg void OnEnChangeMaxRollAng();
    afx_msg void OnEnChangeRollStep();
    afx_msg void OnEnChangeMaxPitchAng();
    afx_msg void OnEnChangePitchStep();
    CComboBox m_typeCombo;
    afx_msg void OnCbnSelchangeCombo1();
    
    afx_msg void OnEnKillfocusRollCenter();
    afx_msg void OnEnChangeRollCenter();
    afx_msg void OnEnKillfocusPitchCenter();
    afx_msg void OnEnChangePitchCenter();
    BOOL m_animate;
    
    afx_msg void OnEnChangeNoicePitch();
    afx_msg void OnEnKillfocusNoicePitch();
    afx_msg void OnEnChangeNoiceRoll();
    afx_msg void OnEnKillfocusNoiceRoll();
    afx_msg void OnBnClickedAnim();
	CString m_headingStr;
	afx_msg void OnEnKillfocusHeading();
};
