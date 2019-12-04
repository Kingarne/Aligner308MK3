namespace ReporterLib
{
    partial class ReportForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.printButton = new System.Windows.Forms.Button();
            this.printPreviewControl = new System.Windows.Forms.PrintPreviewControl();
            this.pageUpDown = new System.Windows.Forms.NumericUpDown();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.reportList = new System.Windows.Forms.ListView();
            this.commentButton = new System.Windows.Forms.Button();
            this.allButton = new System.Windows.Forms.Button();
            this.noneButton = new System.Windows.Forms.Button();
            this.delButton = new System.Windows.Forms.Button();
            this.zoomComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.discardButton = new System.Windows.Forms.Button();
            this.keepButton = new System.Windows.Forms.Button();
            this.finishLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pageUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // printButton
            // 
            this.printButton.Location = new System.Drawing.Point(12, 6);
            this.printButton.Name = "printButton";
            this.printButton.Size = new System.Drawing.Size(75, 23);
            this.printButton.TabIndex = 1;
            this.printButton.Text = "Print...";
            this.printButton.UseVisualStyleBackColor = true;
            this.printButton.Click += new System.EventHandler(this.printButton_Click);
            // 
            // printPreviewControl
            // 
            this.printPreviewControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.printPreviewControl.AutoZoom = false;
            this.printPreviewControl.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.printPreviewControl.Location = new System.Drawing.Point(285, 45);
            this.printPreviewControl.Name = "printPreviewControl";
            this.printPreviewControl.Size = new System.Drawing.Size(457, 659);
            this.printPreviewControl.TabIndex = 2;
            this.printPreviewControl.UseAntiAlias = true;
            this.printPreviewControl.Click += new System.EventHandler(this.printPreviewControl_Click);
            // 
            // pageUpDown
            // 
            this.pageUpDown.Location = new System.Drawing.Point(174, 9);
            this.pageUpDown.Name = "pageUpDown";
            this.pageUpDown.Size = new System.Drawing.Size(50, 20);
            this.pageUpDown.TabIndex = 3;
            this.pageUpDown.ValueChanged += new System.EventHandler(this.pageUpDown_ValueChanged);
            // 
            // reportList
            // 
            this.reportList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.reportList.CheckBoxes = true;
            this.reportList.FullRowSelect = true;
            this.reportList.GridLines = true;
            this.reportList.HideSelection = false;
            this.reportList.Location = new System.Drawing.Point(12, 32);
            this.reportList.MultiSelect = false;
            this.reportList.Name = "reportList";
            this.reportList.Size = new System.Drawing.Size(267, 630);
            this.reportList.TabIndex = 4;
            this.reportList.UseCompatibleStateImageBehavior = false;
            this.reportList.View = System.Windows.Forms.View.Details;
            this.reportList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.reportList_ColumnClick);
            this.reportList.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.reportList_ItemChecked);
            this.reportList.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.reportList_ItemSelectionChanged);
            this.reportList.DoubleClick += new System.EventHandler(this.reportList_DoubleClick);
            // 
            // commentButton
            // 
            this.commentButton.Location = new System.Drawing.Point(93, 6);
            this.commentButton.Name = "commentButton";
            this.commentButton.Size = new System.Drawing.Size(75, 23);
            this.commentButton.TabIndex = 5;
            this.commentButton.Text = "Manage...";
            this.commentButton.UseVisualStyleBackColor = true;
            this.commentButton.Click += new System.EventHandler(this.commentButton_Click);
            // 
            // allButton
            // 
            this.allButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.allButton.Location = new System.Drawing.Point(12, 668);
            this.allButton.Name = "allButton";
            this.allButton.Size = new System.Drawing.Size(42, 19);
            this.allButton.TabIndex = 6;
            this.allButton.Text = "All";
            this.allButton.UseVisualStyleBackColor = true;
            this.allButton.Click += new System.EventHandler(this.allButton_Click);
            // 
            // noneButton
            // 
            this.noneButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.noneButton.Location = new System.Drawing.Point(60, 668);
            this.noneButton.Name = "noneButton";
            this.noneButton.Size = new System.Drawing.Size(42, 19);
            this.noneButton.TabIndex = 7;
            this.noneButton.Text = "None";
            this.noneButton.UseVisualStyleBackColor = true;
            this.noneButton.Click += new System.EventHandler(this.noneButton_Click);
            // 
            // delButton
            // 
            this.delButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.delButton.Location = new System.Drawing.Point(223, 668);
            this.delButton.Name = "delButton";
            this.delButton.Size = new System.Drawing.Size(56, 19);
            this.delButton.TabIndex = 8;
            this.delButton.Text = "Delete...";
            this.delButton.UseVisualStyleBackColor = true;
            this.delButton.Click += new System.EventHandler(this.delButton_Click);
            // 
            // zoomComboBox
            // 
            this.zoomComboBox.FormattingEnabled = true;
            this.zoomComboBox.Location = new System.Drawing.Point(320, 8);
            this.zoomComboBox.Name = "zoomComboBox";
            this.zoomComboBox.Size = new System.Drawing.Size(55, 21);
            this.zoomComboBox.TabIndex = 9;
            this.zoomComboBox.SelectedIndexChanged += new System.EventHandler(this.zoomComboBox_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(282, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Zoom";
            // 
            // discardButton
            // 
            this.discardButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.discardButton.Location = new System.Drawing.Point(582, 681);
            this.discardButton.Name = "discardButton";
            this.discardButton.Size = new System.Drawing.Size(75, 23);
            this.discardButton.TabIndex = 11;
            this.discardButton.Text = "Discard";
            this.discardButton.UseVisualStyleBackColor = true;
            this.discardButton.Click += new System.EventHandler(this.discardButton_Click);
            // 
            // keepButton
            // 
            this.keepButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.keepButton.Location = new System.Drawing.Point(501, 681);
            this.keepButton.Name = "keepButton";
            this.keepButton.Size = new System.Drawing.Size(75, 23);
            this.keepButton.TabIndex = 12;
            this.keepButton.Text = "Keep";
            this.keepButton.UseVisualStyleBackColor = true;
            this.keepButton.Click += new System.EventHandler(this.keepButton_Click);
            // 
            // finishLabel
            // 
            this.finishLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.finishLabel.AutoSize = true;
            this.finishLabel.Location = new System.Drawing.Point(391, 686);
            this.finishLabel.Name = "finishLabel";
            this.finishLabel.Size = new System.Drawing.Size(104, 13);
            this.finishLabel.TabIndex = 13;
            this.finishLabel.Text = "Finish Measurement:";
            // 
            // ReportForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(754, 716);
            this.Controls.Add(this.finishLabel);
            this.Controls.Add(this.keepButton);
            this.Controls.Add(this.discardButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.zoomComboBox);
            this.Controls.Add(this.delButton);
            this.Controls.Add(this.noneButton);
            this.Controls.Add(this.allButton);
            this.Controls.Add(this.commentButton);
            this.Controls.Add(this.reportList);
            this.Controls.Add(this.pageUpDown);
            this.Controls.Add(this.printPreviewControl);
            this.Controls.Add(this.printButton);
            this.MinimumSize = new System.Drawing.Size(770, 755);
            this.Name = "ReportForm";
            this.Text = "Measurement Report";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ReportForm_FormClosing);
            this.Load += new System.EventHandler(this.ReportForm_Load);
            this.SizeChanged += new System.EventHandler(this.ReportForm_SizeChanged);
            ((System.ComponentModel.ISupportInitialize)(this.pageUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button printButton;
        private System.Windows.Forms.PrintPreviewControl printPreviewControl;
        private System.Windows.Forms.NumericUpDown pageUpDown;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.ListView reportList;
        private System.Windows.Forms.Button commentButton;
        private System.Windows.Forms.Button allButton;
        private System.Windows.Forms.Button noneButton;
        private System.Windows.Forms.Button delButton;
        private System.Windows.Forms.ComboBox zoomComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button discardButton;
        private System.Windows.Forms.Button keepButton;
        private System.Windows.Forms.Label finishLabel;
    }
}