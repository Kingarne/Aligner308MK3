// $Id: Util.cpp,v 1.25 2014-07-11 09:29:35 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "AlignmentWizard.h"
#include "SetupLiveDataGraphDlg.h"
#include "SystemSetup.h"
#include "SetupAxesDlg.h"
#include "InfoDlg.h"

int RoundToInt( double v )
{
  int returnValue;

  if( v > 0 )
  {
    returnValue = (int)( v + 0.5 );
  }
  else
  {
    returnValue = (int)( v - 0.5 );
  }

  return returnValue;
}

int RoundToInt( float v )
{
  int returnValue;

  if( v > 0 )
  {
    returnValue = (int)( v + 0.5 );
  }
  else
  {
    returnValue = (int)( v - 0.5 );
  }

  return returnValue;
}

long RoundToLong( double v )
{
  long returnValue;

  if( v > 0 )
  {
    returnValue = (long)( v + 0.5 );
  }
  else
  {
    returnValue = (long)( v - 0.5 );
  }

  return returnValue;
}

long RoundToLong( float v )
{
  long returnValue;

  if( v > 0 )
  {
    returnValue = (long)( v + 0.5 );
  }
  else
  {
    returnValue = (long)( v - 0.5 );
  }

  return returnValue;
}

void FreqToTs( double freq, double* pTs)
{
    *pTs = 1.0f/freq;
}

void FreqToTs( unsigned int FreqvDiv, double* pTs)
{
  unsigned int FShift,J;

  FShift = FreqvDiv & 0x001F;
  *pTs = 1.0;

  for( J = 1; J <= FShift; J++ )
  {
    *pTs = 2 * (*pTs);
  }
  *pTs = (*pTs) / 4000000;
}

// Detta är en rotation med Alfa [radianer] runt origo
// av vektorn representerad av {R,P}. Resultated representeras av {*pAc, *pAl}.
void E1( double* pAc, double* pAl, double R, double P, double Alfa )
{
  *pAc = R * cos( Alfa ) + P * sin( Alfa );
  *pAl = P * cos( Alfa ) - R * sin( Alfa );
}

//DoEvents - lets windows get control during long actions
void DoEvents()
{
	MSG msg;
	while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
  { 
		VERIFY( ::GetMessage( &msg, NULL, 0, 0 ) );
		DispatchMessage( &msg ); 
	}
  SleepEx( 0, TRUE ) ;
}


void CartToVec( double RL, double PL, double *pAL, double *pFiL )
{
  *pAL = sqrt( RL*RL + PL*PL );

  if( PL < 0 )
  {
    *pFiL = 180 - atan( RL / PL ) * 180 / M_PI;
  }
  else if( PL == 0 )
  {
    if( RL < 0 )
    {
      *pFiL = 90;
    }
    else
    {
      *pFiL = -90;
    }
  }
  else
  {
    *pFiL = -atan( RL / PL ) * 180 / M_PI;
  }

  if( *pFiL < 0 )
  {
    *pFiL += 360;
  }
}

CString GetDateStr( void )
{
  CTime currTime = CTime::GetCurrentTime();
  CString date;
  date = currTime.Format("%Y") + "-" + currTime.Format("%m") + "-" + currTime.Format("%d");
  return( date );
}

CString GetTimeStr( BOOL showSeconds )
{
  CTime currTime = CTime::GetCurrentTime();
  CString time;
	if( showSeconds == TRUE )
	{
		time = currTime.Format("%H") + ":" + currTime.Format("%M") + ":" + currTime.Format("%S");
	}
	else
	{
		time = currTime.Format("%H") + ":" + currTime.Format("%M");
	}
  return( time );
}

CString GetDateStampStr( DBTIMESTAMP *timeStamp )
{
  CString date;
	date.Format( "%04d-%02d-%02d", timeStamp->year, timeStamp->month, timeStamp->day ) ;
  return( date );
}

CString GetTimeStampStr( DBTIMESTAMP *timeStamp, BOOL showSeconds )
{
  CString time;
	if( showSeconds == TRUE )
	{
		time.Format( "%02d:%02d:%02d", timeStamp->hour, timeStamp->minute, timeStamp->second ) ;
	}
	else
	{
		time.Format( "%02d:%02d", timeStamp->hour, timeStamp->minute ) ;
	}
  return( time );
}

double GetRange0ToPlusMinus180( double angleIn )
{
  if( ( angleIn <= 180 ) && ( angleIn >= -180 ) )
  {
    return( angleIn );
  }

  double angleOut = angleIn;

  if( angleIn < 0 )
  {
    do
    {
      angleOut += 360;
    }while( angleOut < -180 );
  }
  else
  {
    do
    {
      angleOut -= 360;
    }while( angleOut > 180 );
  }
  return( angleOut );
}

double GetRange0ToPlus360( double angleIn )
{
  if( ( angleIn <= 360 ) && ( angleIn >= 0 ) )
  {
    return( angleIn );
  }

  double angleOut = angleIn;

  if( angleIn < 0 )
  {
    do
    {
      angleOut += 360;
    }while( angleOut < 0 );
  }
  else
  {
    do
    {
      angleOut -= 360;
    }while( angleOut > 360 );
  }
  return( angleOut );
}

void SineFit( double* pAzim, double pTilt[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE], int JC, int N, double* pC2, double* pC1, double* pC0 )
{
  int K;
  double A, B, C, D, E, F, G, H, X, Y, T, Help1;

  A = B = C = D = E = F = G = H = 0;
  *pC0 = *pC1 = *pC2 = 0;

  if( N <= 0 )
  {
    return;
  }

  for( K=1; K<=N; K++ )
  {
    X = cos( DEGREES_TO_RADIANS( pAzim[K] ) );
    Y = -sin( DEGREES_TO_RADIANS( pAzim[K] ) );
    T = pTilt[JC][K];
    A += X;
    B += Y;
    C += T;
    D += SQUARE( X );
    E += X * Y;
    F += T * X;
    G += SQUARE( Y );
    H += T * Y;
  }
  Help1 = 2*E*A*B - N*SQUARE( E ) + D*G*N - D*SQUARE( B ) - G*SQUARE( A );

  if( fabs( Help1 ) < 1E-20 )
  {
    if( Help1 > 0 )
    {
      Help1 = 1E-20;
    }
    else
    {
      Help1 = -1E-20;
    }
  }
  *pC2 = ( H*D*N - H*SQUARE( A ) - B*C*D - E*F*N + E*A*C + B*A*F ) / Help1;
  Help1 = N*D - SQUARE( A );

  if( fabs( Help1 ) < 1E-20 )
  {
    if( Help1 > 0 )
    {
      Help1 = 1E-20;
    }
    else
    {
      Help1 = -1E-20;
    }
  }
  *pC1 = ( N*F - A*C -(N*E -A*B)*(*pC2) ) / Help1;
  *pC0 = ( C - A*(*pC1) - B*(*pC2) ) / N;
}

