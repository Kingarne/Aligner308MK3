// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\history.mrg.h
// compiler-generated file created 05/10/06 at 15:51:57
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

// $Id: history.mrg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "SensorDatabase.h"

#define DECLARE_HISTORY \
public: \
  static BOOL AddData( const Data &data ) ; \
  static BOOL SetComment( const CString &comment ) ; \
  static BOOL AddItem( const ItemData item ) ; \
	static BOOL AddItemErr( const ItemErrData item ) ; \
	static BOOL AddExtItem( const ExtItemData extItem ) ; \
  static BOOL AddGraph( CString filename, BOOL include = TRUE ) ; \
	static BOOL DeleteLast( void ) ; \
  static void ResetMainID( void ) ; \
public: \
	static LONG GetLastID( void ) ; \
	static LONG GetMainID( void ) ; \
private: \
  static LONG m_lastID ; \
  static LONG m_mainID ;

[
    db_command { DELETE FROM HistoryPrint }
]
class /*+++ symbol was renamed:  ClearHistoryPrintTable
 :---*/ _ClearHistoryPrintTableAccessor {
public:
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 27 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_ClearHistoryPrintTableAccessor, L" DELETE FROM HistoryPrint ")
			
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 27 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class ClearHistoryPrintTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 27 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class ClearHistoryPrintTable : public CCommand<CAccessor<_ClearHistoryPrintTableAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM HistoryPrint ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
    db_command { DELETE FROM HistoryPrint WHERE historyID = ? }
]
class /*+++ symbol was renamed:  DeleteHistoryPrintTableItem
 :---*/ _DeleteHistoryPrintTableItemAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 40 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_DeleteHistoryPrintTableItemAccessor, L" DELETE FROM HistoryPrint WHERE historyID = ? ")
			
#injected_line 40 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_DeleteHistoryPrintTableItemAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 40 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class DeleteHistoryPrintTableItem;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 40 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class DeleteHistoryPrintTableItem : public CCommand<CAccessor<_DeleteHistoryPrintTableItemAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM HistoryPrint WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO HistoryPrint ( historyID ) VALUES ( ? ) }
]
class /*+++ symbol was renamed:  AddToHistoryPrint
 :---*/ _AddToHistoryPrintAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 54 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AddToHistoryPrintAccessor, L" INSERT INTO HistoryPrint ( historyID ) VALUES ( ? ) ")
			
#injected_line 54 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AddToHistoryPrintAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 54 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AddToHistoryPrint;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 54 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AddToHistoryPrint : public CCommand<CAccessor<_AddToHistoryPrintAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO HistoryPrint ( historyID ) VALUES ( ? ) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, latitude, angulaDefinitionPart1, angulaDefinitionPart2, project ) VALUES (?,?,?,?,?,?,?,?) }
]
class /*+++ symbol was renamed:  HistoryTable
 :---*/ _HistoryTableAccessor {
public:
  PARM_DB_L(1, TCHAR, operator, 32) ;
  PARM_DB_L(2, TCHAR, dauSerialNumber, 4) ;
	PARM_DB_V(3, DBTIMESTAMP, time) ;
  PARM_DB_L(4, TCHAR, place, 32) ;
	PARM_DB_V(5, double, latitude) ;
  PARM_DB_L(6, TCHAR, angularDefinitionPart1, 100) ;
  PARM_DB_L(7, TCHAR, angularDefinitionPart2, 200) ;
  PARM_DB_L(8, TCHAR, project, 50) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HistoryTableAccessor, L" INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, latitude, angulaDefinitionPart1, angulaDefinitionPart2, project ) VALUES (?,?,?,?,?,?,?,?) ")
			
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_place),  _SIZE_TYPE(m_place), 0, 0, offsetbuf(m_place), offsetbuf(m_length_place), offsetbuf(m_status_place))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_latitude),  _SIZE_TYPE(m_latitude), 0, 0, offsetbuf(m_latitude), 0, offsetbuf(m_status_latitude))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_angularDefinitionPart1),  _SIZE_TYPE(m_angularDefinitionPart1), 0, 0, offsetbuf(m_angularDefinitionPart1), offsetbuf(m_length_angularDefinitionPart1), offsetbuf(m_status_angularDefinitionPart1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_angularDefinitionPart2),  _SIZE_TYPE(m_angularDefinitionPart2), 0, 0, offsetbuf(m_angularDefinitionPart2), offsetbuf(m_length_angularDefinitionPart2), offsetbuf(m_status_angularDefinitionPart2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_project),  _SIZE_TYPE(m_project), 0, 0, offsetbuf(m_project), offsetbuf(m_length_project), offsetbuf(m_status_project))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 68 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HistoryTable : public CCommand<CAccessor<_HistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, latitude, angulaDefinitionPart1, angulaDefinitionPart2, project ) VALUES (?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, project ) VALUES (?,?,?,?,?) }
]
class /*+++ symbol was renamed:  HistoryTable2
 :---*/ _HistoryTable2Accessor {
public:
  PARM_DB_L(1, TCHAR, operator, 32) ;
  PARM_DB_L(2, TCHAR, dauSerialNumber, 4) ;
	PARM_DB_V(3, DBTIMESTAMP, time) ;
  PARM_DB_L(4, TCHAR, place, 32) ;
  PARM_DB_L(5, TCHAR, project, 32) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 92 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HistoryTable2Accessor, L" INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, project ) VALUES (?,?,?,?,?) ")
			
#injected_line 92 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HistoryTable2Accessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_place),  _SIZE_TYPE(m_place), 0, 0, offsetbuf(m_place), offsetbuf(m_length_place), offsetbuf(m_status_place))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_project),  _SIZE_TYPE(m_project), 0, 0, offsetbuf(m_project), offsetbuf(m_length_project), offsetbuf(m_status_project))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 92 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HistoryTable2;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 92 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HistoryTable2 : public CCommand<CAccessor<_HistoryTable2Accessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, project ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { SELECT @@IDENTITY }
]
class /*+++ symbol was renamed:  LastCounter
 :---*/ _LastCounterAccessor {
public:
  DECL_DB_V(1,LONG,id) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    // Empty
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 113 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_LastCounterAccessor, L" SELECT @@IDENTITY ")
			
