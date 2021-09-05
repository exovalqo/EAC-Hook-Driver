#include <ntifs.h>
#include "utils.h"

//Driverless Driver (treat as shellcode entry-point)
NTSTATUS DriverEntry(PDRIVER_OBJECT drv_object, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(drv_object);
	UNREFERENCED_PARAMETER(reg_path);

	DRV_PRINT("Driver Successfully Loaded! Lets Go Baby! \n");

	return STATUS_SUCCESS;
}