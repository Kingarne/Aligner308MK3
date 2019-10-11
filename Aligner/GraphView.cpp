// $Id: GraphView.cpp,v 1.22 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Util.h"
#include "ASound.h"
#include "oc_color.h"
#include "SystemSetup.h"
#include "GraphView.h"
#include "UserMessages.h"
#include "SensorAlignmentPage.h"
#include "ResultTable.h"
#include "Defines.h"
#include "SetupLiveDataGraphDlg.h"
#include <algorithm>
#include <GdiPlus.h>
#include "Type.h"

using namespace Gdiplus;

#define FILE_COL_WIDTH			15
#define EOL_FILE		_T("\r\n")

CString CGraphView::m_GraphFileName = (CString)DEFAULT_LIVE_DATA_FILE_NAME + (CString)DEFAULT_LIVE_DATA_GRAPH_FILE_EXT;
CString CGraphView::m_GraphFileDir = DEFAULT_FILE_DIR ;

IMPLEMENT_DYNCREATE( CGraphView, CFormView )

#define INTENS 220
#define ColorRed            RGB(INTENS, 0, 0)
#define ColorGreen          RGB(0, INTENS, 0)
#define ColorBlue           RGB(0, 0, INTENS)
#define ColorLightRed       RGB(255, 127, 127)
#define ColorLightGreen     RGB(127, 255, 127)
#define ColorLightBlue      RGB(127, 127, 255)

#define NUM_COLORS 12
OLE_COLOR m_defaultColors[NUM_COLORS] =
{
        ocColorBlack,
        ColorRed,    
        ColorGreen,
        ColorBlue,
        ocColorCyan,        
        ocColorYellow,        
        ocColorMagenta,        
        ColorLightRed,        
        ColorLightBlue,        
        ocColorBrown,
        ocColorGray50,
        ColorLightGreen
};           
//ocColorGray50
CGraphView::CGraphView( void ) : CFormView( CGraphView::IDD ), DataRecepient( this )
{
  m_firstUpdate = TRUE ;
  theApp.SetLiveGraphViewActivated( FALSE );
  m_NoOfSeriesY2 = 1;
  m_AxisY2Enabled = FALSE;
  m_AxisY2Min = 0.0;
  m_AxisY2Max = 0.0;
  m_AxisY2MinAutoScaleOn = TRUE;
  m_AxisY2MaxAutoScaleOn = TRUE;
  m_AxisY2Unit = _T("");

  m_NoOfSeries = 1;
  m_UseCommonXValues = TRUE;
  m_GraphType = oc2dTypePlot;
  m_OriginBase = 0;
  m_AxesReversed = FALSE;
  m_AxisXMin = 0.0;
  m_AxisXMax = 1.0;
  m_AxisYMin = 0.0;
  m_AxisYMax = 1.0;
  m_AxisXMinAutoScaleOn = TRUE;
  m_AxisXMaxAutoScaleOn = TRUE;
  m_AxisYMinAutoScaleOn = TRUE;
  m_AxisYMaxAutoScaleOn = TRUE;
  m_AxisXUnit = _T("");
  m_AxisYUnit = _T("");
  m_PolarRange0ToPlusMinus180 = TRUE;
  m_ControlBorder = oc2dBorderPlain;
  m_LegendBorder = oc2dBorderPlain;
  m_ShowLegend = TRUE;
  m_ChartBorder = oc2dBorderPlain;
  m_ChartHeader = _T("");
  m_ChartFooter = _T("");
  m_ControlWidth = 0;
  m_ControlHeight = 0;

    m_graphData.resize(MAX_NO_OF_SERIES+1);
    for( int i=0; i<=MAX_NO_OF_SERIES; i++ )
    {
        m_graphData[i].x = 0.0;
        m_graphData[i].y = 0.0;
        m_PointShapes[i] = oc2dShapeNone;
        m_PointSizes[i] = 7;
        m_PointColors[i] = GetColor( i );
        m_LinePatterns[i] = GetLinePattern( i );
        m_LineColors[i] = GetColor( i );
      
    }
	m_ClusterWidthForOneBar = 5;
	m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
	m_ReductionFactorForGraphData = 1;
	m_ShowLiveDataGraph = FALSE;
	m_LiveDataCounter = 0;
	m_LiveDataCounterStopValue = 0;
	m_StoreLiveDataToFile = FALSE;
	m_ShowBarGraph = FALSE;

    m_dataTypeText[DATA_TYPE_ROLL] = "Roll";
    m_dataTypeText[DATA_TYPE_PITCH] = "Pitch";
    m_dataTypeText[DATA_TYPE_HEADING] = "Heading";
    
}

CGraphView::~CGraphView( void )
{
}

void CGraphView::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange( pDX );
  DDX_Control( pDX, IDC_CHART_CONTROL, m_chart );
  m_pChart = m_chart.GetControlUnknown();
}

BEGIN_MESSAGE_MAP( CGraphView, CFormView )
  ON_BN_CLICKED( IDC_LIVE_GRAPH_QUIT, OnBnClickedLiveGraphQuit )
  ON_BN_CLICKED( IDC_LIVE_GRAPH_PRINT, OnBnClickedLiveGraphPrint )
  ON_BN_CLICKED( IDC_LIVE_GRAPH_SAVE, OnBnClickedLiveGraphSave )
  ON_MESSAGE(UM_NEW_DATA, OnNewData)
  ON_MESSAGE(UM_DAU_ERROR, OnDauError)
  ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LIVE_GRAPH_STOP, OnBnClickedLiveGraphStop)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CGraphView::AssertValid( void ) const
{
	__super::AssertValid();
}

void CGraphView::Dump( CDumpContext &dc ) const
{
	__super::Dump( dc );
}
#endif //_DEBUG

void CGraphView::ClearLiveGraph()
{
  try
  {
    for( int i=1; i<=m_NoOfSeries; i++ )
    {
      m_pData->PutNumPoints( i, 0 );
    }
    m_pChart->GetHeader()->GetText()->PutText( _T("") );
    m_pChart->GetFooter()->GetText()->PutText( _T("") );
    m_pChart->GetChartLabels()->RemoveAll();
    m_pGroup->GetSeriesLabels()->RemoveAll();

    if( IsAxisY2Enabled() == TRUE )
    {
      for( int i=1; i<=m_NoOfSeriesY2; i++ )
      {
        m_pDataY2->PutNumPoints( i, 0 );
      }
      m_pGroupY2->GetSeriesLabels()->RemoveAll();
    }
  }
  catch ( _com_error e )
  {
    TRACE( "Error: %s\n", ( char * )e.Description() );
  }
}
//PSD_INHUNDREDTHSOFMILLIMETERS

