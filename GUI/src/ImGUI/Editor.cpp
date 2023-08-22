﻿#include "Editor.h"

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_impl_sdl2.h"
#include "SDL_image.h"
#include "imgui_internal.h"

#include "ComponentManager.h"
#include "ResourcesManager.h" 
#include "ProjectsManager.h"
#include "ScriptCreation.h"
#include "FileExplorer.h"
#include "ColorPalette.h"
#include "Preferences.h"
#include "Components.h"
#include "GameObject.h"
#include "Hierarchy.h"
#include "MenuBar.h"
#include "Console.h"
#include "Window.h"
#include "Scene.h"
#include "Game.h"

#include <fstream>
#include <iostream>

#include "CheckML.h"


#define _Centered SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED

#define DefaultIconImage "Assets/Images/shyIcon2.png"

Editor* Editor::instance = nullptr;

Editor::Editor() {

	window = nullptr;
	renderer = nullptr;
	projecInfo = nullptr;

	scene = nullptr;
	menuBar = nullptr;
	hierarchy = nullptr;
	fileExplorer = nullptr;
	components = nullptr;
	scriptCreation = nullptr;
	console = nullptr;

	exitEditor = false;
	state = EDITOR_WINDOW;
}

Editor::~Editor() {}

Editor* Editor::getInstance()
{
	if (instance == nullptr)
		instance = new Editor();

	return instance;
}

bool Editor::Init() {

	instance->SplashScreen();

	if (!instance->initImGUIAndSDL())
		return false;

	Components::ComponentManager::Initialise();
	Components::ComponentManager::ReadComponentInfo("Engine\\Components.json");
	Components::ComponentManager::ReadManagerInfo("Engine\\Managers.json");

	ShyEditor::Game::Init("Engine\\Main_Debug.exe", "Engine\\Main_Release.exe");

	ShyEditor::ResourcesManager::Init();

	return true;
}

void Editor::Loop() {

	// The projects management window
	if (!instance->runProjectsWindow())
		instance->exitEditor = true;

	if (!instance->exitEditor) {
		// Configure the SDL window to start the editor
		SDL_SetWindowResizable(instance->window, SDL_TRUE);
		SDL_SetWindowSize(instance->window, _WindowMainSize);
		SDL_SetWindowPosition(instance->window, _Centered);

		// Init the ImGUI windows in the editor
		instance->SetUpWindows();
	}

	// Editor main loop
	while (!instance->exitEditor) {

		ShyEditor::Game::CheckEnd();

		instance->handleInput();
		instance->UpdateAndRenderWindows();
	}

}

void Editor::End() {
	instance->exitEditor = true;
}

void Editor::Close() {

	ShyEditor::ResourcesManager::Release();
	ShyEditor::Game::Release();
	Components::ComponentManager::Release();


	// Cleanup
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(instance->renderer);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();

	delete instance->menuBar;

	for (auto window : instance->windows)
		delete window;

	delete instance->projecInfo;

	delete instance;
	instance = nullptr;
}


bool Editor::initImGUIAndSDL() {

	// ImGUI

	if (!initSDL())
		return false;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	io.ConfigDockingWithShift = false;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();


// SDL

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);


	return true;
}

bool Editor::initSDL() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		// Avisar por el log
		return false;
	}

	// WINDOW
	//SDL_WindowFlags imguiWinFlags = SDL_WindowFlags(ImGui::ImGui_ImplSDL2_GetPlatformWindowFlags());
		// Create our window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("Shy Engine", _Centered, _ProjectSelectionDialogueSize, window_flags);

	// Make sure creating the window succeeded
	if (window == NULL) {
		SDL_Quit();
		// Avisar por el log
		return false;
	}

	// RENDERER

		// Create our renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		// Avisar por el log
		return false;
	}

	SDL_Surface* s = IMG_Load(DefaultIconImage);
	SDL_SetWindowIcon(window, s);
	SDL_FreeSurface(s);

	return true;

}

void Editor::SetUpWindows() {

	// PREFERENCES
	addWindow(new ShyEditor::Preferences());

	// MENU BAR
	menuBar = new ShyEditor::MenuBar();

	// GAME SCENE
	scene = new ShyEditor::Scene();
	addWindow(scene);

	// HIERARCHY
	hierarchy = new ShyEditor::Hierarchy();
	addWindow(hierarchy);

	// FILE EXPLORER
	fileExplorer = new ShyEditor::FileExplorer();
	addWindow(fileExplorer);

	// COMPONENTS
	components = new ShyEditor::ComponentWindow();
	addWindow(components);

	// SCRIPTING
	scriptCreation = new ShyEditor::ScriptCreation();
	addWindow(scriptCreation);

	// CONSOLE
	console = new ShyEditor::Console();
	addWindow(console);

}

void Editor::SplashScreen() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		// Avisar por el log
		return;
	}

	auto window = SDL_CreateWindow("SplashScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 853, 480,
		SDL_WINDOW_BORDERLESS
	);

	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetWindowBordered(window, SDL_FALSE);

	std::string splashscreen = ShyEditor::ResourcesManager::ASSETSFOLDER + "Images\\ShyLogo.png";

	auto surf = IMG_Load(splashscreen.c_str());
	auto texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Color transparente (RGBA)
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);
	SDL_Delay(1500);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

}


