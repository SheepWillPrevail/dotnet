// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

#include "Structures.h"

using namespace System;

namespace amBXLib {

	public ref class amBXBase abstract
	{
	private:
		IamBX* _hIamBX;

	internal:
		amBXBase(IamBX* engine) {
			_hIamBX = engine;
		};

		property IamBX* EngineHandle {
			IamBX* get() {
				return _hIamBX;
			}
		}

		static CompassDirection amBX_loc2CompassDirection(amBX_loc location)  {
			CompassDirection returnValue = CompassDirection::Everywhere;

			if (location & amBX_N)
				returnValue = returnValue | CompassDirection::North;
			if (location & amBX_NE)
				returnValue = returnValue | CompassDirection::NorthEast;
			if (location & amBX_E)
				returnValue = returnValue | CompassDirection::East;
			if (location & amBX_SE)
				returnValue = returnValue | CompassDirection::SouthEast;
			if (location & amBX_S)
				returnValue = returnValue | CompassDirection::South;
			if (location & amBX_SW)
				returnValue = returnValue | CompassDirection::SouthWest;
			if (location & amBX_W)
				returnValue = returnValue | CompassDirection::West;
			if (location & amBX_NW)
				returnValue = returnValue | CompassDirection::NorthWest;
			if (location & amBX_C)
				returnValue = returnValue | CompassDirection::Center;

			return returnValue;
		}
	};
}
