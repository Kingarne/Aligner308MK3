#include "StdAfx.h"
#include "ConfigGrid.h"
#include <fstream>
#include "dau.h"
#include "SystemSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SensorGrid::SensorGrid(void):m_rows(8)
{
    m_initialized = FALSE;
}

SensorGrid::~SensorGrid(void)
{
}

void SensorGrid::Init()
{
    if(m_initialized)
        return;
    
    vector<Column> columns;
	columns.push_back(Column("Channel",60));
	columns.push_back(Column("Sensor",60));
	columns.push_back(Column("Station",110));
	columns.push_back(Column("Type",60));
	columns.push_back(Column("Roll", 50));
	columns.push_back(Column("Pitch", 50));
	columns.push_back(Column("Tempr.", 50));
	columns.push_back(Column("Nominal\nAzimuth",60));
	columns.push_back(Column("Adapter",80));
	columns.push_back(Column("Elev. Error\n[mrad]",60));
	columns.push_back(Column("Azim. Error\n[mrad]",60));
  columns.push_back(Column("X",45));
  columns.push_back(Column("Y",45));
  columns.push_back(Column("Z",45));
  columns.push_back(Column("dX",45));
  columns.push_back(Column("dY",45));
  columns.push_back(Column("dZ",45));
  columns.push_back(Column("Calibration", 60));

	m_cols = columns.size();

 	SetFixedColumnCount(m_cols);
 	SetFixedRowCount(m_rows);
	SetHeaderSort(FALSE);
	SetRowResize(FALSE);
	SetColumnResize(FALSE);
 	SetRowHeight(0, 40);
 	SetColumnWidth(0, 50);
	// 	SetRowHeight(1, 30);
	SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

	SetFixedTextColor(RGB(0,70,213));
	//ExpandColumnsToFit();   // This will remove any existing horz scrollbar
	ExpandToFit(FALSE);
	//ExpandLastColumn();
	LOGFONT lf = *GetItemFont(0, 0);
	//lf.lfWidth = 8;
	lf.lfHeight = 13;
	lf.lfWeight = FW_NORMAL;
	strcpy(lf.lfFaceName, "Tahoma");

	vector<Column>::iterator iter;
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
            SetRowHeight(r, 25);
			SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
			SetItemFont(r, c, &lf);
		}
	}
	//ExpandLastColumn();
	
	m_initialized = TRUE;
}

void SensorGrid::SetCell( int nRow, int nCol, int nVal )
{
	ASSERT(nRow >= 1 && nRow < m_rows);
	ASSERT(nCol >= 3 && nCol < m_cols);	//col 3 is OK col

	CString str;
	str.Format("%d", nVal);
	SetItemText(nRow, nCol, (LPCTSTR)str);
	Invalidate();
}

void SensorGrid::SetRow( int nRow, const vector<CString> &items )
{
	ASSERT(items.size() == m_cols);
	ASSERT(nRow > 0 && nRow < m_rows);

	int nCol = 0;
	vector<CString>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it, nCol++)
		SetItemText(nRow, nCol, (LPCTSTR)*it);
	Invalidate();
}

void SensorGrid::Clear()
{
	for(int i = 1; i < m_rows; i++)
		for(int j = 0; j < m_cols; j++)
			SetItemText(i, j, "");
	Invalidate();
}

void SensorGrid::Reset()
{	
	UpdateGrid();	
}


void SensorGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void SensorGrid::UpdateHeader()
{
    CString str;
    str.Format("Elev. Error\n[%s]",SysSetup->GetUnitText());    
    SetItemText(0, SColElevErr, str);
    str.Format("Azim. Error\n[%s]",SysSetup->GetUnitText());    
    SetItemText(0, SColAzErr, str);

}

