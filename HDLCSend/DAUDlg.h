#pragma once

#include <gdiplus.h>
#include "afxwin.h"
using namespace Gdiplus;

class CHDLCSendDlg;
// DAUDlg dialog

#define TYPE_MINS 0


class DAUDlg : public CDialog
{
	DECLARE_DYNAMIC(DAUDlg)

public:
	DAUDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DAUDlg();

// Dialog Data
	enum { IDD = IDD_DAU_DIALOG };

    CHDLCSendDlg* m_pParent ;
    void Send();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    double CalcAngle(CRect& m_rollRect, CPoint& point);
    void CheckPoint(CPoint& point);
    unsigned short ToProtocolFormat(double deg);

    LARGE_INTEGER m_frequence;
    LARGE_INTEGER m_lastTime;

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedSendButton();

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

    BOOL m_bInduceCRCErr;

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
};
