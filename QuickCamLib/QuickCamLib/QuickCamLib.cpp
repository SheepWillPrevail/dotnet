// Copyright (c) 2008, Robert Meijer <robert@grazz.com>
// All rights reserved. The QuickCam name referred to in this software is a 
// registered trademark and property of Logitech.

// Please read and understand the License.txt file before using this software.

// This is the main DLL file.

#include "stdafx.h"

#include "QuickCamLib.h"

using namespace System::Collections::Generic;
using namespace System::Drawing;

QuickCamLib::QuickCam::QuickCam(CaptureDevice^ device) {
	ConnectDevice(device);
}

QuickCamLib::QuickCam::QuickCam(CaptureDevice^ device, CaptureResolution^ resolution) {
	ConnectDevice(device);
	SetupDevice(resolution);
	pGraphControl->Run();
}

void QuickCamLib::QuickCam::ConnectDevice(CaptureDevice^ device) {
	uDeviceId = device->uNr;

	ICreateDevEnum* pSysDevEnum = NULL;
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);

	IEnumMoniker* pEnumCat = NULL;
	HRESULT hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	IMoniker* pMoniker = NULL;
	ULONG cFetched;
	pEnumCat->Skip(device->uNr - 1);
	pEnumCat->Next(1, &pMoniker, &cFetched);

	IBaseFilter* pFilter;
	hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pFilter);
	pDeviceFilter = pFilter;

	IAMCameraControl* pControl;
	hr = pFilter->QueryInterface(IID_IAMCameraControl, (void **)&pControl);
	pDeviceControl = pControl;

	pMoniker->Release();
	pEnumCat->Release();
	pSysDevEnum->Release();

	// set up the graphbuilder for the capture graphbuilder
	IGraphBuilder* pTempGraph;
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pTempGraph);
	pGraph = pTempGraph;
	
	IMediaControl* pTempGraphControl;
	pGraph->QueryInterface(IID_IMediaControl, (void **)&pTempGraphControl);
	pGraphControl = pTempGraphControl;

	// set up the capture graphbuilder
	ICaptureGraphBuilder2* pTempCaptureGraph;
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&pTempCaptureGraph);
	pCaptureGraph = pTempCaptureGraph;

	IAMStreamConfig* pTempCaptureConfig;
	pCaptureGraph->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pDeviceFilter, IID_IAMStreamConfig, (void **)&pTempCaptureConfig);
	pCaptureConfig = pTempCaptureConfig;

	// intialize the sample grabber and set it to buffer samples (else GetCurrentBuffer fails)
	IBaseFilter* pTempGrabberFilter;
	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pTempGrabberFilter);
	pGrabberFilter = pTempGrabberFilter;

	ISampleGrabber* pTempGrabber;
	pGrabberFilter->QueryInterface(IID_ISampleGrabber, (void **)&pTempGrabber);
	pGrabber = pTempGrabber;
	pGrabber->SetBufferSamples(TRUE);

	// insert the (device) filter into the graph
	pGraph->AddFilter(pDeviceFilter, L"Capture Source");
	pGraph->AddFilter(pGrabberFilter, L"Sample Grabber");
	pCaptureGraph->SetFiltergraph(pGraph);
}

