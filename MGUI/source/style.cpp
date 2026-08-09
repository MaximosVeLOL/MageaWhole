#include <mgui/style.hpp>
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
	const Render::texture_t* STYLE_TEXTURES[T_LAST] = {
		nullptr
	};
	Render::Color GetColorByID(ColorID pID) {
		return STYLE_COLORS[(u8)pID];
	}
	Render::texture_t* GetTextureByID(TextureID pID) {
		return &STYLE_TEXTURES[pID];
	}
}