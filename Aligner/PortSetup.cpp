// $Id: PortSetup.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "PortSetup.h"
#include "SerialPort.h"
#include "Aligner.h"

#define UNUSED_STRING _T("Unused") // Kludge: add to resources

PortSetup::Data PortSetup::m_data ;

const char PORT_REGISTER_SECTION []  = "" ;
const char PORT_DEFAULT_VALUE []     = "" ;
const char PORT_REGISTER_NAME_DAU [] = "DAUPort" ;
const char PORT_REGISTER_NAME_D1 []  = "D1Port" ;
const char PORT_REGISTER_NAME_D2 []  = "D2Port" ;
const char PORT_REGISTER_NAME_D3_A202 []  = "D3Port" ;
const char PORT_REGISTER_NAME_D4_A202 []  = "D4Port" ;

void PortSetup::Init( void )
{
  Registry reg;       
  m_data.m_dauPort = reg.GetStringValue("DAUPort", "");
  m_data.m_d1Port = reg.GetStringValue("D1Port", "");
  m_data.m_d2Port = reg.GetStringValue("D2Port", "");
  m_data.m_d3PortA202 = reg.GetStringValue("D3Port", "");
  m_data.m_d4PortA202 = reg.GetStringValue("D4Port", "");

//   m_data.m_dauPort = AfxGetApp() -> GetProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_DAU, PORT_DEFAULT_VALUE ) ;
//   m_data.m_d1Port = AfxGetApp() -> GetProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D1, PORT_DEFAULT_VALUE ) ;
//   m_data.m_d2Port = AfxGetApp() -> GetProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D2, PORT_DEFAULT_VALUE ) ;
//   m_data.m_d3PortA202 = AfxGetApp() -> GetProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D3_A202, PORT_DEFAULT_VALUE ) ;
//   m_data.m_d4PortA202 = AfxGetApp() -> GetProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D4_A202, PORT_DEFAULT_VALUE ) ;
}

void PortSetup::Exit( void )
{
  Registry reg;       

  reg.SetStringValue("DAUPort", m_data.m_dauPort);
  reg.SetStringValue("D1Port", m_data.m_d1Port);
  reg.SetStringValue("D2Port", m_data.m_d2Port);
  reg.SetStringValue("D3Port", m_data.m_d3PortA202);
  reg.SetStringValue("D4Port", m_data.m_d4PortA202);
        

//   AfxGetApp() -> WriteProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_DAU, m_data.m_dauPort ) ;
//   AfxGetApp() -> WriteProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D1, m_data.m_d1Port ) ;
//   AfxGetApp() -> WriteProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D2, m_data.m_d2Port ) ;
//   AfxGetApp() -> WriteProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D3_A202, m_data.m_d3PortA202 ) ;
//   AfxGetApp() -> WriteProfileString( PORT_REGISTER_SECTION, PORT_REGISTER_NAME_D4_A202, m_data.m_d4PortA202 ) ;
}

INT_PTR PortSetup::DoModal( void )
{
  PortSetupDialog dialog(theApp.IsAligner202Enabled() ? IDD_PORT_SETUP_A202 : IDD_PORT_SETUP) ;
  dialog.m_data = m_data ;
  INT_PTR dialogReturnValue ;
  if (( IDOK == (dialogReturnValue = dialog.DoModal()) ))
  {
    m_data = dialog.m_data ;
    //set dau port to default values
    SerialPort temporaryPort ;
    if (temporaryPort.Open( FALSE, m_data.m_dauPort, TRUE ))
    {
      if( FALSE == temporaryPort.SetToDefault() )
      {
        temporaryPort.Close() ;
        return dialogReturnValue ;
      }
      if( FALSE == temporaryPort.SaveConfig() )
      {
        temporaryPort.Close() ;
        return dialogReturnValue ;
      }
      temporaryPort.Close() ;
    }
    else
    {
      //ASSERT(0) ;
      // TODO: Add error notification.
    }
  }
  return dialogReturnValue ;
}

CString PortSetup::GetDAUPort( void )
{
  return m_data.m_dauPort ;
}

CString PortSetup::GetD1Port( void )
{
//  return m_data.m_d1Port ;										//	A308Pro
  return "COMX" ;
}

