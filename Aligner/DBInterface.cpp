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

	

	CRecordset rs(&m_db);
	//rs.
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

double Chk(double d)
{	
	if(_isnan(d))
	{
		return 0.0f;
	}
	
	return d;
}


CString DBInterface::ToText(double d)
{
    CString str;    
    if(d == _HUGE)
    {
        return "0.0";
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

BOOL DBInterface::ExecuteSQL(CString& sql)
{
	try
	{
		m_db.BeginTrans();
		m_db.ExecuteSQL(sql);
		m_db.CommitTrans();
	}
	catch (CDBException* e)
	{
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return FALSE;
	}	
	return TRUE;
}

BOOL DBInterface::InsertProjectCalibration(ProjectData& project)
{
	if (!m_db.IsOpen())
		return FALSE;

	COleDateTime time(project.m_time);

	CString sql = "";
	sql.Format("INSERT INTO ProjectCalibration( name, dauSerial, operator, location, latitude, projTime, signDef, platformSN ) VALUES ('%s',%d, '%s','%s', %.2f, '%s', %d, %d)",
		project.m_projectName, SysSetup->GetDAUSerial(), project.m_operatorName, project.m_location, project.m_latitude, time.Format(_T("%Y-%m-%d %H:%M:%S")), project.m_signDef, project.m_platform.m_sn);

	ExecuteSQL(sql);

	int id;
	GetLastCounter(id);
	SysSetup->SetProjectID(id);

	return TRUE;
}

BOOL DBInterface::ProjectExist(CString name, CString table)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql = "";
	sql.Format("SELECT name FROM %s WHERE name='%s'", table, name);


	CRecordset rs(&m_db);
	if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::executeDirect))
	{
		if (!rs.IsEOF())
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL DBInterface::InsertProject(ProjectData& project)
{
	if (!m_db.IsOpen())
		return FALSE;

	COleDateTime time(project.m_time);

	CString sql = "";
	sql.Format("INSERT INTO Project( name, dauSerial, operator, location, shipId, latitude, unit, imgIdx, projTime, signDef ) VALUES ('%s',%d, '%s','%s', %d, %.2f, %d, %d, '%s', %d)",
		project.m_projectName, SysSetup->GetDAUSerial(), project.m_operatorName, project.m_location, project.m_shipID, project.m_latitude, project.m_unit, 0, time.Format(_T("%Y-%m-%d %H:%M:%S")), project.m_signDef);
	
	ExecuteSQL(sql);
	
	int id;
	GetLastCounter(id);
	SysSetup->SetProjectID(id);

	return TRUE;
}

BOOL DBInterface::UpdateProjectConfig(int projectID, CString xml, int mode)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString table = (mode == SYSTEM_SETUP_MODE_ALIGNMENT) ? "Project" : "ProjectCalibration";
	CString sql;
	sql.Format("UPDATE %s SET Config='%s' WHERE ID=%d",table,xml,projectID);

	ExecuteSQL(sql);

	return true;
}

BOOL DBInterface::GetCalibrationProjects(vector<ProjectData>& projects)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql = "";
	sql.Format("SELECT * FROM ProjectCalibration");

	ProjectData data;

	CRecordset rs(&m_db);
	if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	{
		//TRACE("count: %d, %d\n", rs.GetRecordCount(), rs.GetODBCFieldCount());
		/*for (int i = 0; i < rs.GetODBCFieldCount(); i++)
		{
			CODBCFieldInfo info;
			rs.GetODBCFieldInfo(i, info);
			TRACE("%d, %s\n", i, info.m_strName);
		}*/

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
			data.m_latitude = val.m_dblVal;
			
			rs.GetFieldValue("signDef", val);
			data.m_signDef = val.m_iVal;
			
			rs.GetFieldValue("platformSN", val);
			data.m_platform.m_sn = val.m_iVal;

			CString str;
			rs.GetFieldValue("config", data.m_config);
			
			data.m_mode = SYSTEM_SETUP_MODE_CALIBRATION;
			projects.push_back(data);

			rs.MoveNext();
		}
	}
	return true;
}

