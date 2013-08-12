// Copyright (c) 2009, Robert Meijer <robert@grazz.com>

// Please read and understand the License.txt file before using this software.

#pragma once

using namespace System;

namespace amBXLib {

	public enum struct RumbleType {
		Boing,
		Crash,
		Engine,
		Explosion,
		Hit,
		Quake,
		Rattle,
		Road,
		Shot,
		Thud,
		Thunder
	};

	public value class amBXRumbleSetting {
	public:
		RumbleType Type;
		float Speed;
		float Intensity;
	};

	public value class amBXColor {
	public:
		float Red;
		float Green;
		float Blue;
	};

	[Flags]
	public enum struct CompassDirection : UInt32 {
		Everywhere = amBX_EVERYWHERE,		
		North = amBX_N,
		NorthEast = amBX_NE,
		East = amBX_E,
		SouthEast = amBX_SE,
		South = amBX_S,
		SouthWest = amBX_SW,
		West = amBX_W,
		NorthWest = amBX_NW,
		Center = amBX_C
	};

	public enum struct RelativeHeight : UInt32 {
		AnyHeight = amBX_ANYHEIGHT,
		EveryHeight = amBX_EVERYHEIGHT,
		Top = amBX_TOP,
		Middle = amBX_MIDDLE,
		Bottom = amBX_BOTTOM
	};
}