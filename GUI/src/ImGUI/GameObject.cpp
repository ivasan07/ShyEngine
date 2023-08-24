#include "GameObject.h"

#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "nlohmann/json.hpp"
#include "ComponentInfo.h"
#include "Preferences.h"
#include "LogManager.h"
#include "Texture.h"
#include "Editor.h"
#include "Camera.h"
#include "Scene.h"
#include "imgui.h"
#include "SDL.h"

#include <fstream>

#include "CheckML.h"

#define GizmoImage "gizmo.png"


namespace ShyEditor {

	int GameObject::lastId = 1;
	std::vector<int> GameObject::unusedIds = std::vector<int>();

	GameObject::GameObject(std::string& path, bool isTransform) : isTransform(isTransform) {

		editor = Editor::getInstance();

		name = path;

		// Transform attributes
		transform = nullptr;
		overlay = nullptr;

		texture_size = new ImVec2(0, 0);

		if (isTransform) {

			transform = new Transform(this);
		}
		else {

			overlay = new Overlay(this);
		}


		imagePath = path;

		parent = nullptr;

		showGizmo = false;
		visible = true;
		renderOrder = 0;

		waitingToDelete = false;

		previousMousePosX = 0;
		previousMousePosY = 0;

		// Gets the list of gameobjects in the scene
		std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();


		if (GameObject::unusedIds.size() != 0) {
			id = GameObject::unusedIds.back();
			GameObject::unusedIds.pop_back();
		}
		else {
			//Ensure id is not being used already
			while (gameObjects.find(GameObject::lastId) != gameObjects.end()) {
				GameObject::lastId++;
			}

			id = GameObject::lastId;
			GameObject::lastId++;
		}



		// Gizmo texture
		gizmo = ResourcesManager::GetInstance()->AddTexture(GizmoImage, true);

		// Gameobject texture
		texture = ResourcesManager::GetInstance()->AddTexture(path, false);

		if (texture->getSDLTexture() != NULL) {

			// Sets the size of the gameobject based on the texture width and height
			*texture_size = ImVec2(texture->getWidth(), texture->getHeight());

			// Add component image
			Components::Component imageComponent = Components::ComponentManager::GetAllComponents().find("Image")->second;
			Components::AttributeValue attributeValue;
			attributeValue.valueString = imagePath;
			imageComponent.getAttribute("fileName").SetValue(attributeValue);
			this->addComponent(imageComponent);

			// Set the gameobject name as the image name
			std::size_t extensionPos = path.find_last_of('.');
			name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;

		}

	}

	GameObject::GameObject(const GameObject& go)
	{
		editor = go.editor;

		name = go.name;

		// Gets the list of gameobjects in the scene
		std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();


		if (GameObject::unusedIds.size() != 0) {
			id = GameObject::unusedIds.back();
			GameObject::unusedIds.pop_back();
		}
		else {
			//Ensure id is not being used already
			while (gameObjects.find(GameObject::lastId) != gameObjects.end()) {
				GameObject::lastId++;
			}

			id = GameObject::lastId;
			GameObject::lastId++;
		}

		for (auto& pair : go.components) {
			Components::Component component = pair.second;

			components.emplace(component.GetName(), component);
		}

		for (auto& pair : go.scripts) {
			Components::Script script = pair.second;

			scripts.emplace(script.GetName(), script);
		}

		for (auto& pair : go.children) {
			GameObject* child = new GameObject(*pair.second);

			children.emplace(child->getId(), child);
		}

		if (go.parent != nullptr) {
			parent = new GameObject(*go.parent);
		}
		else {
			parent = nullptr;
		}

		visible = go.visible;
		showGizmo = go.showGizmo;
		renderOrder = go.renderOrder;

		isTransform = go.isTransform;

		if (isTransform) {
			transform = new Transform(*go.transform, this);
		}
		else {
			overlay = new Overlay(*go.overlay, this);
		}

		texture_size = new ImVec2(*go.texture_size);

		imagePath = go.imagePath;

		texture = go.texture;

		gizmo = go.gizmo;

		leftMouseButtonDown = go.leftMouseButtonDown;
		rightMouseButtonDown = go.rightMouseButtonDown;
		previousMousePosX = go.previousMousePosX;
		previousMousePosY = go.previousMousePosY;
		waitingToDelete = go.waitingToDelete;
	}

