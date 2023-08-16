#include "Vector2D.h"
#include <cassert>
#include <string>
#include "StringTrim.h"


#define PI 3.14159265358979323846264338327950288f

namespace Utilities {

	// Printing a Vector2D
	std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
		os << "(" << v.getX() << "," << v.getY() << ")";
		return os;
	}

	Vector2D Vector2D::rotate(float degrees) const {
		Vector2D r;

		degrees = fmod(degrees, 360.0f);
		if (degrees > 180.0f) {
			degrees = degrees - 360.0f;
		}
		else if (degrees <= -180.0f) {
			degrees = 360.0f + degrees;
		}

		float angle = degrees * PI / 180.0f;
		float sine = sin(angle);
		float cosine = cos(angle);

		//rotation matrix
		float matrix[2][2] = { { cosine, -sine }, { sine, cosine } };

		float x = x_;
		float y = y_;

		r.x_ = matrix[0][0] * x + matrix[0][1] * y;
		r.y_ = matrix[1][0] * x + matrix[1][1] * y;

		return r;
	}

	float Vector2D::angle(const Vector2D& v) const {
		float a2 = atan2(v.getX(), v.getY());
		float a1 = atan2(x_, y_);
		float angle = a1 - a2;
		float K = a1 > a2 ? -2.0f * PI : 2.0f * PI;
		angle = (abs(K + angle) < abs(angle)) ? K + angle : angle;
		return angle * 180.0f / PI;
	}

	Vector2D::Vector2D(const char* str) : Vector2D(std::string(str)) {}

	Vector2D::Vector2D(std::string const& str) {

		std::string trimed = Utilities::trim(str, "([ )]");

		size_t coma = trimed.find(",");

		assert(coma != std::string::npos);

		std::string x = trimed.substr(0, coma);
		std::string y = trimed.substr(coma + 1);

		x_ = std::stof(x);
		y_ = std::stof(y);
	}
}
