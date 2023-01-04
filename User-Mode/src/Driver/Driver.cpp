#include "Driver.h"

void Driver::Init() {
	m_AllocatedMemory = VirtualAlloc(NULL, sizeof(Request_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	PVOID OurPID = reinterpret_cast<PVOID>(GetCurrentProcessId());
	Registry::Write(REG_PATH, RTL_CONSTANT_STRING(L"AllocatedMemory"), &m_AllocatedMemory, REG_QWORD, 8);
	Registry::Write(REG_PATH, RTL_CONSTANT_STRING(L"PID"), &OurPID, REG_QWORD, 8);

	Request_t Request{};
	memcpy(m_AllocatedMemory, &Request, sizeof(Request));
}

void Driver::SendRequest(Request_t Request) {
	memcpy(m_AllocatedMemory, &Request, sizeof(Request));

	// Very bad implemtation, clean later on
	while (true) {
		Request = *(Request_t*)(m_AllocatedMemory);

		if (Request.Operation == EOperation::COMPLETED)
			break;
	}
}

void Driver::Ping() {
	Request_t Request{};
	Request.Operation = EOperation::PING;
	SendRequest(Request);
}