void SensorGrid::UpdateGrid()
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

      SetItemText(row, SColCh, name);
      CString text;

      if( UnitType::Unused != pSensor->GetType() )
      {
        SetItemText(row, SColSensor, pSensor->GetSerialNumber());             
        SetItemText(row, SColStation, pSensor->m_description );             
        SetItemText(row, SColType, UnitType::GetUnitText(pSensor->GetType()) );             
           
			  double roll = -SysSetup->GetSignDef() * pSensor->GetRoll() * 1000.0f;
			  text.Format("%+7.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? roll : MRADIANS_TO_ARCMIN(roll));
			  SetItemText(row, SColRoll, text);
			
			  double pitch = -SysSetup->GetSignDef() * pSensor->GetPitch() * 1000.0f;
			  text.Format("%+7.3f", (SysSetup->GetUnits() == UNIT_MRAD) ? pitch : MRADIANS_TO_ARCMIN(roll));
			  SetItemText(row, SColPitch, text);
						
        int daysLeft = pSensor->DaysToCalibrationExp();

			if (pSensor->GetTemperature() < 68.4f)
			{
        if(daysLeft < 0)
          SetRowBGColor(row, UNCALIBRATED_COLOR);
        
        text.Format(_T("%.1f°"), pSensor->GetTemperature());
				SetItemText(row, SColTmpr, text);
			}
			else
			{
				SetRowBGColor(row, UNCONNECTED_COLOR);
				SetItemText(row, SColTmpr, "-");			      
      }


			text.Format( _T("%+6.1f°"), pSensor->GetNominalAzimuth() );
			SetItemText(row, SColNomAz, text);             
 
			


        if( UnitType::TypeHasAdapter(pSensor->GetType()))
        {
            SetItemText(row, SColAdapter, pSensor->GetAdapterDesc() );                              
            double value = 1000 * pSensor->GetAdapterCalibrationData().m_elevation ;
            text.Format( _T("%+6.2f"), (SysSetup->GetUnits() == UNIT_ARCMIN) ? MRADIANS_TO_ARCMIN(value) : value );
            SetItemText(row, SColElevErr, text );                              

            value = 1000 * pSensor->GetAdapterCalibrationData().m_azimuth ;
            text.Format( _T("%+6.2f"), (SysSetup->GetUnits() == UNIT_ARCMIN) ? MRADIANS_TO_ARCMIN(value) : value );
            SetItemText(row, SColAzErr, text );                              
        }
        else
        {
            text.LoadString(IDS_NA); //Gun parameters Not Applicable
            SetItemText(row, SColAdapter, text );             
            SetItemText(row, SColElevErr, text );
            SetItemText(row, SColAzErr, text );
        }
             
        ParallaxData parallaxdata = pSensor->GetParallaxData();
        text.Format("%+.1f",parallaxdata.x);
        SetItemText(row, SColPlxX, text );             
        text.Format("%+.1f",parallaxdata.y);
        SetItemText(row, SColPlxY, text );             
        text.Format("%+.1f",parallaxdata.z);
        SetItemText(row, SColPlxZ, text );             
        text.Format("%+.1f",parallaxdata.dx);
        SetItemText(row, SColPlxdX, text );             
        text.Format("%+.1f",parallaxdata.dy);
        SetItemText(row, SColPlxdY, text );             
        text.Format("%+.1f",parallaxdata.dz);
        SetItemText(row, SColPlxdZ, text );  
       
        if (daysLeft > 30)
          text = "OK";
        else if (daysLeft < 0)
          text = "Expired!";
        else
          text.Format("Exp. %dd", daysLeft);
        
        SetItemText(row, SCCalib, text);
        }

    }

	Invalidate();
}





GyroGrid::GyroGrid(void):m_rows(3)
{
    m_initialized = FALSE;
}

GyroGrid::~GyroGrid(void)
{
}

void GyroGrid::Init()
{
    if(m_initialized)
        return;

    vector<Column> columns;
    columns.push_back(Column("Channel",60));
    columns.push_back(Column("Station",130));
    columns.push_back(Column("Type",80));
    columns.push_back(Column("Nominal\nAzimuth",60));
    columns.push_back(Column("Gearing",60));

    m_cols = columns.size();

    SetFixedColumnCount(m_cols);
    SetFixedRowCount(m_rows);
    SetHeaderSort(FALSE);
    SetRowResize(FALSE);
    SetColumnResize(FALSE);
    SetRowHeight(0, 40);
    SetColumnWidth(0, 50);
    // 	SetRowHeight(1, 30);
    SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

    SetFixedTextColor(RGB(0,70,213));
    ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandToFit(FALSE);
    ExpandLastColumn();
    LOGFONT lf = *GetItemFont(0, 0);
    //lf.lfWidth = 8;
    lf.lfHeight = 13;
    lf.lfWeight = FW_NORMAL;
    strcpy(lf.lfFaceName, "Tahoma");

    vector<Column>::iterator iter;
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
            SetRowHeight(r, 25);
            SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
            SetItemFont(r, c, &lf);
        }
    }
    ExpandLastColumn();

    m_initialized = TRUE;
}