BOOL DBInterface::GetProjects(vector<ProjectData>& projects)
{
	projects.clear();

	if (!m_db.IsOpen())
		return FALSE;

	CString sql = "";
	sql.Format("SELECT * FROM Project INNER JOIN Ship ON Project.shipID = Ship.ShipID");

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
			data.m_latitude = val.m_dblVal;
			rs.GetFieldValue("unit", val);
			data.m_unit = val.m_iVal;
			rs.GetFieldValue("signDef", val);
			data.m_signDef = val.m_iVal;
			rs.GetFieldValue("ShipName", data.m_shipName);
			rs.GetFieldValue("ClassID", val);
			data.m_shipClass = val.m_iVal;
			CString str;
			rs.GetFieldValue("config", data.m_config);
			rs.GetFieldValue("imgIdx", val);
			data.m_imgIdx = val.m_iVal;
			data.m_mode = SYSTEM_SETUP_MODE_ALIGNMENT;
			projects.push_back(data);
			
			rs.MoveNext();
		}

		
		

		for (auto iter = projects.begin(); iter != projects.end(); iter++)
		{
			sql.Format("SELECT COUNT(*) from Measurement where projectID = %d",iter->m_projectID);
			CRecordset crs(&m_db);
			if (crs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
			{
				for (int i = 0; i < crs.GetODBCFieldCount(); i++)
				{
					CODBCFieldInfo info;
					crs.GetODBCFieldInfo(i, info);
					TRACE("%d, %s\n", i, info.m_strName);
				}

				CDBVariant val;
				crs.GetFieldValue((short)0, val, DEFAULT_FIELD_TYPE);
				iter->m_numMeasurements = val.m_iVal;
			}
		}

	}


	return true;
}

BOOL DBInterface::GetDAUData(int DAUSerial, DAUSetupData& dauData)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql="";
    sql.Format("SELECT * FROM DAUSetup WHERE serialNumber = %d ",DAUSerial);	
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
          //rs.GetFieldValue("serialNumber", dauData.serialStr);                    
          rs.GetFieldValue("serialNumber", val);
			    dauData.serial = val.m_iVal;
			    dauData.serialStr.Format("%03d", dauData.serial);
          rs.GetFieldValue("meta", val);
          dauData.meta = *val.m_pstring;

           /* rs.GetFieldValue("protocolVersion", val);
			dauData.protocolVersion  = val.m_iVal;
            rs.GetFieldValue("sampleAndHold", val);
			dauData.sampleAndHold = val.m_iVal;*/
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
 
            CDBVariant t;
            rs.GetFieldValue("calibTime", t);
            DBTIMESTAMP time = ToDBTimestamp(t.m_pdate);

            TRACE("Name:%s",name);
             
             Sensor *pSensor = new Sensor( name, type ) ;
             sensors.push_back( pSensor ) ;
             pSensor->SetOffset( offset ) ;
             ChannelCalibrationData roll(rollOffset, rollGain, time) ;
             ChannelCalibrationData pitch(pitchOffset, pitchGain, time) ;
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
     sql.Format("SELECT * FROM DAUSetupSyncroChannel WHERE foreignID = %d",id);	
	 //sql.Format("SELECT * FROM DAUSetupSyncroChannel WHERE foreignID = %d ORDER BY [order] ASC", id);
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
 
	CString table = t == UnitType::Types::Gun ? "GunAdapterCalibration" : "TheoAdapterCalibration";
	CString sql="";
	sql.Format("SELECT DISTINCT serialNumber FROM %s ORDER BY serialNumber",table);	
 
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
	sql.Format("SELECT * FROM Ship WHERE ShipName='%s'",name);
	

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



BOOL DBInterface::GetPlatforms(vector<Platform>& platforms)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql = "SELECT * FROM PlatformCorrection";

	int nVal;
	CString strVal;

	Platform platform;

	CRecordset rs(&m_db);
	if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
	{
		while (!rs.IsEOF())
		{
			CDBVariant val;
			rs.GetFieldValue("Id", val);
			platform.m_ID = val.m_iVal;
			rs.GetFieldValue("serialNumber", val);
			platform.m_sn = val.m_iVal;
			rs.GetFieldValue("alpha", val);
			platform.m_alpha= val.m_fltVal;
			rs.GetFieldValue("time", val);
			platform.m_time = ToDBTimestamp(val.m_pdate);

			platforms.push_back(platform);

			rs.MoveNext();
		}
	}
	return TRUE;
}


