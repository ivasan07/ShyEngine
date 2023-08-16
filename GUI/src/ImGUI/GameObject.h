#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

union SDL_Event;

struct ImVec2;
struct SDL_Texture;
struct SDL_Renderer;
class ImGUIManager;
class Transform;
class Camera;

namespace Components {
	class Component;
	class Attribute;
	class Script;
}

namespace PEditor {
	class GameObject
	{
		static int lastId;

		std::string name;
		int id;

		std::string imagePath;
		Components::Component* imageComponent;
		SDL_Texture* text;
		SDL_Texture* gizmoText;
		std::unordered_map<std::string, ::Components::Component> components;

		std::unordered_map<std::string, Components::Script> scripts;

		ImGUIManager* imGuiManager;

		bool visible;

		bool leftMouseButtonDown;
		bool rightMouseButtonDown;

		bool showGizmo;

		bool prefab;

		bool waitingToDelete;

		ImVec2* size;
		ImVec2* pos;

		float previousMousePosX;
		float previousMousePosY;
		float rotation;

		int renderOrder;

		void drawFloat(std::string attrName, ::Components::Attribute* attr);
		void drawVector2(std::string attrName, ::Components::Attribute* attr);
		void drawString(std::string attrName, ::Components::Attribute* attr);
		void drawBool(std::string attrName, ::Components::Attribute* attr);
		void drawColor(std::string attrName, ::Components::Attribute* attr);
		void drawChar(std::string attrName, ::Components::Attribute* attr);
		void drawGameobject(std::string attrName, ::Components::Attribute* attr);

		void translateChildren(GameObject* go, ImVec2* previousPos);

		GameObject* parent;
		std::unordered_map<int, GameObject*> children;

	public:

		GameObject(std::string& path);
		~GameObject();

		SDL_Texture* getTexture();
		std::string getName();
		int getId();

		int getRenderOrder();

		bool isVisible();
		void setVisible(bool visible);

		int getWidth();
		int getHeight();

		void drawTransformInEditor();

		void render(SDL_Renderer* renderer, Camera* camera);
		void handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);
		void update();

		void addComponent(::Components::Component comp);
		void addScript(::Components::Script script);

		std::unordered_map<std::string, ::Components::Component>* getComponents();
		std::unordered_map<std::string, ::Components::Script>* getScripts();

		void setPosition(ImVec2 newPos);
		void setName(const std::string newName);
		ImVec2 getPosition();
		float getRotation();

		bool isWaitingToDelete();
		void toDelete();
	
		bool isPrefab();

		void setParent(GameObject* go);
		GameObject* getParent();
		void removeChild(GameObject* go);
		void addChild(GameObject* go);
		std::unordered_map<int, GameObject*> getChildren();
		bool isAscendant(GameObject* go);


		void drawComponentsInEditor();
		void drawScriptsInEditor();
		std::string toJson();
	};
}