void GyroGrid::Clear()
{
    for(int i = 1; i < m_rows; i++)
        for(int j = 0; j < m_cols; j++)
            SetItemText(i, j, "");
    Invalidate();
}

void GyroGrid::Reset()
{	
    UpdateGrid();	
}


void GyroGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void GyroGrid::UpdateGrid()
{	
    Clear();

    UpdateSyncro(D1, J1, 0);
    UpdateSyncro(D2, J2, 1);        

    Invalidate();
}


void GyroGrid::UpdateSyncro(CString DName, CString JName, int index)
{
    Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(DName));
    Syncro* pJ = DAU::GetDAU().GetSyncro(JName);
    Syncro* pActive=NULL;
    CString name, gearing, type; 
    int row = index+1;
    if(pD != NULL && (pD->GetSyncroType() != DigChTypeProUnused))
    {
        name.Format("%s",pD->GetName());                    
        gearing.Format("N/A");                 
        type = pD->GetDigTypeText();
        pActive = pD;
    }
    else if(pJ != NULL && (pJ->GetType() != UnitType::Unused))
    {
        name.Format("%s",pJ->GetName());            
        gearing.Format("%.1f",pJ->GetGearing());
        type = UnitType::GetUnitText(pJ->GetType());
        pActive = pJ;
    }
    else
    {                
        name.Format("%s/%s",DName, JName);
    }
    SetItemText(row, GColCh, name );  
    
    SetRowBGColor(row, (pActive != NULL) ? USED_COLOR : UNUSED_COLOR);
    
    if(pActive == NULL)
        return;
    
    SetItemText(row, GColStation, pActive->m_description);  

    CString text;
    SetItemText(row, GColType, type );  
    
    text.Format( _T("%+6.1f°"),pActive->GetNominalAzimuth() );
    SetItemText(row, GColNomAz, text);  
    
    SetItemText(row, GColGearing, gearing);   
}


ChannelGrid::ChannelGrid(void):m_rows(9)
{
    m_initialized = FALSE;
}

ChannelGrid::~ChannelGrid(void)
{
}

void ChannelGrid::Init()
{
    if(m_initialized)
        return;

    m_chInfo[D1] = ChInfo(D1, 0, 0.0f);
    m_chInfo[D2] = ChInfo(D2, 1, 0.0f);
  //  m_chInfo[D3] = ChInfo(D3, 2, 0.0f);
    //m_chInfo[J1] = ChInfo(J1, 2, 0.0f);
  //  m_chInfo[D4] = ChInfo(D4, 3, 0.0f);    
   // m_chInfo[J2] = ChInfo(J2, 3, 0.0f);
    m_chInfo[A1] = ChInfo(A1, 2, 26.0f);
    m_chInfo[B1] = ChInfo(B1, 3, 26.0f);
    m_chInfo[C1] = ChInfo(C1, 4, 26.0f);
    m_chInfo[A2] = ChInfo(A2, 5, 115.0f);
    m_chInfo[B2] = ChInfo(B2, 6, 115.0f);
    m_chInfo[C2] = ChInfo(C2, 7, 115.0f);    

    vector<Column> columns;
    columns.push_back(Column("Channel",60));
    columns.push_back(Column("Ref. Voltage\n[V]",70));
    columns.push_back(Column("STD",60));
    columns.push_back(Column("Description",220));
    columns.push_back(Column("Gearing",45));
    columns.push_back(Column("Res.\n[Bits]",45));
    columns.push_back(Column("Type",75));
    
    m_cols = columns.size();

    SetFixedColumnCount(m_cols);
    SetFixedRowCount(m_rows);
    SetHeaderSort(FALSE);
    SetRowResize(FALSE);
    SetColumnResize(FALSE);
    SetRowHeight(0, 40);
    SetColumnWidth(0, 50);
    // 	SetRowHeight(1, 30);
    SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

    SetFixedTextColor(RGB(0,70,213));
    //ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandToFit(FALSE);
    //ExpandLastColumn();
    LOGFONT lf = *GetItemFont(0, 0);
    //lf.lfWidth = 8;
    lf.lfHeight = 13;
    lf.lfWeight = FW_NORMAL;
    strcpy(lf.lfFaceName, "Tahoma");

    vector<Column>::iterator iter;
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
            SetRowHeight(r, 30);
            SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
            SetItemFont(r, c, &lf);
        }
    }
    //ExpandLastColumn();

    m_initialized = TRUE;
}

