// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#include "stdafx.h"

#include "amBXLib.h"

using namespace amBXLib;
using namespace System::Runtime::InteropServices;

amBX::amBX(UInt32 majorVersion, UInt32 minorVersion, String^ applicationName, String^ applicationVersion) {
	IamBX* pEngineHandle;

	IntPtr cAppName = Marshal::StringToHGlobalAnsi(applicationName);
	IntPtr cAppVersion = Marshal::StringToHGlobalAnsi(applicationVersion);

	if (amBXCreateInterface(
			&pEngineHandle, 
			majorVersion, minorVersion, 
			(amBX_char*)cAppName.ToPointer(), (amBX_char*)cAppName.ToPointer(),
			nullptr,
			true)
		!= amBX_OK)
		throw gcnew Exception("Could not create interface to amBX Engine");

	Marshal::FreeHGlobal(cAppVersion);
	Marshal::FreeHGlobal(cAppName);

	_hIamBX = pEngineHandle;

	_bIsRunning = true;

	_tUpdateThread = gcnew Thread(gcnew ThreadStart(this, &amBX::UpdateThreadRun));
	_tUpdateThread->Start();
};

void amBX::UpdateThreadRun() {
	while (_bIsRunning) {
		try {
			_hIamBX->update(_hIamBX, 0);
			Thread::Sleep(1000 / 30);
		}
		catch (ThreadAbortException^) { }
	}
};

amBX::~amBX() {	
	//_tUpdateThread->Abort();
	_bIsRunning = false;
	_tUpdateThread->Join();

	_hIamBX->release(_hIamBX);
};

amBXFan^ amBX::CreateFan(CompassDirection direction, RelativeHeight height) {
	return gcnew amBXFan(_hIamBX, direction, height);
};

amBXLight^ amBX::CreateLight(CompassDirection direction, RelativeHeight height) {
	return gcnew amBXLight(_hIamBX, direction, height);
};

amBXRumble^ amBX::CreateRumble(CompassDirection direction, RelativeHeight height) {
	return gcnew amBXRumble(_hIamBX, direction, height);
};

