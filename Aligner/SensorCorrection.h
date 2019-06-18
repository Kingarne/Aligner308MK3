// $Id: SensorCorrection.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once
[
	db_command(L" \
	SELECT ID, \
		pitchAzimut_0, pitchAzimut_1, pitchAzimut_2, pitchAzimut_3, \
		pitchGain_0, pitchGain_1, pitchGain_2, pitchGain_3, \
		pitchOffset_0, pitchOffset_1, pitchOffset_2, pitchOffset_3, \
		rollAzimut_0, rollAzimut_1, rollAzimut_2, rollAzimut_3, \
		rollGain_0, rollGain_1, rollGain_2, rollGain_3, \
		rollOffset_0, rollOffset_1, rollOffset_2, rollOffset_3, \
		serialNumber \
		FROM SensorCorrection")
]
class SensorCorrection
{
public:
	[ db_column(1, status = m_dwIDStatus, length = m_dwIDLength) ] LONG m_ID ;
	[ db_column(2, status = m_dwpitchAzimut_0Status, length = m_dwpitchAzimut_0Length) ] double m_pitchAzimut_0 ;
	[ db_column(3, status = m_dwpitchAzimut_1Status, length = m_dwpitchAzimut_1Length) ] double m_pitchAzimut_1 ;
	[ db_column(4, status = m_dwpitchAzimut_2Status, length = m_dwpitchAzimut_2Length) ] double m_pitchAzimut_2 ;
	[ db_column(5, status = m_dwpitchAzimut_3Status, length = m_dwpitchAzimut_3Length) ] double m_pitchAzimut_3 ;
	[ db_column(6, status = m_dwpitchGain_0Status, length = m_dwpitchGain_0Length) ] double m_pitchGain_0 ;
	[ db_column(7, status = m_dwpitchGain_1Status, length = m_dwpitchGain_1Length) ] double m_pitchGain_1 ;
	[ db_column(8, status = m_dwpitchGain_2Status, length = m_dwpitchGain_2Length) ] double m_pitchGain_2 ;
	[ db_column(9, status = m_dwpitchGain_3Status, length = m_dwpitchGain_3Length) ] double m_pitchGain_3 ;
	[ db_column(10, status = m_dwpitchOffset_0Status, length = m_dwpitchOffset_0Length) ] double m_pitchOffset_0 ;
	[ db_column(11, status = m_dwpitchOffset_1Status, length = m_dwpitchOffset_1Length) ] double m_pitchOffset_1 ;
	[ db_column(12, status = m_dwpitchOffset_2Status, length = m_dwpitchOffset_2Length) ] double m_pitchOffset_2 ;
	[ db_column(13, status = m_dwpitchOffset_3Status, length = m_dwpitchOffset_3Length) ] double m_pitchOffset_3 ;
	[ db_column(14, status = m_dwrollAzimut_0Status, length = m_dwrollAzimut_0Length) ] double m_rollAzimut_0 ;
	[ db_column(15, status = m_dwrollAzimut_1Status, length = m_dwrollAzimut_1Length) ] double m_rollAzimut_1 ;
	[ db_column(16, status = m_dwrollAzimut_2Status, length = m_dwrollAzimut_2Length) ] double m_rollAzimut_2 ;
	[ db_column(17, status = m_dwrollAzimut_3Status, length = m_dwrollAzimut_3Length) ] double m_rollAzimut_3 ;
	[ db_column(18, status = m_dwrollGain_0Status, length = m_dwrollGain_0Length) ] double m_rollGain_0 ;
	[ db_column(19, status = m_dwrollGain_1Status, length = m_dwrollGain_1Length) ] double m_rollGain_1 ;
	[ db_column(20, status = m_dwrollGain_2Status, length = m_dwrollGain_2Length) ] double m_rollGain_2 ;
	[ db_column(21, status = m_dwrollGain_3Status, length = m_dwrollGain_3Length) ] double m_rollGain_3 ;
	[ db_column(22, status = m_dwrollOffset_0Status, length = m_dwrollOffset_0Length) ] double m_rollOffset_0 ;
	[ db_column(23, status = m_dwrollOffset_1Status, length = m_dwrollOffset_1Length) ] double m_rollOffset_1 ;
	[ db_column(24, status = m_dwrollOffset_2Status, length = m_dwrollOffset_2Length) ] double m_rollOffset_2 ;
	[ db_column(25, status = m_dwrollOffset_3Status, length = m_dwrollOffset_3Length) ] double m_rollOffset_3 ;
	[ db_column(26, status = m_dwserialNumberStatus, length = m_dwserialNumberLength) ] TCHAR m_serialNumber[4] ;	// Serial number of callibrated sensor.