void ChannelGrid::SetCell( int nRow, int nCol, int nVal )
{
    ASSERT(nRow >= 1 && nRow < m_rows);
    ASSERT(nCol >= 3 && nCol < m_cols);	//col 3 is OK col

    CString str;
    str.Format("%d", nVal);
    SetItemText(nRow, nCol, (LPCTSTR)str);
    Invalidate();
}

void ChannelGrid::SetRow( int nRow, const vector<CString> &items )
{
    ASSERT(items.size() == m_cols);
    ASSERT(nRow > 0 && nRow < m_rows);

    int nCol = 0;
    vector<CString>::const_iterator it;
    for(it = items.begin(); it != items.end(); ++it, nCol++)
        SetItemText(nRow, nCol, (LPCTSTR)*it);
    Invalidate();
}

void ChannelGrid::Clear()
{
    for(int i = 1; i < m_rows; i++)
        for(int j = 0; j < m_cols; j++)
        {
            SetItemText(i, j, "");
            SetRowBGColor(i, UNUSED_COLOR);

        }
    Invalidate();
}

void ChannelGrid::Reset()
{	
    UpdateGrid();	
}


void ChannelGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void ChannelGrid::UpdateHeader()
{
  

}

void ChannelGrid::UpdateSyncro(int index)
{
    Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(index));
    Syncro* pC = DAU::GetDAU().GetSyncro(index);
    Syncro* pActive=NULL;
    CString name="N/A", gearing="N/A", resolution="N/A", type="N/A", str="N/A", refV="N/A", digConvType="N/A"; 
    int row = index+1;

    if(pC)
    {
        name = pC->GetName();
    }
        
    if(pD != NULL && (pD->GetSyncroType() != DigChTypeProUnused))
    {
        name.Format("%s",pD->GetName());                    
        type = pD->GetDigTypeText();        
        pActive = pD;
    }
    else if(pC != NULL && (pC->GetType() != UnitType::Unused))
    {
        name.Format("%s",pC->GetName());            
        if(pC->GetType() == UnitType::Composite)
        {
        
        }
        else
        {
            gearing.Format("%.1f",pC->GetGearing());
            resolution.Format("%d",ResolutionToBits(pC->GetResolution()));
            refV.Format("%.1f",pC->GetRefVoltageA202());
            digConvType = pC->GetDigConverterType();
        }
        type = UnitType::GetUnitText(pC->GetType());        
        pActive = pC;
    }
    else
    {                
       name.Format("%s",name);
    }
    SetItemText(row, CColCh, name );  

    SetRowBGColor(row, (pActive != NULL) ? USED_COLOR : UNUSED_COLOR);

    if(pActive == NULL)
        return;
    
    SetItemText(row, CColRefVol, refV);  
    
    SetItemText(row, CColSTD, digConvType); 

    SetItemText(row, CColType, type );  

    SetItemText(row, CColSigDesc, pActive->m_description );  

    SetItemText(row, CColGearing, gearing);   
    
    SetItemText(row, CColResolution, resolution);   
}


void ChannelGrid::UpdateGrid()
{	
    Clear();

    //UpdateHeader();

    int maxNumCh = 10;
    //CString names[10] = { D1, D2, D3, D4, A1, B1, C1, A2, B2, C2 };

// 	for( int ch = 0; ch < maxNumCh ; ch++ )
// 	{	
//         
//         SetItemText(ch, CColCh, names[ch]);             
// //         m_SyncroChannel[i].SetWindowText( (LPCTSTR)names[i] );
// //         m_SyncroRefVoltage[i].SetWindowText( (LPCTSTR)text );
// // 		m_SyncroUnitTypeDescription[i].SetWindowText( (LPCTSTR)text );
// // 		m_SyncroGearing[i].SetWindowText( (LPCTSTR)text );
// //         m_SyncroType[i].SetWindowText( (LPCTSTR)text );
// 	}

    for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    {    
        UpdateSyncro(i);
//         CString name;
//         Syncro *pSyncro = DAU::GetDAU().GetSyncro( i );
//         name = pSyncro->GetName();
//         ChInfo info = m_chInfo[name];
//         SetItemText(i, CColCh, name);             
    }


    Invalidate();
}




