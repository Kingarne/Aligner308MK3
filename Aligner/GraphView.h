// $Id: GraphView.h,v 1.9 2014-07-11 16:30:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "DataRecepient.h"
#include "GlobalData.h"
#include "GraphFileManager.h"

class CResultTable ;

#define DEFAULT_LIVE_DATA_GRAPH_FILE_EXT		_T(".emf")

#define MAX_POLAR_GRAPH_RESOLUTION  1.0 //[mrad] or [arcmin]

#define MIN_NO_OF_SERIES            1
#define MAX_NO_OF_SERIES            20
#define MAX_NO_OF_SAMPLES_IN_GRAPH  20000

#define MAX_NO_OF_CHAR_IN_LEGEND_LABEL 22 //user input, g_AlignerData.Device[g_AlignerData.ObjNo[i]],  will be limited to this number
#define MAX_NO_OF_EXTRA_CHAR_IN_LEGEND_LABEL	2 // this number of extra characters can be added to the limited user input, (length of ": R" ~ length of "WW")
#define MAX_NO_OF_CHAR_IN_TEXT_DATA 30 //user input, g_AlignerData.Ship, .Operator and .Place,  will be limited to this number

// experimental values for the default font ( Tahoma 8 )
#define MAX_WIDTH_OF_CHARACTER      10 //W
#define WIDTH_OF_UNDERSCORE         6 //_
#define WIDTH_OF_SHORT_LINE         4 //-
#define ROW_HEIGHT                  13

#define MAX_LIVE_GRAPH_RESET_VALUE   -DBL_MAX
#define MIN_LIVE_GRAPH_RESET_VALUE   DBL_MAX

#define SHOW_TIME_AXIS_IN_HOURS     10000		//[s]
#define SHOW_TIME_AXIS_IN_MINUTES   1000		//[s]
#define SHOW_TIME_AXIS_IN_SECONDS   0				//[s]

#define DATA_TYPE_NONE      0
#define DATA_TYPE_ROLL      1
#define DATA_TYPE_PITCH     2
#define DATA_TYPE_HEADING   3
#define DATA_TYPE_TEMPR     4

struct GraphData
{
    GraphData():x(0.0f),y(0.0f),axis(Axis_None),valid(0),label(""),dataType(DATA_TYPE_NONE),graphNum(0),id(0){}
    double x;
    double y;
    int axis;
    int valid;
    CString label;
    int dataType;
    int graphNum;
    int id;    

    double m_previousLiveGraphValue;
    double m_maxLiveGraphValue;
    double m_minLiveGraphValue;
};

struct GraphInParam
{
    CString TimeStamp;
    CString DateStamp;
	BOOL ContinousGraph;
};

struct BarGraphInParam : public GraphInParam
{    
};

struct MeasureGraphInParam : public GraphInParam
{
    BOOL writeTiltData;
    BOOL useElev;
    ModeT mode;
    double timeConstant;
    BOOL* pChCon1;
    double* pTempT;
    int RefNo;
    BOOL commonFlatTest;
};

struct TiltFlatnessTestGraphInParam : public GraphInParam
{
    PLModeT PlMode;
    ModeT mode;
    int RefNo;
    double timeConstant;
    int Points;   
};

struct TiltFlatnessFoTestGraphInParam : public GraphInParam
{
    PLModeT PlMode;
    ModeT mode;
    int RefNo;
    double timeConstant;
    int Points;   
	int IndexArmLength ;
};	// Fo : Foundation

struct PolarGraphInParam : public GraphInParam
{
    ModeT mode;
    double tao;
    int RefNo;   
    BOOL commonFlatTest;
};

struct LiveDataGraphInParam : public GraphInParam
{   
	BOOL StoreToFile;
	long CounterStopValue;	
	CString FileName;
	double TimeConstant;
	double Ts;
    BOOL initGraph;
    BOOL startLiveData;
    CString graphFileName;  
    BOOL includeGraph;
};

class CGraphView : public CFormView, public DataRecepient
{
public:
	DECLARE_DYNCREATE(CGraphView)
	DECLARE_MESSAGE_MAP()
public:
	CGraphView( void ) ;
	virtual ~CGraphView( void ) ;