	DBSTATUS m_dwIDStatus ;
	DBSTATUS m_dwpitchAzimut_0Status ;
	DBSTATUS m_dwpitchAzimut_1Status ;
	DBSTATUS m_dwpitchAzimut_2Status ;
	DBSTATUS m_dwpitchAzimut_3Status ;
	DBSTATUS m_dwpitchGain_0Status ;
	DBSTATUS m_dwpitchGain_1Status ;
	DBSTATUS m_dwpitchGain_2Status ;
	DBSTATUS m_dwpitchGain_3Status ;
	DBSTATUS m_dwpitchOffset_0Status ;
	DBSTATUS m_dwpitchOffset_1Status ;
	DBSTATUS m_dwpitchOffset_2Status ;
	DBSTATUS m_dwpitchOffset_3Status ;
	DBSTATUS m_dwrollAzimut_0Status ;
	DBSTATUS m_dwrollAzimut_1Status ;
	DBSTATUS m_dwrollAzimut_2Status ;
	DBSTATUS m_dwrollAzimut_3Status ;
	DBSTATUS m_dwrollGain_0Status ;
	DBSTATUS m_dwrollGain_1Status ;
	DBSTATUS m_dwrollGain_2Status ;
	DBSTATUS m_dwrollGain_3Status ;
	DBSTATUS m_dwrollOffset_0Status ;
	DBSTATUS m_dwrollOffset_1Status ;
	DBSTATUS m_dwrollOffset_2Status ;
	DBSTATUS m_dwrollOffset_3Status ;
	DBSTATUS m_dwserialNumberStatus ;

	DBLENGTH m_dwIDLength ;
	DBLENGTH m_dwpitchAzimut_0Length ;
	DBLENGTH m_dwpitchAzimut_1Length ;
	DBLENGTH m_dwpitchAzimut_2Length ;
	DBLENGTH m_dwpitchAzimut_3Length ;
	DBLENGTH m_dwpitchGain_0Length ;
	DBLENGTH m_dwpitchGain_1Length ;
	DBLENGTH m_dwpitchGain_2Length ;
	DBLENGTH m_dwpitchGain_3Length ;
	DBLENGTH m_dwpitchOffset_0Length ;
	DBLENGTH m_dwpitchOffset_1Length ;
	DBLENGTH m_dwpitchOffset_2Length ;
	DBLENGTH m_dwpitchOffset_3Length ;
	DBLENGTH m_dwrollAzimut_0Length ;
	DBLENGTH m_dwrollAzimut_1Length ;
	DBLENGTH m_dwrollAzimut_2Length ;
	DBLENGTH m_dwrollAzimut_3Length ;
	DBLENGTH m_dwrollGain_0Length ;
	DBLENGTH m_dwrollGain_1Length ;
	DBLENGTH m_dwrollGain_2Length ;
	DBLENGTH m_dwrollGain_3Length ;
	DBLENGTH m_dwrollOffset_0Length ;
	DBLENGTH m_dwrollOffset_1Length ;
	DBLENGTH m_dwrollOffset_2Length ;
	DBLENGTH m_dwrollOffset_3Length ;
	DBLENGTH m_dwserialNumberLength ;

	void GetRowsetProperties( CDBPropSet *pPropSet )
	{
		pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
	}
} ;