ChannelLiveGrid::ChannelLiveGrid(void):m_rows(7)
{
    m_initialized = FALSE;
}

ChannelLiveGrid::~ChannelLiveGrid(void)
{
}

void ChannelLiveGrid::Init()
{
    if(m_initialized)
        return;

    m_indexMap[A1] = 1;
    m_indexMap[B1] = 2;
    m_indexMap[C1] = 3;
    m_indexMap[A2] = 4;
    m_indexMap[B2] = 5;
    m_indexMap[C2] = 6;   

    vector<Column> columns;
    columns.push_back(Column("Channel",50));
    columns.push_back(Column("Type",70));    
    columns.push_back(Column("Value",70));    
    columns.push_back(Column("Description",345));

    m_cols = columns.size();

    SetFixedColumnCount(m_cols);
    SetFixedRowCount(m_rows);
    SetHeaderSort(FALSE);
    SetRowResize(FALSE);
    SetColumnResize(FALSE);
    SetRowHeight(0, 30);
    SetColumnWidth(0, 50);
    // 	SetRowHeight(1, 30);
    SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

    SetFixedTextColor(RGB(0,70,213));
    //ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandToFit(FALSE);
    //ExpandLastColumn();
    LOGFONT lf = *GetItemFont(0, 0);
    //lf.lfWidth = 8;
    lf.lfHeight = 15;
    lf.lfWeight = FW_NORMAL;
    strcpy(lf.lfFaceName, "Tahoma");

    vector<Column>::iterator iter;
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
            SetRowHeight(r, 30);
            SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
            SetItemFont(r, c, &lf);
        }
    }
    //ExpandLastColumn();

    m_initialized = TRUE;
}

void ChannelLiveGrid::SetCell( int nRow, int nCol, int nVal )
{
    ASSERT(nRow >= 1 && nRow < m_rows);
    ASSERT(nCol >= 3 && nCol < m_cols);	//col 3 is OK col

    CString str;
    str.Format("%d", nVal);
    SetItemText(nRow, nCol, (LPCTSTR)str);
    Invalidate();
}

void ChannelLiveGrid::SetRow( int nRow, const vector<CString> &items )
{
    ASSERT(items.size() == m_cols);
    ASSERT(nRow > 0 && nRow < m_rows);

    int nCol = 0;
    vector<CString>::const_iterator it;
    for(it = items.begin(); it != items.end(); ++it, nCol++)
        SetItemText(nRow, nCol, (LPCTSTR)*it);
    Invalidate();
}

void ChannelLiveGrid::Clear()
{
    for(int i = 1; i < m_rows; i++)
        for(int j = 0; j < m_cols; j++)
        {
            SetItemText(i, j, "");
            SetRowBGColor(i, UNUSED_COLOR);

        }
        Invalidate();
}

void ChannelLiveGrid::Reset()
{	
    UpdateGrid();	
}


void ChannelLiveGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void ChannelLiveGrid::UpdateHeader()
{


}

void ChannelLiveGrid::UpdateSyncro(int index)
{
    Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(index));
    if(pD)
        return;
    
    Syncro* pC = DAU::GetDAU().GetSyncro(index);    
    CString name, gearing, type, str; 
    name = pC->GetName();
    int row = m_indexMap[name];

    SetItemText(row, CLColCh, name );      
    SetItemText(row, CLColSigDesc, pC->m_description );  

    SetRowBGColor(row, (pC->GetType() != UnitType::Unused) ? USED_COLOR : UNUSED_COLOR);
    if(pC->GetType() == UnitType::Unused)
        return;

    SetItemText(row, CLColType, UnitType::GetUnitText(pC->GetType()));    
  
}

