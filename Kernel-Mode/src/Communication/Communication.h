#pragma once
#include <registry.h>

namespace Communication {
	enum class EOperation : uint8_t {
		PING = 0,
		COMPLETED
	};

	struct Request_t {
		EOperation Operation;
	};

	inline UNICODE_STRING REG_PATH = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\UX");
	inline PVOID m_AllocatedMemory;
	inline DWORD m_PID;

	void Thread(PVOID Context);
}