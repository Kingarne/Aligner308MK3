// $Id: LiveDataViewA202.cpp,v 1.7 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "LiveDataViewA202.h"
#include "Util.h"
#include "Type.h"
#include "Defines.h"
#include "UserMessages.h"
#include "ResultTable.h"
#include <fstream>
#include ".\livedataviewa202.h"


IMPLEMENT_DYNCREATE(LiveDataViewA202, CFormView)

LiveDataViewA202::LiveDataViewA202()
	: CFormView(LiveDataViewA202::IDD), DataRecepient( this )
{
    m_newDataArrived = FALSE;
    m_showOnlySelected = FALSE ;
    m_pSource = NULL ;
    m_indexMap[D1] = 9;
    m_indexMap[D2] = 8;
    m_indexMap[D3] = 7;
    m_indexMap[D4] = 6;
    m_indexMap[A1] = 5;
    m_indexMap[B1] = 4;
    m_indexMap[C1] = 3;
    m_indexMap[A2] = 2;
    m_indexMap[B2] = 1;
    m_indexMap[C2] = 0;

    m_liveDataDir = ".\\";
    m_liveDataFilename = "LiveData.txt";
}

LiveDataViewA202::~LiveDataViewA202()
{
}

void LiveDataViewA202::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_CHANNEL_LIVE_GRID, m_channelLiveGrid);        
    DDX_Control(pDX, IDC_CHANNEL_LIVE_DIG_GRID, m_channelLiveDigGrid);        
    DDX_Control(pDX, IDC_SD_ERROR, m_sdError);
    DDX_Control(pDX, IDC_DAU_FREQUENCY, m_frequencyControl) ;
}

BEGIN_MESSAGE_MAP(LiveDataViewA202, CFormView)
  ON_BN_CLICKED(IDC_LIVE_DATA_CLOSE, OnBnClickedLiveDataClose)
  ON_CBN_SELCHANGE(IDC_DAU_FREQUENCY, OnCbnSelchangeDauFrequency)
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
  ON_MESSAGE(UM_DAU_ERROR, OnDauError)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_ZERO_TO_RANGE, OnBnClickedZeroToRange)
  ON_BN_CLICKED(IDC_PLUS_MINUS_RANGE, OnBnClickedPlusMinusRange)
  ON_BN_CLICKED(IDC_LIVE_DATA_SAVE_PRINT, OnBnClickedLiveDataSavePrint)
  ON_WM_SHOWWINDOW()
  ON_WM_TIMER()
END_MESSAGE_MAP()

