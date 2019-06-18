// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\createpolynomial.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:29
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: createpolynomial.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "SensorDatabase.h"
#include "CreatePolynomial.h"

IMPLEMENT_DYNAMIC(CreatePolynomial, CDialog)

CreatePolynomial::CreatePolynomial(CWnd* pParent /*=NULL*/)	: CDialog(CreatePolynomial::IDD, pParent)
{
  // Empty
}

CreatePolynomial::~CreatePolynomial( void )
{
  // Empty
}

void CreatePolynomial::DoDataExchange( CDataExchange *pDX )
{
  CDialog::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_GRID, m_grid) ;
  DDX_Control(pDX, IDC_CHART_CONTROL, m_graph) ;
	DDX_Control(pDX, IDC_SERIAL_NUMBER, m_serial) ;
	DDX_Control(pDX, IDC_APPROXIMATION_ORDER, m_order) ;
	DDX_Control(pDX, IDC_AXIS, m_axis) ;
	DDX_Control(pDX, IDC_APPROXIMATION_TYPE, m_type) ;
  if (!pDX -> m_bSaveAndValidate)
  {
    m_pGrid = m_grid.GetControlUnknown() ;
    m_pGraph = m_graph.GetControlUnknown() ;
  }
}

BEGIN_MESSAGE_MAP(CreatePolynomial, CDialog)
	ON_CBN_SELCHANGE(IDC_APPROXIMATION_TYPE, OnCbnSelchangeApproximationType)
	ON_CBN_SELCHANGE(IDC_APPROXIMATION_ORDER, OnCbnSelchangeApproximationOrder)
	ON_BN_CLICKED(ID_SAVE, OnBnClickedSave)
	ON_CBN_SELCHANGE(IDC_SERIAL_NUMBER, OnCbnSelchangeSerialNumber)
  ON_CBN_SELCHANGE(IDC_AXIS, OnCbnSelchangeAxis)
  ON_BN_CLICKED(ID_PRINT_GRAPH, OnBnClickedPrintGraph)
END_MESSAGE_MAP()

template <class T>
void GetDataFromSN( T &table, CString serialNumber, vector<SelectedData> &data )
{
  data.clear() ;
  SET_DB_STRING(table, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( table ) ;
  if (S_OK == hResult)
  {
    hResult = table.MoveFirst() ;
    while( S_OK == hResult)
    {
      data.push_back( SelectedData( FALSE, table.m_operator, table.m_value, table.m_time, table.m_temperature, table.m_dauSerialNumber ) ) ;
      hResult = table.MoveNext() ;
    }
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    ASSERT(0) ;
  }
  table.Close() ;
}

#define SENSOR_DATA_FROM_SN(name) \
  [ db_command { SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM name WHERE serialNumber = ? } ] class name##FromSN

#define SENSOR_DATA_FROM_SN_BODY \
  public: \
	DECL_DB_L(1, TCHAR, dauSerialNumber, 4) ; \
	DECL_DB_L(2, TCHAR, operator, 51) ; \
	DECL_DB_V(3, double, temperature) ; \
	DECL_DB_V(4, DBTIMESTAMP, time) ; \
	DECL_DB_V(5, double, value) ; \
  PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

SENSOR_DATA_FROM_SN(SensorPitchOffsetData)   :---*/ _SensorPitchOffsetDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 88 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorPitchOffsetDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchOffsetData WHERE serialNumber = ? ")
			
#injected_line 88 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorPitchOffsetDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 88 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorPitchOffsetDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 88 ".\\CreatePolynomial.cpp"

class SensorPitchOffsetDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 88 ".\\CreatePolynomial.cpp"

class SensorPitchOffsetDataFromSN : public CCommand<CAccessor<_SensorPitchOffsetDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchOffsetData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_DATA_FROM_SN(SensorPitchGainData)     :---*/ _SensorPitchGainDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 89 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorPitchGainDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchGainData WHERE serialNumber = ? ")
			
#injected_line 89 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorPitchGainDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 89 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorPitchGainDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 89 ".\\CreatePolynomial.cpp"

class SensorPitchGainDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 89 ".\\CreatePolynomial.cpp"

class SensorPitchGainDataFromSN : public CCommand<CAccessor<_SensorPitchGainDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchGainData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_DATA_FROM_SN(SensorPitchAzimuthData)  :---*/ _SensorPitchAzimuthDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 90 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorPitchAzimuthDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchAzimuthData WHERE serialNumber = ? ")
			
#injected_line 90 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorPitchAzimuthDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 90 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorPitchAzimuthDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 90 ".\\CreatePolynomial.cpp"

class SensorPitchAzimuthDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 90 ".\\CreatePolynomial.cpp"

class SensorPitchAzimuthDataFromSN : public CCommand<CAccessor<_SensorPitchAzimuthDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorPitchAzimuthData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_DATA_FROM_SN(SensorRollOffsetData)    :---*/ _SensorRollOffsetDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 91 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorRollOffsetDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollOffsetData WHERE serialNumber = ? ")
			
#injected_line 91 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorRollOffsetDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 91 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorRollOffsetDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 91 ".\\CreatePolynomial.cpp"

class SensorRollOffsetDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 91 ".\\CreatePolynomial.cpp"

class SensorRollOffsetDataFromSN : public CCommand<CAccessor<_SensorRollOffsetDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollOffsetData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_DATA_FROM_SN(SensorRollGainData)      :---*/ _SensorRollGainDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 92 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorRollGainDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollGainData WHERE serialNumber = ? ")
			
#injected_line 92 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorRollGainDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 92 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorRollGainDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 92 ".\\CreatePolynomial.cpp"

class SensorRollGainDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 92 ".\\CreatePolynomial.cpp"

class SensorRollGainDataFromSN : public CCommand<CAccessor<_SensorRollGainDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollGainData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

SENSOR_DATA_FROM_SN(SensorRollAzimuthData)   :---*/ _SensorRollAzimuthDataFromSNAccessor { SENSOR_DATA_FROM_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 93 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_SensorRollAzimuthDataFromSNAccessor, L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollAzimuthData WHERE serialNumber = ? ")
			
#injected_line 93 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_SensorRollAzimuthDataFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_dauSerialNumber),  _SIZE_TYPE(m_dauSerialNumber), 0, 0, offsetbuf(m_dauSerialNumber), offsetbuf(m_length_dauSerialNumber), offsetbuf(m_status_dauSerialNumber))
_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_temperature),  _SIZE_TYPE(m_temperature), 0, 0, offsetbuf(m_temperature), 0, offsetbuf(m_status_temperature))
_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_time),  _SIZE_TYPE(m_time), 0, 0, offsetbuf(m_time), 0, offsetbuf(m_status_time))
_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_value),  _SIZE_TYPE(m_value), 0, 0, offsetbuf(m_value), 0, offsetbuf(m_status_value))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 93 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_SensorRollAzimuthDataFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 93 ".\\CreatePolynomial.cpp"

class SensorRollAzimuthDataFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 93 ".\\CreatePolynomial.cpp"

class SensorRollAzimuthDataFromSN : public CCommand<CAccessor<_SensorRollAzimuthDataFromSNAccessor> >
    			
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
            szCommand = L" SELECT dauSerialNumber, operator, temperature, timeStamp, calibrationValue FROM SensorRollAzimuthData WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void CreatePolynomial::SetupData( void )
{
  if (0 <= m_serial.GetCurSel())
  {
    CString serialNumber ;
    m_serial.GetWindowText( serialNumber ) ;
    switch (2 * m_type.GetCurSel() + m_axis.GetCurSel())
    {
    case 0:
      {
        GetDataFromSN( SensorRollOffsetDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    case 1:
      {
        GetDataFromSN( SensorPitchOffsetDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    case 2:
      {
        GetDataFromSN( SensorRollGainDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    case 3:
      {
        GetDataFromSN( SensorPitchGainDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    case 4:
      {
        GetDataFromSN( SensorRollAzimuthDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    case 5:
      {
        GetDataFromSN( SensorPitchAzimuthDataFromSN(), serialNumber, m_selectedData ) ;
      }
    break ;

    default:
      ASSERT(0) ;
      break ;
    }

    SortInTime( m_selectedData );
  }
}

template <class T>
void GetUniqueSerialNumbers( T &table, CComboBox &serial, CString current )
{
  serial.ResetContent() ;
  HRESULT hResult = ::OpenTable( table ) ;
  if (S_OK == hResult)
  {
    hResult = table.MoveFirst() ;
    while( S_OK == hResult)
    {
      CString text ;
      text.Format( "%03d", atoi(table.m_serialNumber) ) ;
      int index = serial.AddString( text ) ;
      if (text == current)
      {
        serial.SetCurSel( index ) ;
      }
      hResult = table.MoveNext() ;
    }
  }
}

UNIQUE_SN(SensorPitchOffsetData)   :---*/ _UniqueSNFromSensorPitchOffsetDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 170 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchOffsetDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchOffsetData ORDER BY serialNumber ")
			
#injected_line 170 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchOffsetDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 170 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchOffsetData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 170 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchOffsetData : public CCommand<CAccessor<_UniqueSNFromSensorPitchOffsetDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchOffsetData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorPitchGainData)     :---*/ _UniqueSNFromSensorPitchGainDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 171 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchGainDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchGainData ORDER BY serialNumber ")
			
#injected_line 171 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchGainDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 171 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchGainData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 171 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchGainData : public CCommand<CAccessor<_UniqueSNFromSensorPitchGainDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchGainData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorPitchAzimuthData)  :---*/ _UniqueSNFromSensorPitchAzimuthDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 172 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorPitchAzimuthDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorPitchAzimuthData ORDER BY serialNumber ")
			
#injected_line 172 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorPitchAzimuthDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 172 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchAzimuthData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 172 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorPitchAzimuthData : public CCommand<CAccessor<_UniqueSNFromSensorPitchAzimuthDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorPitchAzimuthData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollOffsetData)    :---*/ _UniqueSNFromSensorRollOffsetDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 173 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollOffsetDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollOffsetData ORDER BY serialNumber ")
			
#injected_line 173 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollOffsetDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 173 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollOffsetData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 173 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollOffsetData : public CCommand<CAccessor<_UniqueSNFromSensorRollOffsetDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollOffsetData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollGainData)      :---*/ _UniqueSNFromSensorRollGainDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 174 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollGainDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollGainData ORDER BY serialNumber ")
			
#injected_line 174 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollGainDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 174 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollGainData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 174 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollGainData : public CCommand<CAccessor<_UniqueSNFromSensorRollGainDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollGainData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

UNIQUE_SN(SensorRollAzimuthData)   :---*/ _UniqueSNFromSensorRollAzimuthDataAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 175 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromSensorRollAzimuthDataAccessor, L" SELECT DISTINCT serialNumber FROM SensorRollAzimuthData ORDER BY serialNumber ")
			
#injected_line 175 ".\\CreatePolynomial.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromSensorRollAzimuthDataAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 175 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollAzimuthData;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 175 ".\\CreatePolynomial.cpp"

class UniqueSNFromSensorRollAzimuthData : public CCommand<CAccessor<_UniqueSNFromSensorRollAzimuthDataAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM SensorRollAzimuthData ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void CreatePolynomial::SetupSerial( void )
{
  CString current ;
  m_serial.GetWindowText( current ) ;
  switch (2 * m_type.GetCurSel() + m_axis.GetCurSel())
	{
  case 0: // TODO: Match this in a more elegant way
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorRollOffsetData(), m_serial, current ) ;
    }
		break ;

	case 1:
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorPitchOffsetData(), m_serial, current ) ;
    }
		break ;

	case 2:
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorRollGainData(), m_serial, current ) ;
    }
		break ;

	case 3:
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorPitchGainData(), m_serial, current ) ;
    }
		break ;

	case 4:
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorRollAzimuthData(), m_serial, current ) ;
    }
		break ;

	case 5:
    {
      GetUniqueSerialNumbers( UniqueSNFromSensorPitchAzimuthData(), m_serial, current ) ;
    }
		break ;

  default:
		ASSERT(0) ;
		break ;
	}
}

