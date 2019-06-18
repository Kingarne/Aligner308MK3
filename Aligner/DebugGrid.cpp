#include "StdAfx.h"
#include "DebugGrid.h"
#include <fstream>
#include "dau.h"
#include "SystemSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DebugGrid::DebugGrid(void):m_rows(8)
{
    m_initialized = FALSE;
}

DebugGrid::~DebugGrid(void)
{
}

void DebugGrid::Init()
{
    if(m_initialized)
        return;
    
    vector<DebugColumn> columns;
	columns.push_back(DebugColumn("Ch",20));
    columns.push_back(DebugColumn("OverrangeDetection",25));
    columns.push_back(DebugColumn("OverrangeDetectionActive",25));
    columns.push_back(DebugColumn("Selected",25));    
    columns.push_back(DebugColumn("Ro.",40));
    columns.push_back(DebugColumn("Pi.",40));
    columns.push_back(DebugColumn("Az",30));
    columns.push_back(DebugColumn("AzB",30));
    columns.push_back(DebugColumn("eal",30));
    //columns.push_back(DebugColumn("ealS",40));
    columns.push_back(DebugColumn("ealAbs",30));
    columns.push_back(DebugColumn("eac",40));
    columns.push_back(DebugColumn("zPar",40));
    columns.push_back(DebugColumn("Exist/use",35));
    columns.push_back(DebugColumn("Mode",30));

	m_cols = columns.size();

 	SetFixedColumnCount(m_cols);
 	SetFixedRowCount(m_rows);
	SetHeaderSort(FALSE);
	SetRowResize(FALSE);
	SetColumnResize(FALSE);
 	SetRowHeight(0, 20);
 	SetColumnWidth(0, 50);
	// 	SetRowHeight(1, 30);
	SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

	SetFixedTextColor(RGB(0,70,213));
	//ExpandColumnsToFit();   // This will remove any existing horz scrollbar
	ExpandToFit(FALSE);
	//ExpandLastColumn();
	LOGFONT lf = *GetItemFont(0, 0);
	//lf.lfWidth = 8;
	lf.lfHeight = 12;
	lf.lfWeight = FW_NORMAL;
	strcpy(lf.lfFaceName, "Tahoma");

	vector<DebugColumn>::iterator iter;
	int c=0;
	for(iter=columns.begin();iter!=columns.end();iter++,c++)
	{
		SetItemFormat(0, c, DT_CENTER|DT_VCENTER);
		SetItemText(0, c, iter->header);
		SetItemFont(0, c, &lf);
		SetColumnWidth(c, iter->width);	
		
		for(int r = 1; r < m_rows; r++)
		{
			SetItemFormat(r, c, DT_CENTER|DT_VCENTER);
            SetRowHeight(r, 17);
			SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
			SetItemFont(r, c, &lf);
		}
	}
	//ExpandLastColumn();
	
	m_initialized = TRUE;
}

void DebugGrid::SetCell( int nRow, int nCol, int nVal )
{
	ASSERT(nRow >= 1 && nRow < m_rows);
	ASSERT(nCol >= 3 && nCol < m_cols);	//col 3 is OK col

	CString str;
	str.Format("%d", nVal);
	SetItemText(nRow, nCol, (LPCTSTR)str);
	Invalidate();
}

void DebugGrid::SetRow( int nRow, const vector<CString> &items )
{
	ASSERT(items.size() == m_cols);
	ASSERT(nRow > 0 && nRow < m_rows);

	int nCol = 0;
	vector<CString>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it, nCol++)
		SetItemText(nRow, nCol, (LPCTSTR)*it);
	Invalidate();
}

void DebugGrid::Clear()
{
	for(int i = 1; i < m_rows; i++)
		for(int j = 0; j < m_cols; j++)
			SetItemText(i, j, "");
	Invalidate();
}

void DebugGrid::Reset()
{	
	UpdateGrid();	
}


void DebugGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void DebugGrid::UpdateHeader()
{
   

}

void DebugGrid::UpdateGrid()
{	
    Clear();
    
    UpdateHeader();
    
    map<CString, int> chIndexMap;
    chIndexMap[J3] = 6;
    chIndexMap[J4] = 5;
    chIndexMap[J5] = 4;
    chIndexMap[J6] = 3;
    chIndexMap[J7] = 2;
    chIndexMap[J8] = 1;
    chIndexMap[J9] = 0;

    for( int i = 0; i < min( 7, DAU::GetDAU().GetSensorCount() ); i++ )
    {
        int pos;
        CString name;
        Sensor *pSensor = DAU::GetDAU().GetSensor( i );
        name = pSensor->GetName();
        pos = chIndexMap[name];
        int row = i+1;

        SetRowBGColor(row, (pSensor->GetType() != UnitType::Unused) ? USED_COLOR : UNUSED_COLOR);
        
        SetItemText(row, DColCh, name);
        CString text;

        text.Format("%d",pSensor->GetOverrangeDetection());
        SetItemText(row, DColOD, text);        

        text.Format("%d",pSensor->GetOverrangeDetectionActive());
        SetItemText(row, DColODA, text);        

        text.Format("%d",pSensor->IsSelected());
        SetItemText(row, DColSelect, text);        
        
        double roll = -g_AlignerData.SignDef * pSensor->GetRoll() * 1000.0f;
        text.Format( "%+7.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? roll : MRADIANS_TO_ARCMIN(roll)) ;        
        SetItemText(row, DColRo, text);        
        
        double pitch = -g_AlignerData.SignDef * pSensor->GetPitch() * 1000.0f;
        text.Format( "%+7.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? pitch : MRADIANS_TO_ARCMIN(pitch)) ;
        SetItemText(row, DColPi, text);        
    
        text.Format("%.2f",pSensor->GetCurrentAz());
        SetItemText(row, DColCurrAz, text);        
        
        text.Format("%.2f",pSensor->GetRotateBackAng());
        SetItemText(row, DColCurrAzBack, text);   

        double eal = pSensor->m_elevData.elevAlong;
        text.Format( "%.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? RADIANS_TO_MRADIANS(eal) : MRADIANS_TO_ARCMIN(eal)) ;
        SetItemText(row, DColEal, text);       

//         double ealS = pSensor->m_elevData.elevAlongSum;
//         text.Format( "%.2f", (SysSetup->GetUnits() == UNIT_MRAD) ? RADIANS_TO_MRADIANS(ealS) : MRADIANS_TO_ARCMIN(ealS)) ;
//         SetItemText(row, DColEalS, text);       

        double ealAbs = pSensor->m_elevData.elevAlongAbs;
        text.Format( "%.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? RADIANS_TO_MRADIANS(ealAbs) : MRADIANS_TO_ARCMIN(ealAbs)) ;
        SetItemText(row, DColEalAbs, text);       

        double eac = pSensor->m_elevData.elevAcross;
        text.Format( "%.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? RADIANS_TO_MRADIANS(eac) : MRADIANS_TO_ARCMIN(eac)) ;
        SetItemText(row, DColEac, text);       

        double zPar = pSensor->m_zParCorr;
        text.Format( "%.3f", zPar) ;
        SetItemText(row, DColZPar, text);       
        
        text.Format( "%d/%d", ElevationData::elevErrorExist, ElevationData::useElev) ;
        SetItemText(row, DColEOn, text);       

        text.Format( "%d", pSensor->m_elevData.mode) ;
        SetItemText(row, DColMode, text);       

    }

	Invalidate();
}


