// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

#include "amBXBase.h"

using namespace System;

namespace amBXLib {

	public ref class amBXRumble : amBXBase
	{
	private:
		IamBX_Rumble* _hIamBX_Rumble;

	internal:
		amBXRumble(IamBX* engineHandle, CompassDirection direction, RelativeHeight height);

	public:
		~amBXRumble();

		property bool Enabled {
			bool get();
			void set(bool value);
		}

		property CompassDirection Location {
			CompassDirection get();
		}

		property amBXRumbleSetting RumbleSetting {
			amBXRumbleSetting get();
			void set(amBXRumbleSetting value);
		}
	};
}