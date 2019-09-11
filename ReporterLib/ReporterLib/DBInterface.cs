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
        public class Measurement
        {
            public int ID { get; set; }
            public DateTime Time { get; set; }
            public string Operator { get; set; }
            public string Place { get; set; }
            public string DAUSerial { get; set; }
            public double Latitude { get; set; }
            public string AngDef1 { get; set; }
            public string AngDef2 { get; set; }
            public string Project { get; set; }
            public string CalibInfo { get; set; }
        }



        private OdbcConnection Connection;
        public bool Open()
        {
            Connection = new OdbcConnection();

            string file = "C:\\ProgramData\\Schill Reglerteknik AB\\Aligner 308 MK2\\Aligner308.mdb";

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

        public bool GetMeasurement(int historyId, ref Measurement measurement)
        {
            if (Connection.State != System.Data.ConnectionState.Open)
                return false;

            using (OdbcCommand command = new OdbcCommand("SELECT * FROM History WHERE ID=" + historyId.ToString(), Connection))
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
                        measurement.Operator = (string)dr["operator"];
                        measurement.Place = (string)dr["place"];
                        measurement.Project = (string)dr["project"];
                        measurement.DAUSerial = (string)dr["dauSerialNumber"];
                        measurement.Latitude  = (double)dr["latitude"];
                        measurement.AngDef1 = (string)dr["angulaDefinitionPart1"];
                        measurement.AngDef2 = (string)dr["angulaDefinitionPart2"];
                        measurement.CalibInfo = (string)dr["calibInfo"];
                        measurement.Time = (DateTime)dr["timeOfMeasurement"];
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