void LiveDataViewA202::InitFrequencyControl( void )
{
    m_frequencyControl.ResetContent();
    CString str;
    double rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_1220];    
    str.Format("0 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_305];
    str.Format("1 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_76];
    str.Format("2 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    rate = DAU::GetDAU().m_comThr.m_sampleRateMap[FQ_MODE_19];
    str.Format("3 (%.1fHz)",rate);
    m_frequencyControl.AddString(str);

    m_frequencyControl.SetCurSel(DAU::GetDAU().m_comThr.GetSamplingMode());
// 
//     for (UINT i = HIGHEST_FREQUENCY_INDEX ; i <= LOWEST_FREQUENCY_INDEX ; i++)
//   {
//     CString frequencyString ;
//     frequencyString.Format( "%6.2f", DAU::BitfieldToFrequency( i ) ) ;
//     m_frequencyControl.AddString( frequencyString ) ;
//   }
//   m_frequencyControl.SetCurSel( m_frequency - HIGHEST_FREQUENCY_INDEX ) ;
}

LRESULT LiveDataViewA202::OnNewData( WPARAM, LPARAM )
{
	if (UpdateCurrentValues( TRUE ))
	{
        m_newDataArrived = TRUE;
        //UpdateData() ;
    }
    return TRUE ;
}

LRESULT LiveDataViewA202::OnDauError( WPARAM, LPARAM )
{
  OnBnClickedLiveDataClose();
	return TRUE ;
}

void LiveDataViewA202::UpdateData( void )
{
  if (m_pSource)
  {
		UpdateSyncroData() ;
  }
}

void LiveDataViewA202::SetSyncroChannelState(int ch, BOOL connected)
{
    COLORREF col = connected ? RGB(255, 255, 255) : GetSysColor(COLOR_BTNFACE);
    m_syncroDescription[ch].SetBkColor(col);
    m_syncroValue[ch].SetBkColor(col);    
}


void LiveDataViewA202::UpdateText( void )
{
    m_channelLiveGrid.UpdateGrid();
    m_channelLiveDigGrid.UpdateGrid();  
}

void LiveDataViewA202::UpdateSyncroData( void )
{
    m_channelLiveGrid.UpdateLiveData(m_currentValues);
    m_channelLiveDigGrid.UpdateLiveData(m_currentValues);

//     for (int i = 0 ; i < m_currentValues.GetSyncroCount() ; i++)
//     {
// 		CString valueText ;
//         double syncroValue, gearing ;
//         Syncro *pCurrent = m_pSource -> GetSyncro( i ) ;
//         CString syncroName = pCurrent -> GetName();
// 
//         if ((Unit::Unused != pCurrent -> GetType()) && (pCurrent -> IsSelected() || !m_showOnlySelected))
// 		{
// 		        //ViewUncalibrated:
//             syncroValue = RADIANS_TO_DEGREES( m_currentValues.GetSyncro( i ).GetUncalibratedRoll() );
//             gearing = pCurrent -> GetGearing();
// 
//             if( syncroName.GetAt(0) == 'C' )
//             {
//                 gearing = 1;
//             }
//             AdjustSyncroValueToRangeA202( &syncroValue, gearing );
//             if( g_AlignerData.SignDef == 1 )
//             {
//                 syncroValue = -syncroValue;
//             }
//             valueText.Format( IDS_DEG_FORMAT, syncroValue ) ;
// 		}
//         else
//         {
//             valueText = _T("");
//         }
//         int index = m_indexMap[syncroName];
//         m_syncroValue [index].SetWindowText( valueText ) ;      
//     }
}

// LiveDataViewA202 diagnostics

#ifdef _DEBUG
void LiveDataViewA202::AssertValid() const
{
	CFormView::AssertValid();
}

void LiveDataViewA202::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// LiveDataViewA202 message handlers
void LiveDataViewA202::OnInitialUpdate( void )
{
  CFormView::OnInitialUpdate() ;
#ifdef _DEBUG
	GetDlgItem( IDC_DAU_FREQUENCY ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_DAU_FREQUENCY_HEADER ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_FIFO_HALF_FULL ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_ACQUSITION_IN_PROGRESS ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_FIFO_FULL ) -> ShowWindow( SW_SHOW );
	//GetDlgItem( IDC_FLAGS_HEADER ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_SD_ERROR ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_LIVE_CLOSE2 ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_LIVE_STOP ) -> ShowWindow( SW_SHOW );
	GetDlgItem( IDC_DUMP_HEADER ) -> ShowWindow( SW_SHOW );
#endif

	m_frequency = DAU::GetDAU().GetFrequency() ;
    m_dataFreqMode = DAU::GetDAU().m_comThr.GetSamplingMode();			
    InitFrequencyControl() ;

    m_channelLiveGrid.ShowWindow(SW_SHOW);        
    m_channelLiveGrid.Init();

    m_channelLiveDigGrid.ShowWindow(SW_SHOW);        
    m_channelLiveDigGrid.Init();


    for (int i = 0 ; i < 10 ; i++)
    {
        SetSyncroChannelState(i, FALSE); 
    }

    m_timer = SetTimer(1, 1000, NULL);

//   if (m_pSource)
//   {
//     m_pSource -> AddListener( this ) ;
//   }
}

void LiveDataViewA202::OnDestroy( void ) 
{
  if (m_pSource)
	{
		ResetAllFilters( m_pSource ) ;
	}
  SetSource( NULL ) ;
	CFormView::OnDestroy() ;
}

void LiveDataViewA202::OnActivateView( BOOL activate, CView *pActivateView, CView *pDeactiveView)
{
	if (activate)
	{
        theApp.SetLiveDataViewActivated( TRUE );
	    m_frequency = DAU::GetDAU().GetFrequency() ;
		m_frequencyControl.SetCurSel( DAU::GetDAU().m_comThr.GetSamplingMode() ) ;
		SetSource( &(DAU::GetDAU()) ) ;
    
        if( theApp.IsRangeZeroToRangeA202() )
        {
            OnBnClickedZeroToRange();
        }
        else
        {
            OnBnClickedPlusMinusRange();
        }
        GetDlgItem( IDC_LIVE_DATA_SAVE_PRINT )->EnableWindow( TRUE );
        UpdateText() ;
	}
  
  CFormView::OnActivateView( activate, pActivateView, pDeactiveView ) ;
}

void LiveDataViewA202::ResetFilters( void )
{  
  for (int i = 0 ; i < DAU_NB_SYNCRO_CHANNELS ; i++)
  {
    // No filter
		DAU::GetDAU().GetSyncro( i ) -> ResetFilter() ;
  }
}

void LiveDataViewA202::OnCbnSelchangeDauFrequency( void )
{
    m_dataFreqMode = m_frequencyControl.GetCurSel();
    DAU::GetDAU().m_comThr.SetSamplingMode(m_dataFreqMode);			

//     m_frequency = m_frequencyControl.GetCurSel() + HIGHEST_FREQUENCY_INDEX ;
//   DAU::GetDAU().SetFrequency( m_frequency ) ;
//   DAU::GetDAU().Start() ;
}

void LiveDataViewA202::OnBnClickedLiveDataClose( void )
{
    DAU::GetDAU().EnableErrorEvent( FALSE );
    theApp.SetLiveDataViewActivated( FALSE );
    theApp.SwitchView( theApp.m_pSystemConfigurationView ) ;
}

CString LiveDataViewA202::MakeStringLength(CString msg, int length, char fillCh)
{
    //CString str;
    if(msg.GetLength() > length)
    {
        msg = msg.Left(length-1);
        msg += " ";
    }
    else
    {
        int numSpace = length-msg.GetLength();
        for(int i=0;i<numSpace;i++)
        {
            msg += fillCh;
        }
    }
    return msg;
}

void LiveDataViewA202::StoreLiveDataToFile(CString filename)
{
    CString str;
    ofstream ff(filename, fstream::out|fstream::trunc );
    int colWidth = 15;

    ff << "LIVE DATA (" << COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S")) << ")" << std::endl;
    ff << std::endl;
    ff << "Project:     " << SysSetup->GetProjectName() << std::endl;
    ff << "Comment:     " << SysSetup->GetShipName() << std::endl;
    ff << "Operator:    " << SysSetup->GetOperatorName() << std::endl;
    ff << "Place:       " << SysSetup->GetPlace() << std::endl;
    ff << "DAB S/N:     " << DAU::GetDAU().GetSerialNumber() << std::endl;
    ff << std::endl;
    ff << "SYNCHRO" << std::endl;
    ff << std::endl;
    ff << MakeStringLength("Channel", colWidth) << MakeStringLength("Type", colWidth) << MakeStringLength("Value", colWidth) << MakeStringLength("Description", colWidth) << std::endl;        
    ff << MakeStringLength("-", 90, '-') << std::endl;
    
    int maxNumCh = 8;
    for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    {    
        Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(i));
        if(pD)
            continue;

        Syncro* pC = DAU::GetDAU().GetSyncro(i);            
        CString name = pC->GetName();
        ff << MakeStringLength(name, colWidth) ;
        
        if(pC->GetType() != UnitType::Unused)
        {
            //CString type, str;             
            ff << MakeStringLength(UnitType::GetUnitText(pC->GetType()), colWidth) ;

            double syncroValue = RADIANS_TO_DEGREES( m_currentValues.GetSyncro( i ).GetUncalibratedRoll() );
            double gearing = pC->GetGearing();

            if( name.GetAt(1) == 'C' )
            {
                gearing = 1;
            }
            AdjustSyncroValueToRangeA202( &syncroValue, gearing );
            if( g_AlignerData.SignDef == 1 )
            {
                syncroValue = -syncroValue;
            }
            CString valueText;
            valueText.Format( IDS_DEG_FORMAT, syncroValue ) ;
            ff << MakeStringLength(valueText, colWidth);
            ff << MakeStringLength(pC->m_description, 60) << std::endl;    
        }
        else
        {
            ff << MakeStringLength("-", colWidth) << MakeStringLength("-", colWidth) << MakeStringLength("-", colWidth);
            ff << std::endl;
        }
     }

    ff << std::endl << std::endl;
    ff << "DIGITAL" << std::endl;
    ff << std::endl;
    ff << MakeStringLength("Channel", colWidth) << MakeStringLength("Type", colWidth) << MakeStringLength("Roll", colWidth);
    ff << MakeStringLength("Pitch", colWidth) << MakeStringLength("Heading", colWidth) << MakeStringLength("Description", colWidth) << std::endl;        
    ff << MakeStringLength("-", 130, '-') << std::endl;

    for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    {    
        Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(i));
        if(!pD)
            continue;

        CString name, gearing, type, str, valueText;     
        name = pD->GetName();
        ff << MakeStringLength(name, colWidth) ;

        if(pD->GetSyncroType() != DigChTypeProUnused)
        {
            ff << MakeStringLength(pD->GetDigTypeText(), colWidth) ;
                        
            if(pD->m_dataReceptionOK)
            {
                SingleSensor& val = m_currentValues.GetSyncro( i );
                if(val.GetStatus() == DS_OK)
                {
                    double roll = RADIANS_TO_DEGREES( val.GetUncalibratedRoll() );
                    double pitch = RADIANS_TO_DEGREES( val.GetUncalibratedPitch() );
                    double heading = RADIANS_TO_DEGREES( val.GetUncalibratedHeading() );
                    double gearing = pD->GetGearing();

                    if( name.GetAt(1) == 'C' )
                    {
                        gearing = 1;
                    }
                    AdjustSyncroValueToRangeA202( &roll, gearing );
                    AdjustSyncroValueToRangeA202( &pitch, gearing );
                    AdjustSyncroValueToRangeA202( &heading, gearing );
                    if( g_AlignerData.SignDef == 1 )
                    {
                        roll = -roll;
                        pitch = -pitch;
                        heading = -heading;
                    }
                    valueText.Format( IDS_DEG_FORMAT, roll ) ;
                    ff << MakeStringLength(valueText, colWidth) ;
                    valueText.Format( IDS_DEG_FORMAT, pitch ) ;
                    ff << MakeStringLength(valueText, colWidth) ;
                    valueText.Format( IDS_DEG_FORMAT, heading ) ;
                    ff << MakeStringLength(valueText, colWidth) ;
                }
                else
                {
                    valueText = "CRC Error";
                    ff << MakeStringLength(valueText, colWidth) ;
                    ff << MakeStringLength(valueText, colWidth) ;
                    ff << MakeStringLength(valueText, colWidth) ;
                }
            }            
            else
            {
                valueText = "No data";
                ff << MakeStringLength(valueText, colWidth) ;
                ff << MakeStringLength(valueText, colWidth) ;
                ff << MakeStringLength(valueText, colWidth) ;

            }
                      
            ff << MakeStringLength(pD->m_description, 60);
            ff << std::endl;
        }
        else
        {
            ff << MakeStringLength("-", colWidth) << MakeStringLength("-", colWidth) << MakeStringLength("-", colWidth)<< MakeStringLength("-", colWidth) << MakeStringLength("-", colWidth);
            ff << std::endl;
        }
    }

    ff.flush();
    ff.close(); 
}


