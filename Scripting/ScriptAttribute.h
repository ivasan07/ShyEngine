#pragma once

#include <unordered_map>
#include <string>

using std::string;

class ScriptAttribute
{
	/*
		Tipos que se pueden guardar:
			+bool
			+int
			+float
			+vector2
			+vector3
			+string
	*/


	std::unordered_map<string, bool> booleans;
	std::unordered_map<string, int> integers;
	std::unordered_map<string, float> decimals;
	std::unordered_map<string, string> strings;

public:



	void generateAttribute(string name, bool value);
	void generateAttribute(string name, int value);
	void generateAttribute(string name, float value);
	void generateAttribute(string name, string value);

	void retreiveAttribute(string name, bool& value);
	void retreiveAttribute(string name, int& value);
	void retreiveAttribute(string name, float& value);
	void retreiveAttribute(string name, string& value);
};

