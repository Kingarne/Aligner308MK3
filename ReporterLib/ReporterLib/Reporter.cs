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
        int OpenReport(int projectId, int measId);
        int OpenCalibrationData(int projectId);
    }

    public class Reporter : ReporterI
    {
        //private int m_historyId;
        private ReportForm reportForm;
        public int OpenReport(int projectId, int measId=-1)
        {
            Console.WriteLine("Open report: " + measId.ToString());

            if(reportForm == null)
            {
                reportForm = new ReportForm();
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
                reportForm = new ReportForm();
            }

            reportForm.PrintType = ReportForm.ReportType.RT_Calibration;
            reportForm.ProjectId = projectId;
            reportForm.SetMeasId(-1);
            reportForm.ShowDialog();


            return 1;
        }




    }
}


