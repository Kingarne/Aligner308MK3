#pragma once

#define TIME_CRYPTO_CONST_1   33
#define TIME_CRYPTO_CONST_2   17

#define KEY_NAME        HKEY_CURRENT_USER
#define SUB_KEY_NAME    _T("Software\\Microsoft\\Windows Help")
//#define SUB_KEY_NAME    _T("Software\\Schill Reglerteknik AB\\Test")
#define DEVICE_NAME     _T("UserID")


///////////////////////
// TimeCheck
///////////////////////

class TimeCheck
{
public:
	TimeCheck();
	virtual ~TimeCheck();

  static BOOL IsValidTime( void );  

private:
  static BOOL GetRegTime( COleDateTime &time );
  static BOOL SetRegTime( COleDateTime &time );
};


///////////////////////
// TimeCheck dialog
///////////////////////

class TimeCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(TimeCheckDlg)

public:
	TimeCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TimeCheckDlg();

// Dialog Data
	enum { IDD = IDD_TIME_CHECK_DLG };

private:
  int m_codeSeed;
  BOOL IsCodeValid( void );
  int GetCode( int codeSeed );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
};