void CGraphView::PrintLiveGraph()
{
    try
    {
		CPageSetupDialog pageDialog ;
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
            m_pChart -> DrawToDC( (LONG)(printDC.m_hDC), oc2dFormatEnhMetafile, oc2dScaleToFit, origin.cx, origin.cy, paperSize.cx, paperSize.cy ) ;
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

OLE_COLOR CGraphView::GetColor( int i )
{
  if( i < 0 )
  {
    i = 0;
  }

  return( m_defaultColors[i%NUM_COLORS] );
}

ShapeConstants CGraphView::GetPointShape( int i )
{
  if( i < 0 )
  {
    i = 0;
  }

  while( i >= NO_OF_POINT_SHAPES )
  {
    i -= NO_OF_POINT_SHAPES;
  }
  return( defaultPointShapes[i] );
}

LinePatternConstants CGraphView::GetLinePattern( int i )
{
  if( i < 0 )
  {
    i = 0;
  }

  while( i >= NO_OF_LINE_PATTERNS )
  {
    i -= NO_OF_LINE_PATTERNS;
  }
  return( defaultLinePatterns[i] );
}

BOOL CGraphView::SetGraphType( enum ChartTypeConstants graphType, BOOL update )
{
  m_GraphType = graphType;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetUseCommonXValues( BOOL useCommonXValues, BOOL update )
{
  m_UseCommonXValues = useCommonXValues;
  return update ? UpdateGraph() : TRUE ;

}

BOOL CGraphView::SetNoOfSeries( int noOfSeries, BOOL update )
{
  if( ( noOfSeries >= MIN_NO_OF_SERIES ) && ( noOfSeries <= MAX_NO_OF_SERIES ) )
  {
    m_NoOfSeries = noOfSeries;
    return update ? UpdateGraph() : TRUE ;
  }
  else
  {
    return( FALSE );
  }
}

int CGraphView::GetNoOfSeries( void )
{
  if( ( m_NoOfSeries >= MIN_NO_OF_SERIES ) && ( m_NoOfSeries <= MAX_NO_OF_SERIES ) )
  {
    return( m_NoOfSeries );
  }
  else
  { // should never happen
    m_NoOfSeries = MIN_NO_OF_SERIES;
    UpdateGraph();
    return( MIN_NO_OF_SERIES );
  }
}

BOOL CGraphView::SetOriginBase( double originBase, BOOL update )
{
  m_OriginBase = originBase;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::ReverseAxes( BOOL reverseAxes, BOOL update )
{
  m_AxesReversed = reverseAxes;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisXMin( double axisXMin, BOOL update )
{
  m_AxisXMin = axisXMin;
  m_AxisXMinAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisXMax( double axisXMax, BOOL update )
{
  m_AxisXMax = axisXMax;
  m_AxisXMaxAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}
BOOL CGraphView::SetAxisYMin( double axisYMin, BOOL update )
{
  m_AxisYMin = axisYMin;
  m_AxisYMinAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}
BOOL CGraphView::SetAxisYMax( double axisYMax, BOOL update )
{
  m_AxisYMax = axisYMax;
  m_AxisYMaxAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisY2Min( double axisY2Min, BOOL update )
{
  m_AxisY2Min = axisY2Min;
  m_AxisY2MinAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}
BOOL CGraphView::SetAxisY2Max( double axisY2Max, BOOL update )
{
  m_AxisY2Max = axisY2Max;
  m_AxisY2MaxAutoScaleOn = FALSE;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisXMinAutoScaleOn( BOOL axisXMinAutoScaleOn, BOOL update )
{
  m_AxisXMinAutoScaleOn = axisXMinAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisXMaxAutoScaleOn( BOOL axisXMaxAutoScaleOn, BOOL update )
{
  m_AxisXMaxAutoScaleOn = axisXMaxAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisYMinAutoScaleOn( BOOL axisYMinAutoScaleOn, BOOL update )
{
  m_AxisYMinAutoScaleOn = axisYMinAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisYMaxAutoScaleOn( BOOL axisYMaxAutoScaleOn, BOOL update )
{
  m_AxisYMaxAutoScaleOn = axisYMaxAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisY2MinAutoScaleOn( BOOL axisY2MinAutoScaleOn, BOOL update )
{
  m_AxisY2MinAutoScaleOn = axisY2MinAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisY2MaxAutoScaleOn( BOOL axisY2MaxAutoScaleOn, BOOL update )
{
  m_AxisY2MaxAutoScaleOn = axisY2MaxAutoScaleOn;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisXUnit( CString* axisXUnitType, BOOL update )
{
  m_AxisXUnit = *axisXUnitType;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisYUnit( CString* axisYUnitType, BOOL update )
{
  m_AxisYUnit = *axisYUnitType;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetAxisY2Unit( CString* axisY2UnitType, BOOL update )
{
  m_AxisY2Unit = *axisY2UnitType;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::IsAxisY2Enabled( void ) const 
{
  return( m_AxisY2Enabled );
}

void CGraphView::SetAxisY2Enabled( BOOL axisY2Enabled )
{
  m_AxisY2Enabled = axisY2Enabled;
}

BOOL CGraphView::SetPolarRange0ToPlusMinus180( BOOL polarRange0ToPlusMinus180, BOOL update )
{
  m_PolarRange0ToPlusMinus180 = polarRange0ToPlusMinus180;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetControlBorder( enum BorderTypeConstants controlBorder, BOOL update )
{
  m_ControlBorder = controlBorder;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetLegendBorder( enum BorderTypeConstants legendBorder, BOOL update )
{
  m_LegendBorder = legendBorder;
  return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::ShowLegend( BOOL showLegend, BOOL update )
{
    m_ShowLegend = showLegend;
    return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetChartBorder( enum BorderTypeConstants chartBorder, BOOL update )
{
    m_ChartBorder = chartBorder;
    return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetChartHeader( CString chartHeader, BOOL update )
{
    m_ChartHeader = chartHeader;
    return update ? UpdateGraph() : TRUE ;
}

BOOL CGraphView::SetChartFooter( CString chartFooter, BOOL update )
{
    m_ChartFooter = chartFooter;
    return update ? UpdateGraph() : TRUE ;
}

CString CGraphView::GetChartFooter( void )
{
    return m_ChartFooter;
}

long CGraphView::GetControlWidth( void )
{
    return( m_ControlWidth ) ;
}

long CGraphView::GetControlHeight( void )
{
    return( m_ControlHeight );
}

BOOL CGraphView::SetXYData( int serie, double xData, double yData )
{
    if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
    {
        if( theApp.IsAligner308Enabled())
        {
		    if( GetMRad() == FALSE )
		    {
			    yData = MRADIANS_TO_ARCMIN( yData );
		    }
        }
        m_graphData[serie].x = xData;
        m_graphData[serie].y = yData;
        m_graphData[serie].valid = TRUE;
        
	    return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

BOOL CGraphView::SetLiveXYData( int serie, double xData, double yData, BOOL update )
{
    if( yData > m_graphData[serie].m_maxLiveGraphValue )
    {
        m_graphData[serie].m_maxLiveGraphValue = yData;
    }
    if( yData < m_graphData[serie].m_minLiveGraphValue )
    {
        m_graphData[serie].m_minLiveGraphValue = yData;
    }
    
    if( update == TRUE )
    {
        if( fabs( (m_graphData[serie].m_previousLiveGraphValue - m_graphData[serie].m_maxLiveGraphValue) ) > fabs( (m_graphData[serie].m_previousLiveGraphValue - m_graphData[serie].m_minLiveGraphValue) ) )
        {
            yData = m_graphData[serie].m_maxLiveGraphValue;
        }
        else
        {
            yData = m_graphData[serie].m_minLiveGraphValue;
        }
   
        if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
        {
            if( theApp.IsAligner308Enabled())
            {
                if( GetMRad() == FALSE )
                {
                    yData = MRADIANS_TO_ARCMIN( yData );
                }
            }
            m_graphData[serie].x = xData;
            m_graphData[serie].y = yData;
            m_graphData[serie].valid = TRUE;

            m_graphData[serie].m_maxLiveGraphValue = MAX_LIVE_GRAPH_RESET_VALUE;
            m_graphData[serie].m_minLiveGraphValue = MIN_LIVE_GRAPH_RESET_VALUE;
            m_graphData[serie].m_previousLiveGraphValue = yData;

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    
    }
    return TRUE;
}

BOOL CGraphView::SetPointShape( int serie, ShapeConstants pointShape, BOOL update )
{
    if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
    { 
        m_PointShapes[serie] = pointShape;
        return update ? UpdateGraph() : TRUE ;
    }
    else
    {
        return( FALSE );
    }
}

BOOL CGraphView::SetPointSize( int serie, int pointSize, BOOL update )
{
    if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
    {
        m_PointSizes[serie] = pointSize;
        return update ? UpdateGraph() : TRUE ;
    }
    else
    {
        return( FALSE );
    }
}

BOOL CGraphView::SetPointColor( int serie, OLE_COLOR pointColor, BOOL update )
{
  if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
  {
    m_PointColors[serie] = pointColor;
    return update ? UpdateGraph() : TRUE ;
  }
  else
  {
    return( FALSE );
  }
}

BOOL CGraphView::SetLinePattern( int serie, LinePatternConstants linePattern, BOOL update )
{
  if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
  {
    m_LinePatterns[serie] = linePattern;
    return update ? UpdateGraph() : TRUE ;
  }
  else
  {
    return( FALSE );
  }
}

BOOL CGraphView::SetLineColor( int serie, OLE_COLOR lineColor, BOOL update )
{
  if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
  {
    m_LineColors[serie] = lineColor;
    return update ? UpdateGraph() : TRUE ;
  }
  else
  {
    return( FALSE );
  }
}

struct SortAxis:public std::binary_function<int,int,bool> 
{     
    SortAxis(){}
    bool operator()(GraphData& p1, GraphData& p2){
        return p1.axis<p2.axis;
    } 
}; 

void CGraphView::SortGraphs()
{
    //Make sure that all Y1 comes before Y2
    vector<GraphData>::iterator iter; 
    for(iter = m_graphData.begin();iter!=m_graphData.end();iter++)
    {
        TRACE("label:%s, axis:%d\n",iter->label, iter->axis);
    }

    std::sort(m_graphData.begin(), m_graphData.end(), SortAxis());

    int serieY1=1, serieY2=1;
    for( int gr=1; gr<=m_totalNoSeries; gr++ )
    {
        if(m_graphData[gr].axis == Axis_Y1)
        {
            m_graphData[gr].graphNum = serieY1++;          
        }
        else if(m_graphData[gr].axis == Axis_Y2)
        {
            m_graphData[gr].graphNum = serieY2++;          
        }
    }

    for(iter = m_graphData.begin();iter!=m_graphData.end();iter++)
    {
        TRACE("label:%s, axis:%d, num:%d\n",iter->label, iter->axis, iter->graphNum);
    }



    TRACE("done\n");
}

BOOL CGraphView::UpdateGraphLabels()
{
    int result = TRUE;
    try
    {
        int i;
        m_pGroup->GetSeriesLabels()->RemoveAll();
        m_pGroupY2->GetSeriesLabels()->RemoveAll();

        for( i=1; i<=m_totalNoSeries; i++ ) 
        {
     //       TRACE("**Add series %s, axis:%d\n",m_graphData[i].label, m_graphData[i].axis);
            if(m_graphData[i].axis == Axis_Y1)
            {
                m_pGroup->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
            }
            else if(m_graphData[i].axis == Axis_Y2)
            {
                m_pGroupY2->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
            }
        }
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }
    return result;
}

BOOL CGraphView::AddGraph( int serie, CString serieLabel, int id, int axis, int type )
{
    BOOL result = TRUE;

    if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
    {
        m_graphData[serie].id = id;
        m_graphData[serie].graphNum = serie;
        m_graphData[serie].label = serieLabel;
        m_graphData[serie].axis = axis;
        m_graphData[serie].dataType = type;

        return( result );
    }
    else
    {
        return( FALSE );
    }
}


BOOL CGraphView::SetSerieLabel( int serie, CString serieLabel, int axis)
{
    BOOL result = TRUE;

    if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
    {
        m_graphData[serie].label = serieLabel;
        m_graphData[serie].axis = axis;
        try
        {
            int i;
            m_pGroup->GetSeriesLabels()->RemoveAll();
            m_pGroupY2->GetSeriesLabels()->RemoveAll();
//             for( i=1; i<=m_totalNoSeries; i++ )
//             {
//                 if(m_graphData[i].axis == Axis_Y1)
//                 {
//                     m_pGroup->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
//                 }
//             }
//             for( i=1; i<=m_totalNoSeries; i++ )
//             {
//                 if(m_graphData[i].axis == Axis_Y2)
//                 {
//                     m_pGroupY2->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
//                 }
//             }

            for( i=1; i<=m_totalNoSeries; i++ )
            {
                TRACE("**Add series %s, axis:%d\n",m_graphData[i].label, m_graphData[i].axis);
                if(m_graphData[i].axis == Axis_Y1)
                {
                    m_pGroup->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
                }
                else if(m_graphData[i].axis == Axis_Y2)
                {
                    m_pGroupY2->GetSeriesLabels()->Add( m_graphData[i].label.AllocSysString() );
                }
            }


//             for( i=1; i<=m_NoOfSeries; i++ )
//             {
//                 
//             }
// 
//             if( IsAxisY2Enabled() == TRUE )
//             {
//                 m_pGroupY2->GetSeriesLabels()->RemoveAll();
//                 for( ; i<=m_NoOfSeries + m_NoOfSeriesY2; i++ )
//                 {
//                     
//                 }
//             }
        }
        catch ( _com_error e )
        {
            result = FALSE;
        }
        return( result );
    }
    else
    {
        return( FALSE );
    }
}

//long CGraphView::GetNoOfPoints( int serie )
//{  
//  BOOL result = TRUE;
//  long noOfPoints = 0;
//
//  if( ( serie >= MIN_NO_OF_SERIES ) && ( serie <= MAX_NO_OF_SERIES ) )
//  {  
//    try
//    {
//      noOfPoints = m_pData->GetNumPoints( serie );
//    }
//    catch ( _com_error e )
//    {
//      result = FALSE;
//    }
//    if( result == TRUE )
//    {
//      return( noOfPoints );
//    }
//  }
//
//  return( 0 );
//}

BOOL CGraphView::ReSetAxisXUnitAndMax( CString* axisXUnitType, double axisXMax )
{
  SetAxisXUnit( axisXUnitType, FALSE );
  SetAxisXMax( axisXMax, FALSE );
  BOOL result = TRUE;

  try
  {
    m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMax()->PutValue( m_AxisXMax );
    m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetTitle()->PutText( m_AxisXUnit.AllocSysString() );
    //throw( _com_error( (HRESULT)0 ) ) ;
  }
  catch ( _com_error e )
  {
    result = FALSE;
  }
  return( result );
}

void CGraphView::SetAxisXUnitAndMax( double axisXMax )
{
  CString str;

  if( axisXMax > SHOW_TIME_AXIS_IN_HOURS )
  {
    m_TimeUnit = SHOW_TIME_AXIS_IN_HOURS;
    str.LoadString( IDS_HRS_UNIT );
    ReSetAxisXUnitAndMax( &str, SECONDS_TO_HOURS( axisXMax ) );
  }
  else if( axisXMax > SHOW_TIME_AXIS_IN_MINUTES )
  {
    m_TimeUnit = SHOW_TIME_AXIS_IN_MINUTES;
    str.LoadString( IDS_MIN_UNIT );
    ReSetAxisXUnitAndMax( &str, SECONDS_TO_MINUTES( axisXMax ) );
  }
  else
  {
    m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
    str.LoadString( IDS_S_UNIT );
    ReSetAxisXUnitAndMax( &str, axisXMax );
  }
}

double CGraphView::GetTimeValue( long sampleCounter, double sampleTime )
{
	double timeValue;
	timeValue = sampleCounter * sampleTime;

	switch( m_TimeUnit )
	{
	case SHOW_TIME_AXIS_IN_HOURS:
		timeValue = SECONDS_TO_HOURS( timeValue );
		break;
	case SHOW_TIME_AXIS_IN_MINUTES:
		timeValue = SECONDS_TO_MINUTES( timeValue );
		break;
	case SHOW_TIME_AXIS_IN_SECONDS:
	default:
		break;
	}

	return( timeValue );
}

void CGraphView::SetReductionFactorForGraphData( long totalNumberOfSamples, long reducedNumberOfSamples )
{
	if( reducedNumberOfSamples <= 0 )
	{
		m_ReductionFactorForGraphData = 1;
		return;
	}
	m_ReductionFactorForGraphData = (int)( totalNumberOfSamples / reducedNumberOfSamples );

    if( (( totalNumberOfSamples % reducedNumberOfSamples ) != 0) || (m_ReductionFactorForGraphData == 0) )
    {
        m_ReductionFactorForGraphData++;
    }
}

BOOL CGraphView::TimeToUpdate( long sampleCount )
{
	if( ( ( sampleCount - 1 ) % m_ReductionFactorForGraphData ) == 0 )
	{
		return( TRUE );
	}
	return( FALSE );
}

BOOL CGraphView::InitDefaultPlotGraph( int noOfSeries )
{
    theApp.SetLiveGraphViewActivated( FALSE );
    m_NoOfSeriesY2 = 0;
    m_AxisY2Enabled = FALSE;
    m_AxisY2Min = 0.0;
    m_AxisY2Max = 0.0;
    m_AxisY2MinAutoScaleOn = TRUE;
    m_AxisY2MaxAutoScaleOn = TRUE;
    m_AxisY2Unit = _T("");

    m_pChart->PutAllowUserChanges( FALSE );
    CleanUp();
    m_UseCommonXValues = TRUE;
    m_NoOfSeries = noOfSeries;
    m_totalNoSeries = noOfSeries;
    m_GraphType = oc2dTypePlot;
    m_OriginBase = 0;
    m_AxesReversed = FALSE;
    m_AxisXMin = 0.0;
    m_AxisXMinAutoScaleOn = FALSE;
    m_AxisXMaxAutoScaleOn = TRUE;
    m_AxisYMinAutoScaleOn = TRUE;
    m_AxisYMaxAutoScaleOn = TRUE;
    m_AxisXUnit = _T("");
    m_AxisYUnit = _T("");
    m_ControlBorder = oc2dBorderPlain;
    m_LegendBorder = oc2dBorderNone;
    m_ShowLegend = TRUE;
    m_ChartBorder = oc2dBorderPlain;
    m_ChartHeader = _T("");
    m_ChartFooter = _T("");
    m_pChart->GetFooter()->put_Adjust( oc2dAdjustCenter );

    m_graphData.resize(MAX_NO_OF_SERIES+1);
    for( int i=1; i<=MAX_NO_OF_SERIES; i++ )
    {    
        m_graphData[i].x = 0.0;
        m_graphData[i].y = 0.0;
        m_graphData[i].valid = FALSE;

        m_PointShapes[i] = oc2dShapeNone;
        m_PointSizes[i] = 7;
        m_PointColors[i] = GetColor( i );
        m_LinePatterns[i] = GetLinePattern( i );
        m_LineColors[i] = GetColor( i );        
    }
	m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
	m_ReductionFactorForGraphData = 1;
	m_ShowLiveDataGraph = FALSE;
	m_ShowBarGraph = FALSE;

    return( UpdateGraph() );
}

BOOL CGraphView::InitCalibrationGraph( int noOfSeries, int innoOfSecondSeries )
{
	InitDefaultPlotGraph( noOfSeries );
    if ( 0 < (m_NoOfSeriesY2 = innoOfSecondSeries))
    {
        m_AxisY2Enabled = TRUE ;
    }
    BOOL result = TRUE;
	long noOfRowsInFooter = 5; //there must be space for the x-axis UnitType above the footer  
	long noOfCharactersInLegend = 25;
	long legendLeftPos = m_ControlWidth - noOfCharactersInLegend * MAX_WIDTH_OF_CHARACTER;
	long legendTopPos = ( m_ControlHeight - (noOfSeries * ROW_HEIGHT) ) / 2 ;
    	
    try
    {
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos );
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
        m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );

		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    return( result );
}

BOOL CGraphView::InitDefaultBarGraph( int noOfSeries, BOOL useRoll )
{
    m_barGraphUseRoll = useRoll;
	theApp.SetLiveGraphViewActivated( FALSE );
    m_NoOfSeriesY2 = 0;
    m_AxisY2Enabled = FALSE;
    m_AxisY2Min = 0.0;
    m_AxisY2Max = 0.0;
    m_AxisY2MinAutoScaleOn = TRUE;
    m_AxisY2MaxAutoScaleOn = TRUE;
    m_AxisY2Unit = _T("");

    m_pChart->PutAllowUserChanges( FALSE );
    CleanUp();
    m_UseCommonXValues = TRUE; // must be TRUE for oc2dTypeBar
    m_NoOfSeries = noOfSeries;
    m_totalNoSeries = noOfSeries;
    m_GraphType = oc2dTypeBar;
    m_OriginBase = 0;
    m_AxesReversed = FALSE;
    m_AxisXMin = 0.0;
    m_AxisXMax = 1.0;
	m_AxisYMin = 0.0;
	m_AxisYMax = 0.0;
    m_AxisXMinAutoScaleOn = FALSE;
    m_AxisXMaxAutoScaleOn = FALSE;
    m_AxisYMinAutoScaleOn = TRUE;
    m_AxisYMaxAutoScaleOn = TRUE;
    m_AxisXUnit = _T("");
    m_AxisYUnit = _T("");
    m_PolarRange0ToPlusMinus180 = TRUE;
    m_ControlBorder = oc2dBorderPlain;
    m_LegendBorder = oc2dBorderNone;
    m_ShowLegend = TRUE;
    m_ChartBorder = oc2dBorderPlain;
    m_ChartHeader = _T("");
    m_ChartFooter = _T("");
    m_pChart->GetFooter()->put_Adjust( oc2dAdjustCenter );

    m_graphData.resize(MAX_NO_OF_SERIES+1);
    for( int i=1; i<=MAX_NO_OF_SERIES; i++ )
    {
        m_graphData[i].x = 0.0;
        m_graphData[i].y = 0.0;
        m_graphData[i].valid = FALSE;
        m_PointShapes[i] = GetPointShape( i );
        m_PointSizes[i] = 7;
        m_PointColors[i] = GetColor( i );
        m_LinePatterns[i] = oc2dLineNone;
        m_LineColors[i] = GetColor( i );
    }
    m_ClusterWidthForOneBar = 5;
	m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
	m_ReductionFactorForGraphData = 1;
	m_ShowLiveDataGraph = FALSE;
	m_ShowBarGraph = TRUE;
  
    return( UpdateGraph() );
}

BOOL CGraphView::InitDefaultPolarGraph( int noOfSeries )
{
    theApp.SetLiveGraphViewActivated( FALSE );
    m_NoOfSeriesY2 = 0;
    m_AxisY2Enabled = FALSE;
    m_AxisY2Min = 0.0;
    m_AxisY2Max = 0.0;
    m_AxisY2MinAutoScaleOn = TRUE;
    m_AxisY2MaxAutoScaleOn = TRUE;
    m_AxisY2Unit = _T("");

    m_pChart->PutAllowUserChanges( FALSE );
    CleanUp();
    m_UseCommonXValues = FALSE;
    m_NoOfSeries = noOfSeries;
    m_totalNoSeries = noOfSeries;
    m_GraphType = oc2dTypePolar;
    m_OriginBase = 90;
    m_AxesReversed = TRUE;
    m_AxisYMin = 0.0;
    m_AxisXMinAutoScaleOn = TRUE;
    m_AxisXMaxAutoScaleOn = TRUE;
    m_AxisYMinAutoScaleOn = FALSE;
    m_AxisYMaxAutoScaleOn = TRUE;
    m_AxisXUnit = _T("");
    m_AxisYUnit = _T("");
    m_PolarRange0ToPlusMinus180 = TRUE;
    m_ControlBorder = oc2dBorderPlain;
    m_LegendBorder = oc2dBorderNone;
    m_ShowLegend = TRUE;
    m_ChartBorder = oc2dBorderPlain;
    m_ChartHeader = _T("");
    m_ChartFooter = _T("");
    m_pChart->GetFooter()->put_Adjust( oc2dAdjustCenter );

    m_graphData.resize(MAX_NO_OF_SERIES+1);
    for( int i=1; i<=MAX_NO_OF_SERIES; i++ )
    {
        m_graphData[i].x = 0.0;
        m_graphData[i].y = 0.0;
        m_graphData[i].valid = FALSE;

        m_PointShapes[i] = GetPointShape( i );
        m_PointSizes[i] = 7;
        m_PointColors[i] = GetColor( i );
        m_LinePatterns[i] = oc2dLineNone;
        m_LineColors[i] = GetColor( i );
    }
	m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
	m_ReductionFactorForGraphData = 1;
	m_ShowLiveDataGraph = FALSE;
	m_ShowBarGraph = FALSE;

    return( UpdateGraph() );
}

BOOL CGraphView::InitDefaultLiveDataGraph( int noOfSeries, int noOfSeriesY2 )
{
    theApp.SetLiveGraphViewActivated( FALSE );
    m_NoOfSeriesY2 = noOfSeriesY2;
    m_AxisY2Enabled = ( noOfSeriesY2 == 0 ) ? FALSE : TRUE;
    m_AxisY2Min = 0.0;
    m_AxisY2Max = 0.0;
    m_AxisY2MinAutoScaleOn = TRUE;
    m_AxisY2MaxAutoScaleOn = TRUE;
    m_AxisY2Unit = _T("");

    m_pChart->PutAllowUserChanges( TRUE );
    CleanUp();
    m_UseCommonXValues = TRUE;
    m_NoOfSeries = noOfSeries;
    m_totalNoSeries = m_NoOfSeriesY2+m_NoOfSeries;
    m_GraphType = oc2dTypePlot;
    m_OriginBase = 0;
    m_AxesReversed = FALSE;
    m_AxisXMin = 0.0;
    m_AxisXMinAutoScaleOn = FALSE;
    m_AxisXMaxAutoScaleOn = TRUE;
    m_AxisYMinAutoScaleOn = TRUE;
    m_AxisYMaxAutoScaleOn = TRUE;
    m_AxisXUnit = _T("");
    m_AxisYUnit = _T("");
    m_ControlBorder = oc2dBorderPlain;
    m_LegendBorder = oc2dBorderNone;
    m_ShowLegend = TRUE;
    m_ChartBorder = oc2dBorderPlain;
    m_ChartHeader = _T("");
    m_ChartFooter = _T("");
    m_pChart->GetFooter()->put_Adjust( oc2dAdjustCenter );

    m_graphData.resize(m_totalNoSeries+1);
    for( int i=1; i<m_graphData.size(); i++ )
    {
        m_graphData[i].x = 0.0;
        m_graphData[i].y = 0.0;
        m_graphData[i].valid = FALSE;
        m_graphData[i].axis = Axis_None;

        //         m_ValidGraphDataToBeUpdated[i] = FALSE;
//         m_GraphDataX[i] = 0.0;
//         m_GraphDataY[i] = 0.0;
        //m_SerieLabels[i] = _T("");
        m_PointShapes[i] = oc2dShapeNone;
        m_PointSizes[i] = 17;
        m_PointColors[i] = GetColor( i );
        m_LinePatterns[i] = oc2dLineSolid;//GetLinePattern( i );
        m_LineColors[i] = GetColor( i );
    }
	m_TimeUnit = SHOW_TIME_AXIS_IN_SECONDS;
	m_ReductionFactorForGraphData = 1;
	m_ShowLiveDataGraph = FALSE;
	m_ShowBarGraph = FALSE;

    return( UpdateGraph() );
}

BOOL CGraphView::ShowPolarGraphWithText( PolarGraphInParam* pInParam )
{
    BOOL result = TRUE;
	m_LastSavedGraphFileName = _T("");
    HideGraph();
     
    int noOfEmptyRowsInHeader = 10;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    int noOfEmptyRowsInBlock3Top = 1;
    int noOfEmptyRowsInBlock3Bottom = 1;
    int noOfEmptyRowsInBlock4Top = 1;
    int noOfEmptyRowsInBlock4Bottom = 0;
 
    long legendTextXPosOffset = 52;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
	
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextAmp = 6;
    long averageNoOfCharactersOnTextAngle = 4;  
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());
    
    if( maxLength > MAX_NO_OF_CHAR_IN_TEXT_DATA )
    {
        maxLength = MAX_NO_OF_CHAR_IN_TEXT_DATA;
    }
      
    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }

    long textAreaWidth = 28 * MAX_WIDTH_OF_CHARACTER;

    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
    long timeConstLabelYPos = noOfEmptyRowsInHeader * ROW_HEIGHT;
    long losDirLabelYPos = timeConstLabelYPos + ROW_HEIGHT;
    int i = 0;
    if( ( pInParam->mode == Diff ) && ( g_AlignerData.NoOfBedPlanes != 0 ) )
    {
        i++;
    }
    else if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) )
    {
        i++;
    }
  
    long line2YPos;
    if( i == 0 )
    {
        line2YPos = timeConstLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    }
    else
    {
        line2YPos = losDirLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    }
    long stationLabelYPos = line2YPos + noOfEmptyRowsInBlock3Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long referenceLabelYPos = stationLabelYPos + ROW_HEIGHT;
    long line3YPos = referenceLabelYPos + noOfEmptyRowsInBlock3Bottom * ROW_HEIGHT;
    long legendTopPos = line3YPos - legendTextYPosOffset + noOfEmptyRowsInBlock4Top * ROW_HEIGHT;
    long line4YPos = legendTopPos + ( noOfEmptyRowsInBlock4Bottom + m_NoOfSeries ) * ROW_HEIGHT;
    long verticalLineStartYPos = line2YPos + ROW_HEIGHT;
    long verticalLineStopYPos = line4YPos;

    long legendLeftPos = m_ControlWidth - textAreaWidth;
    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
    long horizontalLineXPos = legendLeftPos;// + chartAndTextAreaOverlap;
    long verticalLine1XPos = m_ControlWidth - 14*MAX_WIDTH_OF_CHARACTER;//legendTextLabelXPos + MAX_NO_OF_CHAR_IN_LEGEND_LABEL * MAX_WIDTH_OF_CHARACTER ;
    long verticalLine2XPos = verticalLine1XPos + (m_ControlWidth - verticalLine1XPos)/2;//textAmpXPos + noOfSpaceCharacters * MAX_WIDTH_OF_CHARACTER;
    long textAmpXPos = verticalLine1XPos + 1*MAX_WIDTH_OF_CHARACTER;//legendTextLabelXPos + ( MAX_NO_OF_CHAR_IN_LEGEND_LABEL + noOfSpaceCharacters + averageNoOfCharactersOnTextAmp ) * MAX_WIDTH_OF_CHARACTER;
    long textAngleXPos = verticalLine2XPos + 2*MAX_WIDTH_OF_CHARACTER; //textAmpXPos + ( noOfSpaceCharacters + averageNoOfCharactersOnTextAngle ) * MAX_WIDTH_OF_CHARACTER;

    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;

    CString solidLine, dashedLine;
    char verticalLineCharacter[2] = {"|"};
    CString str, tmpStr;
	solidLine = _T("");
	for( i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}

    try
    {
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos);// + chartAndTextAreaOverlap );
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
		    //m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );

		if( m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->GetValue() < MAX_POLAR_GRAPH_RESOLUTION )
        {
            m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->PutIsDefault( FALSE );
            m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->PutValue( MAX_POLAR_GRAPH_RESOLUTION );
        }

		if( m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->GetValue() >= 10 )
		{	//show only integers
			m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutValue( 0 );
		}
		else
		{	//show with one decimal
			m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutValue( 1 );
		}
		double temp;
        temp = m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetTickSpacing()->GetValue();
        m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutValue( temp ); //0 to disable
		m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
    
        str.LoadString( IDS_TILT_POLAR_GRAPH_HEADER );
        str += "                           ";
        m_pChart->GetHeader()->GetText()->PutText( str.AllocSysString() );

		str.LoadString( GetMRad() == TRUE ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
		
        tmpStr.LoadString( IDS_TILT_POLAR_GRAPH_FOOTER );
		str += _T("\n \n") + tmpStr;// + _T("\n ");
        m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );

        str = g_AlignerData.AlignMode;
        str.Replace(_T("&&"), _T("&"));
        AddChartLabel("alignModeLabel", str, legendTextLabelXPos, alignModeLabelYPos);           
      
        AddCommonHeader(pInParam, legendLeftPos);

        str.LoadString( IDS_TILT_POLAR_GRAPH_TIME_CONST_LABEL );
        AddChartLabel("timeConstLabel", str, textLabelXPos, timeConstLabelYPos);           
   
        tmpStr.LoadString( IDS_S_UNIT );
        str.Format( _T("%2.1f"), pInParam->tao );
        str += " " + tmpStr;
        AddChartLabel("timeConst", str, textDataXPos, timeConstLabelYPos);           
  
        if(!pInParam->commonFlatTest)
        {
            if( ( pInParam->mode == Diff ) && ( g_AlignerData.NoOfBedPlanes != 0 ) )
            {
                str.LoadString( IDS_TILT_POLAR_GRAPH_LOS_DIR_LABEL );
                AddChartLabel("losDirLabel", str, textLabelXPos, losDirLabelYPos);           
                                
                AddChartLabel("losDir", g_AlignerData.AzDir, textDataXPos, losDirLabelYPos);                           
            }
            else if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) )
            {
                str.LoadString( IDS_TILT_POLAR_GRAPH_PARALLAX_COMP_LABEL );
                AddChartLabel("parallaxCompLabel", str, textLabelXPos, losDirLabelYPos); 
                
                str.LoadString( g_AlignerData.UseParallax ? IDS_ON : IDS_OFF);
                AddChartLabel("parallaxComp", str, textDataXPos, losDirLabelYPos);               
            }
        }
        AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);      
       
        str.LoadString( pInParam->commonFlatTest ? IDS_TILT_POLAR_GRAPH_SENSORS_LABEL : IDS_TILT_POLAR_GRAPH_STATIONS_LABEL );
        AddChartLabel("stationLabel", str, legendTextLabelXPos, stationLabelYPos);              
   
        str.LoadString( IDS_TILT_POLAR_GRAPH_AMPLITUDE_LABEL );
        AddChartLabel("ampLabel", str, textAmpXPos, stationLabelYPos);//, oc2dAnchorWest);              
       
        str.LoadString( IDS_TILT_POLAR_GRAPH_ANGLE_LABEL );
        AddChartLabel("angleLabel", str, textAngleXPos, stationLabelYPos);//, oc2dAnchorWest);              
   
        str.LoadString( IDS_TILT_POLAR_GRAPH_REFERENCE_LABEL );
        AddChartLabel("referenceLabel", str, textRefLabelXPos, referenceLabelYPos);              

        //if( ( pInParam->mode != Stab ) && ( pInParam->mode != AB ) )
		if( pInParam->mode != Stab )
        {
            //str = g_AlignerData.Device[pInParam->RefNo].Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
			if( pInParam->commonFlatTest == TRUE )
			{
				//CString sensorSN;
				//AfxFormatString1( sensorSN, IDS_SN_XXX, GetSensorSerialNumber( pInParam->RefNo ) );
				//AfxFormatString2( str, IDS_SENSOR_NAME_TO_DEVICE, sensorSN, (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
				AfxFormatString1( str, IDS_SN_XXX, GetSensorSerialNumber( pInParam->RefNo ) );
			}
			else
			{
				str = (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
			}
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);                          
        }
        else
        {
            str.LoadString( IDS_TILT_POLAR_GRAPH_HORIZONTAL_PLANE_LABEL );
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);                                      
        }
    
        str.LoadString( (GetMRad() == TRUE) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);        
        AddChartLabel("amplitudeUnitLabel", str, textAmpXPos, referenceLabelYPos);
      
        str.LoadString( IDS_DEG_UNIT );
        AddChartLabel("degLabel", str, textAngleXPos, referenceLabelYPos);
       
        AddChartLabel("line3", dashedLine, horizontalLineXPos, line3YPos);
  
        for( i=1; i<=m_NoOfSeries; i++ )
        {
            //Only one value in each serie
            str.Format( _T("%2.2f"), m_graphData[i].y );
            tmpStr.Format( _T("amp%d"), i );
            AddChartLabel(tmpStr, str, textAmpXPos, line3YPos + i * ROW_HEIGHT);//, oc2dAnchorWest);        
          
            str.Format( _T("%2.1f"), (m_PolarRange0ToPlusMinus180 == TRUE) ? GetRange0ToPlusMinus180( m_graphData[i].x ) : m_graphData[i].x );
            tmpStr.Format( _T("angle%d"), i );
            AddChartLabel(tmpStr, str, textAngleXPos, line3YPos + i * ROW_HEIGHT);//, oc2dAnchorWest);                                  
        }
        AddChartLabel("line4", solidLine, horizontalLineXPos, line4YPos);

        i = 1;
        for( int y=verticalLineStartYPos; y<=verticalLineStopYPos; y+=ROW_HEIGHT )
        {
            tmpStr.Format( _T("vLine1YPos%d"), i );
            AddChartLabel(tmpStr, verticalLineCharacter, verticalLine1XPos, y);
           
            tmpStr.Format( _T("vLine2YPos%d"), i );
            AddChartLabel(tmpStr, verticalLineCharacter, verticalLine2XPos, y);            
            i++;
        }
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}

BOOL CGraphView::ShowMeasureGraphWithText( MeasureGraphInParam* pInParam )
{
    BOOL result = TRUE;
	m_LastSavedGraphFileName = _T("");
    HideGraph();

    //int i, j;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    int noOfEmptyRowsInBlock3Top = 1;
    int noOfEmptyRowsInBlock3Bottom = 1;
    int noOfEmptyRowsInBlock4Top = 1;
    int noOfEmptyRowsInBlock4Bottom = 0;
    int noOfEmptyRowsInBlock5Top = 1;
    int noOfEmptyRowsInBlock5Bottom = 0;
    long chartAndTextAreaOverlap = 0;
    //long legendTextXPosOffset = 55;
    long legendTextXPosOffset = 52;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
    long noOfRowsInFooter = 6; //there must be space for the x-axis UnitType above the footer  
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextTilt = 6;
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());
    if( maxLength > MAX_NO_OF_CHAR_IN_TEXT_DATA )
    {
        maxLength = MAX_NO_OF_CHAR_IN_TEXT_DATA;
    }
      
    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }

    long textAreaWidth = 25*MAX_WIDTH_OF_CHARACTER;//-chartAndTextAreaOverlap + (averageNoOfCharactersOnTextLabel + averageNoOfCharactersOnTextData + 2*noOfSpaceCharacters ) * MAX_WIDTH_OF_CHARACTER;

//     long averageNoOfCharactersOnLegendRow = MAX_NO_OF_CHAR_IN_LEGEND_LABEL + 
//     averageNoOfCharactersOnTextTilt + 3 * noOfSpaceCharacters;
//     long tmpL = averageNoOfCharactersOnLegendRow * MAX_WIDTH_OF_CHARACTER + 
//     legendTextXPosOffset - chartAndTextAreaOverlap;
// 
//     if( tmpL > textAreaWidth )
//     {
//         textAreaWidth = tmpL;
//     }

    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
    long projectLabelYPos = alignModeLabelYPos + 2 * ROW_HEIGHT;
    long shipLabelYPos = projectLabelYPos + ROW_HEIGHT;
    long dateLabelYPos = shipLabelYPos + ROW_HEIGHT;
    long timeLabelYPos = dateLabelYPos + ROW_HEIGHT;
    long operatorLabelYPos = timeLabelYPos + ROW_HEIGHT;
    long placeLabelYPos = operatorLabelYPos + ROW_HEIGHT;
    long line1YPos = placeLabelYPos + noOfEmptyRowsInBlock1Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    long timeConstLabelYPos = line1YPos + noOfEmptyRowsInBlock2Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long losDirLabelYPos = timeConstLabelYPos;
    long elevCompLabelYPos = timeConstLabelYPos;
    long distanceLabelYPos = timeConstLabelYPos;
    long bearingLabelYPos = timeConstLabelYPos;

    if( ( pInParam->mode == Diff ) && ( g_AlignerData.NoOfBedPlanes != 0 ) )
    {
        //OutTextXY(2,84,"LOS Dir: " + Azdir)
        //OutTextXY(2,96,"Elev comp: On/Off")
        losDirLabelYPos += ROW_HEIGHT;
        elevCompLabelYPos = losDirLabelYPos + ROW_HEIGHT;
        distanceLabelYPos = elevCompLabelYPos;
        bearingLabelYPos = distanceLabelYPos;
    }
    else if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) )
    {
        //OutTextXY(2,84,"Parallax comp: On/Off")
        //OutTextXY(2,96,"Elev comp: On/Off")
        //OutTextXY(2,108,"Dist: " + Range + " [m]");
        //OutTextXY(2,120,"Bearing: " + Bear + " °");
        losDirLabelYPos += ROW_HEIGHT;
        elevCompLabelYPos = losDirLabelYPos + ROW_HEIGHT;
        distanceLabelYPos = elevCompLabelYPos + ROW_HEIGHT;
        bearingLabelYPos = distanceLabelYPos + ROW_HEIGHT;
    }
    else if( pInParam->mode == RPath )
    {
        //OutTextXY(2,84,"Azim: " + Bear + " °");
        losDirLabelYPos += ROW_HEIGHT;
        elevCompLabelYPos = losDirLabelYPos;
        distanceLabelYPos = elevCompLabelYPos;
        bearingLabelYPos = distanceLabelYPos;
    }
    //else if( pInParam->mode == AB )
    //{
    //  //OutTextXY(2,84,"Samp. rate: " + Rate + " Hz");
    //  losDirLabelYPos += ROW_HEIGHT;
    //  elevCompLabelYPos = losDirLabelYPos;
    //  distanceLabelYPos = elevCompLabelYPos;
    //  bearingLabelYPos = distanceLabelYPos;
    //}
    long line2YPos = bearingLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    long stationLabelYPos = line2YPos + noOfEmptyRowsInBlock3Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long referenceLabelYPos = stationLabelYPos + ROW_HEIGHT;
    long line3YPos = referenceLabelYPos + noOfEmptyRowsInBlock3Bottom * ROW_HEIGHT;
    long legendTopPos = line3YPos - legendTextYPosOffset + noOfEmptyRowsInBlock4Top * ROW_HEIGHT;
    long line4YPos = legendTopPos + ( noOfEmptyRowsInBlock4Bottom + m_NoOfSeries ) * ROW_HEIGHT;
    long tempLabelYPos = line4YPos + noOfEmptyRowsInBlock5Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    int j = 1; // one line for temperature label
    for( int i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
    {
        if( pInParam->pChCon1[i] == TRUE )
        {
            j++;
        }
    }
    long line5YPos = tempLabelYPos + ( noOfEmptyRowsInBlock5Bottom + j ) * ROW_HEIGHT - ROW_HEIGHT / 2;
    long verticalLineStartYPos = line2YPos + ROW_HEIGHT;
    long verticalLineStopYPos = line4YPos;

    long legendLeftPos = m_ControlWidth - textAreaWidth;
    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
  
    long horizontalLineXPos = legendLeftPos + chartAndTextAreaOverlap;
    long verticalLine1XPos = m_ControlWidth - 7*MAX_WIDTH_OF_CHARACTER;//legendTextLabelXPos + (MAX_NO_OF_CHAR_IN_LEGEND_LABEL+MAX_NO_OF_EXTRA_CHAR_IN_LEGEND_LABEL) * MAX_WIDTH_OF_CHARACTER;
	long textTiltXPos = verticalLine1XPos + averageNoOfCharactersOnTextTilt * MAX_WIDTH_OF_CHARACTER;
    long textTempUnitTypeLabelXPos = verticalLine1XPos + 3*MAX_WIDTH_OF_CHARACTER;

    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;

    CString solidLine, dashedLine;
    char verticalLineCharacter[2] = {"|"};
    CString str, tmpStr;
	solidLine = _T("");
	for( i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}

    try
    {
        if( pInParam->writeTiltData == FALSE )
        {
            m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
            m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
            m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
            m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
            m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
            m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
            m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
            m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos + chartAndTextAreaOverlap );
            m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
            m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
            m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );
            //int width = (int) ( m_pChart->GetChartArea()->GetPlotArea()->GetRight() - m_pChart->GetChartArea()->GetPlotArea()->GetLeft() );

			m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
			m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
			m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );

            if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) || ( pInParam->mode == RPath ) )
            {
                str.LoadString( IDS_MEASURE_GRAPH_FOOTER_ANGLES_POS_UP );
                m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );
            }
            else
            {
                str.LoadString( (g_AlignerData.SignDef == -1) ? IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_UP : IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_DOWN );
                m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );
            }

            str = g_AlignerData.AlignMode;
            str.Replace(_T("&&"), _T("&"));

            AddChartLabel("alignModeLabel", str, legendTextLabelXPos, alignModeLabelYPos);            

            str.LoadString( IDS_GRAPH_PROJECT_LABEL );
            AddChartLabel("projectLabel", str, textLabelXPos, projectLabelYPos);            

            str = SysSetup->GetProjectName().Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
            AddChartLabel("projectName", str, textDataXPos, projectLabelYPos);            
     
            str.LoadString( IDS_TILT_POLAR_GRAPH_SHIP_LABEL );
            AddChartLabel("shipLabel", str, textLabelXPos, shipLabelYPos);            
   
            str = g_AlignerData.Ship.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
            AddChartLabel("shipName", str, textDataXPos, shipLabelYPos);            
     
            str.LoadString( IDS_TILT_POLAR_GRAPH_DATE_LABEL );
            AddChartLabel("dateLabel", str, textLabelXPos, dateLabelYPos);            
     
            AddChartLabel("date", pInParam->DateStamp, textDataXPos, dateLabelYPos);            
           
            str.LoadString( IDS_START_TIME_LABEL );
            AddChartLabel("timeLabel", str, textLabelXPos, timeLabelYPos);            

            AddChartLabel("time", pInParam->TimeStamp, textDataXPos, timeLabelYPos);            

            str.LoadString( IDS_TILT_POLAR_GRAPH_OPERATOR_LABEL );
            AddChartLabel("operatorLabel", str, textLabelXPos, operatorLabelYPos);            
             
            str = g_AlignerData.Operator.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
            AddChartLabel("operator", str, textDataXPos, operatorLabelYPos);            
  
            str.LoadString( IDS_TILT_POLAR_GRAPH_PLACE_LABEL );
            AddChartLabel("placeLabel", str, textLabelXPos, placeLabelYPos);            
                       
            str = g_AlignerData.Place.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
            AddChartLabel("place", str, textDataXPos, placeLabelYPos);                

            AddChartLabel("line1", solidLine, horizontalLineXPos, line1YPos);                
           
            str.LoadString( IDS_TILT_POLAR_GRAPH_TIME_CONST_LABEL );
            AddChartLabel("timeConstLabel", str, textLabelXPos, timeConstLabelYPos);                
                
            tmpStr.LoadString( IDS_S_UNIT );
            str.Format( _T("%2.1f"), pInParam->timeConstant );
            str += " " + tmpStr;
            AddChartLabel("timeConst", str, textDataXPos, timeConstLabelYPos);                
     
            if( pInParam->commonFlatTest == FALSE )
            {
                if( ( pInParam->mode == Diff ) && ( g_AlignerData.NoOfBedPlanes != 0 ) )
                {
                    //OutTextXY(2,84,"LOS Dir: " + Azdir)
                    str.LoadString( IDS_TILT_POLAR_GRAPH_LOS_DIR_LABEL );
                    AddChartLabel("losDirLabel", str, textLabelXPos, losDirLabelYPos);   
                 
                    AddChartLabel("losDir", g_AlignerData.AzDir, textDataXPos, losDirLabelYPos);   
                   
                }
                else if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) )
                {
                    str.LoadString( IDS_TILT_POLAR_GRAPH_PARALLAX_COMP_LABEL );
                    AddChartLabel("parallaxCompLabel", str, textLabelXPos, losDirLabelYPos);   
                 
                    m_pChart->GetChartLabels()->Add()->PutName( "parallaxComp" );
                    
                    str.LoadString( g_AlignerData.UseParallax ? IDS_ON : IDS_OFF);               
                    AddChartLabel("parallaxComp", str, textDataXPos, losDirLabelYPos); 
                    
                    //OutTextXY(2,108,"Dist: " + Range + " [m]");
                    str.LoadString( IDS_MEASURE_GRAPH_DIST_LABEL );
                    AddChartLabel("distanceLabel", str, textLabelXPos, distanceLabelYPos); 
                   
                    tmpStr.LoadString( IDS_M_UNIT );
                    str.Format( _T("%2.1f"), g_AlignerData.Range );
                    str += " " + tmpStr;
                    AddChartLabel("distance", str, textDataXPos, distanceLabelYPos); 
                   
                    //OutTextXY(2,120,"Bearing: " + Bear + " °");
                    str.LoadString( IDS_MEASURE_GRAPH_BEARING_LABEL );
                    AddChartLabel("bearingLabel", str, textLabelXPos, bearingLabelYPos); 
                   
                    tmpStr.LoadString( IDS_MEASURE_GRAPH_BEARING_UNIT_LABEL );
                    str.Format( _T("%2.1f"), g_AlignerData.Bear );
                    str += " " + tmpStr;
                    AddChartLabel("bearing", str, textDataXPos, bearingLabelYPos);                     
                }
                else if( pInParam->mode == RPath )
                {
                    //OutTextXY(2,84,"Azim: " + Bear + " °");
                    str.LoadString( IDS_MEASURE_GRAPH_AZIM_LABEL );
                    AddChartLabel("azimLabel", str, textLabelXPos, losDirLabelYPos);
                    
                    tmpStr.LoadString( IDS_MEASURE_GRAPH_BEARING_UNIT_LABEL );
                    str.Format( _T("%2.1f"), g_AlignerData.Bear );
                    str += " " + tmpStr;
                    AddChartLabel("bearing", str, textDataXPos, losDirLabelYPos);                   
                }

                if( ( ( pInParam->mode == Diff ) && ( g_AlignerData.NoOfBedPlanes != 0 ) ) || ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) )
                {
                    str.LoadString( IDS_MEASURE_GRAPH_ELEV_COMP_LABEL );
                    AddChartLabel("elevCompLabel", str, textLabelXPos, elevCompLabelYPos);
                  
                    m_pChart->GetChartLabels()->Add()->PutName( "elevComp" );

                    //OutTextXY(2,96,"Elev comp: On")
                    str.LoadString( pInParam->useElev ? IDS_ON : IDS_OFF);
                    AddChartLabel("elevComp", str, textDataXPos, elevCompLabelYPos);                 
                }
            }
            AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);                 
          
            //OutTextXY(2,144,"STATIONS");
            str.LoadString( pInParam->commonFlatTest ? IDS_TILT_POLAR_GRAPH_SENSORS_LABEL : IDS_TILT_POLAR_GRAPH_STATIONS_LABEL);
            AddChartLabel("stationLabel", str, legendTextLabelXPos, stationLabelYPos);            
            
            //OutTextXY(Xres-Xval+2,144,"TILT");
            str.LoadString( IDS_MEASURE_GRAPH_TILT_LABEL );
            AddChartLabel("tiltLabel", str, textTiltXPos, stationLabelYPos, oc2dAnchorWest);            
            
            str.LoadString( IDS_TILT_POLAR_GRAPH_REFERENCE_LABEL );
            AddChartLabel("referenceLabel", str, textRefLabelXPos, referenceLabelYPos);            
            if( pInParam->mode != Stab )
            {
                //OutTextXY(2,156,"Re: " + Device[m_InParam.RefObjNo])
                //str = g_AlignerData.Device[pInParam->RefNo].Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
				if( pInParam->commonFlatTest == TRUE )
				{
					//CString sensorSN;
					//AfxFormatString1( sensorSN, IDS_SN_XXX, GetSensorSerialNumber( pInParam->RefNo ) );
					//AfxFormatString2( str, IDS_SENSOR_NAME_TO_DEVICE, sensorSN, (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
					AfxFormatString1( str, IDS_SN_XXX, GetSensorSerialNumber( pInParam->RefNo ) );
				}
				else
				{
					str = (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
				}                
            }
            else
            {
                //OutTextXY(2,156,"Re: Horis. plane");
                str.LoadString( IDS_TILT_POLAR_GRAPH_HORIZONTAL_PLANE_LABEL );                
            }

            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);                       
            //OutTextXY(Xres-Xval+2,156,"["+ UnitTypeS +"]");
            str.LoadString( GetMRad() ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT);
            AddChartLabel("tiltUnitLabel", str, textTiltXPos, referenceLabelYPos, oc2dAnchorWest);           
            
            //OutTextXY(2,168,"-----------------------");
            AddChartLabel("line3", dashedLine, horizontalLineXPos, line3YPos); 
          
            i = 1;
            map<int, LiveGraphInfo>::iterator iter ;
            for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
            //for( i=1; i<=m_NoOfSeries; i++ )
            {
                if( (pInParam->mode == Stab) || (pInParam->mode == StabDiff) || (pInParam->mode == RPath) )
                {
	                //SetSerieLabel( i, GetUnitTypeDescription( iter->second.id ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
                    AddGraph(i++, GetUnitTypeDescription(iter->second.id).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ), iter->second.id);
                }
                else
                {
	                CString serieLabel;
	                if( pInParam->commonFlatTest == TRUE )
	                {
		                //CString sensorSN;
		                //AfxFormatString1( sensorSN, IDS_SN_XXX, GetSensorSerialNumber( g_AlignerData.ObjNo[j] ) );
		                //AfxFormatString2( serieLabel, IDS_SENSOR_NAME_TO_DEVICE, sensorSN, (GetUnitTypeDescription( g_AlignerData.ObjNo[j] )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
		                AfxFormatString1( serieLabel, IDS_SN_XXX, GetSensorSerialNumber( iter->second.id ) );
	                }
	                else
	                {
		                serieLabel = (GetUnitTypeDescription( iter->second.id)).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
	                }
	                AfxFormatString1( str, IDS_MEASURE_GRAPH_R_LABEL, serieLabel );
	                AddGraph(i++, str, iter->second.id);
                    //SetSerieLabel( i++, str );
	                AfxFormatString1( str, IDS_MEASURE_GRAPH_P_LABEL, serieLabel );
                    AddGraph(i++, str, iter->second.id);
                    //SetSerieLabel( i, str );	   
                }
            } 
            
            UpdateGraphLabels();

            AddChartLabel("line4", solidLine, horizontalLineXPos, line4YPos); 

            //show start temperatures
	        j = 0;
            str.LoadString( IDS_START_TEMPERATURE_LABEL );
            AddChartLabel("temperatureLabel", str, textLabelXPos, tempLabelYPos + j * ROW_HEIGHT);

	        str.LoadString( IDS_GRAD_CELCIUS_UNIT );
            AddChartLabel("temperatureUnitlabel", str, textTempUnitTypeLabelXPos, tempLabelYPos + j++ * ROW_HEIGHT);
            
            for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
            {
                if( pInParam->pChCon1[i] == TRUE )
                {
                //OutTextXY(2,j+2,"Temp Ch" + i + ": "+ m_OutParam.TempT[i] + "°C");
                //j = j+12;
                //tmpStr.Format( _T("%d"), i );
			    //str = g_AlignerData.Device[i];
			    if( pInParam->commonFlatTest == TRUE )
			    {
				    AfxFormatString1( str, IDS_SN_XXX, GetSensorSerialNumber( i ) );
			    }
			    else
			    {
                    str = GetUnitTypeDescription( i ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
				}
                //AfxFormatString1( str, IDS_MEASURE_GRAPH_TEMP_CH_LABEL, tmpStr );
                tmpStr.Format( _T("tempCh%d"), i );
                AddChartLabel(tmpStr, str, textLabelXPos, tempLabelYPos + j * ROW_HEIGHT);
              
                str.Format( _T("%2.1f"), pInParam->pTempT[i] );              
                tmpStr.Format( _T("temp%d"), i );
                AddChartLabel(tmpStr, str, textTempUnitTypeLabelXPos, tempLabelYPos + j++ * ROW_HEIGHT );               
            }      
        }

        AddChartLabel("line5", solidLine, horizontalLineXPos, line5YPos); 
       
        i = 1;
        for( int y=verticalLineStartYPos; y<=verticalLineStopYPos; y+=ROW_HEIGHT )
        {
            tmpStr.Format( _T("vLine1YPos%d"), i );
            AddChartLabel(tmpStr, verticalLineCharacter, verticalLine1XPos, y );           
            i++;
        }
    }//pInParam->writeTiltData == FALSE
    else
    {
        for( i=1; i<=m_NoOfSeries; i++ )
        {
            //Only last value in each serie
            if( ( pInParam->mode == Stab ) || ( pInParam->mode == StabDiff ) || ( pInParam->mode == RPath ) )
            {
                //SetColor(Cl[i]);
                //OutTextXY(2,168+12*i, Kh * UP[i] );
                str.Format( _T("%2.2f"), m_graphData[i].y );
                tmpStr.Format( _T("tilt%d"), i );
                AddChartLabel(tmpStr, str, textTiltXPos, line3YPos + i * ROW_HEIGHT, oc2dAnchorWest );             
            }
            else
            {
                //SetColor(Cl[2*i-1]);
                //OutTextXY(2,156+24*i, g_AlignerData.SignDef*Kh*UR[i] );
                //SetColor(Cl[2*i]);
                //OutTextXY(2,168+24*i, g_AlignerData.SignDef*Kh*UP[i]);
                str.Format( _T("%2.2f"), m_graphData[i].y );
                tmpStr.Format( _T("tiltR%d"), i );
                AddChartLabel(tmpStr, str, textTiltXPos, line3YPos + i * ROW_HEIGHT, oc2dAnchorWest );  
                
                i++;
                str.Format( _T("%2.2f"), m_graphData[i].y );
                tmpStr.Format( _T("tiltP%d"), i );
                AddChartLabel(tmpStr, str, textTiltXPos,  line3YPos + i * ROW_HEIGHT , oc2dAnchorWest );                 
           }
        }
        //show final temperatures
        str.LoadString( IDS_FINAL_TEMPERATURE_LABEL );
        m_pChart->GetChartLabels()->GetItem( "temperatureLabel" )->GetText()->PutText( str.AllocSysString() );

        for( i=FIRST_ARRAY_INDEX_SENSOR; i<=LAST_ARRAY_INDEX_SENSOR; i++ )
        {
            if( pInParam->pChCon1[i] == TRUE )
            {
                str.Format( _T("%2.1f"), pInParam->pTempT[i] );
                tmpStr.Format( _T("temp%d"), i );
                m_pChart->GetChartLabels()->GetItem( tmpStr.AllocSysString() )->GetText()->PutText( str.AllocSysString() );
            }
        }
		}//pInParam->writeTiltData == TRUE
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}

BOOL CGraphView::ShowTiltFlatnessTestGraphWithText( TiltFlatnessTestGraphInParam* pInParam )
{
    BOOL result = TRUE;
    m_LastSavedGraphFileName = _T("");
    HideGraph();
   
    int noOfRowsInHeader = 10;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    int noOfEmptyRowsInBlock3Top = 1;
    int noOfEmptyRowsInBlock3Bottom = 1;
    int noOfEmptyRowsInBlock4Top = 1;
    int noOfEmptyRowsInBlock4Bottom = 0;
    long chartAndTextAreaOverlap = 0;
    long legendTextXPosOffset = 52;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
    long noOfRowsInFooter = 6; //there must be space for the x-axis UnitType above the footer
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());
    
    if( maxLength > MAX_NO_OF_CHAR_IN_TEXT_DATA )
    {
        maxLength = MAX_NO_OF_CHAR_IN_TEXT_DATA;
    }
      
    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }

    long textAreaWidth = 25*MAX_WIDTH_OF_CHARACTER;
    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
    long timeConstLabelYPos = noOfRowsInHeader * ROW_HEIGHT;
    long noOfPointsLabelYPos = timeConstLabelYPos + ROW_HEIGHT;
    long line2YPos = noOfPointsLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    long stationLabelYPos = line2YPos + noOfEmptyRowsInBlock3Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long referenceLabelYPos = stationLabelYPos + ROW_HEIGHT;
    long planeTypeLabelYPos;
    if( pInParam->mode != Stab )
    {
        planeTypeLabelYPos = referenceLabelYPos + ROW_HEIGHT;
    }
    else
    {
        planeTypeLabelYPos = referenceLabelYPos; //planeType not shown
    }
    long line3YPos = planeTypeLabelYPos + noOfEmptyRowsInBlock3Bottom * ROW_HEIGHT;
    long legendTopPos = line3YPos - legendTextYPosOffset + noOfEmptyRowsInBlock4Top * ROW_HEIGHT;
    long line4YPos = legendTopPos + ( noOfEmptyRowsInBlock4Bottom + m_NoOfSeries ) * ROW_HEIGHT;
  
    long legendLeftPos = m_ControlWidth - textAreaWidth;
    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
    long horizontalLineXPos = legendLeftPos + chartAndTextAreaOverlap;

    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;

    CString solidLine, dashedLine;
    CString str, tmpStr;
	solidLine = _T("");
	for( int i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( int i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}
  
    try
    {
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos);
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
        m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );

		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );

        str.LoadString( (pInParam->PlMode == Sine ) ? IDS_MEASURE_GRAPH_FOOTER_ANGLES_POS_UP : IDS_POS_ERROR_ABOVE_PLANE);
        m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );

        if( pInParam->PlMode == Linear )
        {
            str.LoadString( g_AlignerData.Found ? IDS_FLATNESS_ERROR_FOUNDATION : IDS_FLATNESS_ERROR_PLATFORM);            
        }
        else
        {
            str = g_AlignerData.AlignMode;
            str.Replace(_T("&&"), _T("&"));
        }

        AddChartLabel("alignModeLabel", str, legendTextLabelXPos, alignModeLabelYPos);           
        
        AddCommonHeader(pInParam, legendLeftPos);

        str.LoadString( IDS_TILT_POLAR_GRAPH_TIME_CONST_LABEL );
        AddChartLabel("timeConstLabel", str, textLabelXPos, timeConstLabelYPos);   
        
        tmpStr.LoadString( IDS_S_UNIT );
        str.Format( _T("%2.1f"), pInParam->timeConstant );
        str += " " + tmpStr;
        AddChartLabel("timeConst", str, textDataXPos, timeConstLabelYPos);   
        
        //OutTextXY(2,84,'No of points: ' + Points);
        str.LoadString( IDS_NO_OF_POINTS_LABEL );
        AddChartLabel("noOfPointsLabel", str, textLabelXPos, noOfPointsLabelYPos);   
                
        str.Format( _T("%d"), pInParam->Points );
        AddChartLabel("noOfPoints", str, textDataXPos, noOfPointsLabelYPos);         

        AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);   
    
        //OutTextXY(2,132,'STATIONS');
        str.LoadString( IDS_TILT_POLAR_GRAPH_STATIONS_LABEL );
        AddChartLabel("stationLabel", str, legendTextLabelXPos, stationLabelYPos);         
       
        str.LoadString( IDS_TILT_POLAR_GRAPH_REFERENCE_LABEL );
        AddChartLabel("referenceLabel", str, textRefLabelXPos, referenceLabelYPos);                

        if( pInParam->mode != Stab )
        {
            //OutTextXY(2,144,'Re: ' + Device[Ref]);            
			str = (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos); 
           
            //OutTextXY(2,156,'    (type ' + PlaneType[Ref] + ')');
			AfxFormatString1( str, IDS_TYPE_LABEL, UnitType::GetUnitText( g_AlignerData.PlaneType[pInParam->RefNo] ) );
            AddChartLabel("planeType", str, legendTextLabelXPos, planeTypeLabelYPos);           
        }
        else
        {
            //OutTextXY(2,144,'Re: Horis. plane');
            str.LoadString( IDS_TILT_POLAR_GRAPH_HORIZONTAL_PLANE_LABEL );
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);                    
        }
  
        //OutTextXY(2,168,'-----------------------');
        AddChartLabel("line3", dashedLine, horizontalLineXPos, line3YPos);               
        
        AddChartLabel("line4", solidLine, horizontalLineXPos, line4YPos);     
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}

