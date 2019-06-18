#include "StdAfx.h"
#include ".\dbinterface.h"


extern NStopWatch sw;

DBInterface* DBInterface::m_ptInstance = NULL;



DBInterface::DBInterface(void)
{
}

DBInterface::~DBInterface(void)
{
    if(m_ptInstance != NULL)
        delete m_ptInstance;
}


DBInterface* DBInterface::Instance()
{	
	if (m_ptInstance != NULL)
	{
		return(m_ptInstance);
	}

	if ((m_ptInstance = new DBInterface) == NULL)
	{
		return NULL;
	}
	
	return m_ptInstance;
}

int DBInterface::Open(CString connectionString)
{


	if(connectionString != "")
	{
		if(!m_pDb.Open(connectionString))
		{	
			TRACE("Error open DB\n");
            return 1;
        }
	}

// 	GetOperators();
// 	GetGuns("TestShip");
	
	return 0;
}

CString DBInterface::ToText(double d)
{
    CString str;    
    if(d == _HUGE)
    {
        return "NULL";
    }
    else
    {
        str.Format("%f",d);
        return str;
    }
}

BOOL DBInterface::GetRecordsFromSql(CString sql, CADORecordset& records)
{
    if(!records.Open(sql, CADORecordset::openQuery))     
    {
        return FALSE;
    }

    return !records.IsEof();
}

BOOL DBInterface::GetDAUData(int DAUSerial, DAUSetupData& dauData)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM DAUSetup WHERE serialID = %d ",DAUSerial);	
    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
   
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("ID", dauData.DBId);                                
            rs.GetFieldValue("serialNumber", dauData.serialStr);                    
            rs.GetFieldValue("serialID", dauData.serial);                    
            rs.GetFieldValue("protocolVersion", dauData.protocolVersion);                    
            rs.GetFieldValue("sampleAndHold", dauData.sampleAndHold);                    
        }
    }    
    return TRUE;
}

BOOL DBInterface::GetSensorCalibTime(CString se, DBTIMESTAMP& time)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT time FROM SensorPitchAzimuthcalibration WHERE serialNumber = '%s'",se);	  

    CADORecordset rs(&m_pDb);
    COleDateTime oleTime;
    oleTime.GetAsDBTIMESTAMP(time);

    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {           
            rs.GetFieldValue("time", oleTime);
            oleTime.GetAsDBTIMESTAMP(time);            
        }
        else return FALSE;
    }    
    return TRUE;
}


BOOL DBInterface::GetAdapterCalibTime(CString ad, DBTIMESTAMP& time)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT time FROM GunAdapterCalibration WHERE serialNumber = '%s'",ad);	

    CADORecordset rs(&m_pDb);
    COleDateTime oleTime;
    oleTime.GetAsDBTIMESTAMP(time);

    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {           
            rs.GetFieldValue("time", oleTime);
            oleTime.GetAsDBTIMESTAMP(time);                      
        }
        else return FALSE;
    }    
    return TRUE;
}


BOOL DBInterface::GetChannelCalibTime(CString ch, int dauId, DBTIMESTAMP& time)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT calibTime FROM DAUSetupSensorChannel WHERE foreignID = %d And name = '%s' ORDER BY name ASC",dauId,ch);	

    CADORecordset rs(&m_pDb);
    COleDateTime oleTime;
           
    oleTime.GetAsDBTIMESTAMP(time);

    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {      
            rs.GetFieldValue("calibTime", oleTime);
            oleTime.GetAsDBTIMESTAMP(time);            
        }         
        else return FALSE;
    }    
    return TRUE;
}


