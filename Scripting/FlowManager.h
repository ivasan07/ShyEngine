#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class IBox;

using std::string;
class FlowManager
{
private:

	const string fileExtension = ".cf";
	const string path = "./";

	std::unordered_map<string, IBox*> scripts;
	std::vector<IBox*> boxes;
public:
	void loadScript(string file);
	IBox* createScript(string file);
};

