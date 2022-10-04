#include <PhysicsManager.h>
#include <SoundManager.h>
#include <RendererManager.h>
#include <InputManager.h>

#include <iostream>
#include <memory>

int main(int argc, char* args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto rm = RendererManager::RendererManager::init("PhosphorusEngine Window", 1280, 720);
	auto im = InputManager::InputManager::init();

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	while (!exit) {

		Uint32 startTime = SDL_GetTicks();

		im->clearState();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit = true;
				continue;
			}
			im->update(event);
		}

		if (im->isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit =  true;
			continue;
		}

		if (im->isJoystickAxisMotion()) {
			std::cout << "Joystick derecho: " << im->getJoystickTriggerValue(im->getJoystickId(), InputManager::InputManager::LEFT_TRIGGER) << std::endl;
			std::cout << "Joystick izquierdo: " << im->getJoystickTriggerValue(im->getJoystickId(), InputManager::InputManager::RIGHT_TRIGGER) << std::endl;
		}
		
		rm->clearRenderer();



		rm->presentRenderer();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 60)
			SDL_Delay(60 - frameTime);
	}

	return 0;
}