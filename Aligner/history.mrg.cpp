// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\history.mrg.cpp
// compiler-generated file created 05/10/06 at 15:50:51
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: history.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $F
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "Resource.h"
#include "SensorDatabase.h"
#include "History.h"
#include "Database.h"
#include "Digital.h"
#include "DAU.h"
#include "GlobalData.h"

[ 
  db_command { DELETE FROM History WHERE ID = ? }
] 
class /*+++ symbol was renamed:  DeleteHistory  :---*/ _DeleteHistoryAccessor {
public:
  PARM_DB_V(1, LONG, ID) ;

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 15 ".\\History.cpp"

				DEFINE_COMMAND_EX(_DeleteHistoryAccessor, L" DELETE FROM History WHERE ID = ? ")
			
#injected_line 15 ".\\History.cpp"

					BEGIN_PARAM_MAP(_DeleteHistoryAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_ID),  _SIZE_TYPE(m_ID), 0, 0, offsetbuf(m_ID), 0, offsetbuf(m_status_ID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 15 ".\\History.cpp"

class DeleteHistory;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 15 ".\\History.cpp"

class DeleteHistory : public CCommand<CAccessor<_DeleteHistoryAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM History WHERE ID = ? ";
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
    db_command { SELECT DISTINCT filename FROM GraphHistory WHERE include=False And historyID=? }
]
class /*+++ symbol was renamed:  MoveFiles  :---*/ _MoveFilesAccessor {
public:
    PARM_DB_V(1, LONG, ID) ;
    DECL_DB_L(1, TCHAR, filename, MAX_PATH) ;
    void GetRowsetProperties( CDBPropSet *pPropSet )
    {
        pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
        pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
        pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
        pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
    }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 23 ".\\History.cpp"

				DEFINE_COMMAND_EX(_MoveFilesAccessor, L" SELECT DISTINCT filename FROM GraphHistory WHERE include=False And historyID=? ")
			
#injected_line 23 ".\\History.cpp"

						BEGIN_ACCESSOR_MAP(_MoveFilesAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_filename),  _SIZE_TYPE(m_filename), 0, 0, offsetbuf(m_filename), offsetbuf(m_length_filename), offsetbuf(m_status_filename))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 23 ".\\History.cpp"

					BEGIN_PARAM_MAP(_MoveFilesAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_ID),  _SIZE_TYPE(m_ID), 0, 0, offsetbuf(m_ID), 0, offsetbuf(m_status_ID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 23 ".\\History.cpp"

class MoveFiles;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 23 ".\\History.cpp"

class MoveFiles : public CCommand<CAccessor<_MoveFilesAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT filename FROM GraphHistory WHERE include=False And historyID=? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'




#define SET_COMMENT(name) \
  [ db_command { UPDATE name SET comment = ? WHERE historyID = ? } ] class name##Comment

#define COMMENT_TABLE_BODY \
public: \
  PARM_DB_L(1, TCHAR, comment, 255) ; \
  PARM_DB_V(2, LONG, historyID) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

#define GUARD(s) do { HRESULT hResult ; if (( S_OK != (hResult = s) )) { Database::DisplayError( hResult, __FILE__, __LINE__ ) ; return FALSE ; } } while (0) ;
#if defined(_DEBUG)
#define GUARD_DEBUG(s,text) do { HRESULT hResult ; if (( S_OK != (hResult = s) )) { Database::DisplayError( hResult, text ) ; return FALSE ; } } while (0) ;
#else
#define GUARD_DEBUG(s,text) GUARD(s)
#endif

//    DeleteHistoryPrintTableItem printDeleter ; \
//    SET_DB_VALUE(printDeleter,historyID,m_mainID) ; \
//    return ::OpenTable( printDeleter ) ; \


BOOL DoDelete( LONG index )
{
    DeleteHistory deleter ;
    SET_DB_VALUE(deleter,ID,index) ;
    BOOL flag = SUCCEEDED(::OpenTable( deleter )) ;
    return flag ;
}

#define IMPLEMENT_HISTORY(name) \
  LONG name##History::m_lastID ; \
  LONG name##History::m_mainID ; \
  LONG name##History::GetLastID( void ) { return m_lastID ; } \
  LONG name##History::GetMainID( void ) { return m_mainID ; } \
  BOOL name##History::DeleteLast( void ) { \
    return DoDelete( m_mainID ) ; \
  } \
  BOOL name##History::SetComment( const CString &comment) { \
    name##HistoryComment table ; \
    SET_DB_VALUE(table,historyID,m_mainID) ; \
    SET_DB_STRING(table,comment,comment) ; \
    return S_OK == ::OpenTable( table ) ; \
  } \
  BOOL name##History::AddGraph( CString filename, BOOL include ) { \
    GraphHistoryTable table ; \
    if (0 == m_mainID) { \
    HistoryTable table ; \
    LastCounter lastID ; \
    DBTIMESTAMP currentTime ; \
    CTime::GetCurrentTime().GetAsDBTIMESTAMP( currentTime ) ; \
    SET_DB_VALUE_NOCHECK(table, time, currentTime) ; \
    SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ; \
    SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ; \
    SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ; \
    SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ; \
    SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ; \
    if (FAILED(::OpenTable( table ))) { \
      return FALSE ; \
    } \
    GUARD_DEBUG(::OpenTable( lastID ), _T("second debug line") ) ; \
    GUARD_DEBUG(lastID.MoveFirst(), _T("third debug line") ) ; \
    m_mainID = lastID.m_id ; \
    } \
    SET_DB_VALUE(table,historyID,m_mainID) ; \
    SET_DB_STRING(table,filename,filename) ; \
    SET_DB_VALUE(table,include,include) ; \
    return S_OK == ::OpenTable( table ) ; \
  } \
  void name##History::ResetMainID( void ) { \
    m_mainID = 0 ; \
  }

