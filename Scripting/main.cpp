#include <iostream>
#include "Script.h"
#include "ScriptAttribute.h"
#include "OperatorManager.h"

#include "FlowManager.h"

using CFlat::IBox;

int main() {
	/*
	*	TODO:
	*	Variables en cada script
	*	Encadenar instrucciones
	*
	*	Queda manejar las referencias a otras entidades de la escena
	*
	*	Hay que eliminar el output de los nodos al terminar el ciclo
	*
	*
	*	El bug de que si por algun motivo añades un input todo peta xd
	*
	*	Cosas que necesita el lenguaje:
	*
	*	Operaciones enlazadas (una operacion que apunta a la siguiente)
	*	Atributos
	*	Metodos
	*	Condicionales
	*	Bucles
	*	Arrays
	*	Estructuras
	*
	*/

	CFlat::OperatorManager::Initialise();
	CFlat::FlowManager flowManager;

	Script script{};
	script.setupScript("MyScript");
	script.init();

	std::cout << "\n== update ==\n";

	int cont = 0;
	int iterationCount = 10;
	while (cont < iterationCount) {
		script.update();
		cont++;
	}


	return 0;
}

