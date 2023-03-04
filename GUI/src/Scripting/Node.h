#pragma once

#include <string>
#include <vector>

struct ImVec2;
class Image;

namespace Scripting {


	class Node {


	public:

		void Move(ImVec2);
		void Render();

	private:


		Image* image;
		std::vector<int> input;
	};
}