	GameObject::~GameObject() {

		if (parent != nullptr)
			parent->removeChild(this);

		parent = nullptr;

		for (auto child : children)
			child.second->setParent(nullptr);

		components.clear();

		delete texture_size;

		if (isTransform)
			delete transform;
		else
			delete overlay;
	}





	// ------------------------------------- Render, update and input -----------------------------------

	void GameObject::RenderTransform(SDL_Renderer* renderer, Camera* camera) {

		ImVec2 position = getAdjustedPosition();

		ImVec2 cameraPosition = camera->GetPosition();
		float cameraScale = camera->GetScale();

		float width = texture_size->x * getScale_x() * cameraScale;
		float height = texture_size->y * getScale_y() * cameraScale;


		ImVec2 worldPosition = ImVec2(position.x * cameraScale + cameraPosition.x, position.y * cameraScale + cameraPosition.y);

		camera->CenterPosition(worldPosition.x, worldPosition.y);

		SDL_Rect dst = { worldPosition.x, worldPosition.y, width, height };

		// Image render
		if (visible && texture != NULL)
			SDL_RenderCopyEx(renderer, texture->getSDLTexture(), NULL, &dst, transform->GetRotation(), NULL, SDL_FLIP_NONE);






		// Render outline
		if (this == editor->getScene()->GetSelectedGameObject()) {

			return;
			// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			SDL_Rect outlineRect = { worldPosition.x, worldPosition.y, width, height };
			int thickness = 3;

			for (int i = 0; i < thickness; i++) {
				SDL_Rect currentRect = { outlineRect.x - i, outlineRect.y - i, outlineRect.w + i * 2, outlineRect.h + i * 2 };
				SDL_RenderDrawRect(renderer, &currentRect);
			}

			if (showGizmo) {
				dst.x += dst.w / 2;
				dst.y -= dst.h / 2;

				SDL_RenderCopy(renderer, gizmo->getSDLTexture(), NULL, &dst);
			}
		}
	}

	void GameObject::update() {

		if (isTransform) {

			auto image = components.find("Image");

			// If the gameobject has no image component
			if (image == components.end()) {
				texture = nullptr;
				return;
			}

			std::string currentImagePath = (image->second).getAttribute("fileName").value.valueString;

			// Checks if the current path exists in the filesystem
			//if (!std::filesystem::exists(currentImagePath)) return;

			// If the image path have changed, checks if the new path is valid
			if (currentImagePath != imagePath) {

				texture = ResourcesManager::GetInstance()->AddTexture(currentImagePath, false);

				if (texture->getSDLTexture() != NULL) {
					imagePath = currentImagePath;
					*texture_size = ImVec2(texture->getWidth(), texture->getHeight());
				}
				else {
					*texture_size = ImVec2(100, 100);
					imagePath = "";
				}
			}
		}

		else {


			overlay->Update();

		}

	}

