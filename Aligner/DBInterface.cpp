#include "StdAfx.h"
#include ".\dbinterface.h"


extern NStopWatch sw;

DBInterface* DBInterface::m_ptInstance = NULL;

DBTIMESTAMP ToDBTimestamp(TIMESTAMP_STRUCT* t)
{
	DBTIMESTAMP ts;
	memcpy(&ts, t, sizeof(TIMESTAMP_STRUCT));
	return ts;
	//ts.year = t->year;
}

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
	CString sDsn;
	//CString sFile = _T("C:\\Program Files (x86)\\Schill Reglerteknik AB\\Aligner 308 MK2\\Aligner308.mdb");

	// Build ODBC connection string
	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), _T("MICROSOFT ACCESS DRIVER (*.mdb)"), connectionString);
	try
	{
		// Open the database
		m_db.Open(NULL, FALSE, FALSE, sDsn);
	}
	catch (CDBException* e)
	{
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}

	


// 	if(connectionString != "")
// 	{
// 		if(!m_db.Open(connectionString))
// 		{	
// 			TRACE("Error open DB\n");
//             return 1;
//         }
// 	}

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

// BOOL DBInterface::GetRecordsFromSql(CString sql, CADORecordset& records)
// {
//     if(!records.Open(sql, CADORecordset::openQuery))     
//     {
//         return FALSE;
//     }
// 
//     return !records.IsEOF();
// }


BOOL DBInterface::InsertProject(ProjectData& project)
{
	if (!m_db.IsOpen())
		return FALSE;

	COleDateTime time(project.m_time);

	CString sql = "";
	sql.Format("INSERT INTO Project( name, operator, location, shipId, latitude, unit, imgIdx, projTime, signDef ) VALUES ('%s','%s','%s', %d, %.2f, %d, %d, '%s', %d)",
		project.m_projectName, project.m_operatorName, project.m_location, project.m_shipID, project.m_latitude, project.m_unit, 0, time.Format(_T("%Y-%m-%d %H:%M:%S")), project.m_signDef);

	m_db.ExecuteSQL(sql);

	int id;
	GetLastCounter(id);
	SysSetup->SetProjectID(id);

	return TRUE;
}

BOOL DBInterface::GetProjects(vector<ProjectData>& projects)
{

	if (!m_db.IsOpen())
		return FALSE;

	CString sql = "";
	sql.Format("SELECT * FROM Project INNER JOIN Ships ON Project.shipID = Ships.ShipID");

	ProjectData data;

	CRecordset rs(&m_db);
	if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	{
		TRACE("count: %d, %d\n",rs.GetRecordCount(), rs.GetODBCFieldCount());
		for (int i = 0; i < rs.GetODBCFieldCount(); i++)
		{
			CODBCFieldInfo info;
			rs.GetODBCFieldInfo(i, info);
			TRACE("%d, %s\n", i, info.m_strName);
		}
			
		while (!rs.IsEOF())
		{
			CDBVariant val;
			rs.GetFieldValue("ID", val);
			data.m_projectID = val.m_iVal;
			rs.GetFieldValue("name", data.m_projectName);
			rs.GetFieldValue("projTime", val);//oleTime);
			data.m_time = ToDBTimestamp(val.m_pdate);
			rs.GetFieldValue("operator", data.m_operatorName);
			rs.GetFieldValue("location", data.m_location);
			rs.GetFieldValue("latitude", val);
			data.m_latitude = val.m_fltVal;
			rs.GetFieldValue("unit", val);
			data.m_unit = val.m_iVal;
			rs.GetFieldValue("signDef", val);
			data.m_signDef = val.m_iVal;
			rs.GetFieldValue("ShipName", data.m_shipName);
			rs.GetFieldValue("ClassID", val);
			data.m_shipClass = val.m_iVal;
			CString str;
			rs.GetFieldValue("config", data.m_config);

			projects.push_back(data);
			
			rs.MoveNext();
		}
	}


	return true;
}

