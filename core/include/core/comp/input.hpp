#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL3/SDL.h> //For SDL_Scancode
#include <core/common.hpp> //For common typedefs
#include <core/comp/component.hpp>


//--------------
//Component: Input!
//
// This is an interesting system, after many issues with other implementations,
// The one that is used currently addresses and fixes all the issues needed,
// No inputs are hard-coded! They use SDL_Keycode/SDL_Scancode names, and everything
// Is implemented from a user made file. This allows for multiple keys to be made,
// And other placeholder/development keys to be removed.
// I will probably add a faster format 
//--------------





COMPONENT_DEFINE_START(useInput, Input, true)
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
		//MouseState() {}
		MouseState() : x(0), y(0), state(0) {}
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
		char name[18] = "!";
		u8 binding1 = 255;
		u8 binding2 = 255;
		//char state = 0;
	};

	typedef const char* inputname_t;

	//extern "C" __declspec(dllexport) MouseState gMouseState;

	CORE_EXPORT MouseState gMouseState;

	CORE_EXPORT InputState* gStates;

	CORE_EXPORT SDL_Keycode gCurrentKey;

	CORE_EXPORT bool gIsShift;

	CORE_EXPORT COMPONENT_INCLUDE_INIT;
	CORE_EXPORT COMPONENT_INCLUDE_UNLOAD;

	CORE_EXPORT void UpdateAllInput();

	CORE_EXPORT bool GetBindingIsUp(inputname_t pCode);
	CORE_EXPORT bool GetBindingWentDown(inputname_t pCode);
	CORE_EXPORT bool GetBindingIsDown(inputname_t pCode);
	CORE_EXPORT bool GetBindingWentUp(inputname_t pCode);

	CORE_EXPORT bool CheckForKeyboardKeyDown(SDL_Keycode pKeycode);

	CORE_EXPORT char GetAxisState(inputname_t pCode);

	
COMPONENT_DEFINE_END


#endif