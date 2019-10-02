using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReporterLib
{
    public partial class EditTextForm : Form
    {
        public EditTextForm()
        {
            InitializeComponent();
            
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void EditTextForm_Load(object sender, EventArgs e)
        {
            textBox.Select(0, 0);
        }
    }
}
