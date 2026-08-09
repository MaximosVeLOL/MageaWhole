#ifndef __ENGINE_H__
#define __ENGINE_H__

class Object;
struct File;
struct MIX_Audio;


typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;
typedef signed long long int s64;

struct EngineFunctions {
	//Console output
	void(*Log)(const char* pFormat, ...);
	//Request an asset of any type using the AL_ functions
	bool(*Asset_Request)(void** pOutput, const char* pDirectory, void(*AssetLoader)(void**, File));
	//Add an instance to the world, should probably make this easier
	void(*World_AddInstance)(Object* pObject);

	//Asset Loaders
	void (*AL_BasicAsset)(void**, File);
	void (*AL_Texture)(void**, File);
	void (*AL_Sound)(void**, File);
	void (*AL_Sprite)(void**, File);

	//Input_GetBinding functions to get if an input is down or not
	bool (*Input_GetBindingIsUp)(const char* pCode);
	bool (*Input_GetBindingWentDown)(const char* pCode);
	bool (*Input_GetBindingIsDown)(const char* pCode);
	bool (*Input_GetBindingWentUp)(const char* pCode);

	//Audio utility
	u8(*Audio_PlaySound)(MIX_Audio*);
	void(*Audio_StopChannel)(u8, s64);
	void(*Audio_PlayMusic)(MIX_Audio*, bool);
	void(*Audio_StopMusic)(s64);

};
EngineFunctions gEngine = { nullptr };

#define CONSOLE_LOG(format, ...) gEngineFunctions.Log(format);
#define ASSET_REQUEST(pOutput, pDirectory, AssetLoader) gEngine.Asset_Request(pOutput, pDirectory, AssetLoader)
#define ASSET_REQUEST_BASIC(pOutput, pDirectory) gEngine.Asset_Request(pOutput, pDirectory, gEngine.AL_BasicAsset)
#define ASSET_REQUEST_TEXTURE(pOutput, pDirectory) gEngine.Asset_Request(pOutput, pDirectory, gEngine.AL_Texture)
#define ASSET_REQUEST_AUDIO(pOutput, pDirectory) gEngine.Asset_Request((void**)pOutput, pDirectory, gEngine.AL_Sound)

#endif