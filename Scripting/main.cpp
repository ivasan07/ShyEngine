#include <iostream>
#include "Script.h"
using CFlat::IBox;

int main()
{
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
