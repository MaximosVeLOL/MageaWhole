#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <SDL3/SDL.h>

#include <core/common.hpp> //For u8
#include <core/rect.hpp> //For RRect
#include <core/vector.hpp> //For RVector

#include <core/comp/component.hpp>

COMPONENT_DEFINE_START(useRender, Render, true)
constexpr bool sUseHA = true;

constexpr u32 WINDOW_WIDTH = 960;
constexpr u32 WINDOW_HEIGHT = 540;

struct Color {
	u8 r = 0, g = 0, b = 0;
	operator SDL_Color() {
		return { r, g, b, 0xFF };
	}
};

//Alignment for the text
//These should both only have 3 values total
struct AlignMode {
	u8 horizontal : 4; //16 total values
	u8 vertical : 4; //16 total values
};


#define A_LEFT 0x00 //		H
#define A_TOP 0x00 //		V
#define A_MIDDLE 0x01 //	Both
#define A_RIGHT 0x02 //		H
#define A_BOTTOM 0x02 //	V

//For the text
CORE_API extern AlignMode gAlignMode;

//Needed by both versions
//SDL_Window* gWindow = nullptr;

//Can either be a SDL_Window or a SDL_Renderer


//Can either be a SDL_Texture* or a SDL_Surface* depending on sUseHA
//typedef void* texture_t;


struct texture {
	void* mTexture = nullptr;

	s32 GetWidth() {
		if (sUseHA) {
			return AsTexture()->w;
		}
		return AsSurface()->w;
	}

	s32 GetHeight() {
		if (sUseHA) {
			return AsTexture()->h;
		}
		return AsSurface()->h;
	}

	void Destroy() {
		if (sUseHA) {
			SDL_DestroyTexture(AsTexture());
			return;
		}
		SDL_DestroySurface(AsSurface());
	}

	operator void* () {
		return mTexture;
	}
	
	operator SDL_Texture* () {
		return static_cast<SDL_Texture*>(mTexture);
	}
	operator SDL_Surface* () {
		return static_cast<SDL_Surface*>(mTexture);
	}
	
	SDL_Texture* AsTexture() {
		return static_cast<SDL_Texture*>(mTexture);
	}
	SDL_Surface* AsSurface() {
		return static_cast<SDL_Surface*>(mTexture);
	}

	~texture() {
		Destroy();
	}
};
//752 bytes
CORE_EXPORT SDL_Renderer* GetRender();
//296 bytes
CORE_EXPORT SDL_Surface* GetSurface();
//456 bytes
CORE_EXPORT SDL_Window* GetWindow();

//There can only be a renderer or a surface, so not both. This means that we use 456 bytes + (752 for HA, 296 for no HA)
//We atleast need 1KB of memory for the renderer

//Stuff that relates to drawing to the screen
CORE_EXPORT void Clear();

CORE_EXPORT void SetColor(SDL_Color pColor);

CORE_EXPORT void Rect(RRect pPosition);
CORE_EXPORT void Rect(RRect pPosition, SDL_Color pColor);

CORE_EXPORT void FillRect(RRect pPosition);
CORE_EXPORT void FillRect(RRect pPosition, SDL_Color pColor);

CORE_EXPORT void Texture(texture* pTexture, RRect pPosition);
CORE_EXPORT void Texture(texture* pTexture, RRect pPosition, RRect pPart);

CORE_EXPORT s16 Text(vector pPosition, const char* pFormat, ...);
CORE_EXPORT s16 GetTextWidth(const char* pFormat, ...);
CORE_EXPORT s16 GetTextHeight();

CORE_EXPORT void ResetFormatting();

CORE_EXPORT void Present();
/* [[nodiscard]]*/ CORE_EXPORT COMPONENT_INCLUDE_INIT;

CORE_EXPORT COMPONENT_INCLUDE_UNLOAD;

COMPONENT_DEFINE_END

#endif