void SineFitFo( double* pAzim, int *pIndexArmLen, double pTilt[SIZE_OF_YT_MATRIX_X_SIZE][SIZE_OF_YT_MATRIX_Y_SIZE], int JC, int N, double* pC2, double* pC1, double* pC0 )
{
    int K;
    double A, B, C, D, E, F, G, H, X, Y, T, Help1;

    A = B = C = D = E = F = G = H = 0;
    *pC0 = *pC1 = *pC2 = 0;

    if( N <= 0 )
    {
        return;
    }
    
    for( int i=1; i<=JC; i++ )
    {
        double armLen = ((double)pIndexArmLen[i]/1000.0f);   
        for( K=1; K<=N; K++ )
        {
            X = cos( DEGREES_TO_RADIANS( pAzim[K] ) ) * armLen;
            Y = -sin( DEGREES_TO_RADIANS( pAzim[K] ) )* armLen;
            T = pTilt[i][K] * armLen;
            A += X;
            B += Y;
            C += T;
            D += SQUARE( X );
            E += X * Y;
            F += T * X;
            G += SQUARE( Y );
            H += T * Y;
        }
    }
    
    int num = N*JC;
    Help1 = 2*E*A*B - num*SQUARE( E ) + D*G*num - D*SQUARE( B ) - G*SQUARE( A );

    if( fabs( Help1 ) < 1E-20 )
    {
        if( Help1 > 0 )
        {
            Help1 = 1E-20;
        }
        else
        {
            Help1 = -1E-20;
        }
    }
    *pC2 = ( H*D*num - H*SQUARE( A ) - B*C*D - E*F*num + E*A*C + B*A*F ) / Help1;
    Help1 = num*D - SQUARE( A );

    if( fabs( Help1 ) < 1E-20 )
    {
        if( Help1 > 0 )
        {
            Help1 = 1E-20;
        }
        else
        {
            Help1 = -1E-20;
        }
    }
    *pC1 = ( num*F - A*C -(num*E -A*B)*(*pC2) ) / Help1;
    *pC0 = ( C - A*(*pC1) - B*(*pC2) ) / num;
}

void CAdjText( CString* pDum, int inc )
{
    if(inc <= pDum->GetLength())
        return;

    int pad = inc - pDum->GetLength();
    int padL = pad/2 + pDum->GetLength();
    int padR = inc;
    
    LAdjText(pDum, padL);
    RAdjText(pDum, padR);    
}


void RAdjText( CString* pDum, int Inc )
{
    while( pDum->GetLength() < Inc )
    {
        *pDum = _T(" ") + *pDum;
    }
    *pDum = pDum->Left( Inc );
}

void LAdjText( CString* pDum, int Inc )
{
	while( pDum->GetLength() < Inc )
	{
		*pDum = *pDum + _T(" ");
	}
	*pDum = pDum->Left( Inc );
}


void SetupSensors(MeasureInParam& inParam)
{
    int temp = inParam.ObjNo[0];
    double dTemp = inParam.FiObj[0];
    inParam.ObjNo[0] = inParam.RefObjNo;
    inParam.FiObj[0] = inParam.FiRef;    
    inParam.rotateBack[0] = inParam.rotateBackRef;    

    for( int i=0; i<=inParam.CountObj; i++ )
    {
        if(IsSensor(inParam.ObjNo[i]))
        {
            Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(inParam.ObjNo[i]));
            if(pSensor)
            {
                pSensor->SetCurrentAz(inParam.FiObj[i]);               
                pSensor->SetRotateBackAng(inParam.rotateBack[i], inParam.rotateBack[i]!=0);               
            }
        }   
    }    
    //Restore 0
    inParam.ObjNo[0] = temp;
    inParam.FiObj[0] = dTemp;

    ElevationData::elevErrorExist = g_AlignerData.ElevErrorExist;
    ElevationData::useElev = inParam.UseElev;
    ElevationData::mode = inParam.Mode1;
    //     pSensor->m_elevData.useElev = inParam.UseElev; 
//     pSensor->m_elevData.elevErrorExist = g_AlignerData.ElevErrorExist;
}


BOOL InitGlobalSensorStrings( BOOL reset /*= FALSE*/ )
{
	int arrayIndex;
	BOOL setupChanged = FALSE;
	int noOfConnectedStations = 0;
	CString channelName;

	Syncro* pSyncro;
	
	for( arrayIndex=FIRST_ARRAY_INDEX_SYNCRO; arrayIndex<=LAST_ARRAY_INDEX_SYNCRO; arrayIndex++ )
	{
		channelName = GetChannelName( arrayIndex );

		if( (pSyncro = DAU::GetDAU().GetSyncro( channelName )) != NULL )
		{
			if( GetUnitTypeSerialNumber( arrayIndex ) != pSyncro->GetSerialNumber() )
			{
				setupChanged = TRUE;
		    	SetUnitTypeSerialNumber( arrayIndex, pSyncro->GetSerialNumber() );
			}

            if( g_AlignerData.PlaneType[arrayIndex] != pSyncro->GetType() )
            {
                setupChanged = TRUE;
                g_AlignerData.PlaneType[arrayIndex] = pSyncro->GetType();
            }

			if( GetUnitTypeDescription( arrayIndex ) != pSyncro->GetUnitTypeDescription() )
            {
                setupChanged = TRUE;
				SetUnitTypeDescription( arrayIndex, pSyncro->GetUnitTypeDescription() );
            }
        }
        else if( IsConnected( channelName ) == TRUE )
        {
            SetNotConnected( channelName );
            setupChanged = TRUE;
        }

        if( IsConnected( channelName ) == TRUE )
        {
        noOfConnectedStations++;
        }
    }

    Sensor* pSensor;
    for( int arrayIndex=FIRST_ARRAY_INDEX_SENSOR; arrayIndex<=LAST_ARRAY_INDEX_SENSOR; arrayIndex++ )
    {
        channelName = GetChannelName( arrayIndex );

        if( (pSensor = DAU::GetDAU().GetSensor( channelName )) != NULL )
        {
		    if( GetUnitTypeSerialNumber( arrayIndex ) != pSensor->GetSerialNumber() )
            {
                setupChanged = TRUE;
				SetUnitTypeSerialNumber( arrayIndex, pSensor->GetSerialNumber() );
            }
			if( g_AlignerData.PlaneType[arrayIndex] != pSensor->GetType() )
            {
                setupChanged = TRUE;
                g_AlignerData.PlaneType[arrayIndex] = pSensor->GetType() ;
            }

			if( GetUnitTypeDescription( arrayIndex ) != pSensor->m_description )
            {
                setupChanged = TRUE;
				SetUnitTypeDescription( arrayIndex, pSensor->m_description );
            }

			if( GetGunAdapterNumber( arrayIndex ) != pSensor->GetAdapterSerialNumber() )
            {
                setupChanged = TRUE;
				SetGunAdapterNumber( arrayIndex, pSensor->GetAdapterSerialNumber() );
            }
        }
        else if( IsConnected( channelName ) == TRUE )
        {
            SetNotConnected( channelName );
            setupChanged = TRUE;
        }

        if( IsConnected( channelName ) == TRUE )
        {
            noOfConnectedStations++;
        }
  }

  if( ( reset == TRUE ) || (setupChanged == TRUE) )
  {
		g_AlignerData.GunAdapterTiltCheckOK = FALSE;
        CheckAndResetMeasurementSetups( UndefinedMode );
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            CSetupLiveDataGraphDlg setupLiveDataGraphDlg;
            setupLiveDataGraphDlg.SetFirstTime( TRUE );
        }
  }

  return (noOfConnectedStations != 0);  
}

