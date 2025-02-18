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
using System.Globalization;
using System.Runtime.ConstrainedExecution;

namespace ReporterLib
{
    public partial class ReportForm : Form
    {

        private DBInterface DBI;
        private PrinterSettings m_printerSettings;
        private PrintDocument m_document;

        public enum ReportType
        {
            RT_Measurement = 0,
            RT_Calibration = 1,
        }

        public ReportType PrintType{get;set;}
        public int ProjectId { get; set; }      
        public string RegPath { get; set; }
        List<int> MeasIds = new List<int>();
        private int MeasNum { get; set; }
        private bool HeadPage { get; set; }

        private bool MultiMode { get; set; }

        private DBInterface.Project Project;

        private DBInterface.Measurement Measurement;
        private Dictionary<int, DBInterface.Measurement> Measurements;
        private Dictionary<DBInterface.MeasType, Func<bool>> PrintMeasFunc = new Dictionary<DBInterface.MeasType, Func<bool>>();

        private List<DBInterface.ImageInfo> Images;
        private List<DBInterface.ChannelBase> Channels;
        private List<DBInterface.ChannelErrBaseList> ChannelErrList;
        private bool OnlyImagesLeft = false;

        private List<DBInterface.SensorCalibrationInfo> SensorCalib;
        private List<DBInterface.DAUData> DAUCalib;
        private List<DBInterface.GunAdaptCalib> GunAdapterCalib;
        private List<DBInterface.TheoAdaptCalib> TheoAdapterCalib;
        private List<DBInterface.PlatformCorrection> PlatformCalib;
        public Dictionary<DBInterface.CalType, string> CalTypeMap { get; set; }

        private int m_page;
        private int m_yPos;
        int yHeadSpace = 15;
        int startYHeadPerc = 35;
        int MargY = 20;
        int SmalMarg = 4;
        Rectangle HeadRect;
        private Font HeadFont = new Font("Ariel", 8, FontStyle.Regular);
        private Font TextFont = new Font("Ariel", 8, FontStyle.Regular);
        private Font H1TextFont = new Font("Ariel", 10, FontStyle.Bold);
        private Font CalibHeadFont = new Font("Ariel", 16, FontStyle.Regular);
        private SolidBrush MainBr = new SolidBrush(Color.Black);
        private int LineWidth = 1;
        private Color RefColor = Color.DarkOrange;

        private Color HeadBGColor = Color.FromArgb(255, 240, 240, 255);
        private double Zoom = 1.0f;

        ReportSorter Sorter;
        static private SortOrder LastSortOrder;
        static private int LastSortColumn;

        PrintPageEventArgs PrintArgs = null;
        private bool Close = false;

        public void SetMeasId(int id)
        {
            if (id > 0)
                MeasIds.Add(id);
        }

        public class TableItem
        {
            public TableItem(string text, float pos, float width, Color? c = null, StringAlignment sa=StringAlignment.Center) { Text = text; PosPerc = pos; WidthPerc = width; Color = c??Color.Black; Align = sa; }
            public string Text { get; set; }
            public float PosPerc { get; set; }
            public float WidthPerc { get; set; }
            public StringAlignment Align { get; set; }
            public Color Color;
        }

        public ReportForm(string path, string regPath)
        {
            InitializeComponent();

            RegAccess.Inst.Subkey = regPath;
            LastSortOrder = SortOrder.Ascending;
            LastSortColumn = 1; //time
            Sorter = new ReportSorter();

            DBI = new DBInterface();
            DBI.Open(path);

            Measurements = new Dictionary<int, DBInterface.Measurement>();

            PrintMeasFunc[DBInterface.MeasType.MT_TiltAlignment] = PrintTiltAlignment;
            PrintMeasFunc[DBInterface.MeasType.MT_AzimuthAlign] = PrintAzimuthAlignment;
            PrintMeasFunc[DBInterface.MeasType.MT_AzVerGyrostab] = PrintAzimuthAlignment;
            PrintMeasFunc[DBInterface.MeasType.MT_AzVerBenchmark] = PrintAzimuthAlignment;
            PrintMeasFunc[DBInterface.MeasType.MT_TiltFlatnessPl] = PrintTiltFlatnessPl;
            PrintMeasFunc[DBInterface.MeasType.MT_TiltFlatnessFo] = PrintTiltFlatnessFo;
            PrintMeasFunc[DBInterface.MeasType.MT_GyroPerf] = PrintGyroPerf;
            PrintMeasFunc[DBInterface.MeasType.MT_CommonFlatTilt] = PrintCommonFlat;
            PrintMeasFunc[DBInterface.MeasType.MT_VerifAbsolute] = PrintAbsoluteMode;
            PrintMeasFunc[DBInterface.MeasType.MT_VerifRelative] = PrintRelativeMode;            
            PrintMeasFunc[DBInterface.MeasType.MT_SensorValidation] = PrintSensorValidation;
            PrintMeasFunc[DBInterface.MeasType.MT_LiveGraph] = PrintLiveGraph;

            CalTypeMap = new Dictionary<DBInterface.CalType, string>();
            CalTypeMap[DBInterface.CalType.CT_PitchAz] = "Pitch Azimuth";
            CalTypeMap[DBInterface.CalType.CT_PitchGain] = "Pitch Scale Factor";
            CalTypeMap[DBInterface.CalType.CT_PitchOffs] = "Pitch Offset";
            CalTypeMap[DBInterface.CalType.CT_RollAz] = "Roll Azimuth";
            CalTypeMap[DBInterface.CalType.CT_RollGain] = "Roll Scale Factor";
            CalTypeMap[DBInterface.CalType.CT_RollOffs] = "Roll Offset";

            int h = Screen.FromControl(this).Bounds.Height;
            this.Height = h - 40;
            this.Location= new Point(this.Location.X, 20);

            //RegAccess.Inst.SetValue("Project\\Place", "Snavlunda");
            string z = (string)RegAccess.Inst.GetValue("Report\\Zoom", "1.1");
            double zoom = 0;
            Zoom = 1.0f;
            if(double.TryParse(z, NumberStyles.Any, CultureInfo.InvariantCulture, out zoom ))
            {
                Zoom = zoom;
            }

            string place = (string)RegAccess.Inst.GetValue("DAU\\HSC\\MaxDataDiff", "def"); 
            string place2 = (string)RegAccess.Inst.GetValue("Project\\Place", "def");
            string place3 = (string)RegAccess.Inst.GetValue("ApplicationKey", "def");
            

            // An int value can be stored without specifying the
            // registry data type, but long values will be stored
            // as strings unless you specify the type. Note that
            // the int is stored in the default name/value
            // pair.
            //  Registry.SetValue(keyName, "", 5280);
            // Registry.SetValue(keyName, "TestLong", 12345678901234,
            //    RegistryValueKind.QWord);
        }


