// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\gaincalibrationpage0.mrg.cpp
// compiler-generated file created 05/10/06 at 15:51:20
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//

// $Id: gaincalibrationpage0.mrg.cpp,v 1.1 2009/10/14 08:09:25 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002-2003

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "GainCalibrationPage0.h"
#include "CalibrationSheet.h"
#include "SensorDatabase.h"

IMPLEMENT_DYNAMIC(GainCalibrationPage0, CPropertyPage)

GainCalibrationPage0::GainCalibrationPage0( void ) : CPropertyPage(GainCalibrationPage0::IDD)
{
  // Empty
}

GainCalibrationPage0::~GainCalibrationPage0( void )
{
  // Empty
}

void GainCalibrationPage0::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control(pDX, IDC_REFERENCE_SERIAL_NUMBER, m_reference) ;
  DDX_Control(pDX, IDC_TABLE_SERIAL_NUMBER, m_table);
}

BEGIN_MESSAGE_MAP(GainCalibrationPage0, CPropertyPage)
  ON_CBN_SELCHANGE(IDC_TABLE_SERIAL_NUMBER, OnCbnSelchangeTableSerialNumber)
END_MESSAGE_MAP()

BOOL GainCalibrationPage0::InitReference( void )
{
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount()  ; i++)
  {
    Sensor *pCurrent = DAU::GetDAU().GetSensor( i ) ;
    if (Unit::Unused != pCurrent -> GetType())
    {
      int index = m_reference.AddString( pCurrent -> GetSerialNumber() ) ;
      if (0 > index || CB_ERR == m_reference.SetItemDataPtr( index, static_cast<void *>(pCurrent) ))
      {
        return FALSE ;
      }
    }
  }
  return TRUE ;
}

UNIQUE_SN(PlatformCorrection)      :---*/ _UniqueSNFromPlatformCorrectionAccessor { UNIQUE_SN_BODY 
	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 50 ".\\GainCalibrationPage0.cpp"

				DEFINE_COMMAND_EX(_UniqueSNFromPlatformCorrectionAccessor, L" SELECT DISTINCT serialNumber FROM PlatformCorrection ORDER BY serialNumber ")
			
#injected_line 50 ".\\GainCalibrationPage0.cpp"

						BEGIN_ACCESSOR_MAP(_UniqueSNFromPlatformCorrectionAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 50 ".\\GainCalibrationPage0.cpp"

class UniqueSNFromPlatformCorrection;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 50 ".\\GainCalibrationPage0.cpp"

class UniqueSNFromPlatformCorrection : public CCommand<CAccessor<_UniqueSNFromPlatformCorrectionAccessor> >
    			
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
            szCommand = L" SELECT DISTINCT serialNumber FROM PlatformCorrection ORDER BY serialNumber ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


BOOL GainCalibrationPage0::InitTable( void )
{
  UniqueSNFromPlatformCorrection table ;
  if (S_OK != ::OpenTable( table ))
  {
    return FALSE ;
  }
  HRESULT hResult = table.MoveFirst() ;
  while (S_OK == hResult)
  {
    m_table.AddString( table.m_serialNumber ) ;
    hResult = table.MoveNext() ;
  }
  switch (m_table.GetCount())
  {
  case 0:
    return FALSE ;

  case 1:
    m_table.SetCurSel( 0 ) ;
    m_table.EnableWindow( FALSE ) ;
    OnCbnSelchangeTableSerialNumber() ;
    break ;

  default:
    m_table.EnableWindow( TRUE ) ;
    break ;
  }
  return TRUE ;
}

BOOL GainCalibrationPage0::OnInitDialog( void )
{
  BOOL focus = __super::OnInitDialog() ;
  if (!InitReference())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  if (!InitTable())
  {
    // TODO: General Failure should handle this!
    ASSERT(0) ;
  }
  
  return focus ;
}

BOOL GainCalibrationPage0::OnSetActive( void )
{
  CPropertySheet *pSheet = static_cast<CPropertySheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CPropertySheet) )) ;
  pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;
  return __super::OnSetActive() ;
}

