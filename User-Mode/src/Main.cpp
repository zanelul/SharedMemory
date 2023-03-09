#include <chrono>
#include <iostream>
#include "Driver/Driver.h"

using namespace std::chrono;

#define LOG(msg) std::cout << "[+] " msg << std::endl;

int main() {
	Driver::Init();

	bool IsRunning = Driver::IsRunning();

	LOG("Driver is " << (IsRunning ? "active" : "not active"));

	while (true) {
		auto start = high_resolution_clock::now();
		Driver::Ping();
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		LOG("Ping request took " << duration.count() << " milliseconds");
		Sleep(3000);
	}

	return getchar();
}