#pragma once
[
  db_command { SELECT ID, \
		pitchAzimut_0, pitchAzimut_1, pitchAzimut_2, pitchAzimut_3, \
		pitchGain_0, pitchGain_1, pitchGain_2, pitchGain_3, \
		pitchOffset_0, pitchOffset_1, pitchOffset_2, pitchOffset_3, \
		rollAzimut_0, rollAzimut_1, rollAzimut_2, rollAzimut_3, \
		rollGain_0, rollGain_1, rollGain_2, rollGain_3, \
		rollOffset_0, rollOffset_1, rollOffset_2, rollOffset_3, \
		serialNumber \
    FROM SensorCorrection WHERE serialNumber = ? }
]
class SensorCorrectionFromSerialNumber
{
public:
	[ db_column(1, status = m_dwIDStatus, length = m_dwIDLength) ] LONG m_ID ;
	[ db_column(2, status = m_dwpitchAzimut_0Status, length = m_dwpitchAzimut_0Length) ] double m_pitchAzimut_0 ;
	[ db_column(3, status = m_dwpitchAzimut_1Status, length = m_dwpitchAzimut_1Length) ] double m_pitchAzimut_1 ;
	[ db_column(4, status = m_dwpitchAzimut_2Status, length = m_dwpitchAzimut_2Length) ] double m_pitchAzimut_2 ;
	[ db_column(5, status = m_dwpitchAzimut_3Status, length = m_dwpitchAzimut_3Length) ] double m_pitchAzimut_3 ;
	[ db_column(6, status = m_dwpitchGain_0Status, length = m_dwpitchGain_0Length) ] double m_pitchGain_0 ;
	[ db_column(7, status = m_dwpitchGain_1Status, length = m_dwpitchGain_1Length) ] double m_pitchGain_1 ;
	[ db_column(8, status = m_dwpitchGain_2Status, length = m_dwpitchGain_2Length) ] double m_pitchGain_2 ;
	[ db_column(9, status = m_dwpitchGain_3Status, length = m_dwpitchGain_3Length) ] double m_pitchGain_3 ;
	[ db_column(10, status = m_dwpitchOffset_0Status, length = m_dwpitchOffset_0Length) ] double m_pitchOffset_0 ;
	[ db_column(11, status = m_dwpitchOffset_1Status, length = m_dwpitchOffset_1Length) ] double m_pitchOffset_1 ;
	[ db_column(12, status = m_dwpitchOffset_2Status, length = m_dwpitchOffset_2Length) ] double m_pitchOffset_2 ;
	[ db_column(13, status = m_dwpitchOffset_3Status, length = m_dwpitchOffset_3Length) ] double m_pitchOffset_3 ;
	[ db_column(14, status = m_dwrollAzimut_0Status, length = m_dwrollAzimut_0Length) ] double m_rollAzimut_0 ;
	[ db_column(15, status = m_dwrollAzimut_1Status, length = m_dwrollAzimut_1Length) ] double m_rollAzimut_1 ;
	[ db_column(16, status = m_dwrollAzimut_2Status, length = m_dwrollAzimut_2Length) ] double m_rollAzimut_2 ;
	[ db_column(17, status = m_dwrollAzimut_3Status, length = m_dwrollAzimut_3Length) ] double m_rollAzimut_3 ;
	[ db_column(18, status = m_dwrollGain_0Status, length = m_dwrollGain_0Length) ] double m_rollGain_0 ;
	[ db_column(19, status = m_dwrollGain_1Status, length = m_dwrollGain_1Length) ] double m_rollGain_1 ;
	[ db_column(20, status = m_dwrollGain_2Status, length = m_dwrollGain_2Length) ] double m_rollGain_2 ;
	[ db_column(21, status = m_dwrollGain_3Status, length = m_dwrollGain_3Length) ] double m_rollGain_3 ;
	[ db_column(22, status = m_dwrollOffset_0Status, length = m_dwrollOffset_0Length) ] double m_rollOffset_0 ;
	[ db_column(23, status = m_dwrollOffset_1Status, length = m_dwrollOffset_1Length) ] double m_rollOffset_1 ;
	[ db_column(24, status = m_dwrollOffset_2Status, length = m_dwrollOffset_2Length) ] double m_rollOffset_2 ;
	[ db_column(25, status = m_dwrollOffset_3Status, length = m_dwrollOffset_3Length) ] double m_rollOffset_3 ;
	[ db_column(26, status = m_dwserialNumberStatus, length = m_dwserialNumberLength) ] TCHAR m_serialNumber[4] ;	// Serial number of callibrated sensor.
	[ db_param(1, DBPARAMIO_INPUT, status = m_dwinserialNumberStatus, length = m_dwinserialNumberLength) ] TCHAR m_inserialNumber [4] ;

