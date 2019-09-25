using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Printing;

namespace ReporterLib
{
    public partial class ReportForm : Form
    {
        private DBInterface DBI;
        private PrinterSettings m_printerSettings;
        private PrintDocument m_document;
        private int m_page;
        private int m_yPos;

        public int ProjectId { get; set; }
        public int MeasId { get; set; }

        private DBInterface.Project Project;
        private DBInterface.Measurement Measurement;

        public ReportForm()
        {
            InitializeComponent();
            DBI = new DBInterface();
            DBI.Open();
        }

        private void ReportForm_Load(object sender, EventArgs e)
        {
            m_printerSettings = new PrinterSettings();

            if (MeasId == -1)
                Text = "Show All";
            else
                Text = "Show " + MeasId.ToString();

        }

        private void printButton_Click(object sender, EventArgs e)
        {
            m_document = new PrintDocument();
            m_document.PrinterSettings = m_printerSettings;

            m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettings);
            m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrint);
            m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPage);
            m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrint);

            printPreviewControl.Document = m_document;
            printPreviewControl.StartPage = 0;



           /* PrintPreviewDialog pd = new PrintPreviewDialog();
            pd.Document = m_document;
            pd.Show();*/
            //printPreviewControl.Update();


        }

        private void QuerySettings(object sender, System.Drawing.Printing.QueryPageSettingsEventArgs e)
        {
             e.PageSettings.Margins = new System.Drawing.Printing.Margins(70, 70, 70, 70);
        }

        private void EndPrint(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            pageUpDown.Minimum = 1;
            pageUpDown.Maximum = m_page;            
        }


        private void BeginPrint(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            m_page = 0;
            m_yPos = 0;

            Project = new DBInterface.Project();
            DBI.GetProject(ProjectId, ref Project);

            Measurement = new DBInterface.Measurement();
            DBI.GetMeasurement(MeasId, ref Measurement);
            
        }

        private void PrintPage(object sender, PrintPageEventArgs e)
        {
            Rectangle bound = e.PageBounds;
            bound.Inflate(-2, -2);
            //e.Graphics.DrawRectangle(new Pen(Color.FromArgb(200, 255, 0, 0)), bound);
            //e.Graphics.DrawRectangle(new Pen(Color.FromArgb(100, 110, 110, 10)), e.MarginBounds);
            DrawPageNum(e);

            if (m_page == 0)
            {
                DrawHeader(e);
            }

            //  string text = "This text to be printed. "+ m_page.ToString();
            // e.Graphics.DrawString(text, new Font("Georgia", 35, FontStyle.Bold), Brushes.Black, 10, 10);

            e.HasMorePages = (m_page++ <= 3);


        }

        private void DrawPageNum(PrintPageEventArgs e)
        {            
            e.Graphics.DrawString((m_page+1).ToString(), new Font("Ariel", 8, FontStyle.Bold), Brushes.Black, e.PageBounds.Width-20, e.PageBounds.Height - 20);

        }


        private void DrawText(string text, Graphics gr, Font font, Brush br, Rectangle rect, double percX, double percY)
        {
            int x = rect.Left + (int)((double)rect.Width * (percX / 100.0f));
            int y = rect.Top + (int)((double)rect.Height * (percY / 100.0f));

            gr.DrawString(text, font, br, x, y);

        }

        private void DrawHeader(PrintPageEventArgs e)
        {
            DBInterface.Measurement measurement = new DBInterface.Measurement();
            DBI.GetMeasurement(MeasId, ref measurement);



            int headX = e.MarginBounds.Left;
            int headY = e.MarginBounds.Top;
            Rectangle headRect = new Rectangle(new Point(headX, headY), new Size(e.MarginBounds.Width, 100));

            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(255, 240, 240, 255)), headRect);
            // e.Graphics.DrawRectangle(new Pen(Color.FromArgb(155,0,0,0)), headRect);
            int ySpace = 17;
            int startYPerc = 25;
            string text = "AZIMUTH ALIGNMENT ERRORS ";

            Font HeadFont = new Font("Ariel", 8, FontStyle.Regular);
            SolidBrush br = new SolidBrush(Color.Black);

            //e.Graphics.DrawString(text, new Font("Ariel", 8, FontStyle.Bold), Brushes.Black, headX +10, headY+10);
            DrawText(text, e.Graphics, new Font("Ariel", 9, FontStyle.Bold), Brushes.Black, headRect, 1, 2);
            DrawText("Project:", e.Graphics, HeadFont, br, headRect, 1, startYPerc);
            DrawText("Ship:", e.Graphics, HeadFont, br, headRect, 1, startYPerc + ySpace);
            DrawText("Operator:", e.Graphics, HeadFont, br, headRect, 1, startYPerc + ySpace*2);
            DrawText("DAU s/n:", e.Graphics, HeadFont, br, headRect, 1, startYPerc + ySpace*3);

            DrawText(Project.Name, e.Graphics, HeadFont, br, headRect, 12, startYPerc);
            DrawText(Project.Ship.ToString(), e.Graphics, HeadFont, br, headRect, 12, startYPerc + ySpace);
            DrawText(Project.Operator, e.Graphics, HeadFont, br, headRect, 12, startYPerc + ySpace*2);
            DrawText(Measurement.DAUSerial, e.Graphics, HeadFont, br, headRect, 12, startYPerc + ySpace*3);

        }

        private void pageUpDown_ValueChanged(object sender, EventArgs e)
        {
            printPreviewControl.StartPage = ((int)pageUpDown.Value) -1 ;
            printPreviewControl.Update();
        }
    }
}
 