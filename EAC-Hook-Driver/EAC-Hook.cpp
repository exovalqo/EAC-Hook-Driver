#include "EAC-Hook.h"
#include "utils.h"

namespace EAC_Hook
{
	UD_IMPORT_ENTRY  dynamic_imports_rva[1] = { {RVA_IoWMIQueryAllData, "IoWMIQueryAllData"} };
	bool apply_all_data_ptr_hooks()
	{
		PVOID system_buffer = ExAllocatePoolWithTag(NonPagedPool, sizeof(UINT64), 'exoP');
		*(UINT64*)system_buffer = 0xdeadbeef;
		for (int i = 0; i < (sizeof(dynamic_imports_rva) / sizeof(dynamic_imports_rva[i])); i++)
		{
			UINT64 VA = Calc_VA(EAC_FILE_PATH, dynamic_imports_rva[i].RVA);
			if (VA != 0)
			{

				MM_COPY_ADDRESS t_buffer = { system_buffer };
				if(MmCopyMemory((PVOID)VA, t_buffer, sizeof(UINT64), MM_COPY_MEMORY_VIRTUAL, 0) == 0)
					DRV_PRINT("Successfully Installed Hook On %s", dynamic_imports_rva[i].API_Name);
				else
					DRV_PRINT("Failed To Install Hook On %s", dynamic_imports_rva[i].API_Name);
			}


		}

		ExFreePoolWithTag(system_buffer, 'exoP');

		return true;




	}
}