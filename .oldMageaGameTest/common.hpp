#ifndef __COMMON_H__
#define __COMMON_H__

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long int s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

template<typename T>
struct VectorTemplate;

template<typename T, typename S>
struct RectTemplate;

struct rect;

struct RRect;

typedef VectorTemplate<u16>


struct RVector;

extern void Log(const char* pFormat, ...);

#endif