void InitSystemSetupData()
{
  g_AlignerData.Place = SysSetup->GetPlace() ;
  g_AlignerData.DauNo = DAU::GetDAU().GetSerialNumber() ;
  g_AlignerData.Operator = SysSetup->GetOperatorName() ;
  g_AlignerData.Ship = SysSetup->GetShipName() ;
	
  SetMRad( SysSetup->GetUnits() == 0 );
  
}

BOOL CheckSystemSetupData( void )
{
  CString text = _T(""), tmp;
  
  if( g_AlignerData.Operator.GetLength() == 0 )
  {
    tmp.LoadString( IDS_SPECIFY_OPERATOR );
    text += tmp + _T("\n");
  }
  if( g_AlignerData.Place.GetLength() == 0 )
  {
    tmp.LoadString( IDS_SPECIFY_PLACE );
    text += tmp + _T("\n");
  }
  if( g_AlignerData.DauNo.GetLength() == 0 )
  {
    tmp.LoadString( IDS_SPECIFY_DAU_SN );
    text += tmp + _T("\n");
  }
  if( g_AlignerData.Ship.GetLength() == 0 )
  {
    tmp.LoadString( IDS_SPECIFY_SHIP );
    text += tmp + _T("\n");
  }

  int noOfCh = 0;

  for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    if( IsConnected( i ) == TRUE )
    {
      noOfCh++;
    }
  }

  if( noOfCh == 0 )
  {
    CheckAndResetMeasurementSetups( UndefinedMode );
		return( FALSE );
  }
  
  if( text == _T("") )
  {
    return( TRUE );
  }
  else
  {
    return( FALSE );
  }
}

void CheckAndResetMeasurementSetups( ALIGNMENT_MODE nextAlignmentMode )
{
	ALIGNMENT_MODE currentAlignmentMode = theApp.GetAlignmentMode();
	BOOL force = FALSE;
    /***  Always force setup for Foundation  ***/
	if (currentAlignmentMode == TiltFlatnesstestFoundationMode || currentAlignmentMode == AzimuthAlignMode)
        force = TRUE;
	    
    if(nextAlignmentMode != UndefinedMode && nextAlignmentMode != LiveDataMode)
        DAU::GetDAU().InitDataCompensator(TRUE);
    else
        DAU::GetDAU().InitDataCompensator(FALSE);

    DAU::GetDAU().ResetSensorCompensations();    

    if( (nextAlignmentMode != UndefinedMode) && (currentAlignmentMode == nextAlignmentMode) && !force )
	{
		return;
	}

	for( int i=0; i<SIZE_OF_ARRAYS; i++ )
	{
		SetEal( i, INVALID_VALUE );
		SetEac( i, INVALID_VALUE );
		g_AlignerData.EalS[i] = INVALID_VALUE;
		SetEalAbs( i, INVALID_VALUE );
		g_AlignerData.ObjNo[i] = 0;
		SetZPar( i, 0 );
	}
	g_AlignerData.NoOfCorr = 0;
	g_AlignerData.RefObjNo = -1;
	g_AlignerData.TestNotSetup = TRUE;
	g_AlignerData.LogData.ResultFromFirstMeasurementExists = FALSE;
	g_AlignerData.UseParallax = FALSE;

	CSetupAxesDlg setupAxesDlg;
	if( nextAlignmentMode == GunAdapterTiltCheckMode )
	{
		if( GetMRad() == TRUE )
		{
			setupAxesDlg.SetMinMaxY( -50, 50 );
		}
		else
		{
			int arcmin;
			arcmin = RoundToInt( MRADIANS_TO_ARCMIN( 50 ) );
			setupAxesDlg.SetMinMaxY( -arcmin, arcmin );
		}
	}
	else
	{
		if( GetMRad() == TRUE )
		{
			setupAxesDlg.SetMinMaxY( DEFAULT_MIN_Y_MRAD, DEFAULT_MAX_Y_MRAD );
		}
		else
		{
			setupAxesDlg.SetMinMaxY( DEFAULT_MIN_Y_ARCMIN, DEFAULT_MAX_Y_ARCMIN );
		}
		g_AlignerData.RY = 0; //Autoscale
	}

	g_AlignerData.RX = (nextAlignmentMode == GyroPerformanceTestMode) ? 600 : 0; 
	
	theApp.SetAlignmentMode( nextAlignmentMode );
}

void ResetAllFilters( DAU *pSource, double Ts, double Teta, double Tao )
{
	ASSERT( pSource != NULL );
	for( int i=0; i<DAU_NB_SENSOR_CHANNELS; i++ )
  {
          {
//            CString debugText ;
//            debugText.Format( _T("KKK111 %f\n"), Tao )  ;
//            ::OutputDebugString( debugText ) ;
          }
    pSource->GetSensor( i )->ResetFilter( Ts, Teta, Tao );
  }

	for( int i=0; i<DAU_NB_SYNCRO_CHANNELS; i++ )
  {
		pSource->GetSyncro( i )->ResetFilter( Ts, Teta, Tao );
  }
}

void ResetAllFilters( DAU *pSource )
{
	ASSERT( pSource != NULL );

  for( int i=0; i<DAU_NB_SENSOR_CHANNELS; i++ )
  {
    pSource->GetSensor( i )->ResetFilter();
  }

  for( int i=0; i<DAU_NB_SYNCRO_CHANNELS; i++ )
  {
		pSource->GetSyncro( i )->ResetFilter();
  }
}

BOOL IsPlaneOrGunConnected()
{
  BOOL planeOrGunConnected = FALSE;

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    if( (IsPlane( i ) == TRUE) || (IsGun( i ) == TRUE) )
    {
      planeOrGunConnected = TRUE;
      break;
    }
  }
  return( planeOrGunConnected );
}

