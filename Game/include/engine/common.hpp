#ifndef __ENGINE_COMMON_H__
#define __ENGINE_COMMON_H__

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long int s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef u16 string_size_t;

//From object.h
typedef s16 POSITION_TYPE;
typedef u8 SIZE_TYPE;
typedef float DECIMAL_TYPE;


#define ARRAY_SIZE(name, type) (sizeof(name) / sizeof(type))

#define EXPORT extern "C" __declspec(dllexport)

#endif