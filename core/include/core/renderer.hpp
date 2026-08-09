#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <SDL3/SDL.h>

#include <core/common.hpp> //For u8
#include <core/rect.hpp> //For RRect
#include <core/vector.hpp> //For RVector

namespace Render {

	constexpr bool sUseHA = true;

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
	extern AlignMode gAlignMode;


	//Needed by both versions
	//SDL_Window* gWindow = nullptr;
	
	//Can either be a SDL_Window or a SDL_Renderer


	//Can either be a SDL_Texture* or a SDL_Surface* depending on sUseHA
	typedef void texture_t;

	s32 Texture_GetWidth(texture_t* pTexture);
	s32 Texture_GetHeight(texture_t* pTexture);
	//752 bytes
	SDL_Renderer* GetRender();
	//296 bytes
	SDL_Surface* GetSurface();
	//456 bytes
	SDL_Window* GetWindow();

	//There can only be a renderer or a surface, so not both. This means that we use 456 bytes + (752 for HA, 296 for no HA)
	//We atleast need 1KB of memory for the renderer

	[[nodiscard]] bool Init();

	//namespace Draw {

	void Clear();

	void SetColor(SDL_Color pColor);

	void Rect(RRect pPosition);
	void Rect(RRect pPosition, SDL_Color pColor);
	void FillRect(RRect pPosition);

	void FillRect(RRect pPosition, SDL_Color pColor);

	void Texture(void* pTexture, RRect pPosition);
	void Texture(void* pTexture, RRect pPosition, RRect pPart);

	s16 Text(vector pPosition, const char* pFormat, ...);
	s16 GetTextWidth(const char* pFormat, ...);
	s16 GetTextHeight();

	void ResetFormatting();

	void Present();
	//}
}

#endif