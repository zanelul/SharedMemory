#include "Memory.h"

DWORD_PTR Memory::GetProcessID(const char* ProcessName) {
	DWORD_PTR ID = NULL;
	ULONG Bytes = NULL;

	ANSI_STRING Ansi{};
	UNICODE_STRING Unicode{};
	RtlInitAnsiString(&Ansi, ProcessName);
	RtlAnsiStringToUnicodeString(&Unicode, &Ansi, TRUE);

	ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &Bytes);
	if (Bytes == 0) return ID;
	PSYSTEM_PROCESS_INFORMATION ProcessInformation = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePool(NonPagedPool, Bytes);
	NTSTATUS Status = ZwQuerySystemInformation(SystemProcessInformation, ProcessInformation, Bytes, &Bytes);
	if (!NT_SUCCESS(Status)) return ID;

	PSYSTEM_PROCESS_INFORMATION CurrentProcess = ProcessInformation;

	while (CurrentProcess) {
		if (CurrentProcess->NextEntryOffset == 0)
			break;

		CurrentProcess = (PSYSTEM_PROCESS_INFORMATION)(((uintptr_t)CurrentProcess) + CurrentProcess->NextEntryOffset);

		if (RtlCompareUnicodeString(&CurrentProcess->ImageName, &Unicode, TRUE) == 0) {
			ID = (DWORD_PTR)CurrentProcess->ProcessId;
			break;
		}
	}

	if (ProcessInformation)
		ExFreePool(ProcessInformation);

	RtlFreeUnicodeString(&Unicode);

	return ID;
}

Memory::Process_t Memory::GetProcess(const char* ProcessName) {
	ANSI_STRING Ansi{};
	UNICODE_STRING Unicode{};
	RtlInitAnsiString(&Ansi, ProcessName);
	RtlAnsiStringToUnicodeString(&Unicode, &Ansi, TRUE);

	Process_t Return{};
	ULONG Bytes = NULL;

	ZwQuerySystemInformation(SystemProcessInformation, NULL, Bytes, &Bytes);
	if (Bytes == 0) return Return;
	PSYSTEM_PROCESS_INFORMATION ProcessInformation = (PSYSTEM_PROCESS_INFORMATION)ExAllocatePool(NonPagedPool, Bytes);
	NTSTATUS Status = ZwQuerySystemInformation(SystemProcessInformation, ProcessInformation, Bytes, &Bytes);
	if (!NT_SUCCESS(Status)) return Return;

	PSYSTEM_PROCESS_INFORMATION CurrentProcess = ProcessInformation;

	while (CurrentProcess) {
		if (CurrentProcess->NextEntryOffset == 0)
			break;

		CurrentProcess = (PSYSTEM_PROCESS_INFORMATION)(((uintptr_t)CurrentProcess) + CurrentProcess->NextEntryOffset);

		if (RtlCompareUnicodeString(&CurrentProcess->ImageName, &Unicode, TRUE) == 0) {
			Return.Name = ProcessName;
			Return.ID = (DWORD_PTR)CurrentProcess->ProcessId;
			break;
		}
	}

	if (ProcessInformation)
		ExFreePool(ProcessInformation);

	if (Return.ID != 0) {
		PEPROCESS Process;
		KAPC_STATE State{};

		Status = PsLookupProcessByProcessId((HANDLE)Return.ID, &Process);
		if (!NT_SUCCESS(Status)) return Return;
		PPEB64 PEB = (PPEB64)PsGetProcessPeb(Process);
		KeStackAttachProcess(Process, &State);
		PPEB_LDR_DATA LdrData = PEB->Ldr;
		if (!LdrData) {
			KeUnstackDetachProcess(&State);
			return Return;
		}

		for (PLIST_ENTRY ListEntry = LdrData->InLoadOrderLinks.Flink; ListEntry != &LdrData->InLoadOrderLinks; ListEntry = ListEntry->Flink) {
			PLDR_DATA_TABLE_ENTRY ModuleEntry = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

			UNREFERENCED_PARAMETER(ModuleEntry);

			if (RtlCompareUnicodeString(&ModuleEntry->BaseDllName, &Unicode, TRUE) == 0) {
				Return.Base = (uintptr_t)ModuleEntry->DllBase;
				break;
			}
		}

		KeUnstackDetachProcess(&State);
	}

	RtlFreeUnicodeString(&Unicode);

	return Return;
}