BOOL IsFixedConnected()
{
  BOOL fixedConnected = FALSE;

  for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    if( IsFixed( i ) == TRUE )
    {
      fixedConnected = TRUE;
      break;
    }
  }
  return( fixedConnected );
}

int GetNoOfConnectedStations()
{
  int NoOfConnectedStations = 0;

  for( int i=FIRST_ARRAY_INDEX_SYNCRO; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
  {
    if( IsConnected( i ) == TRUE )
    {
      NoOfConnectedStations++;
    }
  }
  return( NoOfConnectedStations );
}

void GetMeasurementSetup( DAU *pSource, BOOL enableReference, CString* pSetup, BOOL commonFlatTest /*= FALSE*/ )
{
    CString tmpStr, tmpStr1, tmpStr2;
    pSetup->LoadString( ( commonFlatTest == TRUE ) ? IDS_SELECTED_REFERENCE_SENSOR :IDS_SELECTED_REFERENCE);
  
    *pSetup += _T("\n");

    if( enableReference == TRUE )
    {
        if( commonFlatTest == TRUE )
        {
			AfxFormatString1( tmpStr1, IDS_SN_XXX, GetSensorSerialNumber( g_AlignerData.RefObjNo ) );
			AfxFormatString2( tmpStr, IDS_SENSOR_NAME_TO_DEVICE, tmpStr1, (GetUnitTypeDescription( g_AlignerData.RefObjNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
        }
        else
        {
            tmpStr1 = GetChannelName( g_AlignerData.RefObjNo );
		    tmpStr2 = (GetUnitTypeDescription( g_AlignerData.RefObjNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            AfxFormatString2( tmpStr, IDS_SENSOR_NAME_TO_DEVICE, tmpStr1, tmpStr2 );
        }
    }
    else
    {
        tmpStr.LoadString( IDS_SYMBOLS_FOR_EMPTY_STRING );
    }
    *pSetup +=  tmpStr + _T("\n\n");

    tmpStr.LoadString( ( commonFlatTest == TRUE ) ? IDS_SELECTED_SENSORS : IDS_SELECTED_STATIONS );
    
    *pSetup += tmpStr + _T("\n");

    for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
    {
        if( commonFlatTest == TRUE )
        {
			AfxFormatString1( tmpStr1, IDS_SN_XXX, GetSensorSerialNumber( g_AlignerData.ObjNo[i]) );
			AfxFormatString2( tmpStr, IDS_SENSOR_NAME_TO_DEVICE, tmpStr1, (GetUnitTypeDescription( g_AlignerData.ObjNo[i] )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
        }
        else
        {
            tmpStr1 = GetChannelName( g_AlignerData.ObjNo[i]);
		    tmpStr2 = (GetUnitTypeDescription( g_AlignerData.ObjNo[i] )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            AfxFormatString2( tmpStr, IDS_SENSOR_NAME_TO_DEVICE, tmpStr1, tmpStr2 );
        }
        *pSetup += tmpStr + _T("\n");
    }
}

Digital* GetDigital(CString name)
{
    return dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(name));    
}


CString GetTypeName( int arrayIndex )
{
    CString tmpStr;
    int sI;
    tmpStr = _T("");

    if( IsSyncro( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );
        Digital* pDigital = GetDigital(DAU::GetDAU().GetSyncro( sI )->GetName());
        if(pDigital)
        {
            tmpStr = pDigital->GetDigTypeText();
        }
        else
        {
            tmpStr = UnitType::GetUnitText(DAU::GetDAU().GetSyncro( sI )->GetType());
        }
    }
    else if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        tmpStr = UnitType::GetUnitText(DAU::GetDAU().GetSensor( sI )->GetType());
    }
    return( tmpStr );
}

BOOL IsDigital( int arrayIndex )
{    
    if( IsSyncro( arrayIndex ) == TRUE )
    {
        int sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );
 
        Digital* pDigital = GetDigital(DAU::GetDAU().GetSyncro( sI )->GetName());
        if(pDigital)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

CString GetChannelName( int arrayIndex )
{
    CString tmpStr;
    int sI;
    tmpStr = _T("");

	if( IsSyncro( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );
        tmpStr = DAU::GetDAU().GetSyncro( sI )->GetName();
        
    }
    else if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        tmpStr = DAU::GetDAU().GetSensor( sI )->GetName();
    }
    return( tmpStr );
}

CString GetSensorSerialNumber( int arrayIndex)
{
    CString tmpStr;
    int sI;
    tmpStr = _T("");

    if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        tmpStr = DAU::GetDAU().GetSensor( sI )->GetSerialNumber();
    }
    return( tmpStr );
}

int GetChannelNumber( int arrayIndex )
{
  int sI = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );
  }
  else if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
  }
  return( sI );
}

void SaveResultToDatabase( void )
{
}

void MoveDlgToRightSideOfApp(CWnd *pWnd)
{
    CRect mainWndRect, dlgRect;
    theApp.m_pMainWnd->GetWindowRect( &mainWndRect );
    pWnd->GetWindowRect( &dlgRect );
    int graphWidth;
    
    int newLeft, newTop;
    newTop = mainWndRect.bottom-dlgRect.Height();
    newLeft = mainWndRect.right - dlgRect.Width();
    pWnd->MoveWindow( newLeft, newTop, dlgRect.Width(), dlgRect.Height() );
    
    pWnd->Invalidate();
}

void MoveDlgToRightSideOfTheGraph( CWnd *pWnd, CFormView* pGraph )
{
  CRect mainWndRect, dlgRect;
  theApp.m_pMainWnd->GetWindowRect( &mainWndRect );
  pWnd->GetWindowRect( &dlgRect );
  int graphWidth;
  if( pGraph == NULL )
  {
    graphWidth = (int)((CAlignmentWizard *)pWnd)->m_pGraph->GetControlWidth();
  }
  else
  {
    graphWidth = (int)((CGraphView *)pGraph)->GetControlWidth();
  }
  int newLeft, newTop;
  newTop = (mainWndRect.bottom + mainWndRect.top)/2 - dlgRect.Height()/2;
  newLeft = mainWndRect.right - ( dlgRect.Width() / 3 );
  if( (newLeft < graphWidth) || (graphWidth <= 0) )
  {
    pWnd->MoveWindow( newLeft, newTop, dlgRect.Width(), dlgRect.Height() );
  }
  else
  {
    pWnd->MoveWindow( graphWidth, newTop, dlgRect.Width(), dlgRect.Height() );
  }
  pWnd->Invalidate();
}

void MoveDlgToTheCenterOfMainWnd( CWnd *pWnd )
{
  CRect mainWndRect, dlgRect;
  theApp.m_pMainWnd->GetWindowRect( &mainWndRect );
  pWnd->GetWindowRect( &dlgRect );
  int newLeft, newTop;
  newTop = (mainWndRect.bottom + mainWndRect.top)/2 - dlgRect.Height()/2;
  newLeft = (mainWndRect.left + mainWndRect.right)/2 - dlgRect.Width()/2;
  pWnd->MoveWindow( newLeft, newTop, dlgRect.Width(), dlgRect.Height() );
  pWnd->Invalidate();
}

double AdjustDegAngle( double angle, BOOL range0ToPlusMinus180, int signDef )
{
	double angleOut = angle;

	if( range0ToPlusMinus180 == FALSE )
	{
		angleOut = GetRange0ToPlus360( angleOut );
	}
	else
	{
		angleOut = GetRange0ToPlusMinus180( angleOut );
	}

	if( signDef == -1 )
	{
		if( range0ToPlusMinus180 == FALSE )
		{
			if( angleOut > 180 )
			{
				angleOut -= 180;
			}
			else
			{
				angleOut += 180;
			}
		}
		else
		{
			if( angleOut > 0 )
			{
				angleOut -= 180;
			}
			else
			{
				angleOut += 180;
			}
		}
	}

  return( angleOut );
}

//Setup functions
int GetArrayIndex( CString channelName )
{
  for( int i=0; i<DAU_NB_SYNCRO_CHANNELS; i++ )
  {
    if( channelName == DAU::GetDAU().GetSyncro( i )->GetName() )
    {
      return( CONVERT_SYNCRO_CHANNEL_TO_ARRAY_INDEX( i ) );
    }
  }

  for( int i=0; i<DAU_NB_SENSOR_CHANNELS; i++ )
  {
    if( channelName == DAU::GetDAU().GetSensor( i )->GetName() )
    {
      return( CONVERT_SENSOR_CHANNEL_TO_ARRAY_INDEX( i ) );
    }
  }
  return( -1 );
}

BOOL IsSelected( CString channelName, BOOL asReference/* = TRUE*/, BOOL asToBeTested/* = TRUE*/ )
{
	if( asReference == TRUE )
	{
		if( IsConnected( g_AlignerData.RefObjNo ) == TRUE )
		{
			if( GetChannelName( g_AlignerData.RefObjNo ) == channelName )
			{
				return( TRUE );
			}
		}
	}

	if( asToBeTested == TRUE )
	{
		for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
		{
			if( IsConnected( g_AlignerData.ObjNo[i] ) == TRUE )
			{
				if( GetChannelName( g_AlignerData.ObjNo[i] ) == channelName )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}

BOOL IsConnected( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsConnected( arrayIndex ) );
}

BOOL IsConnected( int arrayIndex )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        if( g_AlignerData.PlaneType[arrayIndex] != UnitType::Unused)
        {
            return  TRUE;
        }
    }
    return FALSE;
}

UnitType::Types GetUnitType(int arrayIndex)
{
	if (IsValidChannel(arrayIndex) == TRUE)
	{		
		return  g_AlignerData.PlaneType[arrayIndex];		
	}
	return UnitType::Unused;
}


BOOL IsFixed( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsFixed( arrayIndex ) );
}


BOOL IsFixed( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    if( g_AlignerData.PlaneType[arrayIndex] == PLANE_TYPE_FIXED )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

BOOL IsTheo(CString channelName)
{
	int arrayIndex;
	arrayIndex = GetArrayIndex(channelName);
	return(IsTheo(arrayIndex));
}

BOOL IsTheo(int arrayIndex)
{
	if (IsValidChannel(arrayIndex) == TRUE)
	{
		if (g_AlignerData.PlaneType[arrayIndex] == PLANE_TYPE_THEO)
		{
			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL IsPlane( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsPlane( arrayIndex ) );
}

BOOL IsPlane( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    if( g_AlignerData.PlaneType[arrayIndex] == PLANE_TYPE_PLANE )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

BOOL IsGun( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsGun( arrayIndex ) );
}

BOOL IsGun( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    if( g_AlignerData.PlaneType[arrayIndex] == PLANE_TYPE_GUN )
    {
      return( TRUE );
    }
  }
  return( FALSE );
}

BOOL IsSensor( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsSensor( arrayIndex ) );
}

BOOL IsSensor( int arrayIndex )
{
  if( ( arrayIndex >= FIRST_ARRAY_INDEX_SENSOR ) && 
      ( arrayIndex <= LAST_ARRAY_INDEX_SENSOR ) )
	{
		return( TRUE );
	}
	return( FALSE );
}

BOOL IsSyncro( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( IsSyncro( arrayIndex ) );
}

BOOL IsSyncro( int arrayIndex )
{
  if( ( arrayIndex >= FIRST_ARRAY_INDEX_SYNCRO ) && 
      ( arrayIndex <= LAST_ARRAY_INDEX_SYNCRO ) )
	{
		return( TRUE );
	}
	return( FALSE );
}

BOOL IsValidChannel( int arrayIndex )
{
	if( IsSyncro( arrayIndex ) == TRUE )
	{
		return( TRUE );
	}

	if( IsSensor( arrayIndex ) == TRUE )
	{
		return( TRUE );
	}
	return( FALSE );
}

void SetNotConnected( CString channelName )
{
    int arrayIndex;
    arrayIndex = GetArrayIndex( channelName );

    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        g_AlignerData.PlaneType[arrayIndex] = PLANE_TYPE_EMPTY;
    }
}

CString GetUnitTypeDescription( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( GetUnitTypeDescription( arrayIndex ) );
}

CString GetUnitTypeDescription( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.Device[arrayIndex] );
  }
	return( DEVICE_EMPTY );
}