#injected_line 113 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

						BEGIN_ACCESSOR_MAP(_LastCounterAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_id),  _SIZE_TYPE(m_id), 0, 0, offsetbuf(m_id), 0, offsetbuf(m_status_id))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 113 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LastCounter;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 113 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LastCounter : public CCommand<CAccessor<_LastCounterAccessor> >
    			
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
            szCommand = L" SELECT @@IDENTITY ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO GraphHistory (historyID, filename, include ) VALUES (?,?,?) }
]
class /*+++ symbol was renamed:  GraphHistoryTable
 :---*/ _GraphHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, filename, 255) ;
	PARM_DB_V(3, BOOL, include) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 127 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_GraphHistoryTableAccessor, L" INSERT INTO GraphHistory (historyID, filename, include ) VALUES (?,?,?) ")
			
#injected_line 127 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_GraphHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_filename),  _SIZE_TYPE(m_filename), 0, 0, offsetbuf(m_filename), offsetbuf(m_length_filename), offsetbuf(m_status_filename))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_include),  _SIZE_TYPE(m_include), 0, 0, offsetbuf(m_include), 0, offsetbuf(m_status_include))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 127 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GraphHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 127 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GraphHistoryTable : public CCommand<CAccessor<_GraphHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO GraphHistory (historyID, filename, include ) VALUES (?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO TiltAlignmentErrorsHistory (historyID, ship, timeConstant, lineOfSightDirection, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) }
]
class /*+++ symbol was renamed:  TiltAlignmentErrorsHistoryTable
 :---*/ _TiltAlignmentErrorsHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_L(4, TCHAR, lineOfSightDirection, 32) ;
  PARM_DB_L(5, TCHAR, elevationCompensation, 3) ;
  PARM_DB_L(6, TCHAR, comment, 255) ;
	PARM_DB_L(7, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 146 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAlignmentErrorsHistoryTableAccessor, L" INSERT INTO TiltAlignmentErrorsHistory (historyID, ship, timeConstant, lineOfSightDirection, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ")
			