	bool GameObject::handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos) {


		return false;
	}





	// ----------------------------- Name, ID and texture getters/setters ---------------------------

	std::string GameObject::getName() {
		return name;
	}

	void GameObject::setName(const std::string& newName) {
		name = newName;
	}

	Texture* GameObject::getTexture() {
		return texture;
	}

	int GameObject::getId() {
		return id;
	}

	void GameObject::setId(int id)
	{
		this->id = id;
	}






	// ----------------------------------- Visibility getters/setters -----------------------------------

	int GameObject::getRenderOrder() {
		return renderOrder;
	}

	bool GameObject::isVisible() {
		return visible;
	}

	void GameObject::setVisible(bool visible) {
		if (this->parent == nullptr) {
			this->visible = visible;
			setChildrenVisible(this, visible);
		}
	}






	// ----------------------------------- Components and Scripts logic --------------------------------------

	void GameObject::addComponent(Components::Component comp) {

		if (components.find(comp.GetName()) == components.end())
			components.insert({ comp.GetName(), comp });

	}

	void GameObject::addScript(Components::Script script) {

		if (scripts.contains(script.GetName())) return;

		scripts.emplace(script.GetName(), script);
	}

	std::unordered_map<std::string, Components::Component>* GameObject::getComponents() {
		return &components;
	}

	std::unordered_map<std::string, Components::Script>* GameObject::getScripts() {
		return &scripts;
	}





	// --------------------------------- Tranform attributes getters/setters -----------------------------------

	void GameObject::setPosition(ImVec2 newPos) {

		transform->SetPosition(newPos.x, newPos.y);
	}

	ImVec2 GameObject::getPosition() {
		return transform->GetPosition();
	}

	float GameObject::getRotation() {
		return transform->GetRotation();
	}

	void GameObject::SetRotation(float r)
	{
		transform->SetRotation(r);
	}

	ImVec2 GameObject::getAdjustedPosition() {

		ImVec2 position = transform->GetPosition();

		float width = texture_size->x * getScale_x();
		float height = texture_size->y * getScale_y();

		// The game objects have their origin at the center
		position.x -= width * 0.5f;
		position.y -= height * 0.5f;

		return position;
	}

	ImVec2 GameObject::getSize() {
		return *texture_size;
	}

	float GameObject::getScale_x() {
		return transform->GetScale().x;
	}

	float GameObject::getScale_y() {
		return transform->GetScale().y;
	}

	void GameObject::SetScale(float x, float y)
	{
		transform->SetScale(x, y);
	}






	// --------------------------------------- Deleting gameobject logic ---------------------------------------

	bool GameObject::isWaitingToDelete() {
		return waitingToDelete;
	}

	void GameObject::toDelete() {
		waitingToDelete = true;

		GameObject::unusedIds.push_back(id);

		for (auto& child : children) {
			child.second->toDelete();
		}
	}





	// --------------------------------- Gameobject children and parent logic ------------------------------------

	void GameObject::setParent(GameObject* go) {
		parent = go;
	}

	GameObject* GameObject::getParent() {
		return parent;
	}

	void GameObject::removeChild(GameObject* go) {
		go->setParent(nullptr);
		children.erase(go->getId());
	}

	void GameObject::addChild(GameObject* go) {
		go->setParent(this);
		children.insert({ go->getId(), go });
	}

	std::unordered_map<int, GameObject*> GameObject::getChildren() {
		return children;
	}

	bool GameObject::isAscendant(GameObject* go) {

		for (auto childPair : children) {
			GameObject* child = childPair.second;

			if (child == go || child->isAscendant(go))
				return true;
		}

		return false;
	}






	// ---------------------------------------- Gameobject drawing logic ----------------------------------------------

	void GameObject::drawTransformInEditor() {

		ImGui::Text("Position");
		ImGui::DragFloat2("##position_drag", (float*)&transform->GetPosition(), 0.3f, 0.0f, 0.0f, "%.2f");

		ImGui::Text("Scale");
		ImGui::DragFloat2("##scale_drag", (float*)&transform->GetScale(), 0.02f, 0.0f, FLT_MAX, "%.2f");

		ImGui::Text("Rotation");
		ImGui::DragFloat("##rotation_drag", &transform->GetRotation(), 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::Text("Render order");
		ImGui::InputInt("##render_order", &renderOrder);
	}

	void GameObject::drawOverlayInEditor()
	{
		int& placement = overlay->GetPlacement();
		ImGui::Combo("Placement mode", &placement,

			"Positioned\0Streched"
		);


		if (placement == 0) {


			ImGui::Text("Position");
			ImGui::DragFloat2("##Overlayposition_drag", (float*)&overlay->GetPosition(), 0.3f, 0.0f, 0.0f, "%.2f");


			ImGui::Text("Size");
			ImGui::DragFloat2("##OverlaySize_drag", (float*)&overlay->GetSize(), 0.3f, 0.0f, 0.0f, "%.2f");


		}
		else {

			ImGui::Text("Left");
			ImGui::DragInt("##OverlayLeft", &overlay->GetLeft(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Top");
			ImGui::DragInt("##OverlayTop", &overlay->GetTop(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Right");
			ImGui::DragInt("##OverlayRight", &overlay->GetRight(), 0.3f, 0.0f, 0.0f, "%d");

			ImGui::Text("Bottom");
			ImGui::DragInt("##OverlayBottom", &overlay->GetBottom(), 0.3f, 0.0f, 0.0f, "%d");
		}


		ImGui::SeparatorText("Anchor");

		ImGui::Text("Anchor");
		ImGui::DragFloat2("##Anchor_drag", (float*)&overlay->GetAnchor(), 0.3f, 0.0f, 0.0f, "%.2f");

		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, -1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, 1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, -1));

		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, 0));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, 0));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, 0));

		DrawArrowButton(overlay->GetAnchor(), ImVec2(-1, 1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(0, -1));
		ImGui::SameLine();
		DrawArrowButton(overlay->GetAnchor(), ImVec2(1, 1));
	}

	void GameObject::drawComponentsInEditor() {

		for (auto it = components.begin(); it != components.end();) {
			std::string componentName = (*it).second.GetName();


			if (ImGui::CollapsingHeader(componentName.c_str())) {

				for (auto& attribute : (*it).second.getAllAttributes()) {
					std::string attributeName = attribute.first;
					Components::Attribute* attr = &attribute.second;

					ImGui::Text(attributeName.c_str());

					switch (attr->getType()) {

					case Components::AttributesType::FLOAT:
						drawFloat(attributeName + it->first, attr);
						break;
					case Components::AttributesType::VECTOR2:
						drawVector2(attributeName + it->first, attr);
						break;
					case Components::AttributesType::STRING:
						drawString(attributeName + it->first, attr);
						break;
					case Components::AttributesType::BOOL:
						drawBool(attributeName + it->first, attr);
						break;
					case Components::AttributesType::COLOR:
						drawColor(attributeName + it->first, attr);
						break;
					case Components::AttributesType::CHAR:
						drawChar(attributeName + it->first, attr);
						break;
					case Components::AttributesType::GAMEOBJECT:
						drawGameobject(attributeName + it->first, attr);
						break;
					default:
						break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1.0f));

				if (ImGui::Button(("Delete component##" + componentName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40)))
					it = components.erase(it);
				else
					++it;

				ImGui::PopStyleColor(2);

			}
			else ++it;
		}
	}

	void GameObject::drawScriptsInEditor() {

		for (auto it = scripts.begin(); it != scripts.end();) {

			std::string scriptName = (*it).first;
			if (ImGui::CollapsingHeader(scriptName.c_str())) {

				for (auto& attribute : (*it).second.getAllAttributes()) {
					std::string attributeName = attribute.first;
					Components::Attribute* attr = &attribute.second;

					ImGui::Text(attributeName.c_str());

					switch (attr->getType()) {

					case Components::AttributesType::FLOAT:
						drawFloat(attributeName + it->first, attr);
						break;
					case Components::AttributesType::VECTOR2:
						drawVector2(attributeName + it->first, attr);
						break;
					case Components::AttributesType::STRING:
						drawString(attributeName + it->first, attr);
						break;
					case Components::AttributesType::BOOL:
						drawBool(attributeName + it->first, attr);
						break;
					case Components::AttributesType::COLOR:
						drawColor(attributeName + it->first, attr);
						break;
					case Components::AttributesType::CHAR:
						drawChar(attributeName + it->first, attr);
						break;
					case Components::AttributesType::GAMEOBJECT:
						drawGameobject(attributeName + it->first, attr);
						break;
					default:
						break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 255.0f, 255.0f, 1.0f));

				if (ImGui::Button(("Edit script##" + scriptName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40)))
					editor->OpenScript(scriptName);

				if (ImGui::Button(("Delete script##" + scriptName).c_str(), ImVec2(ImGui::GetColumnWidth(), 40)))
					it = scripts.erase(it);
				else
					++it;

				ImGui::PopStyleColor(2);

			}
			else ++it;
		}

	}

	void GameObject::drawFloat(std::string attrName, ::Components::Attribute* attr) {

		ImGui::DragFloat(("##" + attrName).c_str(), &attr->value.value.valueFloat, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	void GameObject::drawVector2(std::string attrName, ::Components::Attribute* attr) {

		ImGui::DragFloat2(("##" + attrName).c_str(), (float*)&attr->value.value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	void GameObject::drawString(std::string attrName, ::Components::Attribute* attr) {

		char inputBuffer[256];
		strncpy_s(inputBuffer, attr->value.valueString.c_str(), sizeof(inputBuffer));

		if (ImGui::InputText(("##" + attrName).c_str(), inputBuffer, sizeof(inputBuffer)))
			attr->value.valueString = inputBuffer;

	}

	void GameObject::drawBool(std::string attrName, ::Components::Attribute* attr) {

		ImGui::Checkbox(("##" + attrName).c_str(), &attr->value.value.valueBool);
	}

	void GameObject::drawColor(std::string attrName, ::Components::Attribute* attr) {

		ImGui::ColorEdit3(("##" + attrName).c_str(), (float*)&attr->value.value.valueColor);
	}

	void GameObject::drawChar(std::string attrName, ::Components::Attribute* attr) {

		if (ImGui::InputText(("##" + attrName).c_str(), &attr->value.value.valueChar, 2, ImGuiInputTextFlags_CharsNoBlank)) {

		}
	}

	void GameObject::drawGameobject(std::string attrName, ::Components::Attribute* attr) {

		std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();

		GameObject* go = gameObjects.find((int)attr->value.value.entityIdx) != gameObjects.end() ? gameObjects.find((int)attr->value.value.entityIdx)->second : nullptr;

		if (ImGui::BeginCombo(("##" + attrName).c_str(), go != nullptr ? go->getName().c_str() : "")) {
			for (auto go : gameObjects) {
				if (ImGui::Selectable(go.second->getName().c_str()))
					attr->value.value.entityIdx = go.second->getId();
			}

			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Button(("X##" + attrName).c_str())) {
			attr->value.value.entityIdx = 0;
		}
	}

	void GameObject::DrawArrowButton(ImVec2& value, const ImVec2& dir)
	{

		if (!(dir.x == dir.y && dir.x == 0))
		{
			//Vertical arrows
			if (dir.x == 0)
			{
				if (dir.y < 0) {
					if (ImGui::ArrowButton("##Down", ImGuiDir_Down))
						value = ImVec2(0.5f, 1);
				}
				else
					if (ImGui::ArrowButton("##Up", ImGuiDir_Up))
						value = ImVec2(0.5f, 0);
				return;
			}

			//Horizontal arrows
			if (dir.y == 0) {

				if (dir.x < 0) {
					if (ImGui::ArrowButton("##Left", ImGuiDir_Left))
						value = ImVec2(0, 0.5f);
				}
				else
					if (ImGui::ArrowButton("##Right", ImGuiDir_Right))
						value = ImVec2(1, 0.5f);
				return;
			}
		}

		ImGui::PushID((void*)&dir);

		int size = ImGui::GetFrameHeight();

		if (ImGui::Button("##", ImVec2(size, size))) {

			value = ImVec2(dir.x * 0.5f + 0.5f, dir.y * 0.5f + 0.5f);
		}

		auto drawList = ImGui::GetWindowDrawList();

		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();



		//Diagonal arrows
		ImVec2 a = min;
		ImVec2 b = ImVec2(max.x, min.y);
		ImVec2 c = max;
		ImVec2 d = ImVec2(min.x, max.y);


		size *= 0.25f;

		a.x += size;
		a.y += size;

		b.x -= size;
		b.y += size;

		c.x -= size;
		c.y -= size;

		d.x += size;
		d.y -= size;

		if (dir.x == dir.y && dir.x == 0) {

			drawList->AddQuadFilled(a, b, c, d, IM_COL32(255, 255, 255, 255));
			ImGui::PopID();
			return;
		}



		ImVec2 p1, p2, p3;


		if (dir.x < 0)
		{
			if (dir.y < 0)
			{
				p1 = a;
				p2 = b;
				p3 = d;
			}
			else {
				p1 = a;
				p2 = c;
				p3 = d;
			}
		}
		else {

			if (dir.y < 0)
			{
				p1 = a;
				p2 = b;
				p3 = c;
			}
			else {
				p1 = b;
				p2 = c;
				p3 = d;
			}
		}



		drawList->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));

		ImGui::PopID();
	}




	// ---------------------- Gameobject children settings (Transform and visibility) ----------------------

	void GameObject::translateChildren(GameObject* go, ImVec2* previousPos) {

		ImVec2 parentPreviousPos = { previousPos->x, previousPos->y };

		for (auto childPair : go->getChildren()) {

			ImVec2 childPos = childPair.second->getPosition();

			float xDiff = go->getPosition().x - parentPreviousPos.x;
			float yDiff = go->getPosition().y - parentPreviousPos.y;

			previousPos->x = childPos.x;
			previousPos->y = childPos.y;

			childPair.second->setPosition(ImVec2(childPos.x + xDiff, childPos.y + yDiff));

			translateChildren(childPair.second, previousPos);
		}
	}

	void GameObject::scaleChildren(GameObject* go, int scaleFactor) {

		for (auto& childPair : go->getChildren()) {

			childPair.second->transform->GetScale().x += scaleFactor;
			childPair.second->transform->GetScale().y += scaleFactor;

			scaleChildren(childPair.second, scaleFactor);
		}
	}

	void GameObject::setChildrenVisible(GameObject* go, bool visible) {

		for (auto childPair : go->getChildren()) {

			childPair.second->visible = visible;

			setChildrenVisible(childPair.second, visible);
		}
	}

	void GameObject::rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle) {

		float angleRadians = rotationAngle * (3.14159265359f / 180.0f);

		for (auto& child : go->getChildren())
		{
			auto& childPos = child.second->transform->GetPosition();

			float newX = cos(angleRadians) * (childPos.x - goCenter->transform->GetPosition().x) - sin(angleRadians) * (childPos.y - goCenter->transform->GetPosition().y) + goCenter->transform->GetPosition().x;
			float newY = sin(angleRadians) * (childPos.x - goCenter->transform->GetPosition().x) + cos(angleRadians) * (childPos.y - goCenter->transform->GetPosition().y) + goCenter->transform->GetPosition().y;

			child.second->transform->GetPosition().x = newX;
			child.second->transform->GetPosition().y = newY;
			child.second->transform->GetRotation() += rotationAngle;

			rotateChildren(child.second, goCenter, rotationAngle);
		}
	}




	// ------------------------ Serialization and deseralization logic -------------------------

	std::string GameObject::toJson() {

		nlohmann::ordered_json j;
		j["name"] = name;

		nlohmann::ordered_json childsJson = nlohmann::json::array();
		for (auto childPair : children) {
			auto child = j.parse(childPair.second->toJson());

			childsJson.push_back(child);
		}

		j["id"] = id;
		
		j["childs"] = childsJson;

		j["order"] = renderOrder;

		j["localPosition"] = std::to_string(transform->GetPosition().x) + ", " + std::to_string(transform->GetPosition().y);
		j["localScale"] = std::to_string(transform->GetScale().x) + ", " + std::to_string(transform->GetScale().y);
		j["localRotation"] = std::to_string(transform->GetRotation());

		nlohmann::ordered_json componentsJson = nlohmann::json::array();
		for (auto it = components.begin(); it != components.end(); it++) {

			auto comp = j.parse(it->second.toJson());

			componentsJson.push_back(comp);
		}

		j["components"] = componentsJson;


		nlohmann::ordered_json scriptsJson = nlohmann::json();
		for (auto it = scripts.begin(); it != scripts.end(); it++) {
			auto scriptJson = j.parse(it->second.ToJson());

			if (scriptJson.is_null())
				scriptJson = nlohmann::json::array();

			scriptsJson[it->second.GetName()] = scriptJson;
		}

		j["scripts"] = scriptsJson;


		return j.dump(2);
	}

	GameObject* GameObject::fromJson(std::string json, bool isTransform = true) {

		nlohmann::ordered_json jsonData;
		jsonData = nlohmann::json::parse(json);

		std::string errorMsg = "The JSON gameobject has not the correct format.";

		if (!jsonData.contains("name")) {
			LogManager::LogError(errorMsg);
			return nullptr;
		}

		std::string goName = jsonData["name"];

		GameObject::unusedIds.push_back(jsonData["id"]);

		GameObject* gameObject = new GameObject(goName, isTransform);
		gameObject->name = goName;

		if (!jsonData.contains("id")) {
			LogManager::LogError(errorMsg);
			return nullptr;
		}

		if (!jsonData.contains("childs")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		for (const auto& childJson : jsonData["childs"]) {
			GameObject* child = GameObject::fromJson(childJson.dump());

			gameObject->addChild(child);
			child->setParent(gameObject);
		}


		if (!jsonData.contains("order")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		gameObject->renderOrder = jsonData["order"];



		if (!jsonData.contains("localPosition")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		if (!jsonData.contains("localScale")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		if (!jsonData.contains("localRotation")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		// Deserialize localPosition, localScale, and localRotation
		std::string localPositionStr = jsonData["localPosition"];
		std::string localScaleStr = jsonData["localScale"];
		std::string localRotation = jsonData["localRotation"];

		// Parse localPosition and localScale
		sscanf_s(localPositionStr.c_str(), "%f, %f", &gameObject->transform->GetPosition().x, &gameObject->transform->GetPosition().y);
		sscanf_s(localScaleStr.c_str(), "%f, %f", &gameObject->transform->GetScale().x, &gameObject->transform->GetScale().y);

		gameObject->transform->SetRotation(std::stof(localRotation));


		// Components
		if (!jsonData.contains("components")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		for (const auto& compJson : jsonData["components"]) {
			Components::Component component = Components::Component::fromJson(compJson.dump());
			gameObject->addComponent(component);
		}

		// Scripts
		if (!jsonData.contains("scripts")) {
			LogManager::LogError(errorMsg);
			return gameObject;
		}

		for (const auto& scriptJson : jsonData["scripts"].items()) {
			Components::Script script = Components::Script::fromJson(scriptJson.key(), scriptJson.value().dump());
			gameObject->addScript(script);
		}

		return gameObject;
	}

	bool GameObject::IsTransform()
	{
		return isTransform;
	}

	Overlay* GameObject::GetOverlay()
	{
		return overlay;
	}



	//============ COMPONENTS =================================================================================================================================================================

	Transform::Transform(GameObject* obj) : obj(obj)
	{
		scale = new ImVec2(1, 1);
		position = new ImVec2(0, 0);
		rotation = 0;
	}

	Transform::Transform(const Transform& tr, GameObject* obj)
	{
		this->obj = obj;

		scale = new ImVec2(*tr.scale);
		position = new ImVec2(*tr.position);
		rotation = tr.rotation;
	}

	Transform::~Transform()
	{
		delete scale;
		delete position;

		scale = nullptr;
		position = nullptr;
	}

	ImVec2& Transform::GetPosition()
	{
		return *position;
	}

	ImVec2& Transform::GetScale()
	{
		return *scale;
	}

	float& Transform::GetRotation()
	{
		return rotation;
	}

	void Transform::SetPosition(float x, float y)
	{
		position->x = x;
		position->y = y;
	}

	void Transform::SetScale(float x, float y)
	{
		scale->x = x;
		scale->y = y;
	}

	void Transform::SetRotation(float r)
	{
		rotation = r;
	}




	//===== Overlays=====================================
	Overlay::Overlay(GameObject* obj) : obj(obj)
	{
		placement = 0;

		position = new ImVec2(0, 0);
		anchor = new ImVec2(0.5f, 0.5f);
		size = new ImVec2(100, 100);
		scale = 1;

		left = top = right = bottom = 0;

		image = new OverlayImage();
		text = nullptr;
	}

	Overlay::Overlay(const Overlay& ov, GameObject* obj)
	{
		this->obj = obj;

		placement = ov.placement;
		position = new ImVec2(*ov.position);
		size = new ImVec2(*ov.size);

		anchor = new ImVec2(*ov.anchor);

		left = ov.left;
		top = ov.top;
		right = ov.right;
		bottom = ov.bottom;

		scale = ov.scale;
		interactable = ov.interactable;

		image = new OverlayImage();
		text = nullptr;
	}

	Overlay::~Overlay()
	{
		delete anchor;
		delete size;
		delete position;
		delete image;
	}

	int& Overlay::GetPlacement()
	{
		return placement;
	}

	ImVec2& Overlay::GetAnchor()
	{
		return *anchor;
	}

	float& Overlay::GetScale()
	{
		return scale;
	}

	bool& Overlay::GetInteractable()
	{
		return interactable;
	}

	ImVec2& Overlay::GetPosition()
	{
		return *position;
	}

	ImVec2& Overlay::GetSize()
	{
		return *size;
	}

	int& Overlay::GetLeft()
	{
		return left;
	}

	int& Overlay::GetRight()
	{
		return right;
	}

	int& Overlay::GetTop()
	{
		return top;
	}

	int& Overlay::GetBottom()
	{
		return bottom;
	}

	void Overlay::CalculateRectangle(int& x, int& y, int& w, int& h)
	{
		if (placement == 0) {

			x = (int)std::round(position->x);
			y = (int)std::round(position->y);
			w = (int)std::round(size->x);
			h = (int)std::round(size->y);

			x -= anchor->x * size->x;
			y -= anchor->y * size->y;

			auto parent = obj->getParent();

			if (parent != nullptr) {

				parent->GetOverlay();

				auto parentPos = parent->GetOverlay()->CalculateCenterPoint();
				x += parentPos.x;
				y += parentPos.y;
			}

		}
		else {


			int parent_left, parent_top, parent_width, parent_height;
			auto parent = obj->getParent();


			if (parent != nullptr) {

				parent->GetOverlay()->CalculateRectangle(parent_left, parent_top, parent_width, parent_height);
			}
			else {

				parent_left = 0;
				parent_top = 0;
				parent_width = Preferences::GetData().width;
				parent_height = Preferences::GetData().height;
			}

			x = parent_left + left;
			y = parent_top + top;
			w = parent_width - right - left;
			h = parent_height - bottom - top;
		}

	}

	ImVec2 Overlay::CalculateCenterPoint()
	{
		ImVec2 center;
		if (placement == 0) {

			center = *position;

			auto parent = obj->getParent();

			if (parent != nullptr)
			{
				auto parentCenter = parent->GetOverlay()->CalculateCenterPoint();

				center.x += parentCenter.x;
				center.y += parentCenter.y;
			}

		}
		else {

			int x, y, w, h;
			CalculateRectangle(x, y, w, h);

			center = { x + w * anchor->x, y + h * anchor->y };

		}

		return center;
	}

	void Overlay::Update()
	{

		auto components = obj->getComponents();

		auto imgIt = components->find("OverlayImage");

		if (imgIt != components->end()) {

			auto& imgCmp = *imgIt;
			
			const auto& cmpPath = imgCmp.second.getAttribute("path").value.valueString;

			if (cmpPath != image->GetPath()) {


				Texture* texture = ResourcesManager::GetInstance()->AddTexture(cmpPath, false);

				if (texture->getSDLTexture() == nullptr) {

					texture = nullptr;
				}

				image->SetTexture(cmpPath, texture);
			}

		}
		else {

			image->SetTexture("", nullptr);
		}





		//TODO: text
	}

	void Overlay::Render(SDL_Renderer* renderer, int x, int y, int w, int h)
	{

		if (image != nullptr)
			image->Render(renderer, x, y, w, h);

		//TODO: render text
	}



	OverlayImage::OverlayImage()
	{
		texture = nullptr;
		path = "";
	}

	void OverlayImage::Render(SDL_Renderer* renderer, int x, int y, int w, int h)
	{

		if (texture != nullptr) {

			SDL_Rect dest{ x, y, w, h };
			SDL_RenderCopy(renderer, texture->getSDLTexture(), NULL, &dest);
		}
	}
	std::string OverlayImage::GetPath()
	{
		return path;
	}
	void OverlayImage::SetTexture(std::string path, Texture* texture)
	{
		this->path = path;
		this->texture = texture;
	}
}


