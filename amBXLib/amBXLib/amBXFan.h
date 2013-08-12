// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

#include "amBXBase.h"

using namespace System;

namespace amBXLib {

	public ref class amBXFan : amBXBase
	{
	private:
		IamBX_Fan* _hIamBX_Fan;

	internal:
		amBXFan(IamBX* engineHandle, CompassDirection direction, RelativeHeight height);

	public:
		~amBXFan();

		property bool Enabled {
			bool get();
			void set(bool value);
		}

		property float Intensity {
			float get();
			void set(float value);
		}

		property CompassDirection Location {
			CompassDirection get();
		}
	};
}