BOOL DBInterface::GetShips(vector<Ship>& ships)
{
    if(!m_db.IsOpen())
        return FALSE;

    CString sql = "SELECT * FROM Ship";	

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
     sql.Format("SELECT ClassID FROM Ship WHERE ShipName='%s'",ship);	
    
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
            data.m_time = ToDBTimestamp(val.m_pdate);

         }
     }
 

    return TRUE;
}

BOOL DBInterface::GetSensorCalibrationData(CString calibName, CString SN, SensorTemperatureCalibrationData &data)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";

     sql.Format("SELECT * FROM %s WHERE serialNumber ='%s'",calibName, SN);	
    
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
            rs.GetFieldValue("time", val);
            data.m_time = ToDBTimestamp(val.m_pdate);
         }
     }
    return TRUE;
}

BOOL DBInterface::GetSensorData(CString table, CString SN, vector<SelectedData>& data, CString projName)
{
     data.clear();
 
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
	 if (projName != "")
	 {
		 sql.Format("SELECT * FROM %s LEFT JOIN ProjectCalibration ON %s.calProjId = ProjectCalibration.ID WHERE serialNumber = '%s' AND ProjectCalibration.name = '%s' ORDER BY timeStamp DESC", table, table, SN, projName);
	 }
	 else
	 {
		 sql.Format("SELECT * FROM %s LEFT JOIN ProjectCalibration ON %s.calProjId = ProjectCalibration.ID WHERE serialNumber = '%s' ORDER BY timeStamp DESC", table, table, SN);
	 }
	 //sql.Format("SELECT * FROM %s WHERE serialNumber = '%s' ORDER BY timeStamp DESC", table, SN);
	 
     double date;
     COleDateTime oleTime;
 
	 CRecordset rs(&m_db);
	 if (rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
     {
         while(!rs.IsEOF())
         {
			 CDBVariant val;
             SelectedData selData;    
             rs.GetFieldValue("name", selData.m_projName);                    
			 rs.GetFieldValue("dauSerial", selData.m_dauSerialNumber);
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
			 sql.Format("UPDATE GunAdapterCalibration SET azimuth = %10f, elevation = %.10f, [time] = '%s', calProjId = %d WHERE serialNumber = '%s'", Chk(az), Chk(el), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), SysSetup->GetProjectID(), serial);
			 ExecuteSQL(sql);
		}
         else
         {
             sql.Format("INSERT INTO GunAdapterCalibration ( serialNumber, elevation, azimuth, calProjId ) VALUES ('%s',%f,%f,%d)",serial, Chk(el), Chk(az), SysSetup->GetProjectID());
             ExecuteSQL(sql);    			 
         }
     }
    return TRUE;	
}

BOOL DBInterface::UpdateSensorCalibration(CString table, CString serial, Polynomial& fit)//double a0, double a1, double a2, double a3)
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
			 sql.Format("UPDATE %s SET [time] = '%s', a_0 = %.11f, a_1 = %.11f, a_2 = %.11f, a_3 = %.11f, calProjId = %d WHERE serialNumber = '%s'", table, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), Chk(fit.m_a_0), Chk(fit.m_a_1), Chk(fit.m_a_2), Chk(fit.m_a_3), SysSetup->GetProjectID(), serial);
			 ExecuteSQL(sql);             
         }
         else 
         {
             sql.Format("INSERT INTO %s ( serialNumber, a_0, a_1, a_2, a_3, calProjId) VALUES ('%s',%.11f,%.11f,%.11f,%.11f,%d)",table, serial, Chk(fit.m_a_0), Chk(fit.m_a_1), Chk(fit.m_a_2), Chk(fit.m_a_3), SysSetup->GetProjectID());
             ExecuteSQL(sql);
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
     sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollGain = %f, pitchGain = %f, calibTime = '%s' WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=%s And [DAUSetupSensorChannel].[name]='%s'", Chk(result.m_rollGain), Chk(result.m_pitchGain), COleDateTime::GetCurrentTime().Format( _T("%Y-%m-%d %H:%M:%S")), serialNumber, channel);
 
	 ExecuteSQL(sql);
	 return TRUE;
}

