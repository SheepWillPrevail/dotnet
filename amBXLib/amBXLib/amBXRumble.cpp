// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#include "stdafx.h"

#include <string.h>
#include "amBXRumble.h"

using namespace amBXLib;
using namespace System::Runtime::InteropServices;

amBXRumble::amBXRumble(IamBX* engineHandle, CompassDirection direction, RelativeHeight height) : amBXBase(engineHandle) {
	IamBX_Rumble* rumbleHandle;

	if (EngineHandle->createRumble(EngineHandle, (amBX_u32)direction, (amBX_u32)height, &rumbleHandle)
		!= amBX_OK)
		throw gcnew Exception("Could not create rumble");

	_hIamBX_Rumble = rumbleHandle;
};

amBXRumble::~amBXRumble() {
	_hIamBX_Rumble->release(_hIamBX_Rumble);
};

bool amBXRumble::Enabled::get() {
	amBX_state state;

	_hIamBX_Rumble->getEnabled(_hIamBX_Rumble, &state);

	return (state == ENABLED || state == ENABLING) && !(state == DISABLED || state == DISABLING);
}

void amBXRumble::Enabled::set(bool value) {
	_hIamBX_Rumble->setEnabled(_hIamBX_Rumble, value);
}

CompassDirection amBXRumble::Location::get() {
	amBX_loc location;

	_hIamBX_Rumble->getLocation(_hIamBX_Rumble, &location);

	return amBX_loc2CompassDirection(location);
}

amBXRumbleSetting amBXRumble::RumbleSetting::get() {
	amBXRumbleSetting setting;

	amBX_char cName[32];
	amBX_u32 uNameLength = 0;
	amBX_float speed, intensity;

	memset(cName, 0, 32);

	_hIamBX_Rumble->getRumble(_hIamBX_Rumble, sizeof(cName), cName, &uNameLength, &speed, &intensity);

	setting.Intensity = intensity;
	setting.Speed = speed;
	//String^ name = Marshal::PtrToStringAnsi((IntPtr)&cName);

	return setting;
}

void amBXRumble::RumbleSetting::set(amBXRumbleSetting value) {	
	String^ rumbleName = String::Format("amBX_{0}", value.Type);
	IntPtr cRumbleName = Marshal::StringToHGlobalAnsi(rumbleName);

	_hIamBX_Rumble->setRumble(_hIamBX_Rumble, (amBX_char*)cRumbleName.ToPointer(), value.Speed, value.Intensity);

	Marshal::FreeHGlobal(cRumbleName);	
}