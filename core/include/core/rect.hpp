#ifndef __RECT_H__
#define __RECT_H__

#include <core/common.hpp>

template<typename P, typename S>
struct RectTemplate {
	P x = P(), y = P();
	S width = S(), height = S();

};

//SMH my head
typedef RectTemplate<POSITION_TYPE, SIZE_TYPE> rect;

struct RRect {
	s16 x = 0, y = 0;
	//4096 max!
	u16 width : 12;
	u16 height : 12;
	//u8 __unused;
	operator rect() {
		return { static_cast<short>(x), static_cast<short>(y), static_cast<u8>(width), static_cast<u8>(height) };
	}
};

#endif