#injected_line 146 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAlignmentErrorsHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_lineOfSightDirection),  _SIZE_TYPE(m_lineOfSightDirection), 0, 0, offsetbuf(m_lineOfSightDirection), offsetbuf(m_length_lineOfSightDirection), offsetbuf(m_status_lineOfSightDirection))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_elevationCompensation),  _SIZE_TYPE(m_elevationCompensation), 0, 0, offsetbuf(m_elevationCompensation), offsetbuf(m_length_elevationCompensation), offsetbuf(m_status_elevationCompensation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 146 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAlignmentErrorsHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 146 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAlignmentErrorsHistoryTable : public CCommand<CAccessor<_TiltAlignmentErrorsHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAlignmentErrorsHistory (historyID, ship, timeConstant, lineOfSightDirection, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO TiltAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (?,?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAlignmentErrorsHistoryItemTable
 :---*/ _TiltAlignmentErrorsHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
	PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;
  PARM_DB_V(10, double, elevation) ;
  PARM_DB_V(11, double, bias) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 169 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAlignmentErrorsHistoryItemTableAccessor, L" INSERT INTO TiltAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (?,?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 169 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAlignmentErrorsHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(11, _OLEDB_TYPE(m_bias),  _SIZE_TYPE(m_bias), 0, 0, offsetbuf(m_bias), 0, offsetbuf(m_status_bias))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 169 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAlignmentErrorsHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 169 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAlignmentErrorsHistoryItemTable : public CCommand<CAccessor<_TiltAlignmentErrorsHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (?,?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class TiltAlignmentErrorsHistory
{
private:
  TiltAlignmentErrorsHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    CString m_lineOfSightDirection ;
    CString m_elevationCompensation ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
		CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_bias ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO TiltAndFlatnessHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessHistoryTable
 :---*/ _TiltAndFlatnessHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_V(4, double, numberOfMeasurement) ;
  PARM_DB_L(5, TCHAR, reference, 255) ;
  PARM_DB_L(6, TCHAR, comment, 255) ;
	PARM_DB_L(7, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 238 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessHistoryTableAccessor, L" INSERT INTO TiltAndFlatnessHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ")
			
#injected_line 238 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_numberOfMeasurement),  _SIZE_TYPE(m_numberOfMeasurement), 0, 0, offsetbuf(m_numberOfMeasurement), 0, offsetbuf(m_status_numberOfMeasurement))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_reference),  _SIZE_TYPE(m_reference), 0, 0, offsetbuf(m_reference), offsetbuf(m_length_reference), offsetbuf(m_status_reference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 238 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 238 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryTable : public CCommand<CAccessor<_TiltAndFlatnessHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO TiltAndFlatnessHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessHistoryItemTable
 :---*/ _TiltAndFlatnessHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;
  PARM_DB_V(10, double, elevation) ;
  PARM_DB_V(11, double, standardDeviation) ;
  PARM_DB_V(12, double, maximumDeviation) ;
  PARM_DB_V(13, double, azimuth) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 262 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessHistoryItemTableAccessor, L" INSERT INTO TiltAndFlatnessHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 262 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(11, _OLEDB_TYPE(m_standardDeviation),  _SIZE_TYPE(m_standardDeviation), 0, 0, offsetbuf(m_standardDeviation), 0, offsetbuf(m_status_standardDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(12, _OLEDB_TYPE(m_maximumDeviation),  _SIZE_TYPE(m_maximumDeviation), 0, 0, offsetbuf(m_maximumDeviation), 0, offsetbuf(m_status_maximumDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(13, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 262 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 262 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryItemTable : public CCommand<CAccessor<_TiltAndFlatnessHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO TiltAndFlatnessHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessHistoryExtItemTable
 :---*/ _TiltAndFlatnessHistoryExtItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_V(2, double, azimuthAngle) ;
  PARM_DB_V(3, double, deviationJ3) ;
	PARM_DB_V(4, double, deviationJ4) ;
	PARM_DB_V(5, double, deviationJ5) ;
	PARM_DB_V(6, double, deviationJ6) ;
	PARM_DB_V(7, double, deviationJ7) ;
	PARM_DB_V(8, double, deviationJ8) ;
	PARM_DB_V(9, double, deviationJ9) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 292 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessHistoryExtItemTableAccessor, L" INSERT INTO TiltAndFlatnessHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 292 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessHistoryExtItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_azimuthAngle),  _SIZE_TYPE(m_azimuthAngle), 0, 0, offsetbuf(m_azimuthAngle), 0, offsetbuf(m_status_azimuthAngle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_deviationJ3),  _SIZE_TYPE(m_deviationJ3), 0, 0, offsetbuf(m_deviationJ3), 0, offsetbuf(m_status_deviationJ3))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_deviationJ4),  _SIZE_TYPE(m_deviationJ4), 0, 0, offsetbuf(m_deviationJ4), 0, offsetbuf(m_status_deviationJ4))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_deviationJ5),  _SIZE_TYPE(m_deviationJ5), 0, 0, offsetbuf(m_deviationJ5), 0, offsetbuf(m_status_deviationJ5))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_deviationJ6),  _SIZE_TYPE(m_deviationJ6), 0, 0, offsetbuf(m_deviationJ6), 0, offsetbuf(m_status_deviationJ6))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_deviationJ7),  _SIZE_TYPE(m_deviationJ7), 0, 0, offsetbuf(m_deviationJ7), 0, offsetbuf(m_status_deviationJ7))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_deviationJ8),  _SIZE_TYPE(m_deviationJ8), 0, 0, offsetbuf(m_deviationJ8), 0, offsetbuf(m_status_deviationJ8))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_deviationJ9),  _SIZE_TYPE(m_deviationJ9), 0, 0, offsetbuf(m_deviationJ9), 0, offsetbuf(m_status_deviationJ9))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 292 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryExtItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 292 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessHistoryExtItemTable : public CCommand<CAccessor<_TiltAndFlatnessHistoryExtItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class TiltAndFlatnessHistory
{
private:
  TiltAndFlatnessHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    LONG m_numberOfMeasurements ;
    CString m_reference ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
    double m_maximumDeviation ;
    double m_azimuth ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
		double m_azimuthAngle ;
		double m_deviationJ3 ;
		double m_deviationJ4 ;
		double m_deviationJ5 ;
		double m_deviationJ6 ;
		double m_deviationJ7 ;
		double m_deviationJ8 ;
		double m_deviationJ9 ;
  } ;
	DECLARE_HISTORY ;
} ;
[
  db_command { INSERT INTO TiltAndFlatnessFoHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit, IndexArmLength) VALUES (?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessFoHistoryTable
 :---*/ _TiltAndFlatnessFoHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_V(4, double, numberOfMeasurement) ;
  PARM_DB_L(5, TCHAR, reference, 255) ;
  PARM_DB_L(6, TCHAR, comment, 255) ;
	PARM_DB_L(7, TCHAR, measuredUnit, 10) ;
  PARM_DB_V(8, double, IndexArmLength) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 368 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessFoHistoryTableAccessor, L" INSERT INTO TiltAndFlatnessFoHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit, IndexArmLength) VALUES (?,?,?,?,?,?,?,?) ")
			
#injected_line 368 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessFoHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_numberOfMeasurement),  _SIZE_TYPE(m_numberOfMeasurement), 0, 0, offsetbuf(m_numberOfMeasurement), 0, offsetbuf(m_status_numberOfMeasurement))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_reference),  _SIZE_TYPE(m_reference), 0, 0, offsetbuf(m_reference), offsetbuf(m_length_reference), offsetbuf(m_status_reference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_IndexArmLength),  _SIZE_TYPE(m_IndexArmLength), 0, 0, offsetbuf(m_IndexArmLength), 0, offsetbuf(m_status_IndexArmLength))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 368 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 368 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryTable : public CCommand<CAccessor<_TiltAndFlatnessFoHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessFoHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit, IndexArmLength) VALUES (?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  //db_command { INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
	db_command { INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessFoHistoryItemTable
 :---*/ _TiltAndFlatnessFoHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
//  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(5, double, roll) ;
  PARM_DB_V(6, double, pitch) ;
  PARM_DB_V(7, double, tilt) ;
  PARM_DB_V(8, double, angle) ;
  PARM_DB_V(9, double, elevation) ;
  PARM_DB_V(10, double, standardDeviation) ;
  PARM_DB_V(11, double, maximumDeviation) ;
  PARM_DB_V(12, double, azimuth) ;
  PARM_DB_V(13, double, IndexArmLength) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 394 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessFoHistoryItemTableAccessor, L" INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 394 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessFoHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_standardDeviation),  _SIZE_TYPE(m_standardDeviation), 0, 0, offsetbuf(m_standardDeviation), 0, offsetbuf(m_status_standardDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(11, _OLEDB_TYPE(m_maximumDeviation),  _SIZE_TYPE(m_maximumDeviation), 0, 0, offsetbuf(m_maximumDeviation), 0, offsetbuf(m_status_maximumDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(12, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(13, _OLEDB_TYPE(m_IndexArmLength),  _SIZE_TYPE(m_IndexArmLength), 0, 0, offsetbuf(m_IndexArmLength), 0, offsetbuf(m_status_IndexArmLength))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 394 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 394 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryItemTable : public CCommand<CAccessor<_TiltAndFlatnessFoHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[
  //db_command { INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
	db_command { INSERT INTO TiltAndFlatnessFoHistoryItemErr (historyID, azimuth, error) VALUES (?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessFoHistoryItemErrTable
 :---*/ _TiltAndFlatnessFoHistoryItemErrTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_V(2, double, azimuth) ;
  PARM_DB_V(3, double, error) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 425 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessFoHistoryItemErrTableAccessor, L" INSERT INTO TiltAndFlatnessFoHistoryItemErr (historyID, azimuth, error) VALUES (?,?,?) ")
			
#injected_line 425 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessFoHistoryItemErrTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_error),  _SIZE_TYPE(m_error), 0, 0, offsetbuf(m_error), 0, offsetbuf(m_status_error))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 425 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryItemErrTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 425 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryItemErrTable : public CCommand<CAccessor<_TiltAndFlatnessFoHistoryItemErrTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessFoHistoryItemErr (historyID, azimuth, error) VALUES (?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO TiltAndFlatnessFoHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  TiltAndFlatnessFoHistoryExtItemTable
 :---*/ _TiltAndFlatnessFoHistoryExtItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_V(2, double, azimuthAngle) ;
  PARM_DB_V(3, double, deviationJ3) ;
	PARM_DB_V(4, double, deviationJ4) ;
	PARM_DB_V(5, double, deviationJ5) ;
	PARM_DB_V(6, double, deviationJ6) ;
	PARM_DB_V(7, double, deviationJ7) ;
	PARM_DB_V(8, double, deviationJ8) ;
	PARM_DB_V(9, double, deviationJ9) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 445 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_TiltAndFlatnessFoHistoryExtItemTableAccessor, L" INSERT INTO TiltAndFlatnessFoHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 445 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_TiltAndFlatnessFoHistoryExtItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_azimuthAngle),  _SIZE_TYPE(m_azimuthAngle), 0, 0, offsetbuf(m_azimuthAngle), 0, offsetbuf(m_status_azimuthAngle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_deviationJ3),  _SIZE_TYPE(m_deviationJ3), 0, 0, offsetbuf(m_deviationJ3), 0, offsetbuf(m_status_deviationJ3))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_deviationJ4),  _SIZE_TYPE(m_deviationJ4), 0, 0, offsetbuf(m_deviationJ4), 0, offsetbuf(m_status_deviationJ4))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_deviationJ5),  _SIZE_TYPE(m_deviationJ5), 0, 0, offsetbuf(m_deviationJ5), 0, offsetbuf(m_status_deviationJ5))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_deviationJ6),  _SIZE_TYPE(m_deviationJ6), 0, 0, offsetbuf(m_deviationJ6), 0, offsetbuf(m_status_deviationJ6))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_deviationJ7),  _SIZE_TYPE(m_deviationJ7), 0, 0, offsetbuf(m_deviationJ7), 0, offsetbuf(m_status_deviationJ7))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_deviationJ8),  _SIZE_TYPE(m_deviationJ8), 0, 0, offsetbuf(m_deviationJ8), 0, offsetbuf(m_status_deviationJ8))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_deviationJ9),  _SIZE_TYPE(m_deviationJ9), 0, 0, offsetbuf(m_deviationJ9), 0, offsetbuf(m_status_deviationJ9))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 445 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryExtItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 445 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class TiltAndFlatnessFoHistoryExtItemTable : public CCommand<CAccessor<_TiltAndFlatnessFoHistoryExtItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO TiltAndFlatnessFoHistoryExtItem (historyID, azimuthAngle, deviationJ3, deviationJ4, deviationJ5, deviationJ6, deviationJ7, deviationJ8, deviationJ9) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class TiltAndFlatnessFoHistory
{
private:
  TiltAndFlatnessFoHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    LONG m_numberOfMeasurements ;
    CString m_reference ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
		double m_IndexArmLength ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
//    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
    double m_maximumDeviation ;
    double m_azimuth ;
		double m_IndexArmLength ;
  } ;

  class ItemErrData
  {
  public:
    double m_azimuth ;
    double m_error ;
  } ;

	class ExtItemData
  {
  public:
		double m_azimuthAngle ;
		double m_deviationJ3 ;
		double m_deviationJ4 ;
		double m_deviationJ5 ;
		double m_deviationJ6 ;
		double m_deviationJ7 ;
		double m_deviationJ8 ;
		double m_deviationJ9 ;
  } ;
	DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO GyroPerformanceTestHistory (historyID, ship, timeConstant, comment, measuredUnit) VALUES (?,?,?,?,?) }
]

class /*+++ symbol was renamed:  GyroPerformanceTestHistoryTable
 :---*/ _GyroPerformanceTestHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_L(4, TCHAR, comment, 255) ;
	PARM_DB_L(5, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 528 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_GyroPerformanceTestHistoryTableAccessor, L" INSERT INTO GyroPerformanceTestHistory (historyID, ship, timeConstant, comment, measuredUnit) VALUES (?,?,?,?,?) ")
			
#injected_line 528 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_GyroPerformanceTestHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GyroPerformanceTestHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GyroPerformanceTestHistoryTable : public CCommand<CAccessor<_GyroPerformanceTestHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO GyroPerformanceTestHistory (historyID, ship, timeConstant, comment, measuredUnit) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO GyroPerformanceTestHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  GyroPerformanceTestHistoryItemTable
 :---*/ _GyroPerformanceTestHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 550 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_GyroPerformanceTestHistoryItemTableAccessor, L" INSERT INTO GyroPerformanceTestHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 550 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_GyroPerformanceTestHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 550 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GyroPerformanceTestHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 550 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class GyroPerformanceTestHistoryItemTable : public CCommand<CAccessor<_GyroPerformanceTestHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO GyroPerformanceTestHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class GyroPerformanceTestHistory
{
private:
  GyroPerformanceTestHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO AzimuthAlignmentErrorsHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthAlignmentErrorsHistoryTable
 :---*/ _AzimuthAlignmentErrorsHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
	PARM_DB_V(4, double, rollExcentricity) ;
  PARM_DB_L(5, TCHAR, comment, 255) ;
	PARM_DB_L(6, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 613 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthAlignmentErrorsHistoryTableAccessor, L" INSERT INTO AzimuthAlignmentErrorsHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ")
			
#injected_line 613 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthAlignmentErrorsHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_rollExcentricity),  _SIZE_TYPE(m_rollExcentricity), 0, 0, offsetbuf(m_rollExcentricity), 0, offsetbuf(m_status_rollExcentricity))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 613 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthAlignmentErrorsHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 613 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthAlignmentErrorsHistoryTable : public CCommand<CAccessor<_AzimuthAlignmentErrorsHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthAlignmentErrorsHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO AzimuthAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthAlignmentErrorsHistoryItemTable
 :---*/ _AzimuthAlignmentErrorsHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
	PARM_DB_V(6, double, nominalAzimuth ) ;
	PARM_DB_V(7, double, nominalAzimuthdifference ) ;
	PARM_DB_V(8, double, measuredAzimuthDifference ) ;
	PARM_DB_V(9, double, measuredNominalDifference ) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 636 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthAlignmentErrorsHistoryItemTableAccessor, L" INSERT INTO AzimuthAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 636 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthAlignmentErrorsHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_nominalAzimuth),  _SIZE_TYPE(m_nominalAzimuth), 0, 0, offsetbuf(m_nominalAzimuth), 0, offsetbuf(m_status_nominalAzimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_nominalAzimuthdifference),  _SIZE_TYPE(m_nominalAzimuthdifference), 0, 0, offsetbuf(m_nominalAzimuthdifference), 0, offsetbuf(m_status_nominalAzimuthdifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_measuredAzimuthDifference),  _SIZE_TYPE(m_measuredAzimuthDifference), 0, 0, offsetbuf(m_measuredAzimuthDifference), 0, offsetbuf(m_status_measuredAzimuthDifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_measuredNominalDifference),  _SIZE_TYPE(m_measuredNominalDifference), 0, 0, offsetbuf(m_measuredNominalDifference), 0, offsetbuf(m_status_measuredNominalDifference))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 636 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthAlignmentErrorsHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 636 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthAlignmentErrorsHistoryItemTable : public CCommand<CAccessor<_AzimuthAlignmentErrorsHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class AzimuthAlignmentErrorsHistory
{
private:
  AzimuthAlignmentErrorsHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
		double m_rollExcentricity ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
		double m_nominalAzimuth ;
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO AzimuthVerificationBenchmarkHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthVerificationBenchmarkHistoryTable
 :---*/ _AzimuthVerificationBenchmarkHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
	PARM_DB_V(4, double, rollExcentricity) ;
  PARM_DB_L(5, TCHAR, comment, 255) ;
	PARM_DB_L(6, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 700 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthVerificationBenchmarkHistoryTableAccessor, L" INSERT INTO AzimuthVerificationBenchmarkHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ")
			
#injected_line 700 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthVerificationBenchmarkHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_rollExcentricity),  _SIZE_TYPE(m_rollExcentricity), 0, 0, offsetbuf(m_rollExcentricity), 0, offsetbuf(m_status_rollExcentricity))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 700 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationBenchmarkHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 700 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationBenchmarkHistoryTable : public CCommand<CAccessor<_AzimuthVerificationBenchmarkHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthVerificationBenchmarkHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO AzimuthVerificationBenchmarkHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthVerificationBenchmarkHistoryItemTable
 :---*/ _AzimuthVerificationBenchmarkHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
	PARM_DB_V(6, double, nominalAzimuth ) ;
	PARM_DB_V(7, double, nominalAzimuthdifference ) ;
	PARM_DB_V(8, double, measuredAzimuthDifference ) ;
	PARM_DB_V(9, double, measuredNominalDifference ) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 723 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthVerificationBenchmarkHistoryItemTableAccessor, L" INSERT INTO AzimuthVerificationBenchmarkHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 723 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthVerificationBenchmarkHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_nominalAzimuth),  _SIZE_TYPE(m_nominalAzimuth), 0, 0, offsetbuf(m_nominalAzimuth), 0, offsetbuf(m_status_nominalAzimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_nominalAzimuthdifference),  _SIZE_TYPE(m_nominalAzimuthdifference), 0, 0, offsetbuf(m_nominalAzimuthdifference), 0, offsetbuf(m_status_nominalAzimuthdifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_measuredAzimuthDifference),  _SIZE_TYPE(m_measuredAzimuthDifference), 0, 0, offsetbuf(m_measuredAzimuthDifference), 0, offsetbuf(m_status_measuredAzimuthDifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_measuredNominalDifference),  _SIZE_TYPE(m_measuredNominalDifference), 0, 0, offsetbuf(m_measuredNominalDifference), 0, offsetbuf(m_status_measuredNominalDifference))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 723 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationBenchmarkHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 723 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationBenchmarkHistoryItemTable : public CCommand<CAccessor<_AzimuthVerificationBenchmarkHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthVerificationBenchmarkHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class AzimuthVerificationBenchmarkHistory
{
private:
  AzimuthVerificationBenchmarkHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
		double m_rollExcentricity ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
		double m_nominalAzimuth ;
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO AzimuthVerificationGyrostabilityHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthVerificationGyrostabHistoryTable
 :---*/ _AzimuthVerificationGyrostabHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
	PARM_DB_V(4, double, rollExcentricity) ;
  PARM_DB_L(5, TCHAR, comment, 255) ;
	PARM_DB_L(6, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 787 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthVerificationGyrostabHistoryTableAccessor, L" INSERT INTO AzimuthVerificationGyrostabilityHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ")
			
#injected_line 787 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthVerificationGyrostabHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_rollExcentricity),  _SIZE_TYPE(m_rollExcentricity), 0, 0, offsetbuf(m_rollExcentricity), 0, offsetbuf(m_status_rollExcentricity))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 787 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationGyrostabHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 787 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationGyrostabHistoryTable : public CCommand<CAccessor<_AzimuthVerificationGyrostabHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthVerificationGyrostabilityHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO AzimuthVerificationGyrostabilityHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  AzimuthVerificationGyrostabHistoryItemTable
 :---*/ _AzimuthVerificationGyrostabHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
	PARM_DB_V(6, double, nominalAzimuth ) ;
	PARM_DB_V(7, double, nominalAzimuthdifference ) ;
	PARM_DB_V(8, double, measuredAzimuthDifference ) ;
	PARM_DB_V(9, double, measuredNominalDifference ) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 810 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_AzimuthVerificationGyrostabHistoryItemTableAccessor, L" INSERT INTO AzimuthVerificationGyrostabilityHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 810 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_AzimuthVerificationGyrostabHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_nominalAzimuth),  _SIZE_TYPE(m_nominalAzimuth), 0, 0, offsetbuf(m_nominalAzimuth), 0, offsetbuf(m_status_nominalAzimuth))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_nominalAzimuthdifference),  _SIZE_TYPE(m_nominalAzimuthdifference), 0, 0, offsetbuf(m_nominalAzimuthdifference), 0, offsetbuf(m_status_nominalAzimuthdifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_measuredAzimuthDifference),  _SIZE_TYPE(m_measuredAzimuthDifference), 0, 0, offsetbuf(m_measuredAzimuthDifference), 0, offsetbuf(m_status_measuredAzimuthDifference))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_measuredNominalDifference),  _SIZE_TYPE(m_measuredNominalDifference), 0, 0, offsetbuf(m_measuredNominalDifference), 0, offsetbuf(m_status_measuredNominalDifference))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 810 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationGyrostabHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 810 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class AzimuthVerificationGyrostabHistoryItemTable : public CCommand<CAccessor<_AzimuthVerificationGyrostabHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO AzimuthVerificationGyrostabilityHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class AzimuthVerificationGyrostabHistory
{
private:
  AzimuthVerificationGyrostabHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
		double m_rollExcentricity ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
		double m_nominalAzimuth ;
		double m_nominalAzimuthdifference ;
		double m_measuredAzimuthDifference ;
		double m_measuredNominalDifference ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO HorizonAbsoluteModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  HorizonAbsoluteModeHistoryTable
 :---*/ _HorizonAbsoluteModeHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_L(4, TCHAR, parallaxCompensation, 3) ;
  PARM_DB_L(5, TCHAR, elevationCompensation, 3) ;
  PARM_DB_L(6, TCHAR, comment, 255) ;
	PARM_DB_L(7, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 874 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HorizonAbsoluteModeHistoryTableAccessor, L" INSERT INTO HorizonAbsoluteModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ")
			
#injected_line 874 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HorizonAbsoluteModeHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_parallaxCompensation),  _SIZE_TYPE(m_parallaxCompensation), 0, 0, offsetbuf(m_parallaxCompensation), offsetbuf(m_length_parallaxCompensation), offsetbuf(m_status_parallaxCompensation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_elevationCompensation),  _SIZE_TYPE(m_elevationCompensation), 0, 0, offsetbuf(m_elevationCompensation), offsetbuf(m_length_elevationCompensation), offsetbuf(m_status_elevationCompensation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 874 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonAbsoluteModeHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 874 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonAbsoluteModeHistoryTable : public CCommand<CAccessor<_HorizonAbsoluteModeHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO HorizonAbsoluteModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO HorizonAbsoluteModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  HorizonAbsoluteModeHistoryItemTable
 :---*/ _HorizonAbsoluteModeHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;
  PARM_DB_V(10, double, elevation) ;
  PARM_DB_V(11, double, standardDeviation) ;
	PARM_DB_V(12, double, maximumDeviation) ;
	PARM_DB_V(13, double, azimuth) ;
	 
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 898 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HorizonAbsoluteModeHistoryItemTableAccessor, L" INSERT INTO HorizonAbsoluteModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 898 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HorizonAbsoluteModeHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(11, _OLEDB_TYPE(m_standardDeviation),  _SIZE_TYPE(m_standardDeviation), 0, 0, offsetbuf(m_standardDeviation), 0, offsetbuf(m_status_standardDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(12, _OLEDB_TYPE(m_maximumDeviation),  _SIZE_TYPE(m_maximumDeviation), 0, 0, offsetbuf(m_maximumDeviation), 0, offsetbuf(m_status_maximumDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(13, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 898 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonAbsoluteModeHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 898 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonAbsoluteModeHistoryItemTable : public CCommand<CAccessor<_HorizonAbsoluteModeHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO HorizonAbsoluteModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class HorizonAbsoluteModeHistory
{
private:
  HorizonAbsoluteModeHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
		CString m_parallaxCompensation ;
    CString m_elevationCompensation ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
		double m_maximumDeviation ;
		double m_azimuth ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO HorizonRelativeModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  HorizonRelativeModeHistoryTable
 :---*/ _HorizonRelativeModeHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_L(4, TCHAR, parallaxCompensation, 3) ;
  PARM_DB_L(5, TCHAR, elevationCompensation, 3) ;
  PARM_DB_L(6, TCHAR, comment, 255) ;
	PARM_DB_L(7, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 971 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HorizonRelativeModeHistoryTableAccessor, L" INSERT INTO HorizonRelativeModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ")
			
#injected_line 971 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HorizonRelativeModeHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_parallaxCompensation),  _SIZE_TYPE(m_parallaxCompensation), 0, 0, offsetbuf(m_parallaxCompensation), offsetbuf(m_length_parallaxCompensation), offsetbuf(m_status_parallaxCompensation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_elevationCompensation),  _SIZE_TYPE(m_elevationCompensation), 0, 0, offsetbuf(m_elevationCompensation), offsetbuf(m_length_elevationCompensation), offsetbuf(m_status_elevationCompensation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 971 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonRelativeModeHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 971 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonRelativeModeHistoryTable : public CCommand<CAccessor<_HorizonRelativeModeHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO HorizonRelativeModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO HorizonRelativeModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  HorizonRelativeModeHistoryItemTable
 :---*/ _HorizonRelativeModeHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
  PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;
  PARM_DB_V(10, double, elevation) ;
  PARM_DB_V(11, double, standardDeviation) ;
	PARM_DB_V(12, double, maximumDeviation) ;
	PARM_DB_V(13, double, azimuth) ;
	 
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 995 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_HorizonRelativeModeHistoryItemTableAccessor, L" INSERT INTO HorizonRelativeModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 995 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_HorizonRelativeModeHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_elevation),  _SIZE_TYPE(m_elevation), 0, 0, offsetbuf(m_elevation), 0, offsetbuf(m_status_elevation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(11, _OLEDB_TYPE(m_standardDeviation),  _SIZE_TYPE(m_standardDeviation), 0, 0, offsetbuf(m_standardDeviation), 0, offsetbuf(m_status_standardDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(12, _OLEDB_TYPE(m_maximumDeviation),  _SIZE_TYPE(m_maximumDeviation), 0, 0, offsetbuf(m_maximumDeviation), 0, offsetbuf(m_status_maximumDeviation))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(13, _OLEDB_TYPE(m_azimuth),  _SIZE_TYPE(m_azimuth), 0, 0, offsetbuf(m_azimuth), 0, offsetbuf(m_status_azimuth))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 995 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonRelativeModeHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 995 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class HorizonRelativeModeHistoryItemTable : public CCommand<CAccessor<_HorizonRelativeModeHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO HorizonRelativeModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class HorizonRelativeModeHistory
{
private:
  HorizonRelativeModeHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
		CString m_parallaxCompensation ;
    CString m_elevationCompensation ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
    CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_elevation ;
    double m_standardDeviation ;
		double m_maximumDeviation ;
		double m_azimuth ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO CommonFlatTiltHistory (historyID, ship, timeConstant, comment, measuredUnit ) VALUES (?,?,?,?,?) }
]
class /*+++ symbol was renamed:  CommonFlatTiltHistoryTable
 :---*/ _CommonFlatTiltHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_L(4, TCHAR, comment, 255) ;
	PARM_DB_L(5, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1068 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_CommonFlatTiltHistoryTableAccessor, L" INSERT INTO CommonFlatTiltHistory (historyID, ship, timeConstant, comment, measuredUnit ) VALUES (?,?,?,?,?) ")
			
#injected_line 1068 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_CommonFlatTiltHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1068 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class CommonFlatTiltHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1068 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class CommonFlatTiltHistoryTable : public CCommand<CAccessor<_CommonFlatTiltHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO CommonFlatTiltHistory (historyID, ship, timeConstant, comment, measuredUnit ) VALUES (?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO CommonFlatTiltHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  CommonFlatTiltHistoryItemTable
 :---*/ _CommonFlatTiltHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
	PARM_DB_L(2, TCHAR, station, 32) ;
	PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_V(5, double, roll) ;
  PARM_DB_V(6, double, pitch) ;
  PARM_DB_V(7, double, parallellBias) ;
  PARM_DB_V(8, double, perpendicularBias) ;
  PARM_DB_V(9, double, temperature) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1089 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_CommonFlatTiltHistoryItemTableAccessor, L" INSERT INTO CommonFlatTiltHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (?,?,?,?,?,?,?,?,?) ")
			
#injected_line 1089 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_CommonFlatTiltHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_parallellBias),  _SIZE_TYPE(m_parallellBias), 0, 0, offsetbuf(m_parallellBias), 0, offsetbuf(m_status_parallellBias))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_perpendicularBias),  _SIZE_TYPE(m_perpendicularBias), 0, 0, offsetbuf(m_perpendicularBias), 0, offsetbuf(m_status_perpendicularBias))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1089 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class CommonFlatTiltHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1089 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class CommonFlatTiltHistoryItemTable : public CCommand<CAccessor<_CommonFlatTiltHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO CommonFlatTiltHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class CommonFlatTiltHistory
{
private:
  CommonFlatTiltHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
		CString m_station ;
		CString m_channel ;
    CString m_sensorSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_parallellBias ;
    double m_perpendicularBias ;
    double m_temperature ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO LiveGraphErrorsHistory (historyID, ship, timeConstant, samplingRate, comment, measuredUnit) VALUES (?,?,?,?,?,?) }
]
class /*+++ symbol was renamed:  LiveGraphErrorsHistoryTable
 :---*/ _LiveGraphErrorsHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, timeConstant) ;
  PARM_DB_V(4, double, samplingRate) ;
  PARM_DB_L(5, TCHAR, comment, 255) ;
	PARM_DB_L(6, TCHAR, measuredUnit, 10) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1152 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_LiveGraphErrorsHistoryTableAccessor, L" INSERT INTO LiveGraphErrorsHistory (historyID, ship, timeConstant, samplingRate, comment, measuredUnit) VALUES (?,?,?,?,?,?) ")
			
#injected_line 1152 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_LiveGraphErrorsHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_timeConstant),  _SIZE_TYPE(m_timeConstant), 0, 0, offsetbuf(m_timeConstant), 0, offsetbuf(m_status_timeConstant))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_samplingRate),  _SIZE_TYPE(m_samplingRate), 0, 0, offsetbuf(m_samplingRate), 0, offsetbuf(m_status_samplingRate))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_measuredUnit),  _SIZE_TYPE(m_measuredUnit), 0, 0, offsetbuf(m_measuredUnit), offsetbuf(m_length_measuredUnit), offsetbuf(m_status_measuredUnit))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1152 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveGraphErrorsHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1152 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveGraphErrorsHistoryTable : public CCommand<CAccessor<_LiveGraphErrorsHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO LiveGraphErrorsHistory (historyID, ship, timeConstant, samplingRate, comment, measuredUnit) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO LiveGraphErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (?,?,?,?,?,?,?,?,?,?) }
]

