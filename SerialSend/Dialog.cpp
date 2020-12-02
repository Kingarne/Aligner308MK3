#include "stdafx.h"
#include "SerialSend.h"
#include "SerialPort.h"
#include "Dialog.h"
#include ".\dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
  DECLARE_MESSAGE_MAP()
public:
  CAboutDlg( void ) ;
private:
  enum { IDD = IDD_ABOUTBOX };
protected:
  virtual void DoDataExchange( CDataExchange *pDX ) ;
protected:
  virtual BOOL OnInitDialog( void ) ;
} ;

CAboutDlg::CAboutDlg( void ) : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange( pDX ) ;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

Dialog::Dialog( CWnd *pParent ) : CDialog( Dialog::IDD, pParent ), m_text(_T(""))
, m_hex(FALSE)
{
  m_hIcon = AfxGetApp() -> LoadIcon( IDR_MAINFRAME ) ;
  m_pData = NULL ;
  m_dataLength = 0 ;
  m_addEndChar = FALSE;
}

void Dialog::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Text(pDX, IDC_EDIT1, m_text);
  DDX_Control(pDX, IDC_COMBO2, m_frequency);
  DDX_Control(pDX, IDC_COMBO1, m_comPort);
  DDX_Check(pDX, IDC_HEX, m_hex);
}

BEGIN_MESSAGE_MAP(Dialog, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_COM_SETTINGS_BUTTON, OnBnClickedComSettingsButton) 
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_DAU_BUTTON, OnBnClickedDauButton)
  ON_BN_CLICKED(IDC_ADD_STOP_CHAR, OnBnClickedAddStopChar)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_CAM_CTRL_BUTTON, &Dialog::OnBnClickedCamCtrlButton)
  ON_BN_CLICKED(IDC_APPLY_BUTTON, &Dialog::OnBnClickedApplyButton)
END_MESSAGE_MAP()

void staticCallback(PVOID pData, LPARAM lp)
{
    Dialog* pDlg = (Dialog*)pData;
    pDlg->Callback(lp);
}


BOOL Dialog::OnInitDialog( void )
{
    __super::OnInitDialog() ;
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu *pSysMenu = GetSystemMenu( FALSE ) ;
    if (pSysMenu != NULL)
    {
        CString strAboutMenu ;
        strAboutMenu.LoadString( IDS_ABOUTBOX ) ;
        if (!strAboutMenu.IsEmpty())
        {
        pSysMenu -> AppendMenu( MF_SEPARATOR ) ;
        pSysMenu -> AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu ) ;
        }
    }
    SetIcon( m_hIcon, TRUE ) ;
    SetIcon( m_hIcon, FALSE ) ;
    UINT port ;
    if (SerialPortSet::GetFirstPort( port ))
    {
        do
        {
            m_comPort.AddString( SerialPortSet::GetPortName( port ) ) ;
        }
        while (SerialPortSet::GetNextPort( port )) ;
        m_comPort.SetCurSel( 0 ) ;
    }
    m_frequency.SetCurSel( 0 ) ;
      
    m_dauDlg.m_pSerialPort = &m_serialPort;
    m_dauDlg.Create(IDD_DAU_DIALOG);

	m_camCtrlDlg.m_pSerialPort = &m_serialPort;
	m_camCtrlDlg.Create(IDD_CAM_CTRL_DIALOG);

    CButton* pButt = (CButton*)GetDlgItem(IDC_ADD_STOP_CHAR);
    pButt->SetCheck(m_addEndChar);
	
	m_HFSender.SetCallback(&staticCallback, this);
	m_HFSender.Create();

  

    return TRUE ;
}

void Dialog::Callback(LPARAM lp)
{	
	if (m_text != "")
	{
	
		if (m_hex)
		{
			m_serialPort.Write(reinterpret_cast<const char *>(m_pData), m_dataLength);
			TRACE("Send hex(%d)\n", m_dataLength);
		}
		else
		{
			CString sendText = m_text;
			if (m_addEndChar)
				sendText += "\r\n";
			m_serialPort.Write(sendText, sendText.GetLength());
			TRACE("Send %s\n",sendText);
		}
	}
	else
	{
		m_dauDlg.UpdateValues();
		m_dauDlg.Send();
	}
}

void Dialog::OnSysCommand( UINT nID, LPARAM lParam )
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout ;
    dlgAbout.DoModal() ;
  }
  else
  {
    __super::OnSysCommand( nID, lParam ) ;
  }
}

