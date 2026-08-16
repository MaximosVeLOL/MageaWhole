#ifndef __API_H__
#define __API_H__

#include <core/common.hpp>
#include <core/file.hpp>

#define IN_ENGINE 1

#if IN_ENGINE

#include <core/comp/renderer.hpp>
#include <core/comp/input.hpp>
#include <SDL3_mixer/SDL_mixer.h>
#include <magea/object.hpp>

#else

class Object;

#endif

namespace Game {
	struct API {
		//Console output
		void(*Log)(const char* pFormat, ...);
		//Formatted string
		char* (*format)(const char* pFormat, ...);
		//Request an asset of any type using the AL_ functions
		bool(*Asset_Request)(void** pOutput, const char* pDirectory, void(*AssetLoader)(void**, FileSystem::File), bool pDirectoryIsAsset);
		//Add an instance to the world, should probably make this easier
		void(*World_AddInstance)(Object* pObject);
		void(*World_ResetObjects)();
		void(*World_ResetCollision)();
		void(*World_Reset)();
		void(*World_LoadFromFile)(const char*, bool);


		//Asset Loaders
		void (*AL_BasicAsset)(void**, FileSystem::File);
		void (*AL_Texture)(void**, FileSystem::File);
		void (*AL_Sound)(void**, FileSystem::File);
		void (*AL_Sprite)(void**, FileSystem::File);

	};
}

#endif