class /*+++ symbol was renamed:  LiveGraphErrorsHistoryItemTable
 :---*/ _LiveGraphErrorsHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
	PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_L(4, TCHAR, sensorSerialNumber, 3) ;
  PARM_DB_L(5, TCHAR, adapterSerialNumber, 3) ;
  PARM_DB_V(6, double, roll) ;
  PARM_DB_V(7, double, pitch) ;
  PARM_DB_V(8, double, tilt) ;
  PARM_DB_V(9, double, angle) ;
  PARM_DB_V(10, double, temperature) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1174 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_LiveGraphErrorsHistoryItemTableAccessor, L" INSERT INTO LiveGraphErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (?,?,?,?,?,?,?,?,?,?) ")
			
#injected_line 1174 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_LiveGraphErrorsHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_sensorSerialNumber),  _SIZE_TYPE(m_sensorSerialNumber), 0, 0, offsetbuf(m_sensorSerialNumber), offsetbuf(m_length_sensorSerialNumber), offsetbuf(m_status_sensorSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_adapterSerialNumber),  _SIZE_TYPE(m_adapterSerialNumber), 0, 0, offsetbuf(m_adapterSerialNumber), offsetbuf(m_length_adapterSerialNumber), offsetbuf(m_status_adapterSerialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_roll),  _SIZE_TYPE(m_roll), 0, 0, offsetbuf(m_roll), 0, offsetbuf(m_status_roll))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(7, _OLEDB_TYPE(m_pitch),  _SIZE_TYPE(m_pitch), 0, 0, offsetbuf(m_pitch), 0, offsetbuf(m_status_pitch))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(8, _OLEDB_TYPE(m_tilt),  _SIZE_TYPE(m_tilt), 0, 0, offsetbuf(m_tilt), 0, offsetbuf(m_status_tilt))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(9, _OLEDB_TYPE(m_angle),  _SIZE_TYPE(m_angle), 0, 0, offsetbuf(m_angle), 0, offsetbuf(m_status_angle))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(10, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1174 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveGraphErrorsHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1174 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveGraphErrorsHistoryItemTable : public CCommand<CAccessor<_LiveGraphErrorsHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO LiveGraphErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (?,?,?,?,?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class LiveGraphErrorsHistory
{
private:
  LiveGraphErrorsHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_timeConstant ;
    double m_samplingRate ;
    CString m_comment ;
		CString m_measuredUnit ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
		CString m_channel ;
    CString m_sensorSerialNumber ;
    CString m_adapterSerialNumber ;
    double m_roll ;
    double m_pitch ;
    double m_tilt ;
    double m_angle ;
    double m_temperature ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

[
  db_command { INSERT INTO LiveDataA202ErrorsHistory ([historyID], [ship], [samplingRate], [comment]) VALUES (?,?,?,?) }
]
class /*+++ symbol was renamed:  LiveDataA202ErrorsHistoryTable
 :---*/ _LiveDataA202ErrorsHistoryTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, ship, 32) ;
  PARM_DB_V(3, double, samplingRate) ;
  PARM_DB_L(4, TCHAR, comment, 255) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1240 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_LiveDataA202ErrorsHistoryTableAccessor, L" INSERT INTO LiveDataA202ErrorsHistory ([historyID], [ship], [samplingRate], [comment]) VALUES (?,?,?,?) ")
			
