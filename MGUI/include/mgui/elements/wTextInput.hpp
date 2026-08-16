#pragma once


#include <mgui/widget.hpp>
#include <core/comp/input.hpp>
#include <mgui/style.hpp>

namespace MGUI {
	
	constexpr u8 CURSOR_MAX = 60;
	constexpr u8 CURSOR_TICK = CURSOR_MAX / 2;

	class wTextInput : public Widget {
	public:

		enum Format : u8 {
			//Letters, numbers, ect
			F_ALL = 0,
			F_NUMBERS = 1,
		};

		u8 dTextEnteredIndex = 0;
		char dTextEntered[33] = { '\0' };
		u8 dTextEnteredMax = 32;
		
		bool dActivated = false;
		u8 dTextCursorTimer = 0;

		Format dFormat = F_ALL;

		void OnMouseClickEnd() override {
			dActivated = !dActivated;
		}

		void Tick() override {
			if (!dActivated) return;
			static SDL_Keycode prevKey = 0;
			if (++dTextCursorTimer >= CURSOR_MAX) {
				dTextCursorTimer = 0;
			}
			if (Input::gCurrentKey != prevKey) {
				if (dFormat == F_NUMBERS) {
					if (Input::gCurrentKey == SDLK_BACKSPACE && dTextEnteredIndex > 0) {
						dTextEntered[--dTextEnteredIndex] = '\0';
					}
					else if(Input::gCurrentKey >= SDLK_0 && Input::gCurrentKey <= SDLK_9) {

						char key = SDL_GetKeyName(Input::gCurrentKey)[0];
						if (dTextEnteredIndex + 1 >= dTextEnteredMax) {
							dTextEnteredIndex = dTextEnteredMax - 1;
						}
						dTextEntered[dTextEnteredIndex++] = key;
					}
					
				}
				else {
					switch (Input::gCurrentKey) {
					case SDLK_BACKSPACE:
						if (dTextEnteredIndex == 0) break;
						dTextEntered[--dTextEnteredIndex] = '\0';
						break;

					case SDLK_SPACE:
						dTextEntered[dTextEnteredIndex++] = ' ';
						break;
					default:
						if (/*Input::gCurrentKey != 0 && */Input::gCurrentKey >= SDLK_SLASH && Input::gCurrentKey <= SDLK_LEFTBRACE) {
							if (dTextEnteredIndex + 1 >= dTextEnteredMax) {
								dTextEnteredIndex = dTextEnteredMax - 1;
							}
							char keyTodo = SDL_GetKeyName(Input::gCurrentKey)[0];
							char realKey = (SDL_isdigit(keyTodo) ? keyTodo : (Input::gIsShift ? keyTodo : SDL_tolower(keyTodo)));

							dTextEntered[dTextEnteredIndex++] = realKey;
						}
						break;

						break;
					}
				}

			}
			prevKey = Input::gCurrentKey;
		}

		void OnRender() override {
			uDrawMRect();
			Render::gAlignMode.vertical = A_BOTTOM;
			s16 width = uDrawText({ 0, 4 }, dTextEntered);
			Render::gAlignMode.vertical = A_MIDDLE;
			if (dActivated) {
				
				if (dTextCursorTimer < CURSOR_TICK) {
					uDrawFillRect({ static_cast<s16>(width - mRect.x), 0, 2, (u8)mRect.height }, { false, Style::C_W_OUTLINE });
				}
				
			}

		}

		void OnMouseDeselect() override {
			dActivated = false;
		}

		int uGetAsInteger() {
			//if (!SDL_isdigit(dTextEntered)) return -1;
			return SDL_atoi(dTextEntered);
		}
		char* uGetAsString() {
			return dTextEntered;
		}

		wTextInput() : Widget() {}
		//wTextInput(RRect pPosition) : Widget(pPosition) {}
		wTextInput(RRect pPosition, u8 pTextEnteredMax = 32, Format pFormat = F_ALL) : Widget(pPosition), dTextEnteredMax(pTextEnteredMax), dFormat(pFormat) {}
	};
}