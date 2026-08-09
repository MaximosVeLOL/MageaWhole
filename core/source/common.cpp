#include <core/common.hpp>
#include <SDL3/SDL.h>
#include <core/renderer.hpp>


void Log(const char* pFormat, ...) {
	SDL_Log(pFormat);
}

void DisplayError(const char* pFormat, ...) {
	SDL_Log(pFormat);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error has occured!", format(pFormat), Render::GetWindow());
}

char* format(const char* pFormat, ...) {
	va_list args;
	va_start(args, pFormat);
	char* ret = new char[256];
	SDL_vsnprintf(ret, 255, pFormat, args);
	va_end(args);
	ret = (char*)SDL_realloc(ret, SDL_strlen(ret) + 1);
	

	return ret;
}