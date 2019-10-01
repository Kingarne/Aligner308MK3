﻿using System;
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
        
        public int ProjectId { get; set; }
        public int MeasId { get; set; }

        private bool MultiMode { get; set; }
        private DBInterface.Project Project;

        private DBInterface.Measurement Measurement;
        private Dictionary<int, DBInterface.Measurement> Measurements;
        Dictionary<DBInterface.MeasType, Func<PrintPageEventArgs, int>> PrintMeasFunc = new Dictionary<DBInterface.MeasType, Func<PrintPageEventArgs, int>>();

        private int m_page;
        private int m_yPos;
        int yHeadSpace = 15;
        int startYHeadPerc = 35;
        int MargY = 20;
        Rectangle HeadRect;
        private Font HeadFont = new Font("Ariel", 8, FontStyle.Regular);


        public class TableItem
        {
            public TableItem(string text, float pos, float width, StringAlignment sa=StringAlignment.Center) { Text = text; PosPerc = pos; WidthPerc = width; Align = sa; }
            public string Text { get; set; }
            public float PosPerc { get; set; }
            public float WidthPerc { get; set; }
            public StringAlignment Align { get; set; }
            public Color Color;
        }

        public ReportForm()
        {
            InitializeComponent();
            DBI = new DBInterface();
            DBI.Open();

            Measurements = new Dictionary<int, DBInterface.Measurement>();

            PrintMeasFunc[DBInterface.MeasType.MT_TiltAlignment] = PrintTiltAlignment;
        }

        
        private void ReportForm_Load(object sender, EventArgs e)
        {
            m_printerSettings = new PrinterSettings();

            MultiMode = (MeasId == -1);
            reportList.Visible = MultiMode;

            Project = new DBInterface.Project();
            DBI.GetProject(ProjectId, ref Project);



            if (MultiMode)
            {
                DBI.GetProjectMeasurements(ProjectId, ref Measurements);

                reportList.Columns.Add("Report");
                reportList.Columns.Add("Time");
                reportList.Columns[0].Width = 150;
                reportList.Columns[1].Width = 100;

                UpdateReportList();    
            }

            if (MultiMode)
                Text = ProjectId.ToString() + ": Show All";
            else
                Text = ProjectId.ToString() + ": Show " + MeasId.ToString();


        }

        private void UpdateReportList()
        {
            reportList.Items.Clear();
        
            int i = 0;
            foreach (var meas in Measurements)
            {
                reportList.Items.Add(meas.Value.TypeText);                
                reportList.Items[i].SubItems.Add(meas.Value.Time.ToString("yy/MM/dd HH:mm:ss"));
                reportList.Items[i].Tag = meas.Key;
                i++;
            }
        }

        private string GetAngularDef(int signDef)
        {
            string dir = signDef > 0 ? "down" : "up";
            string text1 = "Angular definitions:\nRoll / Pitch are positive if stbd / aft " + dir + " (Cartesian coordinates).\n";
            string text2 = "The tilt vector mark indicates the high point (Polar coordinates).\nThe angle of the tilt vector is positive clockwise from fore.\nElevation is positive up.\nAzimuth is positive clockwise from fore.";
            return text1 + text2;
        }

        private void PrintPreview()
        {
            m_document = new PrintDocument();
            m_document.PrinterSettings = m_printerSettings;

            m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettings);
            m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrint);
            m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPage);
            m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrint);

            printPreviewControl.Document = m_document;
            printPreviewControl.StartPage = 0;
        }

        private void printButton_Click(object sender, EventArgs e)
        {
            m_document = new PrintDocument();
            m_document.PrinterSettings = m_printerSettings;

            m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettings);
            m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrint);
            m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPage);
            m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrint);


            PrintDialog printDialog = new PrintDialog();
            printDialog.Document = m_document;
            if (printDialog.ShowDialog() == DialogResult.OK)
            {
                m_document.Print();
                m_printerSettings = m_document.PrinterSettings;
            }



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


            Measurement = Measurements[MeasId];
                new DBInterface.Measurement();
   //         DBI.GetMeasurement(MeasId, ref Measurement);
            
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
                PrintMeasFunc[Measurement.Type].Invoke(e);
                //DrawHeader(e);
            }

            //  string text = "This text to be printed. "+ m_page.ToString();
            // e.Graphics.DrawString(text, new Font("Georgia", 35, FontStyle.Bold), Brushes.Black, 10, 10);

            e.HasMorePages = (m_page++ <= 1);


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

        private void pageUpDown_ValueChanged(object sender, EventArgs e)
        {
            printPreviewControl.StartPage = ((int)pageUpDown.Value) - 1;
            printPreviewControl.Update();
        }

        private void reportList_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            ShowSelected();
        }

        private void ShowSelected()
        {
            if (reportList.SelectedItems.Count > 0)
            {
                int item = reportList.SelectedItems[0].Index;
                MeasId = (int)reportList.Items[item].Tag;
                PrintPreview();
                //Logger.Inst.

            }
        }

        private void DrawHeader(PrintPageEventArgs e) 
        {
            //DBInterface.Measurement measurement = new DBInterface.Measurement();
            //DBI.GetMeasurement(MeasId, ref measurement);

           
            SolidBrush br = new SolidBrush(Color.Black);

           
            int headX = e.MarginBounds.Left;
            int headY = e.MarginBounds.Top;

            string angDef = GetAngularDef(Project.SignDef);
            e.Graphics.DrawString(angDef, HeadFont, br, headX, headY);
            SizeF size = e.Graphics.MeasureString(angDef, HeadFont);

            headY = headY + (int)size.Height + MargY;

            HeadRect = new Rectangle(new Point(headX, headY), new Size(e.MarginBounds.Width, 120));

            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(255, 240, 240, 255)), HeadRect);
            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(50,0,0,0)), HeadRect);
            int xPerc = 2;
            //string text = "AZIMUTH ALIGNMENT ERRORS ";
                                  
            //e.Graphics.DrawString(text, new Font("Ariel", 8, FontStyle.Bold), Brushes.Black, headX +10, headY+10);
            DrawText(Measurement.TypeText.ToUpper(), e.Graphics, new Font("Ariel", 10, FontStyle.Bold), Brushes.Black, HeadRect, 2, 6);
            DrawText("Project:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText("Ship:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText("Operator:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace*2);
            DrawText("DAU s/n:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace*3);

            xPerc = xPerc + 10;
            DrawText(Project.Name, e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText(Project.Ship.ToString(), e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText(Project.Operator, e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace*2);
            DrawText(Project.DAUSerial.ToString("###"), e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace*3);

            xPerc = 30;
            DrawText("Date:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText("Place:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText("Latitude:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 2);
            DrawText("Time const:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 3);

            xPerc = xPerc + 10;
            DrawText(Measurement.Time.ToString("yy/MM/dd HH:mm:ss"), e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText(Project.Location, e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText(Project.Latitude.ToString("0.00") + " [deg]", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 2);
            DrawText(Measurement.TimeConstant.ToString("0.0") + " [s]", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 3);

            m_yPos = HeadRect.Bottom + MargY;
        }

        private int DrawTableLine(Graphics gr, List<TableItem> table, Point pos, int width)
        {
            SolidBrush br = new SolidBrush(Color.Black);
            int x = 0;
            int xLoc=pos.X;

            int maxY = 0;    

            foreach (TableItem ti in table)
            {
                StringFormat sf = new StringFormat();
                sf.Alignment = ti.Align;

                if (ti.PosPerc > 0)
                    x = pos.X + (int)(width * (ti.PosPerc / 100.0f));
                else
                    x = xLoc;

                int w = (int)((float)width * (ti.WidthPerc / 100.0f));

                RectangleF textRect = new RectangleF(x, pos.Y, w, 100);
                SizeF size = gr.MeasureString(ti.Text, Font, w, sf);
                gr.DrawString(ti.Text, Font, br, textRect, sf);

              //  Rectangle drawRect = new Rectangle(x, pos.Y, w, 100);
              //  gr.DrawRectangle(new Pen(Color.Black), drawRect);                

                xLoc = (int)textRect.Right;
                if (size.Height > maxY)
                    maxY = (int)size.Height;
            }

            return maxY;
        }

        private int DrawCalibInfo(Graphics gr, DBInterface.Measurement meas, int pos)
        {
            int h = 0;
            int startPos = pos;
            if(meas.CalibInfo != "")
            {
                pos += MargY;
                SolidBrush br = new SolidBrush(Color.Red);
                SizeF size = gr.MeasureString(meas.CalibInfo, Font, HeadRect.Width);
                Rectangle rect = new Rectangle(HeadRect.Left, pos, HeadRect.Width, (int)size.Height);
                DrawText(meas.CalibInfo, gr, Font, br, rect, 2, 0);
                //    gr.DrawString(, Font, br, HeadRect.Width);
                pos += (int)size.Height + MargY;
                gr.DrawLine(new Pen(Color.Black, 2), HeadRect.Left, pos, HeadRect.Right, pos);
                h += pos-startPos;
            }

            return h;
        }

        private int DrawComment(Graphics gr, DBInterface.Measurement meas, int pos)
        {
            int h = 0;
            int startPos = pos;
            if (meas.Comment != "")
            {
                pos += MargY;
                string comment = "Comment:\n" + meas.Comment;

                SolidBrush br = new SolidBrush(Color.Black);
                SizeF size = gr.MeasureString(comment, Font, HeadRect.Width);
                Rectangle rect = new Rectangle(HeadRect.Left, pos, HeadRect.Width, (int)size.Height);
                DrawText(comment, gr, Font, br, rect, 2, 0);
                //    gr.DrawString(, Font, br, HeadRect.Width);

                pos += (int)size.Height;// + MargY;
              //  gr.DrawLine(new Pen(Color.Black, 2), HeadRect.Left, pos, HeadRect.Right, pos);
                h = pos-startPos;
            }


            return h;
        }


        private void SetRefChannel(DBInterface.Measurement meas, List<DBInterface.ChannelBase> channels)
        {
            foreach(var ch in channels)
            {
                if (meas.RefChannel == ch.Channel)
                {
                    ch.IsRef = true;
                    return;
                }
            }
        }

        private int PrintTiltAlignment(PrintPageEventArgs e)
        {
            DrawHeader(e);

            DBInterface.TiltAlignment tam = new DBInterface.TiltAlignment();
            DBI.GetTiltAlignmentMeas(ref Measurement, ref tam);            
            List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();             
            DBI.GetTiltAlignmentCh(tam.ID, ref channels);
            SetRefChannel(Measurement, channels);


            SolidBrush br = new SolidBrush(Color.Black);

            int xPerc = 60;
            DrawText("LOS Dir:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText("Elev. Comp:", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            xPerc = xPerc + 10;
            DrawText(tam.LOSDir, e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc);
            DrawText(tam.ElevComp?"On":"Off", e.Graphics, HeadFont, br, HeadRect, xPerc, startYHeadPerc + yHeadSpace);

            int wPerc = 8;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("Station", 2, 25, StringAlignment.Near));
            table.Add(new TableItem("Ch", -1, 5));
            table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
            table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
            table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
            table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
            table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
            table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
            table.Add(new TableItem("Elev.\n" + Project.UnitText, -1, wPerc));
            table.Add(new TableItem("Bias*\n" + Project.UnitText, -1, wPerc));

            int smalMarg = 4;
            m_yPos += DrawTableLine(e.Graphics, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width);
            m_yPos += smalMarg;
            e.Graphics.DrawLine(new Pen(Color.Black, 2), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += smalMarg;

           
            foreach (DBInterface.TiltAlignmentCh ch in channels)
            {
                table = new List<TableItem>();
                table.Add(new TableItem(ch.Station, 2, 25, StringAlignment.Near));
                table.Add(new TableItem(ch.Channel, -1, 5));
                table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                if (ch.IsRef)
                {
                    string refStr = "Ref";
                    table.Add(new TableItem(refStr, -1, wPerc));
                    table.Add(new TableItem(refStr, -1, wPerc));
                    table.Add(new TableItem(refStr, -1, wPerc));
                    table.Add(new TableItem(refStr, -1, wPerc));
                    table.Add(new TableItem(refStr, -1, wPerc));
                    table.Add(new TableItem(refStr, -1, wPerc));
                }
                else
                {
                    table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.elevation.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.bias.ToString("0.00"), -1, wPerc));                    
                }

                m_yPos += DrawTableLine(e.Graphics, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width);
                m_yPos += smalMarg;
            }

            e.Graphics.DrawLine(new Pen(Color.Black, 2), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
           
            m_yPos += DrawCalibInfo(e.Graphics, Measurement, m_yPos);
            // m_yPos += MargY;

            m_yPos += DrawComment(e.Graphics, Measurement, m_yPos);
            m_yPos += MargY;



            DrawText("*Bias across line-of-sight.", e.Graphics, HeadFont, br, new Rectangle(HeadRect.Left, e.MarginBounds.Bottom, HeadRect.Width, 10), 2, 0);
            // m_yPos += DrawTableLine(e.Graphics, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width);



            return 0;
        }



    }
}
 