        private void ReportForm_Load(object sender, EventArgs e)
        {
            m_printerSettings = new PrinterSettings();

            double[] zoom = new double[] { 0.6, 0.8, 1.0, 1.2, 1.4, 1.6 };
             
            foreach (double d in zoom)
            {
                zoomComboBox.Items.Add((d*100.0).ToString("0") + "%");
                //zoomComboBox.Items[i]
            }
            int i = Array.FindIndex(zoom, el => el == Zoom);
            zoomComboBox.SelectedIndex = i>=0?i:2;            


            Project = new DBInterface.Project();
            DBI.GetProject(ProjectId, ref Project);

            MultiMode = (MeasIds.Count() == 0) && (PrintType == ReportType.RT_Measurement);
            reportList.Visible = MultiMode;
            allButton.Visible = MultiMode;
            noneButton.Visible = MultiMode;
            delButton.Visible = MultiMode;
            keepButton.Visible = false;
            discardButton.Visible = false;
            finishLabel.Visible = false;
           
            if (MultiMode)
            {
                //Text = ProjectId.ToString() + ": Show All";
                DBI.GetProjectMeasurements(ProjectId, ref Measurements);

                reportList.Columns.Add("Include Report");
                reportList.Columns.Add("Time");
                reportList.Columns[0].Width = 150;
                reportList.Columns[1].Width = 120;

                UpdateReportList();
                if (reportList.Items.Count > 0)
                {
                    reportList.Items[0].Checked = true;
                    reportList.Items[0].Selected = true;
                }

            }
            else // Single report
            {
                printPreviewControl.Location = new Point(10, 12 + commentButton.Size.Height + 2);
              

                if (PrintType == ReportType.RT_Measurement)
                {
                    // Text = "Measurement Report"; // ProjectId.ToString() + ": Show " + MeasIds[0].ToString();
                    keepButton.Visible = true;
                    discardButton.Visible = true;
                    finishLabel.Visible = true;
                   // ControlBox = false;

                    printPreviewControl.Size = new Size(Size.Width - 40, Size.Height - 120);

                    Measurement = new DBInterface.Measurement();
                    DBI.GetMeasurement(MeasIds[0], ref Measurement);
                    Measurements[MeasIds[0]] = Measurement;

                    PrintPreview();
                }
                else if(PrintType == ReportType.RT_Calibration)
                {
                    printPreviewControl.Size = new Size(Size.Width - 40, Size.Height - 90);

                    commentButton.Visible = false;
                    PrintCalibrationPreview();

                }
            }

            printPreviewControl.MouseWheel += new MouseEventHandler(printPreview_MouseWheel);
        }

        private void printPreview_MouseWheel(object sender, MouseEventArgs e)
        {
            // Do stuff!
            int delte = e.Delta;
            int currentPage = printPreviewControl.StartPage;
            int inc = e.Delta > 0 ? -1 : 1;
            int newPage = currentPage + inc;
            if (newPage < 0 || newPage >= m_page)
                return;



            printPreviewControl.StartPage = newPage;//((int)pageUpDown.Value) - 1;
            printPreviewControl.Update();

            pageUpDown.Value = newPage+1;
        }


        private void UpdateReportList()
        {
            reportList.Items.Clear();
            reportList.ListViewItemSorter = null;
            reportList.Sorting = SortOrder.None;

            int i = 0;
            foreach (var meas in Measurements)
            {
                reportList.Items.Add(meas.Value.TypeText);                
                reportList.Items[i].SubItems.Add(meas.Value.Time.ToString("yy/MM/dd HH:mm:ss"));
                reportList.Items[i].Tag = meas.Key;
                i++;
            }

            Sorter.ByColumn = LastSortColumn;
            reportList.Sorting = LastSortOrder;
            reportList.ListViewItemSorter = Sorter;
        }

        private string GetAngularDef(int signDef)
        {
            string dir = signDef > 0 ? "down" : "up";
            string text1 = "Angular definitions:\n\tRoll / Pitch are positive if stbd / aft " + dir + " (Cartesian coordinates).\n\t";
            string text2 = "The tilt vector mark indicates the high point (Polar coordinates).\n\tThe angle of the tilt vector is positive clockwise from fore.\n\tElevation is positive up.\n\tAzimuth is positive clockwise from fore.";
            return text1 + text2;
        }

        private bool FillMeasIds()
        {
            if (MultiMode)
            {
                if (reportList.CheckedItems.Count == 0)
                    return false;

                MeasIds = new List<int>();
                int i = 0;
                foreach (var chitem in reportList.CheckedItems)
                {
                    int selected = reportList.CheckedItems[i].Index;
                    MeasIds.Add((int)reportList.Items[selected].Tag);
                    i++;
                }
            }

            return true;
        }

