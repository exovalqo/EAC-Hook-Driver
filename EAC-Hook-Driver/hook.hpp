#pragma once
#include <ntifs.h>
#include <intrin.h>
#include "utils.h"

typedef struct _inline_hook_t
{
    unsigned char code[14];
    unsigned char jmp_code[14];

    void* address;
    void* hook_address;
} inline_hook_t, * pinline_hook_t;

void make_inline_hook(pinline_hook_t, void*, void*, bool);
void enable_inline_hook(pinline_hook_t);
void disable_inline_hook(pinline_hook_t);

inline void make_inline_hook(pinline_hook_t hook, void* hook_from, void* hook_to, bool install)
{
    unsigned char jmp_code[14] = { 0xff, 0x25, 0x0, 0x0, 0x0, 0x0, // jmp    QWORD PTR[rip + 0x0]

                                     // jmp address...
                                     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    // save original bytes, and hook related addresses....
    hook->address = hook_from;
    hook->hook_address = hook_to;
    RtlCopyMemory(hook->code, hook_from, sizeof hook->code);

    // setup hook...
    RtlCopyMemory(jmp_code + 6, &hook_to, sizeof hook_to);
    RtlCopyMemory(hook->jmp_code, jmp_code, sizeof jmp_code);
    if (install)
        enable_inline_hook(hook);
}

inline void enable_inline_hook(pinline_hook_t hook)
{
    KernelMemoryWrite(hook->address, hook->jmp_code, sizeof(hook->jmp_code));
}

inline void disable_inline_hook(pinline_hook_t hook)
{
    KernelMemoryWrite(hook->address, hook->code, sizeof(hook->code));
}
