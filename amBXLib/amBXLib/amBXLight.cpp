// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#include "stdafx.h"

#include "amBXLight.h"

using namespace amBXLib;

amBXLight::amBXLight(IamBX* engineHandle, CompassDirection direction, RelativeHeight height) : amBXBase(engineHandle) {
	IamBX_Light* lightHandle;

	if (EngineHandle->createLight(EngineHandle, (amBX_u32)direction, (amBX_u32)height, &lightHandle)
		!= amBX_OK)
		throw gcnew Exception("Unable to create light");

	_hIamBX_Light = lightHandle;
};

amBXLight::~amBXLight() {
	_hIamBX_Light->release(_hIamBX_Light);
};

amBXColor amBXLight::Color::get() {
	float red, green, blue;

	_hIamBX_Light->getCol(_hIamBX_Light, &red, &green, &blue);

	amBXColor returnValue;
	returnValue.Red = red;
	returnValue.Green = green;
	returnValue.Blue = blue;

	return returnValue;
}

void amBXLight::Color::set(amBXColor value) {
	_hIamBX_Light->setCol(_hIamBX_Light, value.Red, value.Green, value.Blue);
}

bool amBXLight::Enabled::get() {
	amBX_state state;

	_hIamBX_Light->getEnabled(_hIamBX_Light, &state);

	return (state == ENABLED || state == ENABLING) && !(state == DISABLED || state == DISABLING);
}

void amBXLight::Enabled::set(bool value) {
	_hIamBX_Light->setEnabled(_hIamBX_Light, value);
}

Int64 amBXLight::FadeTime::get() {
	amBX_time time;	

	_hIamBX_Light->getFadeTime(_hIamBX_Light, &time);
	
	return time;
}

void amBXLight::FadeTime::set(Int64 value) {
	_hIamBX_Light->setFadeTime(_hIamBX_Light, value);
}

CompassDirection amBXLight::Location::get() {
	amBX_loc location;

	_hIamBX_Light->getLocation(_hIamBX_Light, &location);

	return amBX_loc2CompassDirection(location);
}