// $Id: GunAdapterTiltCheckDlg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "GraphView.h"

// CGunAdapterTiltCheckDlg dialog

class CGunAdapterTiltCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CGunAdapterTiltCheckDlg)

private:
  CString m_Text;
  CString m_TimeStamp;
  CString m_DateStamp;
  CGraphView *m_pGraph;
  BOOL m_ShowGraphPressed;
  BarGraphInParam m_BarGraphParam;
	double m_Tao;
	unsigned int m_freqMode;
  CComboBox m_azCombo;

public:
	CGunAdapterTiltCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGunAdapterTiltCheckDlg();

// Dialog Data
	enum { IDD = IDD_GUN_ADAPTER_TILT_CHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  BOOL OnInitDialog();
  void Destroy();
  BOOL InitGraph();
	void ShowSettingsForAxes();
	void ShowSetup();
  void SetAz(double az);

	DECLARE_MESSAGE_MAP()
private:
  afx_msg void OnBnClickedShowGraph();
  afx_msg void OnBnClickedSetupMeasurement();
	afx_msg void OnBnClickedSetupAxes();
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
protected:
  virtual void OnOK();
  virtual void OnCancel();
public:
	
  afx_msg void OnCbnSelchangeAzCombo();
  afx_msg void OnEnKillfocusKEdit();
};
