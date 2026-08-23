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

char* format(const char* pFormat, ...) {

	va_list args;
	va_start(args, pFormat);
	char* ret = new char[256];
	
	if (vsnprintf(ret, 256, pFormat, args) < 0) {
		delete[] ret;
		Log("Failed to format printed string!");
		char* bruh = new char[SDL_strlen("formatted string") + 1];
		SDL_strlcpy(bruh, "formatted string", SDL_strlen("formatted string") + 1);
		va_end(args);
		return bruh;
	}
	//perror(ret);
	va_end(args);
	ret = (char*)SDL_realloc(ret, SDL_strlen(ret) + 1);
	return ret;

	return nullptr; //Should never happen
}

void DisplayError(const char* pFormat, ...) {
	SDL_Log("(DisplayError) Going to show a messagebox with message (%s)", format(pFormat));
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "An error has occured!", format(pFormat), NULL);
}

string_size_t Strlen(const char* pString) {
	string_size_t ret = 0;
	while (pString[ret++]);
	return ret;
}