SET_COMMENT(TiltAlignmentErrorsHistory)  :---*/ _TiltAlignmentErrorsHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 118 ".\\History.cpp"

				DEFINE_COMMAND_EX(_TiltAlignmentErrorsHistoryCommentAccessor, L" UPDATE TiltAlignmentErrorsHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 118 ".\\History.cpp"

					BEGIN_PARAM_MAP(_TiltAlignmentErrorsHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 118 ".\\History.cpp"

class TiltAlignmentErrorsHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 118 ".\\History.cpp"

class TiltAlignmentErrorsHistoryComment : public CCommand<CAccessor<_TiltAlignmentErrorsHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE TiltAlignmentErrorsHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(TiltAndFlatnessHistory)  :---*/ _TiltAndFlatnessHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 119 ".\\History.cpp"

				DEFINE_COMMAND_EX(_TiltAndFlatnessHistoryCommentAccessor, L" UPDATE TiltAndFlatnessHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 119 ".\\History.cpp"

					BEGIN_PARAM_MAP(_TiltAndFlatnessHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 119 ".\\History.cpp"

class TiltAndFlatnessHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 119 ".\\History.cpp"

class TiltAndFlatnessHistoryComment : public CCommand<CAccessor<_TiltAndFlatnessHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE TiltAndFlatnessHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(TiltAndFlatnessFoHistory)  :---*/ _TiltAndFlatnessFoHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 120 ".\\History.cpp"

				DEFINE_COMMAND_EX(_TiltAndFlatnessFoHistoryCommentAccessor, L" UPDATE TiltAndFlatnessFoHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 120 ".\\History.cpp"

					BEGIN_PARAM_MAP(_TiltAndFlatnessFoHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 120 ".\\History.cpp"

class TiltAndFlatnessFoHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 120 ".\\History.cpp"

class TiltAndFlatnessFoHistoryComment : public CCommand<CAccessor<_TiltAndFlatnessFoHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE TiltAndFlatnessFoHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(GyroPerformanceTestHistory)  :---*/ _GyroPerformanceTestHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 121 ".\\History.cpp"

				DEFINE_COMMAND_EX(_GyroPerformanceTestHistoryCommentAccessor, L" UPDATE GyroPerformanceTestHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 121 ".\\History.cpp"

					BEGIN_PARAM_MAP(_GyroPerformanceTestHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 121 ".\\History.cpp"

class GyroPerformanceTestHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 121 ".\\History.cpp"

class GyroPerformanceTestHistoryComment : public CCommand<CAccessor<_GyroPerformanceTestHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE GyroPerformanceTestHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(AzimuthAlignmentErrorsHistory)  :---*/ _AzimuthAlignmentErrorsHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 122 ".\\History.cpp"

				DEFINE_COMMAND_EX(_AzimuthAlignmentErrorsHistoryCommentAccessor, L" UPDATE AzimuthAlignmentErrorsHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 122 ".\\History.cpp"

					BEGIN_PARAM_MAP(_AzimuthAlignmentErrorsHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 122 ".\\History.cpp"

class AzimuthAlignmentErrorsHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 122 ".\\History.cpp"

class AzimuthAlignmentErrorsHistoryComment : public CCommand<CAccessor<_AzimuthAlignmentErrorsHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE AzimuthAlignmentErrorsHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(AzimuthVerificationBenchmarkHistory)  :---*/ _AzimuthVerificationBenchmarkHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 123 ".\\History.cpp"

				DEFINE_COMMAND_EX(_AzimuthVerificationBenchmarkHistoryCommentAccessor, L" UPDATE AzimuthVerificationBenchmarkHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 123 ".\\History.cpp"

					BEGIN_PARAM_MAP(_AzimuthVerificationBenchmarkHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 123 ".\\History.cpp"

class AzimuthVerificationBenchmarkHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 123 ".\\History.cpp"

class AzimuthVerificationBenchmarkHistoryComment : public CCommand<CAccessor<_AzimuthVerificationBenchmarkHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE AzimuthVerificationBenchmarkHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(AzimuthVerificationGyrostabHistory)  :---*/ _AzimuthVerificationGyrostabHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 124 ".\\History.cpp"

				DEFINE_COMMAND_EX(_AzimuthVerificationGyrostabHistoryCommentAccessor, L" UPDATE AzimuthVerificationGyrostabHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 124 ".\\History.cpp"

					BEGIN_PARAM_MAP(_AzimuthVerificationGyrostabHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 124 ".\\History.cpp"

class AzimuthVerificationGyrostabHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 124 ".\\History.cpp"

class AzimuthVerificationGyrostabHistoryComment : public CCommand<CAccessor<_AzimuthVerificationGyrostabHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE AzimuthVerificationGyrostabHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(HorizonAbsoluteModeHistory)  :---*/ _HorizonAbsoluteModeHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 125 ".\\History.cpp"

				DEFINE_COMMAND_EX(_HorizonAbsoluteModeHistoryCommentAccessor, L" UPDATE HorizonAbsoluteModeHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 125 ".\\History.cpp"

					BEGIN_PARAM_MAP(_HorizonAbsoluteModeHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 125 ".\\History.cpp"

class HorizonAbsoluteModeHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 125 ".\\History.cpp"

class HorizonAbsoluteModeHistoryComment : public CCommand<CAccessor<_HorizonAbsoluteModeHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE HorizonAbsoluteModeHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(HorizonRelativeModeHistory)  :---*/ _HorizonRelativeModeHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 126 ".\\History.cpp"

				DEFINE_COMMAND_EX(_HorizonRelativeModeHistoryCommentAccessor, L" UPDATE HorizonRelativeModeHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 126 ".\\History.cpp"

					BEGIN_PARAM_MAP(_HorizonRelativeModeHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 126 ".\\History.cpp"

class HorizonRelativeModeHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 126 ".\\History.cpp"

class HorizonRelativeModeHistoryComment : public CCommand<CAccessor<_HorizonRelativeModeHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE HorizonRelativeModeHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(CommonFlatTiltHistory)  :---*/ _CommonFlatTiltHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 127 ".\\History.cpp"

				DEFINE_COMMAND_EX(_CommonFlatTiltHistoryCommentAccessor, L" UPDATE CommonFlatTiltHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 127 ".\\History.cpp"

					BEGIN_PARAM_MAP(_CommonFlatTiltHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 127 ".\\History.cpp"

class CommonFlatTiltHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 127 ".\\History.cpp"

class CommonFlatTiltHistoryComment : public CCommand<CAccessor<_CommonFlatTiltHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE CommonFlatTiltHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(LiveGraphErrorsHistory)  :---*/ _LiveGraphErrorsHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 128 ".\\History.cpp"

				DEFINE_COMMAND_EX(_LiveGraphErrorsHistoryCommentAccessor, L" UPDATE LiveGraphErrorsHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 128 ".\\History.cpp"

					BEGIN_PARAM_MAP(_LiveGraphErrorsHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 128 ".\\History.cpp"

class LiveGraphErrorsHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 128 ".\\History.cpp"

class LiveGraphErrorsHistoryComment : public CCommand<CAccessor<_LiveGraphErrorsHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE LiveGraphErrorsHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SET_COMMENT(LiveDataA202ErrorsHistory)  :---*/ _LiveDataA202ErrorsHistoryCommentAccessor { COMMENT_TABLE_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 129 ".\\History.cpp"

				DEFINE_COMMAND_EX(_LiveDataA202ErrorsHistoryCommentAccessor, L" UPDATE LiveDataA202ErrorsHistory SET comment = ? WHERE historyID = ? ")
			
#injected_line 129 ".\\History.cpp"

					BEGIN_PARAM_MAP(_LiveDataA202ErrorsHistoryCommentAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_comment),  _SIZE_TYPE(m_comment), 0, 0, offsetbuf(m_comment), offsetbuf(m_length_comment), offsetbuf(m_status_comment))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_historyID),  _SIZE_TYPE(m_historyID), 0, 0, offsetbuf(m_historyID), 0, offsetbuf(m_status_historyID))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 129 ".\\History.cpp"

class LiveDataA202ErrorsHistoryComment;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 129 ".\\History.cpp"

class LiveDataA202ErrorsHistoryComment : public CCommand<CAccessor<_LiveDataA202ErrorsHistoryCommentAccessor>, CNoRowset >
    			
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
            szCommand = L" UPDATE LiveDataA202ErrorsHistory SET comment = ? WHERE historyID = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


IMPLEMENT_HISTORY(TiltAlignmentErrors) ;
IMPLEMENT_HISTORY(TiltAndFlatness) ;
IMPLEMENT_HISTORY(TiltAndFlatnessFo) ;
IMPLEMENT_HISTORY(GyroPerformanceTest) ;
IMPLEMENT_HISTORY(AzimuthAlignmentErrors) ;
IMPLEMENT_HISTORY(AzimuthVerificationBenchmark) ;
IMPLEMENT_HISTORY(AzimuthVerificationGyrostab) ;
IMPLEMENT_HISTORY(HorizonAbsoluteMode) ;
IMPLEMENT_HISTORY(HorizonRelativeMode) ;
IMPLEMENT_HISTORY(CommonFlatTilt) ;
IMPLEMENT_HISTORY(LiveGraphErrors) ;
IMPLEMENT_HISTORY(LiveDataA202Errors) ;

void LoadSigndefString( CString &signdefText )
{
  if( g_AlignerData.SignDef == -1 )
  {
    signdefText.LoadString( IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_NEG ) ;
  }
  else
  {
    signdefText.LoadString( IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_POS ) ;
  }
}

BOOL TiltAlignmentErrorsHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  TiltAlignmentErrorsHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
	SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;  
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_STRING(errors,lineOfSightDirection,data.m_lineOfSightDirection) ;
  SET_DB_STRING(errors,elevationCompensation,data.m_elevationCompensation) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL TiltAlignmentErrorsHistory::AddItem( const ItemData item )
{
  TiltAlignmentErrorsHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,elevation,item.m_elevation) ;
  SET_DB_VALUE(entry,bias,item.m_bias) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL TiltAndFlatnessHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  TiltAndFlatnessHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_VALUE(errors,numberOfMeasurement,data.m_numberOfMeasurements) ;
  SET_DB_STRING(errors,reference,data.m_reference) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL TiltAndFlatnessHistory::AddItem( const ItemData item )
{
  TiltAndFlatnessHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,elevation,item.m_elevation) ;
  SET_DB_VALUE(entry,standardDeviation,item.m_standardDeviation) ;
  SET_DB_VALUE(entry,maximumDeviation,item.m_maximumDeviation) ;
  SET_DB_VALUE(entry,azimuth,item.m_azimuth) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL TiltAndFlatnessHistory::AddExtItem( const ExtItemData extItem )
{
  return TRUE ;
  //TiltAndFlatnessHistoryExtItemTable extEntry ;
  //SET_DB_VALUE(extEntry,historyID,m_lastID) ;
  //SET_DB_VALUE(extEntry,azimuthAngle,extItem.m_azimuthAngle) ;
  //SET_DB_VALUE(extEntry,deviationJ3,extItem.m_deviationJ3) ;
  //SET_DB_VALUE(extEntry,deviationJ4,extItem.m_deviationJ4) ;
  //SET_DB_VALUE(extEntry,deviationJ5,extItem.m_deviationJ5) ;
  //SET_DB_VALUE(extEntry,deviationJ6,extItem.m_deviationJ6) ;
  //SET_DB_VALUE(extEntry,deviationJ7,extItem.m_deviationJ7) ;
  //SET_DB_VALUE(extEntry,deviationJ8,extItem.m_deviationJ8) ;
  //SET_DB_VALUE(extEntry,deviationJ9,extItem.m_deviationJ9) ;
  //GUARD(::OpenTable( extEntry )) ;
  //return TRUE ;
}

BOOL TiltAndFlatnessFoHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  TiltAndFlatnessFoHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_VALUE(errors,numberOfMeasurement,data.m_numberOfMeasurements) ;
  SET_DB_STRING(errors,reference,data.m_reference) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  SET_DB_VALUE(errors,IndexArmLength,data.m_IndexArmLength) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL TiltAndFlatnessFoHistory::AddItem( const ItemData item )
{
  TiltAndFlatnessFoHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  //SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,elevation,item.m_elevation) ;
  SET_DB_VALUE(entry,standardDeviation,item.m_standardDeviation) ;
  SET_DB_VALUE(entry,maximumDeviation,item.m_maximumDeviation) ;
  SET_DB_VALUE(entry,azimuth,item.m_azimuth) ;
  SET_DB_VALUE(entry,IndexArmLength,item.m_IndexArmLength) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL TiltAndFlatnessFoHistory::AddItemErr( const ItemErrData item )
{
  TiltAndFlatnessFoHistoryItemErrTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_VALUE(entry,azimuth,item.m_azimuth) ;
	SET_DB_VALUE(entry,error,item.m_error) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}


BOOL TiltAndFlatnessFoHistory::AddExtItem( const ExtItemData extItem )
{
  return TRUE ;
  //TiltAndFlatnessHistoryExtItemTable extEntry ;
  //SET_DB_VALUE(extEntry,historyID,m_lastID) ;
  //SET_DB_VALUE(extEntry,azimuthAngle,extItem.m_azimuthAngle) ;
  //SET_DB_VALUE(extEntry,deviationJ3,extItem.m_deviationJ3) ;
  //SET_DB_VALUE(extEntry,deviationJ4,extItem.m_deviationJ4) ;
  //SET_DB_VALUE(extEntry,deviationJ5,extItem.m_deviationJ5) ;
  //SET_DB_VALUE(extEntry,deviationJ6,extItem.m_deviationJ6) ;
  //SET_DB_VALUE(extEntry,deviationJ7,extItem.m_deviationJ7) ;
  //SET_DB_VALUE(extEntry,deviationJ8,extItem.m_deviationJ8) ;
  //SET_DB_VALUE(extEntry,deviationJ9,extItem.m_deviationJ9) ;
  //GUARD(::OpenTable( extEntry )) ;
  //return TRUE ;
}


BOOL GyroPerformanceTestHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  GyroPerformanceTestHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL GyroPerformanceTestHistory::AddItem( const ItemData item )
{
  GyroPerformanceTestHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL AzimuthAlignmentErrorsHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  AzimuthAlignmentErrorsHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
	SET_DB_VALUE(errors,rollExcentricity,data.m_rollExcentricity) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL AzimuthAlignmentErrorsHistory::AddItem( const ItemData item )
{
  AzimuthAlignmentErrorsHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,nominalAzimuth,item.m_nominalAzimuth) ;
  SET_DB_VALUE(entry,nominalAzimuthdifference,item.m_nominalAzimuthdifference) ;
  SET_DB_VALUE(entry,measuredAzimuthDifference,item.m_measuredAzimuthDifference) ;
  SET_DB_VALUE(entry,measuredNominalDifference,item.m_measuredNominalDifference) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL AzimuthVerificationBenchmarkHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  AzimuthVerificationBenchmarkHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
	SET_DB_VALUE(errors,rollExcentricity,data.m_rollExcentricity) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL AzimuthVerificationBenchmarkHistory::AddItem( const ItemData item )
{
  AzimuthVerificationBenchmarkHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,nominalAzimuth,item.m_nominalAzimuth) ;
  SET_DB_VALUE(entry,nominalAzimuthdifference,item.m_nominalAzimuthdifference) ;
  SET_DB_VALUE(entry,measuredAzimuthDifference,item.m_measuredAzimuthDifference) ;
  SET_DB_VALUE(entry,measuredNominalDifference,item.m_measuredNominalDifference) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL AzimuthVerificationGyrostabHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  AzimuthVerificationGyrostabHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
	SET_DB_VALUE(errors,rollExcentricity,data.m_rollExcentricity) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL AzimuthVerificationGyrostabHistory::AddItem( const ItemData item )
{
  AzimuthVerificationGyrostabHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,nominalAzimuth,item.m_nominalAzimuth) ;
  SET_DB_VALUE(entry,nominalAzimuthdifference,item.m_nominalAzimuthdifference) ;
  SET_DB_VALUE(entry,measuredAzimuthDifference,item.m_measuredAzimuthDifference) ;
  SET_DB_VALUE(entry,measuredNominalDifference,item.m_measuredNominalDifference) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL HorizonAbsoluteModeHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  HorizonAbsoluteModeHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
	SET_DB_STRING(errors,parallaxCompensation,data.m_parallaxCompensation) ;
	SET_DB_STRING(errors,elevationCompensation,data.m_elevationCompensation) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL HorizonAbsoluteModeHistory::AddItem( const ItemData item )
{
  HorizonAbsoluteModeHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,elevation,item.m_elevation) ;
  SET_DB_VALUE(entry,standardDeviation,item.m_standardDeviation) ;
  SET_DB_VALUE(entry,maximumDeviation,item.m_maximumDeviation) ;
  SET_DB_VALUE(entry,azimuth,item.m_azimuth) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL HorizonRelativeModeHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  HorizonRelativeModeHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
  SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
	SET_DB_STRING(errors,parallaxCompensation,data.m_parallaxCompensation) ;
	SET_DB_STRING(errors,elevationCompensation,data.m_elevationCompensation) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL HorizonRelativeModeHistory::AddItem( const ItemData item )
{
  HorizonRelativeModeHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,elevation,item.m_elevation) ;
  SET_DB_VALUE(entry,standardDeviation,item.m_standardDeviation) ;
  SET_DB_VALUE(entry,maximumDeviation,item.m_maximumDeviation) ;
  SET_DB_VALUE(entry,azimuth,item.m_azimuth) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL CommonFlatTiltHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
	CommonFlatTiltHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
	SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL CommonFlatTiltHistory::AddItem( const ItemData item )
{
	CommonFlatTiltHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
	SET_DB_VALUE(entry,parallellBias,item.m_parallellBias) ;
	SET_DB_VALUE(entry,perpendicularBias,item.m_perpendicularBias) ;
	SET_DB_VALUE(entry,temperature,item.m_temperature) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL LiveGraphErrorsHistory::AddData( const Data &data )
{
  HistoryTable table ;
  LastCounter lastID ;
  LiveGraphErrorsHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
	SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
	SET_DB_VALUE(table,latitude,SystemSetup::GetLatitude()) ;
  CString tmp ;
  LoadSigndefString( tmp ) ;
  SET_DB_STRING(table,angularDefinitionPart1,(LPCTSTR)tmp) ;
  tmp.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
  SET_DB_STRING(table,angularDefinitionPart2,(LPCTSTR)tmp) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,timeConstant,data.m_timeConstant) ;
  SET_DB_VALUE(errors,samplingRate,data.m_samplingRate) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
	SET_DB_STRING(errors,measuredUnit,(LPCTSTR)data.m_measuredUnit) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL LiveGraphErrorsHistory::AddItem( const ItemData item )
{
  LiveGraphErrorsHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_STRING(entry,sensorSerialNumber,item.m_sensorSerialNumber) ;
  SET_DB_STRING(entry,adapterSerialNumber,item.m_adapterSerialNumber) ;
  SET_DB_VALUE(entry,roll,item.m_roll) ;
  SET_DB_VALUE(entry,pitch,item.m_pitch) ;
  SET_DB_VALUE(entry,tilt,item.m_tilt) ;
  SET_DB_VALUE(entry,angle,item.m_angle) ;
  SET_DB_VALUE(entry,temperature,item.m_temperature) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

BOOL LiveDataA202ErrorsHistory::AddData( const Data &data )
{
  HistoryTable2 table ;
  LastCounter lastID ;
  LiveDataA202ErrorsHistoryTable errors ;
	SET_DB_VALUE_NOCHECK(table, time, data.m_time) ;
  SET_DB_STRING(table,operator,(LPCTSTR)SystemSetup::GetOperatorName()) ;
  SET_DB_STRING(table,place,(LPCTSTR)SystemSetup::GetPlace()) ;
	SET_DB_STRING(table,dauSerialNumber,(LPCTSTR)DAU::GetDAU().GetSerialNumber()) ;
  SET_DB_STRING(table,project,(LPCTSTR)SystemSetup::GetProjectName()) ;
  GUARD(::OpenTable( table )) ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  AddToHistoryPrint addPrintItem ;
  SET_DB_VALUE(addPrintItem,historyID,lastID.m_id) ;
  GUARD(::OpenTable( addPrintItem )) ;
  SET_DB_VALUE(errors,historyID,(m_mainID = lastID.m_id)) ;
  SET_DB_STRING(errors,ship,(LPCTSTR)SystemSetup::GetShipName()) ;
  SET_DB_VALUE(errors,samplingRate,data.m_samplingRate) ;
  SET_DB_STRING(errors,comment,(LPCTSTR)data.m_comment) ;
  GUARD(::OpenTable(errors)) ;
  lastID.Close() ;
  GUARD(::OpenTable( lastID )) ;
  GUARD(lastID.MoveFirst()) ;
  m_lastID = lastID.m_id ;
  return TRUE ;
}

BOOL LiveDataA202ErrorsHistory::AddItem( const ItemData item )
{
  LiveDataA202ErrorsHistoryItemTable entry ;
  SET_DB_VALUE(entry,historyID,m_lastID) ;
  SET_DB_STRING(entry,station,(LPCTSTR)item.m_station) ;
  SET_DB_STRING(entry,channel,item.m_channel) ;
  SET_DB_VALUE(entry,measuredValue,item.m_value) ;
  SET_DB_VALUE(entry,gearing,item.m_gearing) ;
  SET_DB_VALUE(entry,refVoltage,item.m_refVoltage) ;
  GUARD(::OpenTable( entry )) ;
  return TRUE ;
}

#include "Util.h"

BOOL MoveFilesToTemporary( LONG mainId )
{
    MoveFiles move ;
    SET_DB_VALUE(move,ID,mainId) ;
    HRESULT hResult = ::OpenTable( move ) ;
    if (FAILED(hResult))
    {
        return FALSE ;
    }
    hResult = move.MoveFirst() ;
    while (S_OK == hResult)
    {
        MoveFileToTemporaryDirectory( CString( move.m_filename ) ) ;
        hResult = move.MoveNext() ;
    }
    return TRUE ;
}
    
