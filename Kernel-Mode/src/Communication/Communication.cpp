#include "Communication.h"

void Communication::Thread(PVOID Context) {
	UNREFERENCED_PARAMETER(Context);

	while (true) {
		PEPROCESS Process = NULL;
		NTSTATUS Status = PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(m_PID), &Process);

		if (NT_SUCCESS(Status) && Process != NULL) {
			Request_t Request{};
			SIZE_T OutSize = NULL;
			Status = MmCopyVirtualMemory(Process, m_AllocatedMemory, PsGetCurrentProcess(), &Request, sizeof(Request), KernelMode, &OutSize);

			if (NT_SUCCESS(Status)) {
				switch (Request.Operation) {
				case EOperation::PING: {
					SIZE_T OutSize2 = NULL;
					Request.Operation = EOperation::COMPLETED;
					MmCopyVirtualMemory(PsGetCurrentProcess(), &Request, Process, m_AllocatedMemory, sizeof(Request), KernelMode, &OutSize2);
					break;
				}
				}
			}

			ObfDereferenceObject(Process);
		}
		else {
			m_AllocatedMemory = reinterpret_cast<PVOID>(Registry::Read<LONG64>(REG_PATH, RTL_CONSTANT_STRING(L"AllocatedMemory")));
			m_PID = static_cast<DWORD>(Registry::Read<LONG64>(REG_PATH, RTL_CONSTANT_STRING(L"PID")));
		}
	}
}