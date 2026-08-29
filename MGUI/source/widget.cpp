#include <mgui/widget.hpp>
#include <core/comp/renderer.hpp>
#include <mgui/handler.hpp>
#include <mgui/style.hpp>
#include <core/comp/input.hpp>

namespace MGUI {

	bool Widget::uGetMouseHoverBefore() {
		return mParent->mHoverStates[mID].prev;
	}
	bool Widget::uGetMouseHoverCurrent() {
		return mParent->mHoverStates[mID].cur;
	}

	void Widget::uDrawMRect() {
		Render::Rect(mRect, Style::GetColorByID(Style::C_W_OUTLINE));
		if (mBackground.isTexture) {
			Render::Texture(Style::GetTextureByID((Style::TextureID)mBackground.id), mRect);

			return;
		}
		Render::FillRect({static_cast<s16>(mRect.x + 1), static_cast<s16>(mRect.y + 1), static_cast<u16>(mRect.width - 2), static_cast<u16>(mRect.height - 2)}, Style::GetColorByID((Style::ColorID)mBackground.id));
	}

	s16 Widget::uDrawText(vector pOffset, const char* pFormat, ...) {
		Render::SetColor(Style::GetColorByID(Style::C_W_TEXT));
		return Render::Text({ static_cast<s16>(mRect.x + pOffset.x), static_cast<s16>(mRect.y + pOffset.y) }, pFormat);
	}


	void Widget::uDrawRect(RRect pOffset, Render::Color pColor) {
		Render::Rect({ static_cast<s16>(mRect.x + pOffset.x), static_cast<s16>(mRect.y + pOffset.y), pOffset.width, pOffset.height }, pColor);
	}
	void Widget::uDrawFillRect(RRect pOffset, GUIStyle pStyle, bool pOutline) {
		
		RRect r = { static_cast<s16>(mRect.x + pOffset.x + 1), static_cast<s16>(mRect.y + pOffset.y + 1), pOffset.width - 2, pOffset.height - 2 };
		if (pOutline) {
			//RRect rOg = { static_cast<s16>(mRect.x + pOffset.x), static_cast<s16>(mRect.y + pOffset.y), pOffset.width, pOffset.height };
			Render::Rect(r, Style::GetColorByID(Style::C_W_OUTLINE));
			r.x++;
			r.y++;
			r.width -= 2;
			r.height -= 2;
		}
		if (pOutline) {

		}
		if (pStyle.isTexture) {
			Render::Texture(Style::GetTextureByID((Style::TextureID)mBackground.id), r);
			return;
		}
		Render::FillRect(r, Style::GetColorByID((Style::ColorID)pStyle.id));
	}

	Widget* Widget::uFindWidget(const char* pName) {
		return nullptr;
	}
}