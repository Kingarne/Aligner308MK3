using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32;

namespace ReporterLib
{
    public class RegAccess
    {
        static RegAccess sm_reg = new RegAccess();
        public static RegAccess Inst { get { return sm_reg; } }

        public string UserRoot = "HKEY_CURRENT_USER";
        public string Subkey = "SOFTWARE\\Schill Reglerteknik AB\\Aligner 308 MK3\\";
        public string KeyBase { get { return UserRoot + "\\" + Subkey; } }


        public object GetValue(string key, object def)
        {
            int index = key.LastIndexOf("\\");
            string subKey = KeyBase;
            string tempKey = key;
            if (index > 0)
            {
                string add = tempKey.Substring(0, index);
                index += 1;
                tempKey = key.Substring(index, tempKey.Length - index);
                subKey += add;
            }
            //string keyName = UserRoot + "\\" + Subkey;        
            object val = Registry.GetValue(subKey, tempKey, def);
            if(val == null)
            {
                SetValue(key, def);
                return def;
            }
            return val;
        }

        public void SetValue(string key, object val)
        {
            int index = key.LastIndexOf("\\");
            string subKey = KeyBase;
            if (index > 0)
            {
                string add = key.Substring(0, index);
                index += 1;
                key = key.Substring(index, key.Length - index);
                subKey += add;
            }

            Registry.SetValue(subKey, key, val);

        }
    }
}
