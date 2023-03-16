#pragma once
#include <Windows.h>
#include <stdint.h>

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

	// idk what to name this variable
	inline Request_t Requests = {};

	void Init();
	Request_t SendRequest(Request_t Request);

	/* Commands */

	bool IsRunning();
	void Ping();
}