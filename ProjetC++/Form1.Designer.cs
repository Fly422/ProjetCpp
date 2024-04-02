namespace ProjetC__
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            PB_InitialImage = new PictureBox();
            PB_TreatedImage = new PictureBox();
            PB_TreatedImage1 = new PictureBox();
            PB_TreatedImage2 = new PictureBox();
            PB_TreatedImage3 = new PictureBox();
            PB_GroundTruth = new PictureBox();
            Lb_InitialImage = new Label();
            Lb_GroundTruth = new Label();
            Lb_TreatedImage = new Label();
            Lb_OPTIONS = new Label();
            Cb_ImageType = new ComboBox();
            LCb_NbImages = new CheckedListBox();
            ((System.ComponentModel.ISupportInitialize)PB_InitialImage).BeginInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage).BeginInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage2).BeginInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage3).BeginInit();
            ((System.ComponentModel.ISupportInitialize)PB_GroundTruth).BeginInit();
            SuspendLayout();
            // 
            // PB_InitialImage
            // 
            PB_InitialImage.Location = new Point(15, 15);
            PB_InitialImage.Margin = new Padding(4);
            PB_InitialImage.Name = "PB_InitialImage";
            PB_InitialImage.Size = new Size(465, 417);
            PB_InitialImage.TabIndex = 0;
            PB_InitialImage.TabStop = false;
            // 
            // PB_TreatedImage
            // 
            PB_TreatedImage.Location = new Point(488, 15);
            PB_TreatedImage.Margin = new Padding(4);
            PB_TreatedImage.Name = "PB_TreatedImage";
            PB_TreatedImage.Size = new Size(465, 417);
            PB_TreatedImage.TabIndex = 1;
            PB_TreatedImage.TabStop = false;
            // 
            // PB_TreatedImage1
            // 
            PB_TreatedImage1.Location = new Point(960, 15);
            PB_TreatedImage1.Name = "PB_TreatedImage1";
            PB_TreatedImage1.Size = new Size(249, 135);
            PB_TreatedImage1.TabIndex = 2;
            PB_TreatedImage1.TabStop = false;
            // 
            // PB_TreatedImage2
            // 
            PB_TreatedImage2.Location = new Point(960, 156);
            PB_TreatedImage2.Name = "PB_TreatedImage2";
            PB_TreatedImage2.Size = new Size(249, 135);
            PB_TreatedImage2.TabIndex = 3;
            PB_TreatedImage2.TabStop = false;
            // 
            // PB_TreatedImage3
            // 
            PB_TreatedImage3.Location = new Point(960, 297);
            PB_TreatedImage3.Name = "PB_TreatedImage3";
            PB_TreatedImage3.Size = new Size(249, 135);
            PB_TreatedImage3.TabIndex = 4;
            PB_TreatedImage3.TabStop = false;
            // 
            // PB_GroundTruth
            // 
            PB_GroundTruth.Location = new Point(1273, 15);
            PB_GroundTruth.Margin = new Padding(4);
            PB_GroundTruth.Name = "PB_GroundTruth";
            PB_GroundTruth.Size = new Size(465, 417);
            PB_GroundTruth.TabIndex = 5;
            PB_GroundTruth.TabStop = false;
            // 
            // Lb_InitialImage
            // 
            Lb_InitialImage.AutoSize = true;
            Lb_InitialImage.Location = new Point(202, 436);
            Lb_InitialImage.Name = "Lb_InitialImage";
            Lb_InitialImage.Size = new Size(109, 25);
            Lb_InitialImage.TabIndex = 6;
            Lb_InitialImage.Text = "Initial Image";
            // 
            // Lb_GroundTruth
            // 
            Lb_GroundTruth.AutoSize = true;
            Lb_GroundTruth.Location = new Point(1482, 436);
            Lb_GroundTruth.Name = "Lb_GroundTruth";
            Lb_GroundTruth.Size = new Size(116, 25);
            Lb_GroundTruth.TabIndex = 7;
            Lb_GroundTruth.Text = "Ground Truth";
            // 
            // Lb_TreatedImage
            // 
            Lb_TreatedImage.AutoSize = true;
            Lb_TreatedImage.Location = new Point(686, 436);
            Lb_TreatedImage.Name = "Lb_TreatedImage";
            Lb_TreatedImage.Size = new Size(124, 25);
            Lb_TreatedImage.TabIndex = 8;
            Lb_TreatedImage.Text = "Treated Image";
            // 
            // Lb_OPTIONS
            // 
            Lb_OPTIONS.AutoSize = true;
            Lb_OPTIONS.Location = new Point(224, 507);
            Lb_OPTIONS.Name = "Lb_OPTIONS";
            Lb_OPTIONS.Size = new Size(87, 25);
            Lb_OPTIONS.TabIndex = 9;
            Lb_OPTIONS.Text = "OPTIONS";
            // 
            // Cb_ImageType
            // 
            Cb_ImageType.FormattingEnabled = true;
            Cb_ImageType.Location = new Point(174, 535);
            Cb_ImageType.Name = "Cb_ImageType";
            Cb_ImageType.Size = new Size(194, 33);
            Cb_ImageType.TabIndex = 10;
            // 
            // LCb_NbImages
            // 
            LCb_NbImages.FormattingEnabled = true;
            LCb_NbImages.Items.AddRange(new object[] { "Image Seule", "Totalité des images", "Intervalle" });
            LCb_NbImages.Location = new Point(174, 575);
            LCb_NbImages.Name = "LCb_NbImages";
            LCb_NbImages.Size = new Size(194, 88);
            LCb_NbImages.TabIndex = 11;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1751, 731);
            Controls.Add(LCb_NbImages);
            Controls.Add(Cb_ImageType);
            Controls.Add(Lb_OPTIONS);
            Controls.Add(Lb_TreatedImage);
            Controls.Add(Lb_GroundTruth);
            Controls.Add(Lb_InitialImage);
            Controls.Add(PB_GroundTruth);
            Controls.Add(PB_TreatedImage3);
            Controls.Add(PB_TreatedImage2);
            Controls.Add(PB_TreatedImage1);
            Controls.Add(PB_TreatedImage);
            Controls.Add(PB_InitialImage);
            Margin = new Padding(4);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ((System.ComponentModel.ISupportInitialize)PB_InitialImage).EndInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage).EndInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage1).EndInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage2).EndInit();
            ((System.ComponentModel.ISupportInitialize)PB_TreatedImage3).EndInit();
            ((System.ComponentModel.ISupportInitialize)PB_GroundTruth).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private PictureBox PB_InitialImage;
        private PictureBox PB_TreatedImage;
        private PictureBox PB_TreatedImage1;
        private PictureBox PB_TreatedImage2;
        private PictureBox PB_TreatedImage3;
        private PictureBox PB_GroundTruth;
        private Label Lb_InitialImage;
        private Label Lb_GroundTruth;
        private Label Lb_TreatedImage;
        private Label Lb_OPTIONS;
        private ComboBox Cb_ImageType;
        private CheckedListBox LCb_NbImages;
    }
}