// $Id: CreatePolynomial.h,v 1.2 2011/10/13 10:09:11 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "LeastSquare.h"
#include "GraphView.h"

class CreatePolynomial : public CDialog
{
    DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CreatePolynomial)
    DECLARE_EVENTSINK_MAP()
public:
	CreatePolynomial( CWnd* pParent = NULL ) ;
	virtual ~CreatePolynomial( void ) ;
	//template <class L, class I, class R>
	//void UpdateOffsetData( L &load, I &insert, R &remove, CString &serialNumber, double a_0 );
	BOOL UpdateRollOffsetCalibrationData( CString &serialNumber, double a_0 );
	BOOL UpdatePitchOffsetCalibrationData( CString &serialNumber, double a_0 );
	BOOL UpdateRollAzimuthCalibrationData( CString &serialNumber, double a_0 );
	BOOL UpdatePitchAzimuthCalibrationData( CString &serialNumber, double a_0 );

private:
	enum { IDD = IDD_CREATE_POLYNOMIAL };

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
	virtual BOOL OnInitDialog( void ) ;
	afx_msg void OnCbnSelchangeApproximationType( void ) ;
	afx_msg void OnCbnSelchangeApproximationOrder( void ) ;
  afx_msg void OnCbnSelchangeAxis( void ) ;
	afx_msg void OnCbnSelchangeSerialNumber( void ) ;
	afx_msg void OnBnClickedSave( void ) ;
  afx_msg void OnBnClickedPrintGraph( void ) ;
  void BeforeColEdit( short index, short key, short *pCancel ) ;
  void ClassicRead( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void ClassicWrite( VARIANT *pBookmark, short column, VARIANT *pValue ) ;
  void UnboundGetRelativeBookmark( VARIANT* StartLocation, long offset, VARIANT* NewLocation, long* ApproximatePosition ) ;
  void SetNewLocation( VARIANT *pNewLocation, int location ) ;
  void AfterColUpdate( short index ) ;
private:
  void InitGraph( void ) ;
  void ClearGraph( void ) ;
	void SetupSerial( void ) ;
	void SetupSerial( LPCWSTR pSqlCommand ) ;
  void SetupData( void ) ;
  void SetupPolynomial( void ) ;
  void SortInTime( vector<SelectedData> &data );
private:
  CComboBox m_type ;
	CComboBox m_order ;
	CComboBox m_axis ;
  CComboBox m_serial ;
  CWnd m_grid ;
  CWnd m_graph ;
//  TrueDBGrid70::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : commented
  TrueDBGridVer::ITrueDBGridCtrlPtr m_pGrid ;	//	stlo 090414 : replacing the above
  IChart2D_DualPtr m_pGraph ;
  IChartGroup_DualPtr m_pGroup ;
  IChartStyleCollection_DualPtr m_pStyleCollection ;
  IChart2DData_DualPtr m_pData ;
  vector<SelectedData> m_selectedData ;
  LeastSquare m_leastSquare ;
  Polynomial m_fit ;
  CString m_polynomialText ;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
} ;
