#ifndef __STYLE_H__
#define __STYLE_H__

#include <mgui/common.hpp>

namespace Style {

	enum ColorID : u8 {
		C_W_DISABLED = 0,
		C_W_BACKGROUND,
		C_W_HIGHLIGHTED,
		C_W_CLICKED,
		C_W_OUTLINE,
		C_W_TEXT,
		C_BACKGROUND,
		C_LAST,
	};
	enum TextureID : u8 {
		T_BACKGROUND = 0,
		T_CUSTOM0,
		T_CUSTOM1,
		T_CUSTOM2,
		T_CUSTOM3,
		T_LAST,
	};
	MGUI_API void LoadTextures();
	MGUI_API Render::Color GetColorByID(ColorID pID);
	MGUI_API Render::texture* GetTextureByID(TextureID pID);
	MGUI_API void AddCustomTexture(Render::texture* pTexture);
	MGUI_API void FlushTextureByID(TextureID pID);
}
#endif