array<QuickCamLib::CaptureDevice^>^ QuickCamLib::QuickCam::GetAvailableDevices() {
	List<CaptureDevice^>^ lAvailableDevices = gcnew List<CaptureDevice^>();

	ICreateDevEnum* pSysDevEnum = NULL;
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);

	IEnumMoniker* pEnumCat = NULL;
	HRESULT hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	if(hr == S_OK) 
	{
		IMoniker* pMoniker = NULL;
		ULONG cFetched;
		ULONG cIndex = 0;
		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			cIndex += 1;
			if(SUCCEEDED(hr))
			{
				IPropertyBag* pProperties;
				pMoniker->QueryInterface(IID_IPropertyBag, (void **)&pProperties);
			
				VARIANT vName;	
				VariantInit(&vName);
				pProperties->Read(L"FriendlyName", &vName, NULL);

				lAvailableDevices->Add(gcnew CaptureDevice(gcnew String(vName.bstrVal), cIndex));

				VariantClear(&vName);
				pProperties->Release();
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	pSysDevEnum->Release();

	return lAvailableDevices->ToArray();
}

array<QuickCamLib::CaptureResolution^>^ QuickCamLib::QuickCam::InternalGetAvailableResolutions() {
	List<CaptureResolution^>^ lAvailableResolutions = gcnew List<CaptureResolution^>();

	int iCount, iSize;
	pCaptureConfig->GetNumberOfCapabilities(&iCount, &iSize);
	
	for (int i=0; i < iCount; i++) {
		AM_MEDIA_TYPE* sCapType;
		VIDEO_STREAM_CONFIG_CAPS sCapCaps;
		pCaptureConfig->GetStreamCaps(i, &sCapType, (BYTE*)&sCapCaps);
		if (sCapType->subtype == MEDIASUBTYPE_RGB24)
			lAvailableResolutions->Add(gcnew CaptureResolution(uDeviceId, i, sCapType));
	}

	return lAvailableResolutions->ToArray();
}

array<QuickCamLib::CaptureResolution^>^ QuickCamLib::QuickCam::GetAvailableResolutions(CaptureDevice^ device) {
	array<CaptureResolution^>^ aAvailableResolutions;
	QuickCam^ oCam = gcnew QuickCam(device);
	aAvailableResolutions = oCam->InternalGetAvailableResolutions();
	delete oCam;
	return aAvailableResolutions;
}

void QuickCamLib::QuickCam::SetupDevice(CaptureResolution^ resolution) {	
	if (resolution->uDeviceId != uDeviceId)
		throw gcnew SystemException(L"The resolution specified does not belong to this device");
	
	mResolution = resolution;

	// set the resolution and RGB format
	AM_MEDIA_TYPE* sCaptureType;
	VIDEO_STREAM_CONFIG_CAPS sCaptureCaps;
	pCaptureConfig->GetStreamCaps(resolution->iNr, &sCaptureType, (BYTE*)&sCaptureCaps);
	sCaptureType->subtype = MEDIASUBTYPE_RGB24;
	pCaptureConfig->SetFormat(sCaptureType);	

	// let the capture graph figure it out
	pCaptureGraph->RenderStream(NULL, &MEDIATYPE_Video, pDeviceFilter, NULL, pGrabberFilter);
}

void QuickCamLib::QuickCam::Pan(long angle) {
	long flags = KSPROPERTY_CAMERACONTROL_FLAGS_RELATIVE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;	
	pDeviceControl->Set(CameraControl_Pan, angle, flags);
	System::Threading::Thread::Sleep(abs(angle * 10));
}

void QuickCamLib::QuickCam::Tilt(long angle) {
	long flags = KSPROPERTY_CAMERACONTROL_FLAGS_RELATIVE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;	
	pDeviceControl->Set(CameraControl_Tilt, angle, flags);
	System::Threading::Thread::Sleep(abs(angle * 10));
}

void QuickCamLib::QuickCam::Zoom(long percentage) {
	// map 0-100 to 50-200
	if (percentage < 0)
		percentage = 0;
	if (percentage > 100)
		percentage = 100;
	long value = long((percentage * 1.5) + 50);

	long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
	pDeviceControl->Set(CameraControl_Zoom, value, flags);
}

void QuickCamLib::QuickCam::Focus(long value) {
	if (value < 0)
		value = 0;
	if (value > 255)
		value = 255;

	long flags = KSPROPERTY_CAMERACONTROL_FLAGS_ABSOLUTE | KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
	pDeviceControl->Set(CameraControl_Focus, value, flags);
}

void QuickCamLib::QuickCam::ResetPTZ() {
	Pan(180);
	Tilt(180);
	Pan(-64);
	Tilt(-24);
	Zoom(0);
}

Bitmap^ QuickCamLib::QuickCam::Capture() {
	// grab the sample
	long lTempBufferSize = mResolution->lBufferSize;
	long* pTempBuffer = (long*)malloc(mResolution->lBufferSize);
	pGrabber->GetCurrentBuffer(&lTempBufferSize, pTempBuffer);

	// convert it into a System.Drawing.Bitmap (rotate to fix reverse order of the DIB format)
	Bitmap^ hBitmap = gcnew Bitmap(mResolution->lWidth, mResolution->lHeight, mResolution->lWidth * 3, 
		System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(pTempBuffer));
	hBitmap->RotateFlip(RotateFlipType::Rotate180FlipX); // only after this we can free the buffer
	free(pTempBuffer);

	return hBitmap;
}

QuickCamLib::QuickCam::~QuickCam() {
	pGraphControl->Stop();
	pGrabber->Release();
	pGrabberFilter->Release();	
	pCaptureConfig->Release();
	pCaptureGraph->Release();	
	pGraphControl->Release();
	pGraph->Release();
	pDeviceControl->Release();
	pDeviceFilter->Release();
}
