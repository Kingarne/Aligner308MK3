using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReporterLib
{
    public class Logger
    {
        static Logger sm_log = new Logger();
        public static Logger Inst { get { return sm_log; } set { sm_log = value; } }

        public Logger()
        {           
            ExceptionLevel = LogLevel.Level1; // acccept messages at this level or below.
            TraceLevel = LogLevel.Level1; // accept messages at this level or below.
            LogTime = false;            
            ToFile = true;           
        }

        
        public enum LogLevel { None, Level1, Level2, Level3 }
        [Flags] public enum LogMode { ModeException, ModeTrace}
        
        public LogLevel ExceptionLevel { get; set; }
        public LogLevel TraceLevel { get; set; }
        public bool LogTime { get; set; }
        public bool ToFile{ get; set; }
      
        public void LogAndThrow(Exception e, LogLevel lev = LogLevel.Level1)
        {
            Exception(e.Message, lev);

            throw e;         
        }

        public void Time(string msg)
        {
            if (!LogTime)
                return;

            Append(msg, "(Time) ");
        }

        public void Exception(string msg, LogLevel lev = LogLevel.Level1)
        {
            if(lev > ExceptionLevel)
                return;

            Append(msg, "(E" +lev.ToString("D") +") ");           
        }

        public void Trace(string msg, LogLevel lev = LogLevel.Level1)
        {
            if (lev > TraceLevel)
                return;

            Append(msg, "(T" + lev.ToString("D") + ") ");
        }

        private void Append(string msg, string type)
        {
            if (ToFile)
            {
                string path = "c:\\temp\\a308log.txt";
                try
                {
                    StreamWriter sw = File.AppendText(path);

                    string cleanMsg = msg.Replace("\r\n", " ").Replace("\n", " "); ;

                    string text = DateTime.Now.ToString("(yy/MM/dd - HH:mm:ss:fff) ") + type + cleanMsg;

                    sw.WriteLine(text);
                    sw.Close();
                }
                catch (Exception e)
                {

                }
            }
        }

    }
}
