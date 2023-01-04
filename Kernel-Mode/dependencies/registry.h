#pragma once
#include <defines.h>

namespace Registry
{
	__forceinline ULONG GetKeyInfoSize(HANDLE hKey, PUNICODE_STRING Key)
	{
		NTSTATUS Status;
		ULONG KeySize;

		Status = ZwQueryValueKey(hKey, Key, KeyValueFullInformation, 0, 0, &KeySize);

		if (Status == STATUS_BUFFER_TOO_SMALL || Status == STATUS_BUFFER_OVERFLOW)
			return KeySize;

		return 0;
	}

	template <typename type>
	__forceinline type Read(UNICODE_STRING RegPath, UNICODE_STRING Key)
	{
		HANDLE hKey;
		OBJECT_ATTRIBUTES ObjAttr;
		NTSTATUS Status = STATUS_UNSUCCESSFUL;

		InitializeObjectAttributes(&ObjAttr, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		Status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &ObjAttr);

		if (NT_SUCCESS(Status))
		{
			ULONG KeyInfoSize = GetKeyInfoSize(hKey, &Key);
			ULONG KeyInfoSizeNeeded;

			if (KeyInfoSize == NULL)
			{
				ZwClose(hKey);
				return 0;
			}

			// Fuck windows kernel for not allowing "malloc", linux kernel on top :muscle:
			PKEY_VALUE_FULL_INFORMATION pKeyInfo = (PKEY_VALUE_FULL_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, KeyInfoSize, 0x1234);
			RtlZeroMemory(pKeyInfo, KeyInfoSize);

			Status = ZwQueryValueKey(hKey, &Key, KeyValueFullInformation, pKeyInfo, KeyInfoSize, &KeyInfoSizeNeeded);

			if (!NT_SUCCESS(Status) || (KeyInfoSize != KeyInfoSizeNeeded))
			{
				ZwClose(hKey);
				ExFreePoolWithTag(pKeyInfo, 0x1234);
				return 0;
			}

			ZwClose(hKey);
			ExFreePoolWithTag(pKeyInfo, 0x1234);

			return *(type*)((LONG64)pKeyInfo + pKeyInfo->DataOffset);
		}

		return 0;
	}
	__forceinline bool Write(UNICODE_STRING RegPath, UNICODE_STRING Key, PVOID Address, ULONG Type, ULONG Size)
	{
		bool Success = false;
		HANDLE hKey;
		OBJECT_ATTRIBUTES ObjAttr;
		NTSTATUS Status = STATUS_UNSUCCESSFUL;

		InitializeObjectAttributes(&ObjAttr, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		Status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &ObjAttr);

		if (NT_SUCCESS(Status))
		{
			Status = ZwSetValueKey(hKey, &Key, NULL, Type, Address, Size);

			if (NT_SUCCESS(Status))
				Success = true;

			ZwClose(hKey);
		}
		else {
			Status = ZwCreateKey(&hKey, KEY_ALL_ACCESS, &ObjAttr, 0, &RegPath, 0, 0);

			if (NT_SUCCESS(Status))
			{
				Status = ZwSetValueKey(hKey, &Key, NULL, Type, Address, Size);

				if (NT_SUCCESS(Status))
					Success = true;
			}
			ZwClose(hKey);
		}

		return Success;
	}
}