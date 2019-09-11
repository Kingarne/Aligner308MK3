using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReporterLib
{
  
    public interface ReporterI
    {
        int OpenReport(int historyId);
    }

    public class Reporter : ReporterI
    {
        //private int m_historyId;
        private ReportForm reportForm;
        public int OpenReport(int historyId)
        {
            Console.WriteLine("Open report: " + historyId.ToString());

            if(reportForm == null)
            {
                reportForm = new ReportForm();
            }


            reportForm.HistoryId = historyId;
            reportForm.ShowDialog();


            return 0;
        }

        


    }
}


