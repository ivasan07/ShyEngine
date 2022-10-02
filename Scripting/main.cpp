#include <iostream>
#include "Script.h"
#include "ScriptAttribute.h"

using CFlat::IBox;

int main()
{
	/*
	*	TODO:
	*	Variables en cada script
	*	Encadenar instrucciones 
	*/



	Script script{};
	script.setupScript("NivelMireLol");

	script.init();
	std::cout << "\n== update ==\n";
	while (true) {
		script.update();
		break;
	}


	return 0;
}