CString PortSetup::GetD2Port( void )
{
//  return m_data.m_d2Port ;										//	A308Pro
  return "COMY" ;
}

CString PortSetup::GetD3PortA202( void )
{
  return m_data.m_d3PortA202 ;
}

CString PortSetup::GetD4PortA202( void )
{
  return m_data.m_d4PortA202 ;
}

IMPLEMENT_DYNAMIC(PortSetupDialog, CDialog)

PortSetupDialog::PortSetupDialog( UINT DialogID, CWnd *pParent )	: CDialog(DialogID, pParent)
{
  // Empty
}

PortSetupDialog::~PortSetupDialog( void )
{
  // Empty
}

void PortSetupDialog::DoDataExchange( CDataExchange *pDX )
{
  CDialog::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_DAU_COM_PORT, m_dauPortControl);
  DDX_Control(pDX, IDC_D1_COM_PORT, m_d1PortControl);					//	A308Pro , handled by PortUartHdlcSetupDialogPro
  DDX_Control(pDX, IDC_D2_COM_PORT, m_d2PortControl);
  DDX_CBString(pDX, IDC_DAU_COM_PORT, m_data.m_dauPort);
  DDX_CBString(pDX, IDC_D1_COM_PORT, m_data.m_d1Port);
  DDX_CBString(pDX, IDC_D2_COM_PORT, m_data.m_d2Port);

  if( theApp.IsAligner202Enabled() == TRUE )
  {
    DDX_Control(pDX, IDC_D3_COM_PORT_A202, m_d3PortControlA202);
    DDX_Control(pDX, IDC_D4_COM_PORT_A202, m_d4PortControlA202);
    DDX_CBString(pDX, IDC_D3_COM_PORT_A202, m_data.m_d3PortA202);
    DDX_CBString(pDX, IDC_D4_COM_PORT_A202, m_data.m_d4PortA202);
  }

  if (pDX ->m_bSaveAndValidate)
  {
    if (UNUSED_STRING == m_data.m_d1Port)
    {
      m_data.m_d1Port = _T("") ;
    }
    if (UNUSED_STRING == m_data.m_d2Port)
    {
      m_data.m_d2Port = _T("") ;
    }
    if( theApp.IsAligner202Enabled() == TRUE )
    {
      if (UNUSED_STRING == m_data.m_d3PortA202)
      {
        m_data.m_d3PortA202 = _T("") ;
      }
      if (UNUSED_STRING == m_data.m_d4PortA202)
      {
        m_data.m_d4PortA202 = _T("") ;
      }
    }
  }
  DDX_Control(pDX, IDOK, m_okControl);
}

BEGIN_MESSAGE_MAP(PortSetupDialog, CDialog)
  ON_BN_CLICKED(ID_DAU_CHANGE, OnBnClickedDauChange)
  ON_BN_CLICKED(ID_D1_CHANGE, OnBnClickedD1Change)
  ON_BN_CLICKED(ID_D2_CHANGE, OnBnClickedD2Change)
  ON_CBN_SELCHANGE(IDC_DAU_COM_PORT, OnCbnSelchangeDauComPort)
  ON_CBN_SELCHANGE(IDC_D1_COM_PORT, OnCbnSelchangeD1ComPort)
  ON_CBN_SELCHANGE(IDC_D2_COM_PORT, OnCbnSelchangeD2ComPort)
  ON_BN_CLICKED(ID_D3_CHANGE_A202, OnBnClickedD3ChangeA202)
  ON_BN_CLICKED(ID_D4_CHANGE_A202, OnBnClickedD4ChangeA202)
  ON_CBN_SELCHANGE(IDC_D3_COM_PORT_A202, OnCbnSelchangeD3ComPortA202)
  ON_CBN_SELCHANGE(IDC_D4_COM_PORT_A202, OnCbnSelchangeD4ComPortA202)
END_MESSAGE_MAP()

