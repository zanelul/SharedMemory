#include "Driver.h"
#include <thread>

void Driver::Init() {
	AllocatedMemory = VirtualAlloc(NULL, sizeof(Request_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!AllocatedMemory) return;

	Request_t InitReq{};
	InitReq.Operation = EOperation::NONE;
	memcpy(AllocatedMemory, &InitReq, sizeof(InitReq));
}

Driver::Request_t Driver::SendRequest(Request_t Request) {
	memcpy(AllocatedMemory, &Request, sizeof(Request));

	std::this_thread::sleep_for(std::chrono::milliseconds(2));

	// Very bad implemtation, clean later on
	while (true) {
		Request_t Respond = *(Request_t*)(AllocatedMemory);

		if (Respond.Operation == EOperation::COMPLETED)
			return Respond;
	}
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