BOOL DBInterface::UpdateDAUChannelOffset(CString serialNumber, CString channel, DAUOffsetCalibrationResult &result)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("UPDATE DAUSetupSensorChannel, DAUSetup SET rollOffset = %f, pitchOffset = %f WHERE [DAUSetup].[id] = foreignId And [DAUSetup].[serialNumber]=%s And [DAUSetupSensorChannel].[name]='%s'", Chk(result.m_rollOffset), Chk(result.m_pitchOffset), serialNumber, channel);
     
	 ExecuteSQL(sql);
	 return TRUE;
}

BOOL DBInterface::AddSensorCalibrationA0(CString table, CString serial, double a0)
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
			 sql.Format("UPDATE %s SET [time] = '%s', a_0 = %.11f WHERE serialNumber='%s'", table, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), data.m_offset + a0, serial);
			 ExecuteSQL(sql);
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

BOOL DBInterface::InsertSensorData(CString table, CString SN, double temp, double val)
{
    if(!m_db.IsOpen())
		return FALSE;
 
    CString sql="";
    sql.Format("INSERT INTO %s ( serialNumber, temperature, calibrationValue, calProjId ) VALUES ('%s',%f,%f,%d)",table, SN, temp, val, SysSetup->GetProjectID());
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::DeleteRecord(CString table, int id)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("DELETE FROM %s WHERE ID = %d",table, id);
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::ClearTable(CString table)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("DELETE FROM %s",table);
 
    ExecuteSQL(sql);  	
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
 
    ExecuteSQL(sql);  	
	return TRUE;
}

CString GetSigndefString()
{
    CString signdefText;
    signdefText.LoadString( (g_AlignerData.SignDef == -1) ? IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_NEG : IDS_ANGULAR_DEFINITION_PART_1_SIGNDEF_POS) ;
    return signdefText;
}

 
BOOL DBInterface::InsertMeasurement(MeasurementBase& data)
{
	if (!m_db.IsOpen())
		return FALSE;

	COleDateTime time(data.m_time);
	CString sql = "";
	CString serial = DAU::GetDAU().GetSerialNumber();
	int projectID = SysSetup->GetProjectID();

	sql.Format("INSERT INTO Measurement ( projectID, timeOfMeasurement, timeConstant, calibInfo, measType ) VALUES (%d, '%s', %f, '%s', %d)",
		projectID, time.Format(_T("%Y-%m-%d %H:%M:%S")), data.m_timeConstant, data.calibInfo, data.type);

	ExecuteSQL(sql);
	return TRUE;
}



BOOL DBInterface::UpdateComment(CString table, int historyId, CString comment)
{
     if(!m_db.IsOpen())
         return FALSE;
 
    CString sql="";
    sql.Format("UPDATE %s SET comment = '%s' WHERE historyID = %d",table, comment, historyId);        
 
    ExecuteSQL(sql);  	
	return TRUE;
}
 

BOOL DBInterface::InsertTiltAlignment(TiltAlignment::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
   
     CString sql="";
     sql.Format("INSERT INTO TiltAlignment (measID, referenceChannel, lineOfSightDirection, elevationCompensation) VALUES (%d,'%s','%s',%d)",
         measId, data.m_refChannel, data.m_lineOfSightDirection, data.m_elevationCompensation);    
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAlignmentChannel(TiltAlignment::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAlignmentChannel(foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, bias ) VALUES (%d,'%s','%s', %d, '%s','%s',%f,%f,%f,%f,%f,%f)",
		 foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_elevation), Chk(data.m_bias));
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatness(TiltAndFlatness::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatness (measID, numberOfMeasurement, referenceChannel) VALUES (%d,%d,'%s')",
         measId, data.m_numberOfMeasurements, data.m_refChannel);
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessChannel(TiltAndFlatness::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessChannel (foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f,%f,%f,%f,%f)",
         foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_elevation), Chk(data.m_standardDeviation), Chk(data.m_maximumDeviation), Chk(data.m_azimuth));
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessChannelErr(TiltAndFlatness::ChannelErrData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessChannelErr (foreignID, azimuth, error) VALUES (%d,%f,%f)",
         foreignId, Chk(data.m_azimuth), Chk(data.m_error));
 
    ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFo(TiltAndFlatnessFo::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFo (measID, numberOfMeasurement, referenceChannel) VALUES (%d,%d,'%s')",
         measId, data.m_numberOfMeasurements, data.m_refChannel);
 
    ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFoChannel(TiltAndFlatnessFo::ChannelData data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFoChannel (foreignID, station, channel, type, sensorSerialNumber, roll, pitch, tilt, angle, elevation1, elevation2, standardDeviation, bottomError, maximumDeviation, azimuth) VALUES (%d,'%s','%s',%d,'%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
         measId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_elevation), Chk(data.m_elevation2), Chk(data.m_standardDeviation), Chk(data.m_bottomError), Chk(data.m_maximumDeviation), Chk(data.m_azimuth));
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertTiltAndFlatnessFoChannelErr(TiltAndFlatnessFo::ChannelErrData data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO TiltAndFlatnessFoChannelErr (foreignID, azimuth, IndexArmLength1, IndexArmLength2, error1, error2, dh) VALUES (%d,%f,%f,%f,%f,%f,%f)",
         measId, Chk(data.m_azimuth), data.m_indexArmLength1, data.m_indexArmLength2, Chk(data.m_error), Chk(data.m_error2), Chk(data.m_dh));
 
    ExecuteSQL(sql);  	

	return TRUE;
}

