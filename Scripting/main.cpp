#include <iostream>
#include "Script.h"
#include "ScriptAttribute.h"
#include "OperatorManager.h"
using CFlat::IBox;

int main()
{
	/*
	*	TODO:
	*	Variables en cada script
	*	Encadenar instrucciones 
	* 
	*	Hay que eliminar el output de los nodos al terminar el ciclo
	*/

	CFlat::OperatorManager::Initialise();

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
