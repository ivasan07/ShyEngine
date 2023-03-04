#pragma once
#include <vector>
#include "WindowComponent.h"
#include <iostream>

union SDL_Event;

struct SDL_Renderer;
struct SDL_Texture;
struct ImVec2;
class Camera;
class string;
class ImGUIManager;

namespace PEditor {
	class Window;
	class Image;
};

namespace PEditor {
	class Scene : public WindowComponent
	{
	private:

		PEditor::Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture* targetTexture = nullptr;

		Camera* camera;

		std::vector<PEditor::Image*> images;

		int width, height;
		int posX, posY;

	public:

		Scene();

		void addImage(std::string path);
		std::vector<PEditor::Image*> getGameObjects();

		bool entityOutsideCamera(ImVec2 pos, float width, float height);

		virtual void handleInput(SDL_Event* event);
		virtual void render();
	};

}