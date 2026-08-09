#pragma once

#include <MGUI/widget.hpp>
#include <core/input.hpp>

namespace MGUI {
	//You can scroll. Wow.
	class wScrollbar : public Widget {
	public:
		//Floats!
		float dScrollValue = 0;
		//Should this be a constant?
		float dScrollValuePrevious = 0;
		float dScrollValueMax = -1;

		//On the screen
		s16 dScrollRenderValue = 0;

		u8 dScrollWidth = 0;

		void(*OnValueChanged)(float pPrev, float pNext) = nullptr;

		void PreCache() override {
			float sW = (mRect.width / 3);
			dScrollWidth = SDL_lroundf(sW);
			if (dScrollValueMax == -1) dScrollValueMax = (mRect.width / 100);
		}

		void OnRender() override {
			uDrawMRect();
			//V/M
			uDrawFillRect({ (u8)dScrollRenderValue, 0, dScrollWidth, (u8)mRect.height }, {false, Style::C_W_OUTLINE});
		}




		void OnMouseClickHeld() override {
			//Percentage from x/w
			dScrollRenderValue = static_cast<s16>((Input::gMouseState.x - mRect.x));
			dScrollValue = (dScrollRenderValue * dScrollValueMax);
			if (dScrollValue != dScrollValuePrevious && OnValueChanged) {
				OnValueChanged(dScrollValuePrevious, dScrollValue);
			}

			dScrollValuePrevious = dScrollValue;
			//Now we need to convert this base to the other
			//W = 10
			//M-X = 5
			//B = 5
			
			//B/W = 50%
			//We got the percentage!
			//Max * 50% = realValue?
		}

		wScrollbar() : Widget() {}
		wScrollbar(RRect pPosition, float pScrollValueMax = -1, void(*pOnValueChange)(float pPrev, float pNext) = nullptr) : Widget(pPosition), dScrollValueMax(pScrollValueMax) {
			OnValueChanged = pOnValueChange;
		}
	};
}