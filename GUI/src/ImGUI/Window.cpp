#include "Window.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "SDL.h"
#include "ImGUIManager.h"
#include "SDL_image.h"

PEditor::Window::Window(std::string _windowName, WindowFlags f = None)
{
	flags = f;
	windowName = _windowName;


	windowWidth = windowOriWidth = 50;
	windowHeight = windowOriHeight = 50;

	windowPosX = 0;
	windowPosY = 0;
}

void PEditor::Window::setSize(ImVec2 size)
{
	windowWidth = size.x;
	windowHeight = size.y;
}
void PEditor::Window::setPosition(ImVec2 position)
{
	windowPosX = position.x;
	windowPosY = position.y;
}


ImVec2 PEditor::Window::getSize()
{
	return ImVec2(windowWidth, windowHeight);
}

ImVec2 PEditor::Window::getPosition()
{
	return ImVec2(windowPosX, windowPosY);
}


void PEditor::Window::update()
{
	ImGUIManager* manager = ImGUIManager::getInstance();
	ImVec2 originalSize = manager->getOriginalWindowSize();
	ImVec2 currentSize = manager->getMainWindowSize();

	float scaleFactorX = currentSize.x / originalSize.x;
	float scaleFactorY = currentSize.y / originalSize.y;

	windowWidth = windowOriWidth * scaleFactorX;
	windowHeight = windowOriHeight * scaleFactorY;

	windowPosX = windowOriPosX * scaleFactorX;
	windowPosY = windowOriPosY * scaleFactorY;


}

void PEditor::Window::render()
{	
	ImGui::Begin(windowName.c_str(),(bool*)0, (ImGuiWindowFlags_) flags);


	ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));


	ImGui::End();
}

void PEditor::Window::handleInput(SDL_Event* event)
{
}

PEditor::Window::~Window()
{
}

