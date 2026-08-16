#include <core/comp/renderer.hpp>
#include <core/file.hpp> //For the filesystem, used by the font



namespace Render {

	constexpr u16 SCREEN_WIDTH = 960;
	constexpr u16 SCREEN_HEIGHT = 540;

	//Atleast 8 bytes
	//752 bytes
	SDL_Renderer* gRenderer = nullptr;

	//456
	//Also holds the surface, which is 296 bytes
	SDL_Window* gWindow = nullptr;

	//1 byte
	AlignMode gAlignMode = { A_LEFT, A_MIDDLE };

	//336 bytes (for HA)
	texture gFont = {nullptr};

	//The actual size of the font in the image file
	constexpr u8 gFontSize = 16;
	constexpr u8 gFontOutputSize = 10;

	RRect getTexturePartFromAscii(char l) {
		return { static_cast<s16>((l - 33) * gFontSize), 0, gFontSize, gFontSize };
	}

	SDL_Renderer* GetRender() { return gRenderer; }
	SDL_Surface* GetSurface() { return SDL_GetWindowSurface(gWindow); }

	SDL_Window* GetWindow() { return gWindow; }

	//SDL_Renderer* GetRender() { return static_cast<SDL_Renderer*>(gRenderThing); }
	//SDL_Surface* GetSurface() { return SDL_GetWindowSurface(static_cast<SDL_Window*>(gRenderThing)); }
	//SDL_Surface * GetSurface() { return static_cast<SDL_Surface*>(gRenderThing); }
	//SDL_FRect RectToFloat(SDL_Rect pIn) { return { static_cast<float>(pIn.x), static_cast<float>(pIn.y), static_cast<float>(pIn.w), static_cast<float>(pIn.h) }; }

	SDL_Rect RectToSDLRect(RRect pRect) {
		return {static_cast<int>(pRect.x), static_cast<int>(pRect.y), static_cast<int>(pRect.width), static_cast<int>(pRect.height) };
	}
	SDL_FRect RectToSDLFloatRect(RRect pRect) {
		return {static_cast<float>(pRect.x), static_cast<float>(pRect.y), static_cast<float>(pRect.width), static_cast<float>(pRect.height) };
	}


	COMPONENT_DEFINE_INIT {
		if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) return false;
		gWindow = SDL_CreateWindow("Application", 960, 540, SDL_WINDOW_RESIZABLE);
		if (!gWindow)
			return false;
		
		if (sUseHA) {
			gRenderer = SDL_CreateRenderer(gWindow, NULL);
			if (!gRenderer)
				return false;
		}
		//gRenderThing = (sUseHA ? (void*)SDL_CreateRenderer(w, NULL) : w); //If anything other than NULL is used, it crashes
		