void ChannelLiveGrid::UpdateLiveData(DAUValues& values)
{
    TRACE("NEW:\n");
    for (int i = 0 ; i < values.GetSyncroCount() ; i++)
    {
        Digital *pDig = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro( i ));
        if(pDig)
            continue;

        CString valueText ;
        double syncroValue, gearing ;
        Syncro *pCurrent = DAU::GetDAU().GetSyncro( i ) ;
        CString syncroName = pCurrent->GetName();
        //TRACE("name:%s\n",syncroName);

        if ((UnitType::Unused != pCurrent -> GetType()) && (pCurrent -> IsSelected()))// || !m_showOnlySelected))
        {
            //ViewUncalibrated:
            syncroValue = RADIANS_TO_DEGREES( values.GetSyncro( i ).GetUncalibratedRoll() );
            gearing = pCurrent -> GetGearing();

            if( syncroName.GetAt(1) == 'C' )
            {
                gearing = 1;
            }
            AdjustSyncroValueToRangeA202( &syncroValue, gearing );
            if( g_AlignerData.SignDef == 1 )
            {
                syncroValue = -syncroValue;
            }
            valueText.Format( IDS_DEG_FORMAT, syncroValue ) ;
        }
        else
        {
            valueText = _T("");
        }
        
        int row = m_indexMap[syncroName];
        SetItemText(row, CLColValue, valueText ); 
        //TRACE("row:%d\n",row);
        //m_syncroValue [index].SetWindowText( valueText ) ;      
    }


//     CString name;
//     int maxNumCh = 10;
//     for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
//     {  
//         int row = i+1;
//         static int c=0;
//         name.Format("%d",c++);
//         SetItemText(row, CLColValue, name ); 
//     }
    
    Invalidate();
}


void ChannelLiveGrid::UpdateGrid()
{	
    Clear();

    int maxNumCh = 8;
    
    for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    {    
        UpdateSyncro(i);
    }

    Invalidate();
}


ChannelLiveDigGrid::ChannelLiveDigGrid(void):m_rows(3)
{
    m_initialized = FALSE;
}

ChannelLiveDigGrid::~ChannelLiveDigGrid(void)
{
}

void ChannelLiveDigGrid::Init()
{
    if(m_initialized)
        return;

    m_indexMap[CH3] = 1;
    m_indexMap[CH4] = 2;

    vector<Column> columns;
    columns.push_back(Column("Channel",50));
    columns.push_back(Column("Type",80));    
    columns.push_back(Column("Roll",70));    
    columns.push_back(Column("Pitch",70));    
    columns.push_back(Column("Heading",70));    
    columns.push_back(Column("Description",195));

    m_cols = columns.size();

    SetFixedColumnCount(m_cols);
    SetFixedRowCount(m_rows);
    SetHeaderSort(FALSE);
    SetRowResize(FALSE);
    SetColumnResize(FALSE);
    SetRowHeight(0, 30);
    SetColumnWidth(0, 50);
    // 	SetRowHeight(1, 30);
    SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));

    SetFixedTextColor(RGB(0,70,213));
    //ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandToFit(FALSE);
    //ExpandLastColumn();
    LOGFONT lf = *GetItemFont(0, 0);
    //lf.lfWidth = 8;
    lf.lfHeight = 15;
    lf.lfWeight = FW_NORMAL;
    strcpy(lf.lfFaceName, "Tahoma");

    vector<Column>::iterator iter;
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
            SetRowHeight(r, 30);
            SetItemBkColour(r, c, RGB(255,255,255));//::GetSysColor(COLOR_BTNFACE));
            SetItemFont(r, c, &lf);
        }
    }
    //ExpandLastColumn();

    m_initialized = TRUE;
}

void ChannelLiveDigGrid::SetCell( int nRow, int nCol, int nVal )
{
    ASSERT(nRow >= 1 && nRow < m_rows);
    ASSERT(nCol >= 3 && nCol < m_cols);	//col 3 is OK col

    CString str;
    str.Format("%d", nVal);
    SetItemText(nRow, nCol, (LPCTSTR)str);
    Invalidate();
}

void ChannelLiveDigGrid::SetRow( int nRow, const vector<CString> &items )
{
    ASSERT(items.size() == m_cols);
    ASSERT(nRow > 0 && nRow < m_rows);

    int nCol = 0;
    vector<CString>::const_iterator it;
    for(it = items.begin(); it != items.end(); ++it, nCol++)
        SetItemText(nRow, nCol, (LPCTSTR)*it);
    Invalidate();
}

