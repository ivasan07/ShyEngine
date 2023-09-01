#pragma once

#include "Window.h"

#include <string>
#include <nlohmann/json_fwd.hpp>

namespace ShyEditor {

	struct PreferencesColor {

		PreferencesColor() {
			r = g = b = 0; a = 1;
		}

		float r;
		float g;
		float b;
		float a;
	};

	struct ProjectData {

		// General
		std::string initialScene;
		std::string windowTitle;
		std::string creator;
		bool splashScreen;

		// Window
		std::string name;
		std::string icon;
		int width;
		int height;
		bool vsync;
		bool fullscreen;
		bool showCursor;
		PreferencesColor bgColor;

		// Physics
		bool debugPhysics;
		bool debugFramerate;
		float gravity_x;
		float gravity_y;
		std::vector<std::vector<bool>> collisionMatrix;
		std::vector<std::string> layers;

		// Input
		bool closeWithEscape;

		// Overlay
		float timeToDoubleClick;
		float timeToHoldClick;

		// Audio
		int frequency;
		int channels;
		int chunksize;

		// Internal
		std::string buildPath;

	};


	class Preferences : public Window {

	private:

		static Preferences* instance;

		bool open;

		ProjectData data;

		std::string newLayer;
		std::string defaultLayer;
		int nLayers;
		bool updateMatrix;
		bool showError;
		std::wstring errorMsg;

		void GeneralHeader();
		void WindowHeader();
		void AudioHeader();
		void InputHeader();
		void OverlayHeader();
		void PhysicsHeader();

		nlohmann::ordered_json BasicData();

		// Shows a pop-up window with an error message
		void ShowErrorPopup(const std::wstring& errorMessage);

	public:

		Preferences();

		static void Open();

		static void StoreData();
		static void LoadData();

		static void GenerateDebug();
		static void GenerateRelease();
		static void GenerateBuild();

		void Behaviour() override;

		static ProjectData& GetData();

	};

}