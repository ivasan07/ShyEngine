#include "InputManager.h"

#include <iostream>

namespace InputManager {

	InputManager::InputManager()
	{
		initialiseJoysticks();

		kbState_ = SDL_GetKeyboardState(0);
		clearState();
	}

	InputManager::~InputManager() {
		cleanJoysticks();
	}

	void InputManager::clearState() {

		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;
		isMouseButtonEvent_ = false;
		isMouseMotionEvent_ = false;

		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	void InputManager::update(const SDL_Event& event) {

		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case SDL_JOYAXISMOTION:
			onJoystickAxisMotion(event);
			break;
		case SDL_JOYBUTTONDOWN:

			break;
		case SDL_JOYBUTTONUP:

			break;
		default:
			break;
		}
	}

	// --------- MOUSE & KB ------------

	bool InputManager::keyDownEvent() {
		return isKeyDownEvent_;
	}

	bool InputManager::keyUpEvent() {
		return isKeyUpEvent_;
	}

	bool InputManager::isKeyDown(SDL_Scancode key) {
		return keyDownEvent() && kbState_[key] == 1;
	}

	bool InputManager::isKeyUp(SDL_Scancode key) {
		return keyUpEvent() && kbState_[key] == 0;
	}

	bool InputManager::isKeyDown(SDL_Keycode key) {
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	bool InputManager::isKeyUp(SDL_Keycode key) {
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	bool InputManager::mouseMotionEvent() {
		return isMouseMotionEvent_;
	}

	bool InputManager::mouseButtonEvent() {
		return isMouseButtonEvent_;
	}

	const std::pair<Sint32, Sint32>& InputManager::getMousePos() {
		return mousePos_;
	}

	int InputManager::getMouseButtonState(MOUSEBUTTON b) {
		return mbState_[b];
	}

	void InputManager::onKeyDown(const SDL_Event& event) {
		isKeyDownEvent_ = true;
	}

	void InputManager::onKeyUp(const SDL_Event& event) {

		isKeyUpEvent_ = true;
	}

	void InputManager::onMouseMotion(const SDL_Event& event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	void InputManager::onMouseButtonChange(const SDL_Event& event, bool isDown) {
		isMouseButtonEvent_ = true;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

	// ---------- CONTROLLER -----------

	void InputManager::initialiseJoysticks() {

		if (SDL_NumJoysticks() > 0) {

			for (int i = 0; i < SDL_NumJoysticks(); i++) {
				SDL_Joystick* joy = SDL_JoystickOpen(i);

				assert(joy != NULL);
					
				joysticks.push_back(joy);

				joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));
			}

			SDL_JoystickEventState(SDL_ENABLE);
		}
	}

	void InputManager::cleanJoysticks() {
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(joysticks[i]);
			delete joystickValues[i].first;
			delete joystickValues[i].second;
		}
	}

	void InputManager::onJoystickAxisMotion(const SDL_Event& event) {

		if (joysticks.size() > 0) {

			if ((event.jaxis.value < -STICK_DEADZONE) || (event.jaxis.value > STICK_DEADZONE)) // Deadzone
			{
				isAxisMotionEvent_ = true;
				joystickId = event.jaxis.which;

				switch (event.jaxis.axis)
				{
				case 0:
					joystickValues[joystickId].first->setX(event.jaxis.value);
					break;
				case 1:
					joystickValues[joystickId].first->setY(-event.jaxis.value);
					break;
				case 2:
					joystickValues[joystickId].second->setX(event.jaxis.value);
					break;
				case 3:
					joystickValues[joystickId].second->setY(-event.jaxis.value);
					break;
				default:
					break;
				}
			}
			else {
				/*isAxisMotionEvent_ = false;

				for (auto i = 0u; i < joysticks.size(); i++) {
					joystickValues[joystickId].first->setX(0); joystickValues[joystickId].first->setY(0);
					joystickValues[joystickId].second->setX(0); joystickValues[joystickId].second->setY(0);
				}*/
			}
		}
	}

	bool InputManager::isJoystickAxisMotion() {
		return isAxisMotionEvent_;
	}

	bool InputManager::getJoystickId() {
		return joystickId;
	}

	Vector2D InputManager::getJoystickValue(int joy, CONTROLLERSTICK ct)
	{
		Vector2D v = Vector2D();

		switch (ct) {
		case LEFTSTICK:
			v = joystickValues[joy].first;
			break;
		case RIGHTSTICK:
			v = joystickValues[joy].second;
			break;
		default:
			break;
		}

		return v / (MAX_STICK_VALUE);
	}
}