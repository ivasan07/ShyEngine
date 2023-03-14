#include "Game.h"

#include <SceneManager.h>
#include <RendererManager.h>
#include <Scene.h>
#include <Entity.h>
#include <Components/Transform.h>
#include <Components/Image.h>
#include <Components/Collider.h>
#include <Components/Rigidbody.h>
#include <Components/TestComponent.h>

Game::Game(ECS::SceneManager* sm) {
	sceneManager = sm;

	scene = nullptr;
}

void Game::initScenes() {

	firstScene();
}

void Game::firstScene() {
	RendererManager::RendererManager* renderer = RendererManager::RendererManager::instance();

	// Default scene
	scene = sceneManager->createScene("Default scene");

	// Player
	ECS::Entity* player = scene->createEntity("Player");

	auto tr = player->addComponent<ECS::Transform>();
	auto im = player->addComponent<ECS::Image>("link.png");
	auto collider = player->addComponent<ECS::Collider>();
	auto rb = player->addComponent<ECS::Rigidbody>();

	auto tComp = player->addComponent<ECS::TestComponent>();

	// Ground
	ECS::Entity* ground = scene->createEntity("Ground");

	auto grTr = ground->addComponent<ECS::Transform>();
	auto grIm = ground->addComponent<ECS::Image>("ground.png");
	auto grCol = ground->addComponent<ECS::Collider>();

	// Init
	scene->init();

	// Player settings
	tr->setPosition(renderer->getWidth() / 2, renderer->getHeight() / 2);
	tr->setScale(0.5f, 0.5f);
	tr->rotate(45.0f);

	collider->setBounciness(0.5f);
	rb->setBodyType(ECS::Rigidbody::DINAMIC);

	// Ground settings
	grTr->setPosition(renderer->getWidth() / 2, renderer->getHeight());


	// Scripting
	//tr->CallMethod("HolaMamasita", { Scripting::Variable::Component(tr), Scripting::Variable::Float(2)});


	// START
	sceneManager->changeScene(scene, ECS::SceneManager::PUSH);
	sceneManager->manageScenes();

}