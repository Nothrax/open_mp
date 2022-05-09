#pragma once

#include <memory>

enum class Mode{
	INVALID = -1,
	LIMITED = 0,
	SCALABLE
};

struct Settings{
	Mode mode;
	int size;
};

class SettingsParser {
public:
	bool parseSettings(int argc, char** argv);
	std::shared_ptr<Settings> getSettings();
private:
	std::shared_ptr<Settings> settings_;

	Mode modeFromString(const std::string& modeString);
};
