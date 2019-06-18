// $Id: DAUCalibrationPage0.cpp,v 1.3 2010/09/10 07:41:02 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "DAUCalibrationPage0.h"
#include "CalibrationSheet.h"

IMPLEMENT_DYNAMIC(DAUCalibrationPage0, CPropertyPage)

DAUCalibrationPage0::DAUCalibrationPage0( CWnd *pParent ) : CPropertyPage(DAUCalibrationPage0::IDD)
{
}

DAUCalibrationPage0::~DAUCalibrationPage0( void )
{
}

void DAUCalibrationPage0::DoDataExchange( CDataExchange *pDX )
{
  __super::DoDataExchange( pDX ) ;
  DDX_Control( pDX, IDC_AVAILABLE, m_availableControl ) ;
  DDX_Control( pDX, IDC_SELECTED, m_selectedControl ) ;
  DDX_Control( pDX, IDC_SELECT, m_selectControl ) ;
  DDX_Control( pDX, IDC_DESELECT, m_deselectControl ) ;
  DDX_Control( pDX, IDC_SELECT_ALL, m_selectAllControl ) ;
  DDX_Control( pDX, IDC_DESELECT_ALL, m_deselectAllControl ) ;
}

BEGIN_MESSAGE_MAP(DAUCalibrationPage0, CPropertyPage)
  ON_BN_CLICKED(IDC_SELECT, OnBnClickedSelect)
  ON_BN_CLICKED(IDC_DESELECT, OnBnClickedDeselect)
  ON_BN_CLICKED(IDC_SELECT_ALL, OnBnClickedSelectAll)
  ON_BN_CLICKED(IDC_DESELECT_ALL, OnBnClickedDeselectAll)
  ON_LBN_SELCHANGE(IDC_AVAILABLE, OnLbnSelchangeAvailable)
  ON_LBN_SELCHANGE(IDC_SELECTED, OnLbnSelchangeSelected)
END_MESSAGE_MAP()

static void TransferSelected( CListBox &from, CListBox &to )
{
  int index = from.GetCurSel() ;
  if (LB_ERR != index)
  {
    CString text ;
    from.GetText( index, text ) ;
    int data = from.GetItemData(index);
	from.DeleteString( index ) ;
    int toIndex = to.AddString( text ) ;
	to.SetItemData(toIndex, data);
  }
}

void DAUCalibrationPage0::OnBnClickedSelect( void )
{
  TransferSelected( m_availableControl, m_selectedControl ) ;
  HandleSelectionChange() ;
}

void DAUCalibrationPage0::OnBnClickedDeselect( void )
{
  TransferSelected( m_selectedControl, m_availableControl ) ;
  HandleSelectionChange() ;
}

static void TransferAll( CListBox &from, CListBox &to )
{
  int index ;
  while (( LB_ERR != (index = from.GetCount()) && 0 != index ))
  {
    CString text ;
    from.GetText( 0, text ) ;
    int data = from.GetItemData(0);
	from.DeleteString( 0 ) ;
    int toIndex = to.AddString( text ) ;
	to.SetItemData(toIndex, data); 
  }
}

void DAUCalibrationPage0::OnBnClickedSelectAll( void )
{
  TransferAll( m_availableControl, m_selectedControl ) ;
  HandleSelectionChange() ;
}

void DAUCalibrationPage0::OnBnClickedDeselectAll( void )
{
  TransferAll( m_selectedControl, m_availableControl ) ;
  HandleSelectionChange() ;
}

void DAUCalibrationPage0::OnLbnSelchangeAvailable( void )
{
  HandleSelectionChange() ;
}

void DAUCalibrationPage0::OnLbnSelchangeSelected( void )
{
  HandleSelectionChange() ;
}

BOOL DAUCalibrationPage0::OnInitDialog( void )
{
  BOOL selected = __super::OnInitDialog() ;
  for (int i = 0 ; i < DAU::GetDAU().GetSensorCount() ; i++)
  {
    int index = m_availableControl.AddString( DAU::GetDAU().GetSensor( i ) -> GetName()) ;
	m_availableControl.SetItemData(index, i);
  }
  HandleSelectionChange() ;
  return selected ;
}

LRESULT DAUCalibrationPage0::OnWizardNext( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  pSheet->m_sensorData.clear();
  for (int i = 0 ; i < m_selectedControl.GetCount() ; i++)
  {
    CString channelName ;
    m_selectedControl.GetText( i, channelName ) ;
    int index = m_selectedControl.GetItemData(i);
	pSheet->m_sensorData.push_back( CalibrationData( DAU::GetDAU().GetSensor( channelName ), index ) ) ;
	DAU::GetDAU().GetSensor(channelName)->SetOverrangeDetectionActive(TRUE);
  }
  return __super::OnWizardNext() ;
}

void DAUCalibrationPage0::HandleSelectionChange( void )
{
  CalibrationSheet *pSheet = static_cast<CalibrationSheet *>(GetParent()) ;
  ASSERT(pSheet -> IsKindOf( RUNTIME_CLASS(CalibrationSheet) )) ;
  int count ;
  m_selectAllControl.EnableWindow( !(( LB_ERR == (count = m_availableControl.GetCount()) || 0 == count )) ) ;
  if (( LB_ERR == (count = m_selectedControl.GetCount()) || 0 == count ))
  {
    m_deselectAllControl.EnableWindow( FALSE ) ;
    pSheet -> SetWizardButtons( 0 ) ;
  }
  else
  {
    m_deselectAllControl.EnableWindow( TRUE ) ;
    pSheet -> SetWizardButtons( PSWIZB_NEXT ) ;
  }
  m_selectControl.EnableWindow( LB_ERR != m_availableControl.GetCurSel() ) ;
  m_deselectControl.EnableWindow( LB_ERR != m_selectedControl.GetCurSel() ) ;
}
