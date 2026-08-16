#ifndef __MGUI_COMMON_H__
#define __MGUI_COMMON_H__

#include <core/common.hpp>
#include <core/comp/renderer.hpp>

//#include <core/common.hpp>

#ifdef _WIN32
#ifdef MGUI_EXPORTS  // CMake automatically defines <TargetName>_EXPORTS when building the DLL
#define MGUI_API __declspec(dllexport)
#else
#define MGUI_API __declspec(dllimport)
#endif
#else
#define MGUI_API
#endif


namespace MGUI {
	constexpr u8 PARENT_NONE = 255;
}


//A color or a texture!
struct GUIStyle {
	//Are we using a texture or color ID?
	bool isTexture = false;
	//A Style::ID Enum
	u8 id = 0;
};

#endif