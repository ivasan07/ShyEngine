#pragma once
#include <iostream>
#include <list>
#include <unordered_map>

struct SDL_Renderer;
union SDL_Event;
struct ImVec2;
struct ImColor;
class Editor;

namespace Components {
	class Component;
	class Attribute;
	class Script;
}

namespace ShyEditor {

	class Camera;
	class Texture;


	class Transform;
	class Overlay;

	class GameObject {

	public:

		GameObject(std::string& path, bool isTransform);
		~GameObject();

		// Render, update and input
		void RenderTransform(SDL_Renderer* renderer, Camera* camera);
		void update();
		bool handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		// Name, ID and texture getters/setters
		std::string getName();
		void setName(const std::string& newName);
		Texture* getTexture();
		int getId();

		// Visibility getters/setters
		int getRenderOrder();
		bool isVisible();
		void setVisible(bool visible);

		// Components and Scripts logic
		void addComponent(Components::Component comp);
		void addScript(Components::Script script);
		std::unordered_map<std::string, Components::Component>* getComponents();
		std::unordered_map<std::string, Components::Script>* getScripts();

		// Tranform attributes getters/setters
		void setPosition(ImVec2 newPos);
		ImVec2 getPosition();
		float getRotation();
		ImVec2 getAdjustedPosition();
		ImVec2 getSize();
		float getScale_x();
		float getScale_y();

		// Deleting gameobject logic
		bool isWaitingToDelete();
		void toDelete();

		// Gameobject children and parent logic
		void setParent(GameObject* go);
		GameObject* getParent();
		void removeChild(GameObject* go);
		void addChild(GameObject* go);
		std::unordered_map<int, GameObject*> getChildren();
		bool isAscendant(GameObject* go);

		// Gameobject transform, components and scripts drawing
		void drawTransformInEditor();
		void drawOverlayInEditor();
		void drawComponentsInEditor();
		void drawScriptsInEditor();

		// Serialization and deseralization logic
		std::string toJson(bool isPrefab = false);
		static GameObject* fromJson(std::string json, bool isPrefab = false);

		bool IsTransform();

		Overlay* GetOverlay();

	private:

		// Static variable to store the last assigned id (Needed for ids assingment)
		static int lastId;

		// Gameobject name
		std::string name;

		// Gameobject id
		int id;

		// Gameobject components, scripts, children and parent
		std::unordered_map<std::string, ::Components::Component> components;
		std::unordered_map<std::string, Components::Script> scripts;
		std::unordered_map<int, GameObject*> children;
		GameObject* parent;

		// A reference to the editor singleton
		Editor* editor;

		// Gameobject visibility
		bool visible;
		bool showGizmo;
		int renderOrder;


		//Positional components
		bool isTransform;
		Transform* transform;
		Overlay* overlay;

		ImVec2* texture_size;


		// Gameobject texture and gizmo, path to the image and image component
		Components::Component* imageComponent;
		std::string imagePath;
		Texture* texture;
		Texture* gizmo;

		// Some other needed information (Input and deleting gameobject logic)
		bool leftMouseButtonDown;
		bool rightMouseButtonDown;
		float previousMousePosX;
		float previousMousePosY;
		bool waitingToDelete;

		// Gameobject draw methods
		void drawFloat(std::string attrName, Components::Attribute* attr);
		void drawVector2(std::string attrName, Components::Attribute* attr);
		void drawString(std::string attrName, Components::Attribute* attr);
		void drawBool(std::string attrName, Components::Attribute* attr);
		void drawColor(std::string attrName, Components::Attribute* attr);
		void drawChar(std::string attrName, Components::Attribute* attr);
		void drawGameobject(std::string attrName, Components::Attribute* attr);

		void DrawArrowButton(ImVec2& value, const ImVec2& dir);

		// Gameobject children settings (Transform and visibility)
		void translateChildren(GameObject* go, ImVec2* previousPos);
		void scaleChildren(GameObject* go, int scaleFactor);
		void setChildrenVisible(GameObject* go, bool visible);
		void rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle);

	};



//Components


	class Transform {

		GameObject* obj;

		// Transform attributes
		ImVec2* scale;
		ImVec2* position;
		float rotation;

	public:
		Transform(GameObject* obj);
		~Transform();
		
		ImVec2 &GetPosition();
		ImVec2 &GetScale();
		float &GetRotation();

		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetRotation(float r);
	};



	class Overlay {

		GameObject* obj;


		int placement;

		ImVec2* position;
		ImVec2* size;

		ImVec2* anchor;
		int left;
		int top;
		int right;
		int bottom;

		//ImColor* color;

		float scale;
		bool interactable;


	public:

		enum class Placement {

			Positioned, Stretched
		};

		Overlay(GameObject* obj);
		~Overlay();

		int& GetPlacement();

		ImVec2& GetAnchor();

		float& GetScale();

		bool& GetInteractable();

		ImVec2& GetPosition();
		ImVec2& GetSize();


		int& GetLeft();
		int& GetRight();
		int& GetTop();
		int& GetBottom();



		void CalculateRectangle(int& x, int& y, int& w, int& h);
		ImVec2 CalculateCenterPoint();
	};
}