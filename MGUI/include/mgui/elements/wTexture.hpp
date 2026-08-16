#pragma once

#include <mgui/widget.hpp>

namespace MGUI {
	//A widget that displays a texture over its mRect instead of a GUIStyle thing.
	class wTexture : public Widget {
	public:
		//char* dTextureDirectory = nullptr;
		Render::texture* dTexture = nullptr;
		/*
		void PreCache() {
			if (dTextureDirectory) {
				
			}
		}
		*/
		void OnRender() override {
			Render::Texture(dTexture, mRect);
		}

		wTexture() : Widget() {}
		wTexture(RRect pPosition, Render::texture* pTexture = nullptr) : Widget(pPosition), dTexture(pTexture) {}
		/*
		wTexture(RRect pPosition, const char* pDirectory) : Widget(pPosition) {
			string_size_t s = (SDL_strlen(pDirectory) + 1);
			dTextureDirectory = new char[s];
			SDL_strlcpy(dTextureDirectory, pDirectory, s);
		}
		*/
	};
}