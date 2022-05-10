#include <Timer.hpp>

#include <iostream>



void Timer::start(const std::string &message) {
	timePoint_ = std::chrono::high_resolution_clock::now();
	message_ = message;
}

void Timer::stop() {
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - timePoint_);
	std::cout << "[INFO][" << message_ << "] Total processing time " << duration.count()/1000 << "."
			  << duration.count()%1000 << "s"
			  << std::endl;
}
