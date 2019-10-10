using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Odbc;
using System.Windows.Forms;

namespace ReporterLib
{
    class DBInterface
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
            MT_TiltFlatnessRP,
            MT_TiltFlatnessFo,
            MT_AzimuthAlign,
            MT_GyroPerf,
            MT_LiveGraph,
            MT_VerifAbsolute,
            MT_VerifRelative,
            MT_CommonFlatTilt,
            MT_SensorValidation
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
            public int ID { get; set; }
            public int ForeignID { get; set; }
            public string Station { get; set; }
            public int Type { get; set; }
            public string TypeText { get; set; }
            public string Channel { get; set; }
            public float NominalAz { get; set; }
            public string SensorSN { get; set; }
            public string AdapterSN { get; set; }
            public float roll { get; set; }
            public float pitch { get; set; }
            public float tilt { get; set; }
            public float angle { get; set; }
            public float elevation { get; set; }
            public bool IsRef { get; set; }

        }

        public class ChannelErrBase
        {
            public int ID { get; set; }
            public int ForeignID { get; set; }
            public float aziuth { get; set; }
            public float error { get; set; }
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
            public float elevation2 { get; set; }
            public float indexArmL1{ get; set; }
            public float indexArmL2{ get; set; }
            public float error2 { get; set; }
            public float dh{ get; set; }
        }

        private OdbcConnection Connection;
        public bool Open()
        {
            Connection = new OdbcConnection();

            string file = "C:\\ProgramData\\Schill Reglerteknik AB\\Aligner 308 MK3\\Aligner308.mdb";

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


        public bool GetMeasurement(int measId, ref Measurement measurement)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Measurement  INNER JOIN MeasType ON Measurement.measType = MeasType.measType WHERE ID=" + measId.ToString(), Connection))
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

        public bool GetTiltFlatnessFoChErr(int foreignId, ref List<DBInterface.ChannelErrBase> channelErr)
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
                        tfeCh.aziuth = (float)(double)dr["azimuth"];
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

    }
}
