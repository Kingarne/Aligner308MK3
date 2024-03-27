#pragma once
#include ".\gridctrl\gridctrl.h"

#define USED_COLOR			    RGB(200,240,200)
#define UNUSED_COLOR		    ::GetSysColor(COLOR_BTNFACE)
#define UNCONNECTED_COLOR	  RGB(240,240,80)
#define UNCALIBRATED_COLOR	RGB(240,180,80)

struct Column{
	Column(CString head, int w):header(head),width(w){}
	CString header;
	int width;
};

enum SensorColumnName{SColCh, SColSensor, SColStation, SColType, SColRoll, SColPitch, SColTmpr, SColNomAz, SColAdapter, SColElevErr, SColAzErr,
    SColPlxX, SColPlxY, SColPlxZ, SColPlxdX, SColPlxdY, SColPlxdZ, SCCalib};

class SensorGrid : public CGridCtrl
{
public:
	SensorGrid(void);
	~SensorGrid(void);

	void Init();
	void SetCell(int nRow, int nCol, int nVal);
	void SetRow(int nRow, const vector<CString> &items);
	void Clear();
	void Reset();
	
	void UpdateGrid();
//	int StoreToFile();
	
private:
	int m_cols;
	int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateHeader();
	CString GetCalibrationInfo(Sensor& sensor);
  CString GetAdapterCalibrationInfo(Sensor& sensor);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

enum GyroColumnName{GColCh, GColStation, GColType, GColNomAz, GColGearing};
class GyroGrid : public CGridCtrl
{
public:
    GyroGrid(void);
    ~GyroGrid(void);

    void Init();
    void Clear();
    void Reset();

    void UpdateGrid();    

private:
    int m_cols;
    int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateSyncro(CString DName, CString JName, int index);
};

struct ChInfo
{
    ChInfo(){};
    ChInfo(CString n, int p, double r){name=n;pos=p;refVoltage=r;}
    CString name;
    int pos;
    double refVoltage;
};

enum ChannelColumnName{CColCh, CColRefVol, CColSTD, CColSigDesc, CColGearing, CColResolution, CColType};
class ChannelGrid : public CGridCtrl
{
public:
    ChannelGrid(void);
    ~ChannelGrid(void);

    void Init();
    void SetCell(int nRow, int nCol, int nVal);
    void SetRow(int nRow, const vector<CString> &items);
    void Clear();
    void Reset();

    void UpdateGrid();
    //	int StoreToFile();
    map<CString, ChInfo> m_chInfo;

private:
    int m_cols;
    int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateSyncro(int index);
    void UpdateHeader();
};

enum ChannelLiveColumnName{CLColCh, CLColType, CLColValue, CLColSigDesc};
class ChannelLiveGrid : public CGridCtrl
{
public:
    ChannelLiveGrid(void);
    ~ChannelLiveGrid(void);

    void Init();
    void SetCell(int nRow, int nCol, int nVal);
    void SetRow(int nRow, const vector<CString> &items);
    void Clear();
    void Reset();

    void UpdateGrid();
    void UpdateLiveData(DAUValues& values);
    //	int StoreToFile();
    map<CString, int> m_indexMap;

private:
    int m_cols;
    int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateSyncro(int index);
    void UpdateHeader();
};


enum ChannelLiveDigColumnName{CLDColCh, CLDColType, CLDColValue1, CLDColValue2, CLDColValue3, CLDColSigDesc};
class ChannelLiveDigGrid : public CGridCtrl
{
public:
    ChannelLiveDigGrid(void);
    ~ChannelLiveDigGrid(void);

    void Init();
    void SetCell(int nRow, int nCol, int nVal);
    void SetRow(int nRow, const vector<CString> &items);
    void Clear();
    void Reset();

    void UpdateGrid();
    void UpdateLiveData(DAUValues& values);
    //	int StoreToFile();
    map<CString, int> m_indexMap;

private:
    int m_cols;
    int m_rows;
    BOOL m_initialized;

    void SetRowBGColor(int row, COLORREF col);
    void UpdateSyncro(int index);
    void UpdateHeader();
};

