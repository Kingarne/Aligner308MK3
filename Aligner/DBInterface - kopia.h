#pragma once

#include "ado2.h"
#include "LeastSquare.h"
#include "Util.h"
#include "history.h"


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
    BOOL GetShips(vector<CString>& ships);
    BOOL GetStationsOnShip(vector<CString>& ships, CString ship);
    BOOL GetAdapterCalibration(CString adaperSN, AdapterCalibrationData &data);
    BOOL GetSensorCalibrationData(CString calibName, CString SN, SensorTemperatureCalibrationData &data);
    BOOL GetSensorData(CString table, CString SN, vector<SelectedData>& data);
    BOOL GetStationParallaxes(CString station, ParallaxData &data);        
    BOOL GetUniqueSensorSN(CString table, vector<CString>& serial);
    BOOL GetPlatformCorrectionAngle(CString serial, double& alpha);
    BOOL GetLastCounter(int& id);

    BOOL GetChannelCalibTime(CString ch, int dauId, DBTIMESTAMP& time);
    BOOL GetAdapterCalibTime(CString ad, DBTIMESTAMP& time);
    BOOL GetSensorCalibTime(CString se, DBTIMESTAMP& time);

    BOOL UpdateAdapterCalibration(CString serial, double el, double az);
    BOOL UpdateSensorCalibration(CString table, CString serial, CString op, Polynomial& fit);//double a0, double a1, double a2, double a3);
    BOOL UpdateDAUChannelOffset(CString serialNumber, CString channel, DAUOffsetCalibrationResult &result);
    BOOL UpdateDAUChannelGain(CString serialNumber, CString channel, DAUScaleCalibrationResult &result);
    BOOL UpdateComment(CString table, int historyId, CString comment);

    BOOL InsertSensorData(CString table, CString dauSN, CString SN, CString op, double temp, double val);
    BOOL InsertHistoryPrintItem(int historyId);
    BOOL InsertHistoryItem(HistoryData& data);//DBTIMESTAMP ts);
    BOOL InsertHistory2Item(DBTIMESTAMP ts);

    BOOL InsertTiltAlignmentErrors(TiltAlignmentErrorsHistory::Data, int historyId);
    BOOL InsertTiltAlignmentErrorsItem(TiltAlignmentErrorsHistory::ItemData, int historyId);

    BOOL InsertTiltAndFlatness(TiltAndFlatnessHistory::Data data, int historyId);
    BOOL InsertTiltAndFlatnessItem(TiltAndFlatnessHistory::ItemData data, int historyId);
    BOOL InsertTiltAndFlatnessItemErr(TiltAndFlatnessHistory::ItemErrData data, int historyId);

    BOOL InsertTiltAndFlatnessFo(TiltAndFlatnessFoHistory::Data data, int historyId);
    BOOL InsertTiltAndFlatnessFoItem(TiltAndFlatnessFoHistory::ItemData data, int historyId);
    BOOL InsertTiltAndFlatnessFoItemErr(TiltAndFlatnessFoHistory::ItemErrData data, int historyId);

    BOOL InsertGyroPerformanceTest(GyroPerformanceTestHistory::Data data, int historyId);
    BOOL InsertGyroPerformanceTestItem(GyroPerformanceTestHistory::ItemData data, int historyId);

    BOOL InsertAzimuthAlignmentErrors(AzimuthAlignmentErrorsHistory::Data data, int historyId);
    BOOL InsertAzimuthAlignmentErrorsItem(AzimuthAlignmentErrorsHistory::ItemData data, int historyId);

    BOOL InsertAzimuthVerificationBenchmark(AzimuthVerificationBenchmarkHistory::Data data, int historyId);
    BOOL InsertAzimuthVerificationBenchmarkItem(AzimuthVerificationBenchmarkHistory::ItemData data, int historyId);

    BOOL InsertAzimuthVerificationGyrostability(AzimuthVerificationGyrostabilityHistory::Data data, int historyId);
    BOOL InsertAzimuthVerificationGyrostabilityItem(AzimuthVerificationGyrostabilityHistory::ItemData data, int historyId);

    BOOL InsertHorizonAbsoluteMode(HorizonAbsoluteModeHistory::Data data, int historyId);
    BOOL InsertHorizonAbsoluteModeItem(HorizonAbsoluteModeHistory::ItemData data, int historyId);

    BOOL InsertHorizonRelativeMode(HorizonRelativeModeHistory::Data data, int historyId);
    BOOL InsertHorizonRelativeModeItem(HorizonRelativeModeHistory::ItemData data, int historyId);

    BOOL InsertCommonFlatTilt(CommonFlatTiltHistory::Data data, int historyId);
    BOOL InsertCommonFlatTiltItem(CommonFlatTiltHistory::ItemData data, int historyId);

    BOOL InsertLiveGraphErrors(LiveGraphErrorsHistory::Data data, int historyId);
    BOOL InsertLiveGraphErrorsItem(LiveGraphErrorsHistory::ItemData data, int historyId);
    
    BOOL InsertLiveDataA202Errors(LiveDataA202ErrorsHistory::Data data, int historyId);
    BOOL InsertLiveDataA202ErrorsItem(LiveDataA202ErrorsHistory::ItemData data, int historyId);

    BOOL InsertGraph(int historyId, CString file, int include);
    BOOL DeleteGraph(CString file);

    BOOL AddSensorCalibrationA0(CString table, CString serial, CString op, double a0);

    CString ToText(double d);
    CString ToText(LONG l);

    BOOL DeleteRecord(CString table, int id);
    BOOL ClearTable(CString table);    
    
    CADODatabase* GetDB(){return &m_pDb;}    
	BOOL GetRecordsFromSql(CString sql, CADORecordset& records);

	static DBInterface* Instance();

protected:
	
	static DBInterface* m_ptInstance;

	CADODatabase m_pDb;    
};