BOOL DBInterface::GetDAUData(int DAUSerial, DAUSetupData& dauData)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql="";
    sql.Format("SELECT * FROM DAUSetup WHERE serialID = %d ",DAUSerial);	
    int nVal;
    CString strVal;

   // CADORecordset rs(&m_db);
   
	CRecordset rs(&m_db);   
	if(rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
    {
        if(!rs.IsEOF())
        {
			CDBVariant val;
            rs.GetFieldValue("ID", val);
			dauData.DBId = val.m_iVal;
            rs.GetFieldValue("serialNumber", dauData.serialStr);                    
            rs.GetFieldValue("serialID", val);
			dauData.serial = val.m_iVal;
            rs.GetFieldValue("protocolVersion", val);
			dauData.protocolVersion  = val.m_iVal;
            rs.GetFieldValue("sampleAndHold", val);
			dauData.sampleAndHold = val.m_iVal;
        }
    }    
    return TRUE;
}


BOOL DBInterface::GetSensorCalibTime(CString se, DBTIMESTAMP& time)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql="";
    sql.Format("SELECT time FROM SensorPitchAzimuthcalibration WHERE serialNumber = '%s'",se);	  

  //  CADORecordset rs(&m_db);
    COleDateTime oleTime;
    oleTime.GetAsDBTIMESTAMP(time);

	CRecordset rs(&m_db);   
	if(rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
    {
        if(!rs.IsEOF())
        {           
			CDBVariant val;
            rs.GetFieldValue("time", val);//oleTime);
            time = ToDBTimestamp(val.m_pdate);
			//oleTime = val.m_pdate;
			//oleTime.GetAsDBTIMESTAMP(time);            
        }
        else return FALSE;
    }    
    return TRUE;
}


BOOL DBInterface::GetAdapterCalibTime(CString ad, DBTIMESTAMP& time)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql="";
    sql.Format("SELECT [time] FROM GunAdapterCalibration WHERE serialNumber = '%s'",ad);	

    COleDateTime oleTime;   

	CRecordset rs(&m_db);   
	if(rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))   
    {
        if(!rs.IsEOF())
        {           
			CDBVariant val;
            rs.GetFieldValue("time", val);
            time = ToDBTimestamp(val.m_pdate);			
        }
        else return FALSE;
    }    
    return TRUE;
}


BOOL DBInterface::GetChannelCalibTime(CString ch, int dauId, DBTIMESTAMP& time)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT calibTime FROM DAUSetupSensorChannel WHERE foreignID = %d And name = '%s' ORDER BY name ASC",dauId,ch);	
 
     COleDateTime oleTime;
            
     CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         if(!rs.IsEOF())
         {      
			 CDBVariant val;
			 rs.GetFieldValue("calibTime", val);
			 time = ToDBTimestamp(val.m_pdate);
         }         
         else return FALSE;
     }    
    return TRUE;
}