BOOL DBInterface::GetSensors(vector<Sensor*>& sensors, int id)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM DAUSetupSensorChannel WHERE foreignID = %d ORDER BY name ASC",id);	

    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
    CString name;
    int type,offset,pitchOffset,rollOffset;
    double pitchGain, rollGain;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            rs.GetFieldValue("name", name);                    
            rs.GetFieldValue("type", type);                    
            rs.GetFieldValue("offset", offset);                    
            rs.GetFieldValue("pitchOffset", pitchOffset);                    
            rs.GetFieldValue("rollOffset", rollOffset);                    
            rs.GetFieldValue("pitchGain", pitchGain);                    
            rs.GetFieldValue("rollGain", rollGain);                    

            TRACE("Name:%s",name);
            
            Sensor *pSensor = new Sensor( name, type ) ;
            sensors.push_back( pSensor ) ;
            pSensor->SetOffset( offset ) ;
            ChannelCalibrationData roll(rollOffset, rollGain) ;
            ChannelCalibrationData pitch(pitchOffset, pitchGain) ;
            pSensor->SetChannelCalibrationData( roll, pitch ) ;
        
            rs.MoveNext();
        }
    }    
    return TRUE;
}

BOOL DBInterface::GetSyncros(vector<Syncro*>& syncros, int id)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM DAUSetupSyncroChannel WHERE foreignID = %d ORDER BY [order] ASC",id);	

    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
    CString name, type;
    int offset;
    double pitchGain, rollGain;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            double refV = 0.0f;
            rs.GetFieldValue("name", name);                                
            rs.GetFieldValue("offset", offset);                    
            try
            {
                //May not exist
                CADOFieldInfo fldInfo;
                rs.GetFieldInfo("refVoltage", &fldInfo);                    	
                rs.GetFieldValue("refVoltage", refV);     

                rs.GetFieldInfo("type", &fldInfo);                    	
                rs.GetFieldValue("type", type); 
            }
            catch (...)
            {
                
            }
            
            TRACE("Name:%s",name);
            
            Syncro *pSyncro = new Syncro( name ) ;
            pSyncro->SetOffset( offset ) ;
            pSyncro->SetRefVoltageA202(refV);
            pSyncro->SetDigConverterType(type);
            syncros.push_back( pSyncro ) ;                        

            rs.MoveNext();
        }
    }    
    return TRUE;
}

BOOL DBInterface::GetCalibratedSensors(vector<CString>& sensors)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql = "SELECT DISTINCT serialNumber FROM CalibratedSensors ORDER BY serialNumber";	

    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
    CString serial;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            rs.GetFieldValue("serialNumber", serial);                    
            sensors.push_back(serial);

            rs.MoveNext();
        }
    }
    return TRUE;
}

BOOL DBInterface::GetCalibratedAdapters(UnitType::Types t, vector<CString>& adapters)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
	sql.Format("SELECT DISTINCT serialNumber FROM GunAdapterCalibration WHERE type=%d ORDER BY serialNumber",t);	

    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
    CString serial;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            rs.GetFieldValue("serialNumber", serial);                    
            adapters.push_back(serial);

            rs.MoveNext();
        }
    }
    return TRUE;
}


BOOL DBInterface::GetShips(vector<CString>& ships)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql = "SELECT ShipName FROM Ships";	

    int nVal;
    CString strVal;

    CADORecordset rs(&m_pDb);
    CString ship;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            rs.GetFieldValue("ShipName", ship);                    
            ships.push_back(ship);

            rs.MoveNext();
        }
    }
    return TRUE;
}

BOOL DBInterface::GetStationsOnShip(vector<CString>& stations, CString ship)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT ClassID FROM Ships WHERE ShipName='%s'",ship);	
    CADORecordset rs(&m_pDb);
    int id;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("ClassID", id);                    
        }
    }

    sql.Format("SELECT StationName FROM StationPos WHERE ClassID=%d",id);	
    CADORecordset rs2(&m_pDb);
    CString station;
    
    if(rs2.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs2.IsEof())
        {
            rs2.GetFieldValue("StationName", station);                    
            stations.push_back(station);
            rs2.MoveNext();
        }
    }
 
    return TRUE;
}

BOOL DBInterface::GetAdapterCalibration(CString adaperSN, AdapterCalibrationData &data)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT azimuth, elevation FROM GunAdapterCalibration WHERE serialNumber = '%s'",adaperSN);	
    CADORecordset rs(&m_pDb);
    int id;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("elevation", data.m_elevation);                    
            rs.GetFieldValue("azimuth", data.m_azimuth);                    
        }
    }


    return TRUE;
}

