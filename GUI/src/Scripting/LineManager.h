#pragma once

#include <vector>

namespace Scripting {


	class Node;
	class LineManager {

	public:

		void Render();

		void CreateInputLine(Node* input, Node* current, int inputIdx);
		void CreateFollowLine(Node* current, Node* next);


	private:

		std::vector<std::pair<Node*, Node*>> lines;
	};

}