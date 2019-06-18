#pragma once

#if defined(DAU_SIMULATION)

class DAUSimulationDialog : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(DAUSimulationDialog)
public:
	DAUSimulationDialog( DAUData *pData, BOOL *pErrorEvent, CWnd *pParent = NULL) ;
	virtual ~DAUSimulationDialog( void ) ;
private:
	enum { IDD = IDD_DAU_SIMULATION } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
protected:
    afx_msg void OnBnClickedOk( void ) ;
private:
    int m_rollJ1 ;
    int m_pitchJ1 ;
    int m_rollJ2 ;
    int m_pitchJ2 ;
    int m_rollJ3 ;
    int m_pitchJ3 ;
    int m_rollJ4 ;
    int m_pitchJ4 ;
    int m_rollJ5 ;
    int m_pitchJ5 ;
    int m_rollJ6 ;
    int m_pitchJ6 ;
    int m_rollJ7 ;
    int m_pitchJ7 ;
    int m_rollJ8 ;
    int m_pitchJ8 ;
    int m_rollJ9 ;
    int m_pitchJ9 ;
    DAUData *m_pData ;
    BOOL *m_pError ;
public:
  afx_msg void OnBnClickedError();
} ;

class DAUSimulationThread : public CWinThread
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(DAUSimulationThread)
protected:
	DAUSimulationThread( void ) ;
	virtual ~DAUSimulationThread( void ) ;
public:
	virtual BOOL InitInstance( void ) ;
	virtual int ExitInstance( void ) ;
private:
    DAUSimulationDialog m_dialog ;
} ;

#endif