BOOL DBInterface::GetSensorCalibrationData(CString calibName, CString SN, SensorTemperatureCalibrationData &data)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT a_0, a_1, a_2, a_3 FROM %s WHERE serialNumber ='%s'",calibName, SN);	
    CADORecordset rs(&m_pDb);
    int id;
    
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("a_0", data.m_offset);                    
            rs.GetFieldValue("a_1", data.m_linear);                    
            rs.GetFieldValue("a_2", data.m_quadratic);                    
            rs.GetFieldValue("a_3", data.m_cubic);                    
        }
    }
    return TRUE;
}

BOOL DBInterface::GetSensorData(CString table, CString SN, vector<SelectedData>& data)
{
    data.clear();

    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM %s WHERE serialNumber = '%s' ORDER BY timeStamp DESC",table, SN);	
    
    CADORecordset rs(&m_pDb);
    double date;
    COleDateTime oleTime;

    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            SelectedData selData;    
            rs.GetFieldValue("dauSerialNumber", selData.m_dauSerialNumber);                    
            rs.GetFieldValue("operator", selData.m_operatorName);                    
            rs.GetFieldValue("temperature", selData.m_temperature);                                
            rs.GetFieldValue("timeStamp", oleTime);
            oleTime.GetAsDBTIMESTAMP(selData.m_time);            
            rs.GetFieldValue("calibrationValue", selData.m_value);                    
            
            data.push_back(selData);
        
            rs.MoveNext();
        }
    }
    return TRUE;    
}

BOOL DBInterface::GetPlatformCorrectionAngle(CString serial, double& alpha)
{
    if(!m_pDb.IsOpen())
        return FALSE;
    
    CString sql;
    sql.Format("SELECT alpha FROM PlatformCorrection WHERE serialNumber = '%s'",serial);	

    CADORecordset rs(&m_pDb);    

    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("alpha", alpha);                                       
        }
        else return FALSE;
    }
    return TRUE;
}

BOOL DBInterface::GetUniqueSensorSN(CString table, vector<CString>& serial)
{
    serial.clear();

    if(!m_pDb.IsOpen())
        return FALSE;
    
    CString sql;
    sql.Format("SELECT DISTINCT serialNumber FROM %s ORDER BY serialNumber",table);	

    CADORecordset rs(&m_pDb);
    CString sn;
        
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        while(!rs.IsEof())
        {
            rs.GetFieldValue("serialNumber", sn);                    
            serial.push_back(sn);

            rs.MoveNext();
        }
    }
    return TRUE;
}

BOOL DBInterface::GetStationParallaxes(CString station, ParallaxData &data)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM StationPos WHERE stationName = '%s'",station);	
    CADORecordset rs(&m_pDb);
    int id;
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.GetFieldValue("StationX", data.x);                    
            rs.GetFieldValue("StationY", data.y);                    
            rs.GetFieldValue("StationZ", data.z);                    
            rs.GetFieldValue("SensorDeltaX", data.dx);                    
            rs.GetFieldValue("SensorDeltaY", data.dy);                    
            rs.GetFieldValue("SensorDeltaZ", data.dz);                    
        }
    }
    return TRUE;
}

BOOL DBInterface::UpdateAdapterCalibration(CString serial, double el, double az)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM GunAdapterCalibration WHERE serialNumber='%s'",serial);

    CADORecordset rs(&m_pDb);		
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.Edit();				
            rs.SetFieldValue("elevation", el);			
            rs.SetFieldValue("azimuth", az);						
            rs.SetFieldValue("time", COleDateTime::GetCurrentTime());			
            
            rs.Update();		
        }
        else
        {
            sql.Format("INSERT INTO GunAdapterCalibration ( serialNumber, elevation, azimuth ) VALUES ('%s',%f,%f)",serial, el, az);
            return m_pDb.Execute(sql);    
        }
    }
    return TRUE;	
}

