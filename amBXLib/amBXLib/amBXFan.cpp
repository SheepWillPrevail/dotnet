// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#include "stdafx.h"

#include "amBXFan.h"

using namespace amBXLib;

amBXFan::amBXFan(IamBX *engineHandle, amBXLib::CompassDirection direction, amBXLib::RelativeHeight height) : amBXBase(engineHandle) {
	IamBX_Fan* fanHandle;

	if (EngineHandle->createFan(EngineHandle, (amBX_u32)direction, (amBX_u32)height, &fanHandle)
		!= amBX_OK)
		throw gcnew Exception("Could not create fan");

	_hIamBX_Fan = fanHandle;
};

amBXFan::~amBXFan() {
	_hIamBX_Fan->release(_hIamBX_Fan);
};

bool amBXFan::Enabled::get() {
	amBX_state state;

	_hIamBX_Fan->getEnabled(_hIamBX_Fan, &state);

	return (state == ENABLED || state == ENABLING) && !(state == DISABLED || state == DISABLING);
}

void amBXFan::Enabled::set(bool value) {
	_hIamBX_Fan->setEnabled(_hIamBX_Fan, value);
}

float amBXFan::Intensity::get() {
	amBX_float intensity;

	_hIamBX_Fan->getIntensity(_hIamBX_Fan, &intensity);

	return intensity;
}

void amBXFan::Intensity::set(float value) {
	_hIamBX_Fan->setIntensity(_hIamBX_Fan, value);
}

CompassDirection amBXFan::Location::get() {
	amBX_loc location;

	_hIamBX_Fan->getLocation(_hIamBX_Fan, &location);

	return amBX_loc2CompassDirection(location);
}