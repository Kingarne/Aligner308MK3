// $Id: SensorDatabase.h,v 1.4 2011/10/13 10:09:12 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "SystemSetup.h"
#include "Database.h"

// #define INSERT_INTO_SENSOR_DATA_BODY \
//   public: \
// 	PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
// 	PARM_DB_L(2, TCHAR, dauSerialNumber, 4) ; \
// 	PARM_DB_L(3, TCHAR, operator, 51) ; \
// 	PARM_DB_V(4, double, temperature) ; \
// 	PARM_DB_V(5, double, value) ; \
//   void GetRowsetProperties( CDBPropSet *pPropSet ) \
//   { \
//     pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
//     pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
//     pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
//     pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
//   }

// #define INSERT_INTO_SENSOR_DATA(name) \
//   [ db_command { INSERT INTO name ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES (?,?,?,?,?) } ] class InsertInto##name


// template <class T>
// void InsertData( T &table, CString serialNumber, double temperature, double value )
// {
//   SET_DB_STRING(table, serialNumber, serialNumber) ;
//   SET_DB_STRING(table, dauSerialNumber, (LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
//   SET_DB_STRING(table, operator, (LPCTSTR)SysSetup->GetOperatorName()) ;
// //  SET_DB_VALUE(table, time, timestamp) ; // TODO: this should mark the time of the measurement.
//   SET_DB_VALUE(table, temperature, temperature) ;
//   SET_DB_VALUE(table, value, value) ;
// 
//   HRESULT hResult = ::OpenTable( table ) ;
//   if (S_OK != hResult)
//   {
// 		Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
// 		ASSERT(0) ;
//   }
// }
