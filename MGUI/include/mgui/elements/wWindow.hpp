#pragma once

#include <mgui/widget.hpp>
#include <mgui/style.hpp>

namespace MGUI {
	class wWindow : public Widget {
	public:
		bool dUseOutline = false;
		void OnRender() override {
			if (dUseOutline) {
				Render::Rect(mRect, Style::GetColorByID(Style::C_W_OUTLINE));
			}
			if (mBackground.isTexture) {
				return;
			}
			//Use the renderer because we can be above u8
			Render::FillRect({ static_cast<s16>(mRect.x + 1), static_cast<s16>(mRect.y + 1), static_cast<u16>(mRect.width - 2), static_cast<u16>(mRect.height - 2) }, Style::GetColorByID((Style::ColorID)mBackground.id));
		}

		wWindow() { m_sDoHoveringColors = false; }
		wWindow(RRect pPosition) : Widget(pPosition) { m_sDoHoveringColors = false; }
		wWindow(RRect pPosition, Style::ColorID pBackground, bool pUseOutline = false) : Widget(pPosition), dUseOutline(pUseOutline) { mBackground.id = (u8)pBackground; m_sDoHoveringColors = false;}

	};
}