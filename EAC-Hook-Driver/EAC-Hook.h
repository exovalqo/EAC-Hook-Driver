#pragma once
#include <ntifs.h>

#define RVA_IoWMIQueryAllData 0x73000
#define EAC_FILE_PATH "\\??\\C:\\Program Files (x86)\\EasyAntiCheat\\EasyAntiCheat.sys"


typedef struct _UD_IMPORT_ENTRY
{
	UINT64 RVA;
	char* API_Name;

} UD_IMPORT_ENTRY;

namespace EAC_Hook
{
	extern UD_IMPORT_ENTRY dynamic_imports_rva[1];

	bool apply_all_data_ptr_hooks();
};