		SDL_Surface* s = SDL_LoadPNG(FileSystem::GetStringAsAsset("texture/font_debug.png"));
		if (s) {
			if (sUseHA) {
				gFont.mTexture = SDL_CreateTextureFromSurface(GetRender(), s);
				SDL_DestroySurface(s);
				return true;
			}
			gFont.mTexture = s;
		}
		return true;
	}
	void Clear() {
		if (sUseHA) {
			SDL_RenderClear(GetRender());
			return;
		}
		SDL_ClearSurface(GetSurface(), 1, 1, 1, 1);
	}

	void SetColor(SDL_Color pColor) {
		if (sUseHA) {
			SDL_SetRenderDrawColor(GetRender(), pColor.r, pColor.g, pColor.b, 0xFF);
		}
	}

	//Renders a rect with the current draw color
	void Rect(RRect pPosition) {
		if (sUseHA) {
			SDL_FRect pos = RectToSDLFloatRect(pPosition);
			SDL_RenderRect(GetRender(), &pos);
			return;
		}
		SDL_Surface* s = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_XRGB8888);
		SDL_FillSurfaceRect(s, NULL, 0xFFFFFFFF);
		SDL_Rect pos = RectToSDLRect(pPosition);
		SDL_BlitSurface(s, NULL, GetSurface(), &pos);

	}
	//Renders a rect with the selected color
	void Rect(RRect pPosition, SDL_Color pColor) {
		SetColor(pColor);
		Rect(pPosition);
	}

	//Renders a filled rect with the current draw color
	void FillRect(RRect pPosition) {
		if (sUseHA) {
			SDL_FRect pos = RectToSDLFloatRect(pPosition);
			SDL_RenderFillRect(GetRender(), &pos);
		}
	}
	//Renders a filled rect with the selected color
	void FillRect(RRect pPosition, SDL_Color pColor) {
		SetColor(pColor);
		FillRect(pPosition);
	}

	void Texture(texture* pTexture, RRect pPosition) {
		if (sUseHA) {
			SDL_FRect pos = RectToSDLFloatRect(pPosition);
			SDL_RenderTexture(GetRender(), pTexture->AsTexture(), NULL, &pos);
			return;
		}
		SDL_Surface* s = pTexture->AsSurface();
		SDL_Rect pos = RectToSDLRect(pPosition);
		SDL_BlitSurface(s, NULL, GetSurface(), &pos);
		//TODO - Implement
	}
	void Texture(texture* pTexture, RRect pPosition, RRect pPart) {
		SDL_FRect out = RectToSDLFloatRect(pPosition);
		SDL_FRect src = RectToSDLFloatRect(pPart);
		SDL_RenderTexture(GetRender(), pTexture->AsTexture(), &src, &out);
	}

	s16 Text(vector pPosition, const char* pFormat, ...) {
		char* text = format(pFormat);
		switch (gAlignMode.vertical) {
		case A_TOP:
			pPosition.y -= gFontOutputSize;
			break;
		case A_MIDDLE:
			pPosition.y -= (gFontOutputSize / 2);
			break;

		case A_BOTTOM:
			//pPosition.y += gFontOutputSize;

			break;
		}
		//FillRect({ static_cast<u16>(pPosition.x), static_cast<u16>(pPosition.y), static_cast<u8>(SDL_strlen(pFormat) * gFontOutputSize), gFontOutputSize}, { 255, 0, 255 });

		//if (sUseHA) {
		SetColor({ 0, 0, 0, 255 });
		//Get the result of the format
		RRect result = { pPosition.x, pPosition.y, gFontOutputSize, gFontOutputSize };
		for (u32 i = 0; i < SDL_strlen(text);i++) {
			if (text[i] <= 32) {
				if (text[i] == ' ') {
					result.x += gFontOutputSize;
					continue;
				}
				else if (text[i] == '\n') {
					result.y += gFontOutputSize;
				}
				continue;
			}
			else if (SDL_islower(text[i]) || SDL_isdigit(text[i])) {
				result.x -= 2;
				//result.width = gFontOutputSize / 2;
			}
			Texture(&gFont, result, getTexturePartFromAscii(text[i]));

			result.x += gFontOutputSize;
		}

		//SDL_RenderDebugText(GetRender(), static_cast<float>(pPosition.x), static_cast<float>(pPosition.y), text);
		delete[] text;
		return result.x;
		//SDL_RenderDebugTextFormat(GetRender(), static_cast<float>(pPosition.x), static_cast<float>(pPosition.y), pFormat);
		//return;
	//}
	//delete[] text;
	}
	s16 GetTextWidth(const char* pFormat, ...) {
		if (!pFormat) return -1;
		char* text = format(pFormat);
		if (!text) return -1;
		s16 ret = 0;
		for (u32 i = 0; i < SDL_strlen(text);i++) {
			if (SDL_islower(text[i]) || SDL_isdigit(text[i])) {
				ret -= 2;
				//result.width = gFontOutputSize / 2;
			}

			ret += gFontOutputSize;
		}
		return ret;
	}

	void ResetFormatting() {
		gAlignMode = { A_LEFT, A_MIDDLE };
	}

	void Present() {
		if (sUseHA) {
			SDL_RenderPresent(GetRender());
			return;
		}
		SDL_UpdateWindowSurface(gWindow);
	}

	COMPONENT_DEFINE_UNLOAD {
		if (sUseHA) {
			SDL_FlushRenderer(gRenderer);

			SDL_DestroyRenderer(gRenderer);
		}
		else {
		}
		SDL_DestroyWindow(gWindow);
	}
}