BOOL CGraphView::ShowTiltFlatnessFoTestGraphWithText( TiltFlatnessFoTestGraphInParam* pInParam )
{
    BOOL result = TRUE;
	m_LastSavedGraphFileName = _T("");
    HideGraph();
  
    int noOfEmptyRowsInHeader = 10;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    int noOfEmptyRowsInBlock3Top = 1;
    int noOfEmptyRowsInBlock3Bottom = 1;
    int noOfEmptyRowsInBlock4Top = 1;
    int noOfEmptyRowsInBlock4Bottom = 0;
    long chartAndTextAreaOverlap = 0;
    long legendTextXPosOffset = 52;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
    long noOfRowsInFooter = 6; //there must be space for the x-axis UnitType above the footer
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());
    if( maxLength > MAX_NO_OF_CHAR_IN_TEXT_DATA )
    {
        maxLength = MAX_NO_OF_CHAR_IN_TEXT_DATA;
    }
      
    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }

    long textAreaWidth = 27 * MAX_WIDTH_OF_CHARACTER;
    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
    long timeConstLabelYPos = noOfEmptyRowsInHeader * ROW_HEIGHT;
    long noOfPointsLabelYPos = timeConstLabelYPos + ROW_HEIGHT;
    long line2YPos = noOfPointsLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    long stationLabelYPos = line2YPos + noOfEmptyRowsInBlock3Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long referenceLabelYPos = stationLabelYPos + ROW_HEIGHT;
    long planeTypeLabelYPos;
    planeTypeLabelYPos = (pInParam->mode != Stab) ? referenceLabelYPos + ROW_HEIGHT : referenceLabelYPos;
    
    long line3YPos = planeTypeLabelYPos + noOfEmptyRowsInBlock3Bottom * ROW_HEIGHT;
    long legendTopPos = line3YPos - legendTextYPosOffset + noOfEmptyRowsInBlock4Top * ROW_HEIGHT;
    long line4YPos = legendTopPos + ( noOfEmptyRowsInBlock4Bottom + m_NoOfSeries ) * ROW_HEIGHT;
      
    long legendLeftPos = m_ControlWidth - textAreaWidth;
    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
    long horizontalLineXPos = legendLeftPos + chartAndTextAreaOverlap;

    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;

    CString solidLine, dashedLine;
    CString str, tmpStr;
	solidLine = _T("");
	for( int i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( int i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}
  
    try
    {
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos + chartAndTextAreaOverlap );
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
        m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );

		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );

        //OuttextXY(2,1,'Angles pos up')
        str.LoadString( (pInParam->PlMode == Sine) ? IDS_MEASURE_GRAPH_FOOTER_ANGLES_POS_UP : IDS_POS_ERROR_ABOVE_PLANE);
        m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );

        if( pInParam->PlMode == Linear )
        {            
            str.LoadString( ( g_AlignerData.Found == TRUE ) ? IDS_FLATNESS_ERROR_FOUNDATION : IDS_FLATNESS_ERROR_PLATFORM);            
        }
        else
        {
            str = g_AlignerData.AlignMode;
            str.Replace(_T("&&"), _T("&"));
        }
        AddChartLabel("alignModeLabel", str, legendTextLabelXPos, alignModeLabelYPos);              

        AddCommonHeader(pInParam, legendLeftPos);
      
        str.LoadString( IDS_TILT_POLAR_GRAPH_TIME_CONST_LABEL );
        AddChartLabel("timeConstLabel", str, textLabelXPos, timeConstLabelYPos);
       
        tmpStr.LoadString( IDS_S_UNIT );
        str.Format( _T("%2.1f"), pInParam->timeConstant );
        str += " " + tmpStr;
        AddChartLabel(tmpStr, str, textDataXPos, timeConstLabelYPos);
   
        //OutTextXY(2,84,'No of points: ' + Points);
        str = "No of steps:";//.LoadString( IDS_NO_OF_POINTS_LABEL );
        AddChartLabel("noOfPointsLabel", str, textLabelXPos, noOfPointsLabelYPos);
           
        str.Format( _T("%d"), pInParam->Points );
        AddChartLabel("noOfPoints", str, textDataXPos, noOfPointsLabelYPos);
   
        AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);
        
        //OutTextXY(2,132,'STATIONS');
        str.LoadString( IDS_TILT_POLAR_GRAPH_STATIONS_LABEL );
        AddChartLabel("stationLabel", str, legendTextLabelXPos, stationLabelYPos);
      
        str.LoadString( IDS_TILT_POLAR_GRAPH_REFERENCE_LABEL );
        AddChartLabel("referenceLabel", str, textRefLabelXPos, referenceLabelYPos);
   
        if( pInParam->mode != Stab )
        {
            //OutTextXY(2,144,'Re: ' + Device[Ref]);
            //str = g_AlignerData.Device[pInParam->RefNo].Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
			str = (GetUnitTypeDescription( pInParam->RefNo )).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);
            
            //OutTextXY(2,156,'    (type ' + PlaneType[Ref] + ')');
			AfxFormatString1( str, IDS_TYPE_LABEL, UnitType::GetUnitText( g_AlignerData.PlaneType[pInParam->RefNo] ) );
            AddChartLabel("planeType", str, legendTextLabelXPos, planeTypeLabelYPos);           
        }
        else
        {
            //OutTextXY(2,144,'Re: Horis. plane');
            str.LoadString( IDS_TILT_POLAR_GRAPH_HORIZONTAL_PLANE_LABEL );
            AddChartLabel("reference", str, legendTextLabelXPos, referenceLabelYPos);            
        }
  
        //OutTextXY(2,168,'-----------------------');
        AddChartLabel("line3", dashedLine, horizontalLineXPos, line3YPos);
       
        AddChartLabel("line4", solidLine, horizontalLineXPos, line4YPos);      
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}

