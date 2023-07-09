#pragma once
#include <iostream>
#include <unordered_map>

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
		Transform* tr;
		std::unordered_map<int ,Component*> components;

		ImGUIManager* imGuiManager;

		bool visible;

		bool leftMouseButtonDown;

		bool showGizmo;

		bool waitingToDelete;

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

		void render(SDL_Renderer* renderer, Camera* camera);
		void handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		std::unordered_map<int, Component*>* getComponents();

		void setPosition(ImVec2 newPos);
		void setName(const std::string newName);
		ImVec2 getPosition();

		bool isWaitingToDelete();
		void toDelete();
	};
}