LRESULT GainCalibrationPage0::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  if (CB_ERR == m_reference.GetCurSel())
  {
    ::AfxMessageBox( IDS_SELECT_REFERENCE ) ;
    return -1 ;
  }
  if ( (pSheet -> m_alpha == 0) || ( CB_ERR == m_table.GetCurSel() ) )
  {
    ::AfxMessageBox( IDS_SELECT_TABLE ) ;
    return -1 ;
  }
  pSheet -> m_referenceData.m_pSource = static_cast<Sensor *>(m_reference.GetItemDataPtr( m_reference.GetCurSel() )) ;
  ASSERT(NULL != pSheet -> m_referenceData.m_pSource) ;
  return __super::OnWizardNext() ;
}

[
  db_command { SELECT alpha FROM PlatformCorrection WHERE serialNumber = ? }
] 
class /*+++ symbol was renamed:  PlatformCorrectionFromSN
 :---*/ _PlatformCorrectionFromSNAccessor {   public:
	DECL_DB_V(1, double, alpha) ;
  PARM_DB_L(1, TCHAR, serialNumber, 4) ;
  void GetRowsetProperties( CDBPropSet *pPropSet )
  {
    pPropSet -> AddProperty( DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL ) ;
    pPropSet -> AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE ) ;
  }

	//+++ Start Injected Code For Attribute 'db_command'
    #injected_line 128 ".\\GainCalibrationPage0.cpp"

				DEFINE_COMMAND_EX(_PlatformCorrectionFromSNAccessor, L" SELECT alpha FROM PlatformCorrection WHERE serialNumber = ? ")
			
#injected_line 128 ".\\GainCalibrationPage0.cpp"

						BEGIN_ACCESSOR_MAP(_PlatformCorrectionFromSNAccessor, 1)
							BEGIN_ACCESSOR(0, true)
							_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_alpha),  _SIZE_TYPE(m_alpha), 0, 0, offsetbuf(m_alpha), 0, offsetbuf(m_status_alpha))

							END_ACCESSOR()
						END_ACCESSOR_MAP()
					
#injected_line 128 ".\\GainCalibrationPage0.cpp"

					BEGIN_PARAM_MAP(_PlatformCorrectionFromSNAccessor)
						SET_PARAM_TYPE(DBPARAMIO_INPUT)

_COLUMN_ENTRY_CODE(1, _OLEDB_TYPE(m_serialNumber),  _SIZE_TYPE(m_serialNumber), 0, 0, offsetbuf(m_serialNumber), offsetbuf(m_length_serialNumber), offsetbuf(m_status_serialNumber))

					END_PARAM_MAP()
				
	//--- End Injected Code For Attribute 'db_command'
} ;

//+++ Start Injected Code For Attribute 'db_command'
#injected_line 128 ".\\GainCalibrationPage0.cpp"

class PlatformCorrectionFromSN;

//--- End Injected Code For Attribute 'db_command'


//+++ Start Injected Code For Attribute 'db_command'
#injected_line 128 ".\\GainCalibrationPage0.cpp"

class PlatformCorrectionFromSN : public CCommand<CAccessor<_PlatformCorrectionFromSNAccessor> >
    			
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
            szCommand = L" SELECT alpha FROM PlatformCorrection WHERE serialNumber = ? ";
        HRESULT hr = Open(session, szCommand,pPropSet);
#ifdef _DEBUG
        if(FAILED(hr))
            AtlTraceErrorRecords(hr);
#endif
        return hr;
    }

};

//--- End Injected Code For Attribute 'db_command'


void GainCalibrationPage0::OnCbnSelchangeTableSerialNumber( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  PlatformCorrectionFromSN table ;
  CString serialNumber ;
  m_table.GetWindowText( serialNumber ) ;
  if (0 < serialNumber.GetLength())
  {
    SET_DB_STRING(table, serialNumber, serialNumber) ;
    if (S_OK == ::OpenTable( table ))
    {
      if (S_OK == table.MoveFirst())
      {
        pSheet -> m_alpha = table.m_alpha / 1000 ;
        return ;
      }
    }
  }
  pSheet -> m_alpha = 0.0 ;
}
