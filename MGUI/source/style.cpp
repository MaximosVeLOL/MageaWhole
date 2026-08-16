#include <mgui/style.hpp>
#include <core/file.hpp>

namespace Style {
	const Render::Color STYLE_COLORS[C_LAST] = {
		//When a widget is disabled
		{ 70, 70, 70 },
		//Widget background
		{ 255, 255, 255 },
		//When a widget is hovered over
		{ 144, 144, 144 },
		//When a mouse clicks a widget
		{ 72, 72, 72 },
		//The outline of a widget
		{ 0, 0, 0 },
		//The text color,
		{0, 0, 0},
		//The background of a screen?
		{ 200, 200, 200 }
	};
	Render::texture* STYLE_TEXTURES[T_LAST] = {
		nullptr
	};

	void LoadTextures() {
		SDL_Surface* s = SDL_LoadPNG(FileSystem::GetStringAsAsset("texture/ui_background.png"));
		if (!s) return;
		Render::texture* t = STYLE_TEXTURES[T_BACKGROUND];
		t->mTexture = SDL_CreateTextureFromSurface(Render::GetRender(), s);
		if (!t->mTexture) {
			SDL_DestroySurface(s);
		}
	}

	Render::Color GetColorByID(ColorID pID) {
		return STYLE_COLORS[(u8)pID];
	}
	Render::texture* GetTextureByID(TextureID pID) {
		Render::texture* t = STYLE_TEXTURES[pID];
		return t;
	}
	void AddCustomTexture(Render::texture* pTexture) {
		u8 i = T_CUSTOM0;
		for (;i < T_LAST;i++) {
			if (!STYLE_TEXTURES[i]) {
				STYLE_TEXTURES[i] = pTexture;
				break;
			}
		}
		if (i == T_LAST) {
			Log("(MGUI::Style::AddCustomTexture) Failed to add a custom texture to the style.");
		}
	}
	void FlushTextureByID(TextureID pID) {
		Render::texture* texture = STYLE_TEXTURES[pID];
		texture->Destroy();
	}
}