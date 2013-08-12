// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

#include "amBXFan.h"
#include "amBXLight.h"
#include "amBXRumble.h"

using namespace System;
using namespace System::Threading;

namespace amBXLib {

	public ref class amBX
	{
	private:
		IamBX* _hIamBX;
		Thread^ _tUpdateThread;
		void UpdateThreadRun();
		bool _bIsRunning;

	public:
		amBX(UInt32 majorVersion, UInt32 minorVersion, String^ applicationName, String^ applicationVersion);
		~amBX();

		amBXFan^ CreateFan(CompassDirection direction, RelativeHeight height);
		amBXLight^ CreateLight(CompassDirection direction, RelativeHeight height);
		amBXRumble^ CreateRumble(CompassDirection direction, RelativeHeight height);
	};

}
