#include <iostream>
#include <SettingsParser.hpp>

#include <omp.h>

void runLimited();
void runScalable(int size);

int main(int argc, char** argv){
	SettingsParser settingsParser;

	try{
		if(!settingsParser.parseSettings(argc, argv)){
			return EXIT_SUCCESS;
		}
		auto settings = settingsParser.getSettings();
		switch(settings->mode) {
			case Mode::LIMITED:
				std::cout << "[INFO] Running program in runLimited mode."<< std::endl;
				runLimited();
				break;
			case Mode::SCALABLE:
				runScalable(settings->size);
				std::cout << "[INFO] Running program in runScalable mode."<< std::endl;
				break;
		}
	}catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	int id = 5;
#pragma omp parallel private(id)
	{
		id = omp_get_thread_num();
		printf("%d: Hello World!\n", id);
	}
	return 0;
}


void runLimited(){

}

void runScalable(int size){

}