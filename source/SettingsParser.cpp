#include <SettingsParser.hpp>
#include <cxxopts/cxxopts.hpp>



bool SettingsParser::parseSettings(int argc, char **argv) {
	settings_ = std::make_shared<Settings>();

	cxxopts::Options options { "OpenMPEREWPram", "Implementation of EREW PRAM algorithm." };

	options.add_options()("m, mode", R"(Set mode of program, either "runLimited" with 32 values only or "runScalable" for more values.)",
						  cxxopts::value<std::string>()->default_value("runLimited"));
	options.add_options()("h, help", "Print usage.");
	options.add_options()("s, size", "Size of array used for calculation, in runScalable mode only, minimum is 1.",
						  cxxopts::value<int>()->default_value("32"));

	auto cmdArguments = options.parse(argc, argv);
	if(cmdArguments.count("h") || argc == 1) {
		std::cout << options.help() << std::endl;
		return false;
	}

	settings_->mode = modeFromString(cmdArguments["mode"].as<std::string>());
	settings_->size = cmdArguments["size"].as<int>();

	if(settings_->mode == Mode::INVALID){
		std::cerr << "[ERROR] Invalid mode\n";
		std::cerr << options.help() << std::endl;
		return false;
	}

	if(settings_->size < 1){
		std::cerr << "[ERROR] Invalid size\n";
		std::cerr << options.help() << std::endl;
		return false;
	}

	return true;
}

std::shared_ptr<Settings> SettingsParser::getSettings() {
	return settings_;
}

Mode SettingsParser::modeFromString(const std::string &modeString) {
	if(modeString == "limited"){
		return Mode::LIMITED;
	}else if(modeString == "scalable"){
		return Mode::SCALABLE;
	}
	return Mode::INVALID;
}
