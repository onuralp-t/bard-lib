#include <stdint.h> // IWYU pragma: keep
#include <stddef.h> // IWYU pragma: keep
#include <stdbool.h> // IWYU pragma: keep

#define WIN32_LEAN_AND_MEAN
#include "windows.h" // IWYU pragma: keep

typedef void* (__cdecl *RtlCopyMemory_)(void*, const void*, size_t);
typedef void* (__cdecl *RtlMoveMemory_)(void*, const void*, size_t);
typedef void* (__cdecl *RtlFillMemory_)(void*, size_t, int);
typedef size_t (__cdecl *RtlCompareMemory_)(const void*, const void*, size_t);

typedef int (__cdecl *RtlQueryPerformanceFrequency_)(uint64_t*);
typedef int (__cdecl *RtlQueryPerformanceCounter_)(uint64_t*);

union {
    struct {
        RtlCopyMemory_ copy;
        RtlMoveMemory_ move;
        RtlFillMemory_ set;
        RtlCompareMemory_ cmp;
    };
    struct {
        FARPROC fcopy;
        FARPROC fmove;
        FARPROC fset;
        FARPROC fcmp;
    };
} membuiltin_win32 = {0};

union {
    struct {
        RtlQueryPerformanceFrequency_ qpc_freq;
        RtlQueryPerformanceCounter_ qpc_count;
    };
    struct {
        FARPROC fqpc_freq;
        FARPROC fqpc_count;
    };
} time_win32 = {0};

struct {
    char buffer[1024];
    HANDLE stdout_handle;
    DWORD scratch_int;
} stdout_win32 = {0};

struct {
    uint64_t freq;
    uint64_t previous_count;
    uint64_t current_count;
} time_state_win32 = {0};

inline void* memcpy(void* dst, const void* src, size_t n) {return membuiltin_win32.copy(dst, src, n);}
inline void* memmove(void* dst, const void* src, size_t n) {return membuiltin_win32.move(dst, src, n);}
inline void* memset(void* dst, int c, size_t n) {return membuiltin_win32.set(dst, n, c);}
inline int memcmp(const void* p1, const void* p2, size_t n) {
    size_t same = membuiltin_win32.cmp(p1, p2, n);
    if (same == n) return 0;
    const uint8_t *a = (const uint8_t*)p1 + same;
    const uint8_t *b = (const uint8_t*)p2 + same;
    return (*a > *b) - (*a < *b);
}

double time_in_seconds(void) {
    time_win32.qpc_count((void*)&time_state_win32.current_count);
    double ret = (double)((time_state_win32.current_count - time_state_win32.previous_count) / (double)time_state_win32.freq);
    time_state_win32.previous_count = time_state_win32.current_count;
    return ret;
}

#define BA_ALIGN (sizeof(size_t))
#define BA_ONES ((size_t)-1/UCHAR_MAX)
#define BA_HIGHS (BA_ONES * (UCHAR_MAX/2+1))
#define BA_HASZERO(x) ((x)-(BA_ONES & ~(x) & BA_HIGHS))

static size_t bard_strlen_win32(const char *s) // TODO: move this to bardlib
{
	const char *a = s;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	const word *w;
	for (; (uintptr_t)s % BA_ALIGN; s++) if (!*s) return s-a;

	for (w = (const void *)s; !BA_HASZERO(*w); w++);

	s = (const void *)w;
#endif
	for (; *s; s++);
	return s-a;
}

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

void puts_win32(char* msg) {
    WriteFile(stdout_win32.stdout_handle, msg, (DWORD)bard_strlen_win32(msg), &stdout_win32.scratch_int, NULL);
}

// TODO: defines for different compilers
__attribute__((format(printf, 1, 2))) // 1-indexed position of fmt string, and the variadic params
void printf_win32(char* fmt, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, fmt);

    stbsp_vsprintf(stdout_win32.buffer, fmt, args);
    puts_win32(stdout_win32.buffer);
}

static void init_layer_win32() {
    HMODULE ntdll_handle = GetModuleHandleW(L"ntdll.dll");
    // builtins
    membuiltin_win32.fcopy = GetProcAddress(ntdll_handle, "RtlCopyMemory");
    membuiltin_win32.fmove = GetProcAddress(ntdll_handle, "RtlMoveMemory");
    membuiltin_win32.fset = GetProcAddress(ntdll_handle, "RtlFillMemory");
    membuiltin_win32.fcmp = GetProcAddress(ntdll_handle, "RtlCompareMemory");
    // time
    time_win32.fqpc_freq = GetProcAddress(ntdll_handle, "RtlQueryPerformanceFrequency");
    time_win32.fqpc_count = GetProcAddress(ntdll_handle, "RtlQueryPerformanceCounter");
    // time state init
    time_win32.qpc_count((void*)&time_state_win32.previous_count);
    time_win32.qpc_freq((void*)&time_state_win32.freq);
    // stdout handle
    stdout_win32.stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

int user_main();

__attribute((force_align_arg_pointer, used, noreturn))
void entry_point(void)
{
    init_layer_win32();

    int result = user_main();

    ExitProcess(result);
}