static BOOL SetupPort( CComboBox &portControl, const CString &current, BOOL required )
{
  UINT port ;
  if (!SerialPortSet::GetFirstPort( port ))
  {
    return FALSE ;
  }

  UINT index = 0 ;
  if (!required)
  {
    portControl.AddString( UNUSED_STRING ) ;
    if (0 == current.GetLength())
    {
      portControl.SetCurSel( index ) ;
      portControl.SetWindowText( UNUSED_STRING ) ;
    }
    index++ ;
  }
  do
  {
    portControl.AddString( SerialPortSet::GetPortName( port ) ) ;
    if (SerialPortSet::GetPortName( port ) == current)
    {
      portControl.SetCurSel( index ) ;
      portControl.SetWindowText( SerialPortSet::GetPortName( port ) ) ;
    }
    index++ ;
  }
  while (SerialPortSet::GetNextPort( port )) ;
  return TRUE ;
}

void PortSetupDialog::VerifyOK( void )
{
  CString dauText ;
  m_dauPortControl.GetWindowText( dauText ) ;
  m_okControl.EnableWindow( _T("") != dauText ) ;
}

BOOL PortSetupDialog::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  //
  // Kludge: a computer without COM ports are in trouble!
  //
  (void)SetupPort( m_dauPortControl, m_data.m_dauPort, TRUE ) ;
  (void)SetupPort( m_d1PortControl, m_data.m_d1Port, FALSE ) ;
  (void)SetupPort( m_d2PortControl, m_data.m_d2Port, FALSE ) ;

	if( theApp.IsAligner202Enabled() == TRUE )
	{
		(void)SetupPort( m_d3PortControlA202, m_data.m_d3PortA202, FALSE ) ;
		(void)SetupPort( m_d4PortControlA202, m_data.m_d4PortA202, FALSE ) ;
	}
  VerifyOK() ;

  return TRUE ;
}

static void SetToZeroIfMatch( CComboBox &box, const CComboBox &reference )
{
  CString boxText ;
  CString referenceText ;
  box.GetWindowText( boxText ) ;
  reference.GetWindowText( referenceText ) ;
  if (referenceText == boxText)
  {
    box.SetCurSel( 0 ) ;
  }
}

static void SetToEmptyIfMatch( CComboBox &box, const CComboBox &reference )
{
  CString boxText ;
  CString referenceText ;
  box.GetWindowText( boxText ) ;
  reference.GetWindowText( referenceText ) ;
  if (referenceText == boxText)
  {
    box.SetCurSel( -1 ) ;
  }
}

static void ChangePort( const CComboBox &portControl )
{
  CString port ;
  portControl.GetWindowText( port ) ;
  SerialPort temporaryPort ;
  if (temporaryPort.Open( FALSE, port, TRUE ))
  {
    if( FALSE == temporaryPort.DoConfig() )
    {
      temporaryPort.Close();
      return;
    }
    if( FALSE == temporaryPort.SaveConfig() )
    {
      temporaryPort.Close();
      return;
    }
    if( FALSE == temporaryPort.Close() )
    {
      return;
    }
  }
  else
  {
    ASSERT(0) ;
    // TODO: Add error notification.
  }
}

void PortSetupDialog::OnBnClickedDauChange( void )
{
  ChangePort( m_dauPortControl ) ;
}

void PortSetupDialog::OnBnClickedD1Change( void )
{
  ChangePort( m_d1PortControl ) ;											//	A308Pro , handled by PortUartHdlcSetupDialogPro
}

void PortSetupDialog::OnBnClickedD2Change( void )
{
  ChangePort( m_d2PortControl ) ;
}

void PortSetupDialog::OnBnClickedD3ChangeA202( void )
{
  ChangePort( m_d3PortControlA202 ) ;
}

void PortSetupDialog::OnBnClickedD4ChangeA202( void )
{
  ChangePort( m_d4PortControlA202 ) ;
}

void PortSetupDialog::OnCbnSelchangeDauComPort( void )
{
  SetToZeroIfMatch( m_d1PortControl, m_dauPortControl ) ;
  SetToZeroIfMatch( m_d2PortControl, m_dauPortControl ) ;

	if( theApp.IsAligner202Enabled() == TRUE )
	{
		SetToZeroIfMatch( m_d3PortControlA202, m_dauPortControl ) ;
		SetToZeroIfMatch( m_d4PortControlA202, m_dauPortControl ) ;
	}
  VerifyOK() ;
}

