#pragma once
#include <ntddk.h>



typedef struct ServiceDescriptorEntry {
    unsigned int* ServiceTableBase;
    unsigned int* ServiceCounterTableBase; //Used only in checked build   
    unsigned int NumberOfServices;
    unsigned char* ParamTableBase;
} ServiceDescriptorTableEntry_t, * PServiceDescriptorTableEntry_t;

//__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
//#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]  

typedef enum class _SYSTEM_INFORMATION_CLASS {

    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemNextEventIdInformation,
    SystemEventIdsInformation,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemPlugPlayBusInformation,
    SystemDockInformation,
    SystemPowerInformation,
    SystemProcessorSpeedInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation


} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;




typedef struct _SYSTEM_MODULE_ENTRY
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} SYSTEM_MODULE_ENTRY, * PSYSTEM_MODULE_ENTRY;


typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG Count;
    SYSTEM_MODULE_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI ZwQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    IN PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength);


typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION) (
    SYSTEM_INFORMATION_CLASS SystemInformationCLass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength);

extern ZWQUERYSYSTEMINFORMATION UD_ZwQuerySystemInformation;
