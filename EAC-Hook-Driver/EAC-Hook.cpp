#include "EAC-Hook.h"
#include "hook.hpp"
#include "utils.h"

namespace EAC_Hook
{
	NTSTATUS status = false;
	UD_IMPORT_ENTRY  dynamic_imports_rva[1] = { {RVA_IoWMIQueryAllData, "IoWMIQueryAllData"} };
	bool apply_all_data_ptr_hooks()
	{
		PVOID system_buffer = ExAllocatePoolWithTag(NonPagedPool, sizeof(UINT64), 'exoP');
		if (!system_buffer)
		{
			DRV_PRINT("[-] Failed to Allocate Pool in apply_all_data_ptr_hooks\n");
			return false;
		}
		*(UINT64*)system_buffer = 0xdeadbeef;
		for (int i = 0; i < (sizeof(dynamic_imports_rva) / sizeof(dynamic_imports_rva[i])); i++)
		{
			UINT64 VA = Calc_VA(EAC_FILE_PATH, dynamic_imports_rva[i].RVA);
			if (VA != 0)
			{
				DRV_PRINT("Before: %s pointer value was 0x%llX\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
				MM_COPY_ADDRESS t_buffer = { system_buffer };
				SIZE_T returnBytes;

				if (MmCopyMemory((PVOID)VA, t_buffer, sizeof(UINT64), MM_COPY_MEMORY_VIRTUAL, &returnBytes) == STATUS_SUCCESS)
				{
					DRV_PRINT("[+] Successfully Installed Hook On %s at 0x%llX\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
					DRV_PRINT("After Swap: %s pointer value is 0x%llX\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
					status = true;
				}
				else
				{
					DRV_PRINT("[-] Failed To Install Hook On %s\n", dynamic_imports_rva[i].API_Name);
					status = false;
				}
			}
			else
				DRV_PRINT("[-] Calc_VA Failed\n");


		}

		ExFreePoolWithTag(system_buffer, 'exoP');

		return status;




	}

	inline_hook_t hook_info;
	EAC_GetExport originalGetExport = 0;

	UINT64 hkGetExport(UINT64 rcx, UINT64 rdx)
	{
		disable_inline_hook(&hook_info);
		UINT64 returnValue = originalGetExport(rcx, rdx);
		DRV_PRINT("[!]GetExport(0x%llX, 0x%llX) = 0x%llX\n", rcx, rdx,  returnValue);
		enable_inline_hook(&hook_info);
		return returnValue;
	}
	bool hook_Get_Export()
	{
		UINT64 get_export_va = Calc_VA(EAC_FILE_PATH, EAC_GET_EXPORT_FUNC_RVA);
		originalGetExport = (EAC_GetExport)get_export_va;
		DRV_PRINT("[!] originalGetExport = 0x%11X\n", originalGetExport);
		make_inline_hook(&hook_info, (void*)get_export_va, &hkGetExport, true);

		return true;
	}
}