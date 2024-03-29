#include "Scene.h"

#include "ScriptCreationUtilities.h"
#include "ResourcesManager.h"
#include "ProjectsManager.h"
#include "FileExplorer.h"
#include "Preferences.h"
#include "Components.h"
#include "LogManager.h"
#include "Entity.h"
#include "Hierarchy.h"
#include "Window.h"
#include "Editor.h"
#include "Camera.h"
#include "imgui.h"
#include "PrefabManager.h"
#include "SDL.h"
#include "ComponentInfo.h"
#include "ColorPalette.h"

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#include "CheckML.h"

#define Foreground "black.png"

namespace ShyEditor {

	Scene::Scene() : Window("Scene", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {

		sceneScale = 1;
		scenePosition_x = scenePosition_y = 0;

		Editor* editor = Editor::GetInstance();
		editor->SetScene(this);

		renderer = editor->GetRenderer();

		// Load prefabs before loading scene
		PrefabManager::LoadPrefabs();

		// Load the default scene or the last opene scene after creating the windows
		bool load = LoadScene();
		editor->SetAnySceneOpened(load);

		sceneCamera = new Camera(ImVec2(0, 0), 1, renderer);
		sceneCamera->SetConstrains(.1, 10.0);

		uiTexture = nullptr;
		uiWidth = 0; uiHeight = 0;
		ResizeOverlayIfNeccesary();

		foreground = ResourcesManager::GetInstance()->AddTexture(Foreground, true);

		// Pointer to the selected entity in the scene
		selectedEntity = nullptr;

		docked = true;
		viewMode = 0;

		acceptAssetDrop = true;

		editor->GetFileExplorer()->Refresh();

	}

	Scene::~Scene() {

		// Delete entities in the scene
		for (const auto& pair : entities)
			delete pair.second;

		entities.clear();

		// Delete overlays in the scene
		for (const auto& o : overlays)
			delete o;

		overlays.clear();

		delete sceneCamera;
	}

	float& Scene::GetSceneScale()
	{
		return sceneScale;
	}

	ImVec2 Scene::GetScenePosition()
	{
		return ImVec2(scenePosition_x, scenePosition_y);
	}

	void Scene::SetCameraPosition(float x, float y)
	{
		scenePosition_x = x;
		scenePosition_y = y;
	}

	Entity* Scene::AddEntity(std::string path) {

		Entity* entity = new Entity(path, true);
		entities.emplace(entity->GetId(), entity);

		return entity;
	}

	void Scene::AddOverlayChildsToScene(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {
			overlays.push_back(pair.second);

			AddOverlayChildsToScene(pair.second);
		}
	}

	void Scene::AddEntity(Entity* entity) {
		entities.emplace(entity->GetId(), entity);
	}

	void Scene::AddEntityChildsToScene(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {
			entities.emplace(pair.second->GetId(), pair.second);

			AddEntityChildsToScene(pair.second);
		}
	}

	Entity* Scene::AddOverlay(std::string path)
	{
		Entity* entity = new Entity(path, false);
		overlays.push_back(entity);

		return entity;
	}

	void Scene::AddOverlay(Entity* overlay) {
		overlays.push_back(overlay);
	}

	std::map<int, Entity*>& Scene::GetEntities() {
		return entities;
	}

	std::vector<Entity*>& Scene::GetOverlays()
	{
		return overlays;
	}

	std::string Scene::GetSceneName() {
		return sceneName;
	}

	Entity* Scene::GetSelectedEntity() {
		return selectedEntity;
	}

	void Scene::SetSelectedEntity(Entity* entity) {
		selectedEntity = entity;
	}

	void Scene::NewScene(const std::string& name) {

		Editor::GetInstance()->GetFileExplorer()->Refresh();

		nlohmann::ordered_json j;

		scenePath = Editor::GetInstance()->GetProjectInfo().assetPath + "\\" + name + ".scene";
		sceneName = name;
		this->name = sceneName.c_str();

		// Can not create a scene with the same name in the same folder
		std::ifstream file(scenePath);
		if (file.good()) return;

		std::ofstream outputFile(scenePath);
		if (outputFile.is_open()) {

			nlohmann::ordered_json json;

			json["name"] = sceneName;
			json["objects"] = nlohmann::json::array();
			json["overlays"] = nlohmann::json::array();

			outputFile << json.dump(4);
			outputFile.close();

			// Delete info of previous scene
			DeleteContentInScene();
		}
		else LogManager::LogError("Could not open the file to create the scene.");

	}

	void Scene::SaveScene() {

		// Check if scene file exists
		std::ifstream fileExists(scenePath);
		if (!fileExists.good() || !nlohmann::json::accept(fileExists)) {
			LogManager::LogError("Could not find the current scene file.");
			return;
		}

		// Open the file to write the new scene data
		std::ofstream outputFile(scenePath);
		if (!outputFile.is_open()) {
			LogManager::LogError("Could not open the file to save the scene.");
			return;
		}

		nlohmann::ordered_json j = ToJson();

		outputFile << j.dump(4);
		outputFile.close();

	}

	bool Scene::LoadScene() {

		auto path = Editor::GetInstance()->GetProjectInfo().assetPath + "\\" + Editor::GetInstance()->GetLastOpenedScene();
		if (scenePath == path) return true;

		scenePath = path;
		std::filesystem::path p(scenePath);
		sceneName = p.filename().stem().string();
		name = sceneName.c_str();


		std::ifstream inputFile(scenePath);

		if (!inputFile.good() || !nlohmann::ordered_json::accept(inputFile)) {
			LogManager::LogError("Could not open the file to load the scene.");
			Editor::GetInstance()->SetLastOpenedScene("");
			Preferences::GetData().initialScene = "";
			return false;
		}

		inputFile.clear();
		inputFile.seekg(0);

		nlohmann::ordered_json jsonData = nlohmann::ordered_json::parse(inputFile);
		inputFile.close();

		Entity::unusedIds.clear();
		Entity::lastId = 1;
		// Delete info of previous scene
		DeleteContentInScene();

		if (jsonData.contains("position"))
		{
			sscanf_s(jsonData["position"].get<std::string>().c_str(), "%f, %f", &scenePosition_x, &scenePosition_y);

			scenePosition_x *= -1;
			scenePosition_y *= -1;
		}

		if (jsonData.contains("scale"))
		{
			sceneScale = jsonData["scale"].get<float>();
		}


		if (!jsonData.contains("objects")) {
			LogManager::LogError("The scene file has not the expected format.");
			Editor::GetInstance()->SetLastOpenedScene("");
			Preferences::GetData().initialScene = "";
			return false;
		}

		nlohmann::ordered_json& entitiesJson = jsonData["objects"];

		// Iterate through the entities JSON array
		for (nlohmann::ordered_json& entityJson : entitiesJson) {
			Entity* entity = Entity::FromJson(entityJson);
			entities.insert({ entity->GetId(), entity });
			AddEntityChildsToScene(entity);
		}

		if (!jsonData.contains("overlays")) {
			LogManager::LogError("The scene file has not the expected format.");
			ProjectsManager::GetInstance()->StoreLastOpenedScene("");
			Preferences::GetData().initialScene = "";
			return false;
		}

		nlohmann::ordered_json& overlaysJson = jsonData["overlays"];

		// Iterate through the overlay entities JSON array
		for (nlohmann::ordered_json& overlayJson : overlaysJson) {
			Entity* overlay = Entity::FromJson(overlayJson);
			overlays.push_back(overlay);
			AddOverlayChildsToScene(overlay);
		}

		for (auto& entity : entities) {
			UpdateReferencesToEntity(entity.second);
		}

		for (auto& overlay : overlays) {
			UpdateReferencesToEntity(overlay);
		}

		Preferences::GetData().initialScene = Editor::GetInstance()->GetLastOpenedScene();

		return true;
	}

	void Scene::DeleteContentInScene() {

		sceneScale = 1;
		scenePosition_x = scenePosition_y = 0;

		// Delete info of previous scene
		selectedEntity = nullptr;

		for (const auto& pair : entities)
			delete pair.second;

		entities.clear();

		for (const auto& o : overlays)
			delete o;

		overlays.clear();
	}

	void Scene::UpdateReferencesToEntity(Entity* entity) {
		for (auto& script : *entity->GetScripts()) {
			for (auto& attr : script.second.GetAllAttributes()) {
				if (attr.second.GetType() == Components::AttributesType::ENTITY) {
					int idx = attr.second.value.value.entityIdx;

					if (idx > 0) {
						std::map<int, Entity*>& entities = Editor::GetInstance()->GetScene()->GetEntities();
						std::vector<Entity*>& overlays = Editor::GetInstance()->GetScene()->GetOverlays();

						auto entityIt = entities.find(idx);

						if (entityIt != entities.end()) {
							entityIt->second->AddReferenceToEntity(&attr.second);
							continue;
						}

						for (int i = 0; i < overlays.size(); i++) {
							if (overlays[i]->GetId() == idx) {
								overlays[i]->AddReferenceToEntity(&attr.second);
								break;
							}
						}

					}
					else if (idx < 0) {
						std::unordered_map<int, Entity*>& prefabs = PrefabManager::GetPrefabs();

						auto prefabIt = prefabs.find(idx);

						if (prefabIt != prefabs.end()) {
							prefabIt->second->AddReferenceToEntity(&attr.second);
							continue;
						}
					}
				}
			}
		}
	}

	void Scene::RenderChildEntities(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {

			RenderChildEntities(pair.second);
			pair.second->RenderTransform(renderer, sceneCamera);
		}
	}

	void Scene::RenderEntities()
	{
		std::vector<Entity*> sortedEntities;
		for (const auto& pair : entities) {
			if (pair.second->GetParent() == nullptr)
				sortedEntities.push_back(pair.second);
		}

		std::sort(sortedEntities.begin(), sortedEntities.end(), CompareEntitiesRenderOrder);

		for (auto& entity : sortedEntities) {
			entity->RenderTransform(renderer, sceneCamera);
			RenderChildEntities(entity);
		}
	}

	void Scene::RenderFrame()
	{

		auto& color = ColorPalette::GetCurrentPalette().scene;

		int thickness = color.frameWidth;

		if (thickness > 0) {

			// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
			SDL_SetRenderDrawColor(renderer, ColorPaletteParamsInt(color.frame), 255);

			SDL_Rect frameRect{};
			CalculateFrameRect(frameRect.x, frameRect.y, frameRect.w, frameRect.h);

			RenderRectangle(frameRect.x, frameRect.y, frameRect.w, frameRect.h, thickness);

			SDL_SetRenderDrawColor(renderer, r, g, b, a);
		}
	}

	void Scene::RenderUI() {

		if (uiTexture == nullptr) return;

		auto currentTarget = SDL_GetRenderTarget(renderer);
		SDL_SetRenderTarget(renderer, uiTexture);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);


		bool inScene = IsMouseHoveringWindow();
		auto mouse = MousePositionInUI();

		float cameraScale = sceneCamera->GetScale();

		for (auto overlayEntity : overlays) {

			Overlay* overlay = overlayEntity->GetOverlay();


			//TODO: aplicar color y escala

			SDL_Rect dest{};
			overlay->CalculateRectangle(dest.x, dest.y, dest.w, dest.h);


			bool mouseclicked = ImGui::IsMouseClicked(0);


			auto& color = ColorPalette::GetCurrentPalette().scene;

			int thickness = color.UIBorderWidth;

			bool drawUiElements = thickness > 0;

			if (drawUiElements) {

				SDL_SetRenderDrawColor(renderer, ColorPaletteParamsInt(color.UIBorder), 255);
				RenderRectangle(dest.x, dest.y, dest.w, dest.h, thickness);
			}
			overlay->Render(renderer, dest.x, dest.y, dest.w, dest.h);

			if (!inScene) continue;

			if (selectedOverlay.overlay == nullptr)
			{
				thickness *= 2;

				SDL_SetRenderDrawColor(renderer, ColorPaletteParamsInt(color.UIBorderHover), 255);

				if (PointInsideHorizontalSegment(mouse.x, mouse.y, dest.x, dest.y, dest.w, thickness)) {

					if (drawUiElements)
						RenderHorizontalSegment(dest.x, dest.y, dest.w, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_TOP;
				}

				if (PointInsideHorizontalSegment(mouse.x, mouse.y, dest.x, dest.y + dest.h, dest.w, thickness)) {

					if (drawUiElements)
						RenderHorizontalSegment(dest.x, dest.y + dest.h, dest.w, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_BOTTOM;
				}

				if (PointInsideVerticalSegment(mouse.x, mouse.y, dest.x, dest.y, dest.h, thickness)) {

					if (drawUiElements)
						RenderVerticalSegment(dest.x, dest.y, dest.h, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_LEFT;
				}

				if (PointInsideVerticalSegment(mouse.x, mouse.y, dest.x + dest.w, dest.y, dest.h, thickness)) {

					if (drawUiElements)
						RenderVerticalSegment(dest.x + dest.w, dest.y, dest.h, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_RIGHT;
				}
			}



			if (mouseclicked) {

				if (PointInsideRect(mouse.x, mouse.y, dest.x, dest.y, dest.w, dest.h, thickness)) {



					selectedEntity = overlayEntity;
					selectedOverlay.overlay = overlay;

					selectedOverlay.offset_x = dest.x - mouse.x;
					selectedOverlay.offset_y = dest.y - mouse.y;
				}
			}

		}

		SDL_SetRenderTarget(renderer, currentTarget);


		SDL_Rect frameRect = { };
		CalculateFrameRect(frameRect.x, frameRect.y, frameRect.w, frameRect.h);

		SDL_RenderCopy(renderer, uiTexture, NULL, &frameRect);


		if (ImGui::IsMouseReleased(0))
		{
			selectedOverlay.overlay = nullptr;
			selectedOverlay.dir = 0;
		}



		if (selectedOverlay.overlay != nullptr) {

			bool changeSize = selectedOverlay.dir != 0;
			bool isPositioned = selectedOverlay.overlay->GetPlacement() == 0;

			if (changeSize) {


				if (selectedOverlay.dir & DIR_TOP) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().y;
						float anchor = selectedOverlay.overlay->GetAnchor().y;
						float& size = selectedOverlay.overlay->GetSize().y;
						float mousePos = mouse.y;

						//Punto central del overlay para posicionarlo correctamente
						float centerOffset = anchor * size;

						//Guardamos la posicion previa del overlay
						float py = position;
						position = mousePos + centerOffset;

						//El incremento es la diferencia entre la posicion actual y la anterior
						float increment = py - position;

						//Para que parezca que se esta moviendo arrastrandolo de un lado, el incremento hay que multiplicarlo por la posicion de anclaje
						position += increment * anchor;

						//Lo que hayamos aumentado de posicion hay que reducirlo de tama�o y viceversa
						size += increment;
					}
					else
						selectedOverlay.overlay->GetTop() = mouse.y;
				}

				if (selectedOverlay.dir & DIR_LEFT) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().x;
						float anchor = selectedOverlay.overlay->GetAnchor().x;
						float& size = selectedOverlay.overlay->GetSize().x;
						float mousePos = mouse.x;


						//Punto central del overlay para posicionarlo correctamente
						float centerOffset = anchor * size;

						//Guardamos la posicion previa del overlay
						float py = position;
						position = mousePos + centerOffset;

						//El incremento es la diferencia entre la posicion actual y la anterior
						float increment = py - position;

						//Para que parezca que se esta moviendo arrastrandolo de un lado, el incremento hay que multiplicarlo por la posicion de anclaje
						position += increment * anchor;

						//Lo que hayamos aumentado de posicion hay que reducirlo de tama�o y viceversa
						size += increment;
					}
					else
						selectedOverlay.overlay->GetLeft() = mouse.x;
				}

				if (selectedOverlay.dir & DIR_RIGHT) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().x;
						float anchor = selectedOverlay.overlay->GetAnchor().x;
						float& size = selectedOverlay.overlay->GetSize().x;
						float mousePos = mouse.x;

						float centerOffset = anchor * size;

						float psize = size;

						//Calculo las dimensiones del elemento a partir de la esquina superior izquierda al raton
						size = mousePos - (position - centerOffset);

						float increment = psize - size;

						//Lo que haya incrementado de tama�o hay que compensarlo con la posicion, para de esta forma
						//dar la sensacion de que estamos arrastrando desde un lado
						position -= anchor * increment;
					}
					else
						selectedOverlay.overlay->GetRight() = Preferences::GetData().width - mouse.x;
				}

				if (selectedOverlay.dir & DIR_BOTTOM) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().y;
						float anchor = selectedOverlay.overlay->GetAnchor().y;
						float& size = selectedOverlay.overlay->GetSize().y;
						float mousePos = mouse.y;

						float centerOffset = anchor * size;

						float psize = size;

						//Calculo las dimensiones del elemento a partir de la esquina superior izquierda al raton
						size = mousePos - (position - centerOffset);

						float increment = psize - size;

						//Lo que haya incrementado de tama�o hay que compensarlo con la posicion, para de esta forma
						//dar la sensacion de que estamos arrastrando desde un lado
						position -= anchor * increment;
					}
					else
						selectedOverlay.overlay->GetBottom() = Preferences::GetData().height - mouse.y;
				}


			}
			else {


				if (isPositioned) {

					auto& position = selectedOverlay.overlay->GetPosition();
					auto& anchor = selectedOverlay.overlay->GetAnchor();
					auto& size = selectedOverlay.overlay->GetSize();


					position.x = mouse.x + selectedOverlay.offset_x + anchor.x * size.x;
					position.y = mouse.y + selectedOverlay.offset_y + anchor.y * size.y;
				}

				else {

					int& left = selectedOverlay.overlay->GetLeft();
					int& right = selectedOverlay.overlay->GetRight();
					int& top = selectedOverlay.overlay->GetTop();
					int& bottom = selectedOverlay.overlay->GetBottom();


					int pleft = left;
					int ptop = top;

					left = mouse.x + selectedOverlay.offset_x;
					top = mouse.y + selectedOverlay.offset_y;

					right += pleft - left;
					bottom += ptop - top;
				}
			}

		}

	}

	void Scene::HandleInput(SDL_Event* event) {

		if (event->type == SDL_KEYDOWN) {

			if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {

				if (event->key.keysym.scancode == SDL_SCANCODE_S)
					SaveScene();
			}
		}

		if (!focused) return;

		ImVec2 mousePos = ImGui::GetMousePos();
		bool insideWindow = IsMouseHoveringWindow();
		bool anyEntitySelected = false;
		
		
		if (insideWindow && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		{
			for (const auto& pair : entities) {

				if (IsMouseHoveringEntity(pair.second)) {

					anyEntitySelected = true;
					selectedEntity = pair.second;
					dragging = true;
				}

			}
		}


		if (insideWindow && !anyEntitySelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			SetSelectedEntity(nullptr);
		}

		if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
		{
			dragging = false;
		}

		if (dragging && selectedEntity != nullptr && selectedEntity->IsTransform() && event->type == SDL_MOUSEMOTION) {

			float invCameraScale = 1.f / sceneCamera->GetScale();

			if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {

				const float incrementSpeed = .3f;

				float r = selectedEntity->GetWorldRotation();

				r += event->motion.xrel * incrementSpeed;

				selectedEntity->SetWorldRotation(r);
			}
			else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)) {

				const float incrementSpeed = .03f;

				ImVec2 worldScale = selectedEntity->GetWorldScale();

				float x = worldScale.x;
				float y = worldScale.y;

				float xspeed = std::log10(1 + x) * incrementSpeed;
				float yspeed = std::log10(1 + y) * incrementSpeed;

				event->motion.yrel *= -1;

				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {

					event->motion.yrel = event->motion.xrel;
				}

				x += event->motion.xrel * invCameraScale * xspeed;
				y += event->motion.yrel * invCameraScale * yspeed;

				x = std::clamp(x, 0.f, FLT_MAX);
				y = std::clamp(y, 0.f, FLT_MAX);

				ImVec2 newScale = { x, y };
				selectedEntity->SetWorldScale(newScale);
			}
			else {

				auto pos = selectedEntity->GetWorldPosition();
				pos.x += event->motion.xrel * invCameraScale;
				pos.y += event->motion.yrel * invCameraScale;

				selectedEntity->SetWorldPosition(pos);
			}
		}


		sceneCamera->handleInput(event, insideWindow, focused);

		if (event->type == SDL_KEYUP && event->key.keysym.scancode == SDL_SCANCODE_SPACE) {


			if (selectedEntity != nullptr) {

				if (selectedEntity->IsTransform()) {

					auto pos = selectedEntity->GetWorldPosition();
					sceneCamera->SetPosition(pos.x, pos.y);
				}
				else {

					float camScale = sceneCamera->GetScale();
					auto pos = selectedEntity->GetOverlay()->CalculateCenterPoint();
					sceneCamera->SetPosition(pos.x * camScale, pos.y * camScale);

				}
			}
		}

	}

	void Scene::Behaviour() {

		if (Editor::GetInstance()->IsAnySceneOpened()) {

			ResizeOverlayIfNeccesary();

			if (sceneCamera->ShouldResize(windowWidth, windowHeight))
				sceneCamera->Resize(windowWidth, windowHeight);

			auto it = entities.begin();
			while (it != entities.end()) {
				Entity* entity = it->second;

				entity->Update();

				if (entity->IsWaitingToDelete()) {
					selectedEntity = nullptr;

					Entity::unusedIds.push_back(entity->GetId());

					delete entity;
					it = entities.erase(it);
				}
				else {
					++it;
				}
			}


			auto it2 = overlays.begin();
			while (it2 != overlays.end()) {
				Entity* entity = *it2;

				entity->Update();

				if (entity->IsWaitingToDelete()) {
					selectedEntity = nullptr;

					Entity::unusedIds.push_back(entity->GetId());

					delete entity;
					it2 = overlays.erase(it2);
				}
				else {
					++it2;
				}
			}


			sceneCamera->PrepareCameraRender();

			switch (viewMode) {

			case 0:
				RenderEntities();
				RenderUI();
				break;
			case 1:
				RenderEntities();
				break;
			case 2:
				RenderUI();
				break;

			default:
				break;
			}

			RenderFrame();


			sceneCamera->StopCameraRender();

			// Cambiar las propiedades del grid dependiendo del orden de magnitud

			auto& color = ColorPalette::GetCurrentPalette().scene;

			float camScale = sceneCamera->GetScale();

			float spacing = color.gridSpacing * sceneCamera->GetScale();
			int interval = color.gridInterval;

			int offset_x = windowWidth * 0.5f;
			int offset_y = windowHeight * 0.5f;


			if (camScale < 0.7f) {

				spacing *= interval;
			}


			ScriptCreationUtilities::Grid::DrawBackgroundColor(ColorPaletteParams(color.background), color.backgroundAlpha);

			ScriptCreationUtilities::Grid::SetAlpha(color.gridAlpha);
			ScriptCreationUtilities::Grid::SetColor(ColorPaletteParams(color.grid));
			ScriptCreationUtilities::Grid::SetSpacing(spacing);
			ScriptCreationUtilities::Grid::SetThickness(color.gridThickness);
			ScriptCreationUtilities::Grid::SetInterval(interval);
			ScriptCreationUtilities::Grid::SetOffset(offset_x + sceneCamera->GetPosition().x, offset_y + sceneCamera->GetPosition().y);
			ScriptCreationUtilities::Grid::Draw();


			ImGui::SetCursorPos(ImVec2(0, 0));
			ImGui::Image(sceneCamera->GetTexture(), ImVec2(windowWidth, windowHeight));

			ImGui::SetCursorPos(ImVec2(10, ImGui::GetFrameHeight() + 10));
			ImGui::Text(name);
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(100, ImGui::GetFrameHeight() + 10));
			ImGui::Text("Zoom (-/+):");
			ImGui::SameLine();
			ImGui::SliderFloat("##ZoomSlider", &sceneCamera->GetScale(), sceneCamera->GetMinScale(), sceneCamera->GetMaxScale(), "%.3f", ImGuiSliderFlags_Logarithmic);

			ImGui::SameLine();

			ImGui::Text("View:");
			ImGui::SameLine();
			ImGui::RadioButton("##Scene view - Both", &viewMode, 0);
			ImGui::SameLine();
			ImGui::RadioButton("##Scene view - World", &viewMode, 1);
			ImGui::SameLine();
			ImGui::RadioButton("##Scene view - UI", &viewMode, 2);

		}
		else {

			// Dibujar la imagen de fondo
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 uv0 = ImVec2(0, 1);  // Coordenada UV superior izquierda (0,1)
			ImVec2 uv1 = ImVec2(1, 0);  // Coordenada UV inferior derecha (1,0)
			ImGui::GetWindowDrawList()->AddImage(
				(void*)foreground->getSDLTexture(),  // Reemplaza esto con el identificador de la textura
				windowPos,
				ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
				uv0,
				uv1
			);

			std::string text = "No scene loaded";

			// Cambiar el tamaño de la fuente para crear un texto grande
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
			ImGui::SetWindowFontScale(1.2f);

			// Calcular el centro de la ventana
			ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

			// Mover el cursor al centro de la ventana en el eje X
			ImGui::SetCursorPosX(center.x - ImGui::CalcTextSize(text.c_str()).x * 0.5f);

			// Mover el cursor al centro de la ventana en el eje Y
			ImGui::SetCursorPosY(center.y - ImGui::GetTextLineHeightWithSpacing() * 0.5f);

			// Mostrar el texto centrado
			ImGui::TextUnformatted(text.c_str());

			// Restaurar la fuente original después de terminar con esta ventana
			ImGui::PopFont();
			ImGui::SetWindowFontScale(1.0f);

		}


	}

	void Scene::ReceiveAssetDrop(Asset& asset)
	{
		std::string extension = asset.extension;

		if (extension == ".png" || extension == ".jpg") {


			Entity* entity = AddEntity(asset.relativePath);
			entity->SetName(asset.name);

			ImVec2 position = MousePositionInScene();

			entity->SetWorldPosition(position);
			selectedEntity = entity;
		}

		if (asset.isPrefab) {

			Entity* prefab = PrefabManager::GetPrefabById(asset.prefabId);

			//Create a copy of the prefab
			Entity* entity = new Entity(*prefab);
			if (entity->IsTransform()) {
				AddEntity(entity);

				AddEntityChildsToScene(entity);

				ImVec2 position = MousePositionInScene();

				entity->SetWorldPosition(position);
			}
			else {
				AddOverlay(entity);
				AddOverlayChildsToScene(entity);
			}

			selectedEntity = entity;
		}
	}

	ImVec2 Scene::MousePositionInScene() {

		ImVec2 mousepos = ImGui::GetMousePos();

		mousepos.x -= windowPosX;
		mousepos.y -= windowPosY;


		//Trasladar el origen de coordenadas de la esquina superior al centro
		mousepos.x -= windowWidth * 0.5f;
		mousepos.y -= windowHeight * 0.5f;

		ImVec2 cameraPosition = sceneCamera->GetPosition();
		mousepos.x -= cameraPosition.x;
		mousepos.y -= cameraPosition.y;

		float cameraScale = sceneCamera->GetScale();

		mousepos.x /= cameraScale;
		mousepos.y /= cameraScale;

		return mousepos;
	}

	ImVec2 Scene::MousePositionInUI()
	{
		ImVec2 mouse = ImGui::GetMousePos();

		mouse.x -= windowPosX;
		mouse.y -= windowPosY;


		int x, y, w, h;
		CalculateFrameRect(x, y, w, h);

		mouse.x -= x;
		mouse.y -= y;

		mouse.x *= sceneScale;
		mouse.y *= sceneScale;

		float invCameraScale = 1.0f / sceneCamera->GetScale();

		mouse.x *= invCameraScale;
		mouse.y *= invCameraScale;

		return mouse;
	}

	bool Scene::IsMouseHoveringEntity(Entity* entity) {

		auto mousePos = MousePositionInScene();

		auto entityPos = entity->GetAdjustedPosition();
		auto entitySize = entity->GetTextureSize();

		ImVec2 worldScale = entity->GetWorldScale();

		entitySize.x *= worldScale.x;
		entitySize.y *= worldScale.y;

		return mousePos.x > entityPos.x && mousePos.x < entityPos.x + entitySize.x &&
			mousePos.y > entityPos.y && mousePos.y < entityPos.y + entitySize.y;
	}

	bool Scene::CompareEntitiesRenderOrder(Entity* a, Entity* b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
	}

	void Scene::ResizeOverlayIfNeccesary()
	{
		auto& prefData = Preferences::GetData();

		if (uiTexture == nullptr || uiWidth != prefData.width || uiHeight != prefData.height) {

			if (uiTexture != nullptr)
				SDL_DestroyTexture(uiTexture);

			uiWidth = prefData.width;
			uiHeight = prefData.height;

			uiTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, uiWidth, uiHeight);
			SDL_SetTextureBlendMode(uiTexture, SDL_BLENDMODE_BLEND);
		}
	}

	void Scene::CalculateFrameRect(int& x, int& y, int& w, int& h)
	{
		ImVec2 position = sceneCamera->GetPosition();

		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

		float cameraScale = sceneCamera->GetScale();

		float width = gameSizeX * cameraScale;
		float height = gameSizeY * cameraScale;


		sceneCamera->CenterPosition(position.x, position.y);

		float invSceneScale = 1 / sceneScale;

		float w_ = width * invSceneScale;
		float h_ = height * invSceneScale;

		x = position.x - gameSizeX * 0.5f * cameraScale + scenePosition_x * cameraScale + (width - w_) * 0.5f;
		y = position.y - gameSizeY * 0.5f * cameraScale + scenePosition_y * cameraScale + (height - h_) * 0.5f;
		w = w_;
		h = h_;
	}

	bool Scene::PointInsideHorizontalSegment(int x, int y, int sx, int sy, int w, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > sx && x - ht< sx + w && y + ht > sy && y - ht < sy;
	}

	bool Scene::PointInsideVerticalSegment(int x, int y, int sx, int sy, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > sx && x - ht < sx && y + ht> sy && y - ht < sy + h;
	}

	bool Scene::PointInsideRect(int x, int y, int rx, int ry, int rw, int rh, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > rx && x - ht< rx + rw && y + ht > ry && y - ht < ry + rh;
	}

	void Scene::RenderRectangle(int x, int y, int w, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		SDL_Rect frameRect = { x, y, w, h };


		for (int i = 0; i < thickness; i++) //SDL no tiene soporte para cambiar el grosor de la linea
		{

			SDL_RenderDrawRect(renderer, &frameRect);

			frameRect.x--;
			frameRect.y--;
			frameRect.w += 2;
			frameRect.h += 2;
		}


	}

	void Scene::RenderHorizontalSegment(int x, int y, int w, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		x -= ht;
		y -= ht;

		w += ht * 2;

		for (int i = 0; i < ht * 2; i++)
		{

			SDL_RenderDrawLine(renderer, x, y + i, x + w, y + i);
		}
	}

	void Scene::RenderVerticalSegment(int x, int y, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		x -= ht;
		y -= ht;

		h += ht * 2;

		for (int i = 0; i < ht * 2; i++)
		{

			SDL_RenderDrawLine(renderer, x + i, y, x + i, y + h);
		}
	}


	std::string Scene::GetPath() {
		return scenePath;
	}

	nlohmann::ordered_json Scene::ToJson() {

		nlohmann::ordered_json j;

		j["name"] = sceneName;

		j["position"] = std::to_string(-scenePosition_x) + ", " + std::to_string(-scenePosition_y);
		j["scale"] = sceneScale;

		nlohmann::ordered_json entitiesJson = nlohmann::json::array();
		for (const auto& pair : entities) {
			if (pair.second->GetParent() == nullptr)
				entitiesJson.push_back(j.parse(pair.second->ToJson()));
		}

		j["objects"] = entitiesJson;

		nlohmann::ordered_json overlayEntitiesJson = nlohmann::json::array();
		for (const auto& o : overlays) {
			if (o->GetParent() == nullptr)
				overlayEntitiesJson.push_back(j.parse(o->ToJson()));
		}

		j["overlays"] = overlayEntitiesJson;

		return j;
	}

}
