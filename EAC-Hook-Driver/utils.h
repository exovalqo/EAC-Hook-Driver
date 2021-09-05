#pragma once
#include <ntifs.h>
#include <intrin.h>

#define DRV_PRINT(msg, ...) DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[Vital Hook] -> " msg,  __VA_ARGS__)



