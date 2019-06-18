// $Id: SensorAzimutCallibrationData.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

[
	db_command(L"SELECT dauSerialNumber, Id, operator, pitch, pitchTime, pitchTemperature, roll, rollTemperature, rollTime, serialNumber FROM SensorAzimutCallibrationData")
]
class SensorAzimutCallibrationData
{
public:
	[ db_column(1, status=m_dwdauSerialNumberStatus, length=m_dwdauSerialNumberLength) ] TCHAR m_dauSerialNumber[4];	// Serial number of the Data Accusition Unit (DAU) used for callibration.
	[ db_column(2, status=m_dwIdStatus, length=m_dwIdLength) ] LONG m_Id;	// Primary index
	[ db_column(3, status=m_dwoperatorStatus, length=m_dwoperatorLength) ] TCHAR m_operator[51];	// Name of operator performing callibration.
	[ db_column(4, status=m_dwpitchStatus, length=m_dwpitchLength) ] double m_pitch;	// Pitch azimut value.
	[ db_column(5, status=m_dwpitchTimeStatus, length=m_dwpitchTimeLength) ] DATE m_pitchTime;	// Time of pitch callibration measurment start.
	[ db_column(6, status=m_dwpitchTemperatureStatus, length=m_dwpitchTemperatureLength) ] double m_pitchTemperature;	// Temperatur of sensor pitch callibration.
	[ db_column(7, status=m_dwrollStatus, length=m_dwrollLength) ] double m_roll;	// Roll azimut value.
	[ db_column(8, status=m_dwrollTemperatureStatus, length=m_dwrollTemperatureLength) ]  double m_rollTemperature;	// Temperatur of sensor at roll callibration.
	[ db_column(9, status=m_dwrollTimeStatus, length=m_dwrollTimeLength) ] DATE m_rollTime;	// Time of roll callibration measurment start.
	[ db_column(10, status=m_dwserialNumberStatus, length=m_dwserialNumberLength) ] TCHAR m_serialNumber[4];	// Serial number of the sensor unit callibrated.

	DBSTATUS m_dwdauSerialNumberStatus;
	DBSTATUS m_dwIdStatus;
	DBSTATUS m_dwoperatorStatus;
	DBSTATUS m_dwpitchStatus;
	DBSTATUS m_dwpitchTimeStatus;
	DBSTATUS m_dwpitchTemperatureStatus;
	DBSTATUS m_dwrollStatus;
	DBSTATUS m_dwrollTemperatureStatus;
	DBSTATUS m_dwrollTimeStatus;
	DBSTATUS m_dwserialNumberStatus;

	DBLENGTH m_dwdauSerialNumberLength;
	DBLENGTH m_dwIdLength;
	DBLENGTH m_dwoperatorLength;
	DBLENGTH m_dwpitchLength;
	DBLENGTH m_dwpitchTimeLength;
	DBLENGTH m_dwpitchTemperatureLength;
	DBLENGTH m_dwrollLength;
	DBLENGTH m_dwrollTemperatureLength;
	DBLENGTH m_dwrollTimeLength;
	DBLENGTH m_dwserialNumberLength;

	void GetRowsetProperties(CDBPropSet* pPropSet)
	{
		pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
	}
} ;

[
  db_command { SELECT dauSerialNumber, Id, operator, pitch, pitchTime, pitchTemperature, roll, rollTemperature, rollTime, serialNumber FROM SensorAzimutCallibrationData WHERE serialNumber = ? }
]
class SensorAzimutCallibrationDataFromSerialNumber
{
public:
	[ db_column(1, status=m_dwdauSerialNumberStatus, length=m_dwdauSerialNumberLength) ] TCHAR m_dauSerialNumber[4];	// Serial number of the Data Accusition Unit (DAU) used for callibration.
	[ db_column(2, status=m_dwIdStatus, length=m_dwIdLength) ] LONG m_Id;	// Primary index
	[ db_column(3, status=m_dwoperatorStatus, length=m_dwoperatorLength) ] TCHAR m_operator[51];	// Name of operator performing callibration.
	[ db_column(4, status=m_dwpitchStatus, length=m_dwpitchLength) ] double m_pitch;	// Pitch azimut value.
	[ db_column(5, status=m_dwpitchTimeStatus, length=m_dwpitchTimeLength) ] DATE m_pitchTime;	// Time of pitch callibration measurment start.
	[ db_column(6, status=m_dwpitchTemperatureStatus, length=m_dwpitchTemperatureLength) ] double m_pitchTemperature;	// Temperatur of sensor pitch callibration.
	[ db_column(7, status=m_dwrollStatus, length=m_dwrollLength) ] double m_roll;	// Roll azimut value.
	[ db_column(8, status=m_dwrollTemperatureStatus, length=m_dwrollTemperatureLength) ]  double m_rollTemperature;	// Temperatur of sensor at roll callibration.
	[ db_column(9, status=m_dwrollTimeStatus, length=m_dwrollTimeLength) ] DATE m_rollTime;	// Time of roll callibration measurment start.
	[ db_column(10, status=m_dwserialNumberStatus, length=m_dwserialNumberLength) ] TCHAR m_serialNumber[4];	// Serial number of the sensor unit callibrated.
	[ db_param(1, DBPARAMIO_INPUT, status = m_dwinserialNumberStatus, length = m_dwinserialNumberLength) ] TCHAR m_inserialNumber [4] ;

	DBSTATUS m_dwdauSerialNumberStatus;
	DBSTATUS m_dwIdStatus;
	DBSTATUS m_dwoperatorStatus;
	DBSTATUS m_dwpitchStatus;
	DBSTATUS m_dwpitchTimeStatus;
	DBSTATUS m_dwpitchTemperatureStatus;
	DBSTATUS m_dwrollStatus;
	DBSTATUS m_dwrollTemperatureStatus;
	DBSTATUS m_dwrollTimeStatus;
	DBSTATUS m_dwserialNumberStatus;

	DBLENGTH m_dwdauSerialNumberLength;
	DBLENGTH m_dwIdLength;
	DBLENGTH m_dwoperatorLength;
	DBLENGTH m_dwpitchLength;
	DBLENGTH m_dwpitchTimeLength;
	DBLENGTH m_dwpitchTemperatureLength;
	DBLENGTH m_dwrollLength;
	DBLENGTH m_dwrollTemperatureLength;
	DBLENGTH m_dwrollTimeLength;
	DBLENGTH m_dwserialNumberLength;

	DBSTATUS m_dwinserialNumberStatus ;
	DBLENGTH m_dwinserialNumberLength ;

  void GetRowsetProperties(CDBPropSet* pPropSet)
	{
		pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
	}
};


