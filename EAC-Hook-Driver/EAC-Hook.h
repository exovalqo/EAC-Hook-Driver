#pragma once
#include <ntifs.h>

#define RVA_IoWMIQueryAllData 0x73000
#define EAC_FILE_PATH "\\??\\C:\\Program Files (x86)\\EasyAntiCheat\\EasyAntiCheat.sys"
#define EAC_GET_EXPORT_FUNC_RVA 0xCC18

typedef UINT64(*EAC_GetExport)(UINT64 rcx, UINT64 rdx);
typedef struct _UD_IMPORT_ENTRY
{
	UINT64 RVA;
	char* API_Name;

} UD_IMPORT_ENTRY;

namespace EAC_Hook
{
	extern EAC_GetExport originalGetExport;

	extern UD_IMPORT_ENTRY dynamic_imports_rva[1];

	bool apply_all_data_ptr_hooks();

	bool hook_Get_Export();
};