void CGraphView::ShowGraph( void )
{
    theApp.SetLiveGraphViewActivated( TRUE);
    theApp.SwitchView( theApp.m_pGraphView );
}

void CGraphView::HideGraph( void )
{
	theApp.SetLiveGraphViewActivated( FALSE );
    theApp.SwitchView( theApp.m_pSystemConfigurationView );
}

BOOL CGraphView::UpdateLiveData( void )
{
    int result=TRUE;
    int pointCountY1 = ( m_NoOfSeries > 0 ) ? m_pData->GetNumPoints( 1 ) : 0;
    int pointCountY2 = ( m_NoOfSeriesY2 > 0 ) ? m_pDataY2->GetNumPoints( 1 ) : 0;
    
    if( ( pointCountY1*m_NoOfSeries + pointCountY2*m_NoOfSeriesY2 ) > MAX_NO_OF_SAMPLES_IN_GRAPH )
    {
        m_ShowLiveDataGraph = FALSE;
        m_ShowBarGraph = FALSE;
        result = FALSE;
        m_LiveDataCounter = m_LiveDataCounterStopValue + 1;
    }
   
    int newCountY1 = pointCountY1+1;
    int newCountY2 = pointCountY1+1;
    BOOL XY1Set=FALSE, XY2Set=FALSE;

    m_pData->PutIsBatched( TRUE );
    m_pDataY2->PutIsBatched( TRUE );    
    for( int gr=1; gr<=m_totalNoSeries; gr++ )
    {
        int graphNum = m_graphData[gr].graphNum; 
        if(m_graphData[gr].axis == Axis_Y1)
        {            
           // TRACE("UpdateLiveData Y1, serie:%d\n",graphNum); 
            m_pData->PutNumPoints( graphNum, newCountY1 );
            if(!XY1Set)
            {
                m_pData->PutX( ( long )graphNum, newCountY1, m_graphData[gr].x );
                XY1Set = TRUE;
            }
            m_pData->PutY( ( long )graphNum, newCountY1, m_graphData[gr].y );           
        }
        else if(m_graphData[gr].axis == Axis_Y2)
        {            
         //   TRACE("UpdateLiveData Y2, serie:%d\n",graphNum); 
            m_pDataY2->PutNumPoints( graphNum, newCountY2 );
            if(!XY2Set)
            {
                m_pDataY2->PutX( ( long )graphNum, newCountY2 ,m_graphData[gr].x );
                XY2Set = TRUE;
            }
            m_pDataY2->PutY( ( long )graphNum, newCountY2 ,m_graphData[gr].y );          
        }
        
    }

    m_pData->PutIsBatched( FALSE );
    m_pDataY2->PutIsBatched( FALSE );

    
    return result;
}

