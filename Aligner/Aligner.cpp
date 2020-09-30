// $Id: Aligner.cpp,v 1.27 2014-10-16 08:46:55 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002
// CVS check

#include "StdAfx.h"
#include "Aligner.h"
#include "AlignerDoc.h"
#include "AlignerView.h"
#include "MainFrm.h"
//#include "PortSetup.h"
#include "TiltAlignmentWizard.h"
#include "AzimuthAlignWizard.h"
#include "TiltFlatnessTestWizard.h"
#include "GyroPerformanceTestWizard.h"
#include "GunAdapterTiltCheckDlg.h"
#include "TiltVerificationWizard.h"
#include "AzimuthVerificationWizard.h"
#include "SetupLiveDataGraphDlg.h"
#include "SystemConfigurationView.h"
#include "LiveDataViewA202.h"
#include "LiveDataView.h"
#include "Database.h"
#include "Util.h"
#include "Network.h"
#include "TimeCheck.h"
#include <FCNTL.H>
#include <crtdbg.h>
#include "SecrRegistry.h"
#include <gdiplus.h>
#include "InfoDlg.h"
#include "ProjectOpenDlg.h"
#include "ReportManager.h"
//using namespace Gdiplus;

#include <shellapi.h>

#if defined(DAU_SIMULATION)
#include "DAUSimulationDialog.h"
#include ".\aligner.h"
#endif

ULONG_PTR gdiplusToken;

ALIGNER_DATA g_AlignerData ;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAlignerApp, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//***  1.14 ; Prepare before calling CWinApp::OnFileOpen
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	//***  1.14 End
  ON_COMMAND(ID_UTILITIES_DATACONVERSION, OnUtilitiesDataconversion)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_DATACONVERSION, OnUpdateUtilitiesDataconversion)
  ON_COMMAND(ID_ALIGNMENT_TILTALIGNMENT, OnAlignmentTiltalignment)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_TILTALIGNMENT, OnUpdateAlignmentTiltalignment)
//  ON_COMMAND(ID_ALIGNMENT_TILTFLATNESSTEST, OnAlignmentTiltFlatnessTest)
//  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_TILTFLATNESSTEST, OnUpdateAlignmentTiltFlatnessTest)
  ON_COMMAND(ID_ALIGNMENT_TILTFLATNESSTEST_PLATFORM, OnAlignmentTiltFlatnessTestPlatform)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_TILTFLATNESSTEST_PLATFORM, OnUpdateAlignmentTiltFlatnessTestPlatform)
  ON_COMMAND(ID_ALIGNMENT_TILTFLATNESSTEST_FOUNDATION, OnAlignmentTiltFlatnessTestFoundation)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_TILTFLATNESSTEST_FOUNDATION, OnUpdateAlignmentTiltFlatnessTestFoundation)
  ON_COMMAND(ID_UTILITIES_LIVEDATA, OnUtilitiesLivedata)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_LIVEDATA, OnUpdateUtilitiesLivedata)
  ON_COMMAND(ID_UTILITIES_LIVEGRAPH, OnUtilitiesLivegraph)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_LIVEGRAPH, OnUpdateUtilitiesLivegraph)
  ON_COMMAND(ID_ALIGNMENT_AZIMUTHALIGNMENT, OnAlignmentAzimuthAlignment)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_AZIMUTHALIGNMENT, OnUpdateAlignmentAzimuthAlignment)
  ON_COMMAND(ID_ALIGNMENT_GYROPERFORMANCETEST, OnAlignmentGyroPerformanceTest)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_GYROPERFORMANCETEST, OnUpdateAlignmentGyroPerformanceTest)
  ON_COMMAND(ID_ALIGNMENT_GUNADAPTERTILTCHECK, OnAlignmentGunAdapterTiltCheck)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_GUNADAPTERTILTCHECK, OnUpdateAlignmentGunAdapterTiltCheck)
  ON_COMMAND(ID_ALIGNMENT_COMMONFLATTEST, OnAlignmentCommonFlatTest)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_COMMONFLATTEST, OnUpdateAlignmentCommonFlatTest)
  ON_COMMAND(ID_TILTVERIFICATION_HORIZONTEST, OnAlignmentTiltVerificationHorizonTest)
  ON_UPDATE_COMMAND_UI(ID_TILTVERIFICATION_HORIZONTEST, OnUpdateAlignmentTiltVerificationHorizonTest)
  ON_COMMAND(ID_TILTVERIFICATION_AIRTARGETTEST, OnAlignmentTiltVerificationAirTargetTest)
  ON_UPDATE_COMMAND_UI(ID_TILTVERIFICATION_AIRTARGETTEST, OnUpdateAlignmentTiltVerificationAirTargetTest)
  ON_COMMAND(ID_AZIMUTHVERIFICATION_BENCHMARKTEST, OnAlignmentAzimuthVerificationBenchmarkTest)
  ON_UPDATE_COMMAND_UI(ID_AZIMUTHVERIFICATION_BENCHMARKTEST, OnUpdateAlignmentAzimuthVerificationBenchmarkTest)
  ON_COMMAND(ID_AZIMUTHVERIFICATION_GYROSTABTEST, OnAlignmentAzimuthVerificationGyroStabTest)
  ON_UPDATE_COMMAND_UI(ID_AZIMUTHVERIFICATION_GYROSTABTEST, OnUpdateAlignmentAzimuthVerificationGyroStabTest)
  ON_COMMAND(ID_UTILITIES_DA, OnUtilitiesDa)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_DA, OnUpdateUtilitiesDa)
END_MESSAGE_MAP()

CAlignerApp::CAlignerApp( void )
{
  EnableHtmlHelp() ;
  m_showAgainMap[SA_AZIMUTH] = TRUE;
  m_showAgainMap[SA_GUN] = TRUE;
}

CAlignerApp::~CAlignerApp( void )
{
  // Empty
}

CAlignerApp theApp ;


