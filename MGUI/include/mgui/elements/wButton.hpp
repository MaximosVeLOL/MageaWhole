//Use this for derived classes
#pragma once

#include <mgui/widget.hpp>
namespace MGUI {
	class wButton : public Widget {
	public:
		char* dText = nullptr;
		void(*OnClicked)() = nullptr;

		void OnRender() override {
			uDrawMRect();
			uDrawText({ 0, static_cast<u8>(mRect.height / 2) }, dText);
		}

		void OnMouseClickEnd() override {
			if (!OnClicked) return;
			OnClicked();
		}

		wButton(){}
		wButton(RRect pPosition, void(*pOnClicked)(), const char* pText) : Widget(pPosition) { OnClicked = pOnClicked;size_t sL = SDL_strlen(pText); dText = new char[sL + 1];SDL_strlcpy(dText, pText, sL + 1); }
		
	};
}