// Created by John �kerblom 2012-07-??

#ifndef __HOOK_UTILS_H_DEF__
#define __HOOK_UTILS_H_DEF__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

VOID hook_hotpatch_x86(void *old_proc, const void *new_proc, void **orig_proc);
BOOL hook_unhotpatch_x86(void *old_proc);
BOOL hook_trampoline_dis_x86(void **old_proc, const void *new_proc);
BOOL hook_trampoline_dis_x86_by_name(const char *dll_name, const char *proc_name, void *new_proc, void **old_proc);
BOOL hook_trampoline_dis_x86_by_needle(HANDLE module, BYTE *needle, DWORD nsize, int offset_from_needle, void **old_proc, const void *new_proc);
BOOL hook_trampoline_dis_x86_by_needle_preserve(HANDLE module, BYTE *needle, DWORD nsize, int offset_from_needle, void **old_proc, const void *new_proc, void **preserved_old_proc);
BOOL unhook_trampoline_dis_x86_preserved(const void *old_proc, void *preserved_old_proc);

BYTE *hook_find_by_needle(HANDLE module, BYTE *needle, DWORD nsize);

// Convenience macro if using the following naming convention:
//      needle: needle_<name>
//      offset: off_<name>
//      original function: orig_<name>
//      hook function: hooked_<name>
#define HOOK_NEEDLE(m, fn) \
    hook_trampoline_dis_x86_by_needle(m, mem_##fn, sizeof(mem_##fn), off_##fn, (void **)&orig_##fn, hooked_##fn)
#define HOOK_NEEDLE_FAIL_MSG(m, fn) \
    if (FALSE == HOOK_NEEDLE(m, fn)) \
    { \
        MessageBoxA(NULL, #fn, "Hook failed", MB_ICONERROR); \
    }

#define HOOK_NEEDLE_PRESERVE(m, fn) \
    hook_trampoline_dis_x86_by_needle_preserve(m, mem_##fn, sizeof(mem_##fn), off_##fn, (void **)&orig_##fn, hooked_##fn, &preserved_##fn)
#define HOOK_NEEDLE_FAIL_MSG_PRESERVE(m, fn) \
    if (FALSE == HOOK_NEEDLE_PRESERVE(m, fn)) \
    { \
        MessageBoxA(NULL, #fn, "Hook failed", MB_ICONERROR); \
    }

#define UNHOOK_PRESERVED(fn) \
    unhook_trampoline_dis_x86_preserved((void **)orig_##fn, preserved_##fn)

#ifdef __cplusplus
}
#endif

#endif
