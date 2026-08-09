#ifndef __ENGINE_API_H__
#define __ENGINE_API_H__

#include <engine/common.hpp>

struct MIX_Audio;

class Object;
struct File;

namespace Input {
	typedef const char* inputname_t;
}

namespace Engine {
	struct API {
		//Console output
		void(*Log)(const char* pFormat, ...);
		//Formatted string
		char* (*format)(const char* pFormat, ...);
		//Request an asset of any type using the AL_ functions
		bool(*Asset_Request)(void** pOutput, const char* pDirectory, void(*AssetLoader)(void**, File), bool pDirectoryIsAsset);
		//Add an instance to the world, should probably make this easier
		void(*World_AddInstance)(Object* pObject);

		//Asset Loaders
		void (*AL_BasicAsset)(void**, File);
		void (*AL_Texture)(void**, File);
		void (*AL_Sound)(void**, File);
		void (*AL_Sprite)(void**, File);

		//Input_GetBinding functions to get if an input is down or not
		bool (*Input_GetBindingIsUp)(Input::inputname_t pCode);
		bool (*Input_GetBindingWentDown)(Input::inputname_t pCode);
		bool (*Input_GetBindingIsDown)(Input::inputname_t pCode);
		bool (*Input_GetBindingWentUp)(Input::inputname_t pCode);

		//Audio utility
		u8(*Audio_PlaySound)(MIX_Audio*);
		void(*Audio_StopChannel)(u8, s64);
		void(*Audio_PlayMusic)(MIX_Audio*, bool);
		void(*Audio_StopMusic)(s64);

	};

	extern API gEngine;
}

namespace Asset {
	inline bool Request(void** pOutput, const char* pDirectory, void(*AssetLoader)(void**, File), bool pDirectoryIsAsset = true) {
		return Engine::gEngine.Asset_Request(pOutput, pDirectory, AssetLoader, pDirectoryIsAsset);
	}
	inline bool RequestTexture(void** pOutput, const char* pDirectory, bool pDirectoryIsAsset = true) {
		return Request(pOutput, pDirectory, Engine::gEngine.AL_Texture, pDirectoryIsAsset);
	}
	inline bool RequestSound(void** pOutput, const char* pDirectory, bool pDirectoryIsAsset = true) {
		return Request(pOutput, pDirectory, Engine::gEngine.AL_Sound, pDirectoryIsAsset);
	}
	inline bool RequestSprite(void** pOutput, const char* pDirectory, bool pDirectoryIsAsset = true) {
		return Request(pOutput, pDirectory, Engine::gEngine.AL_Sprite, pDirectoryIsAsset);
	}
}
namespace World {
	inline void AddObject(Object* pObject) {
		Engine::gEngine.World_AddInstance(pObject);
	}
}
namespace Input {
	inline bool GetBindingIsUp(const char* pCode) {
		return Engine::gEngine.Input_GetBindingIsUp(pCode);
	}
	inline bool GetBindingWentDown(const char* pCode) {
		return Engine::gEngine.Input_GetBindingWentDown(pCode);
	}
	inline bool GetBindingIsDown(const char* pCode) {
		return Engine::gEngine.Input_GetBindingIsDown(pCode);
	}
	inline bool GetBindingWentUp(const char* pCode) {
		return Engine::gEngine.Input_GetBindingWentUp(pCode);
	}
}

inline void Log(const char* pFormat) {
	Engine::gEngine.Log(pFormat);
}
inline void format(const char* pFormat) {
	Engine::gEngine.format(pFormat);
}

#endif