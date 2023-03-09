#pragma once
#include <defines.h>

namespace Memory {
	struct Process_t {
		const char* Name;
		DWORD_PTR ID;
		uintptr_t Base;
	};

	DWORD_PTR GetProcessID(const char* ProcessName);
	Process_t GetProcess(const char* ProcessName);
}