BOOL DBInterface::InsertGyroPerformance(GyroPerformance::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO GyroPerformance (measID, referenceChannel) VALUES (%d,'%s')",
         measId, data.m_refChannel);
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertGyroPerformanceChannel(GyroPerformance::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO GyroPerformanceChannel (foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle ) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f)",
         foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle));
 
    ExecuteSQL(sql);  	
	return TRUE;
}

BOOL DBInterface::InsertAzimuthAlignment(AzimuthAlignment::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthAlignment (measID, rollExcentricity, referenceChannel) VALUES (%d,%f,'%s')",
             measId, Chk(data.m_rollExcentricity), data.m_refChannel);
 
    ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertAzimuthAlignmentChannel(AzimuthAlignment::ChannelData data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO AzimuthAlignmentChannel (foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, nominalAzimuth, nominalAzimuthdifference, measuredAzimuthDifference, measuredNominalDifference ) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f)",
         measId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_nominalAzimuth), Chk(data.m_nominalAzimuthdifference), Chk(data.m_measuredAzimuthDifference), Chk(data.m_measuredNominalDifference));
     
    ExecuteSQL(sql); 
	return TRUE;
} 
/*
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
*/
BOOL DBInterface::InsertHorizonAbsoluteMode(HorizonAbsoluteMode::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO VerificationAbsoluteMode (measID, parallaxCompensation, elevationCompensation, targetDistance) VALUES (%d,%s,%s,%d)",
		 measId, data.m_parallaxCompensation?"True":"False", data.m_elevationCompensation ? "True" : "False", data.m_range);
 
     ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonAbsoluteModeChannel(HorizonAbsoluteMode::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO VerificationAbsoluteModeChannel (foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f,%f,%f,%f,%f) ",
		 foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_elevation), Chk(data.m_standardDeviation), Chk(data.m_maximumDeviation), Chk(data.m_azimuth));
 
    ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonRelativeMode(HorizonRelativeMode::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO VerificationRelativeMode (measID, referenceChannel, parallaxCompensation, elevationCompensation, targetDistance) VALUES (%d,'%s',%s,%s,%d)",
		 measId, data.m_refChannel, data.m_parallaxCompensation ? "True" : "False", data.m_elevationCompensation ? "True" : "False", data.m_range);
 
    ExecuteSQL(sql); 
	return TRUE;
}

BOOL DBInterface::InsertHorizonRelativeModeChannel(HorizonRelativeMode::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
     return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO VerificationRelativeModeChannel (foreignId, station, channel, type,  sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, elevation, standardDeviation, maximumDeviation, azimuth ) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f,%f,%f,%f,%f) ",
		 foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_elevation), Chk(data.m_standardDeviation), Chk(data.m_maximumDeviation), Chk(data.m_azimuth));
 
    ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::UpdateCalibrationFlag(CString table, int id, BOOL b)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("UPDATE %s SET dbUpdated = %s WHERE measID = %d", table, b ? "True" : "False", id );		

	ExecuteSQL(sql);

	return TRUE;
}

