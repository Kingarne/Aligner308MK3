// $Id: Measure.cpp,v 1.8 2013/04/02 08:28:16 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002


#include "StdAfx.h"
#include "Util.h"
#include "Measure.h"
#include "Filter.h"
#include "ASound.h"
#include "SetupAxesDlg.h"


CMeasure::CMeasure(void):m_pSource(NULL)
{
  m_mode = MEASURE_MODE_OFF;
    m_InParam.Break = FALSE;
  memset( m_InParam.ObjNo, 0, SIZE_OF_ARRAYS * sizeof( int ) );
  m_InParam.RefObjNo = 0;
  m_InParam.Mode1 = Diff;
  m_InParam.Tao = 1;
  memset( m_InParam.FiObj, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  m_InParam.FiRef = 0;
  memset( m_InParam.ZParCorr, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  m_InParam.CountObj = 0;
  m_InParam.RangeX = 0;
  m_InParam.RangeY = 0;
  m_InParam.UseElev = FALSE; 
  m_InParam.Fs = 0x11;
	m_InParam.SperryActive = FALSE;
  memset( m_OutParam.DeltaRoll, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_OutParam.DeltaPitch, 0, SIZE_OF_ARRAYS * sizeof( double ) );
  memset( m_OutParam.TempT, 0, SIZE_OF_ARRAYS * sizeof( double ) );

  m_pGraph = NULL;
}

CMeasure::~CMeasure(void)
{
	ResetAllFilters( m_pSource );
}
/*
  //Global variables which are modified in Measure():
  g_AlignerData.ErrorFlag
  g_AlignerData.PitchRefG
  g_AlignerData.RollRefG
  g_AlignerData.Reject      //modified before read
  g_AlignerData.DStore      //read before modified
  g_AlignerData.ErrorDef    //modified before read

  //Global variables which are only read in Measure():
  g_AlignerData.IDError
  g_AlignerData.ErrorType
  g_AlignerData.SignDef
  g_AlignerData.NoOfBedPlanes
  g_AlignerData.MAE
  g_AlignerData.Eal
  g_AlignerData.Eac
  g_AlignerData.EalAbs
  g_AlignerData.EalS

*/

void CMeasure::SetGraphPointer( CGraphView *pGraph )
{
  m_pGraph = pGraph;
}


void CMeasure::Measure()
{
    long int Tmax, NCount, NStop, N5Tao, AveStart, maxNoOfSamplesOnSerie;
    int i, Tinc;
    CString TString;
    double SR[SIZE_OF_ARRAYS], SP[SIZE_OF_ARRAYS], UR[SIZE_OF_ARRAYS], UP[SIZE_OF_ARRAYS];
    BOOL BusyFlag;
    double RollRefU, PitchRefU;
    double As, Bs, OmegaT, ErrorLim, Error, MaxTilt;
    CString L[19]; //index [0] not used
	BOOL ChCon1[SIZE_OF_ARRAYS];
	double Ts;

    SetSelectedChannels();

    MeasureGraphInParam graphParams;
 
	theApp.SetHelpEnabled( FALSE );
	m_InParam.Break = FALSE;
	g_AlignerData.Reject = FALSE;
    g_AlignerData.ErrorFlag = FALSE;
    NCount = 0;
    FreqToTs( m_InParam.Fs, &Ts );
    memset( ChCon1, FALSE, SIZE_OF_ARRAYS * sizeof( BOOL ) );

    for( i=1; i<=m_InParam.CountObj; i++ )
    {
        ChCon1[m_InParam.ObjNo[i]] = TRUE;
    }

	if( m_InParam.Mode1 != Stab )
    {
        ChCon1[m_InParam.RefObjNo] = TRUE;
    }
    As = 0.5 / ( 1 - TETA );
    Bs = 1 / sqrt( 1 - ( TETA * TETA ) );

    //Init the graph
    int noOfSeriesInGraph;
    if( ( m_InParam.Mode1 == Stab ) || ( m_InParam.Mode1 == StabDiff ) || 
        ( m_InParam.Mode1 == RPath ) )
    {
        noOfSeriesInGraph = m_InParam.CountObj;
    }
    else
    {
        noOfSeriesInGraph = 2 * m_InParam.CountObj;
    }
    m_pGraph->InitDefaultPlotGraph( noOfSeriesInGraph );

    CString str;
    if( GetMRad() == TRUE )
    {
        str.LoadString( IDS_MRAD_UNIT );
        m_pGraph->SetAxisYUnit( &str );
    }
    else
    {
        str.LoadString( IDS_ARCMIN_UNIT );
        m_pGraph->SetAxisYUnit( &str );
    }
    memset( SR, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( SP, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( UR, 0, SIZE_OF_ARRAYS * sizeof( double ) );
    memset( UP, 0, SIZE_OF_ARRAYS * sizeof( double ) );
//     memset( dauData.AbsRoll, 0, SIZE_OF_ARRAYS * sizeof( double ) );
//     memset( dauData.AbsPitch, 0, SIZE_OF_ARRAYS * sizeof( double ) );
//     memset( dauData.TempT, 0, SIZE_OF_ARRAYS * sizeof( double ) );
//     memset( dauData.RollRawData, 0, SIZE_OF_ARRAYS * sizeof( short ) );
//     memset( dauData.PitchRawData, 0, SIZE_OF_ARRAYS * sizeof( short ) );
    RollRefU = 0;
    PitchRefU = 0;
    MaxTilt = 0.1;

	ResetAllFilters( m_pSource, Ts, TETA, m_InParam.Tao );

//     if( m_InParam.UseElev == TRUE )
//     {	// set the initial value for the filters
// 		int sI;
//         if( m_InParam.Mode1 == Diff )
//         {
// 	        double eal, eac;
//             for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//             {
// 				eal = GetEal( i );
// 				if( eal != 1E10 )
//             
// 			    eac = GetEac( i );
// 			    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( i );
// 			    m_pSource->GetSensor( sI )->ResetFilter( Ts, TETA, m_InParam.Tao, MRADIANS_TO_RADIANS( -eac ), MRADIANS_TO_RADIANS( -eal ) );
//             }
//         }    
//         else if( m_InParam.Mode1 == Stab )
//         {
// 		    double ealAbs;
//             for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//             {
// 		        ealAbs = GetEalAbs( i );
// 			    if( ealAbs != 1E10 )
//                 {				
// 			        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( i );
//                 
// //                 CString debugText ;
// //                 debugText.Format( _T("KKK111 %f\n"), m_InParam.Tao )  ;
// //                 ::OutputDebugString( debugText ) ;
//             
// 				    m_pSource->GetSensor( sI )->ResetFilter( Ts, TETA, m_InParam.Tao, 0.0, MRADIANS_TO_RADIANS( -ealAbs ) );
//                 }
//             }
//         }
//         else if( m_InParam.Mode1 == StabDiff )
//         {
//             for( i=1; i<=m_InParam.CountObj; i++ )
//             {
//                 if( g_AlignerData.EalS[i] != 1E10 )
//                 {
// 			        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( i );
//                     ResetAllFilters( m_pSource, Ts, TETA, m_InParam.Tao ) ;
//     //                 CString debugText ;
//     //                 debugText.Format( _T("KKK111 %f\n"), m_InParam.Tao )  ;
//     //                 ::OutputDebugString( debugText ) ;
//     //					m_pSource->GetSensor( sI )->ResetFilter( Ts, TETA, m_InParam.Tao, 0.0, MRADIANS_TO_RADIANS( -g_AlignerData.EalS[i] ) );
//                 }
//             }     
//         }
//     }
    AveStart =100000000;
    Tmax = RoundToLong( 5 * m_InParam.Tao );
    N5Tao = RoundToLong( 5 * m_InParam.Tao / Ts );
    Tinc = RoundToInt( m_InParam.Tao / 2 );

    if( Tinc == 0 )
    {
        Tinc = 1;
    }
    BusyFlag = FALSE;

    // Start of measurements 

    if( m_InParam.RangeX == 0 )
    { //autoscale
        NStop = RoundToLong( NO_OF_TAO_IN_AUTOSCALE_X_AXIS * m_InParam.Tao / Ts );
        if( m_InParam.SperryActive == TRUE )
        {
            NStop += RoundToLong( SPERRY_K / Ts );
        }
		m_pGraph->SetAxisXUnitAndMax( (double)NStop * Ts );
    }
    else
    { //manual
        NStop = RoundToLong( m_InParam.RangeX / Ts );
		m_pGraph->SetAxisXUnitAndMax( (double)m_InParam.RangeX );
    }
    maxNoOfSamplesOnSerie =  MAX_NO_OF_SAMPLES_IN_GRAPH / m_pGraph->GetNoOfSeries();
	m_pGraph->SetReductionFactorForGraphData( NStop, maxNoOfSamplesOnSerie );

    
    DAU::GetDAU().EnableErrorEvent( TRUE );
    m_dauData.clear();
    m_mode = MEASURE_MODE_ON;
    AcquDriverOutParam dauData;
    //get start temperatures
    GetNextData(dauData);

    //Show graph
    graphParams.writeTiltData = FALSE;
    graphParams.useElev = m_InParam.UseElev;
    graphParams.mode = m_InParam.Mode1;
    graphParams.timeConstant = m_InParam.Tao;
    graphParams.pChCon1 = ChCon1;
    graphParams.pTempT = dauData.TempT;
    graphParams.RefNo = m_InParam.RefObjNo;  
    m_pGraph->SetAxisXMin( 0 );
  
	if( m_InParam.RangeY == 0 )
	{ //auto
		m_pGraph->SetAxisYMinAutoScaleOn( TRUE );
		m_pGraph->SetAxisYMaxAutoScaleOn( TRUE );
	}
	else
	{	//manual
        CSetupAxesDlg setupAxesDlg;
		m_pGraph->SetAxisYMin( (double)setupAxesDlg.GetMinY() );
		m_pGraph->SetAxisYMax( (double)setupAxesDlg.GetMaxY() );
	}
    graphParams.TimeStamp = GetTimeStr( TRUE );
    graphParams.DateStamp = GetDateStr();
    graphParams.commonFlatTest = m_InParam.commonFlatTest;
    for( i=1; i<=noOfSeriesInGraph; i++ )
    {
        m_pGraph->SetLinePattern( i, m_pGraph->GetLinePattern( 0 ) );
    }
    m_pGraph->ShowMeasureGraphWithText( &graphParams );

    do
    {
        NCount++;
    
		if( m_InParam.RangeX == 0 )
		{ //autoscale
            if( NCount == N5Tao )
            {
                BusyFlag = TRUE;
                for( i=1; i<=m_InParam.CountObj; i++ )
                {
                    if( fabs( UR[i] ) > MaxTilt )
                    {
                        MaxTilt = fabs( UR[i] );
                    }
                    if( fabs( UP[i] ) > MaxTilt )
                    {
                        MaxTilt = fabs( UP[i] );
                    }
                }

                if( m_InParam.Mode1 == Azim )
                {
                    ErrorLim = 0.001 / MaxTilt;
                }
                else
                {
                    //ErrorLim = 0.005 / MaxTilt;
                    ErrorLim = 0.003 / MaxTilt;
                }

                do
                {
                    Tmax += Tinc;
                    OmegaT = Tmax / m_InParam.Tao;
                    Error = As * exp(-OmegaT) + Bs * exp(-TETA * OmegaT);
                    DoEvents();
                }while( ( Error >= ErrorLim ) && ( g_AlignerData.ErrorDef != ERR_CANCEL ) );

                if( g_AlignerData.ErrorDef == ERR_CANCEL )
                {
					        theApp.SetHelpEnabled( TRUE );
                return;
                }

				NStop = RoundToLong( Tmax / Ts );

                if( m_InParam.SperryActive == TRUE )
                {
                    AveStart = NStop;
                    NStop += RoundToLong( SPERRY_K / Ts );
                    Tmax += SPERRY_K;
                }

				m_pGraph->SetAxisXUnitAndMax( (double)NStop * Ts );
				m_pGraph->SetReductionFactorForGraphData( NStop, maxNoOfSamplesOnSerie );
            }
        }

    
        GetNextData(dauData);
            
        if( g_AlignerData.ErrorDef == ERR_CANCEL )
        {
			theApp.SetHelpEnabled( TRUE );
            return;
        }

// 		for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//         {
//             dauData.AbsPitch[i] -= m_InParam.ZParCorr[i];
//         }

        if( m_InParam.UseElev == TRUE )
        {
//             if( m_InParam.Mode1 == Diff )
//             {
//                 for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//                 {
// 		            if( GetEal( i ) != 1E10 )
//                     {
// 					    dauData.AbsRoll[i] -= GetEac( i );
// 					    dauData.AbsPitch[i] -= GetEal( i );
//                     }
//                 }
//             }
//             else if( m_InParam.Mode1 == Stab )
//             {
//                 for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
//                 {
// 			        if( GetEalAbs( i ) != 1E10 )
//                     {
// 				        dauData.AbsPitch[i] -= GetEalAbs( i );
//                     }
//                 }   
//             }
//             if( m_InParam.Mode1 == StabDiff )
//             {
//                 for( i=1; i<=m_InParam.CountObj; i++ )
//                 {
//                     if( g_AlignerData.EalS[i] != 1E10 )
//                     {
//                         dauData.AbsPitch[m_InParam.ObjNo[i]] -=  g_AlignerData.EalS[i];
//                     }
//                 }
//             }
        }

	    if( m_InParam.Mode1 == Azim )
        {
		    RollRefU = dauData.AbsRoll[m_InParam.RefObjNo];
		    PitchRefU = dauData.AbsPitch[m_InParam.RefObjNo];
        }

        int j = 1;
        for( i=1; i<=m_InParam.CountObj; i++ )
        {
            if( ( m_InParam.Mode1 == Stab ) || ( m_InParam.Mode1 == StabDiff ) )
            {
                if( m_InParam.Mode1 == StabDiff )
                {
			        UP[i] = dauData.AbsPitch[m_InParam.ObjNo[i]] - dauData.AbsPitch[m_InParam.RefObjNo];
                }
                else
                {
			        UP[i] = dauData.AbsPitch[m_InParam.ObjNo[i]];
                }
                if( BusyFlag == FALSE )
                {
				    if( m_pGraph->TimeToUpdate( NCount ) == TRUE )
                    {
			            m_pGraph->SetXYData( i, m_pGraph->GetTimeValue( NCount, Ts ), UP[i] );
                    }
                }
            }
            else
            {
			    
    			
			    //F1 in the pascal code            			
                UR[i] = dauData.AbsRoll[m_InParam.ObjNo[i]] - dauData.AbsRoll[m_InParam.RefObjNo];
                UP[i] = dauData.AbsPitch[m_InParam.ObjNo[i]] - dauData.AbsPitch[m_InParam.RefObjNo];

                //TRACE("RollOb:%f, RollRef:%f, PiOb:%f, PiRef:%f,\n",dauData.AbsRoll[m_InParam.ObjNo[i]],dauData.AbsRoll[m_InParam.RefObjNo],dauData.AbsPitch[m_InParam.ObjNo[i]],dauData.AbsPitch[m_InParam.RefObjNo]);
                //TRACE("UR:%f, UP:%f\n",UR[i],UP[i]);

    //             UR[i] = dauData.AbsRoll[m_InParam.ObjNo[i]] * cos( m_InParam.FiObj[i] ) - 
    //                   dauData.AbsPitch[m_InParam.ObjNo[i]] * sin( m_InParam.FiObj[i] ) - 
    //                   ( dauData.AbsRoll[m_InParam.RefObjNo] * cos( m_InParam.FiRef ) - 
    //                   dauData.AbsPitch[m_InParam.RefObjNo] * sin( m_InParam.FiRef ) );
    //             UP[i] = dauData.AbsPitch[m_InParam.ObjNo[i]] * cos( m_InParam.FiObj[i] ) + 
    //                   dauData.AbsRoll[m_InParam.ObjNo[i]] * sin( m_InParam.FiObj[i] ) - 
    //                   ( dauData.AbsPitch[m_InParam.RefObjNo] * cos( m_InParam.FiRef ) + 
    //                   dauData.AbsRoll[m_InParam.RefObjNo] * sin( m_InParam.FiRef ) );

                if( BusyFlag == FALSE )
                {
                    if( m_InParam.Mode1 == RPath )
                    {
                        if( m_pGraph->TimeToUpdate( NCount ) == TRUE )
                        {
				            m_pGraph->SetXYData( i, m_pGraph->GetTimeValue( NCount, Ts ), UP[i] );
                        }
                    }
                    else
                    {            
                        if( m_pGraph->TimeToUpdate( NCount ) == TRUE )
                        {
						    m_pGraph->SetXYData( j++, m_pGraph->GetTimeValue( NCount, Ts ), g_AlignerData.SignDef*UR[i] );
                            m_pGraph->SetXYData( j++, m_pGraph->GetTimeValue( NCount, Ts ), g_AlignerData.SignDef*UP[i] );
                        }
                    }
                }
            }

            if( NCount > AveStart )
            {
                if( m_InParam.Mode1 == Azim )
                {
                    SR[i] += RollRefU;
                }
                else
                {
                    SR[i] += UR[i];
                }
                SP[i] += UP[i];
            }
        }

        if( BusyFlag == FALSE )
        {
            if( m_pGraph->TimeToUpdate( NCount ) == TRUE )
            {
                m_pGraph->UpdateData(NCount > NStop); // 
            }
        }
        BusyFlag = FALSE;
    
    } while( ( (NCount < NStop) || m_InParam.continousGraph) && ( g_AlignerData.ErrorDef == ERR_NO_ERROR ) && ( m_InParam.Break == FALSE ) );

    m_mode = MEASURE_MODE_OFF;

    if( g_AlignerData.ErrorDef != ERR_NO_ERROR )
    {
        //TODO Nicesound;
        g_AlignerData.ErrorFlag = TRUE;
    }
    else
    {
        if( m_InParam.Break == TRUE )
        {
        g_AlignerData.Reject = TRUE;

		if( m_InParam.RangeX != 0 )
		{ //manual
            //TODO NiceSound;
            m_MsgCaption.LoadString( IDS_QUESTION_CAPTION );
            m_Text.LoadString( IDS_MEASURE_APPROVE_MEASUREMENT );

				    if( MessageBox( m_Text, m_MsgCaption, MB_ICONQUESTION |MB_YESNO|MB_DEFBUTTON1 ) == IDYES )
            {
            g_AlignerData.Reject = FALSE;
            }
        }
    }

    if( g_AlignerData.Reject == FALSE )
    {
      memcpy( m_OutParam.TempT, dauData.TempT, SIZE_OF_ARRAYS * sizeof( double ) );

      for( i=1; i<=m_InParam.CountObj; i++ )
      {
				if( m_InParam.SperryActive == TRUE )
        {
          NCount = RoundToLong( SPERRY_K / Ts );
          m_OutParam.DeltaRoll[i] = SR[i] / NCount;
          m_OutParam.DeltaPitch[i] = SP[i] / NCount;
        }
        else
        {
          if( m_InParam.Mode1 == Azim )
          {
            m_OutParam.DeltaRoll[i] = RollRefU;
          }
          else
          {
            m_OutParam.DeltaRoll[i] = UR[i];
          }
          m_OutParam.DeltaPitch[i] = UP[i];
        }
      }

      //Show graph
	    //save final temperatures
      graphParams.pTempT = dauData.TempT;
      graphParams.writeTiltData = TRUE;
      m_pGraph->ShowMeasureGraphWithText( &graphParams );
			ASound sound;
			sound.PlayNiceSound();
    }
  }
	theApp.SetHelpEnabled( TRUE );
	DAU::GetDAU().EnableErrorEvent( FALSE ); 
	//ControlLine;
}

void CMeasure::AddData(DAUValues data)
{
    if(m_dauData.size() > 500) // Caution, Should not grow beyond 10 or so.
        return;
    
    AcquDriverOutParam param;
   	
  //  m_AcquDriver.m_InParam.SourceStatus = ACQU_IDLE ;
    int sI, syncroCount, sensorCount;
    syncroCount = m_pSource->GetSyncroCount();
    sensorCount = m_pSource->GetSensorCount();

    for ( int i = 0 ; i < syncroCount ; i++ )
    {
        sI = CONVERT_SYNCRO_CHANNEL_TO_ARRAY_INDEX( i );
        CString str;
        str = m_pSource -> GetSyncro( i ) -> GetName();
//         param.AbsRoll [sI] = -RADIANS_TO_MRADIANS( m_pSource->GetSyncro( i )->GetRoll( TRUE ) ) ;
//         param.AbsPitch [sI] = -RADIANS_TO_MRADIANS( m_pSource->GetSyncro( i )->GetPitch( TRUE ) ) ;
//         param.RollRawData [sI] = m_pSource->GetSyncro( i )->GetData().m_roll ;
//         param.PitchRawData [sI] = m_pSource->GetSyncro( i )->GetData().m_pitch ;   		
		
 		param.AbsRoll [sI] = -RADIANS_TO_MRADIANS( data.GetSyncro(i).GetRoll() ) ;
 		param.AbsPitch [sI] = -RADIANS_TO_MRADIANS( data.GetSyncro(i).GetPitch() ) ;
 		param.RollRawData [sI] = data.GetSyncro(i).GetUncalibratedRoll() ;
 		param.PitchRawData [sI] = data.GetSyncro(i).GetUncalibratedPitch() ;   		
	}

    for( int i = 0 ; i < sensorCount ; i++ )
    {
        sI = CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX( i ) ;
        CString str;
        str = m_pSource->GetSensor( i )->GetName();
//         param.AbsRoll[sI]  = -RADIANS_TO_MRADIANS( m_pSource->GetSensor( i )->GetRoll( TRUE ) ) ;
//         param.AbsPitch[sI] = -RADIANS_TO_MRADIANS( m_pSource->GetSensor( i )->GetPitch( TRUE ) ) ;
//         param.TempT[sI] = m_pSource->GetSensor( i ) -> GetTemperature() ;
//         param.RollRawData[sI] = m_pSource->GetSensor( i )->GetData().m_roll ;
//         param.PitchRawData[sI] = m_pSource->GetSensor( i )->GetData().m_pitch ;
    
		//TRACE("Add data:r:%f, p:%f\n",data.GetSensor(i).GetRoll(),data.GetSensor(i).GetPitch());
		param.AbsRoll[sI]  = -RADIANS_TO_MRADIANS( data.GetSensor(i).GetRoll() ) ;
		param.AbsPitch[sI] = -RADIANS_TO_MRADIANS( data.GetSensor(i).GetPitch() ) ;
		param.TempT[sI] = data.GetSensor(i).GetTemperature();
		param.RollRawData[sI] = data.GetSensor(i).GetUncalibratedRoll();
		param.PitchRawData[sI] = data.GetSensor(i).GetUncalibratedPitch() ;
	}
    m_OutParam.TempT[MAIN_BOARD_ARRAY_INDEX] = m_pSource->GetMainboard()->GetTemperature() ;

    m_dauData.push_back(param);
}


void CMeasure::GetNextData(AcquDriverOutParam& param)
{
    BOOL dataFetched = FALSE;
    do 
    {
        DoEvents() ;
        if(m_dauData.size() > 0)
        {
            param = m_dauData.front();
            m_dauData.pop_front();
            
            
            dataFetched = TRUE;
			//TRACE("Get: x=%f, y%f, size:%d\n",param.AbsPitch[0], param.AbsRoll[0], m_dauData.size());
        }
		else
		{			
			Sleep(10);
		}
    } 
    while(!dataFetched && m_InParam.Break == FALSE);


}


