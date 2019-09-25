using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReporterLib
{
  
    public interface ReporterI
    {
        int OpenReport(int projectId, int historyId);
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


            reportForm.MeasId = measId;
            reportForm.ShowDialog();


            return 0;
        }

        


    }
}


