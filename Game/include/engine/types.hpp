#ifndef __ENGINE_TYPES_H__
#define __ENGINE_TYPES_H__

//#include <SDL3/SDL.h>

//From object.h
typedef s16 POSITION_TYPE;
typedef u8 SIZE_TYPE;
typedef float DECIMAL_TYPE;

template<typename P, typename S>
struct RectTemplate {
	P x = P(), y = P();
	S width = S(), height = S();

};

//SMH my head
typedef RectTemplate<POSITION_TYPE, SIZE_TYPE> rect;

struct RRect {
	s16 x = 0, y = 0;
	//4
	u16 width : 12;
	u16 height : 12;
	//u8 __unused;
	operator rect() {
		return { static_cast<short>(x), static_cast<short>(y), static_cast<u8>(width), static_cast<u8>(height) };
	}
};


template<typename T>
struct VectorTemplate {
	T x = T(), y = T();
	void operator +=(VectorTemplate pOther) {
		x += pOther.x;
		y += pOther.y;
	}
};

typedef VectorTemplate<POSITION_TYPE> vector;
static_assert(sizeof(vector) == sizeof(POSITION_TYPE) * 2, "Vector size includes more than x and y!");

typedef VectorTemplate<u16> RVector;

#define FAST_SQUARE(x) ((x) << 2)
#define ABS(x) ((x) >= 0 ? 1 : 0)

inline POSITION_TYPE vector_distance(vector* p1, vector* p2) {
	return 0;
	//signed long int x = SDL_lroundf(SDL_sqrt(FAST_SQUARE(p2->x - p1->x) + FAST_SQUARE(p2->y - p1->y)));
	//return static_cast<POSITION_TYPE>(ABS(x));
}

#endif