BOOL CGraphView::UpdateData( void )
{
    BOOL result = TRUE;
    long pointCount, pointCountY2;
    int serie;

    try
    {
        if( m_UseCommonXValues == TRUE )
        {
			if( m_GraphType == oc2dTypeBar )
			{
			    pointCount = 0;
                pointCountY2 = 0;
			}
			else
			{
                serie = 1;
                pointCount = ( m_NoOfSeries > 0 ) ? m_pData->GetNumPoints( serie ) : 0;
                pointCountY2 = ( m_NoOfSeriesY2 > 0 ) ? m_pDataY2->GetNumPoints( serie ) : 0;
		    }

            if( ( pointCount*m_NoOfSeries + pointCountY2*m_NoOfSeriesY2 ) > MAX_NO_OF_SAMPLES_IN_GRAPH )
            {
                //TODO ?, should never happen
        //        TRACE("\n MAX_NO_OF_SAMPLES_IN_GRAPH");
				m_ShowLiveDataGraph = FALSE;
				m_ShowBarGraph = FALSE;
                result = FALSE;
                m_LiveDataCounter = m_LiveDataCounterStopValue + 1;
            }
            else
            {
                if( m_NoOfSeries > 0 )
                {
                    serie = 1;
                    m_pData->PutIsBatched( TRUE );
                    m_pData->PutNumPoints( serie, pointCount + 1 );
                    m_pData->PutX( ( long )serie, pointCount + 1 ,m_graphData[serie].x );
                    m_pData->PutY( ( long )serie, pointCount + 1 ,m_graphData[serie].y );

                    for( serie=2; serie<=m_NoOfSeries; serie++ )
                    {
                        m_pData->PutNumPoints( serie, pointCount + 1 );
                        m_pData->PutY( ( long )serie, pointCount + 1 , m_graphData[serie].y );
                    }
                    m_pData->PutIsBatched( FALSE );
                }

                if( ( IsAxisY2Enabled() == TRUE ) && ( m_NoOfSeriesY2 > 0 ) )
                {
                    serie = 1;
                    m_pDataY2->PutIsBatched( TRUE );
                    m_pDataY2->PutNumPoints( serie, pointCountY2 + 1 );
                    m_pDataY2->PutX( ( long )serie, pointCountY2 + 1 , m_graphData[m_NoOfSeries + serie].x );
                    m_pDataY2->PutY( ( long )serie, pointCountY2 + 1 , m_graphData[m_NoOfSeries + serie].y );

                    for( serie=2; serie<=m_NoOfSeriesY2; serie++ )
                    {
                        m_pDataY2->PutNumPoints( serie, pointCountY2 + 1 );
                        m_pDataY2->PutY( ( long )serie, pointCountY2 + 1 , m_graphData[m_NoOfSeries + serie].y );
                    }
                    m_pDataY2->PutIsBatched( FALSE );
                }
            }
        }//m_UseCommonXValues == TRUE
        else
        {
            pointCount = 0;
            pointCountY2 = 0;

            for( serie=1; serie<=m_NoOfSeries; serie++ )
            {
                pointCount += m_pData->GetNumPoints( serie );
            }

            for( serie=1; serie<=m_NoOfSeriesY2; serie++ )
            {
                pointCountY2 += m_pDataY2->GetNumPoints( serie );
            }

            if( ( pointCount + pointCountY2 ) > MAX_NO_OF_SAMPLES_IN_GRAPH )
            {
                //TODO ?, should never happen
        //        TRACE("\n MAX_NO_OF_SAMPLES_IN_GRAPH");
				m_ShowLiveDataGraph = FALSE;
				m_ShowBarGraph = FALSE;
                result = FALSE;
                m_LiveDataCounter = m_LiveDataCounterStopValue + 1;
            }
            else
            {
                if( m_NoOfSeries > 0 )
                {
                    m_pData->PutIsBatched( TRUE );

                    for( serie=1; serie<=m_NoOfSeries; serie++ )
                    {
                        if( m_graphData[serie].valid == TRUE )
                        {
                            pointCount = m_pData->GetNumPoints( serie );
                            m_pData->PutNumPoints( serie, pointCount + 1 );
                            m_pData->PutX( ( long )serie, pointCount + 1 , m_graphData[serie].x );
                            m_pData->PutY( ( long )serie, pointCount + 1 , m_graphData[serie].y );
                            m_graphData[serie].valid = FALSE;
                        }
                    }
                    m_pData->PutIsBatched( FALSE );
                }

                if( ( IsAxisY2Enabled() == TRUE) && ( m_NoOfSeriesY2 > 0 ) )
                {
                    m_pDataY2->PutIsBatched( TRUE );

                    for( serie=1; serie<=m_NoOfSeriesY2; serie++ )
                    {
                        if( m_graphData[m_NoOfSeries + serie].valid == TRUE )
                        {
                            pointCountY2 = m_pDataY2->GetNumPoints( serie );
                            m_pDataY2->PutNumPoints( serie, pointCountY2 + 1 );
                            m_pDataY2->PutX( ( long )serie, pointCountY2 + 1 , m_graphData[m_NoOfSeries + serie].x );
                            m_pDataY2->PutY( ( long )serie, pointCountY2 + 1 , m_graphData[m_NoOfSeries + serie].y );
                            m_graphData[m_NoOfSeries + serie].valid = FALSE;
                        }
                    }
                    m_pDataY2->PutIsBatched( FALSE );
                }
            }
        }//m_UseCommonXValues == FALSE
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    return( result );
}

BOOL CGraphView::UpdateGraph( void )
{
    BOOL result = TRUE;

    try
    {
        m_pGroup->PutChartType( m_GraphType );

        if( m_UseCommonXValues == TRUE )
        {
            m_pData->PutLayout( oc2dDataArray );
        }
        else
        {
            m_pData->PutLayout( oc2dDataGeneral );
        }
        m_pData->PutNumSeries( m_NoOfSeries );
        m_pChart->GetChartArea()->GetAxes()->GetItem( 1L )->PutOriginBase( m_OriginBase );
        m_pChart->GetChartArea()->GetAxes()->GetItem( 1L )->PutIsReversed( (VARIANT_BOOL)m_AxesReversed );

        m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMin()->PutIsDefault( (VARIANT_BOOL)m_AxisXMinAutoScaleOn );
        if( m_AxisXMinAutoScaleOn == FALSE )
        {
            m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMin()->PutValue( m_AxisXMin );
        }

        m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMax()->PutIsDefault( (VARIANT_BOOL)m_AxisXMaxAutoScaleOn );
        if( m_AxisXMaxAutoScaleOn == FALSE )
        {
            m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMax()->PutValue( m_AxisXMax );
        }

        m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMin()->PutIsDefault( (VARIANT_BOOL)m_AxisYMinAutoScaleOn );
        if( m_AxisYMinAutoScaleOn == FALSE )
        {
            m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMin()->PutValue( m_AxisYMin );
        }

        m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->PutIsDefault( (VARIANT_BOOL)m_AxisYMaxAutoScaleOn );
        if( m_AxisYMaxAutoScaleOn == FALSE )
        {
            m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMax()->PutValue( m_AxisYMax );
        }    
        
        m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetTitle()->PutText( m_AxisXUnit.AllocSysString() );
        m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetTitle()->PutText( m_AxisYUnit.AllocSysString() );

        if( m_GraphType == oc2dTypePolar )
        {
            m_pChart->GetChartArea()->GetPolar()->PutIsHalfRange( (VARIANT_BOOL)m_PolarRange0ToPlusMinus180 );
        }
        m_pChart->GetBorder()->PutType( m_ControlBorder );
        m_pChart->GetLegend()->GetBorder()->PutType( m_LegendBorder );
        m_pChart->GetLegend()->PutAnchor( oc2dAnchorEast );
        m_pChart->GetLegend()->PutIsShowing( (VARIANT_BOOL)m_ShowLegend );
        m_pChart->GetChartArea()->GetBorder()->PutType( m_ChartBorder );
        m_pChart->GetHeader()->GetText()->PutText( m_ChartHeader.AllocSysString() );
        m_pChart->GetFooter()->GetText()->PutText( m_ChartFooter.AllocSysString() );

        for( int i=1; i<=m_NoOfSeries; i++ )
        {
            m_pData->PutNumPoints( i, 0 );
            m_pStyleCollection->GetItem( ( long )i )->GetSymbol()->PutShape( m_PointShapes[i] );
            m_pStyleCollection->GetItem( ( long )i )->GetSymbol()->PutSize( m_PointSizes[i] );
            m_pStyleCollection->GetItem( ( long )i )->GetSymbol()->PutColor( m_PointColors[i] );
            m_pStyleCollection->GetItem( ( long )i )->GetLine()->PutPattern( m_LinePatterns[i] );
            m_pStyleCollection->GetItem( ( long )i )->GetLine()->PutColor( m_LineColors[i] );
         //   TRACE("Y1, Set line color %d:%d\n",i, m_LineColors[i]);         
        }
    
        if( m_GraphType == oc2dTypeBar )
        {
            m_pChart->GetChartArea()->GetBar()->PutClusterOverlap( -100 );
            m_pChart->GetChartArea()->GetBar()->PutClusterWidth( ( 2 * m_NoOfSeries - 1 ) * m_ClusterWidthForOneBar );
            m_pChart->GetChartArea()->GetAxes()->GetItem("x")->PutAnnotationMethod( oc2dAnnotateValueLabels );
        }
	    else
	    {
		    m_pChart->GetChartArea()->GetAxes()->GetItem("x")->PutAnnotationMethod( oc2dAnnotateValues );
	    }

        //if( m_AxisY2Enabled == TRUE )
        if( theApp.IsAligner202Enabled())
        {
            m_pGroupY2->PutChartType( m_GraphType );

            if( m_UseCommonXValues == TRUE )
            {
                m_pDataY2->PutLayout( oc2dDataArray );
            }
            else
            {
                m_pDataY2->PutLayout( oc2dDataGeneral );
            }
            m_pDataY2->PutNumSeries( m_NoOfSeriesY2 );
            //m_pChart->GetChartArea()->GetAxes()->GetItem( 2L )->PutOriginBase( m_OriginBase );
            //m_pChart->GetChartArea()->GetAxes()->GetItem( 2L )->PutIsReversed( (VARIANT_BOOL)m_AxesReversed );

            m_pChart->GetChartArea()->GetAxes()->GetItem( "y2" )->GetMin()->PutIsDefault( (VARIANT_BOOL)m_AxisY2MinAutoScaleOn );
            if( m_AxisYMinAutoScaleOn == FALSE )
            {
                m_pChart->GetChartArea()->GetAxes()->GetItem( "y2" )->GetMin()->PutValue( m_AxisY2Min );
            }

            m_pChart->GetChartArea()->GetAxes()->GetItem( "y2" )->GetMax()->PutIsDefault( (VARIANT_BOOL)m_AxisY2MaxAutoScaleOn );
            if( m_AxisYMaxAutoScaleOn == FALSE )
            {
                m_pChart->GetChartArea()->GetAxes()->GetItem( "y2" )->GetMax()->PutValue( m_AxisY2Max );
            }    
          
            m_pChart->GetChartArea()->GetAxes()->GetItem( "y2" )->GetTitle()->PutText( m_AxisY2Unit.AllocSysString() );

            for( int i=1; i<=m_NoOfSeriesY2; i++ )
            {
                m_pDataY2->PutNumPoints( i, 0 );
                m_pStyleCollectionY2->GetItem( ( long )i )->GetSymbol()->PutShape( m_PointShapes[i + m_NoOfSeries] );
                m_pStyleCollectionY2->GetItem( ( long )i )->GetSymbol()->PutSize( m_PointSizes[i + m_NoOfSeries] );
                m_pStyleCollectionY2->GetItem( ( long )i )->GetSymbol()->PutColor( m_PointColors[i + m_NoOfSeries] );
                m_pStyleCollectionY2->GetItem( ( long )i )->GetLine()->PutPattern( m_LinePatterns[i + m_NoOfSeries] );
                m_pStyleCollectionY2->GetItem( ( long )i )->GetLine()->PutColor( m_LineColors[i + m_NoOfSeries] );
                TRACE("Y2, Set line color %d:%d\n",i, m_LineColors[i + m_NoOfSeries]);     
            }     
        }

    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    return( result );
}

void CGraphView::OnInitialUpdate( void )
{
    __super::OnInitialUpdate();

    LPUNKNOWN pUnk = m_chart.GetControlUnknown();
    if(pUnk == NULL)
    {
        ::AfxMessageBox("Could not create chart.");   
        return;
    }

    IChart2D_DualPtr chart = pUnk;  

    m_exitWithoutSave = FALSE;
      
    if (m_firstUpdate)
    {
        m_firstUpdate = FALSE ;
        try
        {
            m_pGroup = m_pChart->GetChartGroups()->GetItem( 1L );
            m_pStyleCollection = m_pGroup->GetStyles();
            m_pData = m_pGroup->GetData();
            m_pGroupY2 = m_pChart->GetChartGroups()->GetItem( 2L );
            m_pStyleCollectionY2 = m_pGroupY2->GetStyles();
            m_pDataY2 = m_pGroupY2->GetData();
            m_pChart->GetLegend()->PutIsShowing( FALSE );
            long top = m_pChart->GetChartArea()->GetLocation()->GetTop()->GetValue();
            long height = m_pChart->GetChartArea()->GetLocation()->GetHeight()->GetValue();
            long left = m_pChart->GetChartArea()->GetLocation()->GetLeft()->GetValue();
            long width = m_pChart->GetChartArea()->GetLocation()->GetWidth()->GetValue();
            m_pChart->GetLegend()->PutIsShowing( (VARIANT_BOOL)m_ShowLegend );
            m_ControlWidth = width + 2 * left;
            m_ControlHeight = height + 2 * top;
            CRect rect;
            m_chart.GetWindowRect(&rect);

            UpdateGraph();
        }
            catch ( _com_error e )
            {
    //      TRACE( _T("") );
        }
    }
}

void CGraphView::OnBnClickedLiveGraphQuit( void )
{
    CString MsgCaption, MsgText;
	CString graphFileName;

    if( m_pSource != NULL )
    {
        m_pSource->EnableErrorEvent( FALSE );
    }
    else
    {
        DAU::GetDAU().EnableErrorEvent( FALSE );
    }

    m_ShowLiveDataGraph = FALSE;

    if( (theApp.IsAligner308Enabled()) && (m_exitWithoutSave == FALSE) )
    {
	    if( SaveGraphToUniqueFileName( graphFileName ) == TRUE )
	    {
		    m_LiveGraphFileManager.SaveFileName( graphFileName, FALSE );
	    }
        m_LiveDataGraphParam.includeGraph = TRUE;
        m_LiveDataGraphParam.graphFileName = graphFileName;
	    m_pLiveGraphResultTable = new CResultTable();
	    InitResultTable();

       // added by R2 040624, to get same final values in the graph and in the report
		for( int i=1; i<=g_AlignerData.liveGraphMap.size(); i++ )
		{
		    g_AlignerData.ACR[i] = g_AlignerData.SignDef * m_graphData[i*2-1].m_previousLiveGraphValue;
		    g_AlignerData.ACP[i] = g_AlignerData.SignDef * m_graphData[i*2].m_previousLiveGraphValue;
		    CartToVec( g_AlignerData.ACR[i], g_AlignerData.ACP[i], &(g_AlignerData.VecAmp[i]), &(g_AlignerData.VecArg[i]) ) ;
		}

		MsgCaption.LoadString( IDS_QUESTION_CAPTION );
		MsgText.LoadString( IDS_EXIT_WITH_OPPORTUNITY_TO_SAVE );

		if( IDYES == MessageBox( MsgText, MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
		{
			  
			m_pLiveGraphResultTable->ShowReport( TRUE );
			MsgText.LoadString( IDS_SAVE_THE_RESULT_TABLE_TO_THE_LOG_RECORD );

			if( IDYES == MessageBox( MsgText, MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
			{
				ExitResultTable( FALSE);		
				MsgText.LoadString( IDS_SAVE_THE_GRAPH_TO_THE_LOG_RECORD );
				if( IDYES == MessageBox( MsgText, MsgCaption, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 ) )
				{
					m_LiveGraphFileManager.IncludeToResultTable( TRUE, graphFileName );
				}	
			}
			else
			{
				ExitResultTable( TRUE);
			}

		}
		else
		{
			ExitResultTable( FALSE);
		}
    }
    
  //  m_LiveGraphFileManager.MoveUnwantedToTemporaryDir();  
      
    CleanUp();
    theApp.SetLiveGraphViewActivated( FALSE );
    SetSource( NULL ) ;
}

void CGraphView::CleanUp( void )
{
    m_exitWithoutSave = FALSE;
	CloseLiveDataFile() ;
    GetDlgItem( IDC_LIVE_GRAPH_PRINT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_LIVE_GRAPH_QUIT )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_LIVE_GRAPH_SAVE )->ShowWindow( SW_HIDE );
    GetDlgItem( IDC_LIVE_GRAPH_STOP )->ShowWindow( SW_HIDE );
    //theApp.SwitchView( theApp.m_pAlignerView ) ;
	theApp.SwitchView( theApp.m_pSystemConfigurationView ) ;
    ClearLiveGraph() ;
	if( m_pSource != NULL )
	{
		m_pSource -> Stop() ;
        ResetAllFilters( m_pSource ) ;
	}
}

void CGraphView::OnBnClickedLiveGraphPrint( void )
{
    PrintLiveGraph();
}

void CGraphView::OnBnClickedLiveGraphSave( void )
{  
    int pos;
    if( (pos = CSetupLiveDataGraphDlg::m_TextFileName.Find( "." )) != -1 )
    {
        m_GraphFileName = CSetupLiveDataGraphDlg::m_TextFileName.Left( pos );
    }
    else if( CSetupLiveDataGraphDlg::m_TextFileName.GetLength() > 0 )
    {
        m_GraphFileName = CSetupLiveDataGraphDlg::m_TextFileName;
    }
    else
    {
        m_GraphFileName = (CString)DEFAULT_LIVE_DATA_FILE_NAME + (CString)DEFAULT_LIVE_DATA_GRAPH_FILE_EXT;
    }

    CString dir = SysSetup->GetProjectPath();
    CString fileName = dir + _T("\\") + m_GraphFileName;
    static char BASED_CODE szFilter[] = "Image Files (*.tiff)|*.tiff|";
	CFileDialog dlg(FALSE,_T("tiff"),(LPCTSTR)fileName,OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT,szFilter);   

	if (dlg.DoModal() == IDOK)
	{
        CString oldFileDir = m_GraphFileDir;
		m_GraphFileDir = dlg.GetPathName();
        m_GraphFileName = dlg.GetFileName();

        int iPathLength = m_GraphFileDir.GetLength() - m_GraphFileName.GetLength();
        if( iPathLength >= 0 ) 
        {
            m_GraphFileDir.GetBufferSetLength( iPathLength );
        }
        else 
        {
            m_GraphFileDir = oldFileDir;
        }
	} 

    CString tmp;
    tmp = m_GraphFileDir + m_GraphFileName;

	SaveGraphToFile( m_GraphFileDir + m_GraphFileName );
}

BOOL CGraphView::UpdateSourceBarData( void )
{
    ASSERT( m_pSource != NULL );
    BOOL result = TRUE;
    int i, serieCounter;
    double val, referenceVal, diffVal;

	referenceVal = m_barGraphUseRoll ? GetRoll( g_AlignerData.RefObjNo, m_currentValues ): GetPitch( g_AlignerData.RefObjNo, m_currentValues );

    serieCounter = 1;
    double xPos;
    xPos = (double)(m_ClusterWidthForOneBar + (double)(( 2 * m_NoOfSeries - 1 ) * m_ClusterWidthForOneBar) / 2.0) / 100.0;

    for( int gr=1; gr<=m_NoOfSeries; gr++ )
    {     	
        val = m_barGraphUseRoll ? GetRoll( m_graphData[gr].id, m_currentValues ) : GetPitch( m_graphData[gr].id, m_currentValues );
        diffVal = val - referenceVal;

		if( g_AlignerData.SignDef == 1 )
		{
		    diffVal = -diffVal;	
        }
	    SetXYData( gr, xPos, diffVal );
    }

    UpdateData();

    return( result );
}

double CGraphView::GetDataValue(GraphData &data)
{
    double val=0.0f;
    
    if(theApp.IsAligner202Enabled())
    {
        switch(data.dataType)
        {
            case DATA_TYPE_ROLL:
            {
                val = MRADIANS_TO_DEGREES( GetUncalibratedRoll( data.id, m_currentValues ) );
                AdjustSyncroValueToRangeA202( &val, GetGearing( data.id, m_pSource ) );
                break;
            }
            case DATA_TYPE_PITCH:
            {
                val = MRADIANS_TO_DEGREES( GetUncalibratedPitch( data.id, m_currentValues ) );
                AdjustSyncroValueToRangeA202( &val, GetGearing( data.id, m_pSource ) );
                break;
            }
            case DATA_TYPE_HEADING:
            {
                val = MRADIANS_TO_DEGREES( GetUncalibratedHeading( data.id, m_currentValues ) );
                AdjustSyncroValueToRangeA202( &val, GetGearing( data.id, m_pSource ) );
                break;
            }
            default:break;
        }
    }
    else//A308
    {
        switch(data.dataType)
        {
            case DATA_TYPE_ROLL:
            {
                val = -g_AlignerData.SignDef * GetRoll(data.id, m_currentValues );            
                break;
            }
            case DATA_TYPE_PITCH:
            {
                val = -g_AlignerData.SignDef * GetPitch(data.id, m_currentValues );            
                break;
            }
            case DATA_TYPE_TEMPR:
            {                
                val = GetTemperature( data.id, m_currentValues );
                break;
            }
            default:break;
        }
    }

    
   return val;
}


BOOL CGraphView::UpdateTemprature()
{
    CString str, tmpStr;
    int i=1;
    int sensorNo = 0;    
    map<int, LiveGraphInfo>::iterator iter ;
    for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++,i++)
    {
        if( IsSensor( iter->second.id ) == TRUE )
        {
            sensorNo++;
            double tempr = GetTemperature( iter->second.id, m_currentValues );                      
            m_PreviousLiveGraphTemperature[i] = tempr;
            if(fabs(tempr - iter->second.lastTempr) > 0.5f)
            {
                iter->second.lastTempr = tempr;
                str.Format( _T("%2.1f"), tempr );
                tmpStr.Format( _T("temp%d"), sensorNo );
                m_pChart->GetChartLabels()->GetItem( tmpStr.AllocSysString() )->GetText()->PutText( str.AllocSysString() );
            }
        }
    }
    return (sensorNo > 0);
}

BOOL CGraphView::UpdateLiveDataGraph( void )
{
   // stw.Start();           

    ASSERT( m_pSource != NULL );
    BOOL result = TRUE;
    BOOL updateGraph;
    int i, serieCounter;
    double temperature = DBL_MAX;
    BOOL done = FALSE;        
      
    int updateEvery = m_samplesPerPix <= 1 ? 1 : (int)m_samplesPerPix;
    updateGraph = (m_LiveDataCounter % updateEvery==0);

    m_LiveDataCounter++;

	if( m_LiveDataCounter >= m_LiveDataCounterStopValue )
	{
        done = TRUE;
        updateGraph = TRUE;
	}
    serieCounter = 1;
    double xPos;
	CString str, tmpStr, fileData[4], rowStr;
	int colWidth = FILE_COL_WIDTH;

	if( m_StoreLiveDataToFile == TRUE )
	{
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            fileData[0].Format( _T("%d"), m_LiveDataFileWriteCounter );
			LAdjText( &fileData[0], 10 );
		}
        else
        {
		    rowStr.Format( _T("%d"), m_LiveDataFileWriteCounter );		    
            LAdjText(&rowStr, 12);
        }   
	}
  
    UpdateTemprature();

    for( int gr=1; gr<=m_totalNoSeries; gr++ )
    {        
        double dataValue = GetDataValue(m_graphData[gr]);               
    
        if( updateGraph == TRUE )
        {
            xPos = GetTimeValue( m_LiveDataCounter, m_LiveDataGraphTs );
            SetLiveXYData( gr, xPos, dataValue);                              
        }
    
	    if( m_StoreLiveDataToFile == TRUE && theApp.IsAligner202Enabled() == TRUE)
	    {
           	tmpStr.Format( _T(" %.3f"), dataValue );
			RAdjText( &tmpStr, colWidth );
			fileData[0] += tmpStr + _T(" ");           
	    }
    }

    if( m_StoreLiveDataToFile == TRUE )
    {
        m_LiveDataFileWriteCounter++;
        if(theApp.IsAligner202Enabled() == TRUE)
        {            
            fileData[0] += EOL_FILE;
            m_LiveDataFile.Write( fileData[0], fileData[0].GetLength() );
        }
        else if(theApp.IsAligner308Enabled() == TRUE)
        { 
            CString segStr, pad;                   
            LAdjText(&pad, 10);
            for( int gr=1; gr<=m_totalNoSeries; gr++ )
            {             
                double dataValue = GetDataValue(m_graphData[gr]);
                segStr.Format( _T(" %.3f"), dataValue );
                LAdjText( &segStr, 10 );
                rowStr += segStr;            
        
                //TRACE("id:%d",m_graphData[gr].id);
                if(gr%2==0)
                {
                    //Add temp col
                    double tempr=0;
                    if(IsSensor( m_graphData[gr].id ))
                    {
                        tempr = GetTemperature( m_graphData[gr].id, m_currentValues );                    
                        segStr.Format( _T("%.1f"), tempr);
                    }
                    else
                    {
                        segStr = "-";
                    }
                    CAdjText( &segStr, 8 );
                    rowStr += segStr + pad;                                
                
                }
              
            }
            rowStr += EOL_FILE;
            m_LiveDataFile.Write( rowStr, rowStr.GetLength() );
        }
     }

    if( updateGraph == TRUE )
    {
        UpdateLiveData();
        //UpdateGraphData();
    }

	if( done == TRUE )
	{
        DAU::GetDAU().EnableErrorEvent( FALSE );
       	m_ShowLiveDataGraph = FALSE;
		CloseLiveDataFile();
	
        ASound sound;
		sound.PlayNiceSound();
		GetDlgItem( IDC_LIVE_GRAPH_PRINT )->EnableWindow( TRUE );
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            GetDlgItem( IDC_LIVE_GRAPH_SAVE )->EnableWindow( TRUE );
        }
        GetDlgItem( IDC_LIVE_GRAPH_STOP )->EnableWindow( FALSE );
        m_LiveDataGraphParam.startLiveData = FALSE;
	    FinishLiveDataGraphWithText( &m_LiveDataGraphParam );
	}
// 
//     tr.Format("update:%d",updateGraph);
//     stw.Stop();
//     stw.TraceTime(TRUE, TRUE, tr);
    return( result );
}

afx_msg LRESULT CGraphView::OnNewData( WPARAM, LPARAM )
{
    if( m_pSource == NULL )
	{
		return( FALSE );
	}

    while( UpdateCurrentValues() )
    {
        if( m_ShowLiveDataGraph == TRUE )
        {
            UpdateLiveDataGraph();
        }
        else if( m_ShowBarGraph == TRUE )//else added by R2 040624
        {
            UpdateSourceBarData();
        }
    }
    return TRUE;
}

LRESULT CGraphView::OnDauError( WPARAM, LPARAM )
{
  CString MsgCaption, MsgText;
  MsgCaption.LoadString( IDS_ERROR_CAPTION );

  if( m_ShowLiveDataGraph == TRUE )
  {
    //DAU::GetDAU().Restart( TRUE );
	  m_ShowLiveDataGraph = FALSE;
    CString text;
	  text.LoadString( IDS_RESTART );
	  SetDlgItemText( IDC_LIVE_GRAPH_STOP, text );
    GetDlgItem( IDC_LIVE_GRAPH_STOP )->EnableWindow( FALSE );
	  GetDlgItem( IDC_LIVE_GRAPH_PRINT )->EnableWindow( TRUE );
    if( theApp.IsAligner202Enabled() == TRUE )
    {
      GetDlgItem( IDC_LIVE_GRAPH_SAVE )->EnableWindow( TRUE );
    }
    m_LiveDataGraphParam.startLiveData = FALSE;
    m_exitWithoutSave = TRUE;
    OnBnClickedLiveGraphQuit();
    //PressButton( IDC_LIVE_GRAPH_QUIT );
	  //ShowLiveDataGraphWithText( &m_LiveDataGraphParam );

    //MsgText.LoadString( IDS_INTERNAL_DAU_ERROR_RESTART_LIVE_GRAPH );
    //MessageBox( MsgText, MsgCaption, MB_ICONERROR|MB_OK );
  }
  else if( m_ShowBarGraph == TRUE )
  {
    //DAU::GetDAU().Restart( TRUE );
    m_ShowBarGraph = FALSE;
    m_exitWithoutSave = TRUE;
    OnBnClickedLiveGraphQuit();
  }

	return TRUE ;
}

void CGraphView::OnDestroy() 
{
  if( m_pSource != NULL )
  {
    ResetAllFilters( m_pSource ) ;
    SetSource( NULL ) ;  
  }
  
	CFormView::OnDestroy();
}

BOOL CGraphView::ShowBarGraphWithText( BarGraphInParam* pInParam )
{
    BOOL result = TRUE;
	m_LastSavedGraphFileName = _T("");
    HideGraph();

    int quickAndDirtyYAdjustment = 20; 
    int noOfEmptyRowsInHeader = 9;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    long chartAndTextAreaOverlap = 10;
    long legendTextXPosOffset = 52;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextAmp = 6;
    long averageNoOfCharactersOnTextAngle = 4;  
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());
    
    if( maxLength > MAX_NO_OF_CHAR_IN_TEXT_DATA )
    {
        maxLength = MAX_NO_OF_CHAR_IN_TEXT_DATA;
    }
  
    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }

    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
	long legendTopPos = noOfEmptyRowsInHeader * ROW_HEIGHT + quickAndDirtyYAdjustment;
    long line2YPos = legendTopPos + ( noOfEmptyRowsInBlock2Bottom + m_NoOfSeries ) * ROW_HEIGHT;
  
    long textAreaWidth = 30 * MAX_WIDTH_OF_CHARACTER;
    long legendLeftPos = m_ControlWidth - textAreaWidth;
    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
  
    long horizontalLineXPos = legendLeftPos;
  
    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;  

    CString solidLine, dashedLine;   
    CString str, tmpStr;
	solidLine = _T("");
	for( int i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( int i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}

    try
    {
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos);
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );        

		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutValue( 0 ); //0 to disable

        str.LoadString( g_AlignerData.SignDef == -1 ? IDS_BAR_GRAPH_FOOTER_ANGLES_POS_UP : IDS_BAR_GRAPH_FOOTER_ANGLES_POS_DOWN);
        
        m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );

        AddChartLabel("alignModeLabel", g_AlignerData.AlignMode, legendTextLabelXPos, alignModeLabelYPos);           
        
        AddCommonHeader( pInParam, legendLeftPos );
   
        AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);           

  	    int j = 1;    
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
        {
		    AddGraph(j++, GetUnitTypeDescription(iter->second.id).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ), iter->second.id);
            //SetSerieLabel( j++, GetUnitTypeDescription(iter->second.id).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL ) );
        }
        UpdateGraphLabels();
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}


