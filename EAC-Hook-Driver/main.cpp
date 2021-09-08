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
	//ZWQUERYSYSTEMINFORMATION UD_ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)SYSTEMSERVICE(ZwQuerySystemInformation);

}
//Driverless Driver (treat as shellcode entry-point)
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT drv_object, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(drv_object);
	UNREFERENCED_PARAMETER(reg_path);

	DRV_PRINT("Driver Successfully Loaded! Lets Go Baby! \n");

	Import_Dynamic_APIS();
	EAC_Hook::apply_all_data_ptr_hooks();

	return STATUS_SUCCESS;
}