BOOL DBInterface::GetSensors(vector<Sensor*>& sensors, int id)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM DAUSetupSensorChannel WHERE foreignID = %d ORDER BY name ASC",id);	
 
     int nVal;
     CString strVal;
 
     CString name;
     int type,offset,pitchOffset,rollOffset;
     double pitchGain, rollGain;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
         {
			 CDBVariant val;
             rs.GetFieldValue("name", name);                    
             rs.GetFieldValue("type", val); 
			 type = val.m_iVal;
             rs.GetFieldValue("offset", val);
			 offset = val.m_iVal;
             rs.GetFieldValue("pitchOffset", val);
			 pitchOffset = val.m_iVal;
             rs.GetFieldValue("rollOffset", val);
			 rollOffset = val.m_iVal;
             rs.GetFieldValue("pitchGain", val);
			 pitchGain = val.m_dblVal;
             rs.GetFieldValue("rollGain", val);
			 rollGain = val.m_dblVal;
 
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
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM DAUSetupSyncroChannel WHERE foreignID = %d ORDER BY [order] ASC",id);	
 
     int nVal;
     CString strVal;
 
    
     CString name, type;
     int offset;
     double pitchGain, rollGain;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
         {
			 CDBVariant val;
             double refV = 0.0f;
             rs.GetFieldValue("name", name);                                
             rs.GetFieldValue("offset", val);
			 offset = val.m_iVal;
             try
             {
                 //May not exist
                
                 //rs.GetFieldInfo("refVoltage", &fldInfo);                    	
                 rs.GetFieldValue("refVoltage", val);     
				 refV = val.m_iVal;
                 //rs.GetFieldInfo("type", &fldInfo);                    	
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
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql = "SELECT DISTINCT serialNumber FROM CalibratedSensors ORDER BY serialNumber";	
 
     int nVal;
     CString strVal;
 

     CString serial;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
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
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
 	sql.Format("SELECT DISTINCT serialNumber FROM GunAdapterCalibration WHERE type=%d ORDER BY serialNumber",t);	
 
     int nVal;
     CString strVal;
 
    
     CString serial;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
         {
             rs.GetFieldValue("serialNumber", serial);                    
             adapters.push_back(serial);
 
             rs.MoveNext();
         }
     }
    return TRUE;
}

BOOL DBInterface::GetShip(CString name, Ship& ship)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql;
	sql.Format("SELECT * FROM Ships WHERE ShipName='%s'",name);
	

	CRecordset rs(&m_db);
	if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	{
		if(!rs.IsEOF())
		{
			CDBVariant val;
			rs.GetFieldValue("ShipID", val);
			ship.m_ID = val.m_iVal;
			rs.GetFieldValue("ShipName", ship.m_name);
			rs.GetFieldValue("ClassID", val);
			ship.m_classID = val.m_iVal;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}




BOOL DBInterface::GetShips(vector<Ship>& ships)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql = "SELECT * FROM Ships";	

    int nVal;
    CString strVal;

    Ship ship;
    
	CRecordset rs(&m_db);   
	if(rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
    {
        while(!rs.IsEOF())
        {
			CDBVariant val;
			rs.GetFieldValue("ShipID", val);
			ship.m_ID = val.m_iVal;
			rs.GetFieldValue("ShipName", ship.m_name);                    
			rs.GetFieldValue("ClassID", val);
			ship.m_classID = val.m_iVal;			
			
			ships.push_back(ship);

            rs.MoveNext();
        }
    }
    return TRUE;
}

BOOL DBInterface::GetStationsOnShip(vector<CString>& stations, CString ship)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT ClassID FROM Ships WHERE ShipName='%s'",ship);	
    
     int id;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         if(!rs.IsEOF())
         {
			 CDBVariant val;
             rs.GetFieldValue("ClassID", val);
			 id = val.m_iVal;
         }
     }
 
     sql.Format("SELECT StationName FROM StationPos WHERE ClassID=%d",id);	
     
     CString station;
	 CRecordset rs2(&m_db);
	 if (rs2.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs2.IsEOF())
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
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM GunAdapterCalibration WHERE serialNumber = '%s'",adaperSN);	
	 CString serial;
     int id;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	 {
         if(!rs.IsEOF())
         {
			 CDBVariant val;
			 rs.GetFieldValue("serialNumber", serial); 
			 rs.GetFieldValue("elevation", val);
			 data.m_elevation = val.m_dblVal;
             rs.GetFieldValue("azimuth", val);
			 data.m_azimuth = val.m_dblVal;
			 rs.GetFieldValue("time", val);
			 DBTIMESTAMP time = ToDBTimestamp(val.m_pdate);

         }
     }
 

    return TRUE;
}

BOOL DBInterface::GetSensorCalibrationData(CString calibName, CString SN, SensorTemperatureCalibrationData &data)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";

     sql.Format("SELECT a_0, a_1, a_2, a_3 FROM %s WHERE serialNumber ='%s'",calibName, SN);	
    
     int id;
     
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))	
     {
         if(!rs.IsEOF())
         {
			 CDBVariant val;
             rs.GetFieldValue("a_0", val);
			 data.m_offset = val.m_dblVal;
             rs.GetFieldValue("a_1", val);
			 data.m_linear = val.m_dblVal;
             rs.GetFieldValue("a_2", val);
			 data.m_quadratic = val.m_dblVal;
             rs.GetFieldValue("a_3", val);
			 data.m_cubic = val.m_dblVal;
         }
     }
    return TRUE;
}

BOOL DBInterface::GetSensorData(CString table, CString SN, vector<SelectedData>& data)
{
     data.clear();
 
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM %s WHERE serialNumber = '%s' ORDER BY timeStamp DESC",table, SN);	
     
     double date;
     COleDateTime oleTime;
 
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
         {
			 CDBVariant val;
             SelectedData selData;    
             rs.GetFieldValue("dauSerialNumber", selData.m_dauSerialNumber);                    
             rs.GetFieldValue("operator", selData.m_operatorName);                    
             rs.GetFieldValue("temperature", val);
			 selData.m_temperature = val.m_dblVal;
             rs.GetFieldValue("timeStamp", val);
			 selData.m_time = ToDBTimestamp(val.m_pdate);
             rs.GetFieldValue("calibrationValue", val);                    
			 selData.m_value = val.m_dblVal;
             data.push_back(selData);
         
             rs.MoveNext();
         }
     }
    return TRUE;    
}