void PortSetupDialog::OnCbnSelchangeD1ComPort( void )
{
  SetToEmptyIfMatch( m_dauPortControl, m_d1PortControl ) ;
  SetToZeroIfMatch( m_d2PortControl, m_d1PortControl ) ;

	if( theApp.IsAligner202Enabled() == TRUE )
	{
		SetToZeroIfMatch( m_d3PortControlA202, m_d1PortControl ) ;
		SetToZeroIfMatch( m_d4PortControlA202, m_d1PortControl ) ;
	}
  VerifyOK() ;
}

void PortSetupDialog::OnCbnSelchangeD2ComPort( void )
{
  SetToEmptyIfMatch( m_dauPortControl, m_d2PortControl ) ;
  SetToZeroIfMatch( m_d1PortControl, m_d2PortControl ) ;

	if( theApp.IsAligner202Enabled() == TRUE )
	{
		SetToZeroIfMatch( m_d3PortControlA202, m_d2PortControl ) ;
		SetToZeroIfMatch( m_d4PortControlA202, m_d2PortControl ) ;
	}
  VerifyOK() ;
}

void PortSetupDialog::OnCbnSelchangeD3ComPortA202( void )
{
  SetToEmptyIfMatch( m_dauPortControl, m_d3PortControlA202 ) ;
  SetToZeroIfMatch( m_d1PortControl, m_d3PortControlA202 ) ;
  SetToZeroIfMatch( m_d2PortControl, m_d3PortControlA202 ) ;
  SetToZeroIfMatch( m_d4PortControlA202, m_d3PortControlA202 ) ;
  VerifyOK() ;
}

void PortSetupDialog::OnCbnSelchangeD4ComPortA202( void )
{
  SetToEmptyIfMatch( m_dauPortControl, m_d4PortControlA202 ) ;
  SetToZeroIfMatch( m_d1PortControl, m_d4PortControlA202 ) ;
  SetToZeroIfMatch( m_d2PortControl, m_d4PortControlA202 ) ;
  SetToZeroIfMatch( m_d3PortControlA202, m_d4PortControlA202 ) ;
  VerifyOK() ;
}
//*******************************************************************************//
//***												PortAsyncSetupDialogPro		A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***																					***//
//*******************************************************************************//
//PortUartHdlcSetupDialogPro m_portUartHdlcSetupDialogPro ;

IMPLEMENT_DYNAMIC(PortUartHdlcSetupDialogPro, CDialog)

PortUartHdlcSetupDialogPro::PortUartHdlcSetupDialogPro( UINT DialogID, CWnd *pParent )	: CDialog(DialogID, pParent)
{
  // Empty
}

PortUartHdlcSetupDialogPro::~PortUartHdlcSetupDialogPro( void )
{
  // Empty
}

void PortUartHdlcSetupDialogPro::DoDataExchange( CDataExchange *pDX )
{
  CDialog::DoDataExchange( pDX ) ;
  //DDX_Control(pDX, IDC_D1_COM_PORT, m_d1PortControl);
  DDX_Control(pDX, IDC_COMBO_ASYNC_BPS, m_asyncBitsPerSec);							//	A308Pro
  DDX_Control(pDX, IDC_COMBO_ASYNC_DATABITS, m_asyncDataBits);					//	A308Pro
  DDX_Control(pDX, IDC_COMBO_ASYNC_PARITY, m_asyncParity);							//	A308Pro


  //if (pDX ->m_bSaveAndValidate)
  //{
  //  if (UNUSED_STRING == m_data.m_d1Port)
  //  {
  //    m_data.m_d1Port = _T("") ;
  //  }
  //  if (UNUSED_STRING == m_data.m_d2Port)
  //  {
  //    m_data.m_d2Port = _T("") ;
  //  }
  //  if( theApp.IsAligner202Enabled() == TRUE )
  //  {
  //    if (UNUSED_STRING == m_data.m_d3PortA202)
  //    {
  //      m_data.m_d3PortA202 = _T("") ;
  //    }
  //    if (UNUSED_STRING == m_data.m_d4PortA202)
  //    {
  //      m_data.m_d4PortA202 = _T("") ;
  //    }
  //  }
  //}
  //DDX_Control(pDX, IDOK, m_okControl);
}

