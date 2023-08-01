#pragma once

/*
* A class to test the Engine
*/

namespace ECS {
	class SceneManager;
	class Scene;
}

class Game {

public:

	Game(ECS::SceneManager* sm);
	
	void initScenes();

private:

	ECS::SceneManager* sceneManager;

	ECS::Scene* scene;

	void firstScene();

};