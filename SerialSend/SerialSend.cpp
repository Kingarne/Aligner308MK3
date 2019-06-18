#include "stdafx.h"
#include "SerialPort.h"
#include "SerialSend.h"
#include "Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Application, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

Application::Application( void )
{
}

Application theApp ;

BOOL Application::InitInstance( void )
{
	InitCommonControls() ;
	__super::InitInstance() ;
	AfxEnableControlContainer() ;
	SetRegistryKey( _T("Local AppWizard-Generated Applications") ) ;
  SerialPortSet::Init() ;
	Dialog dlg ;
	m_pMainWnd = &dlg ;
	dlg.DoModal() ;
	return FALSE ;
}
