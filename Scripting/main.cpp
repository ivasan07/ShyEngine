#include <iostream>
#include "Script.h"
#include "ScriptAttribute.h"
#include "OperatorManager.h"


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

	Script script{};
	script.setupScript("MyScript");


	script.init();
	std::cout << "\n== update ==\n";
	while (true) {
		script.update();
		break;
	}


	return 0;
}

