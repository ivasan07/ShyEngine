#pragma once
#include "Node.h"
#include <vector>
#include <WindowComponent.h>

namespace Scripting {


	class LineManager;

	class ScriptingManager : public WindowComponent
	{

	public:

		void handleInput(SDL_Event* event) override;
		void render() override;

	private:


		LineManager* lineManager;
		std::vector<Node*> nodes;
	};

}