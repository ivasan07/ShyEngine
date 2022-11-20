#include "Scene.h"
#include "SDL.h"
#include "Image.h"
#include "Window.h"
#include "imgui.h"
#include "Camera.h"
#include "ImGUIManager.h"

Scene::Scene(PEditor::Window* w)
{
	camera = new Camera(ImVec2(50, 50), 1);

	window = w;
	window->addComponent(this);

	renderer = ImGUIManager::getInstance()->getRenderer();

	// -15 and - 35 is to get the resolution without the offset (TODO: create variable so there are no arbitrary numbers)
	ImVec2 size = window->getSize();
	width = size.x - 15;
	height = size.y - 35;

	ImVec2 position = window->getPosition();
	posX = position.x;
	posY = position.y;

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

}

void Scene::addImage(std::string path)
{
	images.push_back(new Image(path));
}

bool Scene::entityOutsideCamera(ImVec2 pos, float width, float height)
{
	/*if (pos.x > width || pos.y > height || pos.x + width <= 0 || pos.y + height <= 0) {
		return true;
	}

	return false;*/
	return true;
}

void Scene::handleInput(SDL_Event* event)
{
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	if ((mouseX > posX && mouseX < posX + width + 15 && mouseY > posY && mouseY < posY + height + 35) || event->type == SDL_MOUSEBUTTONUP)
		camera->handleInput(event);
}

void Scene::render()
{
	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);
	for (auto image : images) {

		ImVec2 position = ImVec2(image->getPosition().x + camera->getPosition().x , image->getPosition().y + camera->getPosition().y);
		float width = image->getWidth() * camera->getScrollFactor();
		float height = image->getHeight() * camera->getScrollFactor();

		//if (entityOutsideCamera(position, width, height)) continue;

		SDL_Rect dst = { position.x, position.y, width, height };
		SDL_RenderCopy(renderer, image->getTexture(), NULL, &dst);
	}

	SDL_SetRenderTarget(renderer, NULL);

	ImGui::Image(targetTexture, ImVec2(width, height));
}