BOOL CAlignerApp::CheckExpired()
{
	return FALSE;

	SecretRegistry reg;	
	reg.m_strAppName = "Util";

	SYSTEMTIME currT,expT;
	GetLocalTime(&currT);
	expT = currT;
	expT.wYear = 2011;	
	expT.wMonth = 10;
	expT.wDay = 1;
	
	//expTime.wYear = 
	CTime currTime(currT);
	CTime expTime(expT);
 
	if(currTime.GetTime() > expTime.GetTime())
	{
		reg.SetStringValue("String1", "");
	}

	CString obscureString = reg.GetStringValue("String1", "dds");
	if(obscureString != "krackelibankelfnatt")
	{
		return TRUE;
	}
	//if(time.wYear)

	return FALSE;
}

void PrintTime( vector<SelectedData> &data )
{
    vector<SelectedData>::iterator iter;
    for(iter = data.begin();iter!=data.end();iter++)
    {
        DBTIMESTAMP ts = iter->m_time;
        TRACE("%d:%d %d:%d:%d,%d\n",ts.year, ts.month, ts.hour, ts.minute, ts.second, ts.fraction);
    }
}


void SortInTime( vector<SelectedData> &data )
{
    SelectedData tmpData;
    BOOL swap;
    TRACE("Before\n"); 
    PrintTime( data );
    for( unsigned int i=0; i<data.size()-1; i++ )
    {
        for( unsigned int j=i+1; j<data.size(); j++ )
        {
            swap = FALSE;
            if( (data[j].m_time.year > data[i].m_time.year ) )
            {
                swap = TRUE;
            }
            else if(  (data[j].m_time.year == data[i].m_time.year ) )
            {
                if( (data[j].m_time.month > data[i].m_time.month ) )
                {
                    swap = TRUE;
                }
                else if(  (data[j].m_time.month == data[i].m_time.month ) )
                {
                    if( (data[j].m_time.day > data[i].m_time.day ) )
                    {
                        swap = TRUE;
                    }
                    else if(  (data[j].m_time.day == data[i].m_time.day ) )
                    {
                        if( (data[j].m_time.hour > data[i].m_time.hour ) )
                        {
                            swap = TRUE;
                        }
                        else if(  (data[j].m_time.hour == data[i].m_time.hour ) )
                        {
                            if( (data[j].m_time.minute > data[i].m_time.minute ) )
                            {
                                swap = TRUE;
                            }
                            else if(  (data[j].m_time.minute == data[i].m_time.minute ) )
                            {
                                if( (data[j].m_time.second > data[i].m_time.second ) )
                                {
                                    swap = TRUE;
                                }
                                else if(  (data[j].m_time.second == data[i].m_time.second ) )
                                {
                                    if( (data[j].m_time.fraction > data[i].m_time.fraction ) )
                                    {
                                        swap = TRUE;
                                    }
                                    else if(  (data[j].m_time.fraction == data[i].m_time.fraction ) )
                                    {
                                        //nothing to do
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if( swap == TRUE )
            {
                tmpData = data[i];
                data[i] = data[j];
                data[j] = tmpData;
            }
        }
    }
    TRACE("After\n"); 
    PrintTime( data );
}

BOOL CAlignerApp::InitInstance( void )
{
    if(!m_singleton.Create("Aligner308Unique"))
	{
		return FALSE;
	}

	if(CheckExpired())
	{
		::AfxMessageBox( _T("Aligner 308 has expired.") ) ;
		return FALSE;
	}


    InitCommonControls() ;
    CWinApp::InitInstance() ;
    if (!AfxOleInit())
    {
        return FALSE ;
    }

    // Initialize GDI+.    
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    CString appName = IsAligner202Enabled() ? _T("Aligner 202") : _T("Aligner 308");

    AfxEnableControlContainer() ;
    if (NULL != m_pszAppName)
    {
        delete m_pszAppName ;
    }
    m_pszAppName = _tcsdup( appName ) ;

    Registry reg;  
    reg.m_strAppName = appName + " MK3";
    m_DAUPresent = FALSE;
  
    int k = reg.GetIntegerValue("DAU\\HSC\\k", 5);
    DAU::GetDAU().m_dataCompensator.SetK(k);    

    CString str = reg.GetStringValue("DAU\\HSC\\MaxDataDiff", "0.04");
    double maxDiff = atof(str);
    DAU::GetDAU().m_dataCompensator.SetMaxDataDiff(maxDiff);
    //
    

//  SetRegistryKey( _T("Schill Reglerteknik AB") ) ;
  LoadStdProfileSettings( 0 ) ;
  SetRangeZeroToRangeA202( TRUE );
  SetLiveGraphViewActivated( FALSE );
  SetLiveDataViewActivated( FALSE );
  SetTiltAndFlatnessFoundationTestEnabled( TRUE );
  CSingleDocTemplate* pDocTemplate ;

  pDocTemplate = new CSingleDocTemplate(
    IsAligner202Enabled() == TRUE ? IDR_MAINFRAME_A202 : IDR_MAINFRAME,
    RUNTIME_CLASS(CAlignerDoc),
    RUNTIME_CLASS(CMainFrame),
    RUNTIME_CLASS(CAlignerView)) ;
  AddDocTemplate( pDocTemplate ) ;
  CCommandLineInfo cmdInfo ;
  ParseCommandLine( cmdInfo ) ;

  if (!ProcessShellCommand( cmdInfo ))
  {
    return FALSE ;
  }

  if( TimeCheck::IsValidTime() == FALSE )
  {
    return FALSE;
  }

  CView * pActiveView = ((CFrameWnd*) m_pMainWnd) -> GetActiveView();
  m_pAlignerView = pActiveView;

  if( IsAligner202Enabled() == TRUE )
  {
    m_pLiveDataView = static_cast<CView *>(new LiveDataViewA202) ;
  }
  else
  {
    m_pLiveDataView = static_cast<CView *>(new CLiveDataView) ;
  }
  m_pGraphView = static_cast<CView *>(new CGraphView) ;
  m_pSystemConfigurationView = static_cast<CView *>(new SystemConfigurationView) ;
  CDocument* pCurrentDoc = (static_cast<CFrameWnd *>(m_pMainWnd)) -> GetActiveDocument() ;
  m_pAzimuthAlignWizard = NULL;
  m_pAzimuthVerificationWizard = NULL;

  g_AlignerData.Init() ;
  SerialPortSet::Init() ;  
    
 
    if (0)//!VerifyDatabase())
    {
        return FALSE ;
    }
      
  //  if (!Features::CheckCPU())
//     {
//         ::AfxMessageBox( _T("Unable to verify Aligner Hardware!") ) ;
//         return FALSE ;
//     }

    CString connectionString = reg.GetStringValue( "ConnectionString", "" ) ;
	ReportManager::dbPath = connectionString;
	if(DBInterface::Instance()->Open(connectionString) != 0)
    {

    }
 
	SysSetup->LoadProjectFromRegistry();

    CCreateContext newContext ;
    newContext.m_pNewViewClass = NULL ;
    newContext.m_pNewDocTemplate = NULL ;
    newContext.m_pLastView = NULL ;
    newContext.m_pCurrentFrame = NULL ;
    newContext.m_pCurrentDoc = pCurrentDoc ;
    CRect rect( 0, 0, 0, 0 ) ;

    UINT viewID = AFX_IDW_PANE_FIRST + 1 ;
    m_pLiveDataView -> Create( NULL, "AnyWindowName", WS_CHILD, rect, m_pMainWnd, viewID, &newContext ) ;
    m_pLiveDataView -> SendMessage( WM_INITIALUPDATE, 0, 0 ) ;

    viewID = AFX_IDW_PANE_FIRST + 2 ;
    m_pGraphView -> Create( NULL, "AnyWindowName", WS_CHILD, rect, m_pMainWnd, viewID, &newContext ) ;
    m_pGraphView -> SendMessage( WM_INITIALUPDATE, 0, 0 ) ;

    viewID = AFX_IDW_PANE_FIRST + 3 ;
    m_pSystemConfigurationView->Create( NULL, "AnyWindowName", WS_CHILD, rect, m_pMainWnd, viewID, &newContext ) ;
    m_pSystemConfigurationView->SendMessage( WM_INITIALUPDATE, 0, 0 ) ;

    m_pMainWnd -> ShowWindow( SW_SHOWMAXIMIZED ) ;
    m_pMainWnd -> UpdateWindow() ;

    m_AlignmentMode = UndefinedMode ;
    m_HelpEnabled = TRUE;
    m_RangeZeroToRangeA202 = TRUE;

    CString title;

    if( IsAligner202Enabled() == TRUE )
    {
        SetMRad( TRUE );
        title.LoadString( IDS_ALIGNER_202_TITLE ) ;
    }
    else
    {
        title.LoadString( IDS_ALIGNER_308_TITLE ) ;
    } 
        
    m_pMainWnd -> SetWindowText( title );
    m_pMainWnd -> SetActiveWindow() ;
    SwitchView( m_pSystemConfigurationView ); 

    CString installationDirectory = reg.GetStringValue("Directory", "").TrimRight("\\");
    if (0 >= installationDirectory.GetLength())
    {
        ::AfxMessageBox( _T("Unable to retrive installation directory."), MB_ICONERROR ) ;
        return FALSE ;
    }
  
    string base = installationDirectory + "\\Reports\\";

    //SetReportDirectory( base );
    CResultTable rs;
    rs.SetReportFolder( base ) ;

    m_progressWnd.Create(ProgressWnd::IDD) ;

    DAU::GetDAU().m_pParent = m_pMainWnd;
    DAU::GetDAU().m_dauDataPro.Init();  
    DAU::GetDAU().m_comThr.SetProgressWnd(&m_progressWnd);        

#if defined(DAU_SIMULATION)
  AfxBeginThread( RUNTIME_CLASS(DAUSimulationThread) ) ;
#endif
  return TRUE ;
}

// TODO: Kludge ensure that all allocated windows are destroyed!
int CAlignerApp::ExitInstance( void )
{  
    DAU::GetDAU().StopDAUComThread();
    Database::Close() ;  
    SerialPortSet::Exit() ;
    SysSetup->SaveProjectPathToRegistry() ;
    return TRUE ;
}

int Glob_i ;
BOOL OK ;
HWND Handle ;
HINSTANCE HinstRes ;

void CAlignerApp::VersionDbMerge( void )
{
	HWND Handle ;
	HINSTANCE HinstRes ;
	int Cnt1 = 0 ;
	int I1 ;
	//LPOFSTRUCT lpReOpenBuff ;
	CString ExePath, ExeFile, MergeRunningFile, MergeReadyFile ;

	ExePath = SysSetup->GetExecutablePath() ;
	ExeFile = ExePath + "Supplement\\VersionDbMerge.exe" ;
	MergeRunningFile = ExePath + "MergeRunning.txt" ;
//	MergeRunningFile = "C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\\MergeRunning.txt" ;
	MergeReadyFile = ExePath + "MergeReady.txt" ;
//	MergeReadyFile = "C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\\MergeReady.txt" ;

	OK = DeleteFile(LPCTSTR(MergeRunningFile));
	OK = DeleteFile(LPCTSTR(MergeReadyFile));
	HinstRes = ShellExecute(Handle, "open", ExeFile, NULL, NULL, SW_SHOWNORMAL) ;
	while (true) {
	  Sleep(100) ;
		I1 = _open((LPCTSTR)MergeRunningFile, _O_RDONLY) ;
		if (I1 != -1) {
			_close(I1) ;
			break ;
		}
//***  If DB Merge program is not present or if merge is made
//***    the MergeRunningFile will not be seen by this code. The
//***    timeout below makes the program continue.
		if (Cnt1++ > 100)	{			// 10 s
			return ;
		}
	}
	while (true) {
		Sleep(100) ;
		I1 = _open((LPCTSTR)MergeReadyFile, _O_RDONLY) ;
		if (I1 != -1) {
			_close(I1) ;
			break ;
		}
	}
	Sleep(1000) ;		// Make sure DB Merge is closed and all tables as well
	OK = DeleteFile(LPCTSTR(MergeRunningFile));
}
BOOL CAlignerApp::VerifyDatabase( void )
{
  //*** Verify/fix DB updated for this SW version
  Registry reg;
  if(reg.GetIntegerValue("VerifyDB", 1) != 0)
  {
    VersionDbMerge () ;
  }
  
  /*HRESULT hResult = Database::Open() ;
  if (S_OK != hResult)
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
  }
  else
  {*/
		// Call Delphi check for TiltAndFlatnessFoundationTest tables
//		Glob_i = Triple(9) ;
//		Glob_i = TTriple::fnTriple(9) ;
//		Glob_i = fnDouble(9) ;
//char *Path ;
//		HinstRes = ShellExecute(Handle, "open", "C:\Documents and Settings\Default User\Start Menu\Programs\Accessories\notepad.exe", NULL, NULL, SW_SHOWNORMAL) ;
//		HinstRes = ShellExecute(Handle, "open", "notepad.exe", NULL, NULL, SW_SHOWNORMAL) ;
//		HinstRes = ShellExecute(Handle, "open", "C:\\Documents and Settings\\Staffan\\My Documents\\Staffan\\Borland\\A308\\dpr_A308Test.exe", NULL, NULL, SW_SHOWNORMAL) ;
//		HinstRes = ShellExecute(Handle, "open", "..\\Delphi\\dpr_A308Test.exe", NULL, NULL, SW_SHOWNORMAL) ;
//		Path = "C:\\A308\\dpr_A308Test.exe" ;
		//HinstRes = ShellExecute(Handle, "open", Path, NULL, NULL, SW_SHOWNORMAL) ;
		//HinstRes = ShellExecute(Handle, "open", "C:\\A308\\dpr_A308Test.exe", NULL, NULL, SW_SHOWNORMAL) ;
		//HinstRes = ShellExecute(Handle, "open", "dpr_A308Test.exe", NULL, NULL, SW_SHOWNORMAL) ;
		
    //check if the database contains tabels for the TiltAndFlatnessFoundationTest
/*	
		(*** The check below should not be needed as the "VersionDbMerge" procedure   ***)
		(***   above updates the database to the latest version												***)

		HRESULT hResult;
		HistoryTable historyTable;
    DBTIMESTAMP currentTime ;
    CTime::GetCurrentTime().GetAsDBTIMESTAMP( currentTime ) ;
		SET_DB_VALUE_NOCHECK(historyTable, time, currentTime) ;
		SET_DB_STRING(historyTable,project,(LPCTSTR)_T(" ")) ;
		SET_DB_STRING(historyTable,operator,(LPCTSTR)_T(" ")) ;
		SET_DB_STRING(historyTable,place,(LPCTSTR)_T(" ")) ;
		SET_DB_STRING(historyTable,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
		SET_DB_VALUE(historyTable,latitude,SysSetup->GetLatitude()) ;
		SET_DB_STRING(historyTable,angularDefinitionPart1,(LPCTSTR)_T(" ")) ;
		SET_DB_STRING(historyTable,angularDefinitionPart2,(LPCTSTR)_T(" ")) ;
		if( S_OK == ::OpenTable( historyTable ) )
		{
			hResult = S_OK;
		}

		LastCounter lastID ;		
		if( S_OK == ::OpenTable( lastID ) )
		{
			hResult = S_OK;
		}
		if( S_OK == lastID.MoveFirst() )
		{
			hResult = S_OK;
		}

    TiltAndFlatnessFoHistoryTable FoHistoryTable ;
    SET_DB_VALUE(FoHistoryTable,historyID,lastID.m_id ) ;
    SET_DB_STRING(FoHistoryTable,ship,(LPCTSTR)_T(" ") ) ;
    SET_DB_VALUE(FoHistoryTable,timeConstant,0) ;
    SET_DB_VALUE(FoHistoryTable,numberOfMeasurement,0) ;
    SET_DB_STRING(FoHistoryTable,reference,_T(" ")) ;
    SET_DB_STRING(FoHistoryTable,comment,(LPCTSTR)_T(" ")) ;
	  SET_DB_STRING(FoHistoryTable,measuredUnit,(LPCTSTR)_T(" ")) ;
    SET_DB_VALUE(FoHistoryTable,IndexArmLength,0) ;

    if( S_OK == ::OpenTable(FoHistoryTable) )
		{
			hResult = S_OK;
			SetTiltAndFlatnessFoundationTestEnabled( TRUE );
		}
		else
		{
			SetTiltAndFlatnessFoundationTestEnabled( FALSE );
		}

		hResult = S_OK;      // always return S_OK 
*/
//	SetTiltAndFlatnessFoundationTestEnabled( TRUE );
 // }

  return 0;
}

class CAboutDlg : public CDialog
{
public:
  CAboutDlg( CWnd *pParent = NULL ) ;
  enum { IDD = IDD_ABOUTBOX } ;
protected:
  virtual void DoDataExchange( CDataExchange* pDX ) ;
protected:
  DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_version;
    CStatic m_imageCtrl;
} ;

CAboutDlg::CAboutDlg( CWnd *pParent ) : CDialog( CAboutDlg::IDD, pParent )
, m_version(_T(""))
{
  // Empty
}

void CAboutDlg::DoDataExchange( CDataExchange *pDX )
{
	CDialog::DoDataExchange( pDX ) ;
	DDX_Text(pDX, IDC_APP_VERSION, m_version);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BYTE buf[MAX_PATH];
	DWORD d = GetModuleFileName(NULL, (LPSTR)buf, MAX_PATH);
	CString str(buf);
	DWORD len = GetFileVersionInfoSize(str, NULL);
	VS_FIXEDFILEINFO *pInfo;
	if(len > 0)
	{
		BYTE *pData = new BYTE[len];
		BOOL res = GetFileVersionInfo(str, NULL, len, pData);
		UINT size;
		res = VerQueryValue(pData, "\\", (LPVOID*)&pInfo, &size);

		int nFileVersionMajor = HIWORD(pInfo->dwFileVersionMS);
		int nFileVersionMinor = LOWORD(pInfo->dwFileVersionMS);
		int nFileBuildMajor = HIWORD(pInfo->dwFileVersionLS);
		int nFileBuildMinor = LOWORD(pInfo->dwFileVersionLS);

		m_version.Format("Version %d.%d.%d",nFileVersionMajor, nFileVersionMinor, nFileBuildMajor);
		delete pData;
	}


    CRect rect;
    GetClientRect(&rect);
    //m_imageCtrl.MoveWindow(0,0,rect.Width(),rect.Height()-130);

    m_imageCtrl.Create(_T("my static"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE|SS_REALSIZEIMAGE, CRect(0,0,rect.Width(),rect.Height()-25), this);

    HBITMAP hb = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.IsAligner202Enabled() ? IDB_202_SPLASH : IDB_SPLASH_MK3));
    m_imageCtrl.SetBitmap(hb);

    //::SetWindowPos(m_imageCtrl.m_hWnd, HWND_BOTTOM, 0,0,0,0,SWP_NOSIZE);
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//***  1.14
void CAlignerApp::OnFileOpen( void )
{

	ProjectOpenDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SystemSetup::Instance()->SetProject(dlg.m_selectedProj);

		CFrameWnd *p = (CFrameWnd*)m_pMainWnd;
		CAlignerDoc *d = (CAlignerDoc*)p->GetActiveDocument();
		d->OpenConfig();
		d->SetPathName(dlg.m_selectedProj.m_projectName);

		g_AlignerData.Init();

		TRACE("Proj: %s\n",dlg.m_selectedProj.m_projectName);
		
	}

	return;

//***  Tried to initialize the OpenDialog with ProjectPath but failed
//CString Path ;
//LPCTSTR LPath ;
//OpenFileDialog openFileDialog1 ;
//	
//	// openFileDialog1 = new OpenFileDialog();
//
//	Path = SysSetup->GetProjectPath2() ;			//	Path without ProjName
//	LPath = Path ; 
//	FolderBrowserDialog1->RootFolder = "C:\\" ;
//
//	I11 = _chdir(/*const char *dirname */ "C:\\");			//	Change Dir
//	openFileDialog1.InitialDirectory = "c:\\" ;
//  //	SetCurrentDirectory("C:\\") ;

  //CFrameWnd *p = (CFrameWnd*)m_pMainWnd;
  //CAlignerDoc *d = (CAlignerDoc*)p->GetActiveDocument();
  //if( d != NULL )
  //  d->SetPathName("c:\\",0);

//	CAlignerApp::d->SetPathName( "C:\\" ) ;
	//CDocument::SetPathName ("C:\\", False) ;


	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString title = "Open";	

	//dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	
	 //append the "*.*" all files filter
	CString allFilter;
	//VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	CString fileName;
	CString path = SysSetup->GetProjectPath();
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrInitialDir = path;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	

	INT_PTR nResult = dlgFile.DoModal();
	//fileName.ReleaseBuffer();
	if(nResult == IDOK)
		AfxGetApp()->OpenDocumentFile(fileName);

	//CWinApp::OnFileOpen() ;
	
	////***  1.14 Saving proj path previously made only at program exit
	//	Instead of here, it is made in the routine 
	//	CAlignerApp::OpenDocumentFile as the open file name is
	//	conveniently available there.

	//SysSetup->SaveProjectPathToRegistry() ;
	////***  1.14 End
}

void CAlignerApp::OnAppAbout( void )
{
  CAboutDlg aboutDlg ;
  aboutDlg.DoModal() ;
}

CView *CAlignerApp::SwitchView( CView *pNewView )
{
  CView* pActiveView = static_cast<CFrameWnd*>(m_pMainWnd)->GetActiveView() ;
  UINT temp = ::GetWindowLong( pActiveView->m_hWnd, GWL_ID ) ;
  ::SetWindowLong( pActiveView->m_hWnd, GWL_ID, ::GetWindowLong( pNewView->m_hWnd, GWL_ID) ) ;
  ::SetWindowLong( pNewView->m_hWnd, GWL_ID, temp ) ;
  pActiveView->ShowWindow( SW_HIDE ) ;
  pNewView->ShowWindow( SW_SHOW ) ;
  ((CFrameWnd*)m_pMainWnd)->SetActiveView( pNewView ) ;
  (static_cast<CFrameWnd *>(m_pMainWnd))->RecalcLayout() ;
  pNewView->Invalidate() ;
  return pActiveView ;
}

BOOL RetriveDataDirectory( CString &directoryName )
{
  BOOL isValidName = FALSE ;
  LPITEMIDLIST pidlRoot = NULL ;
  LPITEMIDLIST pidlSelected = NULL ;
  BROWSEINFO bi = {0} ;
  LPMALLOC pMalloc = NULL ;
  TCHAR folderName [MAX_PATH] ;

  if (S_OK != SHGetMalloc( &pMalloc ))
  {
    // TODO: This is a serious error and should cause
    // a warning and application termination.
    // It _should_ never happen but ...
    return FALSE ;
  }
  bi.hwndOwner = *(AfxGetMainWnd()) ;
  bi.pidlRoot = NULL ;
  bi.pszDisplayName = folderName ;
  bi.lpszTitle = "Choose a folder to read Aligner 308 v1 data from" ; // TODO add as a resource string.
  bi.ulFlags = 0 ;
  bi.lpfn = NULL ;
  bi.lParam = 0 ;
  pidlSelected = SHBrowseForFolder( &bi ) ;

  if (pidlRoot)
  {
    pMalloc->Free( pidlRoot ) ;
  }

  if (pidlSelected)
  {
    SHGetPathFromIDList( pidlSelected, folderName ) ;
    pMalloc -> Free( pidlSelected ) ;
    directoryName = folderName ;
    isValidName = TRUE ;
  }
  pMalloc -> Release() ;
  return isValidName ;
}

/**
* @brief Determine if a path represents a valid configuration directory.
*
* A directory is considered a valid configuration directory
* if it contains a readable file name LAT.DAT.
*
* @param directoryName - a CString reference holding a pathname
*        to a directory to query.
*
* @returns TRUE if the directory matches validation criteria or
*          FALSE if not.
*/
BOOL IsValidDataDirectory( const CString &directoryName )
{
  CFile file ;
  if (file.Open( directoryName, CFile::modeRead ))
  {
    file.Close() ;
    return TRUE ;
  }
  return FALSE ;
}

void CAlignerApp::OnUtilitiesDataconversion()
{
  CString directoryName ;
  if (!RetriveDataDirectory( directoryName ))
  {
    return ;
  }

  if (!IsValidDataDirectory( directoryName ))
  {
    AfxMessageBox( IDS_INVALID_DATA_DIRECTORY, MB_OK | MB_ICONINFORMATION ) ;
    return ;
  }
}

void CAlignerApp::OnUpdateUtilitiesDataconversion(CCmdUI *pCmdUI)
{
  pCmdUI -> Enable( FALSE ) ; 
}

void CAlignerApp::OnAlignmentTiltalignment()
{
	CheckAndResetMeasurementSetups( TiltAlignmentMode );
	m_AlignmentMode = TiltAlignmentMode;
	CTiltAlignmentWizard TiltAlignmentWizard ;
	int res = TiltAlignmentWizard.DoModal() ;

	if (res == IDABORT)
	{
		//User want to run a Gun Adapter Test
		OnAlignmentGunAdapterTiltCheck();
	}

	TRACE("res:%d\n",res);
}

void CAlignerApp::OnUpdateAlignmentTiltalignment( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}

void CAlignerApp::OnAlignmentAzimuthAlignment()
{
	/*InfoDlg dlg;

	dlg.SetTitle("Notification");
	dlg.SetInfo("It is recommended that the 'Sensor Validation Test' is performed prior to the 'Azimuth Alignment', to eliminate azimuth errors. \r\n\r\nDo you wish to run 'Sensor Validation Test'?");
	dlg.SetYesNoMode(TRUE);
	int result = dlg.DoModal();
		
	if (result == IDOK)
	{
		CAlignerDoc *pDoc = (CAlignerDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
		pDoc->OnCalibrationDailyazimutherrortest();
		//AfxMessageBox("Do meassure");
		return;
	}*/

	CheckAndResetMeasurementSetups( AzimuthAlignMode );
	m_pAzimuthAlignWizard = static_cast<CPropertySheet *>(new CAzimuthAlignWizard ) ;
	int res = m_pAzimuthAlignWizard->DoModal();
	delete m_pAzimuthAlignWizard;
	m_pAzimuthAlignWizard = NULL;

	if (res == RUN_SVT)
	{
		CAlignerDoc *pDoc = (CAlignerDoc*)((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
		pDoc->OnCalibrationDailyazimutherrortest();
	}

	if (res == RUN_GATC)
	{
		//User want to run a Gun Adapter Test
		OnAlignmentGunAdapterTiltCheck();
	}

}

void CAlignerApp::OnUpdateAlignmentAzimuthAlignment( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlignAzimuth() ) ;  
}

void CAlignerApp::OnAlignmentGyroPerformanceTest()
{
  CheckAndResetMeasurementSetups( GyroPerformanceTestMode );
  CGyroPerformanceTestWizard GyroPerformanceTestWizard ;
  GyroPerformanceTestWizard.DoModal() ;  
}

void CAlignerApp::OnUpdateAlignmentGyroPerformanceTest( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}

void CAlignerApp::OnAlignmentGunAdapterTiltCheck()
{
  CheckAndResetMeasurementSetups( GunAdapterTiltCheckMode );
  CGunAdapterTiltCheckDlg GunAdapterTiltCheckDlg;
  GunAdapterTiltCheckDlg.DoModal();
}

void CAlignerApp::OnUpdateAlignmentGunAdapterTiltCheck( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}

void CAlignerApp::OnAlignmentTiltFlatnessTest( void )
{
  CheckAndResetMeasurementSetups( TiltFlatnesstestMode );
  CTiltFlatnessTestWizard TiltFlatnessTestWizard ;
  TiltFlatnessTestWizard.DoModal() ;
}

void CAlignerApp::OnUpdateAlignmentTiltFlatnessTest( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}
void CAlignerApp::OnAlignmentTiltFlatnessTestPlatform( void )
{
  CheckAndResetMeasurementSetups( TiltFlatnesstestMode );
  CTiltFlatnessTestWizard TiltFlatnessTestWizard ;
  int res = TiltFlatnessTestWizard.DoModal() ;

  if (res == IDABORT)
  {
	  //User want to run a Gun Adapter Test
	  OnAlignmentGunAdapterTiltCheck();
  }
}

void CAlignerApp::OnUpdateAlignmentTiltFlatnessTestPlatform( CCmdUI *pCmdUI )
{
  if (NULL != pCmdUI -> m_pSubMenu)
  {
    pCmdUI -> m_pMenu -> EnableMenuItem( pCmdUI -> m_nIndex,
      MF_BYPOSITION | (GetCanAlign() ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)) ) ;
  }
  else
  {
    pCmdUI -> Enable( GetCanAlign() ) ;
  }
}

void CAlignerApp::OnAlignmentTiltFlatnessTestFoundation( void )
{
  CheckAndResetMeasurementSetups( TiltFlatnesstestFoundationMode );
  CTiltFlatnessTestWizard TiltFlatnessTestWizard ;
  TiltFlatnessTestWizard.DoModal() ;
}
void CAlignerApp::OnUpdateAlignmentTiltFlatnessTestFoundation( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() && IsTiltAndFlatnessFoundationTestEnabled() ) ;
}

void CAlignerApp::OnAlignmentTiltVerificationHorizonTest()
{
  CheckAndResetMeasurementSetups( TiltVerHorizonMode );
  CTiltVerificationWizard TiltVerificationWizard( FALSE ) ;
  TiltVerificationWizard.DoModal() ;
}

void CAlignerApp::OnUpdateAlignmentTiltVerificationHorizonTest(CCmdUI *pCmdUI)
{
  if (NULL != pCmdUI -> m_pSubMenu)
  {
    pCmdUI -> m_pMenu -> EnableMenuItem( pCmdUI -> m_nIndex,
      MF_BYPOSITION | (GetCanAlign() ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)) ) ;
  }
  else
  {
    pCmdUI -> Enable( GetCanAlign() ) ;
  }
}

void CAlignerApp::OnAlignmentTiltVerificationAirTargetTest()
{
  CheckAndResetMeasurementSetups( TiltVerAirTargetMode );
  CTiltVerificationWizard TiltVerificationWizard( TRUE ) ;
  TiltVerificationWizard.DoModal() ;
}

void CAlignerApp::OnUpdateAlignmentTiltVerificationAirTargetTest(CCmdUI *pCmdUI)
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}

void CAlignerApp::OnAlignmentAzimuthVerificationBenchmarkTest()
{
  CheckAndResetMeasurementSetups( AzimuthVerBenchmarkMode );

  m_pAzimuthVerificationWizard = static_cast<CPropertySheet *>(new CAzimuthVerificationWizard ) ;
  m_pAzimuthVerificationWizard->DoModal();
  delete m_pAzimuthVerificationWizard;
  m_pAzimuthVerificationWizard = NULL;
}

void CAlignerApp::OnUpdateAlignmentAzimuthVerificationBenchmarkTest(CCmdUI *pCmdUI)
{
  if (NULL != pCmdUI -> m_pSubMenu)
  {
    pCmdUI -> m_pMenu -> EnableMenuItem( pCmdUI -> m_nIndex,
      MF_BYPOSITION | (GetCanAlignAzimuth() ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)) ) ;
  }
  else
  {
    pCmdUI -> Enable( GetCanAlignAzimuth() ) ;  
  }
}

void CAlignerApp::OnAlignmentAzimuthVerificationGyroStabTest()
{
  CheckAndResetMeasurementSetups( AzimuthVerGyroStabMode );
  CAzimuthVerificationWizard AzimuthVerificationWizard( FALSE ) ;
  AzimuthVerificationWizard.DoModal() ;
}

void CAlignerApp::OnUpdateAlignmentAzimuthVerificationGyroStabTest(CCmdUI *pCmdUI)
{
  pCmdUI -> Enable( GetCanAlignAzimuth() ) ;  
}

BOOL CAlignerApp::GetCanCalibrate( void ) const
{
  if (IsLiveDataViewActivated())
  {
    return FALSE ;
  }
  if (SYSTEM_SETUP_MODE_CALIBRATION != SysSetup->GetMode())
  {
    return FALSE ;
  }
  InitGlobalSensorStrings( FALSE );
  InitSystemSetupData();

  return DAU::GetDAU().IsOpen() ;
}

BOOL CAlignerApp::GetCanAlign( void ) const
{
  if (IsLiveDataViewActivated())
  {
    return FALSE ;
  }
  if( (DAU::GetDAU().IsOpen() && (0 < SysSetup->GetShipName().GetLength()) && (0 < SysSetup->GetOperatorName().GetLength()) && (SYSTEM_SETUP_MODE_ALIGNMENT == SysSetup->GetMode()) ) == FALSE )
  {
    return( FALSE );
  }

  if( InitGlobalSensorStrings( FALSE ) == FALSE )
  {
    return( FALSE );
  }
  InitSystemSetupData();

  if( CheckSystemSetupData() == FALSE )
  {
    return( FALSE );
  }  
  return( TRUE );
}

BOOL CAlignerApp::GetCanAlignAzimuth( void ) const
{
  if( ( GetCanAlign() == TRUE ) && ( DAU::GetDAU().GetEnableAzimuthMeasurement() == TRUE ) )
  {
    return( TRUE );
  }
  return( FALSE );
}

void CAlignerApp::OnUtilitiesLivedata( void )
{
    CheckAndResetMeasurementSetups( LiveDataMode );
    SetUsedAsSelectedChannels();//ClearSelectedChannels();
    DAU::GetDAU().SetOverrangeDetectionActive( FALSE );
    
    ::ResetAllFilters( &(DAU::GetDAU()) ) ;
    DAU::GetDAU().m_comThr.SetSamplingMode(FQ_MODE_305);			
    
    CLiveDataView *pView = dynamic_cast<CLiveDataView *>(m_pLiveDataView);
    if(pView)
    {
       pView->SetShowOnlySelected( FALSE ) ;
       pView->ResetFilters( 0.0 ) ;
    }
        
    SwitchView( m_pLiveDataView );  
}

void CAlignerApp::OnUpdateUtilitiesLivedata( CCmdUI *pCmdUI )
{
  if( IsAligner202Enabled() == TRUE )
  {
    pCmdUI -> Enable( GetCanAlign() && SysSetup->GetCanConfigure() && DAU::GetDAU().IsOpen() && (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
  }
  else
  {
    pCmdUI -> Enable( SysSetup->GetCanConfigure() && DAU::GetDAU().IsOpen() && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
  }
}

void CAlignerApp::OnUtilitiesLivegraph( void )
{
  CheckAndResetMeasurementSetups( LiveDataGraphMode ) ;
  m_AlignmentMode = LiveDataGraphMode ;
  SwitchView( m_pSystemConfigurationView );
  CSetupLiveDataGraphDlg setupLiveDataGraphDlg ;
  setupLiveDataGraphDlg.DoModal() ;
}

void CAlignerApp::OnUpdateUtilitiesLivegraph( CCmdUI *pCmdUI )
{
  if( IsAligner202Enabled() == TRUE )
  {
    pCmdUI -> Enable( GetCanAlign() && SysSetup->GetCanConfigure() && DAU::GetDAU().IsOpen() && (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
  }
  else
  {
    pCmdUI -> Enable( GetCanAlign() && SysSetup->GetCanConfigure() && DAU::GetDAU().IsOpen() ) ;
  }
}

void CAlignerApp::OnAlignmentCommonFlatTest( void )
{
  CheckAndResetMeasurementSetups( CommonFlatTestMode );
  m_AlignmentMode = CommonFlatTestMode;
  CTiltAlignmentWizard TiltAlignmentWizard( TRUE ) ;
  TiltAlignmentWizard.DoModal() ;
 
  CAlignerDoc *pDoc = static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(theApp.m_pMainWnd)->GetActiveDocument());
  pDoc->LoadSensorCalibration();

}

void CAlignerApp::OnUpdateAlignmentCommonFlatTest( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanAlign() ) ;
}

const char ALIGNER202_ENABLED_REGISTER_SECTION [] = "" ;
const char ALIGNER202_ENABLED_REGISTER_NAME []    = "Aligner202Enabled" ;
const BOOL  ALIGNER202_ENABLED_DEFAULT_VALUE       = FALSE ;

/*
void CAlignerApp::AddToDocument( LONG id )
{
  static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(m_pMainWnd) -> GetActiveDocument()) -> AddMeasurment( id ) ;
}

void CAlignerApp::RemoveFromDocument( LONG id )
{
  static_cast<CAlignerDoc *>(static_cast<CFrameWnd *>(m_pMainWnd) -> GetActiveDocument()) -> RemoveMeasurment( id ) ;
}
*/
void CAlignerApp::OnUtilitiesDa( void )
{
  DAU::GetDAU().m_comThr.BeginDACTest(FULL_DAC_TEST);
//     if ()
//   {
//     ::AfxMessageBox( _T("AD/DA Loop Test Succesfull") ) ;
//   }
//   else
//   {
//     ::AfxMessageBox( _T("AD/DA Loop Test Failed!"), MB_ICONERROR ) ;
//   }
}

void CAlignerApp::OnUpdateUtilitiesDa( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( SYSTEM_SETUP_MODE_CALIBRATION == SysSetup->GetMode() ) ;
}

BOOL CAlignerApp::ReInit( void )
{
  DAU::GetDAU().Restart( TRUE );

  return( TRUE );
}
CString ExtractPath(CString CStringIn) 
{
	int Pos ;
	CString CStrTemp ;

	Pos = CStringIn.Find('.') ;
	if (Pos == -1)
		return(CStringIn) ;
	Pos = CStringIn.ReverseFind('\\') ;
	CStrTemp = CStringIn.Left(Pos) ;
	return(CStrTemp) ;
}

CString ExtractFatherDir(CString CStringIn) 
{
	int Pos ;
	CString CStrTemp ;

	Pos = CStringIn.ReverseFind('\\') ;
	CStrTemp = CStringIn.Left(Pos) ;
	return(CStrTemp) ;
}

//void ExtractPath(char *PathOut, /*const*/ char *FileNameIn) 
////***  1.14 
//{
//	CString CStrTemp ;
//	int  ChDot = '.' ;
//	int  ChBS = '\\' ;												//	a single '\' is interpreted as new line
//	int NoOfCh ;
//	char *pCh ;
//	char ChStrTemp[200] ;
//
//	pCh = strrchr( FileNameIn, ChDot ) ;			//	Find last "." in string
//	//if (pCh == NULL) {
//	//	PathOut = FileNameIn ;									//	Already a path ; no file name found
//	//	return ;
//	//}
//	pCh = strrchr( FileNameIn, ChBS ) ;				//	Find last "\" in string
//  NoOfCh = (int)(pCh - FileNameIn + 1) - 2 ;
////	strncpy(pCh, FileNameIn, NoOfCh) ;
//	strncpy((char*)ChStrTemp, FileNameIn, NoOfCh) ;
//	//CStrTemp = (char*)ChStrTemp ;
//	pCh = ChStrTemp ;
//	PathOut = ChStrTemp ;
////	PathOut = pCh ;
//}

CDocument* CAlignerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
  // TODO: Add your specialized code here and/or call the base class

  CFrameWnd *p = (CFrameWnd*)m_pMainWnd;
  CAlignerDoc *d = (CAlignerDoc*)p->GetActiveDocument();
	if( d != NULL )
  {
    CString newPathName = lpszFileName;
    CString currentPathName = d->GetPathName();
	
	//***  1.14 Saving previously made only at program exit
		CString ProjPathDir ;

		ProjPathDir = ExtractPath(newPathName) ;
		if (ProjPathDir != newPathName)						//	File name found ?
			ProjPathDir = ExtractFatherDir(ProjPathDir) ;
		SysSetup->SetProjectPath(ProjPathDir) ;
		SysSetup->SaveProjectPathToRegistry() ;
	//***  1.14 End
	
    //if( d->m_probeError == TRUE && newPathName == currentPathName )
    if( newPathName == currentPathName )
    {
      d->SetPathName("c:\\",0);
    }
  }
  return CWinApp::OpenDocumentFile(lpszFileName);
}

void CAlignerApp::RenameDocument( CString newName )
{
  CFrameWnd *p = (CFrameWnd*)m_pMainWnd;
  CAlignerDoc *d = (CAlignerDoc*)p->GetActiveDocument();
  if( d != NULL )
  {
    CString currentPathName = d->GetTitle();
    d->SetTitle( newName );
    //d->SetPathName( newName,0 );
  }
}