BEGIN_MESSAGE_MAP(PortUartHdlcSetupDialogPro, CDialog)
  //ON_BN_CLICKED(ID_DAU_CHANGE, OnBnClickedDauChange)
  //ON_BN_CLICKED(ID_D1_CHANGE, OnBnClickedD1Change)
  //ON_BN_CLICKED(ID_D2_CHANGE, OnBnClickedD2Change)
  //ON_CBN_SELCHANGE(IDC_DAU_COM_PORT, OnCbnSelchangeDauComPort)
  //ON_CBN_SELCHANGE(IDC_D1_COM_PORT, OnCbnSelchangeD1ComPort)
  //ON_CBN_SELCHANGE(IDC_D2_COM_PORT, OnCbnSelchangeD2ComPort)
  //ON_BN_CLICKED(ID_D3_CHANGE_A202, OnBnClickedD3ChangeA202)
  //ON_BN_CLICKED(ID_D4_CHANGE_A202, OnBnClickedD4ChangeA202)
  //ON_CBN_SELCHANGE(IDC_D3_COM_PORT_A202, OnCbnSelchangeD3ComPortA202)
  //ON_BN_CLICKED(ID_D1_CHANGE, OnBnClickedD1Change)
  ON_CBN_SELCHANGE(IDC_COMBO_ASYNC_BPS, OnCbnSelChangeAsyncBps)
  ON_CBN_SELCHANGE(IDC_COMBO_ASYNC_DATABITS, OnCbnSelChangeAsyncDataBits)
  ON_CBN_SELCHANGE(IDC_COMBO_ASYNC_PARITY, OnCbnSelChangeAsyncParity)
//	ON_WM_SHOWWINDOW(IDD_PORT_ASYNC_SETUP_PRO, OnWmShowWindowPortAsyncSetupPro)
END_MESSAGE_MAP()



BOOL PortUartHdlcSetupDialogPro::OnInitDialog( void )
{
  CDialog::OnInitDialog() ;
  //(void)SetupPort( m_dauPortControl, m_data.m_dauPort, TRUE ) ;
  //(void)SetupPort( m_d1PortControl, m_data.m_d1Port, FALSE ) ;
  //(void)SetupPort( m_d2PortControl, m_data.m_d2Port, FALSE ) ;

  //VerifyOK() ;
//	m_asyncBitsPerSec.MaxDropDownItems = 4 ;
// int a = 1 ;
// int b = 2 ;
// int c = a + b ;
//pmyComboBox->SetCurSel(nCount-1);
m_asyncBitsPerSec.SetCurSel(3) ;
m_asyncDataBits.SetCurSel(3) ;
m_asyncParity.SetCurSel(3) ;

//CString S1 = "S1" ;
//CString S2 = "S2" ;
//	m_asyncBitsPerSec.AddString(S1) ;
//	m_asyncBitsPerSec.AddString(S2) ;

  return TRUE ;
}
void PortUartHdlcSetupDialogPro::OnBnClickedD1Change( void )
{
  //ChangePort( m_d1PortControl ) ;
// int a = 1 ;
// int b = 2 ;
// int c = a + b ;

}
void PortUartHdlcSetupDialogPro::OnCbnSelChangeAsyncBps( void )
{
CString S1 ;
CString S2 ;
CString S3 ;
//MainBoardData *pMainBoardData ;


//pMainBoardData = DAU::GetMainBoardData() ;
//pMainBoardData = DAU::GetMainBoardData() ;

m_asyncBitsPerSec.GetWindowText( S1 ) ;
//DAUDataPro::m_dauDataPro.m_mainBoard.m_uartBaudRateA = atoi( S1 ) ;

//MainBoardDataSetup m_mainBoardDataSetup ;
//m_data.m_mainBoardDataSetup.SetDigChParams( S1, S1, S1) ;
	
//pMainBoardData->m_uartBaudRateA = 333 ;

S2 = S1 ;
S3 = S2 ;

// int a = 1 ;
// int b = 2 ;
// int c = a + b ;
}

void PortUartHdlcSetupDialogPro::OnCbnSelChangeAsyncDataBits( void )
{
// int a = 1 ;
// int b = 2 ;
// int c = a + b ;
}

void PortUartHdlcSetupDialogPro::OnCbnSelChangeAsyncParity( void )
{
// int a = 1 ;
// int b = 2 ;
// int c = a + b ;
}


