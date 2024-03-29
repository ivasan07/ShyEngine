#include "LogManager.h"

#include <iostream>

#include "CheckML.h"

#define LOGFILE "ShyEditor.log"


namespace ShyEditor {

	LogManager* LogManager::instance = nullptr;

	LogManager::LogManager() {}

	LogManager::~LogManager() {}

	LogManager* LogManager::GetInstance() {

		if (instance == nullptr)
			Init();

		return instance;
	}

	bool LogManager::Init() {
		instance = new LogManager();

		// Open log file
		instance->logFile.open(LOGFILE, std::ios::out);

		if (!instance->logFile.is_open()) {
			std::cout << "Could not open the log file! Restart the editor and try again." << std::endl;
			return false;
		}

		instance->logFile << "In this file, you will find a list of the errors or warnings from the editor during its execution.\n " << std::endl;

		return true;
	}

	void LogManager::Release() {

		// Close log file
		if (instance->logFile.is_open())
			instance->logFile.close();

		delete instance;
		instance = nullptr;
	}

	void LogManager::LogError(const std::string& msg) {
		instance->logFile << "ERROR: " << msg << std::endl;
	}

	void LogManager::LogWarning(const std::string& msg) {
		instance->logFile << "WARNING: " << msg << std::endl;
	}

}