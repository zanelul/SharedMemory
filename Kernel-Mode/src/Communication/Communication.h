#pragma once
#include "../Memory/Memory.h"

namespace Communication {
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

	void Thread(PVOID Context);
}