	enum { IDD = IDD_LIVE_GRAPH_FORM } ;

	BOOL UpdateSourceBarData( void );
    BOOL UpdateLiveDataGraph( void );
    void ClearLiveGraph( void );
    void PrintLiveGraph( void );
    OLE_COLOR GetColor( int i );
   ShapeConstants GetPointShape( int i );
   LinePatternConstants GetLinePattern( int i );
    BOOL SetGraphType( enum ChartTypeConstants graphType, BOOL update = TRUE );
    BOOL SetUseCommonXValues( BOOL useCommonXValues, BOOL update = TRUE );
    BOOL SetNoOfSeries( int noOfSeries, BOOL update = TRUE );
    int GetNoOfSeries( void );
    BOOL SetOriginBase( double originBase, BOOL update = TRUE );
    BOOL ReverseAxes( BOOL reverseAxes, BOOL update = TRUE);
    BOOL SetAxisXMin( double axisXMin, BOOL update = TRUE );
    BOOL SetAxisXMax( double axisXMax, BOOL update = TRUE );
    BOOL SetAxisYMin( double axisYMin, BOOL update = TRUE );
    BOOL SetAxisYMax( double axisYMax, BOOL update = TRUE );
    BOOL SetAxisXMinAutoScaleOn( BOOL axisXMinAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisXMaxAutoScaleOn( BOOL axisXMaxAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisYMinAutoScaleOn( BOOL axisYMinAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisYMaxAutoScaleOn( BOOL axisYMaxAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisXUnit( CString* axisXUnit, BOOL update = TRUE );
    BOOL SetAxisYUnit( CString* axisYUnit, BOOL update = TRUE );
    BOOL SetPolarRange0ToPlusMinus180( BOOL polarRange0ToPlusMinus180, BOOL update = TRUE );
	BOOL GetPolarRange0ToPlusMinus180( void ) const;
    BOOL SetControlBorder( enum BorderTypeConstants controlBorder, BOOL update = TRUE );
    BOOL SetLegendBorder( enum BorderTypeConstants legendBorder, BOOL update = TRUE );
    BOOL ShowLegend( BOOL showLegend, BOOL update = TRUE );
    BOOL SetChartBorder( enum BorderTypeConstants chartBorder, BOOL update = TRUE );
    BOOL SetChartHeader( CString chartHeader, BOOL update = TRUE );
    BOOL SetChartFooter( CString chartFooter, BOOL update = TRUE );
    CString GetChartFooter( void );
    long GetControlWidth( void );
    long GetControlHeight( void );

    BOOL SetXYData( int serie, double xData, double yData );
    BOOL SetLiveXYData( int serie, double xData, double yData, BOOL update=TRUE );
    BOOL SetPointShape( int serie, ShapeConstants pointShape, BOOL update = TRUE );
    BOOL SetPointSize( int serie, int pointSize, BOOL update = TRUE );
    BOOL SetPointColor( int serie, OLE_COLOR pointColor, BOOL update = TRUE );
    BOOL SetLinePattern( int serie, LinePatternConstants linePattern, BOOL update = TRUE );
    BOOL SetLineColor( int serie, OLE_COLOR lineColor, BOOL update = TRUE );    
    BOOL SetSerieLabel( int serie, CString serieLabel, int axis=Axis_Y1);
    BOOL AddGraph( int serie, CString serieLabel, int id, int axis=Axis_Y1, int type=DATA_TYPE_ROLL );
    CString GetGraphText(LiveGraphInfo& info, int dataType);
    CString GetAxisText(LiveGraphInfo& info, int dataType);
    double GetDataValue(GraphData &data);
    BOOL UpdateGraphLabels();
    void SortGraphs();
    long GetNoOfPoints( int serie );
    BOOL ReSetAxisXUnitAndMax( CString* axisXUnit, double axisXMax );
	void SetAxisXUnitAndMax( double axisXMax );
	double GetTimeValue( long sampleCounter, double sampleTime );
	void SetReductionFactorForGraphData( long totalNumberOfSamples, long reducedNumberOfSamples );
	BOOL TimeToUpdate( long sampleCount );
  
	BOOL InitCalibrationGraph( int noOfSeries, int anothernameforthisvariable );
    BOOL InitDefaultBarGraph( int noOfSeries, BOOL useRoll = TRUE );
    BOOL InitDefaultPlotGraph( int noOfSeries );
    BOOL InitDefaultPolarGraph( int noOfSeries );
    BOOL InitDefaultLiveDataGraph( int noOfSeries, int noOfSeriesY2 = 0 );
    
    BOOL ShowPolarGraphWithText( PolarGraphInParam* pInParam );
    BOOL ShowMeasureGraphWithText( MeasureGraphInParam* pInParam );
    BOOL ShowTiltFlatnessTestGraphWithText( TiltFlatnessTestGraphInParam* pInParam );
    BOOL ShowTiltFlatnessFoTestGraphWithText( TiltFlatnessFoTestGraphInParam* pInParam );
    BOOL ShowBarGraphWithText( BarGraphInParam* pInParam );
    BOOL ShowLiveDataGraphWithText( LiveDataGraphInParam* pInParam );
	
    void AddCommonHeader( GraphInParam* pInParam, int startX );
    void ClearMeasuredLiveGraphValues( BOOL clearTemp );
    void ShowGraph( void );
    void HideGraph( void );
    BOOL UpdateData( BOOL shiftData) ;
    BOOL UpdateLiveData( void ) ;
    BOOL UpdateGraph( void );
    BOOL SaveGraphToUniqueFileName( CString &fileName, bool resultImage=false);
    BOOL SaveGraphToFile( CString &fileName );
	BOOL WriteLiveDataFileHeader( LiveDataGraphInParam* pInParam );
	void CloseLiveDataFile( void );
    void InitResultTable( void );
	void ExitResultTable( BOOL DeleteReport);
	//BOOL AddGraphFileNameToDataBase( void );
	void CleanUp( void );
    virtual void OnInitialUpdate() ;
    void AddChartLabel(CString labelName, CString text, int x, int y, AnchorConstants anchor=oc2dAnchorEast, AttachMethodConstants method=oc2dAttachCoord);
    BOOL FinishLiveDataGraphWithText( LiveDataGraphInParam* pInParam );

    BOOL UpdateTemprature();
// Y2 Axis for A202
public:
    BOOL SetAxisY2Min( double axisY2Min, BOOL update = TRUE );
    BOOL SetAxisY2Max( double axisY2Max, BOOL update = TRUE );
    BOOL SetAxisY2MinAutoScaleOn( BOOL axisY2MinAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisY2MaxAutoScaleOn( BOOL axisY2MaxAutoScaleOn, BOOL update = TRUE );
    BOOL SetAxisY2Unit( CString* axisY2Unit, BOOL update = TRUE );
  
    BOOL WriteLiveDataFileHeaderA202( LiveDataGraphInParam* pInParam );
	BOOL IsAxisY2Enabled( void ) const ;
  void SetAxisY2Enabled( BOOL axisY2Enabled = TRUE );

protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;

public:
    CWnd m_chart ;
    IChart2D_DualPtr m_pChart ;
    
    IChartGroup_DualPtr m_pGroup ;
    IChartStyleCollection_DualPtr m_pStyleCollection ;
    IChart2DData_DualPtr m_pData ;
    
    IChartGroup_DualPtr m_pGroupY2 ;
    IChartStyleCollection_DualPtr m_pStyleCollectionY2 ;
    IChart2DData_DualPtr m_pDataY2 ;
	 
	CString m_LastSavedGraphFileName ;
    LiveDataGraphInParam m_LiveDataGraphParam;
    CResultTable* m_pLiveGraphResultTable;
	DBTIMESTAMP m_LiveGraphStartTimeStamp;
    GraphFileManager m_LiveGraphFileManager;
private:
    BOOL m_firstUpdate ;    
    BOOL m_UseCommonXValues;//the common x-value is taken from the serie 1
    enum ChartTypeConstants m_GraphType;
    double m_OriginBase;
    BOOL m_AxesReversed;
    double m_AxisXMin;
    double m_AxisXMax;
    double m_AxisYMin;
    double m_AxisYMax;
    BOOL m_AxisXMinAutoScaleOn;
    BOOL m_AxisXMaxAutoScaleOn;
    BOOL m_AxisYMinAutoScaleOn;
    BOOL m_AxisYMaxAutoScaleOn;
    CString m_AxisXUnit;
    CString m_AxisYUnit;
    BOOL m_PolarRange0ToPlusMinus180;
    enum BorderTypeConstants m_ControlBorder;
    enum BorderTypeConstants m_LegendBorder;
    BOOL m_ShowLegend;
    enum BorderTypeConstants m_ChartBorder;
    CString m_ChartHeader;
    CString m_ChartFooter;
    long m_ControlWidth;
    long m_ControlHeight;
    
    
    vector<GraphData> m_graphData;//[MAX_NO_OF_SERIES+1];
    //GraphData m_graphData[MAX_NO_OF_SERIES+1];
    
    //BOOL m_ValidGraphDataToBeUpdated[MAX_NO_OF_SERIES+1];    //index [0] not used, serie 1 in index 1
    //double m_GraphDataX[MAX_NO_OF_SERIES+1];                 //index [0] not used, serie 1 in index 1
    //double m_GraphDataY[MAX_NO_OF_SERIES+1];                 //index [0] not used, serie 1 in index 1
    //CString m_SerieLabels[MAX_NO_OF_SERIES+1];               //index [0] not used, serie 1 in index 1

    ShapeConstants m_PointShapes[MAX_NO_OF_SERIES+1];        //index [0] not used, serie 1 in index 1
    int m_PointSizes[MAX_NO_OF_SERIES+1];                    //index [0] not used, serie 1 in index 1
    OLE_COLOR m_PointColors[MAX_NO_OF_SERIES+1];             //index [0] not used, serie 1 in index 1
    LinePatternConstants m_LinePatterns[MAX_NO_OF_SERIES+1]; //index [0] not used, serie 1 in index 1
    OLE_COLOR m_LineColors[MAX_NO_OF_SERIES+1];              //index [0] not used, serie 1 in index 1
    map<int, CString> m_dataTypeText;
	
    BOOL m_ShowBarGraph ;
    BOOL m_barGraphUseRoll ;
	BarGraphInParam m_BarGraphParam ;
    int m_ClusterWidthForOneBar ;
	int m_TimeUnit ;
	int m_ReductionFactorForGraphData ;
    BOOL m_ShowLiveDataGraph ;
    long m_LiveDataCounter ;
	long m_LiveDataFileWriteCounter ;
	long m_LiveDataCounterStopValue;
	BOOL m_continousGraph;
    
    double m_PreviousLiveGraphTemperature[SIZE_OF_ARRAYS] ;
	
    BOOL m_StoreLiveDataToFile ;
	CFile m_LiveDataFile ;
	double m_LiveDataGraphTs ;
    static CString m_GraphFileName;
	void TraceData(int ser, bool onlyY=false);
  // Y2 Axis for A202
public:
    static CString m_GraphFileDir;
    
private:
    int m_NoOfSeries;
    int m_NoOfSeriesY2;
    int m_totalNoSeries;
    double m_AxisY2Min;
    double m_AxisY2Max;
    BOOL m_AxisY2MinAutoScaleOn;
    BOOL m_AxisY2MaxAutoScaleOn;
    CString m_AxisY2Unit;
    BOOL m_AxisY2Enabled;
    BOOL m_exitWithoutSave;

    int m_graphWndWidth;
    double m_samplesPerPix;

protected:
    afx_msg void OnBnClickedLiveGraphPrint() ;
    afx_msg void OnBnClickedLiveGraphSave() ;
    afx_msg void OnDestroy();
	afx_msg void OnBnClickedLiveGraphStop() ;
    afx_msg LRESULT OnNewData( WPARAM, LPARAM ) ;
    afx_msg LRESULT OnDauError( WPARAM, LPARAM );
	afx_msg void OnBnClickedLiveGraphQuit() ;
	
#ifdef _DEBUG
	virtual void AssertValid( void ) const ;
	virtual void Dump( CDumpContext &dc ) const ;
#endif
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
} ;

inline
BOOL CGraphView::GetPolarRange0ToPlusMinus180( void ) const
{
	return m_PolarRange0ToPlusMinus180;
}
