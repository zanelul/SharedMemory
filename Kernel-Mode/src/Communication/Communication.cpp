#include "Communication.h"

#define UM_PROCESS "notepad.exe"

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
			LOG("Found user-mode program!");
			LOG("User-mode: %s (%lld | 0x%llX)", Usermode.Name, Usermode.ID, Usermode.Base);
		}
		else {
			Usermode = Memory::GetProcess(UM_PROCESS);
		}
	}
}