BOOL DBInterface::GetPlatformCorrectionAngle(CString serial, double& alpha)
{
     if(!m_db.IsOpen())
         return FALSE;
     
     CString sql="";
     sql.Format("SELECT alpha FROM PlatformCorrection WHERE serialNumber = '%s'",serial);	
 
 
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	 {
         if(!rs.IsEOF())
         {
			 CDBVariant val;
             rs.GetFieldValue("alpha", val);
			 alpha = val.m_fltVal;
         }
         else return FALSE;
     }
    return TRUE;
}

BOOL DBInterface::GetUniqueSensorSN(CString table, vector<CString>& serial)
{
     serial.clear();
 
     if(!m_db.IsOpen())
         return FALSE;
     
     CString sql="";
     sql.Format("SELECT DISTINCT serialNumber FROM %s ORDER BY serialNumber",table);	
 
     CString sn;
         
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	 {
         while(!rs.IsEOF())
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
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM StationPos WHERE stationName = '%s'",station);	
    
     int id;
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         if(!rs.IsEOF())
         {
			 CDBVariant val;
             rs.GetFieldValue("StationX", val);
			 data.x = val.m_dblVal;
             rs.GetFieldValue("StationY", val);
			 data.y = val.m_dblVal;
             rs.GetFieldValue("StationZ", val);
			 data.z = val.m_dblVal;
             rs.GetFieldValue("SensorDeltaX", val);
			 data.dx = val.m_dblVal;
             rs.GetFieldValue("SensorDeltaY", val);
			 data.dy = val.m_dblVal;
             rs.GetFieldValue("SensorDeltaZ", val);
			 data.dz = val.m_dblVal;
         }
     }
    return TRUE;
}

BOOL DBInterface::UpdateAdapterCalibration(CString serial, double el, double az)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM GunAdapterCalibration WHERE serialNumber='%s'",serial);
 
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::executeDirect))
     {
         if(!rs.IsEOF())
         {
			 sql.Format("UPDATE GunAdapterCalibration SET azimuth = %10f, elevation = %.10f, [time] = '%s' WHERE serialNumber = '%s'", az, el, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), serial);
			 m_db.ExecuteSQL(sql);
		}
         else
         {
             sql.Format("INSERT INTO GunAdapterCalibration ( serialNumber, elevation, azimuth ) VALUES ('%s',%f,%f)",serial, el, az);
             m_db.ExecuteSQL(sql);    			 
         }
     }
    return TRUE;	
}

BOOL DBInterface::UpdateSensorCalibration(CString table, CString serial, CString op, Polynomial& fit)//double a0, double a1, double a2, double a3)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM %s WHERE serialNumber='%s'",table, serial);
 
    
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::executeDirect))
     {
         if(!rs.IsEOF())
         {
			 sql.Format("UPDATE %s SET [time] = '%s', operator = '%s', a_0 = %.11f, a_1 = %.11f, a_2 = %.11f, a_3 = %.11f WHERE serialNumber = '%s'", table, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), op, fit.m_a_0, fit.m_a_1, fit.m_a_2, fit.m_a_3, serial);
			 m_db.ExecuteSQL(sql);             
         }
         else 
         {
             sql.Format("INSERT INTO %s ( serialNumber, operator, a_0, a_1, a_2, a_3) VALUES ('%s','%s',%.11f,%.11f,%.11f,%.11f)",table, serial, op, fit.m_a_0, fit.m_a_1, fit.m_a_2, fit.m_a_3);
             m_db.ExecuteSQL(sql);
			 return TRUE;
		 }
     }
    return TRUE;	
}

BOOL DBInterface::UpdateDAUChannelGain(CString serialNumber, CString channel, DAUScaleCalibrationResult &result)
{
    if(!m_db.IsOpen())
        return FALSE;
 
     CString sql="";
     sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = %f, pitchGain = %f, calibTime = '%s' WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]='%s' And [DAUSetupSensorChannel].[name]='%s'",result.m_rollGain, result.m_pitchGain, COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S")), serialNumber, channel);
 
	 m_db.ExecuteSQL(sql);
	 return TRUE;
}

