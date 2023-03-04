#include "ScriptingManager.h"
#include "LineManager.h"
#include "Node.h"
#include "SDL.h"

void Scripting::ScriptingManager::handleInput(SDL_Event* event)
{
	SDL_Event e = *event;




}

void Scripting::ScriptingManager::render()
{
	lineManager->Render();


	for (Node* node : nodes) {
		node->Render();
	}
}
