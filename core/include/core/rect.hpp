#ifndef __RECT_H__
#define __RECT_H__

#include <core/common.hpp>

template<typename P, typename S>
struct RectTemplate {
	P x = P(), y = P();
	S width = S(), height = S();

};

//SMH my head
typedef RectTemplate<POSITION_TYPE, SIZE_TYPE> Rect;

struct RRect {
	s16 x = 0, y = 0;
	//4096 max!
	u16 width : 12;
	u16 height : 12;
	//u8 __unused;
	operator Rect() {
		return { static_cast<POSITION_TYPE>(x), static_cast<POSITION_TYPE>(y), static_cast<SIZE_TYPE>(width), static_cast<SIZE_TYPE>(height) };
	}
};

#endif