BOOL DBInterface::UpdateSensorCalibration(CString table, CString serial, CString op, Polynomial& fit)//double a0, double a1, double a2, double a3)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM %s WHERE serialNumber='%s'",table, serial);

    CADORecordset rs(&m_pDb);		
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.Edit();				
            rs.SetFieldValue("time", COleDateTime::GetCurrentTime());			
            rs.SetFieldValue("operator", op);						
            rs.SetFieldValue("a_0", fit.m_a_0);			
            rs.SetFieldValue("a_1", fit.m_a_1);			
            rs.SetFieldValue("a_2", fit.m_a_2);			
            rs.SetFieldValue("a_3", fit.m_a_3);			

            rs.Update();		
        }
        else 
        {
            sql.Format("INSERT INTO %s ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES ('%s','%s',%f,%f,%f,%f)",table, serial, op, fit.m_a_0, fit.m_a_1, fit.m_a_2, fit.m_a_3);
            return m_pDb.Execute(sql);         
        }
    }
    return TRUE;	
}

BOOL DBInterface::UpdateDAUChannelGain(CString serialNumber, CString channel, DAUScaleCalibrationResult &result)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = %f, pitchGain = %f, calibTime = '%s' WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]='%s' And [DAUSetupSensorChannel].[name]='%s'",result.m_rollGain, result.m_pitchGain, COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S")), serialNumber, channel);

    BOOL b = m_pDb.Execute(sql);  
    return b;  
}

BOOL DBInterface::UpdateDAUChannelOffset(CString serialNumber, CString channel, DAUOffsetCalibrationResult &result)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = %d, pitchOffset = %d WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]='%s' And [DAUSetupSensorChannel].[name]='%s'",result.m_rollOffset, result.m_pitchOffset, serialNumber, channel);
    
    BOOL b = m_pDb.Execute(sql);  
    return b;  
}

BOOL DBInterface::AddSensorCalibrationA0(CString table, CString serial, CString op, double a0)
{
    SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
    DBInterface::Instance()->GetSensorCalibrationData(table, serial, data) ;

    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT * FROM %s WHERE serialNumber='%s'",table, serial);

    CADORecordset rs(&m_pDb);		
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {
            rs.Edit();				
            rs.SetFieldValue("time", COleDateTime::GetCurrentTime());			
            rs.SetFieldValue("operator", op);						
            rs.SetFieldValue("a_0", data.m_offset + a0);			
          
            rs.Update();		
        }
        else return FALSE;
    }
    return TRUE;	
}

BOOL DBInterface::InsertSensorData(CString table, CString dauSN, CString SN, CString op, double temp, double val)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO %s ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES ('%s','%s','%s',%f,%f)",table, SN, dauSN, op, temp, val);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::DeleteRecord(CString table, int id)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("DELETE FROM %s WHERE ID = %d",table, id);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::ClearTable(CString table)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("DELETE FROM %s",table);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::GetLastCounter(int& id)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("SELECT @@IDENTITY");
   
    CADORecordset rs(&m_pDb);		
    if(rs.Open(sql, CADORecordset::openQuery))     
    {
        if(!rs.IsEof())
        {                        
            rs.GetFieldValue(0,id);            
        }
        else return FALSE;
    }
    return TRUE;
}

BOOL DBInterface::InsertHistoryPrintItem(int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO HistoryPrint ( historyID ) VALUES ( %d )",historyId);

    return m_pDb.Execute(sql);  	
}

CString GetSigndefString()
{
    CString signdefText;
    signdefText.LoadString( (g_AlignerData.SignDef == -1) ? IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_NEG : IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_POS) ;
    return signdefText;
}