void SetUnitTypeDescription( int arrayIndex, CString UnitTypeDescription )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    g_AlignerData.Device[arrayIndex] = UnitTypeDescription;
  }
	return;
}

CString GetUnitTypeSerialNumber( CString channelName )
{
  int arrayIndex;
  arrayIndex = GetArrayIndex( channelName );
  return( GetUnitTypeSerialNumber( arrayIndex ) );
}

CString GetUnitTypeSerialNumber( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.Incl[arrayIndex] );
  }
	return( INCL_EMPTY );
}

void SetUnitTypeSerialNumber( int arrayIndex, CString UnitTypeSerialNumber )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
		g_AlignerData.Incl[arrayIndex] = UnitTypeSerialNumber;
  }
	return;
}

double GetZPar( int arrayIndex )
{
    if(IsSensor(arrayIndex))
    {
        Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(arrayIndex));
        if(pSensor) return pSensor->GetParallaxData().z;
        
    }
    return 0.0f;
//     if( IsValidChannel( arrayIndex ) == TRUE )
//   {
//     return( g_AlignerData.ZPar[arrayIndex] );
//   }
// 	return( 0 );
}

void SetZPar( int arrayIndex, double zPar )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
		g_AlignerData.ZPar[arrayIndex] = zPar;
  }
	return;
}

