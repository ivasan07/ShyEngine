#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <InputManager.h>

#include <iostream>
#include <memory>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int main(int argc, char* args[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto rm = RendererManager::RendererManager::init("PhosphorusEngine Window", WIN_WIDTH, WIN_HEIGHT);
	auto im = InputManager::InputManager::init();
	auto sm = SoundManager::SoundManager::init();


	SDL_Rect* r = new SDL_Rect({ rm->getWidth() / 2 - 25, rm->getHeight() / 2 - 25, 50, 50});
	SDL_Color c = rm->createColor(0x00000000);

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


		// Test de teclado
		if (im->isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit =  true;
			continue;
		}

		// Test de botones
		if (im->isJoystickButtonEventDown()) {
			for (int i = 0; i < im->getJoysticksNumButtons(); i++) {
				std::cout << im->getJoystickButtonState(i) << "  ";
			}
			std::cout << std::endl;
		}

		// Test de triggers
		if (im->isJoystickAxisMotion()) {
			if (im->isLeftTriggerMotion()) {

			}
		}

		rm->clearRenderer(c);

		SDL_SetRenderDrawColor(rm->getRenderer(), 255, 185, 0, 255);
		SDL_RenderFillRect(rm->getRenderer(), r);

		rm->presentRenderer();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 60)
			SDL_Delay(60 - frameTime);
	}

	delete r; r = NULL;

	return 0;
}