BOOL DBInterface::InsertHistoryItem(HistoryData& data)//DBTIMESTAMP ts)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    COleDateTime time(data.m_time);
    CString str;
    str.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
    CString sql;
    CString op = SysSetup->GetOperatorName();
    CString serial = DAU::GetDAU().GetSerialNumber();
    CString place = SysSetup->GetPlace();
    CString project = SysSetup->GetProjectName();

    sql.Format("INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, latitude, angulaDefinitionPart1, angulaDefinitionPart2, project, calibInfo ) VALUES ('%s','%s','%s','%s',%f,'%s','%s','%s','%s')",
        op, serial, time.Format( _T("%Y-%m-%d %H:%M:%S")), place, SysSetup->GetLatitude(), GetSigndefString(), str, project, data.calibInfo);

    return m_pDb.Execute(sql);  	
}
  
//A202
BOOL DBInterface::InsertHistory2Item(DBTIMESTAMP ts)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    COleDateTime time(ts);        
    CString sql;
    CString op = SysSetup->GetOperatorName();
    CString serial = DAU::GetDAU().GetSerialNumber();
    CString place = SysSetup->GetPlace();
    CString project = SysSetup->GetProjectName();

    sql.Format("INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, project ) VALUES ('%s','%s','%s','%s','%s')",
        op, serial, time.Format( _T("%Y-%m-%d %H:%M:%S")), place, project);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::UpdateComment(CString table, int historyId, CString comment)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("UPDATE %s SET comment = '%s' WHERE historyID = %d",table, comment, historyId);        

    return m_pDb.Execute(sql);  	
}


BOOL DBInterface::InsertTiltAlignmentErrors(TiltAlignmentErrorsHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;
  
    CString sql;
    sql.Format("INSERT INTO TiltAlignmentErrorsHistory (historyID, ship, timeConstant, lineOfSightDirection, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_lineOfSightDirection, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);    

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAlignmentErrorsItem(TiltAlignmentErrorsHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s)",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_bias));

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatness(TiltAndFlatnessHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit) VALUES (%d,'%s',%f,%d,'%s','%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_numberOfMeasurements, data.m_reference, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatnessItem(TiltAndFlatnessHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s,%s,%s)",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_standardDeviation), ToText(data.m_maximumDeviation), ToText(data.m_azimuth));

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatnessItemErr(TiltAndFlatnessHistory::ItemErrData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessHistoryItemErr (historyID, azimuth, error) VALUES (%d,%s,%s)",
        historyId, ToText(data.m_azimuth), ToText(data.m_error));

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatnessFo(TiltAndFlatnessFoHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessFoHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit, IndexArmLength) VALUES (%d,'%s',%f,%d,'%s','%s','%s',%f)",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_numberOfMeasurements, data.m_reference, data.m_comment, data.m_measuredUnit, data.m_IndexArmLength);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatnessFoItem(TiltAndFlatnessFoHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, tilt, angle, elevation1, elevation2, standardDeviation, bottomError, maximumDeviation, azimuth, IndexArmLength, IndexArm2Length, refstation, refchannel, refsensorSerialNumber) VALUES (%d,'%s','%s','%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,'%s','%s','%s')",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_roll, data.m_pitch, data.m_tilt, data.m_angle, data.m_elevation1, data.m_elevation2, data.m_standardDeviation, data.m_bottomError, data.m_maximumDeviation, data.m_azimuth, data.m_IndexArmLength, data.m_IndexArm2Length, data.m_refstation, data.m_refchannel, data.m_refsensorSerialNumber);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertTiltAndFlatnessFoItemErr(TiltAndFlatnessFoHistory::ItemErrData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO TiltAndFlatnessFoHistoryItemErr (historyID, azimuth, error1, error2, dh) VALUES (%d,%s,%s,%s,%s)",
        historyId, ToText(data.m_azimuth), ToText(data.m_error1), ToText(data.m_error2), ToText(data.m_dh));

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertGyroPerformanceTest(GyroPerformanceTestHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO GyroPerformanceTestHistory (historyID, ship, timeConstant, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertGyroPerformanceTestItem(GyroPerformanceTestHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO GyroPerformanceTestHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s)",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle));

    return m_pDb.Execute(sql);  	
}

BOOL DBInterface::InsertAzimuthAlignmentErrors(AzimuthAlignmentErrorsHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthAlignmentErrorsHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
            historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql); 
}

BOOL DBInterface::InsertAzimuthAlignmentErrorsItem(AzimuthAlignmentErrorsHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s)",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));
    
    return m_pDb.Execute(sql);  
} 

BOOL DBInterface::InsertAzimuthVerificationBenchmark(AzimuthVerificationBenchmarkHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthVerificationBenchmarkHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql); 
}

