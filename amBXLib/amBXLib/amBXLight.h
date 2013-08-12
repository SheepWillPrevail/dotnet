// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

#include "amBXBase.h"

using namespace System;

namespace amBXLib {

	public ref class amBXLight : amBXBase
	{
	private:
		IamBX_Light* _hIamBX_Light;

	internal:
		amBXLight(IamBX* engineHandle, CompassDirection direction, RelativeHeight height);

	public:
		~amBXLight();

		property amBXColor Color {
			amBXColor get();
			void set(amBXColor value);
		}

		property bool Enabled {
			bool get();
			void set(bool value);
		}

		property Int64 FadeTime {
			Int64 get();
			void set(Int64 value);
		}

		property CompassDirection Location {
			CompassDirection get();
		}
	};
}
