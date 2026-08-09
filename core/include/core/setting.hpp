#ifndef __SETTING_H__
#define __SETTING_H__

//Basically, ConVars, but they use SDL3 Properties

#include <SDL3/SDL.h>
#include <core/common.hpp>
#include <core/file.hpp>

namespace Settings {
	SDL_PropertiesID gID = 0;
	
	void Init() {
		File pFile("_dev/settings.txt");
		if (!pFile) return;
		gID = SDL_CreateProperties();
	}
}

#endif