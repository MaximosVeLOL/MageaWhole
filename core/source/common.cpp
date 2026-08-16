#include <core/common.hpp>
#include <SDL3/SDL.h>
//#include <core/comp/renderer.hpp>
/*
EXPORT void WeAreASharedLibrary() {
	return;
}
*/
void Log(const char* pFormat, ...) {
	SDL_Log(pFormat);
}

void DisplayError(const char* pFormat, ...) {
	SDL_Log("(DisplayError) Going to show a messagebox with message (%s)", format(pFormat));
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error has occured!", format(pFormat), NULL);
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

string_size_t Strlen(const char* pString) {
	string_size_t ret = 0;
	while (pString[ret++]);
	return ret;
}