BOOL DBInterface::UpdateDAUChannelOffset(CString serialNumber, CString channel, DAUOffsetCalibrationResult &result)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = %d, pitchOffset = %d WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]='%s' And [DAUSetupSensorChannel].[name]='%s'",result.m_rollOffset, result.m_pitchOffset, serialNumber, channel);
     
	 m_db.ExecuteSQL(sql);
	 return TRUE;
}

BOOL DBInterface::AddSensorCalibrationA0(CString table, CString serial, CString op, double a0)
{
     SensorTemperatureCalibrationData data( 0, 0, 0, 0 );
     DBInterface::Instance()->GetSensorCalibrationData(table, serial, data) ;
 
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT * FROM %s WHERE serialNumber='%s'",table, serial);
      
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::executeDirect))
     {
         if(!rs.IsEOF())
         {
			 sql.Format("UPDATE %s SET [time] = '%s', operator = '%s', a_0 = %.11f WHERE serialNumber='%s'", table, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), op, data.m_offset + a0, serial);
			 m_db.ExecuteSQL(sql);
			 /*rs.Edit();				
             rs.SetFieldValue("time", COleDateTime::GetCurrentTime());			
             rs.SetFieldValue("operator", op);						
             rs.SetFieldValue("a_0", data.m_offset + a0);			
           
             rs.Update();*/		
         }
         else return FALSE;
     }
    return TRUE;	
}

BOOL DBInterface::InsertSensorData(CString table, CString dauSN, CString SN, CString op, double temp, double val)
{
    if(!m_db.IsOpen())
		return FALSE;
 
    CString sql="";
    sql.Format("INSERT INTO %s ( serialNumber, dauSerialNumber, operator, temperature, calibrationValue ) VALUES ('%s','%s','%s',%f,%f)",table, SN, dauSN, op, temp, val);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::DeleteRecord(CString table, int id)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("DELETE FROM %s WHERE ID = %d",table, id);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::ClearTable(CString table)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("DELETE FROM %s",table);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::GetLastCounter(int& id)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("SELECT @@IDENTITY");
       
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::executeDirect))
     {
         if(!rs.IsEOF())
         {                       
			 CDBVariant val;

             rs.GetFieldValue((short)0,val);
			 id = val.m_iVal;
         }
         else return FALSE;
     }
    return TRUE;
}

BOOL DBInterface::InsertHistoryPrintItem(int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO HistoryPrint ( historyID ) VALUES ( %d )",historyId);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

CString GetSigndefString()
{
    CString signdefText;
    signdefText.LoadString( (g_AlignerData.SignDef == -1) ? IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_NEG : IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_POS) ;
    return signdefText;
}


BOOL DBInterface::InsertHistoryItem(HistoryData& data)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     COleDateTime time(data.m_time);
     CString str;
     str.LoadString( IDS_ANGULAR_DEFINITION_PART_2 );
     CString sql="";    
     CString serial = DAU::GetDAU().GetSerialNumber();    
     int projectID = SysSetup->GetProjectID();
 
     sql.Format("INSERT INTO History ( dauSerialNumber, timeOfMeasurement, project, calibInfo ) VALUES ('%s','%s',%d,'%s')",
         serial, time.Format( _T("%Y-%m-%d %H:%M:%S")), projectID, data.calibInfo);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}
  
//A202
BOOL DBInterface::InsertHistory2Item(DBTIMESTAMP ts)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     COleDateTime time(ts);        
     CString sql="";
     CString op = SysSetup->GetOperatorName();
     CString serial = DAU::GetDAU().GetSerialNumber();
     CString place = SysSetup->GetPlace();
     CString project = SysSetup->GetProjectName();
 
     sql.Format("INSERT INTO History ( operator, dauSerialNumber, timeOfMeasurement, place, project ) VALUES ('%s','%s','%s','%s','%s')",
         op, serial, time.Format( _T("%Y-%m-%d %H:%M:%S")), place, project);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::UpdateComment(CString table, int historyId, CString comment)
{
     if(!m_db.IsOpen())
         return FALSE;
 
    CString sql="";
    sql.Format("UPDATE %s SET comment = '%s' WHERE historyID = %d",table, comment, historyId);        
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}