void CreatePolynomial::SetupPolynomial( void )
{
  ClearGraph() ;
  m_polynomialText = _T("") ;
  m_leastSquare.Clear() ;
  double minT = DBL_MAX ;
  double maxT = DBL_MIN ;
  for (vector<SelectedData>::iterator i = m_selectedData.begin() ; i != m_selectedData.end() ; i++)
  {
    int point ;
    if (i -> m_use)
    {
      m_leastSquare.Add( i -> m_temperature, i -> m_value ) ;
      m_pData -> PutNumPoints( 1L, point = m_pData -> GetNumPoints( 1L ) + 1 ) ;
      m_pData -> PutX( 1L, point, i -> m_temperature ) ;
      switch (m_type.GetCurSel())
      {
      case 0:
        m_pData -> PutY( 1L, point, i -> m_value ) ;
        break ;

      case 1:
        m_pData -> PutY( 1L, point, i -> m_value ) ;
        break ;

      case 2:
        m_pData -> PutY( 1L, point, i -> m_value ) ;
        break ;

      default:
        ASSERT(0) ;
        break ;
      }
      minT = min( minT, i -> m_temperature ) ;
      maxT = max( maxT, i -> m_temperature ) ;
    }
  }

  if (maxT < minT)
  {
    return ;
  }
//  minT -= 10.0 ;
//  maxT += 10.0 ;
  minT = -20.0 ;
  maxT = +70.0 ;

  const int numberOfIntervals = 250 ;
  m_fit.Create( m_order.GetCurSel(), m_leastSquare ) ;
  if (0 <= m_fit.GetOrder())
  {
    m_polynomialText += _T("S/N ") ;
    CString serial ;
    m_serial.GetWindowText( serial ) ;
    m_polynomialText += serial ;
    if (0 == m_axis.GetCurSel())
    {
        m_polynomialText += _T(" roll ") ;
    }
    else
    {
        m_polynomialText += _T(" pitch ") ;

    }
    m_pData -> PutIsBatched( TRUE ) ;
    m_pData -> PutNumPoints( 2L, numberOfIntervals + 1 ) ;
    for (int i = 0 ; i <= numberOfIntervals ; i++)
    {
      m_pData -> PutX( 2L, i + 1, minT + i * (maxT - minT) / (double)numberOfIntervals ) ;
      switch (m_type.GetCurSel())
      {
      case 0:
        m_pData -> PutY( 2L, i + 1, m_fit( minT + i * (maxT - minT) / (double)numberOfIntervals ) ) ;
        break ;

      case 1:
        m_pData -> PutY( 2L, i + 1, m_fit( minT + i * (maxT - minT) / (double)numberOfIntervals ) ) ;
        break ;

      case 2:
        m_pData -> PutY( 2L, i + 1, m_fit( minT + i * (maxT - minT) / (double)numberOfIntervals ) ) ;
        break ;

      default:
        ASSERT(0) ;
        break ;
      }
    }
    switch (m_type.GetCurSel())
    {
    case 0:
      m_polynomialText += _T("offset(T)=") ;
      break ;

    case 1:
      m_polynomialText += _T("scale factor(T)=") ;
      break ;

    case 2:
      m_polynomialText += _T("azimuth(T)=") ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
    CString polynomialText ;
    switch (m_fit.GetOrder())
    {
    case 0:
      polynomialText.Format( _T(" %f"), m_fit.m_a_0 ) ;
      break ;
    case 1:
      polynomialText.Format( _T(" %f + %f T"), m_fit.m_a_0, m_fit.m_a_1 ) ;
      break ;
    case 2:
      polynomialText.Format( _T(" %f + %f T + %f T^2"), m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2 ) ;
      break ;
    case 3:
      polynomialText.Format( _T(" %f + %f T + %f T^2 + %f T^3"), m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;
    }
    m_polynomialText += polynomialText ;

    int n = 0 ;
    double sum = 0 ;
    for (vector<SelectedData>::iterator i = m_selectedData.begin() ; i != m_selectedData.end() ; i++)
    {
      if (i -> m_use)
      {
        double value = i -> m_value ;
        double temperature = i -> m_temperature ;
        double term = value - m_fit( temperature ) ;
        sum += term * term ;
        n++ ;
      }
    }
    double sigma = -1.0 ;
    if (1 < n)
    {
      sigma = sqrt( sum / (n - 1) ) ;
    }
    if (1 == m_type.GetCurSel())
    {
      polynomialText.Format( _T(" sigma=%6.3f%% "), sigma * 100) ;
    }
    else
    {
      polynomialText.Format( _T(" sigma=%6.3f "), sigma ) ;
    }
    if (-1 != sigma)
    {
      m_polynomialText += polynomialText ;
    }
    else
    {
      m_polynomialText += _T(" ");
    }
    m_polynomialText += COleDateTime::GetCurrentTime().Format( _T("%x %X") ) ;
    m_pGraph -> GetHeader() -> GetText() -> PutText( _bstr_t( m_polynomialText ) ) ;
    m_pData -> PutIsBatched( FALSE ) ;
  }
}

void CreatePolynomial::ClearGraph( void )
{
  try
  {
    for (int i = 1 ; i <= m_pData -> GetNumSeries() ; i++)
    {
      m_pData -> PutNumPoints( i, 0 ) ;
    }
  }
  catch ( _com_error e )
  {
    TRACE( "Error: %s\n", ( char * )e.Description() ) ;
  }
  switch (m_type.GetCurSel())
  {
  case 0:
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMin() -> PutValue( -5 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMax() -> PutValue( +5 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetOrigin() -> PutValue( 0.0 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetTitle() -> PutText( _T("[mrad]") );
    break ;

  case 1:
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMin() -> PutValue( 0.99 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMax() -> PutValue( 1.01 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetOrigin() -> PutValue( 1.0 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetTitle() -> PutText( _T(" ") );
    break ;

  case 2:
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMin() -> PutValue( -5 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetMax() -> PutValue( +5 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetOrigin() -> PutValue( 0.0 ) ;
    m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "y" ) -> GetTitle() -> PutText( _T("[mrad]") );
    break ;
  }
}

void CreatePolynomial::InitGraph( void )
{
  m_pGroup = m_pGraph -> GetChartGroups() -> GetItem( 1L ) ;
  m_pStyleCollection = m_pGroup -> GetStyles() ;
  m_pData = m_pGroup -> GetData() ;
  m_pData -> PutLayout( oc2dDataGeneral ) ;
  m_pData -> PutNumSeries( 3 ) ;

  m_pGraph -> GetChartArea() -> GetAxes( )-> GetItem( "x" ) -> GetMin() -> PutValue( -20 ) ;
  m_pGraph -> GetChartArea() -> GetAxes( )-> GetItem( "x" ) -> GetMax() -> PutValue( 70 ) ;
  m_pGraph -> GetChartArea() -> GetAxes() -> GetItem( "x" ) -> GetTitle() -> PutText( _T("[°C]") );

  // Serie 1.
  IChartStyle_DualPtr pStyle = m_pStyleCollection -> GetItem( 1L ) ;
  pStyle -> GetSymbol() -> PutShape( oc2dShapeCircle ) ;
  pStyle -> GetSymbol() -> PutSize( 8 ) ;
  pStyle -> GetSymbol() -> PutColor( RGB(0,0,0) ) ;
  pStyle -> GetLine() -> PutPattern( oc2dLineNone ) ;
  pStyle -> GetLine() -> PutColor( RGB(0,0,0) ) ;

  // Serie 2.
  pStyle = m_pStyleCollection -> GetItem( 2L ) ;
  pStyle -> GetSymbol() -> PutShape( oc2dShapeNone ) ;
  pStyle -> GetSymbol() -> PutSize( 1 ) ;
  pStyle -> GetSymbol() -> PutColor( RGB(0,0,0) ) ;
  pStyle -> GetLine() -> PutPattern( oc2dLineSolid ) ;
  pStyle -> GetLine() -> PutColor( RGB(0,0,0) ) ;

  // Serie 3.
  pStyle = m_pStyleCollection -> GetItem( 3L ) ;
  pStyle -> GetSymbol() -> PutShape( oc2dShapeNone ) ;
  pStyle -> GetSymbol() -> PutSize( 1 ) ;
  pStyle -> GetSymbol() -> PutColor( RGB(0,0,0) ) ;
  pStyle -> GetLine() -> PutPattern( oc2dLineShortDash ) ;
  pStyle -> GetLine() -> PutColor( RGB(0,0,0) ) ;

}

BOOL CreatePolynomial::OnInitDialog( void )
{
	CDialog::OnInitDialog() ;
	m_type.SetCurSel( 0 ) ;
    m_axis.SetCurSel( 0 ) ;
	m_order.SetCurSel( 0 ) ;
	SetupSerial() ;
    SetupPolynomial() ;
    InitGraph() ;
	return TRUE ;
}

void CreatePolynomial::OnCbnSelchangeApproximationType( void )
{
  SetupSerial() ;
  SetupData() ;
  m_pGrid -> Close() ;
  m_pGrid -> ReOpen( _variant_t(0L, VT_I4) ) ;
  SetupPolynomial() ;
}

void CreatePolynomial::OnCbnSelchangeAxis( void )
{
  SetupData() ;
  m_pGrid -> Close() ;
  m_pGrid -> ReOpen( _variant_t(0L, VT_I4) ) ;
  SetupPolynomial() ;
}

void CreatePolynomial::OnCbnSelchangeApproximationOrder( void )
{
  SetupPolynomial() ;
}

void CreatePolynomial::OnCbnSelchangeSerialNumber( void )
{
  SetupData() ;
  m_pGrid -> Close() ;
  m_pGrid -> ReOpen( _variant_t(0L, VT_I4) ) ;
  SetupPolynomial() ;
}

#define INSERT_INTO_SENSOR_CALIBRATION_BODY \
  public: \
  PARM_DB_L(1, TCHAR, serialNumber, 4) ; \
	PARM_DB_L(2, TCHAR, operator, 51) ; \
	PARM_DB_V(3, double, a_0) ; \
	PARM_DB_V(4, double, a_1) ; \
	PARM_DB_V(5, double, a_2) ; \
	PARM_DB_V(6, double, a_3) ; \
  void GetRowsetProperties( CDBPropSet *pPropSet ) \
  { \
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ; \
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ; \
  }

#define INSERT_INTO_SENSOR_CALIBRATION(name) \
  [ db_command { INSERT INTO name ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) } ] class InsertInto##name

INSERT_INTO_SENSOR_CALIBRATION(SensorPitchOffsetCalibration)   :---*/ _InsertIntoSensorPitchOffsetCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 526 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchOffsetCalibrationAccessor, L" INSERT INTO SensorPitchOffsetCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 526 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchOffsetCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 526 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 526 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchOffsetCalibration : public CCommand<CAccessor<_InsertIntoSensorPitchOffsetCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchOffsetCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_CALIBRATION(SensorPitchGainCalibration)     :---*/ _InsertIntoSensorPitchGainCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 527 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchGainCalibrationAccessor, L" INSERT INTO SensorPitchGainCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 527 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchGainCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 527 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 527 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchGainCalibration : public CCommand<CAccessor<_InsertIntoSensorPitchGainCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchGainCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_CALIBRATION(SensorPitchAzimuthCalibration)  :---*/ _InsertIntoSensorPitchAzimuthCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 528 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorPitchAzimuthCalibrationAccessor, L" INSERT INTO SensorPitchAzimuthCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 528 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorPitchAzimuthCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 528 ".\\CreatePolynomial.cpp"

class InsertIntoSensorPitchAzimuthCalibration : public CCommand<CAccessor<_InsertIntoSensorPitchAzimuthCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorPitchAzimuthCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_CALIBRATION(SensorRollOffsetCalibration)    :---*/ _InsertIntoSensorRollOffsetCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 529 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollOffsetCalibrationAccessor, L" INSERT INTO SensorRollOffsetCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 529 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollOffsetCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 529 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 529 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollOffsetCalibration : public CCommand<CAccessor<_InsertIntoSensorRollOffsetCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollOffsetCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_CALIBRATION(SensorRollGainCalibration)      :---*/ _InsertIntoSensorRollGainCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 530 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollGainCalibrationAccessor, L" INSERT INTO SensorRollGainCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 530 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollGainCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 530 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 530 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollGainCalibration : public CCommand<CAccessor<_InsertIntoSensorRollGainCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollGainCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

INSERT_INTO_SENSOR_CALIBRATION(SensorRollAzimuthCalibration)   :---*/ _InsertIntoSensorRollAzimuthCalibrationAccessor { INSERT_INTO_SENSOR_CALIBRATION_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 531 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_InsertIntoSensorRollAzimuthCalibrationAccessor, L" INSERT INTO SensorRollAzimuthCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ")
			
#injected_line 531 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_InsertIntoSensorRollAzimuthCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(2, _OLEDB_TYPE(m_operator),  _SIZE_TYPE(m_operator), 0, 0, offsetbuf(m_operator), offsetbuf(m_length_operator), offsetbuf(m_status_operator))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(3, _OLEDB_TYPE(m_a_0),  _SIZE_TYPE(m_a_0), 0, 0, offsetbuf(m_a_0), 0, offsetbuf(m_status_a_0))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(4, _OLEDB_TYPE(m_a_1),  _SIZE_TYPE(m_a_1), 0, 0, offsetbuf(m_a_1), 0, offsetbuf(m_status_a_1))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(5, _OLEDB_TYPE(m_a_2),  _SIZE_TYPE(m_a_2), 0, 0, offsetbuf(m_a_2), 0, offsetbuf(m_status_a_2))
SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(6, _OLEDB_TYPE(m_a_3),  _SIZE_TYPE(m_a_3), 0, 0, offsetbuf(m_a_3), 0, offsetbuf(m_status_a_3))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 531 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 531 ".\\CreatePolynomial.cpp"

class InsertIntoSensorRollAzimuthCalibration : public CCommand<CAccessor<_InsertIntoSensorRollAzimuthCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" INSERT INTO SensorRollAzimuthCalibration ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES (?,?,?,?,?,?) ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


[ db_command { DELETE FROM SensorPitchOffsetCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorPitchOffsetCalibration  :---*/ _DeleteSensorPitchOffsetCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 533 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorPitchOffsetCalibrationAccessor, L" DELETE FROM SensorPitchOffsetCalibration WHERE serialNumber = ? ")
			
#injected_line 533 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorPitchOffsetCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 533 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 533 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchOffsetCalibration : public CCommand<CAccessor<_DeleteSensorPitchOffsetCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorPitchOffsetCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[ db_command { DELETE FROM SensorPitchGainCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorPitchGainCalibration  :---*/ _DeleteSensorPitchGainCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 534 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorPitchGainCalibrationAccessor, L" DELETE FROM SensorPitchGainCalibration WHERE serialNumber = ? ")
			
#injected_line 534 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorPitchGainCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 534 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 534 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchGainCalibration : public CCommand<CAccessor<_DeleteSensorPitchGainCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorPitchGainCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[ db_command { DELETE FROM SensorPitchAzimuthCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorPitchAzimuthCalibration  :---*/ _DeleteSensorPitchAzimuthCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 535 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorPitchAzimuthCalibrationAccessor, L" DELETE FROM SensorPitchAzimuthCalibration WHERE serialNumber = ? ")
			
#injected_line 535 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorPitchAzimuthCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 535 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 535 ".\\CreatePolynomial.cpp"

class DeleteSensorPitchAzimuthCalibration : public CCommand<CAccessor<_DeleteSensorPitchAzimuthCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorPitchAzimuthCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[ db_command { DELETE FROM SensorRollOffsetCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorRollOffsetCalibration  :---*/ _DeleteSensorRollOffsetCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 536 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorRollOffsetCalibrationAccessor, L" DELETE FROM SensorRollOffsetCalibration WHERE serialNumber = ? ")
			
#injected_line 536 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorRollOffsetCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 536 ".\\CreatePolynomial.cpp"

class DeleteSensorRollOffsetCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 536 ".\\CreatePolynomial.cpp"

class DeleteSensorRollOffsetCalibration : public CCommand<CAccessor<_DeleteSensorRollOffsetCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorRollOffsetCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[ db_command { DELETE FROM SensorRollGainCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorRollGainCalibration  :---*/ _DeleteSensorRollGainCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 537 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorRollGainCalibrationAccessor, L" DELETE FROM SensorRollGainCalibration WHERE serialNumber = ? ")
			
#injected_line 537 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorRollGainCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 537 ".\\CreatePolynomial.cpp"

class DeleteSensorRollGainCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 537 ".\\CreatePolynomial.cpp"

class DeleteSensorRollGainCalibration : public CCommand<CAccessor<_DeleteSensorRollGainCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorRollGainCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'

[ db_command { DELETE FROM SensorRollAzimuthCalibration WHERE serialNumber = ? } ] class /*+++ symbol was renamed:  DeleteSensorRollAzimuthCalibration  :---*/ _DeleteSensorRollAzimuthCalibrationAccessor { public: PARM_DB_L(1, TCHAR, serialNumber, 4) 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 538 ".\\CreatePolynomial.cpp"

				DEFINE_COMMAND_EX(_DeleteSensorRollAzimuthCalibrationAccessor, L" DELETE FROM SensorRollAzimuthCalibration WHERE serialNumber = ? ")
			
#injected_line 538 ".\\CreatePolynomial.cpp"

					BEGIN_PARAM_MAP(_DeleteSensorRollAzimuthCalibrationAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 538 ".\\CreatePolynomial.cpp"

class DeleteSensorRollAzimuthCalibration;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 538 ".\\CreatePolynomial.cpp"

class DeleteSensorRollAzimuthCalibration : public CCommand<CAccessor<_DeleteSensorRollAzimuthCalibrationAccessor>, CNoRowset >
    			
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
            szCommand = L" DELETE FROM SensorRollAzimuthCalibration WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void CreatePolynomial::UpdateRollOffsetCalibrationData( CString &serialNumber, double a_0 )
{
	DeleteSensorRollOffsetCalibration remove;
	InsertIntoSensorRollOffsetCalibration insert;
	SensorRollOffsetCalibrationFromSN load;

	SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
	DAU::GetDAU().GetSensor( serialNumber )->LoadCalibration( load, serialNumber, data);

  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
 
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_STRING(insert, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
    SET_DB_VALUE(insert, a_0, a_0 + data.m_offset ) ;
		SET_DB_VALUE(insert, a_1, data.m_linear ) ;
		SET_DB_VALUE(insert, a_2, data.m_quadratic ) ;
		SET_DB_VALUE(insert, a_3, data.m_cubic ) ;

    hResult = ::OpenTable( insert ) ;

		if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

void CreatePolynomial::UpdatePitchOffsetCalibrationData( CString &serialNumber, double a_0 )
{
	DeleteSensorPitchOffsetCalibration remove;
	InsertIntoSensorPitchOffsetCalibration insert;
	SensorPitchOffsetCalibrationFromSN load;

	SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
	DAU::GetDAU().GetSensor( serialNumber )->LoadCalibration( load, serialNumber, data);

  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
 
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_STRING(insert, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
    SET_DB_VALUE(insert, a_0, a_0 + data.m_offset ) ;
		SET_DB_VALUE(insert, a_1, data.m_linear ) ;
		SET_DB_VALUE(insert, a_2, data.m_quadratic ) ;
		SET_DB_VALUE(insert, a_3, data.m_cubic ) ;

    hResult = ::OpenTable( insert ) ;

		if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

void CreatePolynomial::UpdateRollAzimuthCalibrationData( CString &serialNumber, double a_0 )
{
  DeleteSensorRollAzimuthCalibration remove;
	InsertIntoSensorRollAzimuthCalibration insert;
	SensorRollAzimuthCalibrationFromSN load;

	SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
	DAU::GetDAU().GetSensor( serialNumber )->LoadCalibration( load, serialNumber, data);

  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
 
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_STRING(insert, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
    SET_DB_VALUE(insert, a_0, a_0 + data.m_offset ) ;
		SET_DB_VALUE(insert, a_1, data.m_linear ) ;
		SET_DB_VALUE(insert, a_2, data.m_quadratic ) ;
		SET_DB_VALUE(insert, a_3, data.m_cubic ) ;

    hResult = ::OpenTable( insert ) ;

		if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

void CreatePolynomial::UpdatePitchAzimuthCalibrationData( CString &serialNumber, double a_0 )
{
	DeleteSensorPitchAzimuthCalibration remove;
	InsertIntoSensorPitchAzimuthCalibration insert;
	SensorPitchAzimuthCalibrationFromSN load;

	SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
	DAU::GetDAU().GetSensor( serialNumber )->LoadCalibration( load, serialNumber, data);

  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
 
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_STRING(insert, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
    SET_DB_VALUE(insert, a_0, a_0 + data.m_offset ) ;
		SET_DB_VALUE(insert, a_1, data.m_linear ) ;
		SET_DB_VALUE(insert, a_2, data.m_quadratic ) ;
		SET_DB_VALUE(insert, a_3, data.m_cubic ) ;

    hResult = ::OpenTable( insert ) ;

		if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
  }
}

template <class T, class S>
HRESULT UpdateData( T &insert, S &remove, CString &serialNumber, double a_0, double a_1, double a_2, double a_3 )
{
  SET_DB_STRING(remove, serialNumber, serialNumber) ;
  HRESULT hResult = ::OpenTable( remove ) ;
  if (S_OK == hResult)
  {
    remove.Close() ;
    SET_DB_STRING(insert, serialNumber, serialNumber) ;
    SET_DB_STRING(insert, operator, (LPCTSTR)SystemSetup::GetOperatorName()) ;
    SET_DB_VALUE(insert, a_0, a_0 ) ;
    SET_DB_VALUE(insert, a_1, a_1 ) ;
    SET_DB_VALUE(insert, a_2, a_2 ) ;
    SET_DB_VALUE(insert, a_3, a_3 ) ;
    hResult = ::OpenTable( insert ) ;
    if (S_OK != hResult)
    {
      Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
      ASSERT(0) ;
    }
    insert.Close() ;
  }
  else
  {
    Database::DisplayError( hResult, __FILE__, __LINE__ ) ;
    remove.Close() ;
    ASSERT(0) ;
    hResult = E_FAIL ;
  }
  return hResult ;
}

void CreatePolynomial::OnBnClickedSave( void )
{
  if (IDOK != ::AfxMessageBox( _T("This will generate a new calibration model!\nContinue?"), MB_ICONWARNING | MB_OKCANCEL ))
  {
    return ;
  }
  if (0 <= m_fit.GetOrder())
  {
    HRESULT hResult = S_OK ;
    CString serialNumber ;
    m_serial.GetWindowText( serialNumber ) ;
    switch (2 * m_type.GetCurSel() + m_axis.GetCurSel())
    {
    case 0:
        hResult = ::UpdateData( InsertIntoSensorRollOffsetCalibration(), DeleteSensorRollOffsetCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    case 1:
        hResult = ::UpdateData( InsertIntoSensorPitchOffsetCalibration(), DeleteSensorPitchOffsetCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    case 2:
        hResult = ::UpdateData( InsertIntoSensorRollGainCalibration(), DeleteSensorRollGainCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    case 3:
        hResult = ::UpdateData( InsertIntoSensorPitchGainCalibration(), DeleteSensorPitchGainCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    case 4:
        hResult = ::UpdateData( InsertIntoSensorRollAzimuthCalibration(), DeleteSensorRollAzimuthCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    case 5:
        hResult = ::UpdateData( InsertIntoSensorPitchAzimuthCalibration(), DeleteSensorPitchAzimuthCalibration(), serialNumber, m_fit.m_a_0, m_fit.m_a_1, m_fit.m_a_2, m_fit.m_a_3 ) ;
      break ;

    default:
      ASSERT(0) ;
      break ;
    }
    if (FAILED(hResult))
    {
      ::AfxMessageBox( _T("Unable to store model parameters.") ) ;
    }
    else
    {
      ::AfxMessageBox( _T("Succesfully stored model parameters.") ) ;
    }

  }
  else
  {
      ::AfxMessageBox( _T("Unable to calculate approximation model parameters.") ) ;
  }
}

void CreatePolynomial::OnBnClickedPrintGraph( void )
{
  try
  {
		CPageSetupDialog pageDialog ;    ;
		if (IDOK == pageDialog.DoModal())
		{
			AfxGetApp() -> SelectPrinter( pageDialog.m_psd.hDevNames, pageDialog.m_psd.hDevMode ) ;
      CRect margins ;
      pageDialog.GetMargins( margins, NULL ) ;
      CSize paperSize( pageDialog.GetPaperSize() ) ;
      paperSize.cx -= margins.left + margins.right ;
      paperSize.cy -= margins.top + margins.bottom ;
			HDC hPrintDC = pageDialog.CreatePrinterDC() ;
			if (NULL == hPrintDC)
			{
				// No printer
			}
			else
			{
				CDC printDC ;
				printDC.Attach( hPrintDC ) ;
        printDC.SetMapMode( MM_HIENGLISH ) ;
				if (0 > printDC.StartDoc( _T("Graph View") ))
				{
					// no Start
				}
				else
				{
					if (0 > printDC.StartPage())
					{
						// No page
					}
					else
					{
            CSize origin( margins.left, margins.top ) ;
            //if (PSD_INTHOUSANDTHSOFINCHES | pageDialog.m_psd.Flags)
            //{
            //  paperSize.cx = paperSize.cx * 2540 / 1000 ;
            //  paperSize.cy = paperSize.cy * 2540 / 1000 ;
            //  origin.cx = origin.cx * 2540 / 1000 ;
            //  origin.cy = origin.cy * 2540 / 1000 ;
            //}
            printDC.HIMETRICtoDP( &paperSize ) ;
            printDC.HIMETRICtoDP( &origin ) ;
            m_pGraph -> DrawToDC( (LONG)(printDC.m_hDC), oc2dFormatEnhMetafile, oc2dScaleToFit, origin.cx, origin.cy, paperSize.cx, paperSize.cy ) ;
						printDC.EndPage() ;
						printDC.EndDoc() ;
					}
				}
			}
		}
		else
		{
			// Check for no printer condition!
		}
  }
  catch ( _com_error e )
  {
    TRACE( "Error: %s\n", ( char * )e.Description() );
  }
}

BEGIN_EVENTSINK_MAP(CreatePolynomial, CDialog)
  ON_EVENT(CreatePolynomial, IDC_GRID, 26, BeforeColEdit, VTS_I2 VTS_I2 VTS_PI2)
  ON_EVENT(CreatePolynomial, IDC_GRID, 43, ClassicRead, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(CreatePolynomial, IDC_GRID, 44, ClassicWrite, VTS_PVARIANT VTS_I2 VTS_PVARIANT)
  ON_EVENT(CreatePolynomial, IDC_GRID, 42, UnboundGetRelativeBookmark, VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PI4)
  ON_EVENT(CreatePolynomial, IDC_GRID, 1, AfterColUpdate, VTS_I2)
END_EVENTSINK_MAP()

/**
 *  This event handler is called by the grid before a cell is edited and is
 *  implemented here to prevent editing of cells other than use column cells.
 */
void CreatePolynomial::BeforeColEdit( short index, short key, short *pCancel )
{
  (void)key ;
  if (index != 0)
  {
    *pCancel = -1 ;
  }
}

void CreatePolynomial::ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  USES_CONVERSION ;
  ASSERT(VT_I4 == pBookmark -> vt) ;
  switch (column)
  {
  case 0:
    pValue -> vt = VT_BOOL ;
    pValue -> boolVal = (bool)(m_selectedData [pBookmark -> lVal].m_use) ;
    break ;

  case 1:
    pValue -> vt = VT_R8 ;
    pValue -> dblVal = m_selectedData [pBookmark -> lVal].m_temperature ;
    break ;

  case 2:
    pValue -> vt = VT_R8 ;
    pValue -> dblVal = m_selectedData [pBookmark -> lVal].m_value ;
    break ;
  
  case 3:
    pValue -> vt = VT_DATE ;
    pValue -> date = ATL::COleDateTime(  m_selectedData [pBookmark -> lVal].m_time ) ;
    break ;

  case 4:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( m_selectedData [pBookmark -> lVal].m_operatorName).copy() ;
    break ;

  case 5:
    pValue -> vt = VT_BSTR ;
    pValue -> bstrVal = _bstr_t( m_selectedData [pBookmark -> lVal].m_dauSerialNumber).copy() ;
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
}

void CreatePolynomial::ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue )
{
  if (VT_I4 == pBookmark -> vt)
	{
		ASSERT(VT_BSTR == pValue -> vt) ;
		m_selectedData [pBookmark -> lVal].m_use = _variant_t(pValue) == _variant_t(L"0") ? 0 : -1 ;
		SetupPolynomial() ;
	}
}

void CreatePolynomial::SetNewLocation( VARIANT *pNewLocation, int location )
{
  if (location < 0 || location >= (int)(m_selectedData.size()) )
  {
    pNewLocation -> vt = VT_NULL ;
  }
  else
  {
    pNewLocation -> vt = VT_I4 ;
    pNewLocation -> lVal = location ;
  }
}

void CreatePolynomial::UnboundGetRelativeBookmark( VARIANT *pStartLocation, long offset, VARIANT *pNewLocation, long *pApproximatePosition )
{
  (void)pApproximatePosition ;
  switch (pStartLocation -> vt)
  {
  case VT_NULL:
    if (0 <= offset)
    {
      ASSERT(offset) ;
      SetNewLocation( pNewLocation, offset - 1 ) ;
    }
    else
    {
      SetNewLocation( pNewLocation, (int)(m_selectedData.size() + offset)) ;
    }
    break ;

  case VT_I4:
    SetNewLocation( pNewLocation, pStartLocation -> lVal + offset ) ;
    break ;

  default:
    ASSERT(0) ;
    pNewLocation -> vt = VT_NULL ;
    break ;
  }
  if (VT_I4 == pStartLocation -> vt)
  {
    *pApproximatePosition = pStartLocation -> lVal ;
  }
}

void CreatePolynomial::AfterColUpdate( short index )
{
  (void)index ;
  m_pGrid -> Update() ;
}

void CreatePolynomial::SortInTime( vector<SelectedData> &data )
{
  SelectedData tmpData;
  BOOL swap;

  for( unsigned int i=0; i<data.size()-1; i++ )
  {
    for( unsigned int j=i+1; j<data.size(); j++ )
    {
      swap = FALSE;
      if( (data[j].m_time.year > data[i].m_time.year ) )
      {
        swap = TRUE;
      }
      else if(  (data[j].m_time.year == data[i].m_time.year ) )
      {
        if( (data[j].m_time.month > data[i].m_time.month ) )
        {
          swap = TRUE;
        }
        else if(  (data[j].m_time.month == data[i].m_time.month ) )
        {
          if( (data[j].m_time.day > data[i].m_time.day ) )
          {
            swap = TRUE;
          }
          else if(  (data[j].m_time.day == data[i].m_time.day ) )
          {
            if( (data[j].m_time.hour > data[i].m_time.hour ) )
            {
              swap = TRUE;
            }
            else if(  (data[j].m_time.hour == data[i].m_time.hour ) )
            {
              if( (data[j].m_time.minute > data[i].m_time.minute ) )
              {
                swap = TRUE;
              }
              else if(  (data[j].m_time.minute == data[i].m_time.minute ) )
              {
                if( (data[j].m_time.second > data[i].m_time.second ) )
                {
                  swap = TRUE;
                }
                else if(  (data[j].m_time.second == data[i].m_time.second ) )
                {
                  if( (data[j].m_time.fraction > data[i].m_time.fraction ) )
                  {
                    swap = TRUE;
                  }
                  else if(  (data[j].m_time.fraction == data[i].m_time.fraction ) )
                  {
                    //nothing to do
                  }
                }
              }
            }
          }
        }
      }

      if( swap == TRUE )
      {
        tmpData = data[i];
        data[i] = data[j];
        data[j] = tmpData;
      }
    }
  }
}
