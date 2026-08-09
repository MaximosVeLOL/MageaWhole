#include <mgui/handler.hpp>
#include <mgui/widget.hpp>
#include <mgui/style.hpp>
#include <core/input.hpp>

namespace MGUI {
	
	constexpr u8 MAX_WIDGETS = 50;

	void Screen::AddWidget(Widget* pWidget, const char* pName) {
		if (mWidgetCount + 1 >= MAX_WIDGETS) return;
		pWidget->mParent = this;
		pWidget->mID = mWidgetCount;
		if (pName) {
			string_size_t l = SDL_strlen(pName) + 1;
			mWidgetNames[mWidgetCount] = new char[l];
			SDL_strlcpy(mWidgetNames[mWidgetCount], pName, l);
		}
		else {
			mWidgetNames[mWidgetCount] = format("Unset(%d)", mWidgetCount);
		}
		mWidgets[mWidgetCount++] = pWidget;
		
	}

	Widget* Screen::FindWidget(const char* pName) {
		for (u8 i = 0; i < mWidgetCount;i++) {
			if (SDL_strcmp(mWidgetNames[i], pName) == 0) {
				return mWidgets[i];
			}
		}
		return nullptr;
	}

	void Screen::UpdateAndRender() {
		Render::Clear();
		static u8 prevMouseState = 0;
		mMouseOverWidget = false;
		for (u8 i = 0; i < mWidgetCount;i++) {
			mWidgets[i]->Tick();
			if (Input::gMouseState.x >= mWidgets[i]->mRect.x && Input::gMouseState.x <= mWidgets[i]->mRect.x + mWidgets[i]->mRect.width
				&& Input::gMouseState.y >= mWidgets[i]->mRect.y && Input::gMouseState.y <= mWidgets[i]->mRect.y + mWidgets[i]->mRect.height) {
				mHoverStates[i].cur = true;
				mMouseOverWidget = true;
				HoverState& h = mHoverStates[i];
				if (h.cur) {
					if (h.prev) {
						mWidgets[i]->OnMouseHovering();
						if(mWidgets[i]->m_sDoHoveringColors) mWidgets[i]->mBackground.id = Style::C_W_HIGHLIGHTED;
					}
					else mWidgets[i]->OnMouseHover();
				}
				else {
					mWidgets[i]->OnMouseEndHover();
				}
				if (Input::gMouseState.state == SDL_BUTTON_LEFT) {
					if (mWidgets[i]->m_sDoHoveringColors) mWidgets[i]->mBackground.id = Style::C_W_CLICKED;
					switch (prevMouseState) {
					case 0:
						mWidgets[i]->OnMouseClickStart();
						break;

					case SDL_BUTTON_LEFT:
						mWidgets[i]->OnMouseClickHeld();
						break;

					}
				}
				else if(prevMouseState == SDL_BUTTON_LEFT)
					mWidgets[i]->OnMouseClickEnd();
			}
			else {
				if (Input::gMouseState.state == SDL_BUTTON_LEFT) mWidgets[i]->OnMouseDeselect();
				if (mWidgets[i]->m_sDoHoveringColors) mWidgets[i]->mBackground.id = Style::C_W_BACKGROUND;
			}
			mHoverStates[i].prev = mHoverStates[i].cur;

			mWidgets[i]->Render();
		}
		prevMouseState = Input::gMouseState.state;
	}

	Screen::Screen() {
		mWidgets = new Widget * [MAX_WIDGETS];
		mHoverStates = new HoverState[MAX_WIDGETS];
		mWidgetNames = new char* [MAX_WIDGETS];
	}
}