//int GetZeroRef( int arrayIndex )
double GetZeroRef( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.ZeroRef[arrayIndex] );
  }
	return( 0 );
}

//void SetZeroRef( int arrayIndex, int zeroRef )
void SetZeroRef( int arrayIndex, double zeroRef )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
		g_AlignerData.ZeroRef[arrayIndex] = zeroRef;
  }
	return;
}

// int GetIndexArm2Length( int arrayIndex )
// {
//     if( IsValidChannel( arrayIndex ) == TRUE )
//     {
//         return( g_AlignerData.IA2_Len[arrayIndex] );
//     }
//     return( 0 );
// }

// void SetIndexArm2Length( int arrayIndex, int indexArmLength )
// {
//     if( IsValidChannel( arrayIndex ) == TRUE )
//     {
//         g_AlignerData.IA2_Len[arrayIndex] = indexArmLength;
//     }
//     return;
// }

int GetIndexArmLength( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.IA_Len[arrayIndex] );
  }
	return( 0 );
}

void SetIndexArmLength( int arrayIndex, int indexArmLength )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
		g_AlignerData.IA_Len[arrayIndex] = indexArmLength;
    }
	return;
}

CString GetGunAdapterNumber( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
		return( g_AlignerData.GunAdap[arrayIndex] );
  }
	return( GUN_ADAP_EMPTY );
}

void SetGunAdapterNumber( int arrayIndex, CString gunAdapterNumber )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
		g_AlignerData.GunAdap[arrayIndex] = gunAdapterNumber;
    }
}

double GetEal( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.Eal[arrayIndex] );
  }
	return( 0 );
}

void SetEal( int arrayIndex, double eal )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        g_AlignerData.Eal[arrayIndex] = eal;
        if(IsSensor(arrayIndex))
        {
            Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(arrayIndex));
            if(pSensor) pSensor->m_elevData.elevAlong = MRADIANS_TO_RADIANS(-eal);
        }
    }
}

double GetEac( int arrayIndex )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        return( g_AlignerData.Eac[arrayIndex] );
    }
	return( 0 );
}

void SetEac( int arrayIndex, double eac )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        g_AlignerData.Eac[arrayIndex] = eac;
        if(IsSensor(arrayIndex))
        {
            Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(arrayIndex));
            if(pSensor) pSensor->m_elevData.elevAcross = MRADIANS_TO_RADIANS(-eac);
        }
    
    }
	return;
}

double GetEalAbs( int arrayIndex )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        return( g_AlignerData.EalAbs[arrayIndex] );
    }
	return( 0 );
}

void SetEalAbs( int arrayIndex, double ealAbs )
{
    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        g_AlignerData.EalAbs[arrayIndex] = ealAbs;
        if(IsSensor(arrayIndex))
        {
            Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(arrayIndex));
            if(pSensor)
            {                
                pSensor->m_elevData.elevAlongAbs = (ealAbs == INVALID_VALUE) ? INVALID_VALUE : MRADIANS_TO_RADIANS(-ealAbs);
            }
        }
    }
}

double ReadTemperature( int arrayIndex )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
    return( g_AlignerData.Tsens[arrayIndex] );
  }
	return( 0 );
}

void SetTemperature( int arrayIndex, double temperature )
{
  if( IsValidChannel( arrayIndex ) == TRUE )
  {
		g_AlignerData.Tsens[arrayIndex] = temperature;
  }
	return;
}

void CopyTemperature( double* tempToCopy )
{
	for( int i=0; i<SIZE_OF_ARRAYS; i++ )
	{
		g_AlignerData.Tsens[i] = tempToCopy[i];
	}
}

double GetTemperature( int arrayIndex, const DAUValues &values )
{
	int sI ;
  double temperatureValue = TEMPERATURE_NOT_AVAILABLE ;

  if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex ) ;
		if (sI < values.GetSensorCount())
		{
			temperatureValue = values.GetSensor( sI ).GetTemperature() ;
		}
  }
	return temperatureValue ;
}

double GetRoll( int arrayIndex, const DAUValues &values )
{
	int sI;
  double rollValue = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
		rollValue = sI < values.GetSyncroCount() ? RADIANS_TO_MRADIANS( values.GetSyncro( sI ).GetRoll() ) : 0.0 ;
  }
  else if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
		rollValue = sI < values.GetSensorCount() ? RADIANS_TO_MRADIANS( values.GetSensor( sI ).GetRoll() ) : 0.0 ;
  }
	return rollValue ;
}

double GetUncalibratedRoll( int arrayIndex, const DAUValues &values )
{
	int sI;
    double val = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
    {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
        val = sI < values.GetSyncroCount() ? RADIANS_TO_MRADIANS( values.GetSyncro( sI ).GetUncalibratedRoll() ) : 0.0 ;
    }
    else if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        val = sI < values.GetSensorCount() ? RADIANS_TO_MRADIANS( values.GetSensor( sI ).GetUncalibratedRoll() ) : 0.0 ;
    }
	return val ;
}

double GetUncalibratedPitch( int arrayIndex, const DAUValues &values )
{
    int sI;
    double val = 0;

    if( IsSyncro( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
        val = sI < values.GetSyncroCount() ? RADIANS_TO_MRADIANS( values.GetSyncro( sI ).GetUncalibratedPitch() ) : 0.0 ;
    }
    else if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        val = sI < values.GetSensorCount() ? RADIANS_TO_MRADIANS( values.GetSensor( sI ).GetUncalibratedPitch() ) : 0.0 ;
    }
    return val ;
}

double GetUncalibratedHeading( int arrayIndex, const DAUValues &values )
{
    int sI;
    double val = 0;

    if( IsSyncro( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
        val = sI < values.GetSyncroCount() ? RADIANS_TO_MRADIANS( values.GetSyncro( sI ).GetUncalibratedHeading() ) : 0.0 ;
    }
    else if( IsSensor( arrayIndex ) == TRUE )
    {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
        val = sI < values.GetSensorCount() ? RADIANS_TO_MRADIANS( values.GetSensor( sI ).GetUncalibratedHeading() ) : 0.0 ;
    }
    return val ;
}


double GetPitch( int arrayIndex, const DAUValues &values )
{
	int sI;
  double pitchValue = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex ) ;
		pitchValue = sI < values.GetSyncroCount() ? RADIANS_TO_MRADIANS( values.GetSyncro( sI ).GetPitch() ) : 0.0 ;
  }
  else if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );
		pitchValue = sI < values.GetSensorCount() ? RADIANS_TO_MRADIANS( values.GetSensor( sI ).GetPitch() ) : 0.0 ;
  }
	return pitchValue ;
}