        private void PrintCalibrationPreview()
        {          
            m_document = new PrintDocument();
            m_document.PrinterSettings = m_printerSettings;

            m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettingsCalib);
            m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrintCalib);
            m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPageCalib);
            m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrintCalib);

            printPreviewControl.Document = m_document;
            printPreviewControl.StartPage = 0;
            pageUpDown.Value = 1;
        }

        private void PrintPreview()
        {
            if (!FillMeasIds())
            {
                printPreviewControl.Document = null;
                m_page = 0;
                pageUpDown.Minimum = 0;
                pageUpDown.Maximum = 0;
                pageUpDown.Value = 0;
                numLabel.Text = "/ 0";
                return;
            }
            
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


            if (PrintType == ReportType.RT_Measurement)
            {
                m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettings);
                m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrint);
                m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPage);
                m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrint);
            }
            else
            {
                m_document.QueryPageSettings += new System.Drawing.Printing.QueryPageSettingsEventHandler(QuerySettingsCalib);
                m_document.BeginPrint += new System.Drawing.Printing.PrintEventHandler(BeginPrintCalib);
                m_document.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(PrintPageCalib);
                m_document.EndPrint += new System.Drawing.Printing.PrintEventHandler(EndPrintCalib);

            }

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

        private void QuerySettingsCalib(object sender, System.Drawing.Printing.QueryPageSettingsEventArgs e)
        {
            e.PageSettings.Margins = new System.Drawing.Printing.Margins(70, 70, 70, 50);
        }

        private void EndPrintCalib(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            pageUpDown.Minimum = 1;
            pageUpDown.Maximum = m_page;
            UpdateColumns();
            numLabel.Text = "/ " + m_page.ToString();
        }

        private void BeginPrintCalib(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            m_page = 0;
            m_yPos = 0;
            MeasNum = 0;
            HeadPage = true;
        }

        private void PrintPageCalib(object sender, PrintPageEventArgs e)
        {         
            PrintArgs = e;
            Rectangle bound = e.PageBounds;
            bound.Inflate(-2, -2);
            DrawPageNum(e);

            bool done = PrintCalibrationData();

            if (done)
            {
            
            }
            else
            {
                HeadPage = false;
            }

            m_page++;
            m_yPos = PrintArgs.MarginBounds.Top;
            e.HasMorePages = !done;
        }

        private void QuerySettings(object sender, System.Drawing.Printing.QueryPageSettingsEventArgs e)
        {
             e.PageSettings.Margins = new System.Drawing.Printing.Margins(70, 70, 70, 50);
        }

        private void EndPrint(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            pageUpDown.Minimum = 1;
            pageUpDown.Value = 1;
            pageUpDown.Maximum = m_page;
            UpdateColumns();
            numLabel.Text = "/ " + m_page.ToString();
        }


        private void BeginPrint(object sender, System.Drawing.Printing.PrintEventArgs e)
        {
            m_page = 0;
            m_yPos = 0;
            MeasNum = 0;
            HeadPage = true;
            Measurement = null;           
            
            if (MeasIds.Count > 0)
                Measurement = Measurements[MeasIds[MeasNum]];
        }

        private void PrintPage(object sender, PrintPageEventArgs e)
        {
            if (Measurement == null)
            {
                e.HasMorePages = false;
                return;
            }

            if (!PrintMeasFunc.ContainsKey(Measurement.Type))
            {
                return;
            }

            PrintArgs = e;
            Rectangle bound = e.PageBounds;
            bound.Inflate(-2, -2);
            DrawPageNum(e);

            
            bool done = PrintMeasFunc[Measurement.Type].Invoke();
           
            if(done)
            {
                OnlyImagesLeft = false;                
                Images = null;

                if (++MeasNum < MeasIds.Count)
                {
                    //We have more reports
                    done = false;
                    HeadPage = true;
                    m_yPos = 0;
                    Measurement = Measurements[MeasIds[MeasNum]];
                   
                }
            }
            else
            {
                HeadPage = false;
            }

            m_page++;
            m_yPos = PrintArgs.MarginBounds.Top;
            e.HasMorePages = !done;
        }

        private void DrawPageNum(PrintPageEventArgs e)
        {
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;

            RectangleF rect = new RectangleF(e.MarginBounds.Left, e.MarginBounds.Bottom, e.MarginBounds.Width, 20);
            string str = "Schill Reglerteknik AB";
            PrintArgs.Graphics.DrawString(str, new Font("Ariel", 8, FontStyle.Regular), Brushes.Black, rect, sf);
            PrintArgs.Graphics.DrawString((m_page+1).ToString(), new Font("Ariel", 8, FontStyle.Bold), Brushes.Black, e.MarginBounds.Right, e.MarginBounds.Bottom);

        }


        private void DrawText(string text, Graphics gr, Font font, Brush br, Rectangle rect, double percX, double percY, StringFormat sf=null)
        {
            int x = rect.Left + (int)((double)rect.Width * (percX / 100.0f));
            int y = rect.Top + (int)((double)rect.Height * (percY / 100.0f));

            if(sf == null)
                gr.DrawString(text, font, br, x, y);
            else
                gr.DrawString(text, font, br, x, y, sf);
        }

        private void pageUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (pageUpDown.Value < 1)
                return;
            printPreviewControl.StartPage = ((int)pageUpDown.Value) - 1;
            printPreviewControl.Update();
        }

        private void reportList_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            PrintPreview();


           /* if (reportList.CheckedItems.Count > 0)
            {
                int item = reportList.CheckedItems[0].Index;
                MeasId = (int)reportList.Items[item].Tag;
                PrintPreview();
                //Logger.Inst.

            }
           // reportList.CheckedItems[0].Index;
           */
        }


        private void commentButton_Click(object sender, EventArgs e)
        {
            OpenComment();
        }

        private void OpenComment()
        {
            int measId = 0;
            if (MultiMode)
            {
                if (reportList.SelectedItems.Count > 0)
                {
                    int item = reportList.SelectedItems[0].Index;
                    measId = (int)reportList.Items[item].Tag;
                }
            }
            else
            {
                measId = MeasIds[0];
            }
            if (measId == 0)
                return;

            DBInterface.Measurement meas = Measurements[measId];

            EditTextForm etf = new EditTextForm(DBI, meas.ID);
            etf.textBox.Text = meas.Comment;
            if (etf.ShowDialog(this) == DialogResult.OK)
            {
                meas.Comment = etf.textBox.Text;
                DBI.UpdateComment(meas.ID, meas.Comment);
                PrintPreview();
            }
            else return;


            
        }

        private void reportList_DoubleClick(object sender, EventArgs e)
        {
        
        }

        private void reportList_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            UpdateStates();    
        }

        private void UpdateStates()
        {
            commentButton.Enabled = (reportList.SelectedItems.Count > 0);
        }

        private void DrawHeader() 
        {
            //DBInterface.Measurement measurement = new DBInterface.Measurement();
            //DBI.GetMeasurement(MeasId, ref measurement);                       
            Graphics gr = PrintArgs.Graphics;

            startYHeadPerc = 40;
            int headX = PrintArgs.MarginBounds.Left;
            int headY = PrintArgs.MarginBounds.Top;

            string angDef = GetAngularDef(Project.SignDef);
            gr.DrawString(angDef, HeadFont, new SolidBrush(Color.FromArgb(255, 75,75,75)), headX, headY);
            SizeF size = gr.MeasureString(angDef, HeadFont);

            headY = headY + (int)size.Height + MargY;

            HeadRect = new Rectangle(new Point(headX, headY), new Size(PrintArgs.MarginBounds.Width, 140));

            Color lineCol = Color.FromArgb(255, 180, 180, 180);
            gr.FillRectangle(new SolidBrush(HeadBGColor), HeadRect);
            gr.DrawRectangle(new Pen(lineCol), HeadRect);
            int xPerc = 2;
            //string text = "AZIMUTH ALIGNMENT ERRORS ";

            //gr.DrawString(text, new Font("Ariel", 8, FontStyle.Bold), Brushes.Black, headX +10, headY+10);
            StringFormat sfc = new StringFormat();
            sfc.Alignment = StringAlignment.Center;

            DrawText(Measurement.TypeText.ToUpper(), gr, new Font("Ariel", 10, FontStyle.Bold), Brushes.Black, HeadRect, 50, 8, sfc);
            DrawText("PROJECT", gr, HeadFont, Brushes.Black, HeadRect, 25, 22, sfc);
            DrawText("MEASUREMENT", gr, HeadFont, Brushes.Black, HeadRect, 75, 22, sfc);

            int y = HeadRect.Top + (int)((double)HeadRect.Height * (35 / 100.0f));
            int x1 = HeadRect.Left + (int)((double)HeadRect.Width * (2 / 100.0f));
            int x2 = HeadRect.Left + (int)((double)HeadRect.Width * (55 / 100.0f));
            gr.DrawLine(new Pen(lineCol, 1), x1, y, x2, y);
            x1 = HeadRect.Left + (int)((double)HeadRect.Width * (60 / 100.0f));
            x2 = HeadRect.Left + (int)((double)HeadRect.Width * (98 / 100.0f));
            gr.DrawLine(new Pen(lineCol, 1), x1, y, x2, y);

            DrawText("Project:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
            DrawText("Ship:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText("Operator:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace*2);
            DrawText("DAU s/n:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace*3);

            xPerc = xPerc + 10;
            DrawText(Project.Name, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
            DrawText(Project.Ship.ToString(), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText(Project.Operator, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace*2);
            DrawText(Project.DAUSerial.ToString("###"), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace*3);

            xPerc = 30;
            DrawText("Date:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
            DrawText("Place:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText("Latitude:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 2);
            DrawText("Units:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 3);
            

            xPerc = xPerc + 10;
            DrawText(Measurement.Time.ToString("yyyy/MM/dd HH:mm:ss"), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
            DrawText(Project.Location, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
            DrawText(Project.Latitude.ToString("0.00") + " [deg]", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 2);
            DrawText(Project.UnitText, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace * 3);

            xPerc = 60;
            DrawText("Time const:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
            xPerc = xPerc + 10;
            DrawText(Measurement.TimeConstant.ToString("0.0") + " [s]", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);

            startYHeadPerc += yHeadSpace;
            m_yPos = HeadRect.Bottom + MargY;
        }

        private int DrawTableLine(Graphics gr, List<TableItem> table, Point pos, int width, Font font)
        {            
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
                SizeF size = gr.MeasureString(ti.Text, font, w, sf);
                
                gr.DrawString(ti.Text, font, new SolidBrush(ti.Color), textRect, sf);

              //  Rectangle drawRect = new Rectangle(x, pos.Y, w, 100);
              //  gr.DrawRectangle(new Pen(Color.Black), drawRect);                

                xLoc = (int)textRect.Right;
                if (size.Height > maxY)
                    maxY = (int)size.Height;
            }

            return maxY;
        }

        private int DrawPlainText(Graphics gr, string text, int pos)
        {
            int h = 0;
            int startPos = pos;
            if (text != "")
            {
                //pos += MargY;
                SolidBrush br = new SolidBrush(Color.Black);
                SizeF size = gr.MeasureString(text, Font, HeadRect.Width);
                Rectangle rect = new Rectangle(HeadRect.Left, pos, HeadRect.Width, (int)size.Height);
                DrawText(text, gr, Font, br, rect, 2, 0);
                pos += (int)size.Height + MargY;
                h += pos - startPos;
            }

            return h;
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
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, pos, HeadRect.Right, pos);
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
                pos += MargY/2;
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

        private bool EnoughSpace(int needed)
        {
            return (m_yPos + needed) < PrintArgs.MarginBounds.Bottom;                        
        }

        private bool DrawImage(Graphics gr, DBInterface.ImageInfo image)
        {
            Image im;
            try
            {
                im = Image.FromFile(image.Path);
            }
            catch(Exception e)
            {
                SolidBrush red = new SolidBrush(Color.Red);
                gr.DrawString(image.Path, Font, red, HeadRect.Left, m_yPos);
                m_yPos += MargY;
                return true;
            }
            double ratio = (double)im.Width / im.Height;
            double w = HeadRect.Width;
            double h = w / ratio;

            if(!EnoughSpace((int)h))
            {
                //Image dont fit.                
                return false;
            }

            gr.DrawImage(im, new Rectangle(HeadRect.Left, m_yPos, (int)w, (int)h));
            m_yPos += (int)h+MargY;

            return true;
        }


        private bool DrawImages(Graphics gr, ref List<DBInterface.ImageInfo> images)
        {
            foreach(DBInterface.ImageInfo ii in images)
            {
                if (ii.Include && !ii.Drawn)
                {
                    ii.Drawn = DrawImage(gr, ii);
                    if (!ii.Drawn)
                        return false;
                }
            }

            return true;
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

        private void printPreviewControl_Click(object sender, EventArgs e)
        {
            printPreviewControl.Focus();
        }

        private void ReportForm_SizeChanged(object sender, EventArgs e)
        {
            UpdateColumns();
        }

        private void UpdateColumns()
        {
            if (printPreviewControl.Document == null)
                return;

            int oldColumns = printPreviewControl.Columns;
            int newColumns = 1;

            if (m_page > 1)
            {
                float col = printPreviewControl.Size.Width / (printPreviewControl.Document.DefaultPageSettings.PaperSize.Width * (float)printPreviewControl.Zoom) ;
               
                //float col = (float)printPreviewControl.Size.Width / printPreviewControl.Size.Height * (float)printPreviewControl.Zoom / 0.7f;
                int c = (int)col;
                if (c < 1) c = 1;
                newColumns = Math.Min(m_page, c);             
            }

            if (newColumns != oldColumns)
            {
                printPreviewControl.Columns = newColumns;
              
                printPreviewControl.Update();
            }



        }

        private void allButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in reportList.Items)
            {
                item.Checked = true;
            }
        }

        private void noneButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in reportList.Items)
            {
                item.Checked = false;
            }
        }

        private void reportList_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            this.reportList.ItemChecked -= new System.Windows.Forms.ItemCheckedEventHandler(this.reportList_ItemChecked);
            reportList.ListViewItemSorter = null;

            if (Sorter.LastSort == e.Column)
            {
                if (reportList.Sorting == SortOrder.Ascending)
                    reportList.Sorting = SortOrder.Descending;
                else
                    reportList.Sorting = SortOrder.Ascending;
            }
            else
            {
                reportList.Sorting = SortOrder.Descending;
            }

            LastSortOrder = reportList.Sorting;
            LastSortColumn = e.Column;
            Sorter.ByColumn = e.Column;
            reportList.ListViewItemSorter = Sorter;
            this.reportList.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.reportList_ItemChecked);

            PrintPreview();
        }

        private void delButton_Click(object sender, EventArgs e)
        {
            if (reportList.SelectedItems.Count > 0)
            {
                int item = reportList.SelectedItems[0].Index;
                int measId = (int)reportList.Items[item].Tag;
                string time = reportList.Items[item].SubItems[1].Text;

                string msg = $"Delete measurement '{reportList.Items[item].Text}' ({time})";
                DialogResult res = MessageBox.Show(msg, "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
                if(res == DialogResult.Yes)
                {
                    DBI.DeleteMeasurement(measId);

                    DBI.GetProjectMeasurements(ProjectId, ref Measurements);
                   
                    UpdateReportList();

                }
                PrintPreview();
            } 
        }

        private bool PrintTiltAlignment()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {               
                DrawHeader();

                DBInterface.TiltAlignment tam = new DBInterface.TiltAlignment();
                DBI.GetTiltAlignmentMeas(ref Measurement, ref tam);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetTiltAlignmentCh(tam.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);                

                int xPerc = 60;
                DrawText("LOS Dir:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText("Elev. Comp:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
                xPerc = xPerc + 10;
                DrawText(tam.LOSDir, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText(tam.ElevComp ? "On" : "Off", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                table.Add(new TableItem("Elev.\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Bias*\n" + Project.UnitText, -1, wPerc));
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.TiltAlignmentCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem("", -1, wPerc));
                        table.Add(new TableItem("", -1, wPerc));
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

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;



                DrawText("*Bias across line-of-sight.", gr, HeadFont, MainBr, new Rectangle(HeadRect.Left, PrintArgs.PageBounds.Bottom, HeadRect.Width, 10), 2, 0);
            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintAzimuthAlignment()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.AzimuthAlignment aam = new DBInterface.AzimuthAlignment();
                DBI.GetAzimuthAlignmentMeas(ref Measurement, ref aam);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetAzimuthAlignmentCh(aam.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);
                
                int xPerc = 60;
                DrawText("Roll Exc:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);               
                xPerc = xPerc + 10;
                DrawText(aam.RollExcentricity.ToString("0.00"), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                                
                int wPerc = 10;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Nom Az\n[deg]", -1, wPerc));
                table.Add(new TableItem("Nom \u0394Az\n[deg]", -1, wPerc));
                table.Add(new TableItem("Meas \u0394Az\n[deg]", -1, wPerc));
                table.Add(new TableItem("Az Err\n[deg]", -1, wPerc));
                table.Add(new TableItem("Az Err\n" + Project.UnitText, -1, wPerc));                
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;
                
                foreach (DBInterface.AzimuthAlignmentCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                    table.Add(new TableItem(ch.NominalAz.ToString("0.00"), -1, wPerc));
                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));                        
                    }
                    else
                    {                       
                        table.Add(new TableItem(ch.NominalAzimuthDiff.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.MeasuredAzimuthDiff.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.MeasuredNominalDiff.ToString("0.00"), -1, wPerc));
                        double nomDiff = ch.MeasuredNominalDiff * Math.PI / 180.0f * 1000.0f;
                        table.Add(new TableItem(nomDiff.ToString("0.00"), -1, wPerc));
                        
                    }

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;

            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintTiltFlatnessPl()
        {            
            Graphics gr = PrintArgs.Graphics;
            if (Images != null && OnlyImagesLeft)
                return DrawImages(gr, ref Images);

            int wPerc = 7;            
            List<TableItem> table;

            if (HeadPage)
            {
                DrawHeader();
            }
            DBInterface.TiltFlatnessPl m = new DBInterface.TiltFlatnessPl();
            DBI.GetTiltFlatnessPlMeas(ref Measurement, ref m);

            Channels = new List<DBInterface.ChannelBase>();
            DBI.GetTiltFlatnessPlCh(m.ID, ref Channels);
            SetRefChannel(Measurement, Channels);

            bool resRef = Channels.Any(c => !c.IsRef);
            bool resCh = Channels.Any(c => c.IsRef);

            if (!resRef || !resCh)
                return true;

            if (HeadPage)
            {
                ChannelErrList = new List<DBInterface.ChannelErrBaseList>();
                foreach (DBInterface.TiltFlatnessPlCh ch in Channels)
                {
                    if (!ch.IsRef)
                    {
                        DBInterface.ChannelErrBaseList errList = new DBInterface.ChannelErrBaseList();
                        DBI.GetTiltFlatnessPlChErr(ch.ID, errList);
                        ChannelErrList.Add(errList);
                    }
                }
               

                int yPerc = startYHeadPerc;
                int xPerc = 60;
                DrawText("Measurements:", gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                xPerc = xPerc + 15;
                DrawText(m.numMeas.ToString(), gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);

               
                table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 20, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                table.Add(new TableItem("Elev.\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Std dev\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Max dev\n" + Project.UnitText, -1, wPerc+1));
                table.Add(new TableItem("Azim\n[deg]", -1, wPerc));
               
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;

                foreach (DBInterface.TiltFlatnessPlCh ch in Channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 20, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                    }
                    else
                    {
                        table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.elevation.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.stdDev.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.maxDev.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.azimuth.ToString("0.00"), -1, wPerc));


                    }

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += MargY;

                table = new List<TableItem>();
                table.Add(new TableItem("Flatness Errors", 2, 20));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, H1TextFont);
                m_yPos += MargY;

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

            }

            if (ChannelErrList.Count == 0)
                return true;

            //Print error table
            DBInterface.ChannelErrBaseList baseList = ChannelErrList[0];
            if (baseList.Any(e => !e.done))
            {
                wPerc = 10;
                table = new List<TableItem>();
                table.Add(new TableItem("Azimuth\n  [deg]", 2, wPerc, Color.Black, StringAlignment.Near));
                Channels.ForEach(c => { if (!c.IsRef) table.Add(new TableItem(c.Station + "\n" + Project.UnitText, -1, wPerc)); });
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Left + HeadRect.Width*0.11f*(ChannelErrList.Count+1), m_yPos);
                m_yPos += SmalMarg;


                int i = 0;
                foreach (DBInterface.ChannelErrBase err in baseList)
                {
                    if (err.done)
                        continue;

                    if (m_yPos + 10 > PrintArgs.MarginBounds.Bottom)
                    {
                        //Don't fit, new page.
                        return false;
                    }
                    table = new List<TableItem>();
                    table.Add(new TableItem(err.azimuth.ToString("0.00"), 2, wPerc, Color.Black, StringAlignment.Near));

                    foreach (DBInterface.ChannelErrBaseList errL in ChannelErrList)
                    {
                        table.Add(new TableItem(errL[i].error.ToString("0.00"), -1, wPerc));
                    }
                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                    err.done = true;
                    i++;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Left + HeadRect.Width * 0.11f * (ChannelErrList.Count + 1), m_yPos);
            }

            if (!EnoughSpace(50))
            {
                //Don't fit, new page.
                return false;
            }

            m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);

            m_yPos += DrawComment(gr, Measurement, m_yPos);
            m_yPos += MargY;

            OnlyImagesLeft = true;
            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintTiltFlatnessFo()
        {
            Graphics gr = PrintArgs.Graphics;
            if (Images != null && OnlyImagesLeft)
                return DrawImages(gr, ref Images);
            
            int wPerc = 10;
            List<TableItem> table = new List<TableItem>();

            if (HeadPage)
            {
                DrawHeader();
            }

            DBInterface.TiltFlatnessFo m = new DBInterface.TiltFlatnessFo();
            DBI.GetTiltFlatnessFoMeas(ref Measurement, ref m);

            Channels = new List<DBInterface.ChannelBase>();
            DBI.GetTiltFlatnessFoCh(m.ID, ref Channels);
            SetRefChannel(Measurement, Channels);            

            bool resRef = Channels.Any(c => !c.IsRef);
            bool resCh = Channels.Any(c => c.IsRef);

            if (!resRef || !resCh)
                return true;

            DBInterface.ChannelBase foCh1 = Channels.First(c => !c.IsRef);
            if (foCh1 == null)
                return true;

            DBInterface.ChannelBase foCh2 = Channels.First(c => c.IsRef);
            if (foCh2 == null)
                return true;

            DBInterface.TiltFlatnessFoCh measCh = foCh1 as DBInterface.TiltFlatnessFoCh;
            DBInterface.TiltFlatnessFoCh refCh = foCh2 as DBInterface.TiltFlatnessFoCh;
            
            if(HeadPage)
            { 
                ChannelErrList = new List<DBInterface.ChannelErrBaseList>();

                ChannelErrList.Add(new DBInterface.ChannelErrBaseList());
                DBI.GetTiltFlatnessFoChErr(measCh.ID, ChannelErrList[0]);
                
                int yPerc = startYHeadPerc;
                int xPerc = 60;
                DrawText("Measurements:", gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                xPerc = xPerc + 15;
                DrawText(m.numMeas.ToString(), gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                bool warping = false;               
                if (ChannelErrList[0].Count > 0)
                {
                    DBInterface.TiltFlatnessFoChErr chErr = ChannelErrList[0][0] as DBInterface.TiltFlatnessFoChErr;

                    if (chErr.indexArmL2 != -1) // -1 indicates rectangular foundation
                    {
                        if (chErr.indexArmL1 > 0)
                        {
                            yPerc += yHeadSpace;
                            xPerc = 60;
                            DrawText("Ix Arm Length 1:", gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                            xPerc = xPerc + 15;
                            DrawText(chErr.indexArmL1.ToString(), gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);

                        }
                        if (chErr.indexArmL2 > 0)
                        {
                            warping = true;
                            yPerc += yHeadSpace;
                            xPerc = 60;
                            DrawText("Ix Arm Length 2:", gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                            xPerc = xPerc + 15;
                            DrawText(chErr.indexArmL2.ToString(), gr, HeadFont, MainBr, HeadRect, xPerc, yPerc);
                            yPerc += yHeadSpace;
                        }
                    }                    
                }
                else
                    return true;


                table.Add(new TableItem("Station", 25, 30, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Sensor\n" + Project.UnitText, -1, wPerc));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left + HeadRect.Width * 0.2f, m_yPos, HeadRect.Right - HeadRect.Width * 0.2f, m_yPos);
                m_yPos += SmalMarg;

                //Reference row
                table = new List<TableItem>();
                table.Add(new TableItem("Reference: " + refCh.Station, 25, 30, RefColor, StringAlignment.Near));
                table.Add(new TableItem(refCh.Channel, -1, wPerc, RefColor));
                table.Add(new TableItem(refCh.SensorSN, -1, wPerc, RefColor));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;

                //Measured object row
                table = new List<TableItem>();
                table.Add(new TableItem("Measured:  " + measCh.Station, 25, 30, Color.Black, StringAlignment.Near));
                table.Add(new TableItem(measCh.Channel, -1, wPerc));
                table.Add(new TableItem(measCh.SensorSN, -1, wPerc));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left + HeadRect.Width * 0.2f, m_yPos, HeadRect.Right - HeadRect.Width * 0.2f, m_yPos);
                m_yPos += MargY;


                table = new List<TableItem>();
                table.Add(new TableItem("Measurement Result", 40, 22));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, H1TextFont);
                m_yPos += MargY;

                wPerc = 10;
                table = new List<TableItem>();

                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));               
                table.Add(new TableItem("Std dev\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Max dev\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Azim\n[deg]", -1, wPerc));
                table.Add(new TableItem("Elev1\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Elev2\n" + Project.UnitText, -1, wPerc));
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;

                table = new List<TableItem>();
                table.Add(new TableItem(measCh.roll.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.pitch.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.tilt.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.angle.ToString("0.00"), -1, wPerc));               
                table.Add(new TableItem(measCh.stdDev.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.maxDev.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.azimuth.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(measCh.elevation.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(warping ?measCh.elevation2.ToString("0.00"):"-", -1, wPerc));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += MargY * 1;

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

            }

            
            //Print error table
            if (ChannelErrList[0].Any(e => !e.done))
            {
                DBInterface.TiltFlatnessFoChErr chErr = ChannelErrList[0][0] as DBInterface.TiltFlatnessFoChErr;
                bool isCircular = chErr.indexArmL2 != -1;

                table = new List<TableItem>();
                table.Add(new TableItem(isCircular ? "Circular" : "Rectangular" + " Foundation", 40, 24));
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, H1TextFont);
                m_yPos += MargY;

                table = new List<TableItem>();
                table.Add(new TableItem("Azim\n[deg]", 20, wPerc));
                if(!isCircular)
                    table.Add(new TableItem("Arm\n[mm]", -1, wPerc));
                table.Add(new TableItem("Err1\n[mm]", -1, wPerc));
                if (isCircular)
                    table.Add(new TableItem("Err2\n[mm]", -1, wPerc));
                table.Add(new TableItem("h1\n[mm]", -1, wPerc));
                if (isCircular)
                {
                    table.Add(new TableItem("h2\n[mm]", -1, wPerc));
                    table.Add(new TableItem("dh\n[mm]", -1, wPerc));
                }
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left + HeadRect.Width * 0.2f, m_yPos, HeadRect.Right - HeadRect.Width * 0.1f, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.TiltFlatnessFoChErr err in ChannelErrList[0])
                {
                    if (err.done)
                        continue;

                    if (!EnoughSpace(10))
                    {
                        //Don't fit, new page.
                        return false;
                    }

                    bool MultiArms = err.indexArmL2 > 0;
                    table = new List<TableItem>();
                    table.Add(new TableItem(err.azimuth.ToString("0.00"), 20, wPerc));
                    if (!isCircular)
                        table.Add(new TableItem(err.indexArmL1.ToString(), -1, wPerc));
                    table.Add(new TableItem(err.error.ToString("0.00"), -1, wPerc));
                    if (isCircular)
                        table.Add(new TableItem(MultiArms ? err.error2.ToString("0.00") : "-", -1, wPerc));
                    table.Add(new TableItem((err.error - measCh.bottomErr).ToString("0.00"), -1, wPerc));
                    if (isCircular)
                    {
                        table.Add(new TableItem(MultiArms ? (err.error2 - measCh.bottomErr).ToString("0.00") : "-", -1, wPerc));
                        table.Add(new TableItem(MultiArms ? err.dh.ToString("0.00") : "-", -1, wPerc));
                    }
                    err.done = true;

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left + HeadRect.Width * 0.2f, m_yPos, HeadRect.Right - HeadRect.Width * 0.1f, m_yPos);
            }

            if (m_yPos + 50 > PrintArgs.MarginBounds.Bottom)
            {
                //Don't fit, new page.
                return false;
            }

            m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);

            m_yPos += DrawComment(gr, Measurement, m_yPos);
            m_yPos += MargY;

            OnlyImagesLeft = true;
            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintGyroPerf()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.GyroPerf gpm = new DBInterface.GyroPerf();
                DBI.GetGyroPerfMeas(ref Measurement, ref gpm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetGyroPertCh(gpm.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));               
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.GyroPerfCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));                   
                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                    }
                    else
                    {
                        table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));
                    }

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;
            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintCommonFlat()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.CommonFlat cfm = new DBInterface.CommonFlat();
                DBI.GetCommonFlatMeas(ref Measurement, ref cfm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetCommonFlatCh(cfm.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

                m_yPos -= MargY/2;
                string s = "Sensor offset updated: " + (cfm.DBUpdated ? "Yes" : "No");
                m_yPos += DrawPlainText(gr, s, m_yPos);
                m_yPos += SmalMarg;

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));               
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Parallell\nBias*\n" + Project.UnitText, -1, wPerc+2));
                table.Add(new TableItem("Perpendicular\nBias*\n" + Project.UnitText, -1, wPerc+5));
                table.Add(new TableItem("Temperature\n[°C]", -1, wPerc+5));
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;

               
                foreach (DBInterface.CommonFlatCh ch in channels)
                {
                    table = new List<TableItem>();
                    //table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem("Calibration Flat", 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));

                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                    }
                    else
                    {
                        table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                    }

                    table.Add(new TableItem(ch.ParallellBias.ToString("0.00"), -1, wPerc+2));
                    table.Add(new TableItem(ch.PerpendicularBias.ToString("0.00"), -1, wPerc+5));
                    table.Add(new TableItem(ch.Temperature.ToString("0.00"), -1, wPerc + 5));                    

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;
                DrawText("*Bias across line-of-sight.", gr, HeadFont, MainBr, new Rectangle(HeadRect.Left, PrintArgs.PageBounds.Bottom - 30, HeadRect.Width, 10), 2, 0);
            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            DrawText("*Bias across line-of-sight.", gr, HeadFont, MainBr, new Rectangle(HeadRect.Left, PrintArgs.PageBounds.Bottom - 30, HeadRect.Width, 10), 2, 0);

            return true;
        }

        private bool PrintAbsoluteMode()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.AbsoulteModeVerif amm = new DBInterface.AbsoulteModeVerif();
                DBI.GetAboluteModeMeas(ref Measurement, ref amm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetAboluteModeCh(amm.ID, ref channels);
               // SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

                int xPerc = 60;
                DrawText("Parallax Comp:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText("Elevation Comp:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
                string td = amm.parallaxComp ? "" : "Target Distance:";
                DrawText(td, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + 2*yHeadSpace);
                xPerc = xPerc + 15;
                DrawText(amm.parallaxComp ? "On" : "Off", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText(amm.elevationComp? "On" : "Off", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
                td = amm.parallaxComp ? "": amm.range.ToString() + " [m]";
                DrawText(td, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + 2*yHeadSpace);

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                table.Add(new TableItem("Elev.\n" + Project.UnitText, -1, wPerc));
                               
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.AbsoulteModeVerifCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                   
                    table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.elevation.ToString("0.00"), -1, wPerc));                    
                    

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;


            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintRelativeMode()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.RelativeModeVerif rmm = new DBInterface.RelativeModeVerif();
                DBI.GetRelativeModeMeas(ref Measurement, ref rmm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetRelativeModeCh(rmm.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

                
                int xPerc = 60;
                DrawText("Parallax Comp:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText("Elevation Comp:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
                string td = rmm.parallaxComp ? "" : "Target Distance:";
                DrawText(td, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + 2*yHeadSpace);
                xPerc = xPerc + 15;
                DrawText(rmm.parallaxComp ? "On" : "Off", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                DrawText(rmm.elevationComp ? "On" : "Off", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + yHeadSpace);
                td = rmm.parallaxComp ? "":rmm.range.ToString() + " [m]";
                DrawText(td, gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc + 2*yHeadSpace);

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                table.Add(new TableItem("Elev.\n" + Project.UnitText, -1, wPerc));
                
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.RelativeModeVerifCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                    if (ch.IsRef)
                    {
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                       
                    }
                    else
                    {
                        table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));
                        table.Add(new TableItem(ch.elevation.ToString("0.00"), -1, wPerc));                    
                    }

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;


            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }
                      
        private bool PrintSensorValidation()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.SensorValidation svm = new DBInterface.SensorValidation();
                DBI.GetSensorvalidationMeas(ref Measurement, ref svm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetSensorvalidationMeasCh(svm.ID, ref channels);
                SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);
                
                int xPerc = 60;
                DrawText("Platform S/N: " + svm.PlatformSN.ToString("000"), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);
                
                m_yPos -= MargY / 2;
                string s = "Sensor azimuth updated: " + (svm.DBUpdated ? "Yes" : "No");
                m_yPos += DrawPlainText(gr, s, m_yPos);
                m_yPos += SmalMarg;               

                int wPerc = 12;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 20, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));               
                table.Add(new TableItem("Roll Scale\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch Scale\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Roll Azimuth\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch Azimuth\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Temperature\n[°C]", -1, wPerc + 3));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;

                foreach (DBInterface.SensorValidationCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem("Calib. SF & Az. Table", 2, 20, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    if (ch.IsRef) 
                    { 
                        string refStr = "Ref";
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                        table.Add(new TableItem(refStr, -1, wPerc, RefColor));
                    }
                    else 
                    {
                        table.Add(new TableItem(ch.rollScale.ToString("0.00000"), -1, wPerc));
                        table.Add(new TableItem(ch.pitchScale.ToString("0.00000"), -1, wPerc));
                        table.Add(new TableItem(ch.rollAz.ToString("0.00000"), -1, wPerc));
                        table.Add(new TableItem(ch.pitchAz.ToString("0.00000"), -1, wPerc));
                    }
                    table.Add(new TableItem(ch.temperature.ToString("0.00"), -1, wPerc + 3));

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }
                
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);              

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;
            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintLiveGraph()
        {
            Graphics gr = PrintArgs.Graphics;
            if (HeadPage)
            {
                DrawHeader();

                DBInterface.LiveGraph lgm = new DBInterface.LiveGraph();
                DBI.GetLiveGraphMeas(ref Measurement, ref lgm);

                List<DBInterface.ChannelBase> channels = new List<DBInterface.ChannelBase>();
                DBI.GetLiveGraphCh(lgm.ID, ref channels);
               // SetRefChannel(Measurement, channels);

                Images = new List<DBInterface.ImageInfo>();
                DBI.GetImages(Measurement.ID, ref Images);

                int xPerc = 60;
                DrawText("Sample Rate:", gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);                
                xPerc = xPerc + 10;
                DrawText(lgm.sampleRate.ToString("0.00"), gr, HeadFont, MainBr, HeadRect, xPerc, startYHeadPerc);                

                int wPerc = 8;
                List<TableItem> table = new List<TableItem>();
                table.Add(new TableItem("Station", 2, 25, Color.Black, StringAlignment.Near));
                table.Add(new TableItem("Ch", -1, 5));
                table.Add(new TableItem("Sensor\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Adapt.\n(s/n)", -1, wPerc));
                table.Add(new TableItem("Roll\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Pitch\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Tilt\n" + Project.UnitText, -1, wPerc));
                table.Add(new TableItem("Angle\n[deg]", -1, wPerc));
                table.Add(new TableItem("Temperature\n[°C]", -1, wPerc+3));
                               
                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;


                foreach (DBInterface.LiveGraphCh ch in channels)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(ch.Station, 2, 25, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(ch.Channel, -1, 5));
                    table.Add(new TableItem(ch.SensorSN, -1, wPerc));
                    table.Add(new TableItem(ch.AdapterSN, -1, wPerc));
                    table.Add(new TableItem(ch.roll.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.pitch.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.tilt.ToString("0.00"), -1, wPerc));
                    table.Add(new TableItem(ch.angle.ToString("0.00"), -1, wPerc));                   
                    table.Add(new TableItem(ch.temperature.ToString("0.00"), -1, wPerc+3));                   

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                    m_yPos += SmalMarg;
                }

                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

                m_yPos += DrawCalibInfo(gr, Measurement, m_yPos);
                // m_yPos += MargY;

                m_yPos += DrawComment(gr, Measurement, m_yPos);
                m_yPos += MargY;
                
            }

            if (Images != null)
                return DrawImages(gr, ref Images);

            return true;
        }

        private bool PrintCalibrationData()
        {
            Graphics gr = PrintArgs.Graphics;

            
            if (HeadPage)
            {
                m_yPos = PrintArgs.MarginBounds.Top;

                DAUCalib = new List<DBInterface.DAUData>();
                DBI.GetDAUCalibration(ref DAUCalib);

                SensorCalib = new List<DBInterface.SensorCalibrationInfo>();
                DBI.GetSensorCalibration(ref SensorCalib);

                GunAdapterCalib = new List<DBInterface.GunAdaptCalib>();
                DBI.GetGunAdapter(ref GunAdapterCalib);

                TheoAdapterCalib = new List<DBInterface.TheoAdaptCalib>();
                DBI.GetTheoAdapter(ref TheoAdapterCalib);

                PlatformCalib = new List<DBInterface.PlatformCorrection>();
                DBI.GetPlatformCorrections(ref PlatformCalib);

                HeadRect = new Rectangle(new Point(PrintArgs.MarginBounds.Left, PrintArgs.MarginBounds.Top), new Size(PrintArgs.MarginBounds.Width, 120));                

                StringFormat sf = new StringFormat();
                sf.Alignment = StringAlignment.Center;

                gr.DrawString("Aligner 308 Calibration Data", CalibHeadFont, new SolidBrush(Color.Black), HeadRect, sf);
                m_yPos += 2 * MargY;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += 2 * MargY;
            }


            if(!DrawDAUCalib())
                return false;

            if (!DrawSensorCalib())
                return false;

            if (!DrawGunAdapterCalib())
                return false;

            if (!DrawTheoAdapterCalib())
                return false;

            if (!DrawPlatformCalib())
                return false;


            return true;
        }

        private bool DrawDAUCalib()
        {
            if(!DAUCalib.Any(e => !e.done))
            {
                return true;
            }

            if (!EnoughSpace(50))
                return false;

            Graphics gr = PrintArgs.Graphics;
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            Rectangle rect = new Rectangle(new Point(HeadRect.Left, m_yPos), new Size(PrintArgs.MarginBounds.Width, 40));
            gr.FillRectangle(new SolidBrush(HeadBGColor), rect);
            gr.DrawString("Data Acquisition Unit", CalibHeadFont, new SolidBrush(Color.Black), rect, sf);

            m_yPos += rect.Height+MargY;

            int wPerc = 12;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("S/N", 2, wPerc, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("Channel", -1, wPerc));
            table.Add(new TableItem("Roll Gain", -1, wPerc));
            table.Add(new TableItem("Pitch Gain", -1, wPerc));
            table.Add(new TableItem("Roll Offset\n[bits]", -1, wPerc));
            table.Add(new TableItem("Pitch Offset\n[bits]", -1, wPerc));
            table.Add(new TableItem("Date", -1, 17));

            m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
            m_yPos += SmalMarg;
            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += SmalMarg;

            foreach (DBInterface.DAUData dd in DAUCalib)
            {                
                if (!EnoughSpace(110))
                    return false;

                if (dd.done)
                    continue;

                table = new List<TableItem>();
                table.Add(new TableItem(dd.sn.ToString(), 2, 15, Color.Black, StringAlignment.Near));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);

                foreach (DBInterface.DAUChData cd in dd.chData)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(cd.ch, 14, wPerc));
                    table.Add(new TableItem(cd.rollGain.ToString("0.00000"), -1, wPerc));
                    table.Add(new TableItem(cd.pitchGain.ToString("0.00000"), -1, wPerc));
                    table.Add(new TableItem(cd.rollOffs.ToString(), -1, wPerc));
                    table.Add(new TableItem(cd.pitchOffs.ToString(), -1, wPerc));
                    table.Add(new TableItem(cd.time.ToString("yyyy/MM/dd HH:mm:ss"), -1, 17));

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);                    
                }
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;
                dd.done = true;
               
            }

            m_yPos += MargY;

            return true;
        }

        private string CalTypeString(DBInterface.CalType type)
        {
            if (!CalTypeMap.ContainsKey(type))
                return "-";

            return CalTypeMap[type];
        }

        private bool DrawSensorCalib()
        {
            if (!SensorCalib.Any(e => !e.done))
            {
                return true;
            }

            if (!EnoughSpace(50))
                return false;

            Graphics gr = PrintArgs.Graphics;
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            Rectangle rect = new Rectangle(new Point(HeadRect.Left, m_yPos), new Size(PrintArgs.MarginBounds.Width, 40));
            gr.FillRectangle(new SolidBrush(HeadBGColor), rect);
            gr.DrawString("Sensor Unit", CalibHeadFont, new SolidBrush(Color.Black), rect, sf);
            m_yPos += rect.Height + MargY;

            int wPerc = 12;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("S/N", 2, 6, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("Parameter", 8, 15, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("a_0", -1, wPerc));
            table.Add(new TableItem("a_1", -1, wPerc));
            table.Add(new TableItem("a_2", -1, wPerc));
            table.Add(new TableItem("a_3", -1, wPerc));
            table.Add(new TableItem("Date", -1, 17));
            table.Add(new TableItem("Platform", -1, wPerc));

            m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
            m_yPos += SmalMarg;
            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += SmalMarg;

            foreach (DBInterface.SensorCalibrationInfo sc in SensorCalib)
            {
                if (!EnoughSpace(110))
                    return false;

                if (sc.done)
                    continue;

                table = new List<TableItem>();
                table.Add(new TableItem(sc.sn.ToString(), 2, 6, Color.Black, StringAlignment.Near));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);

                foreach (var cd in sc.calData)
                {
                    table = new List<TableItem>();
                    table.Add(new TableItem(CalTypeString(cd.Value.type), 8, 15, Color.Black, StringAlignment.Near));
                    table.Add(new TableItem(cd.Value.a[0].ToString("0.00000"), -1, wPerc));
                    table.Add(new TableItem(cd.Value.a[1].ToString("0.00000"), -1, wPerc));
                    table.Add(new TableItem(cd.Value.a[2].ToString("0.000e+00"), -1, wPerc));
                    table.Add(new TableItem(cd.Value.a[3].ToString("0.000e+00"), -1, wPerc));
                    table.Add(new TableItem(cd.Value.time.ToString("yyyy/MM/dd HH:mm:ss"), -1, 17));
                    string platform = (cd.Value.platformSN > 0) ? cd.Value.platformSN.ToString("000") : "-";
                    table.Add(new TableItem(platform, -1, wPerc));

                    m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                }
                m_yPos += SmalMarg;
                gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
                m_yPos += SmalMarg;
                sc.done = true;

            }

            m_yPos += MargY;

            return true;
        }

        private bool DrawGunAdapterCalib()
        {
            if (!GunAdapterCalib.Any(e => !e.done))
            {
                return true;
            }

            if (!EnoughSpace(100))
                return false;

            Graphics gr = PrintArgs.Graphics;
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            Rectangle rect = new Rectangle(new Point(HeadRect.Left, m_yPos), new Size(PrintArgs.MarginBounds.Width, 40));
            gr.FillRectangle(new SolidBrush(HeadBGColor), rect);
            gr.DrawString("Gun Adapter", CalibHeadFont, new SolidBrush(Color.Black), rect, sf);

            m_yPos += rect.Height + MargY;

            int wPerc = 25;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("S/N", 2, wPerc, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("Elevation\n[mrad]", -1, wPerc));
            table.Add(new TableItem("Azimuth\n[mrad]", -1, wPerc));
            table.Add(new TableItem("Date", -1, wPerc));

            m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
            m_yPos += SmalMarg;
            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += SmalMarg;

            foreach (DBInterface.GunAdaptCalib ga in GunAdapterCalib)
            {
                if (!EnoughSpace(10))
                    return false;

                if (ga.done)
                    continue;

                table = new List<TableItem>();
                table.Add(new TableItem(ga.sn.ToString(), 2, wPerc, Color.Black, StringAlignment.Near));
                table.Add(new TableItem(ga.elevation.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(ga.azimuth.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(ga.time.ToString("yyyy/MM/dd HH:mm:ss"), -1, wPerc));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
                
                m_yPos += SmalMarg;
                ga.done = true;
            }

            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);          

            m_yPos += MargY;

            return true;
        }

        private bool DrawTheoAdapterCalib()
        {
            if (!TheoAdapterCalib.Any(e => !e.done))
            {
                return true;
            }

            if (!EnoughSpace(100))
                return false;

            Graphics gr = PrintArgs.Graphics;
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            Rectangle rect = new Rectangle(new Point(HeadRect.Left, m_yPos), new Size(PrintArgs.MarginBounds.Width, 40));
            gr.FillRectangle(new SolidBrush(HeadBGColor), rect);
            gr.DrawString("Theo Adapter", CalibHeadFont, new SolidBrush(Color.Black), rect, sf);

            m_yPos += rect.Height + MargY;

            int wPerc = 20;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("S/N", 2, wPerc, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("Elevation\n[mrad]", -1, wPerc));
            table.Add(new TableItem("Azimuth\n[mrad]", -1, wPerc));
            table.Add(new TableItem("Type", -1, wPerc));
            table.Add(new TableItem("Date", -1, wPerc));

            m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
            m_yPos += SmalMarg;
            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += SmalMarg;

            foreach (DBInterface.TheoAdaptCalib ta in TheoAdapterCalib)
            {
                if (!EnoughSpace(10))
                    return false;

                if (ta.done)
                    continue;

                table = new List<TableItem>();
                table.Add(new TableItem(ta.sn.ToString(), 2, wPerc, Color.Black, StringAlignment.Near));
                table.Add(new TableItem(ta.elevation.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(ta.azimuth.ToString("0.00"), -1, wPerc));
                table.Add(new TableItem(ta.type==0?"Adjustable":"Fix", -1, wPerc));
                table.Add(new TableItem(ta.time.ToString("yyyy/MM/dd HH:mm:ss"), -1, wPerc));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);

                m_yPos += SmalMarg;
                ta.done = true;
            }

            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);

            m_yPos += MargY;

            return true;
        }

        private bool DrawPlatformCalib()
        {
            if (!PlatformCalib.Any(e => !e.done))
            {
                return true;
            }

            if (!EnoughSpace(100))
                return false;

            Graphics gr = PrintArgs.Graphics;
            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            Rectangle rect = new Rectangle(new Point(HeadRect.Left, m_yPos), new Size(PrintArgs.MarginBounds.Width, 40));
            gr.FillRectangle(new SolidBrush(HeadBGColor), rect);
            gr.DrawString("Scale Factor and Azimuth Calibration Fixture", CalibHeadFont, new SolidBrush(Color.Black), rect, sf);

            m_yPos += rect.Height + MargY;

            int wPerc = 33;
            List<TableItem> table = new List<TableItem>();
            table.Add(new TableItem("S/N", 2, wPerc, Color.Black, StringAlignment.Near));
            table.Add(new TableItem("Wedge Angle\n[mrad]", -1, wPerc));            
            table.Add(new TableItem("Date", -1, wPerc));

            m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);
            m_yPos += SmalMarg;
            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += SmalMarg;

            foreach (DBInterface.PlatformCorrection pc in PlatformCalib)
            {
                if (!EnoughSpace(10))
                    return false;

                if (pc.done)
                    continue;

                table = new List<TableItem>();
                table.Add(new TableItem(pc.sn.ToString("000"), 2, wPerc, Color.Black, StringAlignment.Near));
                table.Add(new TableItem(pc.alpha.ToString("0.000"), -1, wPerc));              
                table.Add(new TableItem(pc.time.ToString("yyyy/MM/dd HH:mm:ss"), -1, wPerc));

                m_yPos += DrawTableLine(gr, table, new Point(HeadRect.Left, m_yPos), HeadRect.Width, TextFont);                
                m_yPos += SmalMarg;
                pc.done = true;
            }

            gr.DrawLine(new Pen(Color.Black, LineWidth), HeadRect.Left, m_yPos, HeadRect.Right, m_yPos);
            m_yPos += MargY;

            return true;
        }

        private void zoomComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string zoomStr = zoomComboBox.Items[zoomComboBox.SelectedIndex].ToString().TrimEnd('%');
                double zoom = double.Parse(zoomStr);
                zoom /= 100.0f;
                
                RegAccess.Inst.SetValue("Report\\Zoom", zoom.ToString("0.0", CultureInfo.InvariantCulture));

                printPreviewControl.Zoom = zoom;
                printPreviewControl.Update();
            }
            catch (Exception ex)
            {

            }
        }

        private void ReportForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!MultiMode && PrintType == ReportType.RT_Measurement)
            {
                if (!Close)
                {
                    DialogResult res = MessageBox.Show("Keep measurement? (press No to discard)", "Warning", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
                    if (res == DialogResult.Cancel)
                        e.Cancel = true;
                    else
                        DialogResult = (res == DialogResult.Yes) ? DialogResult.OK : DialogResult.Cancel;
                }
            }

        }

        private void keepButton_Click(object sender, EventArgs e)
        {
            Close = true;
            DialogResult = DialogResult.OK;
        }

        private void discardButton_Click(object sender, EventArgs e)
        {
            Close = true;
            DialogResult = DialogResult.Cancel;
        }
    }
}
 