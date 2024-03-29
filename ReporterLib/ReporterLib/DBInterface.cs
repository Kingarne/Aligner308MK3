﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Odbc;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace ReporterLib
{
    public class DBInterface
    {
        public class Project
        {
            public int ID { get; set; }
            public string Name{ get; set; }
            public DateTime Time { get; set; }
            public int DAUSerial { get; set; }
            public string Operator { get; set; }
            public string Location { get; set; }
            public string Ship { get; set; }
            public float Latitude{ get; set; }
            public string UnitText { get; set; }
            public int SignDef { get; set; }
        }

        public enum MeasType
        {
            MT_TiltAlignment = 1,
            MT_TiltFlatnessPl,
            MT_TiltFlatnessFo,
            MT_AzimuthAlign,
            MT_AzVerGyrostab,
            MT_AzVerBenchmark,           
            MT_VerifAbsolute,
            MT_VerifRelative,
            MT_CommonFlatTilt,
            MT_SensorValidation,
            MT_GyroPerf,
            MT_LiveGraph
        }

        public class ImageInfo
        {
            public int ID { get; set; }
            public string Path { get; set; }
            public bool Include { get; set; }
            public bool Drawn { get; set; }
            //Bitmap bitmap;
        }

        public class Measurement
        {
            public int ID { get; set; }
            public int ProjectID { get; set; }
            public DateTime Time { get; set; }            
            public string CalibInfo { get; set; }
            public MeasType Type { get; set; }
            public string TypeText { get; set; }
            public float TimeConstant{ get; set; }
            public string Comment { get; set; }
            public string RefChannel { get; set; }
        }

        public class AlignmentBase
        {
            public int ID { get; set; }
        }

        public class ChannelBase
        {
            public ChannelBase() { IsRef = false; }
            public int ID { get; set; }
            public int ForeignID { get; set; }
            public string Station { get; set; }
            public int Type { get; set; }
            public string TypeText { get; set; }
            public string Channel { get; set; }           
            public string SensorSN { get; set; }
            public string AdapterSN { get; set; }
            public float NominalAz { get; set; }
            public float roll { get; set; }
            public float pitch { get; set; }
            public float tilt { get; set; }
            public float angle { get; set; }
            public float elevation { get; set; }
            public bool IsRef { get; set; }

        }

        public class ChannelErrBase
        {
            public ChannelErrBase() { done = false; }
            public bool done; // for printing
            public int ID { get; set; }
            public int ForeignID { get; set; }
            public float azimuth { get; set; }
            public float error { get; set; }
        }

        [ClassInterface(ClassInterfaceType.None)]
        public class ChannelErrBaseList: List<ChannelErrBase>
        {

        }

        public class TiltAlignment : AlignmentBase
        {
            public string LOSDir { get; set; }
            public bool ElevComp { get; set; }
        }

        public class TiltAlignmentCh :  ChannelBase
        {          
            public float bias { get; set; }
        }

        public class AzimuthAlignment : AlignmentBase
        {
            public float RollExcentricity { get; set; }            
        }

        public class AzimuthAlignmentCh : ChannelBase
        {
            public float NominalAzimuthDiff { get; set; }
            public float MeasuredAzimuthDiff { get; set; }
            public float MeasuredNominalDiff { get; set; }
        }

        public class TiltFlatnessPl : AlignmentBase
        {
            public int numMeas { get; set; }
        }

        public class TiltFlatnessPlCh : ChannelBase
        {            
            public float stdDev { get; set; }
            public float maxDev { get; set; }            
            public float azimuth { get; set; }
        }

        public class TiltFlatnessPlChErr : ChannelErrBase
        {
           
        }

        public class TiltFlatnessFo : AlignmentBase
        {
            public int numMeas { get; set; }           
        }

        public class TiltFlatnessFoCh : ChannelBase
        {
            public float elevation2 { get; set; }
            public float stdDev { get; set; }
            public float maxDev { get; set; }
            public float bottomErr { get; set; }
            public float azimuth { get; set; }
        }

        public class TiltFlatnessFoChErr : ChannelErrBase
        {
            public float indexArmL1{ get; set; }
            public float indexArmL2{ get; set; }
            public float error2 { get; set; }
            public float dh{ get; set; }
        }

        public class GyroPerf : AlignmentBase
        {
            
        }

        public class GyroPerfCh : ChannelBase
        {

        }

        public class CommonFlat : AlignmentBase
        {
            public bool DBUpdated { get; set;}        
        }

        public class CommonFlatCh : ChannelBase
        {
            public double ParallellBias { get; set; }
            public double PerpendicularBias { get; set; }
            public double Temperature { get; set; }
        }

        public class AbsoulteModeVerif : AlignmentBase
        {
            public int range { get; set; }
            public bool parallaxComp { get; set; }
            public bool elevationComp { get; set; }
        }

        public class AbsoulteModeVerifCh : ChannelBase
        {
            public double stdDev { get; set; }
            public double maxDev { get; set; }
            public double azuimuth { get; set; }
        }

        public class RelativeModeVerif : AlignmentBase
        {
            public int range { get; set; }
            public bool parallaxComp { get; set; }
            public bool elevationComp { get; set; }
        }

        public class RelativeModeVerifCh : ChannelBase
        {
            public double stdDev { get; set; }
            public double maxDev { get; set; }
            public double azuimuth { get; set; }
        }

        public class SensorValidation: AlignmentBase
        {
            public int PlatformSN { get; set; }
            public bool DBUpdated { get; set; }
        }

        public class SensorValidationCh : ChannelBase
        {
            public double rollScale { get; set; }
            public double pitchScale { get; set; }
            public double rollAz { get; set; }
            public double pitchAz{ get; set; }
            public double temperature { get; set; }
        }       

        public class LiveGraph: AlignmentBase
        {
            public double sampleRate{ get; set; }            
        }

        public class LiveGraphCh : ChannelBase
        {
            public double temperature { get; set; }
        }

        //Calilbration
        public class DAUChData
        {
            public DateTime time;
            public string ch;
            public double pitchGain;
            public double rollGain;
            public int pitchOffs;
            public int rollOffs;
        }

        public class DAUData
        {            
            public bool done = false;
            public int ID;
            public int sn;
            public List<DAUChData> chData = new List<DAUChData>();
        }

        public enum CalType
        {
            CT_PitchAz = 0,
            CT_PitchGain,
            CT_PitchOffs,
            CT_RollAz,
            CT_RollGain,
            CT_RollOffs,
        }

        public class CalibData
        {
            public CalibData(CalType ct) { type = ct; }           
            public CalType type;
            public DateTime time { get; set; }
            public double[] a = new double[4];
            public int platformSN { get; set; }
        }        

        public class SensorCalibrationInfo
        {
            public SensorCalibrationInfo()
            {
                calData = new Dictionary<CalType, CalibData>();
                calData[CalType.CT_PitchAz] = new CalibData(CalType.CT_PitchAz);
                calData[CalType.CT_PitchGain] = new CalibData(CalType.CT_PitchGain);
                calData[CalType.CT_PitchOffs] = new CalibData(CalType.CT_PitchOffs);
                calData[CalType.CT_RollAz] = new CalibData(CalType.CT_RollAz);
                calData[CalType.CT_RollGain] = new CalibData(CalType.CT_RollGain);
                calData[CalType.CT_RollOffs] = new CalibData(CalType.CT_RollOffs);
            }
            public bool done = false;
            public string sn { get; set; }            
            public Dictionary<CalType, CalibData> calData{ get; set; }       
        }
               
        public class GunAdaptCalib
        {
            public bool done = false;
            public string sn { get; set; }
            public DateTime time { get; set; }
            public double elevation { get; set; }
            public double azimuth { get; set; }
        }

        public class TheoAdaptCalib
        {
            public bool done = false;
            public string sn { get; set; }
            public DateTime time { get; set; }
            public double elevation { get; set; }
            public double azimuth { get; set; }
            public int type { get; set; }
        }

        public class PlatformCorrection
        {
            public bool done = false;
            public int sn { get; set; }
            public DateTime time { get; set; }
            public double alpha { get; set; }        
        }




        private OdbcConnection Connection;
        public bool Open(string path)
        {
            Connection = new OdbcConnection();

            string file = path; 

            string myConnectionString = @"Driver={Microsoft Access Driver (*.mdb)};" + "Dbq="+file+";Uid=Admin;Pwd=;";
            Connection.ConnectionString = myConnectionString;
            try
            {
                Connection.Open();
            }
            catch (Exception e)
            {
                MessageBox.Show("Error ODBC: " + e.Message);
            }
                //execute queries, etc           

            return true;

        }

        public string GetUnitText(int unit)
        {
            return (unit == 0) ? "[mrad]" : "[arcmin]";

        }

        
        public bool GetProject(int projId, ref Project project)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Project INNER JOIN Ship ON Project.shipID = Ship.ShipID WHERE ID=" + projId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {

                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            dr.GetDataTypeName(i);
                        }

                        project.ID = (int)dr["ID"];
                        project.Name = (string)dr["name"];                      
                        project.DAUSerial = (int)dr["dauSerial"];
                        project.Operator = (string)dr["operator"];
                        project.Location = (string)dr["location"];
                        project.Ship = (string)dr["ShipName"];
                        project.Time = (DateTime)dr["projTime"];
                        project.Latitude= (float)(double)dr["latitude"];
                        project.SignDef = (int)dr["signDef"];
                        int unit = (int)dr["unit"];
                        project.UnitText = GetUnitText(unit);

                        Logger.Inst.Trace("ID: " + project.ID.ToString(), Logger.LogLevel.Level1);
                    }
                }
            }


            return true;
        }

        public bool GetProjectMeasurements(int projId, ref Dictionary<int, Measurement> measurements)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            measurements.Clear();

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Measurement INNER JOIN MeasType ON Measurement.measType = MeasType.measType WHERE projectID=" + projId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {

                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        Measurement measurement = new Measurement();

                        measurement.ID = (int)dr["ID"];
                        measurement.ProjectID = (int)dr["projectID"];                        
                        measurement.CalibInfo = (string)dr["calibInfo"];
                        measurement.Time = (DateTime)dr["timeOfMeasurement"];
                        measurement.Type = (MeasType)(int)dr["measType"];
                        measurement.TypeText = (string)dr["measTypeName"];
                        measurement.TimeConstant = (float)dr["timeConstant"];
                        measurement.Comment = (string)dr["comment"];

                        measurements[measurement.ID] = measurement;

                        Logger.Inst.Trace("ID: " + measurement.ID.ToString(), Logger.LogLevel.Level1);
                    }
                }
            }


            return true;
        }

        public bool UpdateComment(int measId, string comment)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            string sql = String.Format("UPDATE Measurement SET comment='{0}' WHERE ID={1}",comment, measId);            

            using (OdbcCommand command = new OdbcCommand(sql, Connection))
            {
                command.ExecuteNonQuery();

                
            }
            return true;

        }

        public bool DeleteMeasurement(int id)
        {           
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("DELETE FROM Measurement WHERE ID =" + id.ToString(), Connection))
            {
                command.ExecuteNonQuery();
            }
            return true;
        }


        public bool GetMeasurement(int measId, ref Measurement measurement)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Measurement INNER JOIN MeasType ON Measurement.measType = MeasType.measType WHERE ID=" + measId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                        for(int i=0;i< dr.FieldCount;i++)
                        {
                            string name = dr.GetName(i);
                        }

                        measurement.ID = (int)dr["ID"];
                        measurement.ProjectID = (int)dr["projectID"];                        
                        measurement.CalibInfo = (string)dr["calibInfo"];
                        measurement.Time = (DateTime)dr["timeOfMeasurement"];
                        measurement.Type = (MeasType)(int)dr["measType"];
                        measurement.TypeText = (string)dr["measTypeName"];
                        measurement.TimeConstant = (float)dr["timeConstant"];
                        measurement.Comment = (string)dr["comment"];
                        //measurement.Time = dr["timeOfMeasurement"];
                        //salvo.Time = Db2Time((int)time);


                        Logger.Inst.Trace("ID: " + measurement.ID.ToString(), Logger.LogLevel.Level1);
                        // Console.WriteLine(dr.FieldCount);
                        // Console.WriteLine(dr["ID"].ToString());
                    }
                }
            }


            return true;
        }


        public bool GetTiltAlignmentMeas(ref DBInterface.Measurement meas, ref TiltAlignment tam)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAlignment WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string t = dr.GetDataTypeName(i);
                        }

                        tam.ID = (int)dr["ID"];                                                
                        tam.LOSDir = (string)dr["lineOfSightDirection"];
                        tam.ElevComp = (bool)dr["elevationCompensation"];
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetTiltAlignmentCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAlignmentChannel INNER JOIN StationType ON TiltAlignmentChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.TiltAlignmentCh taCh = new DBInterface.TiltAlignmentCh();

                        taCh.ID = (int)dr["ID"];
                        taCh.ForeignID = (int)dr["foreignID"];
                        taCh.Type = (int)dr["type"];
                        taCh.TypeText = (string)dr["stationTypeName"];
                        taCh.Station = (string)dr["station"];
                        taCh.Channel = (string)dr["channel"];
                        taCh.SensorSN = (string)dr["sensorSerialNumber"];
                        taCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        taCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        taCh.roll = (float)(double)dr["roll"];
                        taCh.pitch = (float)(double)dr["pitch"];
                        taCh.tilt = (float)(double)dr["tilt"];
                        taCh.angle = (float)(double)dr["angle"];
                        taCh.elevation= (float)(double)dr["elevation"];
                        taCh.bias = (float)(double)dr["bias"];

                        channels.Add(taCh);
                    }
                }
            }

            return true;
        }

        public bool GetGyroPerfMeas(ref DBInterface.Measurement meas, ref GyroPerf tam)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM GyroPerformance WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string t = dr.GetDataTypeName(i);
                        }

                        tam.ID = (int)dr["ID"];                       
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetGyroPertCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM GyroPerformanceChannel INNER JOIN StationType ON GyroPerformanceChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.GyroPerfCh taCh = new DBInterface.GyroPerfCh();

                        taCh.ID = (int)dr["ID"];
                        taCh.ForeignID = (int)dr["foreignID"];
                        taCh.Type = (int)dr["type"];
                        taCh.TypeText = (string)dr["stationTypeName"];
                        taCh.Station = (string)dr["station"];
                        taCh.Channel = (string)dr["channel"];
                        taCh.SensorSN = (string)dr["sensorSerialNumber"];
                        taCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        taCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        taCh.roll = (float)(double)dr["roll"];
                        taCh.pitch = (float)(double)dr["pitch"];
                        taCh.tilt = (float)(double)dr["tilt"];
                        taCh.angle = (float)(double)dr["angle"];                                          

                        channels.Add(taCh);
                    }
                }
            }

            return true;
        }

        public bool GetSensorvalidationMeas(ref DBInterface.Measurement meas, ref SensorValidation svm)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorValidation WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {
                        svm.ID = (int)dr["ID"];
                        svm.DBUpdated = (bool)dr["dbUpdated"];
                        svm.PlatformSN = (int)dr["platformSN"]; 
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetSensorvalidationMeasCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorValidationChannel WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        DBInterface.SensorValidationCh svCh = new DBInterface.SensorValidationCh();

                        svCh.ID = (int)dr["ID"];
                        svCh.ForeignID = (int)dr["foreignID"];
                        svCh.Station = (string)dr["station"];
                        svCh.Channel = (string)dr["channel"];
                        svCh.SensorSN = (string)dr["sensorSerialNumber"];
                        svCh.rollScale = (float)(double)dr["rollSF"];
                        svCh.pitchScale= (float)(double)dr["pitchSF"];
                        svCh.rollAz = (float)(double)dr["rollAzErr"];
                        svCh.pitchAz = (float)(double)dr["pitchAzErr"];
                        svCh.temperature = (float)(double)dr["temperature"];

                        channels.Add(svCh);
                    }
                }
            }

            return true;
        }


        public bool GetCommonFlatMeas(ref DBInterface.Measurement meas, ref CommonFlat cfm)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM CommonFlatTilt WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {
                        cfm.ID = (int)dr["ID"];
                        cfm.DBUpdated  = (bool)dr["dbUpdated"];
                        string refCh = (string)dr["referenceChannel"];                        
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetCommonFlatCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;
            
            using (OdbcCommand command = new OdbcCommand("SELECT * FROM CommonFlatTiltChannel WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {                       
                        DBInterface.CommonFlatCh cfCh = new DBInterface.CommonFlatCh();

                        cfCh.ID = (int)dr["ID"];
                        cfCh.ForeignID = (int)dr["foreignID"];                    
                        cfCh.Station = (string)dr["station"];
                        cfCh.Channel = (string)dr["channel"];
                        cfCh.SensorSN = (string)dr["sensorSerialNumber"];
                        cfCh.roll = (float)(double)dr["roll"];
                        cfCh.pitch = (float)(double)dr["pitch"];
                        cfCh.ParallellBias = (float)(double)dr["parallellBias"];
                        cfCh.PerpendicularBias = (float)(double)dr["perpendicularBias"];
                        cfCh.Temperature = (float)(double)dr["temperature"];

                        channels.Add(cfCh);
                    }
                }
            }

            return true;
        }

        public bool GetAzimuthAlignmentMeas(ref DBInterface.Measurement meas, ref AzimuthAlignment tam)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM AzimuthAlignment WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string t = dr.GetDataTypeName(i);
                        }

                        tam.ID = (int)dr["ID"];
                        tam.RollExcentricity = (float)(double)dr["RollExcentricity"];
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetAzimuthAlignmentCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM AzimuthAlignmentChannel INNER JOIN StationType ON AzimuthAlignmentChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.AzimuthAlignmentCh aaCh = new DBInterface.AzimuthAlignmentCh();

                        aaCh.ID = (int)dr["ID"];
                        aaCh.ForeignID = (int)dr["foreignID"];
                        aaCh.Type = (int)dr["type"];
                        aaCh.TypeText = (string)dr["stationTypeName"];
                        aaCh.Station = (string)dr["station"];
                        aaCh.Channel = (string)dr["channel"];
                        aaCh.SensorSN = (string)dr["sensorSerialNumber"];
                        aaCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        aaCh.NominalAz = (float)(double)dr["nominalAzimuth"];
                        aaCh.NominalAzimuthDiff = (float)(double)dr["nominalAzimuthDifference"];
                        aaCh.MeasuredAzimuthDiff = (float)(double)dr["measuredAzimuthDifference"];
                        aaCh.MeasuredNominalDiff = (float)(double)dr["measuredNominalDifference"];

                        channels.Add(aaCh);
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessPlMeas(ref DBInterface.Measurement meas, ref TiltFlatnessPl tfp)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatness WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                       /* for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string t = dr.GetDataTypeName(i);
                        }*/

                        tfp.ID = (int)dr["ID"];
                        tfp.numMeas = (int)dr["numberOfMeasurement"];
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessPlCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatnessChannel INNER JOIN StationType ON TiltAndFlatnessChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        DBInterface.TiltFlatnessPlCh tfCh = new DBInterface.TiltFlatnessPlCh();

                        tfCh.ID = (int)dr["ID"];
                        tfCh.ForeignID = (int)dr["foreignID"];
                        tfCh.Type = (int)dr["type"];
                        tfCh.TypeText = (string)dr["stationTypeName"];
                        tfCh.Station = (string)dr["station"];
                        tfCh.Channel = (string)dr["channel"];
                        tfCh.SensorSN = (string)dr["sensorSerialNumber"];
                        //tfCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        tfCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        tfCh.roll = (float)(double)dr["roll"];
                        tfCh.pitch = (float)(double)dr["pitch"];
                        tfCh.tilt = (float)(double)dr["tilt"];
                        tfCh.angle = (float)(double)dr["angle"];
                        tfCh.elevation = (float)(double)dr["elevation"];                       
                        tfCh.stdDev = (float)(double)dr["standardDeviation"];                        
                        tfCh.maxDev = (float)(double)dr["maximumDeviation"];
                        tfCh.azimuth = (float)(double)dr["azimuth"];

                        channels.Add(tfCh);
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessPlChErr(int foreignId, DBInterface.ChannelErrBaseList channelErr)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatnessChannelErr WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        DBInterface.TiltFlatnessFoChErr tfeCh = new DBInterface.TiltFlatnessFoChErr();

                        tfeCh.ID = (int)dr["ID"];
                        tfeCh.ForeignID = (int)dr["foreignID"];
                        tfeCh.azimuth = (float)(double)dr["azimuth"];
                        tfeCh.error = (float)(double)dr["error"];                        

                        channelErr.Add(tfeCh);
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessFoMeas(ref DBInterface.Measurement meas, ref TiltFlatnessFo tff)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatnessFo WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {

                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string t = dr.GetDataTypeName(i);
                        }

                        tff.ID = (int)dr["ID"];
                        tff.numMeas = (int)dr["numberOfMeasurement"];                        
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessFoCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatnessFoChannel INNER JOIN StationType ON TiltAndFlatnessFoChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {                       
                        DBInterface.TiltFlatnessFoCh tfCh = new DBInterface.TiltFlatnessFoCh();

                        tfCh.ID = (int)dr["ID"];
                        tfCh.ForeignID = (int)dr["foreignID"];
                        tfCh.Type = (int)dr["type"];
                        tfCh.TypeText = (string)dr["stationTypeName"];
                        tfCh.Station = (string)dr["station"];
                        tfCh.Channel = (string)dr["channel"];
                        tfCh.SensorSN = (string)dr["sensorSerialNumber"];
                        //tfCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        tfCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        tfCh.roll = (float)(double)dr["roll"];
                        tfCh.pitch = (float)(double)dr["pitch"];
                        tfCh.tilt = (float)(double)dr["tilt"];
                        tfCh.angle = (float)(double)dr["angle"];
                        tfCh.elevation = (float)(double)dr["elevation1"];
                        tfCh.elevation2 = (float)(double)dr["elevation2"];
                        tfCh.stdDev = (float)(double)dr["standardDeviation"];
                        tfCh.bottomErr = (float)(double)dr["bottomError"];
                        tfCh.maxDev = (float)(double)dr["maximumDeviation"];
                        tfCh.azimuth = (float)(double)dr["azimuth"];

                        channels.Add(tfCh);
                    }
                }
            }

            return true;
        }

        public bool GetTiltFlatnessFoChErr(int foreignId, DBInterface.ChannelErrBaseList channelErr)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TiltAndFlatnessFoChannelErr WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        DBInterface.TiltFlatnessFoChErr tfeCh = new DBInterface.TiltFlatnessFoChErr();

                        tfeCh.ID = (int)dr["ID"];
                        tfeCh.ForeignID = (int)dr["foreignID"];
                        tfeCh.azimuth = (float)(double)dr["azimuth"]; 
                        tfeCh.indexArmL1 = (float)(double)dr["IndexArmLength1"];
                        tfeCh.indexArmL2 = (float)(double)dr["IndexArmLength2"];
                        tfeCh.error = (float)(double)dr["error1"];
                        tfeCh.error2 = (float)(double)dr["error2"];
                        tfeCh.dh = (float)(double)dr["dh"];

                        channelErr.Add(tfeCh);
                    }
                }
            }

            return true;
        }

        public bool GetAboluteModeMeas(ref DBInterface.Measurement meas, ref AbsoulteModeVerif amv)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM VerificationAbsoluteMode WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {
                        amv.ID = (int)dr["ID"];
                        amv.parallaxComp = (bool)dr["parallaxCompensation"];
                        amv.elevationComp = (bool)dr["elevationCompensation"];
                        amv.range = (int)dr["targetDistance"];
                    }
                }
            }

            return true;
        }

        public bool GetAboluteModeCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM VerificationAbsoluteModeChannel INNER JOIN StationType ON VerificationAbsoluteModeChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.AbsoulteModeVerifCh amvCh = new DBInterface.AbsoulteModeVerifCh();

                        amvCh.ID = (int)dr["ID"];
                        amvCh.ForeignID = (int)dr["foreignID"];
                        amvCh.Type = (int)dr["type"];
                        amvCh.TypeText = (string)dr["stationTypeName"];
                        amvCh.Station = (string)dr["station"];
                        amvCh.Channel = (string)dr["channel"];
                        amvCh.SensorSN = (string)dr["sensorSerialNumber"];
                        amvCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        amvCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        amvCh.roll = (float)(double)dr["roll"];
                        amvCh.pitch = (float)(double)dr["pitch"];
                        amvCh.tilt = (float)(double)dr["tilt"];
                        amvCh.angle = (float)(double)dr["angle"];
                        amvCh.elevation = (float)(double)dr["elevation"];
                        amvCh.stdDev = (float)(double)dr["standardDeviation"];
                        amvCh.maxDev = (float)(double)dr["maximumDeviation"];
                        amvCh.azuimuth = (float)(double)dr["azimuth"];

                        channels.Add(amvCh);
                    }
                }
            }

            return true;
        }

        public bool GetRelativeModeMeas(ref DBInterface.Measurement meas, ref RelativeModeVerif rmv)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM VerificationRelativeMode WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {
                        rmv.ID = (int)dr["ID"];
                        rmv.parallaxComp = (bool)dr["parallaxCompensation"];
                        rmv.elevationComp = (bool)dr["elevationCompensation"];
                        rmv.range = (int)dr["targetDistance"];
                        string refCh = (string)dr["referenceChannel"];
                        meas.RefChannel = refCh;
                    }
                }
            }

            return true;
        }

        public bool GetRelativeModeCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM VerificationRelativeModeChannel INNER JOIN StationType ON VerificationRelativeModeChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.RelativeModeVerifCh amvCh = new DBInterface.RelativeModeVerifCh();

                        amvCh.ID = (int)dr["ID"];
                        amvCh.ForeignID = (int)dr["foreignID"];
                        amvCh.Type = (int)dr["type"];
                        amvCh.TypeText = (string)dr["stationTypeName"];
                        amvCh.Station = (string)dr["station"];
                        amvCh.Channel = (string)dr["channel"];
                        amvCh.SensorSN = (string)dr["sensorSerialNumber"];
                        amvCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        amvCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        amvCh.roll = (float)(double)dr["roll"];
                        amvCh.pitch = (float)(double)dr["pitch"];
                        amvCh.tilt = (float)(double)dr["tilt"];
                        amvCh.angle = (float)(double)dr["angle"];
                        amvCh.elevation = (float)(double)dr["elevation"];
                        amvCh.stdDev = (float)(double)dr["standardDeviation"];
                        amvCh.maxDev = (float)(double)dr["maximumDeviation"];
                        amvCh.azuimuth = (float)(double)dr["azimuth"];

                        channels.Add(amvCh);
                    }
                }
            }

            return true;
        }

        public bool GetImages(int measId, ref List<ImageInfo> images)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;
            
            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Graph WHERE measID=" + measId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        ImageInfo im = new ImageInfo();

                        im.ID = (int)dr["ID"];
                        im.Path = (string)dr["filename"];
                        im.Include = (bool)dr["include"];
                        im.Drawn = false;

                        images.Add(im);
                    }
                }
            }

            return true;
        }

        public bool UpdateImage(ImageInfo img)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            string sql = String.Format("UPDATE Graph SET include={0} WHERE ID={1}", img.Include, img.ID);

            using (OdbcCommand command = new OdbcCommand(sql, Connection))
            {
                command.ExecuteNonQuery();


            }

            return true;
        }

         
        public bool GetLiveGraphMeas(ref DBInterface.Measurement meas, ref LiveGraph lgm)
        {

            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM LiveGraph WHERE measID=" + meas.ID.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    if (dr.Read())
                    {                        
                        lgm.ID = (int)dr["ID"];
                        lgm.sampleRate= (double)dr["samplingRate"];                        
                    }
                }
            }

            return true;
        }

        public bool GetLiveGraphCh(int foreignId, ref List<DBInterface.ChannelBase> channels)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM LiveGraphChannel INNER JOIN StationType ON LiveGraphChannel.type = StationType.stationType WHERE foreignID=" + foreignId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        /*for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                        }*/
                        DBInterface.LiveGraphCh lgCh = new DBInterface.LiveGraphCh();

                        lgCh.ID = (int)dr["ID"];
                        lgCh.ForeignID = (int)dr["foreignID"];
                        lgCh.Type = (int)dr["type"];
                        lgCh.TypeText = (string)dr["stationTypeName"];
                        lgCh.Station = (string)dr["station"];
                        lgCh.Channel = (string)dr["channel"];
                        lgCh.SensorSN = (string)dr["sensorSerialNumber"];
                        lgCh.AdapterSN = (string)dr["adapterSerialNumber"];
                        lgCh.NominalAz = (float)(double)dr["NominalAzimuth"];
                        lgCh.roll = (float)(double)dr["roll"];
                        lgCh.pitch = (float)(double)dr["pitch"];
                        lgCh.tilt = (float)(double)dr["tilt"];
                        lgCh.angle = (float)(double)dr["angle"];
                        lgCh.temperature = (double)dr["temperature"];

                        channels.Add(lgCh);
                    }
                }
            }

            return true;
        }

        public bool GetDAUCalibration(ref List<DAUData> DAUCalib)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM DAUSetup", Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        DAUData dd = new DAUData();

                        dd.ID = (int)dr["ID"];
                        dd.sn = (int)dr["serialNumber"];                        

                        DAUCalib.Add(dd);
                    }
                }
            }

            foreach (DAUData dd in DAUCalib)
            {
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM DAUSetupSensorChannel WHERE foreignID=" + dd.ID, Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        while (dr.Read())
                        {
                            DAUChData cd = new DAUChData();

                            cd.time = (DateTime)dr["calibTime"];
                            cd.ch = (string)dr["name"];
                            cd.rollGain = (double)dr["rollGain"];
                            cd.pitchGain = (double)dr["pitchGain"];
                            cd.rollOffs = (int)dr["rollOffset"];
                            cd.pitchOffs = (int)dr["pitchOffset"];

                            dd.chData.Add(cd);                            
                        }
                    }
                }
            }


            return true;
        }

        public bool GetSensorCalibration(ref List<SensorCalibrationInfo> SensCalib)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;            

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorPitchAzimuthCalibration LEFT JOIN ProjectCalibration ON SensorPitchAzimuthCalibration.calProjId = ProjectCalibration.ID", Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        for (int i = 0; i < dr.FieldCount; i++)
                        {
                            string name = dr.GetName(i);
                            string typ = dr.GetDataTypeName(i);
                        }

                        SensorCalibrationInfo sc = new SensorCalibrationInfo();

                        sc.sn = (string)dr["serialNumber"];
                        sc.calData[CalType.CT_PitchAz].time = (DateTime)dr["time"];
                        sc.calData[CalType.CT_PitchAz].a[0] = (double)dr["a_0"];
                        sc.calData[CalType.CT_PitchAz].a[1] = (double)dr["a_1"];
                        sc.calData[CalType.CT_PitchAz].a[2] = (double)dr["a_2"];
                        sc.calData[CalType.CT_PitchAz].a[3] = (double)dr["a_3"];
                        sc.calData[CalType.CT_PitchAz].platformSN = 0;
                        var v = dr["platformSN"];
                        if(v != System.DBNull.Value)
                            sc.calData[CalType.CT_PitchAz].platformSN = (int)v;

                        SensCalib.Add(sc);
                    }
                }
            }


            foreach (SensorCalibrationInfo sc in SensCalib)
            {
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorPitchGainCalibration LEFT JOIN ProjectCalibration ON SensorPitchGainCalibration.calProjId = ProjectCalibration.ID Where serialNumber='" + sc.sn+"'", Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        if (dr.Read())
                        {                            
                            sc.calData[CalType.CT_PitchGain].time = (DateTime)dr["time"];
                            sc.calData[CalType.CT_PitchGain].a[0] = (double)dr["a_0"];
                            sc.calData[CalType.CT_PitchGain].a[1] = (double)dr["a_1"];
                            sc.calData[CalType.CT_PitchGain].a[2] = (double)dr["a_2"];
                            sc.calData[CalType.CT_PitchGain].a[3] = (double)dr["a_3"];
                            sc.calData[CalType.CT_PitchGain].platformSN = 0;
                            var v = dr["platformSN"];
                            if (v != System.DBNull.Value)
                                sc.calData[CalType.CT_PitchGain].platformSN = (int)v;
                        }
                    }
                }
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorPitchOffsetCalibration LEFT JOIN ProjectCalibration ON SensorPitchOffsetCalibration.calProjId = ProjectCalibration.ID Where serialNumber='" + sc.sn + "'", Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        if (dr.Read())
                        {
                            sc.calData[CalType.CT_PitchOffs].time = (DateTime)dr["time"];
                            sc.calData[CalType.CT_PitchOffs].a[0] = (double)dr["a_0"];
                            sc.calData[CalType.CT_PitchOffs].a[1] = (double)dr["a_1"];
                            sc.calData[CalType.CT_PitchOffs].a[2] = (double)dr["a_2"];
                            sc.calData[CalType.CT_PitchOffs].a[3] = (double)dr["a_3"];
                            sc.calData[CalType.CT_PitchOffs].platformSN = 0;
                            var v = dr["platformSN"];
                            if (v != System.DBNull.Value)
                                sc.calData[CalType.CT_PitchOffs].platformSN = (int)v;
                        }
                    }
                }
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorRollAzimuthCalibration LEFT JOIN ProjectCalibration ON SensorRollAzimuthCalibration.calProjId = ProjectCalibration.ID Where serialNumber='" + sc.sn + "'", Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        if (dr.Read())
                        {
                            sc.calData[CalType.CT_RollAz].time = (DateTime)dr["time"];
                            sc.calData[CalType.CT_RollAz].a[0] = (double)dr["a_0"];
                            sc.calData[CalType.CT_RollAz].a[1] = (double)dr["a_1"];
                            sc.calData[CalType.CT_RollAz].a[2] = (double)dr["a_2"];
                            sc.calData[CalType.CT_RollAz].a[3] = (double)dr["a_3"];
                            sc.calData[CalType.CT_RollAz].platformSN = 0;
                            var v = dr["platformSN"];
                            if (v != System.DBNull.Value)
                                sc.calData[CalType.CT_RollAz].platformSN = (int)v;
                        }
                    }
                }
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorRollGainCalibration LEFT JOIN ProjectCalibration ON SensorRollGainCalibration.calProjId = ProjectCalibration.ID Where serialNumber='" + sc.sn + "'", Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        if (dr.Read())
                        {
                            sc.calData[CalType.CT_RollGain].time = (DateTime)dr["time"];
                            sc.calData[CalType.CT_RollGain].a[0] = (double)dr["a_0"];
                            sc.calData[CalType.CT_RollGain].a[1] = (double)dr["a_1"];
                            sc.calData[CalType.CT_RollGain].a[2] = (double)dr["a_2"];
                            sc.calData[CalType.CT_RollGain].a[3] = (double)dr["a_3"];
                            sc.calData[CalType.CT_RollGain].platformSN = 0;
                            var v = dr["platformSN"];
                            if (v != System.DBNull.Value)
                                sc.calData[CalType.CT_RollGain].platformSN = (int)v;
                        }
                    }
                }
                using (OdbcCommand command = new OdbcCommand("SELECT * FROM SensorRollOffsetCalibration LEFT JOIN ProjectCalibration ON SensorRollOffsetCalibration.calProjId = ProjectCalibration.ID Where serialNumber='" + sc.sn + "'", Connection))
                {
                    using (OdbcDataReader dr = command.ExecuteReader())
                    {
                        if (dr.Read())
                        {
                            sc.calData[CalType.CT_RollOffs].time = (DateTime)dr["time"];
                            sc.calData[CalType.CT_RollOffs].a[0] = (double)dr["a_0"];
                            sc.calData[CalType.CT_RollOffs].a[1] = (double)dr["a_1"];
                            sc.calData[CalType.CT_RollOffs].a[2] = (double)dr["a_2"];
                            sc.calData[CalType.CT_RollOffs].a[3] = (double)dr["a_3"];
                            sc.calData[CalType.CT_RollOffs].platformSN = 0;
                            var v = dr["platformSN"];
                            if (v != System.DBNull.Value)
                                sc.calData[CalType.CT_RollOffs].platformSN = (int)v;
                        }
                    }
                }
            }
            
            return true;
        }

        public bool GetGunAdapter(ref List<GunAdaptCalib> adapters)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM GunAdapterCalibration", Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        GunAdaptCalib ga = new GunAdaptCalib();
                        
                        ga.sn= (string)dr["serialNumber"];
                        ga.time = (DateTime)dr["time"];                       
                        ga.azimuth = (double)dr["azimuth"];
                        ga.elevation = (double)dr["elevation"];

                        adapters.Add(ga);
                    }
                }
            }

            return true;
        }

        public bool GetTheoAdapter(ref List<TheoAdaptCalib> adapters)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM TheoAdapterCalibration", Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        TheoAdaptCalib ta = new TheoAdaptCalib();

                        ta.sn = (string)dr["serialNumber"];
                        ta.time = (DateTime)dr["time"];
                        ta.azimuth = (double)dr["azimuth"];
                        ta.elevation = (double)dr["elevation"];
                        ta.type = (int)dr["type"];

                        adapters.Add(ta);
                    }
                }
            }

            return true;
        }

        public bool GetPlatformCorrections(ref List<PlatformCorrection> platforms)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM PlatformCorrection", Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {
                        PlatformCorrection pc = new PlatformCorrection();

                        pc.sn = (int)dr["serialNumber"];
                        pc.time = (DateTime)dr["time"];                        
                        pc.alpha = (double)(float)dr["alpha"];
                         
                        platforms.Add(pc);
                    }
                }
            }

            return true;
        }

    }
}
