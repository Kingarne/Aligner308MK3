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
            public string Operator { get; set; }
            public string Location { get; set; }
            public string Ship { get; set; }
            public float Latitude{ get; set; }
            public int Unit { get; set; }
            public int SignDef { get; set; }
        }


        public class Measurement
        {
            public int ID { get; set; }
            public int ProjectID { get; set; }
            public DateTime Time { get; set; }
            public string DAUSerial { get; set; }            
            public string CalibInfo { get; set; }
            public int Type { get; set; }
            public string TypeText { get; set; }
            public float TimeConstant{ get; set; }
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
                MessageBox.Show("Error ODBC");
            }
                //execute queries, etc           

            return true;

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
                        }

                        project.ID = (int)dr["ID"];
                        project.Name = (string)dr["name"];
                        project.Operator = (string)dr["operator"];
                        project.Location = (string)dr["location"];
                        project.Ship = (string)dr["ShipName"];
                        project.Time = (DateTime)dr["projTime"];
                        project.Latitude= (float)(double)dr["latitude"];
                        project.SignDef = (int)dr["signDef"];

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
                        measurement.DAUSerial = (string)dr["dauSerialNumber"];
                        measurement.CalibInfo = (string)dr["calibInfo"];
                        measurement.Time = (DateTime)dr["timeOfMeasurement"];
                        measurement.Type = (int)dr["measType"];
                        measurement.TypeText = (string)dr["measTypeName"];
                        measurement.TimeConstant = (float)dr["timeConstant"];

                        measurements[measurement.ID] = measurement;

                        Logger.Inst.Trace("ID: " + measurement.ID.ToString(), Logger.LogLevel.Level1);
                    }
                }
            }


            return true;
        }
               


        public bool GetMeasurement(int measId, ref Measurement measurement)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM Measurement WHERE ID=" + measId.ToString(), Connection))
            {
                using (OdbcDataReader dr = command.ExecuteReader())
                {
                    while (dr.Read())
                    {

                        for(int i=0;i< dr.FieldCount;i++)
                        {
                            string name = dr.GetName(i);
                        }

                        measurement.ID = (int)dr["ID"];
                        measurement.ProjectID = (int)dr["projectID"];
                        measurement.DAUSerial = (string)dr["dauSerialNumber"];
                        measurement.CalibInfo = (string)dr["calibInfo"];
                        measurement.Time = (DateTime)dr["timeOfMeasurement"];
                        measurement.Type = (int)dr["measType"];
                        measurement.TimeConstant = (float)dr["timeConstant"];

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


    }
}
