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
	//TODO: codigo defensivo por si algunos nodos no tienen entrada
	//TODO: hago dos busquedas lo cual esta bastante feo
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

	std::vector<std::pair<int, int>> boxLink;

	for (json in : input) {

		int id = in["id"].get<int>();

		//TODO: seguramente sea chulo hacer algo como con los operadores para no tener una cadena enorme de ifs

		if (in["type"].get<string>() == "fValue") {

			boxes[id] = new CFlat::Float(in["value"].get<float>());
		}

		else if (in["type"].get<string>() == "sValue") {

			boxes[id] = new CFlat::String(in["value"].get<std::string>());
		}
	}

	jsonarray operators = data["operator"].get<jsonarray>();


	for (json op : operators) {

		int id = op["id"].get<int>();

		int type = op["type"].get<int>();

		if (op.contains("next")) {
			int nextBox = op["next"].get<int>();
			
			boxLink.push_back(std::make_pair(id, nextBox));
		}

		boxes[id] = new CFlat::IBox(type);
		
		jsonarray operatorInput = op["input"].get<jsonarray>();
		for (json oi : operatorInput)
			boxInput[id].push_back(oi.get<int>());

	}

	for (int i = 0; i < boxCount; i++) {

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

	//Links
	for (std::pair<int, int> pair : boxLink) {

		boxes[pair.first]->nextBox = boxes[pair.second];
	}


	scripts.emplace(file, script);
	return script;
}