void ChannelLiveDigGrid::Clear()
{
    for(int i = 1; i < m_rows; i++)
        for(int j = 0; j < m_cols; j++)
        {
            SetItemText(i, j, "");
            SetRowBGColor(i, UNUSED_COLOR);

        }
        Invalidate();
}

void ChannelLiveDigGrid::Reset()
{	
    UpdateGrid();	
}


void ChannelLiveDigGrid::SetRowBGColor(int row, COLORREF col)
{
    for(int c = 0; c < m_cols; c++)
    {
        SetItemBkColour(row, c, col);
    }
}

void ChannelLiveDigGrid::UpdateHeader()
{


}

void ChannelLiveDigGrid::UpdateSyncro(int index)
{
    Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(index));
    if(!pD)
        return;
    
   
    CString name, gearing, type, str;     
    name = pD->GetName();
    int row = m_indexMap[name];
    
    SetItemText(row, CLDColCh, pD->GetName());  
    SetRowBGColor(row, (pD->GetSyncroType() != DigChTypeProUnused) ? USED_COLOR : UNUSED_COLOR);    

    if(pD->GetSyncroType() == DigChTypeProUnused)
        return;

    SetItemText(row, CLDColType, pD->GetDigTypeText());      
    SetItemText(row, CLDColSigDesc, pD->m_description );  
}

void ChannelLiveDigGrid::UpdateLiveData(DAUValues& values)
{
    TRACE("NEW:\n");
    for (int i = 0 ; i < values.GetSyncroCount() ; i++)
    {
        Digital* pD = dynamic_cast<Digital*>(DAU::GetDAU().GetSyncro(i));
        if(!pD)
            continue;

        CString valueText ;
        double roll, pitch, heading, gearing ;
        Syncro *pCurrent = DAU::GetDAU().GetSyncro( i ) ;
        CString syncroName = pCurrent->GetName();
        //TRACE("name:%s\n",syncroName);
        int index = m_indexMap[syncroName];

        if ((pD->GetType() != UnitType::Unused) && (pD->IsSelected()))// || !m_showOnlySelected))
        {                        
            if(pD->m_dataReceptionOK)
            {
                SingleSensor& val = values.GetSyncro( i );
                if(val.GetStatus() == DS_OK)
                {
                    roll = RADIANS_TO_DEGREES( val.GetUncalibratedRoll() );
                    pitch = RADIANS_TO_DEGREES( val.GetUncalibratedPitch() );
                    heading = RADIANS_TO_DEGREES( val.GetUncalibratedHeading() );
                    gearing = pCurrent->GetGearing();

                    if( syncroName.GetAt(1) == 'C' )
                    {
                        gearing = 1;
                    }
                    AdjustSyncroValueToRangeA202( &roll, gearing );
                    AdjustSyncroValueToRangeA202( &pitch, gearing );
                    AdjustSyncroValueToRangeA202( &heading, gearing );
                    if( g_AlignerData.SignDef == 1 )
                    {
                        roll = -roll;
                        pitch = -pitch;
                        heading = -heading;
                    }
                    valueText.Format( IDS_DEG_FORMAT, roll ) ;
                    SetItemText(index, CLDColValue1, valueText); 
                    valueText.Format( IDS_DEG_FORMAT, pitch ) ;
                    SetItemText(index, CLDColValue2, valueText); 
                    valueText.Format( IDS_DEG_FORMAT, heading ) ;
                    SetItemText(index, CLDColValue3, valueText); 
                }
                else
                {
                    valueText = "CRC Error";
                    SetItemText(index, CLDColValue1, valueText); 
                    SetItemText(index, CLDColValue2, valueText); 
                    SetItemText(index, CLDColValue3, valueText); 
                }
            }            
            else
            {
                valueText = "No data";
                SetItemText(index, CLDColValue1, valueText); 
                SetItemText(index, CLDColValue2, valueText); 
                SetItemText(index, CLDColValue3, valueText); 

            }
        }
        else
        {
            SetItemText(index, CLDColValue1, "" ); 
            SetItemText(index, CLDColValue2, "" ); 
            SetItemText(index, CLDColValue3, "" ); 
        }
        
        
        //m_syncroValue [index].SetWindowText( valueText ) ;      
    }


    //     CString name;
    //     int maxNumCh = 10;
    //     for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    //     {  
    //         int row = i+1;
    //         static int c=0;
    //         name.Format("%d",c++);
    //         SetItemText(row, CLColValue, name ); 
    //     }

    Invalidate();
}