BOOL DBInterface::InsertAzimuthVerificationBenchmarkItem(AzimuthVerificationBenchmarkHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthVerificationBenchmarkHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s)",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertAzimuthVerificationGyrostability(AzimuthVerificationGyrostabilityHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthVerificationGyrostabilityHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql); 
}

BOOL DBInterface::InsertAzimuthVerificationGyrostabilityItem(AzimuthVerificationGyrostabilityHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO AzimuthVerificationGyrostabilityHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s) ",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertHorizonAbsoluteMode(HorizonAbsoluteModeHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO HorizonAbsoluteModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_parallaxCompensation, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql); 
}

BOOL DBInterface::InsertHorizonAbsoluteModeItem(HorizonAbsoluteModeHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO HorizonAbsoluteModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s','%s','%s',%f,%f,%f,%f,%f,%f,%f,%f) ",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_roll, data.m_pitch, data.m_tilt, data.m_angle, data.m_elevation, data.m_standardDeviation, data.m_maximumDeviation, data.m_azimuth);

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertHorizonRelativeMode(HorizonRelativeModeHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO HorizonRelativeModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_parallaxCompensation, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql); 
}

BOOL DBInterface::InsertHorizonRelativeModeItem(HorizonRelativeModeHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
    return FALSE;

    CString sql;
    sql.Format("INSERT INTO HorizonRelativeModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s,%s,%s) ",
            historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_standardDeviation), ToText(data.m_maximumDeviation), ToText(data.m_azimuth));

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertCommonFlatTilt(CommonFlatTiltHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO CommonFlatTiltHistory (historyID, ship, timeConstant, comment, measuredUnit ) VALUES (%d,'%s',%f,'%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql);
}

BOOL DBInterface::InsertCommonFlatTiltItem(CommonFlatTiltHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO CommonFlatTiltHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (%d,'%s','%s','%s',%s,%s,%f,%f,%f) ",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), data.m_parallellBias, data.m_perpendicularBias, data.m_temperature);

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertLiveGraphErrors(LiveGraphErrorsHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO LiveGraphErrorsHistory (historyID, ship, timeConstant, samplingRate, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
        historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_samplingRate, data.m_comment, data.m_measuredUnit);

    return m_pDb.Execute(sql);
}

BOOL DBInterface::InsertLiveGraphErrorsItem(LiveGraphErrorsHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO LiveGraphErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s) ",
        historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_temperature));

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertLiveDataA202Errors(LiveDataA202ErrorsHistory::Data data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO LiveDataA202ErrorsHistory ([historyID], [ship], [samplingRate], [comment]) VALUES (%d,'%s',%f,'%s')",
        historyId, SysSetup->GetShipName(), data.m_samplingRate, data.m_comment);

    return m_pDb.Execute(sql);  
}


BOOL DBInterface::InsertLiveDataA202ErrorsItem(LiveDataA202ErrorsHistory::ItemData data, int historyId)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO LiveDataA202ErrorsHistoryItem ([historyID], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (%d,'%s','%s',%f,%f,%f) ",
        historyId, data.m_station, data.m_channel, data.m_value, data.m_gearing, data.m_refVoltage);

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::InsertGraph(int historyId, CString file, int include)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("INSERT INTO GraphHistory (historyID, filename, include ) VALUES (%d,'%s',%d)",historyId, file, include);           

    return m_pDb.Execute(sql);  
}

BOOL DBInterface::DeleteGraph(CString file)
{
    if(!m_pDb.IsOpen())
        return FALSE;

    CString sql;
    sql.Format("DELETE FROM GraphHistory WHERE filename='%s'",file);        
    
    return m_pDb.Execute(sql);  
}


