#pragma once
#include <Windows.h>
#include <stdint.h>

// Make this global so we can pattern scan it
inline PVOID AllocatedMemory = NULL;

namespace Driver {
	enum class EOperation : uint8_t {
		NONE = 0,
		COMPLETED,
		RUNNING,
		PING,
	};

	struct Request_t {
		EOperation Operation;
		uintptr_t Return;
	};

	void Init();
	Request_t SendRequest(Request_t Request);

	/* Commands */

	bool IsRunning();
	void Ping();
}