#injected_line 1240 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_LiveDataA202ErrorsHistoryTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_ship),  _SIZE_TYPE(m_ship), 0, 0, offsetbuf(m_ship), offsetbuf(m_length_ship), offsetbuf(m_status_ship))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_samplingRate),  _SIZE_TYPE(m_samplingRate), 0, 0, offsetbuf(m_samplingRate), 0, offsetbuf(m_status_samplingRate))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1240 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveDataA202ErrorsHistoryTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1240 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveDataA202ErrorsHistoryTable : public CCommand<CAccessor<_LiveDataA202ErrorsHistoryTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO LiveDataA202ErrorsHistory ([historyID], [ship], [samplingRate], [comment]) VALUES (?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[
  db_command { INSERT INTO LiveDataA202ErrorsHistoryItem ([historyID], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (?,?,?,?,?,?) }
]
class /*+++ symbol was renamed:  LiveDataA202ErrorsHistoryItemTable
 :---*/ _LiveDataA202ErrorsHistoryItemTableAccessor {
public:
  PARM_DB_V(1, LONG, historyID) ;
  PARM_DB_L(2, TCHAR, station, 32) ;
	PARM_DB_L(3, TCHAR, channel, 3) ;
  PARM_DB_V(4, double, measuredValue) ;
  PARM_DB_V(5, double, gearing) ;
  PARM_DB_V(6, double, refVoltage) ;

  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 1260 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

				DEFINE_COMMAND_EX(_LiveDataA202ErrorsHistoryItemTableAccessor, L" INSERT INTO LiveDataA202ErrorsHistoryItem ([historyID], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (?,?,?,?,?,?) ")
			
#injected_line 1260 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

					BEGIN_PARAM_MAP(_LiveDataA202ErrorsHistoryItemTableAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_station),  _SIZE_TYPE(m_station), 0, 0, offsetbuf(m_station), offsetbuf(m_length_station), offsetbuf(m_status_station))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_channel),  _SIZE_TYPE(m_channel), 0, 0, offsetbuf(m_channel), offsetbuf(m_length_channel), offsetbuf(m_status_channel))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_measuredValue),  _SIZE_TYPE(m_measuredValue), 0, 0, offsetbuf(m_measuredValue), 0, offsetbuf(m_status_measuredValue))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_gearing),  _SIZE_TYPE(m_gearing), 0, 0, offsetbuf(m_gearing), 0, offsetbuf(m_status_gearing))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_refVoltage),  _SIZE_TYPE(m_refVoltage), 0, 0, offsetbuf(m_refVoltage), 0, offsetbuf(m_status_refVoltage))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1260 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveDataA202ErrorsHistoryItemTable;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 1260 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\History.h"

class LiveDataA202ErrorsHistoryItemTable : public CCommand<CAccessor<_LiveDataA202ErrorsHistoryItemTableAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO LiveDataA202ErrorsHistoryItem ([historyID], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


class LiveDataA202ErrorsHistory
{
private:
  LiveDataA202ErrorsHistory( void ) {} ;
public:
  class Data
  {
  public:
    double m_samplingRate ;
    CString m_comment ;
		DBTIMESTAMP m_time ;
  } ;

  class ItemData
  {
  public:
    CString m_station ;
		CString m_channel ;
    double m_value ;
    double m_gearing ;
    double m_refVoltage ;
  } ;
  class ItemErrData
  {
  public:
  } ;
  class ExtItemData
  {
  public:
  } ;
  DECLARE_HISTORY ;
} ;

BOOL MoveFilesToTemporary( LONG mainId ) ;
