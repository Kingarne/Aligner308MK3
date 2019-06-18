// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\sensordatabase.mrg.h
// compiler-generated file created 05/10/06 at 15:51:56
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

// $Id: sensordatabase.mrg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "SystemSetup.h"
#include "Database.h"

#define UNIQUE_SN_BODY \
public: \
  DECL_DB_L(1, TCHAR, serialNumber, 4) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
  }

#define INSERT_INTO_SENSOR_DATA_BODY \
  public: \
	PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
	PARM_DB_L(2, TCHAR, dauSerialNumber, 4) ; \
	PARM_DB_L(3, TCHAR, operator, 51) ; \
	PARM_DB_V(4, double, temperature) ; \
	PARM_DB_V(5, double, value) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

#define UNIQUE_SN(name) \
  [ db_command { SELECT DISTINCT serialNumber FROM name ORDER BY serialNumber } ] class UniqueSNFrom##name

#define INSERT_INTO_SENSOR_DATA(name) \
  [ db_command { INSERT INTO name ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) } ] class InsertInto##name

#define DELETE_SENSOR_CALIBRATION(name) \
  [ db_command { DELETE FROM name WHERE serialNumber = ? } ] class Delete##name

#define SENSOR_CALIBRATION_FROM_SN(name) \
  [ db_command { SELECT operator, time, a_0, a_1, a_2, a_3 FROM name WHERE serialNumber = ? } ] class name##FromSN

#define SENSOR_CALIBRATION_FROM_SN_BODY \
  public: \
	DECL_DB_L(1, TCHAR, operator, 51) ; \
	DECL_DB_V(2, DBTIMESTAMP, time) ; \
	DECL_DB_V(3, double, a_0) ; \
	DECL_DB_V(4, double, a_1) ; \
	DECL_DB_V(5, double, a_2) ; \
	DECL_DB_V(6, double, a_3) ; \
  PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

SENSOR_CALIBRATION_FROM_SN(SensorPitchOffsetCalibration)   :---*/ _SensorPitchOffsetCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 63 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorPitchOffsetCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchOffsetCalibration WHERE serialNumber = ? ")
			
#injected_line 63 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorPitchOffsetCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 63 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorPitchOffsetCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 63 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchOffsetCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 63 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchOffsetCalibrationFromSN : public CCommand<CAccessor<_SensorPitchOffsetCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchOffsetCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_CALIBRATION_FROM_SN(SensorPitchGainCalibration)     :---*/ _SensorPitchGainCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 64 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorPitchGainCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchGainCalibration WHERE serialNumber = ? ")
			
#injected_line 64 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorPitchGainCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 64 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorPitchGainCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 64 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchGainCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 64 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchGainCalibrationFromSN : public CCommand<CAccessor<_SensorPitchGainCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchGainCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_CALIBRATION_FROM_SN(SensorPitchAzimuthCalibration)  :---*/ _SensorPitchAzimuthCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 65 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorPitchAzimuthCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchAzimuthCalibration WHERE serialNumber = ? ")
			
#injected_line 65 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorPitchAzimuthCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 65 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorPitchAzimuthCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 65 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchAzimuthCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 65 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorPitchAzimuthCalibrationFromSN : public CCommand<CAccessor<_SensorPitchAzimuthCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorPitchAzimuthCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_CALIBRATION_FROM_SN(SensorRollOffsetCalibration)    :---*/ _SensorRollOffsetCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 66 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorRollOffsetCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollOffsetCalibration WHERE serialNumber = ? ")
			
#injected_line 66 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorRollOffsetCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 66 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorRollOffsetCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 66 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollOffsetCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 66 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollOffsetCalibrationFromSN : public CCommand<CAccessor<_SensorRollOffsetCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollOffsetCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_CALIBRATION_FROM_SN(SensorRollGainCalibration)      :---*/ _SensorRollGainCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 67 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorRollGainCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollGainCalibration WHERE serialNumber = ? ")
			
