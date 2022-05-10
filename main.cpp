
#include <SettingsParser.hpp>
#include <Timer.hpp>

#include <omp.h>

#include <iostream>
#include <cmath>



Timer timer;

const static int numberOfThreads = 32; //todo change


uint64_t runTest(uint64_t size);

uint64_t runLimited();

uint64_t runScalable(uint64_t size);

void fillArray(uint64_t *array, uint64_t size);

int main(int argc, char **argv) {
	SettingsParser settingsParser;
	uint64_t result, test;

	try {
		if(!settingsParser.parseSettings(argc, argv)) {
			return EXIT_SUCCESS;
		}
		auto settings = settingsParser.getSettings();
		switch(settings->mode) {
			case Mode::LIMITED:
				std::cout << "[INFO] Running program in Limited mode." << std::endl;
				result = runLimited();
				break;
			case Mode::SCALABLE:
				std::cout << "[INFO] Running program in Scalable mode." << std::endl;
				result = runScalable(settings->size);
				break;
			case Mode::INVALID:
				std::cout << "[ERROR] Invalid mode." << std::endl;
				return EXIT_FAILURE;
		}
		test = runTest(settings->size);
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}


	std::cout << "[INFO] Completed! Result is: " << result << " and should be: " << test << std::endl;
	return 0;
}

uint64_t runLimited() {
	static int size = 32;
	uint64_t M[size];
	uint64_t y[size];
	fillArray(M, size);
	fillArray(y, size);
	omp_set_num_threads(numberOfThreads);

	int end = ceil(log2(size));

	timer.start("limited");
	for(int j = 0; j < end; j++) {
		int offset = pow(2, j);

#pragma omp parallel shared(M, y)
		{
#pragma omp for
			for(int i = offset; i < size; i++) {
				y[i] = y[i] + M[i - offset];
			}
#pragma omp for
			for(int i = offset; i < size; i++) {
				M[i] = y[i];
			}
		}
	}
	timer.stop();


	return y[size - 1];
}

uint64_t runScalable(uint64_t size) {
	uint64_t y[size];
	uint64_t z[size];
	uint64_t si[numberOfThreads];
	fillArray(y, size);
	fillArray(z, size);
	uint64_t dataIndex, end, offset, threadIndex, i, j;
	omp_set_num_threads(numberOfThreads);

	int stepsPerThread = size/numberOfThreads;

	timer.start("limited");

#pragma omp parallel shared(y, z, si, numberOfThreads, stepsPerThread) private(dataIndex, threadIndex)
	{
		threadIndex = omp_get_thread_num();
		si[threadIndex] = 0;
		for(dataIndex = threadIndex*stepsPerThread; (dataIndex < (threadIndex + 1)*stepsPerThread) ||
													((dataIndex >= numberOfThreads*stepsPerThread) &&
													 (dataIndex < size)); dataIndex++) {
			si[threadIndex] += *(y + dataIndex);
			y[dataIndex] = si[threadIndex];
		}
		z[threadIndex] = si[threadIndex];
	}


	end = ceil(log(numberOfThreads)/log(2));


	for(j = 0; j < end; j++) {
		offset = pow(2, j);
#pragma omp parallel shared(y, z, stepsPerThread, numberOfThreads)
		{
#pragma omp for
			for(i = offset; i < numberOfThreads; i++) {
				z[i] += y[(i - offset + 1)*stepsPerThread - 1];
			}
#pragma omp for
			for(i = offset; i < numberOfThreads; i++) {
				if(i == numberOfThreads - 1) {
					y[size - 1] = z[i];
				} else {
					y[(i + 1)*stepsPerThread - 1] = z[i];
				}
			}
		}
	}

#pragma omp parallel shared(y, stepsPerThread, numberOfThreads) private(i, j)
	{
		j = omp_get_thread_num();
		for(i = j*stepsPerThread; (i < (j + 1)*stepsPerThread - 1) ||
								  ((i >= (numberOfThreads - 1)*stepsPerThread) && (i < size - 1)); i++) {
			if(j != 0) {
				y[i] += y[j*stepsPerThread - 1];
			}
		}
	}


	timer.stop();


	return y[size - 1];
}

uint64_t runTest(uint64_t size) {
	uint64_t y[size];
	fillArray(y, size);
	uint64_t result = 0;

	timer.start("test");
	for(uint64_t i = 0; i < size; i++) {
		result += y[i];
	}
	timer.stop();

	return result;
}

void fillArray(uint64_t *array, uint64_t size) {
	for(uint64_t i = 0; i < size; i++) {
		array[i] = i + 1;
	}
}