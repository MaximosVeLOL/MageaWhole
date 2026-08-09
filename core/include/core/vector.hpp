#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <core/common.hpp>
#include <core/math.hpp>
#include <SDL3/SDL.h>

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

//typedef VectorTemplate<s16> RVector;

inline POSITION_TYPE vector_distance(vector* p1, vector* p2) {
	signed long int x = SDL_lroundf(SDL_sqrt(FAST_SQUARE(p2->x - p1->x) + FAST_SQUARE(p2->y - p1->y)));
	return static_cast<POSITION_TYPE>(ABS(x));
}

#endif