BOOL DBInterface::InsertTiltAlignmentErrors(TiltAlignmentErrorsHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
   
     CString sql="";
     sql.Format("INSERT INTO TiltAlignmentErrorsHistory (historyID, ship, timeConstant, lineOfSightDirection, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_lineOfSightDirection, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);    
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAlignmentErrorsItem(TiltAlignmentErrorsHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s)",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_bias));
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatness(TiltAndFlatnessHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit) VALUES (%d,'%s',%f,%d,'%s','%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_numberOfMeasurements, data.m_reference, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessItem(TiltAndFlatnessHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s,%s,%s)",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_standardDeviation), ToText(data.m_maximumDeviation), ToText(data.m_azimuth));
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessItemErr(TiltAndFlatnessHistory::ItemErrData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessHistoryItemErr (historyID, azimuth, error) VALUES (%d,%s,%s)",
         historyId, ToText(data.m_azimuth), ToText(data.m_error));
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFo(TiltAndFlatnessFoHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFoHistory (historyID, ship, timeConstant, numberOfMeasurement, reference, comment, measuredUnit, IndexArmLength) VALUES (%d,'%s',%f,%d,'%s','%s','%s',%f)",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_numberOfMeasurements, data.m_reference, data.m_comment, data.m_measuredUnit, data.m_IndexArmLength);
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFoItem(TiltAndFlatnessFoHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFoHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, tilt, angle, elevation1, elevation2, standardDeviation, bottomError, maximumDeviation, azimuth, IndexArmLength, IndexArm2Length, refstation, refchannel, refsensorSerialNumber) VALUES (%d,'%s','%s','%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,'%s','%s','%s')",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_roll, data.m_pitch, data.m_tilt, data.m_angle, data.m_elevation1, data.m_elevation2, data.m_standardDeviation, data.m_bottomError, data.m_maximumDeviation, data.m_azimuth, data.m_IndexArmLength, data.m_IndexArm2Length, data.m_refstation, data.m_refchannel, data.m_refsensorSerialNumber);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFoItemErr(TiltAndFlatnessFoHistory::ItemErrData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFoHistoryItemErr (historyID, azimuth, error1, error2, dh) VALUES (%d,%s,%s,%s,%s)",
         historyId, ToText(data.m_azimuth), ToText(data.m_error1), ToText(data.m_error2), ToText(data.m_dh));
 
    m_db.ExecuteSQL(sql);  	

	return TRUE;
}

BOOL DBInterface::InsertGyroPerformanceTest(GyroPerformanceTestHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO GyroPerformanceTestHistory (historyID, ship, timeConstant, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertGyroPerformanceTestItem(GyroPerformanceTestHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO GyroPerformanceTestHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s)",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle));
 
    m_db.ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertAzimuthAlignmentErrors(AzimuthAlignmentErrorsHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthAlignmentErrorsHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
             historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertAzimuthAlignmentErrorsItem(AzimuthAlignmentErrorsHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthAlignmentErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s)",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));
     
    m_db.ExecuteSQL(sql); 
	return TRUE;
} 

BOOL DBInterface::InsertAzimuthVerificationBenchmark(AzimuthVerificationBenchmarkHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthVerificationBenchmarkHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertAzimuthVerificationBenchmarkItem(AzimuthVerificationBenchmarkHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthVerificationBenchmarkHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s)",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertAzimuthVerificationGyrostability(AzimuthVerificationGyrostabilityHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthVerificationGyrostabilityHistory (historyID, ship, timeConstant, rollExcentricity, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_rollExcentricity, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertAzimuthVerificationGyrostabilityItem(AzimuthVerificationGyrostabilityHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthVerificationGyrostabilityHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s','%s','%s',%f,%s,%s,%s) ",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_nominalAzimuth, ToText(data.m_nominalAzimuthdifference), ToText(data.m_measuredAzimuthDifference), ToText(data.m_measuredNominalDifference));
 
    m_db.ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertHorizonAbsoluteMode(HorizonAbsoluteModeHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO HorizonAbsoluteModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_parallaxCompensation, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);
 
     m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonAbsoluteModeItem(HorizonAbsoluteModeHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO HorizonAbsoluteModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s','%s','%s',%f,%f,%f,%f,%f,%f,%f,%f) ",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, data.m_roll, data.m_pitch, data.m_tilt, data.m_angle, data.m_elevation, data.m_standardDeviation, data.m_maximumDeviation, data.m_azimuth);
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonRelativeMode(HorizonRelativeModeHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO HorizonRelativeModeHistory (historyID, ship, timeConstant, parallaxCompensation, elevationCompensation, comment, measuredUnit) VALUES (%d,'%s',%f,'%s','%s','%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_parallaxCompensation, data.m_elevationCompensation, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonRelativeModeItem(HorizonRelativeModeHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
     return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO HorizonRelativeModeHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s,%s,%s,%s) ",
             historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_elevation), ToText(data.m_standardDeviation), ToText(data.m_maximumDeviation), ToText(data.m_azimuth));
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::UpdateCalibrationFlag(CString table, int id, BOOL b)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("UPDATE %s SET dbUpdated = %s WHERE historyId = %d", table, b ? "True" : "False", id );		

	m_db.ExecuteSQL(sql);

	return TRUE;
}

BOOL DBInterface::InsertCommonFlatTilt(CommonFlatTiltHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO CommonFlatTiltHistory (historyID, ship, timeConstant, comment, measuredUnit, dbUpdated ) VALUES (%d,'%s',%f,'%s','%s', False)",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_comment, data.m_measuredUnit );
 
    m_db.ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertCommonFlatTiltItem(CommonFlatTiltHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO CommonFlatTiltHistoryItem (historyID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (%d,'%s','%s','%s',%s,%s,%f,%f,%f) ",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), data.m_parallellBias, data.m_perpendicularBias, data.m_temperature);
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertSensorValidation(SensorValidationHistory::Data data, int historyId)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("INSERT INTO SensorValidationHistory (historyID, ship, timeConstant, comment, measuredUnit, dbUpdated ) VALUES (%d,'%s',%f,'%s','%s', False)",
		historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_comment, data.m_measuredUnit);

	m_db.ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertSensorValidationItem(SensorValidationHistory::ItemData data, int historyId)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("INSERT INTO SensorValidationHistoryItem (historyID, station, channel, sensorSerialNumber, rollSF, pitchSF, rollAzErr, pitchAzErr, temperature ) VALUES (%d,'%s','%s','%s',%s,%s,%s,%s,%f) ",
		historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, ToText(data.m_rollSc), ToText(data.m_pitchSc), ToText(data.m_rollAzErr), ToText(data.m_pitchAzErr), data.m_temperature);

	m_db.ExecuteSQL(sql);
	return TRUE;
}


BOOL DBInterface::InsertLiveGraphErrors(LiveGraphErrorsHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveGraphErrorsHistory (historyID, ship, timeConstant, samplingRate, comment, measuredUnit) VALUES (%d,'%s',%f,%f,'%s','%s')",
         historyId, SysSetup->GetShipName(), data.m_timeConstant, data.m_samplingRate, data.m_comment, data.m_measuredUnit);
 
    m_db.ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertLiveGraphErrorsItem(LiveGraphErrorsHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveGraphErrorsHistoryItem (historyID, station, channel, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (%d,'%s','%s','%s','%s',%s,%s,%s,%s,%s) ",
         historyId, data.m_station, data.m_channel, data.m_sensorSerialNumber, data.m_adapterSerialNumber, ToText(data.m_roll), ToText(data.m_pitch), ToText(data.m_tilt), ToText(data.m_angle), ToText(data.m_temperature));
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertLiveDataA202Errors(LiveDataA202ErrorsHistory::Data data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveDataA202ErrorsHistory ([historyID], [ship], [samplingRate], [comment]) VALUES (%d,'%s',%f,'%s')",
         historyId, SysSetup->GetShipName(), data.m_samplingRate, data.m_comment);
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}


BOOL DBInterface::InsertLiveDataA202ErrorsItem(LiveDataA202ErrorsHistory::ItemData data, int historyId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveDataA202ErrorsHistoryItem ([historyID], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (%d,'%s','%s',%f,%f,%f) ",
         historyId, data.m_station, data.m_channel, data.m_value, data.m_gearing, data.m_refVoltage);
 
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertGraph(int historyId, CString file, int include)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO GraphHistory (historyID, filename, include ) VALUES (%d,'%s',%d)",historyId, file, include);           
	 TRACE(sql + "\n");
    m_db.ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::DeleteGraph(CString file)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("DELETE FROM GraphHistory WHERE filename='%s'",file);        
	 TRACE(sql + "\n");
    m_db.ExecuteSQL(sql);
	return TRUE;
}


