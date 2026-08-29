#pragma once


#include <mgui/widget.hpp>

namespace MGUI {
	//wNumber - A selectable range of numbers that can probably be 
	//selected using a text input, and 2 buttons for incrementing and decreasing the value.
	template<typename T>
	class wNumber : public Widget {
	public:
		T dValue = T();
		T dValueIncreaseValue = 1;
		T dValueMin = T();
		T dValueMax = T();
		char* dValueString = nullptr;

		static const u8 BUTTON_WIDTH = 25;

		void PreCache() override {
			dValueString = format("%s", "0");
		}

		void(*onIncrementCallback)(T dOldValue);

		void OnIncrement() {
			if (onIncrementCallback)
				onIncrementCallback(dValue);
			delete[] dValueString;
			dValueString = format("%d", (int)dValue);
		}

		void Tick() override {

		}

		void OnMouseClickEnd() override {
			if (Input::gMouseState.x <= mRect.x + BUTTON_WIDTH) {
				int temp = (dValue - dValueIncreaseValue);
				temp = MIN(temp, dValueMin);
				dValue = static_cast<T>(temp);
				OnIncrement();

			}
			//Text input (TODO)
			else if(Input::gMouseState.x <= mRect.x + (mRect.width - BUTTON_WIDTH)) {

			}
			else {
				int temp = (dValue + dValueIncreaseValue);
				temp = MAX(temp, dValueMax);
				dValue = static_cast<T>(temp);
				OnIncrement();

			}
		}

		void OnRender() override {
			uDrawFillRect({ 0, 0, BUTTON_WIDTH, mRect.height }, mBackground, true);
			uDrawFillRect({ BUTTON_WIDTH, 0, static_cast<u8>(mRect.width - BUTTON_WIDTH), mRect.height }, mBackground, true);
			uDrawText({ static_cast<u8>(BUTTON_WIDTH), 0 }, dValueString);
			uDrawFillRect({ static_cast<u8>(mRect.width - BUTTON_WIDTH), 0, BUTTON_WIDTH, mRect.height }, mBackground, true);
		}

		wNumber() : Widget() {}
		wNumber(RRect pPosition, T pValueMin = T(), T pValueMax = T(), void(*pOnIncrement)(T pValue) = 0) : Widget(pPosition), dValueMin(pValueMin), dValueMax(pValueMax) { onIncrementCallback = pOnIncrement; };
	};
}