void Dialog::OnPaint( void )
{
  if (IsIconic())
  {
    CPaintDC dc( this ) ;
    SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0 ) ;
    int cxIcon = GetSystemMetrics( SM_CXICON ) ;
    int cyIcon = GetSystemMetrics( SM_CYICON ) ;
    CRect rect ;
    GetClientRect( &rect ) ;
    int x = (rect.Width() - cxIcon + 1) / 2 ;
    int y = (rect.Height() - cyIcon + 1) / 2 ;
    dc.DrawIcon( x, y, m_hIcon ) ;
  }
  else
  {
    __super::OnPaint() ;
  }
}
HCURSOR Dialog::OnQueryDragIcon( void )
{
  return static_cast<HCURSOR>( m_hIcon ) ;
}

void Dialog::OnBnClickedComSettingsButton( void )
{
    CString name ;
    m_comPort.GetWindowText( name ) ;
        
    if (!m_serialPort.IsOpen())
    {
        //name = "\\\\.\\" + name;  
        if (!m_serialPort.Open( name ) )
        {
            return ;
        }
    }	
    m_serialPort.DoConfig() ;
    m_serialPort.SaveConfig() ;

	TRACE("Com %s is open\n",name);
}

void Dialog::OnBnClickedApplyButton()
{
   // KillTimer( 1 ) ;
    UpdateData() ;
    UpdateHex() ;
    CString name ;
    m_comPort.GetWindowText( name ) ;
    if (!m_serialPort.IsOpen())
    {
        //name = "\\\\.\\" + name;    
        if (!m_serialPort.Open( name, TRUE ) )
        {
        return ;
        }
    }
    m_HFSender.SetFrequence(0);
    double frequence=0.0f ;
   // int interval = 0;
    switch (m_frequency.GetCurSel())
    {
    case CB_ERR:
        m_serialPort.Close() ;
        return ;

    case 0:
        frequence = 1 ;
        break ;

    case 1:
        frequence = 2 ;
        //interval = 500;
        break ;

    case 2:
        frequence = 5 ;
        break ;

    case 3:        
        frequence = 10 ;
        break ;

    case 4:
        frequence = 30.5 ;
        break ;

    case 5:
        frequence = 100 ;
        break ;
    case 6:
        frequence = 200 ;
        break ;
    }

    m_HFSender.SetFrequence(frequence);

   //  if(frequence > 0)
   //      SetTimer( 1, interval, NULL ) ;
}

BOOL CAboutDlg::OnInitDialog( void )
{
  __super::OnInitDialog() ;
  return TRUE ;
}

void Dialog::OnTimer( UINT nIDEvent )
{
    TRACE("text:%s\n",m_text);
    
    if(m_text != "")
    {
        if (m_hex)
        {
            m_serialPort.Write( reinterpret_cast<const char *>(m_pData), m_dataLength ) ;
        }
        else
        {
            CString sendText = m_text ;
            if(m_addEndChar)
                sendText += "\r\n";
            m_serialPort.Write( sendText, sendText.GetLength() ) ;
        }
    }
    else
    {
        m_dauDlg.UpdateValues();
        m_dauDlg.Send();
    }

    __super::OnTimer( nIDEvent ) ;
}

void Dialog::UpdateHex( void )
{
  if (m_hex)
  {
    std::list<int> dataList ;
    TCHAR *pText = m_text.LockBuffer() ;
    while (true)
    {
      int data ;
      int scaned ;
      int assigned = _stscanf( pText, _T("%x%n"), &data, &scaned ) ;
      if (0 >= assigned)
      {
        break ;
      }
      pText += scaned ;
      dataList.push_back( data ) ;
    }
    m_text.UnlockBuffer() ;
    ClearData() ;
    m_pData = new BYTE [m_dataLength = dataList.size()] ;
    int offset = 0 ;
    for (list<int>::iterator i = dataList.begin() ; i != dataList.end() ; i++)
    {
      m_pData [offset++] = *i ;
    }
  }
}

void Dialog::ClearData( void )
{
  if (m_pData)
  {
    delete m_pData ;
    m_pData = NULL ;
  }
  m_dataLength = 0 ;
}
void Dialog::OnBnClickedDauButton()
{
	m_HFSender.SetCallback(&staticCallback, this);
	m_HFSender.Create();       


	m_dauDlg.ShowWindow(m_dauDlg.IsWindowVisible()?SW_HIDE:SW_SHOW);
}

void Dialog::OnBnClickedAddStopChar()
{
    CButton* pButt = (CButton*)GetDlgItem(IDC_ADD_STOP_CHAR);
    m_addEndChar = pButt->GetCheck();
}

void Dialog::OnDestroy()
{
    CDialog::OnDestroy();

    m_HFSender.Destroy();
}


void Dialog::OnBnClickedCamCtrlButton()
{
	
	m_camCtrlDlg.StartReadThread();
	m_camCtrlDlg.ShowWindow(m_camCtrlDlg.IsWindowVisible()?SW_HIDE:SW_SHOW);
}


