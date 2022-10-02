#include "ScriptAttribute.h"

void ScriptAttribute::generateAttribute(string name, bool value)
{
	booleans.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, int value)
{
	integers.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, float value)
{
	decimals.emplace(name, value);
}

void ScriptAttribute::generateAttribute(string name, string value)
{
	strings.emplace(name, value);
}

void ScriptAttribute::retreiveAttribute(string name, bool& value)
{
	value = booleans[name];
}

void ScriptAttribute::retreiveAttribute(string name, int& value)
{
	value = integers[name];
}

void ScriptAttribute::retreiveAttribute(string name, float& value)
{
	value = decimals[name];
}

void ScriptAttribute::retreiveAttribute(string name, string& value)
{
	value = strings[name];
}