BOOL DBInterface::InsertCommonFlatTilt(CommonFlatTilt::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO CommonFlatTilt (measID, referenceChannel, dbUpdated ) VALUES (%d,'%s',False)",
         measId, data.m_refChannel);
 
    ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertCommonFlatTiltChannel(CommonFlatTilt::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO CommonFlatTiltChannel (foreignID, station, channel, sensorSerialNumber, roll, pitch, parallellBias, perpendicularBias, temperature ) VALUES (%d,'%s','%s','%s',%f,%f,%f,%f,%f) ",
		 foreignId, data.m_station, data.m_channel, data.m_sensorSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_parallellBias), Chk(data.m_perpendicularBias), Chk(data.m_temperature));
 
    ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertSensorValidation(SensorValidation::Data data, int measId)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("INSERT INTO SensorValidation (measID, referenceChannel, platformSN, dbUpdated ) VALUES (%d,'%s',%d,False)", measId, data.m_refChannel, data.platformSN);

	ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertSensorValidationChannel(SensorValidation::ChannelData data, int foreignId)
{
	if (!m_db.IsOpen())
		return FALSE;

	CString sql="";
	sql.Format("INSERT INTO SensorValidationChannel (foreignID, station, channel, sensorSerialNumber, rollSF, pitchSF, rollAzErr, pitchAzErr, temperature ) VALUES (%d,'%s','%s','%s',%f,%f,%f,%f,%f) ",
		foreignId, data.m_station, data.m_channel, data.m_sensorSerialNumber, Chk(data.m_rollSc), Chk(data.m_pitchSc), Chk(data.m_rollAzErr), Chk(data.m_pitchAzErr), Chk(data.m_temperature));

	ExecuteSQL(sql);
	return TRUE;
}


BOOL DBInterface::InsertLiveGraph(LiveGraph::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveGraph (measID, samplingRate) VALUES (%d,%f)",
		 measId, data.m_samplingRate);
 
    ExecuteSQL(sql);
	return TRUE;
}

BOOL DBInterface::InsertLiveGraphChannel(LiveGraph::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveGraphChannel (foreignID, station, channel, type, sensorSerialNumber, adapterSerialNumber, roll, pitch, tilt, angle, temperature ) VALUES (%d,'%s','%s',%d,'%s','%s',%f,%f,%f,%f,%f) ",
		 foreignId, data.m_station, data.m_channel, data.m_type, data.m_sensorSerialNumber, data.m_adapterSerialNumber, Chk(data.m_roll), Chk(data.m_pitch), Chk(data.m_tilt), Chk(data.m_angle), Chk(data.m_temperature));
 
    ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertLiveDataA202(LiveDataA202::Data data, int measId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveDataA202 ([historyID], [ship], [samplingRate], [comment]) VALUES (%d,'%s',%f,'%s')",
         measId, SysSetup->GetShipName(), Chk(data.m_samplingRate), data.m_comment);
 
    ExecuteSQL(sql);  
	return TRUE;
}


BOOL DBInterface::InsertLiveDataA202Channel(LiveDataA202::ChannelData data, int foreignId)
{
     if(!m_db.IsOpen())
         return FALSE;
 
     CString sql="";
     sql.Format("INSERT INTO LiveDataA202Channel([foreignId], [station], [channel], [measuredValue], [gearing], [refVoltage] ) VALUES (%d,'%s','%s',%f,%f,%f) ",
		 foreignId, data.m_station, data.m_channel, data.m_value, data.m_gearing, data.m_refVoltage);
 
    ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::InsertGraph(int measId, CString file, int include)
{
    if(!m_db.IsOpen())
		return FALSE;
 
    CString sql="";
    sql.Format("INSERT INTO Graph (measID, filename, include ) VALUES (%d,'%s',%d)", measId, file, include);           
	//TRACE(sql + "\n");
	ExecuteSQL(sql);  
	return TRUE;
}

BOOL DBInterface::DeleteGraph(CString file)
{
    if(!m_db.IsOpen())
        return FALSE;
 
    CString sql="";
    sql.Format("DELETE FROM Graph WHERE filename='%s'",file);        
	//TRACE(sql + "\n");
    ExecuteSQL(sql);
	return TRUE;
}