void ChannelLiveDigGrid::UpdateGrid()
{	
    Clear();

    //UpdateHeader();

    int maxNumCh = 8;
    //CString names[10] = { D1, D2, D3, D4, A1, B1, C1, A2, B2, C2 };

    // 	for( int ch = 0; ch < maxNumCh ; ch++ )
    // 	{	
    //         
    //         SetItemText(ch, CColCh, names[ch]);             
    // //         m_SyncroChannel[i].SetWindowText( (LPCTSTR)names[i] );
    // //         m_SyncroRefVoltage[i].SetWindowText( (LPCTSTR)text );
    // // 		m_SyncroUnitTypeDescription[i].SetWindowText( (LPCTSTR)text );
    // // 		m_SyncroGearing[i].SetWindowText( (LPCTSTR)text );
    // //         m_SyncroType[i].SetWindowText( (LPCTSTR)text );
    // 	}

    for( int i = 0; i < min( maxNumCh, DAU::GetDAU().GetSyncroCount() ); i++ )
    {    
        UpdateSyncro(i);
        //         CString name;
        //         Syncro *pSyncro = DAU::GetDAU().GetSyncro( i );
        //         name = pSyncro->GetName();
        //         ChInfo info = m_chInfo[name];
        //         SetItemText(i, CColCh, name);             
    }


    Invalidate();
}



BEGIN_MESSAGE_MAP(SensorGrid, CGridCtrl)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void SensorGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
 CGridCtrl::OnMouseMove(nFlags, point);
	CCellID idCurrentCell;
	idCurrentCell = GetCellFromPt(point);
	CGridCellBase* pCell = GetCell(idCurrentCell.row, idCurrentCell.col);
	if (!pCell)
		return;

	CRect TextRect, CellRect;
	GetCellRect(idCurrentCell.row, idCurrentCell.col, &TextRect);
	pCell->GetTipTextRect(&TextRect);
	GetCellRect(idCurrentCell.row, idCurrentCell.col, CellRect);

	if (idCurrentCell.col != 1 || idCurrentCell.row == 0)
		return;

	TextRect.right = TextRect.left + 2; //So that we allways display tip.
	CString str = pCell->GetText();
	if (str == "")
		return;

	Sensor* pSensor = DAU::GetDAU().GetSensorFromSN(str);
	if (!pSensor)
		return;

	CString info = GetCalibrationInfo(*pSensor);

	m_TitleTip.Show(TextRect, info, 0, CellRect,
		pCell->GetFont(), GetTitleTipTextClr(), GetTitleTipBackClr());

	TRACE("Move:%d,%d\n", idCurrentCell.row, idCurrentCell.col);

}

CString SensorGrid::GetCalibrationInfo(Sensor& sensor)
{
	CString info, str;
	str.Format("Sensor:%s\n\n", sensor.GetSerialNumber());
	info += str;
	str.Format("                           A0               A1              A2             A3                    Date\n");
	info += str;

	SensorTemperatureCalibrationData cal = sensor.m_rollOffsetTemperatureCalibration;
	str.Format(" Roll Offset: %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str;

	cal = sensor.m_rollGainTemperatureCalibration;
	str.Format(" Roll Scale:   %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str;

	cal = sensor.m_rollAzimuthTemperatureCalibration;
	str.Format(" Roll Azim:    %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str + "\n";

	cal = sensor.m_pitchOffsetTemperatureCalibration;
	str.Format(" Pitch Offset:  %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str;

	cal = sensor.m_pitchGainTemperatureCalibration;
	str.Format(" Pitch Scale:    %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str;

	cal = sensor.m_pitchAzimuthTemperatureCalibration;
	str.Format(" Pitch Azim:     %.5f,  %.2e,  %.2e,  %.2e  (%02d/%02d/%02d %02d:%02d:%02d)\n", cal.m_offset, cal.m_linear, cal.m_quadratic, cal.m_cubic,
		cal.m_time.year - 2000, cal.m_time.month, cal.m_time.day, cal.m_time.hour, cal.m_time.minute, cal.m_time.second);
	info += str;


	return info;
}