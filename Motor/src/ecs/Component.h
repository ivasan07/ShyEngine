#pragma once

#include "Scripting/Variable.h"
#include <string>

#include <EditorExport.h>

#define ECS_Version 1.0


namespace ECS {

	class Entity;
	class Scene;

	class Component {

		friend Entity;

	public:

		Component();

		virtual ~Component() {};

		Entity* getEntity();

		Scene* getScene();

		bool isActive();

		void setActive(bool ent_active);

		void remove();

		void remove(Component* c);

		bool isRemoved();

	private:

		virtual void init() {};

		virtual void start() {};

		virtual void update(float deltaTime) {};

		virtual void lateUpdate(float deltaTime) {};

		virtual void render() {};

		virtual void fixedUpdate(float fixedDeltaTime) {};

		virtual void onActive() {};

		virtual void onDeacitve() {};

		virtual void onSceneUp() {};

		virtual void onSceneDown() {};

		virtual void onCollisionEnter(Entity* b) {};

		virtual void onCollisionStay(Entity* b) {};

		virtual void onCollisionExit(Entity* b) {};

		virtual void onTriggerEnter(Entity* b) {};

		virtual void onTriggerStay(Entity* b) {};

		virtual void onTriggerExit(Entity* b) {};

		virtual void onDestroy() {};

		virtual void onClickBegin() {};

		virtual void onClickHold() {};

		virtual void onClick() {};

		virtual void onDoubleClick() {};

		virtual void onRightClick() {};

		virtual void onMouseEnter() {};
		
		virtual void onMouseHover() {};

		virtual void onMouseExit() {};

	protected:

		void print(std::string const& message, std::string const& className = "");
		void printError(std::string const& message, std::string const& className = "");
		void printWarning(std::string const& message, std::string const& className = "");

		Entity* entity;

		bool active;

		bool removed;

		bool inRemovedComponentList;

		bool valid;

	};
}

