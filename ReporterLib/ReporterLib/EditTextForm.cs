using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace ReporterLib
{
    public partial class EditTextForm : Form
    {
        private int Id;
        private DBInterface DBI;
        private List<DBInterface.ImageInfo> Images;

        public EditTextForm(DBInterface dbi, int id)
        {
            InitializeComponent();
            Id = id;
            DBI = dbi;

            Images = new List<DBInterface.ImageInfo>();
            DBI.GetImages(id, ref Images);
        }

        private void EditTextForm_Load(object sender, EventArgs e)
        {
            textBox.Select(0, 0);

            imgListView.Columns.Add("Image");            
            imgListView.Columns[0].Width = -2;
           
            UpdateImgList();
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            foreach( ListViewItem item in imgListView.Items)
            {
                DBInterface.ImageInfo img = (DBInterface.ImageInfo)item.Tag;
                img.Include = item.Checked;
                DBI.UpdateImage(img);
            }

            DialogResult = DialogResult.OK;
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }           

        private void UpdateImgList()
        {
          //  imgListView.Clear();
            if (Images.Count == 0)
                return;

            //int i = 0;
            foreach (var img in Images)
            {
                ListViewItem item = imgListView.Items.Add(Path.GetFileName(img.Path));
                //imgListView.Items[i].SubItems.Add(meas.Value.Time.ToString("yy/MM/dd HH:mm:ss"));
                item.Tag = img;
                item.Checked = img.Include;
                //i++;
            }

            imgListView.Items[0].Selected = true;

        }

        private void imgListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (imgListView.SelectedItems.Count > 0)
            {
                int item = imgListView.SelectedItems[0].Index;
                try
                {
                    DBInterface.ImageInfo img = (DBInterface.ImageInfo)imgListView.Items[item].Tag;              
                    Image bm = Bitmap.FromFile(img.Path);

                    imageBox.Image = bm;
                }
                catch(Exception ex)
                {

                }
            }
        }
    }
}
