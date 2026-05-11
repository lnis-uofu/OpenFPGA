#ifndef OPENFPGA_WINDOWS_COMPATIBILITY_H
#define OPENFPGA_WINDOWS_COMPATIBILITY_H

/***
 * Windows MSVC Compatibility Fixes
 *
 * MSVC's <locale> header defines template functions isdigit() and isxdigit()
 * which conflict with macros from <cctype> if included in certain order.
 * This header undefines these macros to prevent conflicts.
 ***/

#include <cctype>

#ifdef isdigit
#undef isdigit
#endif

#ifdef isxdigit
#undef isxdigit
#endif

#endif