#pragma once

#include <chrono>
#include <string>

class Timer {
public:
	void start(const std::string& message);
	void stop();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> timePoint_;
	std::string message_;
};

