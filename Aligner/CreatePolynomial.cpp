// $Id: CreatePolynomial.cpp,v 1.5 2011/10/13 10:09:11 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
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
                DBInterface::Instance()->GetSensorData("SensorRollOffsetData", serialNumber, m_selectedData);
                //GetDataFromSN( SensorRollOffsetDataFromSN(), serialNumber, m_selectedData ) ;
            }
            break ;

            case 1:
            {
                DBInterface::Instance()->GetSensorData("SensorPitchOffsetData", serialNumber, m_selectedData);                
            }
            break ;

            case 2:
            {
                DBInterface::Instance()->GetSensorData("SensorRollGainData", serialNumber, m_selectedData);                                
            }
            break ;

            case 3:
            {
                DBInterface::Instance()->GetSensorData("SensorPitchGainData", serialNumber, m_selectedData);                                                
            }
            break ;

            case 4:
            {
                DBInterface::Instance()->GetSensorData("SensorRollAzimuthData", serialNumber, m_selectedData);                                                                
            }
            break ;

            case 5:
            {
                DBInterface::Instance()->GetSensorData("SensorPitchAzimuthData", serialNumber, m_selectedData);                                                                                
            }
            break ;

            default:
            ASSERT(0) ;
            break ;
        }

       // SortInTime( m_selectedData );
    }
}

void GetUniqueSerialNumbers( CString table, CComboBox &serial, CString current )
{
    vector<CString> serialNumbers;
    DBInterface::Instance()->GetUniqueSensorSN(table, serialNumbers);          
    
    vector<CString>::iterator iter;
    for(iter=serialNumbers.begin();iter!=serialNumbers.end();iter++)
    {
        CString text ;
        text.Format( "%03d", atoi(*iter) ) ;
        int index = serial.AddString( text ) ;
        if (text == current)
        {
            serial.SetCurSel( index ) ;
        }
    }
}

void CreatePolynomial::SetupSerial( void )
{
    CString current ;
    m_serial.GetWindowText( current ) ;
    switch (2 * m_type.GetCurSel() + m_axis.GetCurSel())
	{
        case 0: // TODO: Match this in a more elegant way
        {            
            GetUniqueSerialNumbers( "SensorRollOffsetData", m_serial, current ) ;                        
        }
		break ;

	    case 1:
        {
            GetUniqueSerialNumbers( "SensorPitchOffsetData", m_serial, current ) ;                            
        }
		break ;

	    case 2:
        {
            GetUniqueSerialNumbers( "SensorRollGainData", m_serial, current ) ;                                        
        }
		break ;

	    case 3:
        {
            GetUniqueSerialNumbers( "SensorPitchGainData", m_serial, current ) ;                                                                
        }
		break ;

	    case 4:
        {
            GetUniqueSerialNumbers( "SensorRollAzimuthData", m_serial, current ) ;             
        }
		break ;

	    case 5:
        {
            GetUniqueSerialNumbers( "SensorPitchAzimuthData", m_serial, current ) ;                         
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

BOOL CreatePolynomial::UpdateRollOffsetCalibrationData( CString &serialNumber, double a_0 )
{
	return DBInterface::Instance()->AddSensorCalibrationA0("SensorRollOffsetCalibration", serialNumber, SysSetup->GetOperatorName(), a_0) ;   
}

BOOL CreatePolynomial::UpdatePitchOffsetCalibrationData( CString &serialNumber, double a_0 )
{
	return DBInterface::Instance()->AddSensorCalibrationA0("SensorPitchOffsetCalibration", serialNumber, SysSetup->GetOperatorName(), a_0) ;
}

BOOL CreatePolynomial::UpdateRollAzimuthCalibrationData( CString &serialNumber, double a_0 )
{
    return DBInterface::Instance()->AddSensorCalibrationA0("SensorRollAzimuthCalibration", serialNumber, SysSetup->GetOperatorName(), a_0) ;    
}

BOOL CreatePolynomial::UpdatePitchAzimuthCalibrationData( CString &serialNumber, double a_0 )
{
	return DBInterface::Instance()->AddSensorCalibrationA0("SensorPitchAzimuthCalibration", serialNumber, SysSetup->GetOperatorName(), a_0) ;      
}

void CreatePolynomial::OnBnClickedSave( void )
{
    if (IDOK != ::AfxMessageBox( _T("This will generate a new calibration model!\nContinue?"), MB_ICONWARNING | MB_OKCANCEL ))
    {
        return ;
    }
    if (0 <= m_fit.GetOrder())
    {
        BOOL result = FALSE;
        CString serialNumber ;
        m_serial.GetWindowText( serialNumber ) ;
        switch (2 * m_type.GetCurSel() + m_axis.GetCurSel())
        {
        case 0:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorRollOffsetCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        case 1:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorPitchOffsetCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        case 2:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorRollGainCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        case 3:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorPitchGainCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        case 4:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorRollAzimuthCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        case 5:
            result = DBInterface::Instance()->UpdateSensorCalibration("SensorPitchAzimuthCalibration", serialNumber, SysSetup->GetOperatorName(), m_fit);        
        break ;

        default:
        ASSERT(0) ;
        break ;
        }
        if (result == FALSE)
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