void LiveDataViewA202::OnBnClickedLiveDataSavePrint()
{
 //   Beep(1000,100);
    
	CString dir = SysSetup->GetProjectPath();
    CString fileName = dir + _T("\\") + m_liveDataFilename;
	static char BASED_CODE szFilter[] = "Live Data Files (*.txt)|*.txt|";
    CFileDialog dlg(FALSE,_T("txt"),(LPCTSTR)fileName,OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT,szFilter);
   
    if (dlg.DoModal() == IDOK)
    {
        m_liveDataDir= dlg.GetPathName();
        m_liveDataFilename = dlg.GetFileName();
        m_liveDataDir.TrimRight(m_liveDataFilename);

        StoreLiveDataToFile(m_liveDataDir+m_liveDataFilename);
    }
    //     GetDlgItem( IDC_LIVE_DATA_SAVE_PRINT )->EnableWindow( FALSE );
//     CString text, msgCaption;
//     CString temporary;
// 
//     CResultTable resultTable;
// 	resultTable.m_InParam.Version = LIVE_DATA_A202;
//     COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( resultTable.m_InParam.Time );
// 	resultTable.m_InParam.AngleRange0ToPlusMinus180 = TRUE;
// 	resultTable.m_InParam.SignDef = g_AlignerData.SignDef;
// 	resultTable.m_InParam.pACA = NULL;
// 	resultTable.m_InParam.Comment = DB_EMPTY_STRING;
// 	resultTable.m_InParam.ShowPresenetDialog = TRUE;
// 	resultTable.m_InParam.SavedToDataBase = FALSE;
// 	resultTable.m_InParam.pTemperature = NULL;
// 	//resultTable.m_InParam.SamplingRate = 0;
//     m_frequencyControl.GetWindowText( temporary );
//     resultTable.m_InParam.SamplingRate = atof( temporary ) ;
//     resultTable.m_pLiveDataA202Param = m_liveDataA202Param;
// 
//     g_AlignerData.NoOfCorr = 0;
// 
//     for (int i = 0 ; i < m_currentValues.GetSyncroCount() ; i++)
//     {
//         Syncro *pCurrent = m_pSource -> GetSyncro( i ) ;
//         CString syncroName = pCurrent -> GetName();
// 
//         if( (Unit::Unused != pCurrent -> GetType()))
//         {  
// 	        int index = m_indexMap[syncroName];
//             m_syncroDescription[index].GetWindowText( temporary );
//             m_liveDataA202Param[g_AlignerData.NoOfCorr].m_station = temporary;
//             m_syncroHeader[index].GetWindowText( temporary );//== syncroName
//             m_liveDataA202Param[g_AlignerData.NoOfCorr].m_channel = temporary;
//             m_syncroValue[index].GetWindowText( temporary );
//             m_liveDataA202Param[g_AlignerData.NoOfCorr].m_value = atof( temporary );
//             
//             m_liveDataA202Param[g_AlignerData.NoOfCorr].m_gearing = pCurrent->GetGearing();
//             m_liveDataA202Param[g_AlignerData.NoOfCorr].m_refVoltage = pCurrent->GetRefVoltageA202();
//             g_AlignerData.NoOfCorr++;
// 		}
// 	}
// 
//     msgCaption.LoadString( IDS_QUESTION_CAPTION );
// 	text.LoadString( IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD );
// 
//     resultTable.ShowReport( FALSE );
// 
// 	if( IDYES == MessageBox( text, msgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
// 	{
// 		resultTable.CloseReport();
// 	}
// 	else
// 	{
// 		resultTable.CloseReport();
//         resultTable.DeleteLast();
// 	}
//     theApp.SetLiveDataViewActivated( FALSE );
//     theApp.SwitchView( theApp.m_pSystemConfigurationView ) ;
}


void LiveDataViewA202::SetShowOnlySelected( BOOL flag )
{
    m_showOnlySelected = flag ;
}

void LiveDataViewA202::OnBnClickedZeroToRange()
{
    theApp.SetRangeZeroToRangeA202( TRUE );
    CheckRadioButton( IDC_ZERO_TO_RANGE, IDC_PLUS_MINUS_RANGE, IDC_ZERO_TO_RANGE );
}

void LiveDataViewA202::OnBnClickedPlusMinusRange()
{
    theApp.SetRangeZeroToRangeA202( FALSE );
    CheckRadioButton( IDC_ZERO_TO_RANGE, IDC_PLUS_MINUS_RANGE, IDC_PLUS_MINUS_RANGE );
}

void LiveDataViewA202::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if( bShow == FALSE )
    {
        SetSource( NULL ) ;
    }
    __super::OnShowWindow(bShow, nStatus);
}

void LiveDataViewA202::OnTimer(UINT nIDEvent)
{
    if(m_newDataArrived)
    {
        UpdateData();
        m_newDataArrived = FALSE;
    }


    __super::OnTimer(nIDEvent);
}