#injected_line 67 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorRollGainCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 67 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorRollGainCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 67 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollGainCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 67 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollGainCalibrationFromSN : public CCommand<CAccessor<_SensorRollGainCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollGainCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_CALIBRATION_FROM_SN(SensorRollAzimuthCalibration)   :---*/ _SensorRollAzimuthCalibrationFromSNAccessor { SENSOR_CALIBRATION_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_SensorRollAzimuthCalibrationFromSNAccessor, L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollAzimuthCalibration WHERE serialNumber = ? ")
			
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_SensorRollAzimuthCalibrationFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

					BEGIN_PARAM_MAP(_SensorRollAzimuthCalibrationFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollAzimuthCalibrationFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class SensorRollAzimuthCalibrationFromSN : public CCommand<CAccessor<_SensorRollAzimuthCalibrationFromSNAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT operator, time, a_0, a_1, a_2, a_3 FROM SensorRollAzimuthCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


UNIQUE_SN(SensorPitchOffsetCalibration)   :---*/ _UniqueSNFromSensorPitchOffsetCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 70 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchOffsetCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchOffsetCalibration ORDER BY serialNumber ")
			
#injected_line 70 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchOffsetCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 70 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 70 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchOffsetCalibration : public CCommand<CAccessor<_UniqueSNFromSensorPitchOffsetCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchOffsetCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorPitchGainCalibration)     :---*/ _UniqueSNFromSensorPitchGainCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 71 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchGainCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchGainCalibration ORDER BY serialNumber ")
			
#injected_line 71 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchGainCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 71 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 71 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchGainCalibration : public CCommand<CAccessor<_UniqueSNFromSensorPitchGainCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchGainCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorPitchAzimuthCalibration)  :---*/ _UniqueSNFromSensorPitchAzimuthCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 72 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchAzimuthCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchAzimuthCalibration ORDER BY serialNumber ")
			
#injected_line 72 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchAzimuthCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 72 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 72 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorPitchAzimuthCalibration : public CCommand<CAccessor<_UniqueSNFromSensorPitchAzimuthCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchAzimuthCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollOffsetCalibration)    :---*/ _UniqueSNFromSensorRollOffsetCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 73 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollOffsetCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollOffsetCalibration ORDER BY serialNumber ")
			
#injected_line 73 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollOffsetCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 73 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 73 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollOffsetCalibration : public CCommand<CAccessor<_UniqueSNFromSensorRollOffsetCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollOffsetCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollGainCalibration)      :---*/ _UniqueSNFromSensorRollGainCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 74 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollGainCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollGainCalibration ORDER BY serialNumber ")
			
#injected_line 74 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollGainCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 74 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 74 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollGainCalibration : public CCommand<CAccessor<_UniqueSNFromSensorRollGainCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollGainCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollAzimuthCalibration)   :---*/ _UniqueSNFromSensorRollAzimuthCalibrationAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 75 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollAzimuthCalibrationAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollAzimuthCalibration ORDER BY serialNumber ")
			
#injected_line 75 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollAzimuthCalibrationAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 75 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 75 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromSensorRollAzimuthCalibration : public CCommand<CAccessor<_UniqueSNFromSensorRollAzimuthCalibrationAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollAzimuthCalibration ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


UNIQUE_SN(DAUCorrection)                  :---*/ _UniqueSNFromDAUCorrectionAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 77 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

				DEFINE_COMMAND_EX(_UniqueSNFromDAUCorrectionAccessor, L" SELECT DISTINCT serialNumber FROM DAUCorrection ORDER BY serialNumber ")
			
#injected_line 77 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromDAUCorrectionAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 77 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromDAUCorrection;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 77 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\SensorDatabase.h"

class UniqueSNFromDAUCorrection : public CCommand<CAccessor<_UniqueSNFromDAUCorrectionAccessor> >
    			
{
public:
    HRESULT OpenRowset(const CSession& session, LPCWSTR szCommand=NULL)
            {
        DBPROPSET *pPropSet = NULL;
        CDBPropSet propset(DBPROPSET_ROWSET);
        __if_exists(HasBookmark)
            	
        {
            if( HasBookmark() )
		{
                propset.AddProperty(DBPROP_IRowsetLocate, true);
                pPropSet= &propset;
            }
        }	
        __if_exists(GetRowsetProperties)
            	
        {
            GetRowsetProperties(&propset);
            pPropSet= &propset;
        }
        if (szCommand == NULL)
            szCommand = L" SELECT DISTINCT serialNumber FROM DAUCorrection ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


template <class T>
void InsertData( T &table, CString serialNumber, double temperature, double value )
{
  SET_DB_STRING(table, serialNumber, serialNumber) ;
  SET_DB_STRING(table, dauSerialNumber, (LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
  SET_DB_STRING(table, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
//  SET_DB_VALUE(table, time, timestamp) ; // TODO: this should mark the time of the measurement.
  SET_DB_VALUE(table, temperature, temperature) ;
  SET_DB_VALUE(table, value, value) ;

  HRESULT hResult = ::OpenTable( table ) ;
  if (S_OK != hResult)
  {
		Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
		ASSERT(0) ;
  }
}
