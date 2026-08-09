#ifndef __MGUI_COMMON_H__
#define __MGUI_COMMON_H__

#include <core/common.hpp>
#include <core/renderer.hpp>

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