double GetNominalAzimuthDegree( int arrayIndex, DAU *pSource /*= NULL*/ )
{
	int sI;
  double azimuth = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );

    if( pSource == NULL )
    {
			azimuth = DAU::GetDAU().GetSyncro( sI )->GetNominalAzimuthDegree();
    }
    else
    {
			azimuth = pSource->GetSyncro( sI )->GetNominalAzimuthDegree();
		}
  }
  else if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );

    if( pSource == NULL )
    {
			azimuth = DAU::GetDAU().GetSensor( sI )->GetNominalAzimuthDegree();
    }
    else
    {
			azimuth = pSource->GetSensor( sI )->GetNominalAzimuthDegree();
		}
  }
	return( azimuth );
}

double GetNominalAzimuthMRad( int arrayIndex, DAU *pSource /*= NULL*/ )
{
	int sI;
  double azimuth = 0;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );

    if( pSource == NULL )
    {
			azimuth = DAU::GetDAU().GetSyncro( sI )->GetNominalAzimuthRad();
    }
    else
    {
			azimuth = pSource->GetSyncro( sI )->GetNominalAzimuthRad();
		}
  }
  else if( IsSensor( arrayIndex ) == TRUE )
  {
    sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( arrayIndex );

    if( pSource == NULL )
    {
			azimuth = DAU::GetDAU().GetSensor( sI )->GetNominalAzimuthRad();
    }
    else
    {
			azimuth = pSource->GetSensor( sI )->GetNominalAzimuthRad();
		}
  }

	return( RADIANS_TO_MRADIANS( azimuth ) );
}

double GetGearing( int arrayIndex, DAU *pSource /*= NULL*/ )
{
	int sI;
  double gearing = 1;

	if( IsSyncro( arrayIndex ) == TRUE )
  {
		sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( arrayIndex );

    if( pSource == NULL )
    {
			gearing = DAU::GetDAU().GetSyncro( sI )->GetGearing();
    }
    else
    {
			gearing = pSource->GetSyncro( sI )->GetGearing();
		}
  }
	return( gearing );
}

void SetSelectedChannels( DAU *pSource /*= NULL*/, BOOL fromLiveGraphData )
{
    DAU *pDau;
    if( pSource == NULL )
    {
        pDau = &(DAU::GetDAU());
    }
    else
    {
        pDau = pSource;
    }

    ClearSelectedChannels( pDau );

    int sI;

    if( g_AlignerData.RefObjNo != -1 )
    {
        if( IsSyncro( g_AlignerData.RefObjNo ) == TRUE )
        {
        sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( g_AlignerData.RefObjNo );
        pDau->GetSyncro( sI )->SetIsSelected( TRUE );
        pDau->GetSyncro( sI )->SetOverrangeDetectionActive(TRUE);
        }
        else
        {
        sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( g_AlignerData.RefObjNo );
        pDau->GetSensor( sI )->SetIsSelected( TRUE );
        pDau->GetSensor( sI )->SetOverrangeDetectionActive(TRUE);
        }
    }
    
    if(fromLiveGraphData)
    {
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
        {
            if( IsSyncro( iter->second.id ) == TRUE )
            {
                sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( iter->second.id );
                pDau->GetSyncro( sI )->SetIsSelected( TRUE );
                pDau->GetSyncro( sI )->SetOverrangeDetectionActive(TRUE);
            }
            else
            {
                sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( iter->second.id );
                pDau->GetSensor( sI )->SetIsSelected( TRUE );
                pDau->GetSensor( sI )->SetOverrangeDetectionActive(TRUE);
            }
        }

    }
    else
    {
        g_AlignerData.liveGraphMap.clear();  
        for( int i=1; i<=g_AlignerData.NoOfCorr; i++ )
        {        
            CString chName = GetChannelName(g_AlignerData.ObjNo[i]);;
            g_AlignerData.liveGraphMap[g_AlignerData.ObjNo[i]] = LiveGraphInfo(chName, g_AlignerData.ObjNo[i]);

            if( IsSyncro( g_AlignerData.ObjNo[i] ) == TRUE )
            {
                sI = CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL( g_AlignerData.ObjNo[i] );
                pDau->GetSyncro( sI )->SetIsSelected( TRUE );
                pDau->GetSyncro( sI )->SetOverrangeDetectionActive(TRUE);
            }
            else
            {
                sI = CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL( g_AlignerData.ObjNo[i] );
                pDau->GetSensor( sI )->SetIsSelected( TRUE );
                pDau->GetSensor( sI )->SetOverrangeDetectionActive(TRUE);
            }
        }
    }
    

    
    
  return;
}

void ClearSelectedChannels( DAU *pSource /*= NULL*/ )
{
  DAU *pDau;
  if( pSource == NULL )
  {
    pDau = &(DAU::GetDAU());
  }
  else
  {
    pDau = pSource;
  }

  for( int i=0; i<pDau->GetSyncroCount(); i++ )
  {
    pDau->GetSyncro( i )->SetIsSelected( FALSE );
    pDau->GetSyncro( i )->SetOverrangeDetectionActive(FALSE);
  }
  for( int i=0; i<pDau->GetSensorCount(); i++ )
  {
    pDau->GetSensor( i )->SetIsSelected( FALSE );
    pDau->GetSensor( i )->SetOverrangeDetectionActive(FALSE);
  }

  return;
}



void SetUsedAsSelectedChannels(DAU *pSource /*= NULL*/)
{
	DAU *pDau = (pSource == NULL) ? &(DAU::GetDAU()) : pSource;

	for (int i = 0; i<pDau->GetSyncroCount(); i++)
	{
		Syncro* pSync = pDau->GetSyncro(i);
		pSync->SetIsSelected(pSync->GetType() != UnitType::Unused);
		pSync->SetOverrangeDetectionActive(FALSE);
	}
	for (int i = 0; i<pDau->GetSensorCount(); i++)
	{
		Sensor* pSensor = pDau->GetSensor(i);
		pSensor->SetIsSelected(pSensor->GetType() != UnitType::Unused);
		pSensor->SetOverrangeDetectionActive(FALSE);
	}

	return;
}



void ResetAllNomAz()
{
	DAU *pDau = &(DAU::GetDAU());

	for( int i=0; i<pDau->GetSensorCount(); i++ )
	{
		Sensor* pSensor = pDau->GetSensor(i);
		pSensor->SetNominalAzimuth(0.0f);		
	}
	
}


void SetMRad( BOOL mRad )
{	
	if( mRad == TRUE )
	{
		g_AlignerData.Kh = 1;
		SysSetup->SetUnits( UNIT_MRAD );
	}
	else
	{
		g_AlignerData.Kh = MTA;
		SysSetup->SetUnits( UNIT_ARCMIN );
	}
}

BOOL GetMRad()
{
  return (SysSetup->GetUnits() == UNIT_MRAD);  
}


CDelay::CDelay()
{
  m_ready = TRUE;
}

