#pragma once
#include <iostream>
#include <list>

union SDL_Event;

struct ImVec2;
struct SDL_Texture;
struct SDL_Renderer;
class ImGUIManager;
class Transform;
class Component;
class Camera;

namespace PEditor {
	class GameObject
	{
		std::string name;
		SDL_Texture* text;
		SDL_Texture* gizmoText;
		std::list<Component*> components;

		ImGUIManager* imGuiManager;

		bool visible;

		bool leftMouseButtonDown;
		bool rightMouseButtonDown;

		bool showGizmo;

		bool waitingToDelete;

		ImVec2* size;
		ImVec2* pos;

		float previousMousePosX;
		float previousMousePosY;
		float rotation;

	public:

		GameObject(std::string& path);
		~GameObject();

		virtual void render();

		SDL_Texture* getTexture();
		std::string getName();

		bool isVisible();
		void setVisible(bool visible);

		int getWidth();
		int getHeight();

		void drawTransformInEditor();

		void render(SDL_Renderer* renderer, Camera* camera);
		void handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		std::list<Component*>* getComponents();

		void setPosition(ImVec2 newPos);
		void setName(const std::string newName);
		ImVec2 getPosition();
		float getRotation();

		bool isWaitingToDelete();
		void toDelete();
	
		std::string toJson();
	};
}