BOOL CGraphView::SaveGraphToUniqueFileName( CString &fileName, bool resultImage)
{
  USES_CONVERSION ;

//	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, theApp.m_pMainWnd);
//    CDocument* pDoc1 = pFrame->GetActiveDocument();
	CString name = SysSetup->GetProjectName();// pDoc1->GetTitle();
    CString path = SysSetup->GetProjectPath() ;
    CString tempBmpName;

    path += resultImage ? _T("\\Images\\") : _T("\\IntermediateImages\\");

	std::string stdPath = path ;
    
    if( ::CreateDirectory( stdPath.c_str(), NULL ) == FALSE )
	{
        DWORD error = GetLastError();
        if(error != ERROR_ALREADY_EXISTS)
        {
            CString msg; msg.Format("Unable to create directory for images, err:%d.",error);
            AfxMessageBox( msg ) ;
    		return( FALSE );
        }
	}
	
    name += _T("(");

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString str; str.Format("%02d%02d%02d-%02d%02d%02d-%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);

	name += str;// COleDateTime::GetCurrentTime().Format(_T("%Y%m%d-%H%M%S:"));
	CString tiffName = name + _T(").tiff");;

    fileName = path + tiffName;

    _bstr_t footer = m_pChart -> GetFooter() -> GetText() -> GetText() ;
    CString footerText( footer.GetBSTR() ) ;
    footerText += _T("\n") ;
    footerText += tiffName ;
    m_pChart -> GetFooter() -> GetText() -> PutText( footerText.AllocSysString() ) ;
    BOOL saveFlag = SaveGraphToFile( fileName ) ;
    if (!saveFlag)
    {
        AfxMessageBox( _T("Unable to save file") ) ;
    }

   // fileName = fileName.TrimRight(".bmp") + ".tiff";

    return saveFlag;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    Status myStat = GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j){
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 ){
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

BOOL Bmp2Tiff(CString bmpFileName, BOOL deleteBmp=FALSE)
{
    {
        USES_CONVERSION;
        Gdiplus::Bitmap jpg(T2W(bmpFileName));
        CLSID encoderClsid;
        int res = GetEncoderClsid(L"image/tiff", &encoderClsid);        
        
        CString noPref = bmpFileName;
        CString tiffName = noPref.TrimRight(".bmp") + ".tiff";
         
        jpg.Save(T2W(tiffName), &encoderClsid, NULL);    
    }

    if(deleteBmp)
    {
        ::DeleteFile(bmpFileName);
    }

    return 0;
}


BOOL CGraphView::SaveGraphToFile( CString &fileName )
{
    CString noPref = fileName;
    CString bmpName = noPref.TrimRight(".tiff") + ".bmp";    
    if( m_pChart->DrawToFile( _bstr_t(bmpName).copy(), oc2dFormatBitmap) != 0 )
	{
		m_LastSavedGraphFileName = fileName;
		Bmp2Tiff(bmpName, TRUE);
        return( TRUE );
	}
    return( FALSE );
}

BOOL CGraphView::WriteLiveDataFileHeader( LiveDataGraphInParam* pInParam )
{
	CString str, tmpStr;
    CloseLiveDataFile();

	if( m_LiveDataFile.Open( pInParam->FileName, CFile::modeWrite|CFile::modeCreate, NULL ) ) 
	{
		str = _T("File Name: ") + pInParam->FileName + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%d"), 10 + g_AlignerData.liveGraphMap.size()/*g_AlignerData.NoOfCorr*/ );
		str = _T("Header length (lines): ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = _T("Start date [y-m-d]: ") + pInParam->DateStamp + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = _T("Start time [h:m:s]: ") + pInParam->TimeStamp + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		if( pInParam->TimeConstant != 0 )
		{
			tmpStr.Format( _T("%2.1f"), pInParam->TimeConstant );
		}
		else
		{
			tmpStr = _T("The filter is Off");
		}

        str = _T("Filter time const [s]: ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%.2f"), 1 / pInParam->Ts );
		str = _T("Sampling rate [Hz]: ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%.1f"), m_AxisXMax );
	    switch( m_TimeUnit )
	    {
	    case SHOW_TIME_AXIS_IN_HOURS:
		    str = _T("Nom. measuring time [hrs]: ") + tmpStr + EOL_FILE;
		    break;
	    case SHOW_TIME_AXIS_IN_MINUTES:
		    str = _T("Nom. measuring time [min]: ") + tmpStr + EOL_FILE;
		    break;
	    case SHOW_TIME_AXIS_IN_SECONDS:
	    default:
            str = _T("Nom. measuring time [s]: ") + tmpStr + EOL_FILE;
		    break;
	    }
		m_LiveDataFile.Write( str, str.GetLength() );
 
        tmpStr.LoadString( (SysSetup->GetUnits() == 0) ? IDS_MRAD_UNIT  : IDS_ARCMIN_UNIT);

        str = _T("Angular Unit: ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		CString strArray[SIZE_OF_ARRAYS + 2];
		strArray[1+g_AlignerData.liveGraphMap.size()] = _T(" ");
        strArray[2+g_AlignerData.liveGraphMap.size()] = _T("Spec.");

		
        CString head1="", head2="", head3="", chStr, pad="";
        LAdjText(&pad, 15);
        int i=1;
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++, i++) //Fix
        {			
			tmpStr = GetChannelName(iter->second.id);         
			head1 += _T("Chan. ") + tmpStr + _T(": ") + GetUnitTypeDescription( iter->second.id ) + EOL_FILE;          
			CAdjText( &tmpStr, 23 );
            chStr = "Roll     Pitch     Temp";          
            head2 += pad + tmpStr;
            head3 += pad + chStr;         
		}
        head1 += EOL_FILE;
        head2 += EOL_FILE;
        head3 += EOL_FILE;

        m_LiveDataFile.Write( head1, head1.GetLength() );
        m_LiveDataFile.Write( head2, head2.GetLength() );
        m_LiveDataFile.Write( head3, head3.GetLength() );

		m_LiveDataFileWriteCounter = 1;

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOL CGraphView::WriteLiveDataFileHeaderA202( LiveDataGraphInParam* pInParam )
{
	CString str, tmpStr;
    CloseLiveDataFile();

	if( m_LiveDataFile.Open( pInParam->FileName, CFile::modeWrite|CFile::modeCreate, NULL ) ) 
	{
		str = _T("File Name: ") + pInParam->FileName + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = _T("Comment: ") + g_AlignerData.Ship + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%d"), 12 );
		str = _T("Header length (lines): ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = _T("Start date [y-m-d]: ") + pInParam->DateStamp + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = _T("Start time [h:m:s]: ") + pInParam->TimeStamp + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%.2f"), 1 / pInParam->Ts );
		str = _T("Sampling rate [Hz]: ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		tmpStr.Format( _T("%.1f"), m_AxisXMax );
	    switch( m_TimeUnit )
	    {
	    case SHOW_TIME_AXIS_IN_HOURS:
		    str = _T("Nom. measuring time [hrs]: ") + tmpStr + EOL_FILE;
		    break;
	    case SHOW_TIME_AXIS_IN_MINUTES:
		    str = _T("Nom. measuring time [min]: ") + tmpStr + EOL_FILE;
		    break;
	    case SHOW_TIME_AXIS_IN_SECONDS:
	    default:
            str = _T("Nom. measuring time [s]: ") + tmpStr + EOL_FILE;
		    break;
	    }
		m_LiveDataFile.Write( str, str.GetLength() );

        tmpStr.LoadString( IDS_DEG_UNIT );
		str = _T("Angular Unit: ") + tmpStr + EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

		str = EOL_FILE;
		m_LiveDataFile.Write( str, str.GetLength() );

        CString strArray[3], channelName, signalDescription;
        double gearing;
		int firstColumnWidth = 10;
		int columnWidth = FILE_COL_WIDTH;
        strArray[0] = _T("Channel");
        strArray[1] = _T("Gearing");
        strArray[2] = _T("Signal");
		LAdjText(&strArray[0], firstColumnWidth);
		LAdjText(&strArray[1], firstColumnWidth);
		LAdjText(&strArray[2], firstColumnWidth);


	    for(int graph = 1; graph <=m_totalNoSeries ; graph++)
        {
			channelName = GetChannelName( m_graphData[graph].id );
            channelName += (IsDigital(m_graphData[graph].id) || theApp.IsAligner308Enabled()) ? " - " + m_dataTypeText[m_graphData[graph].dataType] : "";
            tmpStr = channelName;
            RAdjText( &tmpStr, columnWidth );
            strArray[0] += tmpStr + _T(" ");

            if( (channelName.GetAt(0) == 'D') || (channelName.GetAt(0) == 'C') ) //digital or composite channel
            {
                tmpStr.LoadString( IDS_NA );
            }
            else
            {
                gearing = GetGearing( m_graphData[graph].id, m_pSource );
                tmpStr.Format( _T("%.1f"), gearing );
            }
            RAdjText( &tmpStr, columnWidth );
            strArray[1] += tmpStr + _T(" ");

            tmpStr = GetUnitTypeDescription( m_graphData[graph].id );
            RAdjText( &tmpStr, columnWidth );
            strArray[2] += tmpStr + _T(" ");
	    }

        for( int i=0; i<3; i++ )
		{
			strArray[i] += EOL_FILE;
			m_LiveDataFile.Write( strArray[i], strArray[i].GetLength() );
		}
        strArray[0] = EOL_FILE;
        m_LiveDataFile.Write( strArray[0], strArray[0].GetLength() );

		m_LiveDataFileWriteCounter = 1;

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

void CGraphView::CloseLiveDataFile( void )
{
	if( m_LiveDataFile.m_hFile != CFile::hFileNull )
	{
		m_LiveDataFile.Close();
	}
}

void CGraphView::OnBnClickedLiveGraphStop()
{
	CString text;

	if( m_ShowLiveDataGraph == TRUE )
	{
        if( m_pSource != NULL )
        {
            m_pSource->EnableErrorEvent( FALSE );
        }
        else
        {
            DAU::GetDAU().EnableErrorEvent( FALSE );
        }
		m_ShowLiveDataGraph = FALSE;
		text.LoadString( IDS_RESTART );
		SetDlgItemText( IDC_LIVE_GRAPH_STOP, text );
		GetDlgItem( IDC_LIVE_GRAPH_PRINT )->EnableWindow( TRUE );
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            GetDlgItem( IDC_LIVE_GRAPH_SAVE )->EnableWindow( TRUE );
        }
        m_LiveDataGraphParam.startLiveData = FALSE;
	    FinishLiveDataGraphWithText( &m_LiveDataGraphParam );
    }
	else
	{    
        if( m_pSource != NULL )
        {
            m_pSource->EnableErrorEvent( TRUE );
        }
        else
        {
            DAU::GetDAU().EnableErrorEvent( TRUE );
        }
        m_ShowLiveDataGraph = TRUE;
        text.LoadString( IDS_STOP );
        SetDlgItemText( IDC_LIVE_GRAPH_STOP, text );
		GetDlgItem( IDC_LIVE_GRAPH_PRINT )->EnableWindow( FALSE );
        if( theApp.IsAligner202Enabled() == TRUE )
        {
            GetDlgItem( IDC_LIVE_GRAPH_SAVE )->EnableWindow( FALSE );
        }
		ClearMeasuredLiveGraphValues( FALSE );
        UpdateGraph();
	    //Get the timestamp to be stored in the graph and table
	    COleDateTime::GetCurrentTime().GetAsDBTIMESTAMP( m_LiveGraphStartTimeStamp );
	    m_LiveDataGraphParam.TimeStamp = GetTimeStampStr( &(m_LiveGraphStartTimeStamp), TRUE );
        m_LiveDataGraphParam.DateStamp = GetDateStampStr( &(m_LiveGraphStartTimeStamp) );
        m_LiveDataGraphParam.initGraph = TRUE;
        m_LiveDataGraphParam.startLiveData = TRUE;
	    ShowLiveDataGraphWithText( &m_LiveDataGraphParam );
	}
}

void CGraphView::OnActivateView( BOOL activate, CView* pActivateView, CView* pDeactiveView )
{
	if (activate)
	{
    if( ( theApp.IsAligner202Enabled() == TRUE ) && ( theApp.IsLiveGraphViewActivated() == FALSE ) )
    {
        theApp.SetLiveGraphViewActivated( TRUE );
		  SetSource( &(DAU::GetDAU()) ) ;
//      TRACE(_T("\n Activate graphview"));
    }
	}
	else
	{
//        TRACE(_T("\n DeActivate Graphview"));
	}
	__super::OnActivateView( activate, pActivateView, pDeactiveView ) ;
}

void CGraphView::AddChartLabel(CString labelName, CString text, int x, int y, AnchorConstants anchor, AttachMethodConstants method)
{
    m_pChart->GetChartLabels()->Add()->PutName(labelName.AllocSysString());
    m_pChart->GetChartLabels()->GetItem(labelName.AllocSysString())->GetText()->PutText( text.AllocSysString() );
    m_pChart->GetChartLabels()->GetItem(labelName.AllocSysString())->PutAnchor( anchor );
    m_pChart->GetChartLabels()->GetItem(labelName.AllocSysString())->PutAttachMethod( method );
    m_pChart->GetChartLabels()->GetItem(labelName.AllocSysString())->GetAttachCoord()->PutX( x );
    m_pChart->GetChartLabels()->GetItem(labelName.AllocSysString())->GetAttachCoord()->PutY( y );
}

BOOL CGraphView::ShowLiveDataGraphWithText( LiveDataGraphInParam* pInParam )
{
    BOOL result = TRUE;
	m_LastSavedGraphFileName = _T("");
    HideGraph();
	m_StoreLiveDataToFile = pInParam->StoreToFile;
	m_LiveDataCounterStopValue = pInParam->CounterStopValue;
	m_LiveDataGraphTs = pInParam->Ts;    

    //int i, j;
    int noOfEmptyRowsInHeader= 10;
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;
    int noOfEmptyRowsInBlock2Top = 1;
    int noOfEmptyRowsInBlock2Bottom = 0;
    int noOfEmptyRowsInBlock3Top = 1;
    int noOfEmptyRowsInBlock3Bottom = 1;
    int noOfEmptyRowsInBlock4Top = 1;
    int noOfEmptyRowsInBlock4Bottom = 0;
    int noOfEmptyRowsInBlock5Top = 1;
    int noOfEmptyRowsInBlock5Bottom = 0;
    long chartAndTextAreaOverlap = 0;
    //long legendTextXPosOffset = 55;
    long legendTextXPosOffset = 50;
    long legendTextYPosOffset = 6;
    long legendSymbolXPosOffset = 16;
    long noOfRowsInFooter = 6; //there must be space for the x-axis UnitType above the footer  
    long noOfSpaceCharacters = 1;
    long averageNoOfCharactersOnTextTilt = 6;
    long averageNoOfCharactersOnTextLabel = 10;
    long averageNoOfCharactersOnTextData = 15;

    int maxLength = max(g_AlignerData.Ship.GetLength(), g_AlignerData.Operator.GetLength());
    maxLength = max(maxLength, g_AlignerData.Place.GetLength());

    if( ((2*maxLength) / 3) > averageNoOfCharactersOnTextData )
    {
        averageNoOfCharactersOnTextData = (2*maxLength) / 3;
    }
    
    long textAreaWidth = (theApp.IsAligner308Enabled() ? 25 : 35) * MAX_WIDTH_OF_CHARACTER;
//     long averageNoOfCharactersOnLegendRow = MAX_NO_OF_CHAR_IN_LEGEND_LABEL + 
//         averageNoOfCharactersOnTextTilt + 3 * noOfSpaceCharacters;
//     long tmpL = averageNoOfCharactersOnLegendRow * MAX_WIDTH_OF_CHARACTER + 
//         legendTextXPosOffset - chartAndTextAreaOverlap;
// 
//     if( tmpL > textAreaWidth )
//     {
//         textAreaWidth = tmpL;
//     }

    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;

    long timeConstLabelYPos = noOfEmptyRowsInHeader * ROW_HEIGHT; /*+ ROW_HEIGHT / 2*/;
    long samplingRateLabelYPos = timeConstLabelYPos + ROW_HEIGHT;
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        samplingRateLabelYPos = timeConstLabelYPos;
    }
  
    long line2YPos = samplingRateLabelYPos + noOfEmptyRowsInBlock2Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    long stationLabelYPos = line2YPos + noOfEmptyRowsInBlock3Top * ROW_HEIGHT + ROW_HEIGHT / 2;
    long referenceLabelYPos = stationLabelYPos + ROW_HEIGHT;
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        referenceLabelYPos += ROW_HEIGHT;
    }
    long line3YPos = referenceLabelYPos + noOfEmptyRowsInBlock3Bottom * ROW_HEIGHT;
    long legendTopPos = line3YPos - legendTextYPosOffset + noOfEmptyRowsInBlock4Top * ROW_HEIGHT;
    long line4YPos = legendTopPos + ( noOfEmptyRowsInBlock4Bottom + m_NoOfSeries + m_NoOfSeriesY2 ) * ROW_HEIGHT;
    long tempLabelYPos = line4YPos + noOfEmptyRowsInBlock5Top * ROW_HEIGHT + ROW_HEIGHT / 2;
     
    int numSensors = 1; // one line for temperature label
    map<int, LiveGraphInfo>::iterator iter ;
    for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
    {
        if( IsSensor(iter->second.id) == TRUE )
        {
            numSensors++;
        }
    }

    long line5YPos = tempLabelYPos + ( noOfEmptyRowsInBlock5Bottom + numSensors ) * ROW_HEIGHT - ROW_HEIGHT / 2;
    long verticalLineStartYPos = line2YPos + ROW_HEIGHT;
    long verticalLineStopYPos = line4YPos;

    long legendLeftPos = m_ControlWidth - textAreaWidth;
    
    m_graphWndWidth = legendLeftPos;
    m_samplesPerPix = (double)g_AlignerData.RX / (double)m_graphWndWidth / pInParam->Ts;

    long textRefLabelXPos = legendLeftPos + legendSymbolXPosOffset;
    long legendTextLabelXPos = legendLeftPos + legendTextXPosOffset;
    long textLabelXPos = textRefLabelXPos;	
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;    
    long horizontalLineXPos = legendLeftPos + chartAndTextAreaOverlap;
    long verticalLine1XPos = m_ControlWidth - 7*MAX_WIDTH_OF_CHARACTER;//legendTextLabelXPos + (MAX_NO_OF_CHAR_IN_LEGEND_LABEL+MAX_NO_OF_EXTRA_CHAR_IN_LEGEND_LABEL) * MAX_WIDTH_OF_CHARACTER;
	long textTempUnitTypeLabelXPos = verticalLine1XPos + 3*MAX_WIDTH_OF_CHARACTER;
    long textTiltXPos = verticalLine1XPos + averageNoOfCharactersOnTextTilt * MAX_WIDTH_OF_CHARACTER;
    
    int noOfUnderscoresInTheLine = ( int )( textAreaWidth / WIDTH_OF_UNDERSCORE + 1 )-1;
    int noOfShortLineInTheLine = ( int )( textAreaWidth / WIDTH_OF_SHORT_LINE + 1 )-1;

    CString solidLine, dashedLine;
    char verticalLineCharacter[2] = {"|"};
    CString str, tmpStr;
	solidLine = _T("");
	for( int i=0; i<noOfUnderscoresInTheLine; i++ )
	{
		solidLine += _T("_");
	}
	dashedLine = _T("");
	for( int i=0; i<noOfShortLineInTheLine; i++ )
	{
		dashedLine += _T("-");
	}

    try
    {        
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutIsDefault( FALSE );
        m_pChart->GetChartArea()->GetLocation()->GetTop()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetHeight()->PutValue( m_ControlHeight );
        m_pChart->GetChartArea()->GetLocation()->GetLeft()->PutValue( 0 );
        m_pChart->GetChartArea()->GetLocation()->GetWidth()->PutValue( legendLeftPos + chartAndTextAreaOverlap );
        m_pChart->GetLegend()->GetLocation()->GetTop()->PutValue( legendTopPos );
        m_pChart->GetLegend()->GetLocation()->GetLeft()->PutValue( legendLeftPos );
        m_pChart->GetChartArea()->GetPlotArea()->GetBottom()->PutValue( noOfRowsInFooter * ROW_HEIGHT );

		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetPrecision()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "y" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );
		m_pChart->GetChartArea()->GetAxes()->GetItem( "x" )->GetMajorGrid()->GetSpacing()->PutIsDefault( TRUE );

        str = _T("");
        
        if( theApp.IsAligner308Enabled())
        {                
            str.LoadString((g_AlignerData.SignDef == -1) ? IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_UP : IDS_MEASURE_GRAPH_FOOTER_ROLL_PITCH_DOWN);
        }

        m_pChart->GetFooter()->GetText()->PutText( str.AllocSysString() );

        str.LoadString( theApp.IsAligner202Enabled() ? IDS_LIVE_DATA_GRAPH_A202 : IDS_LIVE_DATA_GRAPH);
     
        AddChartLabel("alignModeLabel", str, legendTextLabelXPos, alignModeLabelYPos);           
        
        AddCommonHeader(pInParam, legendLeftPos);//textLabelXPos);

        if( theApp.IsAligner308Enabled())
        {
            str.LoadString( IDS_TILT_POLAR_GRAPH_TIME_CONST_LABEL );
            AddChartLabel("timeConstLabel", str, textLabelXPos, timeConstLabelYPos);                    
                            
            tmpStr.LoadString( IDS_S_UNIT );
            str.Format( _T("%2.1f"), pInParam->TimeConstant );
            str += " " + tmpStr;
            
            AddChartLabel("timeConst", str, textDataXPos, timeConstLabelYPos);                                 
        }
     
        str.LoadString( IDS_SAMPLING_RATE_GRAPH_LABEL );
        AddChartLabel("samplingRateLabel", str, textLabelXPos, samplingRateLabelYPos);                                 
        
        tmpStr.LoadString( IDS_HZ_UNIT_GRAPH_LABEL );
        str.Format( _T("%2.1f"), (pInParam->Ts != 0) ? 1 / pInParam->Ts : 0.0f);            
        str += " " + tmpStr;
        AddChartLabel("samplingRate", str, textDataXPos, samplingRateLabelYPos);                                 
        
        AddChartLabel("line2", solidLine, horizontalLineXPos, line2YPos);      
        
        str.LoadString( theApp.IsAligner202Enabled() ? IDS_TILT_POLAR_GRAPH_CHANNELS_LABEL : IDS_TILT_POLAR_GRAPH_STATIONS_LABEL);            
        AddChartLabel("stationLabel", str, legendTextLabelXPos, stationLabelYPos);                  
        
        str.LoadString( theApp.IsAligner202Enabled() ? IDS_MEASURE_GRAPH_FINAL_VALUE1_LABEL : IDS_MEASURE_GRAPH_TILT_LABEL);            
        AddChartLabel("tiltLabel", str, textTiltXPos, stationLabelYPos, oc2dAnchorWest);                  
        
        if( theApp.IsAligner202Enabled())
        {
            str.LoadString( IDS_DEG_UNIT );
        }
        else
        {
            str.LoadString( (GetMRad() == TRUE) ? IDS_MRAD_UNIT : IDS_ARCMIN_UNIT );                
        }

        AddChartLabel("tiltUnitLabel", str, textTiltXPos, referenceLabelYPos, oc2dAnchorWest);                  
        
        AddChartLabel("line3", dashedLine, horizontalLineXPos, line3YPos);       
    
        CString UnitTypeDescription, digStr, axisStr, type;
        CString y1= "Y1", y2="Y2";
        int numChars = 48;

        int graphNum=1;
        map<int, LiveGraphInfo>::iterator iter ;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
        {            
         //   TRACE("Add graph:%s, axis:%d\n",UnitTypeDescription,iter->second.roll);
            if(iter->second.roll != Axis_None)
            {                                                           
                str = GetGraphText(iter->second, DATA_TYPE_ROLL);
                AddGraph( graphNum++, str.Left(numChars), iter->second.id, iter->second.roll, DATA_TYPE_ROLL);
            }
            if(iter->second.pitch != Axis_None)
            {                
                str = GetGraphText(iter->second, DATA_TYPE_PITCH);
                AddGraph( graphNum++, str.Left(numChars), iter->second.id, iter->second.pitch, DATA_TYPE_PITCH);
            }
            if(iter->second.heading != Axis_None)
            {             
                str = GetGraphText(iter->second, DATA_TYPE_HEADING);
                AddGraph( graphNum++, str.Left(numChars), iter->second.id, iter->second.heading, DATA_TYPE_HEADING);
            }
        }
        SortGraphs();
        UpdateGraphLabels();

        AddChartLabel("line4", solidLine, horizontalLineXPos, line4YPos);           

        UpdateCurrentValues();
        for( int i=1; i<=m_totalNoSeries; i++ )
        {
            str.Format("");// = _T("e");
            tmpStr.Format( _T("Graph%d"), i );
            AddChartLabel(tmpStr, str, textTiltXPos, line3YPos + i * ROW_HEIGHT, oc2dAnchorWest);                
        }

        int hasTempr = 0;
        for(iter=g_AlignerData.liveGraphMap.begin(); iter!=g_AlignerData.liveGraphMap.end(); iter++)
        {    
            if( IsSensor( iter->second.id ))
            {
                hasTempr++;
                str = GetUnitTypeDescription( iter->second.id ).Left( MAX_NO_OF_CHAR_IN_LEGEND_LABEL );
                tmpStr.Format( _T("tempCh%d"), hasTempr );
                AddChartLabel(tmpStr, str, textLabelXPos, tempLabelYPos + hasTempr * ROW_HEIGHT);                
                                    
                str.Format( _T("%2.1f"), GetTemperature( iter->second.id, m_currentValues ) );
                tmpStr.Format( _T("temp%d"), hasTempr );
                AddChartLabel(tmpStr, str, textTempUnitTypeLabelXPos, tempLabelYPos + hasTempr * ROW_HEIGHT);                                
            }
        }
        if( hasTempr > 0 )
        {
            str.LoadString( IDS_START_TEMPERATURE_LABEL );
            AddChartLabel("temperatureLabel", str, textLabelXPos, tempLabelYPos);                                                

            str.LoadString( IDS_GRAD_CELCIUS_UNIT );
            AddChartLabel("temperatureUnitlabel", str, textTempUnitTypeLabelXPos, tempLabelYPos);

            AddChartLabel("line5", solidLine, horizontalLineXPos, line5YPos);              
        }
            
        int line = 1;
        for( int y=verticalLineStartYPos; y<=verticalLineStopYPos; y+=ROW_HEIGHT )
        {
            tmpStr.Format( _T("vLine1YPos%d"), line );
            AddChartLabel(tmpStr, verticalLineCharacter, verticalLine1XPos + 12, y);                             
            line++;
        }      
    }
    catch ( _com_error e )
    {
        result = FALSE;
    }

    if( result == TRUE  )
    {
	    if( m_StoreLiveDataToFile == TRUE )
		{
            if(theApp.IsAligner202Enabled() == TRUE )
            {
			    WriteLiveDataFileHeaderA202( pInParam );
            }
            else
            {
                WriteLiveDataFileHeader( pInParam );
            }
		}

        m_LiveDataCounter = 0;

		ClearMeasuredLiveGraphValues( FALSE );
        GetDlgItem( IDC_LIVE_GRAPH_PRINT )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_LIVE_GRAPH_PRINT )->EnableWindow( FALSE );

        if( theApp.IsAligner202Enabled() == TRUE )
        {
            GetDlgItem( IDC_LIVE_GRAPH_SAVE )->ShowWindow( SW_SHOW );
            GetDlgItem( IDC_LIVE_GRAPH_SAVE )->EnableWindow( FALSE );
        }
        GetDlgItem( IDC_LIVE_GRAPH_QUIT )->ShowWindow( SW_SHOW );
		CString text;
        text.LoadString( IDS_STOP );
        SetDlgItemText( IDC_LIVE_GRAPH_STOP, text );
		GetDlgItem( IDC_LIVE_GRAPH_STOP )->ShowWindow( SW_SHOW );
        GetDlgItem( IDC_LIVE_GRAPH_STOP )->EnableWindow( TRUE );
        //ShowGraph();
    		
		m_ShowLiveDataGraph = TRUE;        
    }

    if( result == TRUE )
    {
        ShowGraph();
    }

    return( result );
}


