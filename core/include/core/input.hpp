#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL3/SDL.h> //For SDL_Scancode
#include <core/common.hpp> //For common typedefs
#include <core/component.hpp>
COMPONENT_DEFINE_START(useInput, Input)
	struct InputState {
		bool current : 4;
		bool previous : 4;
	};
	static_assert(sizeof(InputState) == sizeof(unsigned char), "sizeof InputState is not 1 byte!");

//#pragma pack(4)
	struct MouseState {
		u16 x : 12;
		u16 y : 12;
		u8 state : 8;
		//SDL_MouseButtonFlags state = SDL_BUTTON_MASK;
	};
	//An input that uses a keyboard key and a 
	struct InputBinding {
		char name[18] = "!";
		u8 keyboard = SDL_SCANCODE_UNKNOWN;
		s8 button = SDL_GAMEPAD_BUTTON_INVALID;
		//u8 : 2;
	};

	struct Axis {
		char name[17] = "!";
		u8 binding1 = 0;
		u8 binding2 = 0;
		char state = 0;
	};

	typedef const char* inputname_t;

	extern MouseState gMouseState;

	extern InputState* gStates;

	extern SDL_Keycode gCurrentKey;

	extern bool gIsShift;

	COMPONENT_INCLUDE_INIT;
	COMPONENT_INCLUDE_UNLOAD;

	void UpdateAllInput();

	extern bool GetBindingIsUp(inputname_t pCode);
	extern bool GetBindingWentDown(inputname_t pCode);
	extern bool GetBindingIsDown(inputname_t pCode);
	extern bool GetBindingWentUp(inputname_t pCode);

	extern bool CheckForKeyboardKeyDown(SDL_Keycode pKeycode);

	
COMPONENT_DEFINE_END


#endif