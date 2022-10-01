#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace CFlat {
	class IBox;
}

using std::string;

class FlowManager
{
public:
	struct Script{

		CFlat::IBox* init;
		CFlat::IBox* update;
	};

private:


	const string fileExtension = ".cf";
	const string path = "./";

	std::vector<CFlat::IBox*> boxes;
	std::unordered_map <string, Script* > scripts;
public:

	FlowManager::Script* getBoxes(std::string file);
	FlowManager::Script* loadScript(string file);
};

