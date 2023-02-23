#include "Engine.h"

#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <Scene.h>
#include <SDLUtils.h>
#include <EngineTime.h>

#include "Game.h"
#include <iostream>

#include <chrono>
using namespace std::chrono;

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

Engine::Engine() {
	physicsManager = nullptr; rendererManager = nullptr; inputManager = nullptr; 
	soundManager = nullptr; sceneManager = nullptr; engineTime = nullptr;
}

void Engine::init() {

	sceneManager = ECS::SceneManager::init();
	rendererManager = RendererManager::RendererManager::init("MyEngine Window", WIN_WIDTH, WIN_HEIGHT);
	physicsManager = PhysicsManager::PhysicsManager::init(Utilities::Vector2D(0, 9.81f));
	inputManager = InputManager::InputManager::init();
	soundManager = SoundManager::SoundManager::init();
	engineTime = Utilities::EngineTime::init();

	physicsManager->enableDebugDraw(true);

	Game g(sceneManager);
	g.initScenes();
}

void Engine::close() {}

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
		scene->render();
		physicsManager->debugDraw();
		rendererManager->presentRenderer();

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
