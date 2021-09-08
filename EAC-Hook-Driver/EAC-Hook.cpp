#include "EAC-Hook.h"
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
				DRV_PRINT("Before: %s pointer value was 0x%Xll\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
				MM_COPY_ADDRESS t_buffer = { system_buffer };
				SIZE_T returnBytes;

				if (MmCopyMemory((PVOID)VA, t_buffer, sizeof(UINT64), MM_COPY_MEMORY_VIRTUAL, &returnBytes) == STATUS_SUCCESS)
				{
					DRV_PRINT("[+] Successfully Installed Hook On %s at 0x%Xll\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
					DRV_PRINT("After Swap: %s pointer value is 0x%Xll\n", dynamic_imports_rva[i].API_Name, *(UINT64*)VA);
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
}