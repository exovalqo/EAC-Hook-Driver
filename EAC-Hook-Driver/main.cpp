#include <ntifs.h>
#include "utils.h"
#include "Undocumented_Functions.h"
#include "EAC-Hook.h"

ZWQUERYSYSTEMINFORMATION UD_ZwQuerySystemInformation = 0;

void Import_Dynamic_APIS()
{
	UNICODE_STRING function;
	RtlInitUnicodeString(&function, L"ZwQuerySystemInformation");
	
	UD_ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)MmGetSystemRoutineAddress(&function);
	if(UD_ZwQuerySystemInformation)
		DRV_PRINT("[+] Successfully found ZwQuerySystemInformation Address at 0x%X\n", UD_ZwQuerySystemInformation);
	else
		DRV_PRINT("[-] Failed to found ZwQuerySystemInformation Address\n");
	//ZWQUERYSYSTEMINFORMATION UD_ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)SYSTEMSERVICE(ZwQuerySystemInformation);

}

void DriverShellCode(IN PUNICODE_STRING FullImageName,				
					 IN HANDLE			ProcessId,                // pid into which image is being mapped
					 IN PIMAGE_INFO		ImageInfo)
{
	
	if (!ProcessId && wcsstr(FullImageName->Buffer, L"EasyAntiCheat.sys"))
	{ 
		DRV_PRINT("Executing Shellcode\n");
		Import_Dynamic_APIS();
		if (EAC_Hook::apply_all_data_ptr_hooks())
			DRV_PRINT("[+] Successfully hooked all dynamic imports\n");
	}
}
//Driverless Driver (treat as shellcode entry-point)
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT drv_object, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(drv_object);
	UNREFERENCED_PARAMETER(reg_path);

	DRV_PRINT("Driver Successfully Loaded! Lets Go Baby! \n");

	//Calling this section in DriverShellCode||
	// 
	//Import_Dynamic_APIS();
	//EAC_Hook::apply_all_data_ptr_hooks();
	// 
	//Calling this section in DriverShellCode||

	return PsSetLoadImageNotifyRoutine(&DriverShellCode);
}