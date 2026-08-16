#ifndef __COMMON_H__
#define __COMMON_H__

#include <core/comopt.hpp>
#if CO_C_BASE == CO_C_STANDARD_C
#include <stdio.h>
#endif
#if CO_C_BASE == CO_C_STANDARD_CPP

#endif
#if CO_C_BASE == CO_C_BASE_SDL3
#include <SDL3/SDL.h>

#endif

#ifdef _WIN32
#ifdef core_EXPORTS  // CMake automatically defines <TargetName>_EXPORTS when building the DLL
#define CORE_EXPORT __declspec(dllexport) extern
#define CORE_API __declspec(dllexport)
#else
#define CORE_EXPORT __declspec(dllimport) extern
#define CORE_API __declspec(dllimport)
#endif
#else
#define CORE_EXPORT
#endif

#include <stdfloat>

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long int s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef u16 string_size_t;

CORE_EXPORT void Log(const char* pFormat, ...);

CORE_EXPORT void DisplayError(const char* pFormat, ...);

CORE_EXPORT char* format(const char* pFormat, ...);

CORE_EXPORT string_size_t Strlen(const char* pString);

//From object.h
typedef s16 POSITION_TYPE;
typedef u8 SIZE_TYPE;
typedef float DECIMAL_TYPE;


#define ARRAY_SIZE(name, type) (sizeof(name) / sizeof(type))



#endif