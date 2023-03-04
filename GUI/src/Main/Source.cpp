#include <ImGUIManager.h>
#include "imgui_files/imgui.h"
#include "Window.h"
#include "Scene.h"
#include "Image.h"
#include "MenuBar.h"
#include "Hierarchy.h"
#include "FileExplorer.h"

int main(int, char**)
{

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	//MENU BAR
	PEditor::MenuBar* menuBar = new PEditor::MenuBar();
	imGUIManager->addWindow(menuBar);

	//SCENE
	PEditor::Scene* scene = new PEditor::Scene();

	//HIERARCHY
	PEditor::Hierarchy* hierarchy = new PEditor::Hierarchy();
	imGUIManager->addWindow(hierarchy);

	//FILE EXPLORER
	PEditor::FileExplorer* fileExplorer = new PEditor::FileExplorer();
	imGUIManager->addWindow(fileExplorer);

	imGUIManager->loop();
}