BOOL CGraphView::FinishLiveDataGraphWithText( LiveDataGraphInParam* pInParam )
{
    UpdateCurrentValues();
    
    CString str, tmpStr;
   
    if( UpdateTemprature() > 0 )
    {
        int id = (pInParam->startLiveData == TRUE ) ?  IDS_START_TEMPERATURE_LABEL : IDS_FINAL_TEMPERATURE_LABEL;
        str.LoadString( id );

        m_pChart->GetChartLabels()->GetItem( "temperatureLabel" )->GetText()->PutText( str.AllocSysString() );
    }

    double val=0;          
    for(int graph=1 ; graph<=m_totalNoSeries ; graph++)
    {
        str = _T("");
        if(!pInParam->startLiveData)
        {
            val = (GetMRad() == TRUE) ? m_graphData[graph].m_previousLiveGraphValue : MRADIANS_TO_ARCMIN( m_graphData[graph].m_previousLiveGraphValue);
            str.Format( _T("%2.2f"), val );                                                           
        }
        tmpStr.Format( _T("Graph%d"), graph );
        m_pChart->GetChartLabels()->GetItem( tmpStr.AllocSysString() )->GetText()->PutText( str.AllocSysString() );
    }
    return 0;
}

CString CGraphView::GetAxisText(LiveGraphInfo& info, int dataType)
{
    CString y1= "Y1", y2="Y2", str="";
    switch(dataType)
    {
        case DATA_TYPE_ROLL: str = (info.roll == Axis_Y1) ? y1 : y2 ; break;
        case DATA_TYPE_PITCH: str = (info.pitch == Axis_Y1) ? y1 : y2; break;
        case DATA_TYPE_HEADING: str = (info.heading == Axis_Y1) ? y1 : y2; break;       
        default:break;
    }
    return str;
}