CDelay::~CDelay()
{

}

void CDelay::StartDelay( DWORD delaySeconds )
{
	if( delaySeconds == 0 )
	{
		return;
	}
	m_delayTimeSeconds = delaySeconds;
	m_ready = FALSE;
	m_break = FALSE;
  
	do
	{
		DoEvents();
		Sleep( 1000 );
		m_delayTimeSeconds--;
	}while( ( m_delayTimeSeconds > 0 ) && ( m_break == FALSE ) );
	m_ready = TRUE;
}

void CDelay::ResetDelay( void )
{
	m_break = TRUE;
}

BOOL CDelay::IsReady( void )
{
	return( m_ready );
}

BOOL GetCompositeAngle( double coarseAngle, double fineAngle, double fineAngleGearing, double* pCompositeAngle )
{
  double baseIntervalSize, coarseAngleBase, angleError, compositeAngle;
  int IntegerCoarseAngleBase;

  if( fineAngleGearing == 0 )
  {
    *pCompositeAngle = 0;
    return( FALSE );
  }
  coarseAngle = RADIANS_TO_DEGREES( coarseAngle );
  fineAngle = RADIANS_TO_DEGREES( fineAngle );

  baseIntervalSize = 360 / fineAngleGearing;
  coarseAngleBase = coarseAngle / baseIntervalSize;
  IntegerCoarseAngleBase = RoundToInt( coarseAngleBase );
  compositeAngle = baseIntervalSize * IntegerCoarseAngleBase + fineAngle;
  angleError = compositeAngle - coarseAngle;

  if( angleError > 180 )
  {
    angleError -= 360;
  }
  else if( angleError < -180 )
  {
    angleError += 360;
  }

  if( angleError > baseIntervalSize/2 )
  {
    IntegerCoarseAngleBase--;
  }
  else if( angleError < -baseIntervalSize/2 )
  {
    IntegerCoarseAngleBase++;
  }
  compositeAngle = baseIntervalSize * IntegerCoarseAngleBase + fineAngle;

  if( compositeAngle > 180 )
  {
    compositeAngle -= 360;
  }
  else if( compositeAngle < -180 )
  {
    compositeAngle += 360;
  }
  *pCompositeAngle = DEGREES_TO_RADIANS( compositeAngle );
  return( TRUE );
}

void AdjustSyncroValueToRangeA202( double* pSyncroValue, double gearing )
{
  if( gearing < 1 )
  {
    gearing = 1;
  }
  double minValue, maxValue, range;
  range = 360 / gearing;
  *pSyncroValue -= ((long )(*pSyncroValue / range)) * range;

  if( theApp.IsRangeZeroToRangeA202() == TRUE ) // 0 <= syncroValue <= +360/gearing
  {
    minValue = 0;
    maxValue = 360 / gearing;
  }
  else // -180/gearing <= syncroValue <= +180/gearing
  {
    minValue = -180 / gearing;
    maxValue = 180 / gearing;
  }

  if( *pSyncroValue < minValue )
  {
    *pSyncroValue += range;
  }
  else if( *pSyncroValue > maxValue )
  {
    *pSyncroValue -= range;
  }
}

/*BOOL MoveFileToTemporaryDirectory( CString &fileNameWithPath )
{
    TRACE("MoveFileToTemporaryDirectory: %s\n",fileNameWithPath);
    CString path, filename;
    int position = fileNameWithPath.ReverseFind( '\\' );
    if( position < 0 )
    {
        char ca [MAX_PATH];
        GetCurrentDirectory( sizeof(ca), ca );
        path = ca;
        path += _T("\\");
        filename = fileNameWithPath;
    }
    else
    {
        if( fileNameWithPath.GetLength() - (position + 1) > 0 )
        {
            filename = fileNameWithPath.Right( fileNameWithPath.GetLength() - (position + 1) );
        }
        else
        {
            filename = fileNameWithPath.Right( position );
        }
        path = fileNameWithPath.Left( position );
        position = path.ReverseFind( '\\' );
        if( position < 0 )
        {
            char ca [MAX_PATH];
            GetCurrentDirectory( sizeof(ca), ca );
            path = ca;
            path += _T("\\");
        }
        else
        {
            path = path.Left( position + 1 );
        }
    }
    path += _T("IntermediateImages\\");
    std::string stdTemporaryPath = path ;

    if( ::CreateDirectory( stdTemporaryPath.c_str(), NULL ) == FALSE )
	{
        DWORD error = GetLastError();
        if(error != ERROR_ALREADY_EXISTS)
        {
            CString msg; msg.Format("Unable to create directory for temporary images, err:%d.",error);
            AfxMessageBox( msg ) ;
            return( FALSE );
        }        
	}

    path += filename;
    BOOL result = MoveFileEx( (LPCTSTR)fileNameWithPath, (LPCTSTR)path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED );

    //Also remove from DB
    DBInterface::Instance()->DeleteGraph(fileNameWithPath); 
    TRACE("Remove:%s\n",fileNameWithPath);

    return( result );

}
*/
CString GetFileName( CString &fileNameWithPath )
{
    CString filename;
    int position = fileNameWithPath.ReverseFind( '\\' );
    if( position < 0 )
    {
        filename = fileNameWithPath;
    }
    else
    {
        if( fileNameWithPath.GetLength() - (position + 1) > 0 )
        {
            filename = fileNameWithPath.Right( fileNameWithPath.GetLength() - (position + 1) );
        }
        else
        {
            filename = fileNameWithPath.Right( position );
        }
    }

    return( filename );
}

CString D1Name()
{
    return theApp.IsAligner202Enabled() ? CH3 : D1;
}

CString D2Name()
{
    return theApp.IsAligner202Enabled() ? CH4 : D2;
}


int ResolutionToBits(int res)
{
    return 10+(res*2);
}


CString GetDAUName()
{
	return theApp.IsAligner308Enabled() ? "DAU" : "DAB";
}


BOOL IsGunPresent()
{
	if ((g_AlignerData.NoOfCorr > 0) && (IsConnected(g_AlignerData.RefObjNo) == TRUE))
	{
		if (IsGun(g_AlignerData.RefObjNo) == TRUE)
		{
			return TRUE;
		}

		for (int i = 1; i <= g_AlignerData.NoOfCorr; i++)
		{
			if (IsGun(g_AlignerData.ObjNo[i]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


int AskGunAdapterTiltCheck()
{
	InfoDlg dlg;

	dlg.SetTitle("Notification");
	dlg.SetInfo("It is recommended that the 'Gun Adapter Tilt Check' is performed prior to the Alignment, to eliminate errors.\r\n\r\nDo you wish to run 'Gun Adapter Tilt Check'?");
	dlg.SetYesNoMode(TRUE);
	int result = dlg.DoModal();

	return result;


}