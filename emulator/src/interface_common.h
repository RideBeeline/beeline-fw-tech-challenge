#ifndef INTERFACE_COMMON_H_42350
#define INTERFACE_COMMON_H_42350

#if defined(__GNUC__)
#define EXPORT_TO_LIB extern "C" __attribute__((visibility("default")))
#elif defined(__MINGW32__)
#define EXPORT_TO_LIB extern "C" __declspec(dllexport)
#else
#error "Unknown target platform."
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#endif
