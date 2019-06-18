#include "stdafx.h"
#include "Aligner.h"
#include "DAU.h"
#include "DAUSimulationDialog.h"

#if defined(DAU_SIMULATION)

IMPLEMENT_DYNAMIC(DAUSimulationDialog, CDialog)

DAUSimulationDialog::DAUSimulationDialog(DAUData *pData, BOOL *pErrorEvent, CWnd *pParent) : CDialog(DAUSimulationDialog::IDD, pParent)
    , m_rollJ1(0), m_pitchJ1(0), m_rollJ2(0), m_pitchJ2(0), m_rollJ3(0), m_pitchJ3(0), m_rollJ4(0), m_pitchJ4(0), m_rollJ5(0), m_pitchJ5(0), m_rollJ6(0), m_pitchJ6(0), m_rollJ7(0), m_pitchJ7(0), m_rollJ8(0), m_pitchJ8(0), m_rollJ9(0), m_pitchJ9(0)
{
    m_pData = pData ;
    m_pError = pErrorEvent ;
}

DAUSimulationDialog::~DAUSimulationDialog( void )
{
}

void DAUSimulationDialog::DoDataExchange( CDataExchange *pDX )
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_J1_ROLL, m_rollJ1);
    DDX_Text(pDX, IDC_J1_PITCH, m_pitchJ1);
    DDX_Text(pDX, IDC_J2_ROLL, m_rollJ2);
    DDX_Text(pDX, IDC_J2_PITCH, m_pitchJ2);
    DDX_Text(pDX, IDC_J3_ROLL, m_rollJ3);
    DDX_Text(pDX, IDC_J3_PITCH, m_pitchJ3);
    DDX_Text(pDX, IDC_J4_ROLL, m_rollJ4);
    DDX_Text(pDX, IDC_J4_PITCH, m_pitchJ4);
    DDX_Text(pDX, IDC_J5_ROLL, m_rollJ5);
    DDX_Text(pDX, IDC_J5_PITCH, m_pitchJ5);
    DDX_Text(pDX, IDC_J6_ROLL, m_rollJ6);
    DDX_Text(pDX, IDC_J6_PITCH, m_pitchJ6);
    DDX_Text(pDX, IDC_J7_ROLL, m_rollJ7);
    DDX_Text(pDX, IDC_J7_PITCH, m_pitchJ7);
    DDX_Text(pDX, IDC_J8_ROLL, m_rollJ8);
    DDX_Text(pDX, IDC_J8_PITCH, m_pitchJ8);
    DDX_Text(pDX, IDC_J9_ROLL, m_rollJ9);
    DDX_Text(pDX, IDC_J9_PITCH, m_pitchJ9);
}

BEGIN_MESSAGE_MAP(DAUSimulationDialog, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(ID_ERROR, OnBnClickedError)
END_MESSAGE_MAP()

void DAUSimulationDialog::OnBnClickedOk( void )
{
    UpdateData() ;
    m_pData -> m_sensor [0].m_roll = m_rollJ9 ;
    m_pData -> m_sensor [0].m_pitch = m_pitchJ9 ;
    m_pData -> m_sensor [1].m_roll = m_rollJ8 ;
    m_pData -> m_sensor [1].m_pitch = m_pitchJ8 ;
    m_pData -> m_sensor [2].m_roll = m_rollJ7 ;
    m_pData -> m_sensor [2].m_pitch = m_pitchJ7 ;
    m_pData -> m_sensor [3].m_roll = m_rollJ6 ;
    m_pData -> m_sensor [3].m_pitch = m_pitchJ6 ;
    m_pData -> m_sensor [4].m_roll = m_rollJ5 ;
    m_pData -> m_sensor [4].m_pitch = m_pitchJ5 ;
    m_pData -> m_sensor [5].m_roll = m_rollJ4 ;
    m_pData -> m_sensor [5].m_pitch = m_pitchJ4 ;
    m_pData -> m_sensor [6].m_roll = m_rollJ3 ;
    m_pData -> m_sensor [6].m_pitch = m_pitchJ3 ;
    m_pData -> m_syncro [0].m_roll = m_rollJ2 ;
    m_pData -> m_syncro [0].m_pitch = m_pitchJ2 ;
    m_pData -> m_syncro [1].m_roll = m_rollJ1 ;
    m_pData -> m_syncro [1].m_pitch = m_pitchJ1 ;
}

IMPLEMENT_DYNCREATE(DAUSimulationThread, CWinThread)

DAUSimulationThread::DAUSimulationThread( void ) : m_dialog( DAU::GetDAU().GetSimulationDataPointer(), DAU::GetDAU().GetSimulationErrorPointer() )
{
}

DAUSimulationThread::~DAUSimulationThread( void )
{
}

BOOL DAUSimulationThread::InitInstance( void )
{
    m_dialog.Create( IDD_DAU_SIMULATION ) ;
    m_dialog.ShowWindow( SW_SHOW ) ;
	return TRUE ;
}

int DAUSimulationThread::ExitInstance( void )
{
	return CWinThread::ExitInstance() ;
}

BEGIN_MESSAGE_MAP(DAUSimulationThread, CWinThread)
END_MESSAGE_MAP()

void DAUSimulationDialog::OnBnClickedError()
{
  *m_pError = TRUE ;
}
#endif
