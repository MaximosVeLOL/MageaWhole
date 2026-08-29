#ifndef __COMOPT_H__
#define __COMOPT_H__

#pragma region Core Stuff

enum CO_C_BASE_ENUM : unsigned char {
	CO_C_BASE_STANDARD_CPP = 0, //Use C++ STL
	CO_C_BASE_STANDARD_C = 1, //Use C STL
	CO_C_BASE_SDL3 = 2,
};

#define CO_C_BASE CO_C_BASE_SDL3


#pragma endregion

#pragma region Optimizations

#define CO_O_MULTI_THREADING 0

#define CO_O_LOWRAM 0

#define CO_O_BADCPU 0

#pragma endregion

#define CO_PACKED_FILES 0

//Do we use dll files for the game or angelscript for game stuff?
#define CO_G_SCRIPTING 0

#endif