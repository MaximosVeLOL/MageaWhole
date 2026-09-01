#pragma once


#include <mgui/widget.hpp>
#include <core/math.hpp>

#include <functional>

namespace MGUI {

	//A Combo-Box, where you can select multiple options
	class wEnum : public Widget {
	public:
		u8 dIndex = 0;
		//Array of C strings
		char** dValues = nullptr;
		u8 dValueAmount = 0;
		u8 dValueHeight = 20;
		
		bool dActivated = false;


		//void(*OnSelect)(u8 pPrev, u8 pNext) = nullptr;
		std::function<void(u8, u8)> eOnSelect;

		void OnMouseClickEnd() override {
			if (dActivated) {
				mRect.height = dValueHeight;
				u16 checkPos = Input::gMouseState.y - mRect.y;
				u8 gridPos = (Grid<u8, u16>(checkPos, dValueHeight) / dValueHeight) - 1;
				if (gridPos < 255) {
					u8 prev = dIndex;
					dIndex = gridPos;
					eOnSelect(prev, gridPos);
					//if (OnSelect)
					//	OnSelect(prev, gridPos);
				}
			}
			else mRect.height = ((dValueAmount + 1) * dValueHeight);
			dActivated = !dActivated;
			


		}

		void OnRender() override {
			static u8 i;
			static u8 offsetY = dValueHeight;
			static GUIStyle style = {false, Style::C_W_BACKGROUND};
			uDrawRect({ 0, 0, static_cast<u8>(mRect.width), static_cast<u8>(dValueHeight) }, Style::GetColorByID(Style::C_W_OUTLINE));
			uDrawFillRect({ 1, 1, static_cast<u8>(mRect.width - 2), static_cast<u8>(dValueHeight - 2) }, mBackground);
			uDrawText({ 0, static_cast<u8>(dValueHeight / 2) }, dValues[dIndex]);
			if (dActivated) {
				for (i = 0; i < dValueAmount;i++) {
					style.id = (i == dIndex ? Style::C_W_CLICKED : Style::C_W_BACKGROUND);
					uDrawRect({ 0, offsetY, static_cast<u8>(mRect.width), dValueHeight }, Style::GetColorByID(Style::C_W_OUTLINE));
					uDrawFillRect({ 1, static_cast<u8>(offsetY + 1), static_cast<u8>(mRect.width - 2), static_cast<u8>(dValueHeight - 2) }, style);
					offsetY += dValueHeight;
					uDrawText({ 0, static_cast<u8>(offsetY - static_cast<u8>(dValueHeight / 2))}, dValues[i]);
				}
				offsetY = dValueHeight;
			}

		}

		wEnum() : Widget() {}
		wEnum(RRect pPosition) : Widget(pPosition) {}
		wEnum(RRect pPosition, const char** pValues, u8 pValueAmount, u8 pValueHeight) : Widget(pPosition), dValueAmount(pValueAmount), dValueHeight(pValueHeight) {
			u8 strlen;
			dValues = new char* [pValueAmount];
			for (u8 i = 0; i < pValueAmount;i++) {
				strlen = Strlen(pValues[i]);
				dValues[i] = new char[strlen + 1];
				SDL_strlcpy(dValues[i], pValues[i], strlen + 1);
			}
		}
		/*
		wEnum(RRect pPosition, const char** pValues, u8 pValueAmount, u8 pValueHeight, void(*pOnSelect)(u8 pPrev, u8 pNext)) : wEnum(pPosition, pValues, pValueAmount, pValueHeight) {
			OnSelect = pOnSelect;
		}
		*/
		wEnum(RRect pPosition, const char** pValues, u8 pValueAmount, u8 pValueHeight, std::function<void(u8 pPrev, u8 pNext)> pEOnSelect) : wEnum(pPosition, pValues, pValueAmount, pValueHeight) {
			eOnSelect = pEOnSelect;
		}
	};
}