#pragma once

#include <mgui/widget.hpp>
#include <mgui/style.hpp>

namespace MGUI {
	class wWindow : public Widget {
	public:
		bool dUseOutline : 2;
		bool dIsDraggable : 2;

		bool dActivated : 2;
		bool _unused : 2;
		VectorTemplate<u8> dMouseOffset = { 0, 0 };

		void PreCache() override {
			dActivated = false;
		}

		void OnMouseHovering() override {
			if (Input::gMouseState.state != SDL_BUTTON_LEFT || Input::gMouseState.y > mRect.y + 20) return;
			dActivated = true;
			dMouseOffset = { static_cast<u8>(Input::gMouseState.x - mRect.x), static_cast<u8>(Input::gMouseState.y - mRect.y) };
		}

		void OnMouseEndHover() override {
		}

		void Tick() override {
			if (dActivated) {
				mRect.x = Input::gMouseState.x - dMouseOffset.x;
				mRect.y = Input::gMouseState.y - dMouseOffset.y;
				if (Input::gMouseState.state != SDL_BUTTON_LEFT)
					dActivated = false;
			}
		}

		void OnRender() override {
			if (dUseOutline) {
				Render::Rect(mRect, Style::GetColorByID(Style::C_W_OUTLINE));
			}
			if (mBackground.isTexture) {
				return;
			}
			//Use the renderer because we can be above u8
			Render::FillRect({ static_cast<s16>(mRect.x + 1), static_cast<s16>(mRect.y + 1), static_cast<u16>(mRect.width - 2), static_cast<u16>(mRect.height - 2) }, Style::GetColorByID((Style::ColorID)mBackground.id));
			if (dIsDraggable) {
				Render::Rect({ mRect.x, mRect.y, mRect.width, 20 }, Style::GetColorByID(Style::C_W_OUTLINE));
			}
		}

		wWindow() { m_sDoHoveringColors = false; }
		wWindow(RRect pPosition) : Widget(pPosition) { m_sDoHoveringColors = false; }
		wWindow(RRect pPosition, Style::ColorID pBackground, bool pUseOutline = false, bool pIsDraggable = false) : Widget(pPosition), dUseOutline(pUseOutline), dIsDraggable(pIsDraggable) { mBackground.id = (u8)pBackground; m_sDoHoveringColors = false;}
	};
}