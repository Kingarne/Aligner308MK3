using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReporterLib
{
  
    public interface ReporterI
    {
        unsafe void SetDBPath(byte* s, int len);
        int OpenReport(int projectId, int measId);
        int OpenCalibrationData(int projectId);
    }

    

    public class Reporter : ReporterI
    {
        //private int m_historyId;
        private ReportForm reportForm;
        private string Path = "";

        unsafe public void SetDBPath(byte* s, int len)
        {
            byte[] arr = new byte[len];
            System.Runtime.InteropServices.Marshal.Copy((IntPtr)s, arr, 0, len);            

            Path = System.Text.Encoding.UTF8.GetString(arr);
        }

        public int OpenReport(int projectId, int measId=-1)
        {
            Console.WriteLine("Open report: " + " "  + measId.ToString());

           
            //string path = Encoding.Default.GetString(s);

            if(reportForm == null)
            {
                reportForm = new ReportForm(Path);
            }

            reportForm.PrintType = ReportForm.ReportType.RT_Measurement;
            reportForm.ProjectId = projectId;
            reportForm.SetMeasId(measId);
            DialogResult res =  reportForm.ShowDialog();


            return (res == DialogResult.OK) ? 1 : 0;
        }

        public int OpenCalibrationData(int projectId)
        {
            Console.WriteLine("View Calibration Data");

            if (reportForm == null)
            {
                reportForm = new ReportForm(Path);
            }

            reportForm.PrintType = ReportForm.ReportType.RT_Calibration;
            reportForm.ProjectId = projectId;
            reportForm.SetMeasId(-1);
            reportForm.ShowDialog();


            return 1;
        }




    }
}


