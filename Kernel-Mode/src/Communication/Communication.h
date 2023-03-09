#pragma once
#include "../Memory/Memory.h"

namespace Communication {
	enum class EOperation : uint8_t {
		PING = 0,
		COMPLETED
	};

	struct Request_t {
		EOperation Operation;
	};

	void Thread(PVOID Context);
}