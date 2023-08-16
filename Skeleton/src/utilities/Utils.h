#pragma once

#include "Color.h"

namespace Utilities {

	class Utils {

	public:

		// Returns a random integer between min and max.
		static int RandomIntBetween(int min, int max);

		// Returns a random float between min and max.
		static float RandomBetween(float min, float max);

		// Returns a random angle between two given values (in degrees).
		static float RandomAngleBetween(float min, float max);

		// Converts an angle from degrees to radians.
		static float DegreesToRadians(float angle);

		// Converts an angle from radians to degrees.
		static float RadiansToDegrees(float angle);

		// Generates a random color.
		static Color RandomColor();

		// Generates a random color among the given ones.
		static Color RandomColorBetween(cColor c1, cColor c2);

	};

}

