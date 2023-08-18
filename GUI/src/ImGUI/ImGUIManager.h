#pragma once
#include <vector>
#include <string>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;
struct ImVec2;

namespace PEditor {
	class Window;
	class Scene;
	class MenuBar;
	class Hierarchy;
	class FileExplorer;
	class ComponentWindow;
	class ScriptCreation;
	class ProjectsManager;
	class Console;
	struct ProjectInfo;
};

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

#define _ProjectSelectionDialogueSize 1080, 720
#define _WindowMainSize 1920, 1080

class ImGUIManager
{

public:

	enum EDITOR_STATE { EDITOR_WINDOW, SCRIPTING_WINDOW };

private:

	static ImGUIManager* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Surface* winSurface = nullptr;
	SDL_Renderer* renderer = nullptr;

	// Project info structure 
	PEditor::ProjectInfo* projecInfo;

	bool exitGame = false;
	EDITOR_STATE state;

	// All editor windows
	std::vector<PEditor::Window*> windows;

	void initImGUI();
	void initSDL();
	void initWindows();
	void createSDLWindow(const char* name, int posX, int posY, int sizeX, int sizeY);
	void createSDLRenderer();

	PEditor::Scene* scene = nullptr;
	PEditor::MenuBar* menuBar = nullptr;
	PEditor::Hierarchy* hierarchy = nullptr;
	PEditor::FileExplorer* fileExplorer = nullptr;
	PEditor::ComponentWindow* components = nullptr;
	PEditor::ScriptCreation* scriptCreation = nullptr;
	PEditor::Console* console = nullptr;

	void SplashScreen();

public:

	static ImGUIManager* getInstance();

	ImGUIManager();

	void init();
	void loop();
	void exit();

	void update();
	void render();
	void handleInput();

	void addWindow(PEditor::Window* window);
	void setScene(PEditor::Scene* scene);

	void changeEditorState(const EDITOR_STATE& state);
	void setProjectInfo(PEditor::ProjectInfo* pInfo);

	SDL_Renderer* getRenderer();
	ImVec2 getMainWindowSize();

	PEditor::Scene* getScene();
	PEditor::MenuBar* getMenuBar();
	PEditor::Hierarchy* getHierarchy();
	PEditor::FileExplorer* getFileExplorer();
	PEditor::ComponentWindow* getComponents();
	PEditor::ScriptCreation* getScriptCreation();

	void OpenScript(const std::string& script);

	~ImGUIManager();
};
