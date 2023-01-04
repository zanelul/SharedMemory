#pragma once
#include "Registry.h"

inline UNICODE_STRING REG_PATH = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\UX");

namespace Driver {
	enum class EOperation : uint8_t {
		PING = 0,
		COMPLETED
	};

	struct Request_t {
		EOperation Operation;
	};

	inline PVOID m_AllocatedMemory;

	void Init();
	void SendRequest(Request_t Request);
	void Ping();
}