	DBSTATUS m_dwIDStatus ;
	DBSTATUS m_dwpitchAzimut_0Status ;
	DBSTATUS m_dwpitchAzimut_1Status ;
	DBSTATUS m_dwpitchAzimut_2Status ;
	DBSTATUS m_dwpitchAzimut_3Status ;
	DBSTATUS m_dwpitchGain_0Status ;
	DBSTATUS m_dwpitchGain_1Status ;
	DBSTATUS m_dwpitchGain_2Status ;
	DBSTATUS m_dwpitchGain_3Status ;
	DBSTATUS m_dwpitchOffset_0Status ;
	DBSTATUS m_dwpitchOffset_1Status ;
	DBSTATUS m_dwpitchOffset_2Status ;
	DBSTATUS m_dwpitchOffset_3Status ;
	DBSTATUS m_dwrollAzimut_0Status ;
	DBSTATUS m_dwrollAzimut_1Status ;
	DBSTATUS m_dwrollAzimut_2Status ;
	DBSTATUS m_dwrollAzimut_3Status ;
	DBSTATUS m_dwrollGain_0Status ;
	DBSTATUS m_dwrollGain_1Status ;
	DBSTATUS m_dwrollGain_2Status ;
	DBSTATUS m_dwrollGain_3Status ;
	DBSTATUS m_dwrollOffset_0Status ;
	DBSTATUS m_dwrollOffset_1Status ;
	DBSTATUS m_dwrollOffset_2Status ;
	DBSTATUS m_dwrollOffset_3Status ;
	DBSTATUS m_dwserialNumberStatus ;
	DBSTATUS m_dwinserialNumberStatus ;

	DBLENGTH m_dwIDLength ;
	DBLENGTH m_dwpitchAzimut_0Length ;
	DBLENGTH m_dwpitchAzimut_1Length ;
	DBLENGTH m_dwpitchAzimut_2Length ;
	DBLENGTH m_dwpitchAzimut_3Length ;
	DBLENGTH m_dwpitchGain_0Length ;
	DBLENGTH m_dwpitchGain_1Length ;
	DBLENGTH m_dwpitchGain_2Length ;
	DBLENGTH m_dwpitchGain_3Length ;
	DBLENGTH m_dwpitchOffset_0Length ;
	DBLENGTH m_dwpitchOffset_1Length ;
	DBLENGTH m_dwpitchOffset_2Length ;
	DBLENGTH m_dwpitchOffset_3Length ;
	DBLENGTH m_dwrollAzimut_0Length ;
	DBLENGTH m_dwrollAzimut_1Length ;
	DBLENGTH m_dwrollAzimut_2Length ;
	DBLENGTH m_dwrollAzimut_3Length ;
	DBLENGTH m_dwrollGain_0Length ;
	DBLENGTH m_dwrollGain_1Length ;
	DBLENGTH m_dwrollGain_2Length ;
	DBLENGTH m_dwrollGain_3Length ;
	DBLENGTH m_dwrollOffset_0Length ;
	DBLENGTH m_dwrollOffset_1Length ;
	DBLENGTH m_dwrollOffset_2Length ;
	DBLENGTH m_dwrollOffset_3Length ;
	DBLENGTH m_dwserialNumberLength ;
	DBLENGTH m_dwinserialNumberLength ;

	void GetRowsetProperties( CDBPropSet *pPropSet )
	{
		pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
		pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
	}
} ;
