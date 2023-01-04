#pragma once
#include <ntifs.h>

typedef unsigned char uint8_t;
typedef unsigned long DWORD;

#ifdef _DEBUG
#define LOG(msg, ...) DbgPrintEx(0, 0, "[SharedMemory] " __FUNCTION__ "(%d): " msg, __LINE__, __VA_ARGS__);
#else
#define LOG(msg, ...)
#endif

extern "C" {
	NTSTATUS NTAPI MmCopyVirtualMemory(
		PEPROCESS SourceProcess,
		PVOID SourceAddress,
		PEPROCESS TargetProcess,
		PVOID TargetAddress,
		SIZE_T BufferSize,
		KPROCESSOR_MODE PreviousMode,
		PSIZE_T ReturnSize
	);
}