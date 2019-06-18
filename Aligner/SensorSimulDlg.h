#pragma once

#pragma once


#include "afxwin.h"
#include <gdiplus.h>
//using namespace Gdiplus;

class SerialPort;
// SensorSimulDlg dialog

#define TYPE_SIGMA40_ID01 0
#define TYPE_SIGMA40_ID03 1
#define TYPE_SENSORX      2
#define TYPE_LUCAS        3

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
    unsigned char pad[20];
};


class SensorSimulDlg : public CDialog
{
    DECLARE_DYNAMIC(SensorSimulDlg)

public:
    SensorSimulDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~SensorSimulDlg();

    // Dialog Data
    enum { IDD = IDD_SENSOR_SIMUL_DLG };

    void Send();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

    Sigma40Id01Data m_sigma40id01Data;

    double CalcAngle(CRect& m_rollRect, CPoint& point);
    void CheckPoint(CPoint& point);
    unsigned short ToProtocolFormat(double deg);
    unsigned short ToSensorXFormat(double deg);

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedSendButton();

    void SendDauData();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();

    void Draw(Gdiplus::Graphics &gr);
    void Update();

    CString m_id;
    CRect m_rollRect;
    CRect m_pitchRect;

    short m_roll;
    short m_pitch;
    double m_rollAng;
    double m_pitchAng;
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    short m_protRoll;
    short m_protPitch;
    afx_msg void OnBnClickedPitchAuto();
    afx_msg void OnBnClickedRollAuto();
    BOOL m_autoRoll;
    BOOL m_autoPitch;
    int m_timer;

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
    CComboBox m_typeCombo;
    afx_msg void OnCbnSelchangeTypeCombo();
};

