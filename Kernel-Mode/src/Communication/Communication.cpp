#include "Communication.h"

#define UM_PROCESS "User-Mode.exe"

void Communication::Thread(PVOID Context) {
	UNREFERENCED_PARAMETER(Context);

	Memory::Process_t Usermode = {};

	while (true) {
		DWORD_PTR UsermodePID = Memory::GetProcessID(UM_PROCESS);
		if (UsermodePID == 0) {
			Usermode = {};
			continue;
		}
		
		if (Usermode.Base != 0) {
			PVOID Address = (PVOID)(Usermode.Base + 0x5638);
			PEPROCESS Process = NULL;
			NTSTATUS Status = PsLookupProcessByProcessId((HANDLE)Usermode.ID, &Process);

			if (NT_SUCCESS(Status)) {
				SIZE_T OutSize = NULL;

				Request_t Request{};
				Status = MmCopyVirtualMemory(Process, Address, PsGetCurrentProcess(), &Request, sizeof(Request), KernelMode, &OutSize);

				if (NT_SUCCESS(Status)) {
					switch (Request.Operation) {
					case EOperation::RUNNING: {
						SIZE_T OutSize2 = NULL;
						Request.Operation = EOperation::COMPLETED;
						Request.Return = 0x123;
						MmCopyVirtualMemory(PsGetCurrentProcess(), &Request, Process, Address, sizeof(Request), KernelMode, &OutSize2);
						break;
					}
					case EOperation::PING: {
						SIZE_T OutSize2 = NULL;
						Request.Operation = EOperation::COMPLETED;
						MmCopyVirtualMemory(PsGetCurrentProcess(), &Request, Process, Address, sizeof(Request), KernelMode, &OutSize2);
						break;
					}
					case EOperation::NONE:
						break;
					}
				}

				ObfDereferenceObject(Process);
			}
		}
		else {
			Usermode = Memory::GetProcess(UM_PROCESS);
		}
	}
}