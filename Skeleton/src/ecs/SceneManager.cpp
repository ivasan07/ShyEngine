#include "SceneManager.h"
#include "Scene.h"

#include "SceneLoader.h"
#include "RendererManager.h"

namespace ECS {

	SceneManager::SceneManager() {
		mode = PUSH;
		change = false;
	}

	SceneManager::~SceneManager() {
		removeAllScenes();
	}

	Scene* SceneManager::getActualScene() {

		if (scenes.empty()) return nullptr;

		return scenes.top();
	}

	int SceneManager::getNumberOfScenes() {
		return scenes.size();
	}

	std::string SceneManager::GetCurrentScenePath() {
		return currentScenePath;
	}

	Scene* SceneManager::createScene(const std::string& sce_name) {
		return new Scene(sce_name);
	}


	void SceneManager::ChangeScene(std::string scene, int m) {
		currentScenePath = scene;
		mode = (LOAD_MODE) m;
		change = true;
	}

	void SceneManager::ResetScene() {
		
		ChangeScene(currentScenePath, LOAD_MODE::POP_AND_PUSH);
	}

	void SceneManager::manageScenes() {
		if (change) {
			switch (mode) {

			case PUSH:
				pushScene();
				break;

			case POP:
				popScene();
				break;

			case POP_AND_PUSH:
				popAndPushScene();
				break;

			case CLEAR_AND_PUSH:
				removeAllScenes();
				pushScene();
				break;

			case CLEAR:
				removeAllScenes();
				break;

			default:
				break;
			}
		}

		change = false;
	}

	void SceneManager::pushScene() {

		if (scenes.size() > 0) scenes.top()->onSceneDown();

		scenes.push(LoadScene(currentScenePath));
		scenes.top()->start();
	}

	void SceneManager::popScene() {
		if (scenes.size() > 0) {
			scenes.top()->onDestroy();
			delete scenes.top();
			scenes.pop();

			if (scenes.size() > 0) scenes.top()->onSceneUp();
		}
	}

	void SceneManager::popAndPushScene() {
		if (scenes.size() > 0) {
			scenes.top()->onDestroy();
			delete scenes.top();
			scenes.pop();

			scenes.push(LoadScene(currentScenePath));
			scenes.top()->start();
		}
	}

	void SceneManager::removeAllScenes() {
		auto size = scenes.size();
		for (int i = 0; i < size; i++) popScene();
	}


	Scene* SceneManager::LoadScene(std::string const& scenePath)
	{
		Scene* scene = SceneLoader::LoadScene(scenePath);

		if (scene == nullptr) {
			return nullptr;
		}

		scene->init();
		return scene;
	}

	void SceneManager::SetScene(ECS::Scene* scene) {

		scenes.push(scene);
		scene->start();
	}

	void SceneManager::EndGame()
	{
		ChangeScene("", LOAD_MODE::CLEAR);
	}

	float SceneManager::CameraScale()
	{
		return scenes.top()->cameraScale;
	}

	Utilities::Vector2D SceneManager::CameraPosition()
	{
		return scenes.top()->cameraPosition;
	}

	void SceneManager::SetCameraScale(float newScale)
	{
		scenes.top()->cameraScale = newScale;
	}

	void SceneManager::SetCameraPosition(Utilities::Vector2D const& newPosition)
	{
		scenes.top()->cameraPosition = newPosition;
	}


	void SceneManager::AdjustRectToCamera(int& x, int& y, int& w, int& h) {

		auto campos = CameraPosition();
		float camscale = CameraScale();

		x += campos.x_;
		y += campos.y_;

		x *= camscale;
		y *= camscale;
		w *= camscale;
		h *= camscale;

		int winwid = Renderer::RendererManager::instance()->getWidth() * 0.5f;
		int winhei = Renderer::RendererManager::instance()->getHeight() * 0.5f;

		x += winwid - (winwid * camscale);
		y += winhei - (winhei * camscale);
	}


}
