#include "StdAfx.h"
#include "Setup.h"
#include "Util.h"
#include "GlobalData.h"

CSetup::CSetup(void)
{
}

CSetup::~CSetup(void)
{
}

BOOL CSetup::IsConnected( CString channelName )
{
    int arrayIndex;
    arrayIndex = GetArrayIndex( channelName );

    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        if( g_AlignerData.PlaneType[arrayIndex] != UnitType::Unused )
        {
            return( TRUE );
        }
    }
    return( FALSE );
}

void CSetup::SetNotConnected( CString channelName )
{
    int arrayIndex;
    arrayIndex = GetArrayIndex( channelName );

    if( IsValidChannel( arrayIndex  ) == TRUE )
    {
        g_AlignerData.PlaneType[arrayIndex] = UnitType::Unused;
    }
}

void CSetup::SetConnected( CString channelName )
{
    int arrayIndex;
    arrayIndex = GetArrayIndex( channelName );

    if( IsValidChannel( arrayIndex ) == TRUE )
    {
        g_AlignerData.PlaneType[arrayIndex] = UnitType::Fixed;
    }
}