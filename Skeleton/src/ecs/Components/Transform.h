#pragma once

#include "Component.h"
#include "Vector2D.h"

#include "Scripting/Variable.h"

#include <list>

namespace ECS {

	class Transform : public Component {
	public:

		Transform();
		Transform(const Utilities::Vector2D& position, const Utilities::Vector2D& scale, float rotation);

		void onDestroy() override;

		Utilities::Vector2D* getPositionPointer();
		Utilities::Vector2D* getScalePointer();

		//TODO: que esto no devuelva un puntero o hacer otro metodo para las fisicas
		const double* getRotationPointer();

		void SetParent(Transform* tr);

	private:
		void SetChildren(Transform* tr);
		void RemoveChildren(Transform* tr);

		//TODO: global setters

	publish:

		//	Local getters and setters
		Utilities::Vector2D GetLocalPosition();
		Utilities::Vector2D GetWorldPosition();

		void SetLocalPosition(Utilities::Vector2D position);
		void SetLocalPositionX(float x);
		void SetLocalPositionY(float y);

		void SetWorldPosition(Utilities::Vector2D position);

		Utilities::Vector2D GetLocalScale();
		Utilities::Vector2D GetWorldScale();

		void SetScale(Utilities::Vector2D scale);
		void SetScaleX(float x);
		void SetScaleY(float y);

		void SetWorldScale(Utilities::Vector2D scale);

		float GetLocalRotation();
		float GetWorldRotation();

		void SetLocalRotation(float rotation);
		void SetWorldRotation(float rotation);

		void Translate(Utilities::Vector2D direction);

		void TranslateX(float x);
		void TranslateY(float y);

		void Rotate(float rotation);
		void Scale(float scale);

	private:

		reflect Utilities::Vector2D localPosition;
		reflect Utilities::Vector2D localScale;
		reflect double localRotation;


		Transform* parent;
		std::list<Transform*> children;
	};

}