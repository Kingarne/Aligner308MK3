// $Id: PortSetup.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

/**
 *  @brief PortSetup handles COM port setup issues.
 *
 *  PortSetup is a static class acting namespace for application wide setup issues.
 *  PortSetup::Data defines information that is required when opening communication
 *  to external measurement units.
 *
 *  PortSetup should never be instanciated, the private constructor ensures this. All
 *  usage of PortSetup is through the static methods declared below.
 */
class PortSetup
{
private:
  PortSetup( void ) {} ;
public:
  /**
   *  @brief Encapsulation of COM port setup data.
   *
   *  PortSetup::Data encapsulates dialog variables - This is done so that setup of
   *  dialog/data can be done with a single assignment.
   */
  class Data {
  public:
    Data( void ) : m_dauPort(), m_d1Port(), m_d2Port(), m_d3PortA202(), m_d4PortA202() {
      // Empty
    } ;
    Data( Data &source) {
      m_dauPort = source.m_dauPort ;
      m_d1Port = source.m_d1Port ;
      m_d2Port = source.m_d2Port ;
      m_d3PortA202 = source.m_d3PortA202 ;
      m_d4PortA202 = source.m_d4PortA202 ;
    } ;
  public:
    CString m_dauPort ;
    CString m_d1Port ;
    CString m_d2Port ;
    CString m_d3PortA202 ;
    CString m_d4PortA202 ;
  } ;
public:
  static void Init( void ) ;
  static void Exit( void ) ;
  static INT_PTR DoModal( void ) ;
  static CString GetDAUPort( void ) ;
  static CString GetD1Port( void ) ;
  static CString GetD2Port( void ) ;
  static CString GetD3PortA202( void ) ;
  static CString GetD4PortA202( void ) ;
private:
  static Data m_data ;
} ;

/**
 *  @brief Wrapper to handle the COM port setup dialog.
 *
 *  PortSetupDialog is a (mostly) wizard generated class to handle interaction
 *  with the COM Port Setup menu accessed via the main application menu.
 */
class PortSetupDialog : public CDialog
{
	DECLARE_DYNAMIC(PortSetupDialog)
	DECLARE_MESSAGE_MAP()
public:
	PortSetupDialog( UINT DialogID, CWnd *pParent = NULL ) ;
	virtual ~PortSetupDialog( void ) ;
public:
	//enum { IDD = IDD_PORT_SETUP } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
public:
  virtual BOOL OnInitDialog( void ) ;
private:
  void VerifyOK( void ) ;
public:
  PortSetup::Data m_data ;
private:
  afx_msg void OnBnClickedDauChange( void ) ;
  afx_msg void OnBnClickedD1Change( void ) ;
  afx_msg void OnBnClickedD2Change( void ) ;
  afx_msg void OnBnClickedD3ChangeA202( void ) ;
  afx_msg void OnBnClickedD4ChangeA202( void ) ;
  afx_msg void OnCbnSelchangeDauComPort( void ) ;
  afx_msg void OnCbnSelchangeD1ComPort( void ) ;
  afx_msg void OnCbnSelchangeD2ComPort( void ) ;
  afx_msg void OnCbnSelchangeD3ComPortA202( void ) ;
  afx_msg void OnCbnSelchangeD4ComPortA202( void ) ;
private:
  BOOL SetupDAUPort( void ) ;
  BOOL SetupD1Port( void ) ;
  BOOL SetupD2Port( void ) ;
  BOOL SetupD3PortA202( void ) ;
  BOOL SetupD4PortA202( void ) ;
private:
  CComboBox m_dauPortControl ;
  CComboBox m_d1PortControl ;
  CComboBox m_d2PortControl ;
  CComboBox m_d3PortControlA202 ;
  CComboBox m_d4PortControlA202 ;
  CButton m_okControl;
} ;

//*******************************************************************************//
//***												PortAsyncSetupDialogPro		A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***																					***//
//*******************************************************************************//
class PortUartHdlcSetupDialogPro : public CDialog
{
	DECLARE_DYNAMIC(PortUartHdlcSetupDialogPro)
	DECLARE_MESSAGE_MAP()
public:
	PortUartHdlcSetupDialogPro( UINT DialogID, CWnd *pParent = NULL ) ;
	virtual ~PortUartHdlcSetupDialogPro( void ) ;
public:
	//enum { IDD = IDD_PORT_SETUP } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
public:
  virtual BOOL OnInitDialog( void ) ;
//private:
//  void VerifyOK( void ) ;
//public:
//  PortSetup::Data m_data ;
//  void ChangePortParamsD1( void ) ;																//	A308Pro
private:
//  afx_msg void OnBnClickedDauChange( void ) ;
  afx_msg void OnBnClickedD1Change( void ) ;
//  afx_msg void OnBnClickedD2Change( void ) ;
//  afx_msg void OnBnClickedD3ChangeA202( void ) ;
//  afx_msg void OnBnClickedD4ChangeA202( void ) ;
//  afx_msg void OnCbnSelchangeDauComPort( void ) ;
//  afx_msg void OnCbnSelchangeD1ComPort( void ) ;
//  afx_msg void OnCbnSelchangeD2ComPort( void ) ;
//  afx_msg void OnCbnSelchangeD3ComPortA202( void ) ;
//  afx_msg void OnCbnSelchangeD4ComPortA202( void ) ;
//	afx_msg void OnWmShowWindowPortAsyncSetupPro( BOOL, UINT );
	afx_msg void OnCbnSelChangeAsyncBps( void ) ;
	afx_msg void OnCbnSelChangeAsyncDataBits( void ) ;
	afx_msg void OnCbnSelChangeAsyncParity( void ) ;
//private:
//  BOOL SetupDAUPort( void ) ;
//  BOOL SetupD1Port( void ) ;
//  BOOL SetupD2Port( void ) ;
//  BOOL SetupD3PortA202( void ) ;
//  BOOL SetupD4PortA202( void ) ;
private:
  //CComboBox m_dauPortControl ;
  //CComboBox m_d1PortControl ;
  //CComboBox m_d2PortControl ;
  //CComboBox m_d3PortControlA202 ;
  //CComboBox m_d4PortControlA202 ;
  //CButton m_okControl;
  CComboBox m_d1PortControl ;
  CComboBox m_asyncBitsPerSec ;																		//	A308Pro
  CComboBox m_asyncDataBits ;																			//	A308Pro
  CComboBox m_asyncParity ;																				//	A308Pro
  //CString m_asyncBitsPerSec ;																		//	A308Pro
  //CString m_asyncDataBits ;																			//	A308Pro
  //CString m_asyncParity ;																				//	A308Pro

} ;