CString CGraphView::GetGraphText(LiveGraphInfo& info, int dataType)
{
    CString str;

    CString UnitTypeDescription = (GetUnitTypeDescription( info.id ));

    if(theApp.IsAligner202Enabled())
    {
      
        str.Format("%s: %s, %s%s, %s",GetAxisText(info, dataType), info.name, info.type, info.dig ? " - " + m_dataTypeText[dataType] : "", UnitTypeDescription );                        
    }
    else
    {        
        str.Format("%s: %s, %s",m_dataTypeText[dataType], info.name, UnitTypeDescription );                            
    }


    return str;
}


void CGraphView::ClearMeasuredLiveGraphValues( BOOL clearTemp )
{
    for( int i=0; i<m_graphData.size(); i++ )
    {
        m_graphData[i].m_maxLiveGraphValue = MAX_LIVE_GRAPH_RESET_VALUE;
        m_graphData[i].m_minLiveGraphValue = MIN_LIVE_GRAPH_RESET_VALUE;
        m_graphData[i].m_previousLiveGraphValue = 0;

        if( clearTemp == TRUE )
        {
            m_PreviousLiveGraphTemperature[i] = 0;
        }
    }
}

void CGraphView::InitResultTable( void )
{
	m_pLiveGraphResultTable->m_InParam.Version = LIVE_GRAPH;
	//m_pLiveGraphResultTable->m_InParam.Title.LoadString( IDS_LIVE_DATA_GRAPH );
	m_pLiveGraphResultTable->m_InParam.Time = m_LiveGraphStartTimeStamp;
	m_pLiveGraphResultTable->m_InParam.AngleRange0ToPlusMinus180 = TRUE;
	m_pLiveGraphResultTable->m_InParam.SignDef = g_AlignerData.SignDef;
	m_pLiveGraphResultTable->m_InParam.pACA = NULL;
	m_pLiveGraphResultTable->m_InParam.Comment = DB_EMPTY_STRING;
	m_pLiveGraphResultTable->m_InParam.ShowPresenetDialog = TRUE;
	m_pLiveGraphResultTable->m_InParam.SavedToDataBase = FALSE;
	m_pLiveGraphResultTable->m_InParam.pTemperature = m_PreviousLiveGraphTemperature;
    m_pLiveGraphResultTable->m_InParam.timeConstant = m_LiveDataGraphParam.TimeConstant;
//     m_pLiveGraphResultTable->m_InParam.graphFileName = m_LiveDataGraphParam.graphFileName;
//     m_pLiveGraphResultTable->m_InParam.includeGraph = m_LiveDataGraphParam.includeGraph;
    m_pLiveGraphResultTable->m_InParam.pFileGraphManager = &m_LiveGraphFileManager;


	if( m_LiveDataGraphTs != 0 )
	{
		m_pLiveGraphResultTable->m_InParam.SamplingRate = 1 / m_LiveDataGraphTs;
	}
	else
	{
		m_pLiveGraphResultTable->m_InParam.SamplingRate = 0;
	}
}

void CGraphView::ExitResultTable( BOOL DeleteReport )
{
/*	if( m_pLiveGraphResultTable != NULL )
	{
    //    m_pLiveGraphResultTable->CloseReport();

		if( DeleteReport == TRUE )
        {
            theApp.RemoveFromDocument( m_pLiveGraphResultTable -> m_reportHistoryMainID ) ;
            m_pLiveGraphResultTable->DeleteLast();
        }
        else
        {
            theApp.AddToDocument( m_pLiveGraphResultTable->m_reportHistoryMainID ) ;
        }

		delete m_pLiveGraphResultTable;
		m_pLiveGraphResultTable = NULL;
    }*/
}

/*BOOL CGraphView::AddGraphFileNameToDataBase( void )
{
	BOOL result = TRUE;

	for( int i = 0; i < m_LiveGraphFileManager.GetNoOfSavedFiles(); i++ )
	{
        if( m_LiveGraphFileManager.GetIncludeToResultTable( i ) == TRUE )
        {
		    if( TRUE != m_pLiveGraphResultTable->AddGraph( m_LiveGraphFileManager.GetFileName( i ), m_LiveGraphFileManager.GetIncludeToResultTable( i ) ) )
		    {
			    result = FALSE;
		    }
        }
        else
        {
            MoveFileToTemporaryDirectory( m_LiveGraphFileManager.GetFileName( i ) ) ;
        }
	}
	return( result );
}*/

void CGraphView::AddCommonHeader( GraphInParam* pInParam, int leftXPos )
{
    int averageNoOfCharactersOnTextLabel = 10;
    long textLabelXPos = leftXPos + 16;
    long textDataXPos = textLabelXPos + averageNoOfCharactersOnTextLabel * MAX_WIDTH_OF_CHARACTER;
    
    int noOfEmptyRowsInBlock1Top = 1;
    int noOfEmptyRowsInBlock1Bottom = 0;

    long alignModeLabelYPos = noOfEmptyRowsInBlock1Top * ROW_HEIGHT;
    long projectLabelYPos = alignModeLabelYPos + 2 * ROW_HEIGHT;
    long shipLabelYPos = projectLabelYPos + ROW_HEIGHT;
    long dateLabelYPos = shipLabelYPos + ROW_HEIGHT;
    long timeLabelYPos = dateLabelYPos + ROW_HEIGHT;
    long operatorLabelYPos = timeLabelYPos + ROW_HEIGHT;
    long placeLabelYPos = operatorLabelYPos + ROW_HEIGHT;
    long line1YPos = placeLabelYPos + noOfEmptyRowsInBlock1Bottom * ROW_HEIGHT + ROW_HEIGHT / 2;
    
    int noOfUnderscoresInTheLine = ( int )( (m_ControlWidth-leftXPos) / WIDTH_OF_UNDERSCORE + 1 )-1;    

    CString solidLine;
    solidLine = _T("");
    for( int i=0; i<noOfUnderscoresInTheLine; i++ )
    {
        solidLine += _T("_");
    }

    CString str;
    str.LoadString( IDS_GRAPH_PROJECT_LABEL );
    AddChartLabel("projectLabel", str, textLabelXPos, projectLabelYPos);           

    str = SysSetup->GetProjectName().Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
    AddChartLabel("projectName", str, textDataXPos, projectLabelYPos);           

    str.LoadString( theApp.IsAligner202Enabled() ? IDS_TILT_POLAR_GRAPH_SHIP_LABEL_A202 : IDS_TILT_POLAR_GRAPH_SHIP_LABEL);

    m_pChart->GetChartLabels()->Add()->PutName( "shipLabel" );
    AddChartLabel("shipLabel", str, textLabelXPos, shipLabelYPos);           

    m_pChart->GetChartLabels()->Add()->PutName( "shipName" );
    str = g_AlignerData.Ship.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
    AddChartLabel("shipName", str, textDataXPos, shipLabelYPos);           

    str.LoadString( IDS_TILT_POLAR_GRAPH_DATE_LABEL );
    AddChartLabel("dateLabel", str, textLabelXPos, dateLabelYPos);           

    AddChartLabel("date", pInParam->DateStamp, textDataXPos, dateLabelYPos);           

    str.LoadString( IDS_START_TIME_LABEL );
    AddChartLabel("timeLabel", str, textLabelXPos, timeLabelYPos);           

    AddChartLabel("time", pInParam->TimeStamp, textDataXPos, timeLabelYPos);           

    str.LoadString( IDS_TILT_POLAR_GRAPH_OPERATOR_LABEL );
    AddChartLabel("operatorLabel", str, textLabelXPos, operatorLabelYPos);           

    str = g_AlignerData.Operator.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );
    AddChartLabel("operator", str, textDataXPos, operatorLabelYPos);           

    str.LoadString( IDS_TILT_POLAR_GRAPH_PLACE_LABEL );
    AddChartLabel("placeLabel", str, textLabelXPos, placeLabelYPos);            

    str = g_AlignerData.Place.Left( MAX_NO_OF_CHAR_IN_TEXT_DATA );                       
    AddChartLabel("place", str, textDataXPos, placeLabelYPos);                    

    //Line      
    AddChartLabel("line1", solidLine, leftXPos, line1YPos);      
}