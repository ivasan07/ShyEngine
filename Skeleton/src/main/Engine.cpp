#include "Engine.h"

#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <ResourcesManager.h>
#include <ContactListener.h>
#include <Scene.h>
#include <SDLUtils.h>
#include <EngineTime.h>
#include <RenderManager.h>
#include <OverlayManager.h>

#include <Component.h>

#include <ECSUtilities/FunctionManager.h>
#include <ECSUtilities/ComponentFactory.h>
#include <ECSUtilities/ClassReflection.h>

#include <Scripting/ScriptManager.h>
#include <Scripting/ScriptFunctionality.h>

#include <ConsoleManager.h>

#include "Game.h"

#include <chrono>

using namespace std::chrono;

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

Engine::Engine() {

	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr; contactListener = nullptr;
	soundManager = nullptr; sceneManager = nullptr; engineTime = nullptr; resourcesManager = nullptr;
	scriptManager = nullptr;
	scriptFunctionality = nullptr;
	componentFactory = nullptr;
	renderManager = nullptr;
	overlayManager = nullptr;
}

void Engine::init() {

	if (ECS_Version != ECSfunc_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the scripting version. This may cause unexpected behaviour");
	}
	
	//TODO: Se guardan algunos punteros a managers que realmente no son necesarios

	if (ECS_Version != ECSreflection_Version) {
		Console::Output::PrintWarning("Engine version", "The engine version does not match the editor reflection version. This may cause unexpected behaviour");
	}


	sceneManager = ECS::SceneManager::init();
	rendererManager = Renderer::RendererManager::init("MyEngine Window", WIN_WIDTH, WIN_HEIGHT);
	physicsManager = Physics::PhysicsManager::init(Utilities::Vector2D(0, 9.81f));
	contactListener = ECS::ContactListener::init();
	inputManager = Input::InputManager::init();
	soundManager = SoundManager::SoundManager::init();
	resourcesManager = Resources::ResourcesManager::init();
	engineTime = Utilities::EngineTime::init();
	scriptManager = Scripting::ScriptManager::init();
	scriptFunctionality = Scripting::ScriptFunctionality::init();
	componentFactory = ComponentFactory::init();
	renderManager = ECS::RenderManager::init();
	overlayManager = ECS::OverlayManager::init();

	physicsManager->enableDebugDraw(true);


	//Pablo esto es lo tuyo, descomenta esto y comenta lo siguiente
	Game(sceneManager).initScenes();

	//sceneManager->changeScene(sceneManager->LoadScene("DefaultScene.json"), ECS::SceneManager::PUSH);
	//sceneManager->manageScenes();
}

void Engine::update() {

	SDL_Event event;

	steady_clock::time_point beginTime = high_resolution_clock::now();
	steady_clock::time_point physicsTime = beginTime;
	steady_clock::time_point startTime = beginTime;

	while (true) {

		// Scene
		auto scene = sceneManager->getActualScene();
		if (scene == nullptr) break;

		// Input
		if (!inputManager->handleInput(event)) break;

		// FixedUpdate
		while (physicsTime < beginTime) {

			scene->fixedUpdate(engineTime->fixedDeltaTime);
			physicsManager->fixedUpdate(engineTime->fixedDeltaTime);

			physicsTime += std::chrono::milliseconds((int) (engineTime->fixedDeltaTime * 1000));
		}

		// Update
		scene->update(engineTime->deltaTime);

		// LateUpdate
		scene->lateUpdate(engineTime->deltaTime);

		// Render
		rendererManager->clearRenderer(Utilities::createColor(0x835CF3FF));
		//scene->render();
		renderManager->Render();
		physicsManager->debugDraw();
		overlayManager->Render();


		rendererManager->presentRenderer();

		// Handling physics bodies
		physicsManager->handleBodies();

		// Remove dead entities
		scene->removeEntities();

		// Change scene if necessary
		sceneManager->manageScenes();

		// Time
		steady_clock::time_point endTime = high_resolution_clock::now();

		duration<float, std::milli> timeSinceStart = endTime - startTime;
		duration<double, std::milli> frameTime = endTime - beginTime;

		engineTime->timeSinceStart = timeSinceStart.count();
		engineTime->deltaTime = frameTime.count() * 0.001f;
		engineTime->frames++;

		beginTime = endTime;

	}
}

void Engine::close() {

	sceneManager->close();

}
