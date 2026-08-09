#include <core/math.hpp>
#include <SDL3/SDL.h>

template<typename R, typename T>
R testGrid(T x, unsigned char pGrid) {
	return static_cast<R>(SDL_lroundf(x / pGrid) * pGrid);
}