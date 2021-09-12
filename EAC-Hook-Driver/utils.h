#pragma once
#include <ntifs.h>
#include "Undocumented_Functions.h"


#define DRV_PRINT(msg, ...) DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[Vital Hook] -> " msg,  __VA_ARGS__)

//NTSYSAPI LONG NTAPI RtlCompareString( _In_ const STRING * String1, _In_ const STRING * String2, _In_ BOOLEAN CaseInSensitive);
inline bool Get_Base_Address(IN char* file_path, OUT PVOID* ImageBase, OUT ULONG* ImageSize)
{
	STRING f_path;
	RtlInitAnsiString(&f_path, file_path);
	//Add Error Checking (Create #define)
	ULONG mod_buffer_length = 0;
	UD_ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemModuleInformation, 0, mod_buffer_length, &mod_buffer_length);


	PSYSTEM_MODULE_INFORMATION pMods_List = (PSYSTEM_MODULE_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, mod_buffer_length, 'modL');
	UD_ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)0x0B, pMods_List, mod_buffer_length, &mod_buffer_length);

	PSYSTEM_MODULE_ENTRY curr_Mod = pMods_List->Module;
	STRING current_mod_path;

	PVOID Image_Base;
	ULONG Image_Size;

	bool status = false;


	for (ULONG i = 0; i < pMods_List->Count; i++)
	{
		RtlInitAnsiString(&current_mod_path, (PCSZ)curr_Mod[i].FullPathName);
		//DRV_PRINT("Current Module Name: %s\n", (PCSZ)curr_Mod[i].FullPathName);
		if (RtlCompareString(&f_path, &current_mod_path, TRUE) == 0)
		{
			*ImageBase = curr_Mod[i].ImageBase;
			*ImageSize = curr_Mod[i].ImageSize;
			return true;
		}

	}

	return false;
}

inline ULONG64 Calc_VA(char* file_path, UINT32 RVA)
{
	PVOID ImageBase;
	ULONG ImageSize;
	if (Get_Base_Address(file_path, &ImageBase, &ImageSize))
		return (UINT64)(ImageBase)+RVA;
	DRV_PRINT("[-] Failed to get base address of %s\n", file_path);
	return 0;
}

inline NTSTATUS KernelMemoryWrite(PVOID address, PVOID buffer, ULONG size)
{
	PMDL mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);

	if (!mdl)
	{
		return STATUS_UNSUCCESSFUL;
	}

	MmProbeAndLockPages(mdl, KernelMode, IoWriteAccess);

	PVOID mapped_page = MmMapLockedPagesSpecifyCache(mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);

	if (!mapped_page)
	{
		MmUnlockPages(mdl);
		IoFreeMdl(mdl);
		return STATUS_UNSUCCESSFUL;
	}

	NTSTATUS status = MmProtectMdlSystemAddress(mdl, PAGE_READWRITE);

	if (!NT_SUCCESS(status))
	{
		MmUnmapLockedPages(mapped_page, mdl);
		MmUnlockPages(mdl);
		IoFreeMdl(mdl);
		return STATUS_UNSUCCESSFUL;
	}

	//status = MmProtectMdlSystemAddress(mdl, PAGE_READWRITE);

	RtlCopyMemory(mapped_page, buffer, size);

	MmUnmapLockedPages(mapped_page, mdl);
	MmUnlockPages(mdl);
	IoFreeMdl(mdl);

	return STATUS_SUCCESS;
}


