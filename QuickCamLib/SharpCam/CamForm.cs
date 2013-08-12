using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using QuickCamLib;

namespace SharpCam
{
    public partial class CamForm : Form
    {
        Size mStartSize;

        CaptureDevice[] mDevices;
        CaptureResolution[] mDeviceResolutions;
        CaptureDevice mSelectedDevice;
        CaptureResolution mSelectedResolution;
        QuickCam mCam;

        public CamForm()
        {
            InitializeComponent();
            mStartSize = this.Size;
        }

        private void CamForm_Load(object sender, EventArgs e)
        {
            mDevices = QuickCam.GetAvailableDevices();

            cbDevices.Items.AddRange(mDevices);
        }

        private void cbDevices_SelectedIndexChanged(object sender, EventArgs e)
        {
            mSelectedDevice = mDevices[cbDevices.SelectedIndex];
            mDeviceResolutions = QuickCam.GetAvailableResolutions(mSelectedDevice);

            cbResolutions.Items.Clear();
            cbResolutions.Items.AddRange(mDeviceResolutions);
        }

        private void cbResolutions_SelectedIndexChanged(object sender, EventArgs e)
        {
            mSelectedResolution = mDeviceResolutions[cbResolutions.SelectedIndex];

            if (mCam != null)
                mCam.Dispose();
            
            mCam = new QuickCam(mSelectedDevice, mSelectedResolution);
            mCam.ResetPTZ();
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            CaptureImage();
        }

        private void cbPreview_CheckedChanged(object sender, EventArgs e)
        {
            tmrCaptureTick.Enabled = cbPreview.Checked;
        }

        private void tmrCaptureTick_Tick(object sender, EventArgs e)
        {
            CaptureImage();
        }

        private void CaptureImage()
        {
            if (mCam != null)
                pbCaptured.Image = mCam.Capture();
        }

        private void CamForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (mCam != null)
                mCam.Dispose();
        }

        private void tbZoom_ValueChanged(object sender, EventArgs e)
        {
            if (mCam != null)
                mCam.Zoom(tbZoom.Value);
        }
    }
}
