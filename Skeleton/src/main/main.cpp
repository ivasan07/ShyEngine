#include <memory>
#include <iostream>
#include "Engine.h"
#include "Save.h"
#include "ConsoleManager.h"
#include <string>
#include <format>

#include "Scripting/ScriptFunctionality.h"


int main(int argc, char* args[]) {

	//float time = Scripting::ScriptFunctionality::instance()->Time_Now();

	//std::cout << Scripting::ScriptFunctionality::instance()->Time_WeekDay(time) << std::endl;
	//std::cout << Scripting::ScriptFunctionality::instance()->Time_Month(time) << std::endl;

	//std::cout << Scripting::ScriptFunctionality::instance()->Time_TimeStamp(time) << std::endl;
	//std::cout << Scripting::ScriptFunctionality::instance()->Time_DDMMYY(time) << std::endl;
	//std::cout << Scripting::ScriptFunctionality::instance()->Time_TimeHHMMSS(time) << std::endl;
	//std::cout << Scripting::ScriptFunctionality::instance()->Time_TimeHHMMSS(Scripting::ScriptFunctionality::instance()->Time_Now()) << std::endl;
	//std::cout << time << std::endl;
	//std::cout << Scripting::ScriptFunctionality::instance()->Time_Now() << std::endl;

	/*ECS::SaveManager* save = ECS::SaveManager::instance();

	int slot = 1;

	if (!save->Exists(1)) {

		save->SetSlot("contador", Scripting::Variable::Int(0));
	}

	else {

		save->Load(1);
		save->SetSlot("contador", Scripting::Variable(save->GetSlot("contador").value.Float + 1));
	}

	Console::Output::PrintNoFormat(std::format("Has abierto el juego {} veces", save->GetSlot("contador").value.Float));

	save->Save(1);*/

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;

	bool ret = engine.init();

	if (!ret) {
		return -1;
	}

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";

	return 0;
}
