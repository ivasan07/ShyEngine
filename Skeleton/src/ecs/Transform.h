#pragma once

#include "Component.h"
#include "Vector2D.h"

namespace ECS {

	class Transform : public Component {
	public:

		Transform();

		Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation);

		// Getters
		Utilities::Vector2D getPosition();
		Utilities::Vector2D getScale();
		float getRotation();

		// Setters
		void setPosition(const Utilities::Vector2D& position);
		void setScale(const Utilities::Vector2D& scale);
		void setRotation(float rotation);

		// Modifiers
		void translate(const Utilities::Vector2D& position);
		void rotate(float rotation);
		void scale(float scale);

	private:

		Utilities::Vector2D position_;
		Utilities::Vector2D scale_;
		float rotation_;
	};

}