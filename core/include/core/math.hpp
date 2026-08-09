#ifndef __MATH_H__
#define __MATH_H__

#define MIN(v, m) (v < m ? m : v)
#define MAX(v, m) (v > m ? m : v)
#define CLAMP(v, min, max) (v < min ? min : (v > max ? max : v))

#define SIGN(x) ((x) >= 0 ? 1 : -1)

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define FAST_SQUARE(x) ((x) << 2)

template<typename R, typename T>
inline R Grid(T x, unsigned char pGrid) {
	return static_cast<R>(SDL_lroundf(x / pGrid) * pGrid);
}

#endif