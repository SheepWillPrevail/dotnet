// Copyright (c) 2008, Robert Meijer <robert@grazz.com>
// All rights reserved. The QuickCam name referred to in this software is a 
// registered trademark and property of Logitech.

// Please read and understand the License.txt file before using this software.

// QuickCamLib.h

#pragma once

using namespace System;
using namespace System::Drawing;

namespace QuickCamLib {
	ref class QuickCam;
	
	public ref class CaptureDevice
	{
	internal:
		String^ sName;
		ULONG uNr;

		CaptureDevice(String^ name, ULONG nr) {
			sName = name;
			uNr = nr;
		}

	public:
		virtual String^ ToString() override {
			return sName;
		}
	};

	public ref class CaptureResolution
	{
	internal:
		ULONG uDeviceId;
		int iNr;
		long lWidth, lHeight, lBufferSize;

		CaptureResolution(ULONG deviceid, int nr, AM_MEDIA_TYPE* type) {
			uDeviceId = deviceid;
			iNr = nr;
			
			VIDEOINFOHEADER* info = (VIDEOINFOHEADER*)type->pbFormat;
			lWidth = info->bmiHeader.biWidth;
			lHeight = info->bmiHeader.biHeight;
			lBufferSize = type->lSampleSize;
		}

	public:
		property long Width {
			long get() {
				return lWidth;
			}
		}
		property long Height {
			long get() {
				return lHeight;
			}
		}
		virtual String^ ToString() override {
			return String::Format("{0}x{1}", lWidth, lHeight);
		}
	};

	public ref class QuickCam
	{
	private:
		CaptureResolution^ mResolution;
		ULONG uDeviceId;

		IBaseFilter* pDeviceFilter;
		IAMCameraControl* pDeviceControl;
		IGraphBuilder* pGraph;
		IMediaControl* pGraphControl;
		ICaptureGraphBuilder2* pCaptureGraph;
		IAMStreamConfig* pCaptureConfig;
		IBaseFilter* pGrabberFilter;
		ISampleGrabber* pGrabber;

		void ConnectDevice(CaptureDevice^ device);
		void SetupDevice(CaptureResolution^ resolution);
	internal:
		QuickCam(CaptureDevice^ device);
		array<CaptureResolution^>^ InternalGetAvailableResolutions();

	public:
		QuickCam(CaptureDevice^ device, CaptureResolution^ resolution);
		~QuickCam();
		
		void Pan(long angle);
		void Tilt(long angle);
		void Zoom(long percentage);
		void Focus(long focus);
		void ResetPTZ();
		Bitmap^ Capture();

		static array<CaptureDevice^>^ GetAvailableDevices();
		static array<CaptureResolution^>^ GetAvailableResolutions(CaptureDevice^ device);
	};
}
