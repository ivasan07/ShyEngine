#include "Image.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"


Image::Image(std::string& path)
{
	posX = 0;
	posY = 0;

	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	width = surface->w;
	height = surface->h;
}

void Image::render()
{
	ImGui::Image(text, ImVec2(width, height));
}

SDL_Texture* Image::getTexture()
{
	return text;
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

void Image::setPosition(ImVec2 newPos)
{
	posX = newPos.x;
	posY = newPos.y;
}

ImVec2 Image::getPosition()
{
	return ImVec2(posX,posY);
}


