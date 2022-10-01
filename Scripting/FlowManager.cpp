#include "FlowManager.h"
#include <fstream>
#include "json.hpp"
#include <iostream>
#include "CFlatBasics.h"

using nlohmann::json;
using std::string;
using jsonarray = std::vector<json>;



FlowManager::Script* FlowManager::getBoxes(std::string file)
{
	return scripts.find(file)->second;
}

FlowManager::Script* FlowManager::loadScript(string file)
{
	if (scripts.find(file) != scripts.end()) {
		return getBoxes(file);
	}

	std::ifstream f(path + file + fileExtension);
	if (!f.good()) {

		std::cout << "El archivo no existe";
		return nullptr;
	}

	json data = json::parse(f);

	int boxCount = data["nodeCount"].get<int>();

	boxes = std::vector<CFlat::IBox*>(boxCount);
	std::vector<std::vector<int>> boxInput(boxCount);

	jsonarray input = data["input"].get<jsonarray>();



	for (json in : input) {

		int id = in["id"].get<int>();

		if (in["type"].get<string>() == "fValue") {

			boxes[id] = new CFlat::Float(in["value"].get<float>());
		}
	}

	jsonarray operators = data["operator"].get<jsonarray>();


	for (json op : operators) {

		int id = op["id"].get<int>();

		string type = op["type"].get<std::string>();

		if (type == "Add") {

			boxes[id] = new CFlat::Add();
		}
		else if (type == "Subtract") {

			boxes[id] = new CFlat::Subtract();
		}
		else if (type == "Mult") {

			boxes[id] = new CFlat::Multiply();
		}
		else if (type == "Print") {

			boxes[id] = new CFlat::Print();
		}

		jsonarray operatorInput = op["input"].get<jsonarray>();
		for (json oi : operatorInput)
			boxInput[id].push_back(oi.get<int>());

	}

	for (int i = input.size(); i < boxCount; i++) {

		for (int c = 0; c < boxInput[i].size(); c++) {
			CFlat::IBox* inputBox = boxes[boxInput[i][c]];
			boxes[i]->addInput(inputBox);
		}
	}

	Script* script = new Script{ nullptr, nullptr };

	if (data.contains("init")) {
		int initid = data["init"].get<int>();
		if (initid >= 0)
			script->init = boxes[initid];
	}

	if (data.contains("update")) {
		int updateid = data["update"].get<int>();
		if (updateid >= 0)
			script->update = boxes[updateid];
	}

	scripts.emplace(file, script);
	return script;
}

