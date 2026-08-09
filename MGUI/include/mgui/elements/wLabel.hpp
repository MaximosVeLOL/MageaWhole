#pragma once

#include <mgui/widget.hpp>

namespace MGUI {
	class wLabel : public Widget {
	public:
		char* dText = nullptr;

		void OnRender() override {
			uDrawText({ 0, 0 }, dText);
		}

		void SetText(char* pText) {
			delete[] dText;
			dText = pText;
		}

		wLabel() {}
		wLabel(RRect pPosition, const char* pText) : Widget(pPosition) { size_t sL = SDL_strlen(pText); dText = new char[sL + 1];SDL_strlcpy(dText, pText, sL + 1); }
	};
}