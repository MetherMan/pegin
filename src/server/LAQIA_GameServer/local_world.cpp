#include <stdio.h>
#include <string.h>

extern "C" FILE* __real_fopen(const char* filename, const char* mode);

// Link-time wrapping preserves the native world's existing spawn corrections.
// DATA is supported by every released family updater; saves are never involved.
extern "C" FILE* __wrap_fopen(const char* filename, const char* mode)
{
    if (filename && mode && mode[0] == 'r' && !strchr(mode, '+') &&
        !strncmp(filename, "./MAP/", 6) && !strchr(filename + 6, '/')) {
        char overlay[256];
        snprintf(overlay, sizeof(overlay), "./DATA/LocalWorld_%s", filename + 6);
        FILE* fp = __real_fopen(overlay, mode);
        if (fp) return fp;
    }
    return __real_fopen(filename, mode);
}
