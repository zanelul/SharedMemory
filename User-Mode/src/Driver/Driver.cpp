#include "Driver.h"
#include <thread>

void Driver::Init() {
	/* Going to add some initialization to this later */
}

Driver::Request_t Driver::SendRequest(Request_t Request) {
	memcpy(&Requests, &Request, sizeof(Request_t));

	while (Requests.Operation != EOperation::COMPLETED) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	return Requests;
}

bool Driver::IsRunning() {
	Request_t Request{};
	Request.Operation = EOperation::RUNNING;
	Request = SendRequest(Request);
	return Request.Return == 0x123;
}

void Driver::Ping() {
	Request_t Request{};
	Request.Operation = EOperation::PING;
	SendRequest(Request);
}