bool Editor::runProjectsWindow() {

	instance->addWindow(new ShyEditor::ColorPalette("theme"));

	SDL_SetWindowPosition(instance->window, _Centered);

	ShyEditor::ProjectsManager dialog;

	if (!dialog.MakeFolderToStoreRecentProjects())
		return false;

	auto result = dialog.ManageProjectSelection(instance->renderer);

	if (result == ShyEditor::ProjectsManager::Result::CLOSED)
		return false;

	Components::ComponentManager::ReadScripts(instance->projecInfo->path + "Scripts\\");
	ShyEditor::ResourcesManager::GetInstance()->SetEngineResourcesPath(instance->projecInfo->path + "Assets\\");

	return true;

}


void Editor::UpdateAndRenderWindows()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	auto static once = true;

	if (once) {

		once = false;

		auto id = ImGui::DockSpaceOverViewport();

		auto rootNode = ImGui::DockBuilderGetNode(id);
		if (rootNode->IsEmpty()) {

			ImGui::DockBuilderRemoveNode(id);

			ImGui::DockBuilderAddNode(id);
			ImGui::DockBuilderSetNodeSize(id, getMainWindowSize());


			auto abajo = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.3f, nullptr, &id);
			auto izquierda = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.2f, nullptr, &id);
			auto centro = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.6f, nullptr, &id);


			ImGui::DockBuilderDockWindow("Hierarchy", izquierda);
			ImGui::DockBuilderDockWindow("File Explorer", abajo);
			ImGui::DockBuilderDockWindow("Scene", centro);
			ImGui::DockBuilderDockWindow("Components", id);


			ImGui::DockBuilderFinish(id);
		}
	}
	else
	{
		if (state == Editor::EDITOR_WINDOW)
			ImGui::DockSpaceOverViewport();
	}



	ShyEditor::ColorPalette::ApplyPalette();

	for (auto window : windows)
	{
		if (window->CanBeDrawnOnTop() && !window->isDocked())
			window->UpdateWindow();
		else
			switch (state)
			{
			case Editor::SCRIPTING_WINDOW:
				if (window == scriptCreation)
					window->UpdateWindow();
				break;
			case Editor::EDITOR_WINDOW:
				if (window != scriptCreation)
					window->UpdateWindow();
				break;
			default:
				break;
			}
	}

	if (state == Editor::EDITOR_WINDOW)
		menuBar->Update();

	if (ImGui::IsMouseReleased(0)) {

		if (ShyEditor::ResourcesManager::IsAnyAssetSelected())
			ShyEditor::ResourcesManager::UnselectAsset();
	}

	// Rendering
	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);

}

void Editor::handleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		if (event.type == SDL_QUIT)
			exitEditor = true;

		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			exitEditor = true;

		for (auto window : windows)
			window->HandleInput(&event);
	}
}

void Editor::changeEditorState(const EDITOR_STATE& state) {

	switch (state) {
	case Editor::EDITOR_WINDOW:
		break;
	case Editor::SCRIPTING_WINDOW:
		scriptCreation->Load();
		break;
	default:
		break;
	}

	this->state = state;
}

void Editor::setProjectInfo(ShyEditor::ProjectInfo* pInfo) {
	this->projecInfo = pInfo;
}

ShyEditor::ProjectInfo& Editor::getProjectInfo() {
	return *projecInfo;
}

void Editor::addWindow(ShyEditor::Window* window) {
	windows.push_back(window);
}

void Editor::setScene(ShyEditor::Scene* scene) {
	this->scene = scene;
}

SDL_Renderer* Editor::getRenderer() {
	return renderer;
}

ImVec2 Editor::getMainWindowSize() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return ImVec2(w, h);
}

ShyEditor::Scene* Editor::getScene() {
	return scene;
}

ShyEditor::MenuBar* Editor::getMenuBar() {
	return menuBar;
}

ShyEditor::Hierarchy* Editor::getHierarchy() {
	return hierarchy;
}

ShyEditor::FileExplorer* Editor::getFileExplorer() {
	return fileExplorer;
}

ShyEditor::ComponentWindow* Editor::getComponents() {
	return components;
}

ShyEditor::ScriptCreation* Editor::getScriptCreation() {
	return scriptCreation;
}

ShyEditor::Console* Editor::getConsole()
{
	return console;
}

void Editor::OpenScript(const std::string& script) {
	scriptCreation->SetName(script);
	changeEditorState(EDITOR_STATE::SCRIPTING_WINDOW);
}

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "SDL.h"
#include <cstdio>


int Editor::Probando()
{

	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return 0;
	}
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


		ImGui::Begin("File system");
		ImGui::Text("heyy");
		ImGui::End();


		ImGui::Begin("Jerarquia");
		ImGui::Text("Noooo");
		ImGui::End();



		ImGui::Begin("Scene");
		ImGui::Text("Noooo");
		ImGui::End();



		ImGui::Begin("Componentes");
		ImGui::Text("Noooo");
		ImGui::End();



		// Rendering
		ImGui::Render();
		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

	return 0;
}


int Editor::Example() {

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return 0;
	}
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();


		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
