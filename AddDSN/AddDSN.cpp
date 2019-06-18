#include "stdafx.h"
#include "AddDSN.h"
#include "registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp ;

using namespace std ;

void CreateDBReg( LPCTSTR lpDSN, LPCTSTR lpOptions )
{
	CString str;
	str.Format("Create (Reg): '%s', '%s'",lpDSN,lpOptions);
	AfxMessageBox(str);
		
	Registry reg;
	reg.m_rootKey = HKEY_CURRENT_USER;
	reg.m_regPath = "SOFTWARE\\ODBC\\ODBC.INI\\";
	reg.m_strAppName = lpDSN;

	//str.Format("%");
	reg.SetStringValue("DBQ", lpOptions);
	reg.SetStringValue("Driver", "C:\\Windows\\system32\\odbcjt32.dll");
	reg.SetIntegerValue("DriverId", 25);
	reg.SetIntegerValue("SafeTransaktions", 0);
	reg.SetStringValue("UID", "");

	reg.SetStringValue("Engines\\Jet\\ImplicitCommitSync", "");
	reg.SetIntegerValue("Engines\\Jet\\PageTimeout", 5);
	reg.SetIntegerValue("Engines\\Jet\\MaxBufferSize", 2048);
	reg.SetIntegerValue("Engines\\Jet\\Threads", 3);
	reg.SetStringValue("Engines\\Jet\\UserCommitSync", "Yes");
	
	reg.m_regPath = "SOFTWARE\\ODBC\\";
	reg.m_strAppName = "ODBC.INI";
	str.Format("ODBC Data Sources\\%s",lpDSN);
	reg.SetStringValue(str, "Microsoft Access Driver (*.mdb)");
}


void CreateDB( LPCTSTR lpDSN, LPCTSTR lpOptions )
{
	/*char *p = new char [strlen( lpDSN ) + strlen( lpOptions ) + 11] ;
	strcpy( p, "DSN=" ) ;
	strcpy( p + 4, lpDSN ) ;
	strcpy( p + 5 + strlen( lpDSN ), "DBQ=" ) ;
	strcpy( p + 9 + strlen( lpDSN ), lpOptions ) ;
	p [10 + strlen( lpDSN ) + strlen( lpOptions )] = 0 ;
	//::SQLConfigDataSource( NULL, ODBC_ADD_DSN, "Microsoft Access Driver (*.mdb)", p ) ;*/
	CString str;
	
	HMODULE hInstLib = LoadLibrary(_T("ODBCCP32.DLL"));
	if (hInstLib)
	{
		char *p = new char [strlen( lpDSN ) + strlen( lpOptions ) + 11] ;
		strcpy( p, "DSN=" ) ;
		strcpy( p + 4, lpDSN ) ;
		strcpy( p + 5 + strlen( lpDSN ), "DBQ=" ) ;
		strcpy( p + 9 + strlen( lpDSN ), lpOptions ) ;
		p [10 + strlen( lpDSN ) + strlen( lpOptions )] = 0 ;
		
		BOOL (__stdcall *lpfSQLConfigDataSource)(HWND,UINT, LPCSTR, LPCSTR);
		RETCODE (__stdcall *lpfSQLInstallerError)(WORD,DWORD*,LPSTR,WORD,WORD*);

		lpfSQLConfigDataSource = (BOOL(__stdcall *)(HWND,UINT, LPCSTR, LPCSTR)) GetProcAddress(hInstLib,"SQLConfigDataSource");		
		
		//str.Format("Create: '%s', '%s'",p,p+15);
		//AfxMessageBox(str);
		BOOL res = (*lpfSQLConfigDataSource)( NULL, ODBC_ADD_DSN, "Microsoft Access Driver (*.mdb)", p ) ;
		
		/*if(!res)
			AfxMessageBox("Fail!!");
		else
			AfxMessageBox("Okay!");
			*/
		delete [] p ;
	}
	
	
}

void RemoveExe()
{
    Registry reg;
    CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");

    CString exePath;
    exePath = dir + "\\" + "Aligner 308.exe";	

    if(!::DeleteFile(exePath))
	{
		int err = GetLastError();
		TRACE("Err:%d");
	}
}

int _tmain( int argc, TCHAR *argv [], TCHAR *envp [] )
{

    if (!AfxWinInit( ::GetModuleHandle( NULL ), NULL, ::GetCommandLine(), 0 ))
	{
    ::AfxMessageBox( _T("Unable to initiate MFC") ) ;
		return 1 ;
	}
  if (argc < 3)
  {
    ::AfxMessageBox( _T("Wrong argument count") ) ;
    return 1 ;
  }
    
  CString argCommand, versionPrefix="";
    if(argc > 2)
       argCommand = argv [2];
    
    int t = atoi( argv [1] ) ;

	if(t != 202 && t != 308)
	{
		CString str;str.Format("Unknown program type: %d",t);
		::AfxMessageBox( str ) ;
		return 0;
	}

	if(argc > 3)
		versionPrefix = argv [3];


    CString appName, dbName, dsnName ;
    appName.Format("Aligner %d MK2",t);
    if(versionPrefix != "")
		appName.Format("%s %s",appName, versionPrefix);
	
	dbName.Format("Aligner%d.mdb",t) ;
    dsnName.Format("Aligner%d",t) ;    

    Registry reg;
    reg.m_strAppName = appName;

    if(argCommand == "RemoveExe")
    {
        RemoveExe();
        return 0;
    }	
	else if(argCommand == "DSN")
	{	
		CString dir = reg.GetStringValue("Directory", "").TrimRight("\\");
// 		if(versionPrefix != "")
// 			dir.Format("%s %s",dir, versionPrefix);

		CString dbPath ;  
		dbPath = dir + "\\" + dbName;
	
		//CreateDB( dsnName, dbPath ) ;
		CreateDBReg( dsnName, dbPath ) ;
		return 0;
	}

    return 0 ;
}
