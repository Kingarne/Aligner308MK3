// TimeCheck.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "TimeCheck.h"
#include ".\timecheck.h"

/////////////////////////
// TimeCheck
/////////////////////////

TimeCheck::TimeCheck()
{
}

TimeCheck::~TimeCheck()
{
}

BOOL TimeCheck::IsValidTime( void )
{
  COleDateTime regTime;
  GetRegTime( regTime );
  COleDateTime currTime = COleDateTime::GetCurrentTime();

  if( regTime > currTime )
  {
    TimeCheckDlg timeCheckDlg;
    if( IDOK != timeCheckDlg.DoModal() )
    {
      return FALSE ;
    }
  }

  SetRegTime( currTime );

  return( TRUE );
}

BOOL TimeCheck::GetRegTime( COleDateTime &time )
{
  time = COleDateTime::GetCurrentTime();
 
	HKEY hSubKey ;
  CString subKeyName = SUB_KEY_NAME;
	if (ERROR_SUCCESS != RegCreateKeyEx( KEY_NAME, (LPCTSTR)subKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return FALSE;
	}

	COleDateTime regTime;
  DWORD regTimeSize = sizeof(regTime) ;
  CString deviceName = DEVICE_NAME;
	if (ERROR_SUCCESS != RegQueryValueEx( hSubKey, (LPCTSTR)deviceName, NULL, NULL, reinterpret_cast<unsigned char *>(&regTime), &regTimeSize ) )
  {
    RegCloseKey( hSubKey ) ;
    return( FALSE );
  }
  
  RegCloseKey( hSubKey ) ;
  time = regTime;
  return( TRUE );
}

BOOL TimeCheck::SetRegTime( COleDateTime &time )
{
	HKEY hSubKey ;
  CString subKeyName = SUB_KEY_NAME;
	if (ERROR_SUCCESS != RegCreateKeyEx( KEY_NAME, (LPCTSTR)subKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL))
	{
		return FALSE;
	}

  DWORD timeSize = sizeof(time) ;
  CString deviceName = DEVICE_NAME;
	if (ERROR_SUCCESS != RegSetValueEx( hSubKey, (LPCTSTR)deviceName, 0, NULL, reinterpret_cast<unsigned char *>(&time), timeSize ) )
  {
    RegCloseKey( hSubKey ) ;
    return FALSE;
	}

	RegCloseKey( hSubKey ) ;
  return TRUE;
}

/////////////////////////
// TimeCheck dialog
/////////////////////////

IMPLEMENT_DYNAMIC(TimeCheckDlg, CDialog)
TimeCheckDlg::TimeCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TimeCheckDlg::IDD, pParent)
{
}

TimeCheckDlg::~TimeCheckDlg()
{
}

void TimeCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL TimeCheckDlg::IsCodeValid( void )
{
  CString codeStr;
  GetDlgItem( IDC_TIME_CODE )->GetWindowText( codeStr );
  int code = atoi( codeStr ) ;
  int validCode = GetCode( m_codeSeed );

  if( code == validCode )
  {
    return TRUE;
  }
  return FALSE;
}

int TimeCheckDlg::GetCode( int codeSeed )
{
  int code;
  code = (int)(abs(codeSeed - TIME_CRYPTO_CONST_1) / TIME_CRYPTO_CONST_2);
  return( code );
}

BEGIN_MESSAGE_MAP(TimeCheckDlg, CDialog)
END_MESSAGE_MAP()

// TimeCheckDlg message handlers
BOOL TimeCheckDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  //Seed the random-number generator with current time so that
  //the numbers will be different every time we run.
  srand( (unsigned)time( NULL ) );
  m_codeSeed = abs( rand() );
  CString codeSeedStr;
  codeSeedStr.Format( "%d", m_codeSeed ) ;
  GetDlgItem( IDC_TIME_CODE )->SetWindowText( codeSeedStr );
    

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void TimeCheckDlg::OnOK()
{
  // TODO: Add your specialized code here and/or call the base class
  if( IsCodeValid() == FALSE )
  {
    ::AfxMessageBox( _T("Invalid code") );
    return;
  }
  CDialog::OnOK();
}
