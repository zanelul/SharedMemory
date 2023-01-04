#include "Driver/Driver.h"

#include <chrono>
#include <iostream>
using namespace std::chrono;

int main() {
	Driver::Init();

	while (true) {
		auto start = high_resolution_clock::now();
		for (int i = 0; i < 5; i++) {
			Driver::Ping();
		}
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		std::cout << " 5 requests took " << duration.count() << " nanoseconds" << std::endl;
		Sleep(3000);
	}

	return getchar();
}