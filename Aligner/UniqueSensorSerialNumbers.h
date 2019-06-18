// $Id: UniqueSensorSerialNumbers.h,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
[
	db_command(L" \
	SELECT DISTINCT serialNumber FROM SensorCorrection ORDER BY serialNumber")
]
class UniqueSensorSerialNumbers
{
public:
	[ db_column(1, status = m_dwserialNumberStatus, length = m_dwserialNumberLength) ] TCHAR m_serialNumber[4] ;	// Serial number of callibrated sensor.
	DBSTATUS m_dwserialNumberStatus ;

	DBLENGTH m_dwserialNumberLength ;

	void GetRowsetProperties( CDBPropSet *pPropSet )
	{
		pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
	}
} ;
