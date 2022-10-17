#pragma once

#include <string>
#include "ScriptAttribute.h"

using std::string;

namespace CFlat {

	class IBox;
}

class Script
{

private:
	CFlat::IBox* initBox;
	CFlat::IBox* updateBox;

	void iteration(CFlat::IBox* startingBox);

	ScriptAttribute attributes;

public:

	void setupScript(std::string script);

	void init();

	void update();
};
