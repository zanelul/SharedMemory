#pragma once
#include <ntifs.h>

typedef unsigned char uint8_t;
typedef unsigned long DWORD;

#define LOG(msg, ...) DbgPrintEx(0, 0, "[+] " __FUNCTION__ "(%d): " msg, __LINE__, __VA_ARGS__);

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    // ...
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
    unsigned int Length;
    int Initialized;
    void* SSHandle;
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    // ...
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB64
{
    unsigned char InheritedAddressSpace;	// 0x0000 
    unsigned char ReadImageFileExecOptions;	// 0x0001 
    unsigned char BeingDebugged;			// 0x0002 
    unsigned char BitField;					// 0x0003 
    unsigned char pad_0x0004[0x4];			// 0x0004
    PVOID Mutant;							// 0x0008 
    PVOID ImageBaseAddress;					// 0x0010 
    PPEB_LDR_DATA Ldr;						// 0x0018
    // ...
} PEB64, * PPEB64;

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[MAXIMUM_FILENAME_LENGTH];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG                   NextEntryOffset;
    ULONG                   NumberOfThreads;
    LARGE_INTEGER           Reserved[3];
    LARGE_INTEGER           CreateTime;
    LARGE_INTEGER           UserTime;
    LARGE_INTEGER           KernelTime;
    UNICODE_STRING          ImageName;
    KPRIORITY               BasePriority;
    HANDLE                  ProcessId;
    HANDLE                  InheritedFromProcessId;
    ULONG                   HandleCount;
    ULONG                   Reserved2[2];
    ULONG                   PrivatePageCount;
    VM_COUNTERS             VirtualMemoryCounters;
    IO_COUNTERS             IoCounters;
    void*                   Threads[1];
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemNextEventIdInformation,
    SystemEventIdsInformation,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemPlugPlayBusInformation,
    SystemDockInformation,
} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;

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

	NTSTATUS ZwQuerySystemInformation(
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID SystemInformation,
		IN ULONG SystemInformationLength,
		OUT PULONG ReturnLength OPTIONAL
	);

    NTKERNELAPI PPEB NTAPI PsGetProcessPeb(
        IN PEPROCESS Process
    );
}