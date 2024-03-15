#pragma once

//#include "ado2.h"
#include "afxdb.h"
#include "LeastSquare.h"
#include "Util.h"
#include "history.h"
#include "SystemSetup.h"

class DBInterface
{
public:
	DBInterface(void);
	~DBInterface(void);

	int Open(CString connectionString);

    BOOL GetDAUData(int DAUSerial, DAUSetupData& dauData);
    BOOL GetSensors(vector<Sensor*>& sensors, int id);    
    BOOL GetSyncros(vector<Syncro*>& syncros, int id);
    BOOL GetCalibratedSensors(vector<CString>& sensors);
    BOOL GetCalibratedAdapters(UnitType::Types t, vector<CString>& adapters);	
	BOOL GetShip(CString name, Ship& ship);
	BOOL GetShips(vector<Ship>& ships);
    BOOL GetStationsOnShip(vector<CString>& ships, CString ship);
    BOOL GetAdapterCalibration(CString adaperSN, UnitType::Types t, AdapterCalibrationData &data);
    BOOL GetTheoAdapterType(CString adaperSN, AdapterData::Type& type);
    BOOL GetSensorCalibrationData(CString calibName, CString SN, SensorTemperatureCalibrationData &data);
    BOOL GetSensorData(CString table, CString SN, vector<SelectedData>& data, CString projName);
    BOOL GetStationParallaxes(CString station, ParallaxData &data);        
    BOOL GetUniqueSensorSN(CString table, vector<CString>& serial);
    BOOL GetPlatformCorrectionAngle(CString serial, double& alpha);
	BOOL GetPlatforms(vector<Platform>& platforms);
	BOOL GetLastCounter(int& id);

    BOOL GetChannelCalibTime(CString ch, int dauId, DBTIMESTAMP& time);
    BOOL GetAdapterCalibTime(CString ad, DBTIMESTAMP& time);
    BOOL GetSensorCalibTime(CString se, DBTIMESTAMP& time);
	
	BOOL GetProjects(vector<ProjectData>& projects);
  BOOL GetProjects202(vector<ProjectData>& projects);
	BOOL GetCalibrationProjects(vector<ProjectData>& projects);
	
    BOOL UpdateAdapterCalibration(CString serial, double el, double az);
    BOOL UpdateTheoAdapterCalibration(CString serial, double el, double az);
    BOOL UpdateSensorCalibration(CString table, CString serial, Polynomial& fit);//double a0, double a1, double a2, double a3);
    BOOL UpdateDAUChannelOffset(CString serialNumber, CString channel, DAUOffsetCalibrationResult &result);
    BOOL UpdateDAUChannelGain(CString serialNumber, CString channel, DAUScaleCalibrationResult &result);
    BOOL UpdateComment(CString table, int historyId, CString comment);
	BOOL UpdateProjectConfig(int projectID, CString xml, int mode);

	BOOL UpdateCalibrationFlag(CString table, int id, BOOL b);

	BOOL ProjectExist(CString name, CString table);
	BOOL InsertProject(ProjectData& project);
	BOOL InsertProjectCalibration(ProjectData& project);

    BOOL InsertSensorData(CString table, CString SN, double temp, double val);
    BOOL InsertHistoryPrintItem(int historyId);
    
	BOOL InsertMeasurement(MeasurementBase& data);

    BOOL InsertTiltAlignment(TiltAlignment::Data, int measId);
    BOOL InsertTiltAlignmentChannel(TiltAlignment::ChannelData, int measId);

    BOOL InsertTiltAndFlatness(TiltAndFlatness::Data data, int measId);
    BOOL InsertTiltAndFlatnessChannel(TiltAndFlatness::ChannelData data, int measId);
    BOOL InsertTiltAndFlatnessChannelErr(TiltAndFlatness::ChannelErrData data, int measId);

    BOOL InsertTiltAndFlatnessFo(TiltAndFlatnessFo::Data data, int measId);
    BOOL InsertTiltAndFlatnessFoChannel(TiltAndFlatnessFo::ChannelData data, int measId);
    BOOL InsertTiltAndFlatnessFoChannelErr(TiltAndFlatnessFo::ChannelErrData data, int measId);

    BOOL InsertGyroPerformance(GyroPerformance::Data data, int measId);
    BOOL InsertGyroPerformanceChannel(GyroPerformance::ChannelData data, int measId);

    BOOL InsertAzimuthAlignment(AzimuthAlignment::Data data, int measId);
    BOOL InsertAzimuthAlignmentChannel(AzimuthAlignment::ChannelData data, int measId);

    BOOL InsertHorizonAbsoluteMode(HorizonAbsoluteMode::Data data, int measId);
    BOOL InsertHorizonAbsoluteModeChannel(HorizonAbsoluteMode::ChannelData data, int measId);

    BOOL InsertHorizonRelativeMode(HorizonRelativeMode::Data data, int measId);
    BOOL InsertHorizonRelativeModeChannel(HorizonRelativeMode::ChannelData data, int measId);

    BOOL InsertCommonFlatTilt(CommonFlatTilt::Data data, int measId);
    BOOL InsertCommonFlatTiltChannel(CommonFlatTilt::ChannelData data, int measId);

	BOOL InsertSensorValidation(SensorValidation::Data data, int measId);
	BOOL InsertSensorValidationChannel(SensorValidation::ChannelData data, int measId);

    BOOL InsertLiveGraph(LiveGraph::Data data, int measId);
    BOOL InsertLiveGraphChannel(LiveGraph::ChannelData data, int measId);
    
    BOOL InsertLiveDataA202(LiveDataA202::Data data, int measId);
    BOOL InsertLiveDataA202Channel(LiveDataA202::ChannelData data, int measId);

    BOOL InsertGraph(int historyId, CString file, int include);
    BOOL DeleteGraph(CString file);

    BOOL AddSensorCalibrationA0(CString table, CString serial, double a0);	

    CString ToText(double d);
    CString ToText(LONG l);

    BOOL DeleteRecord(CString table, int id);
    BOOL ClearTable(CString table);    
    
	BOOL ExecuteSQL(CString& sql);
  //  CADODatabase* GetDB(){return &m_pDb;}    
	//BOOL GetRecordsFromSql(CString sql, CADORecordset& records);

	static DBInterface* Instance();

protected:
	
	static DBInterface* m_ptInstance;
	CDatabase m_db;

	//CADODatabase m_pDb;    
};
