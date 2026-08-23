#include <core/comp/input.hpp>

#include <core/common.hpp>

#include <core/file.hpp>

#include <SDL3/SDL_keyboard.h>

#include <vector>

namespace Input {

	InputState* gStates = nullptr;

	

	MouseState gMouseState = MouseState();

	InputBinding* gBindings = nullptr;
	u8 gBindingCount = 0;

	Axis* gAxis = nullptr;
	u8 gAxisCount = 0;

	COMPONENT_DEFINE_INIT {
		size_t fileSize = 0;
		u8* file = (u8*)SDL_LoadFile(FileSystem::GetStringAsAsset("_dev/inputs.txt"), &fileSize);
		if (!file) {
			//delete file;
			Log("Failed to load inputs.txt! Reverting...");
			return false;
		}

		//char* file = (char*)pInputs.Read(pInputs.mInfo.size);
		//if (!file) return false;

		//Temp string for reading
		char temp[32] = "!";
		u8 tempIndex = 0;


		std::vector<InputBinding> tempBindings;
		std::vector<Axis> tempAxis;
		
		u8 mode = 0;
		/* Modes
		* 0 - Detect region (bind {}, axis {})
		* 1 - Detect bind names
		* 2 - Parse bind parameters
		* 3 - Detect axis names
		* 4 - Parse axis parameters
		*/
		//Index in file
		u32 i = 0;
		
		//Can we do (continue)?
		bool doSkip = false;

		//Temporary variable
		u8 tempVar = 0;

		while (i < fileSize) {
			//Skip whitespaces
			if (file[i] == 0x0D || file[i] == 0x0A || file[i] == '\n' || file[i] == ' ') {
				i++;
				continue;
			}
			
			switch (mode) {
			case 0: //Region checks
				//Check for the region start char
				if (file[i] != '{')
					break;
				if (i < 4) {
					Log("Input region at (%d) does not have a name!", i);
					return false;
				}
				//keys{
				//01234
				//Result:
				//keys
				//Before 8/10/26/7:49 PM, this was tempIndex + 1,
				//And because temp only included the null terminators, no errors occured,
				//And it only failed because of the axis region. SMH my head
				temp[tempIndex] = '\0';
				if (SDL_strcmp(temp, "bind") == 0) {
					mode = 1; //Detect keys
				}
				else if (SDL_strcmp(temp, "axis") == 0) {
					mode = 3; //Detect axis
				}
				else {
					Log("Undefined region! (%s)", temp);
					delete[] file;
					return false;
				}
				doSkip = true;
				break;

			case 1: //Detect key names
				if (file[i] == '}') {
					mode = 0;
					doSkip = true;
					break;
				}
				else if (file[i] != '(') break;
				temp[tempIndex] = '\0';
				//up (Up, dpup)
				//Remove whitespaces
				//up(Up,dpup)
				//Show our part
				//up(
				//012
				//tempIndex = 2
				//Add null terminator
				//up\0
				tempBindings.push_back(InputBinding());
				//strcopy
				if (tempIndex > 18) throw("tempIndex for binding name is above 18! Please use a shorter name!");
				for (u8 i = 0; i < tempIndex + 1;i++) {
					tempBindings.back().name[i] = temp[i];
				}
				mode = 2;
				//TODO - check if this is okay
				doSkip = true;
				break;

			case 2: //Parse key parameters
				if (file[i] == ')') {
					temp[tempIndex] = '\0';
					//Check for 2 parameters
					//Which means we are using
					//keyboard and gamepad
					
					//This can be Up,\0
					//Or it can be Up,\0dpup\0
					tempBindings.back().keyboard = SDL_GetScancodeFromName(temp);

					//Check for gamepad parameter
					if (tempVar == 1) {
						//Get strlen of keyboard name
						tempVar = static_cast<u8>(SDL_strlen(temp));
						//Skip to the start of the button name
						tempBindings.back().button = SDL_GetGamepadButtonFromString(temp + tempVar + 1);
					}
					mode = 1;
					doSkip = true;
				}
				else if (file[i] == ',') {
					//up(Up, dpup)
					//Up,dpup
					//0123456
					//tempIndex = 2
					//Skip so it doesn't get overwritten
					temp[1 + tempIndex++] = '\0';
					tempVar++;
				}
				
				break;

				//These are simple copy and pastes from the key section.
				//Any updates from them should be put here!!!

			case 3: //Detect axis name
				//throw("Unimplemented!");
				if (file[i] == '}') {
					mode = 0;
					doSkip = true;
					break; //We shouldn't put other data after axises, but we can, so whatevers!
				}
				else if (file[i] != '(') break;

				//up (Up, dpup)
				//Remove whitespaces
				//up(Up,dpup)
				//Show our part
				//up(
				//012
				//tempIndex = 2
				//Add null terminator
				//up\0
				tempAxis.push_back(Axis());
				//strcopy
				if (tempIndex > 18) throw("tempIndex for binding name is above 18! Please use a shorter name!");
				for (u8 j = 0; j < tempIndex;j++) {
					tempAxis.back().name[j] = temp[j];
				}
				mode = 4;
				//TODO - check if this is okay
				doSkip = true;

				break;

			case 4: {//Parse key parameters
				if (file[i] == ')') {
					temp[tempIndex] = '\0';
					//Check for 2 parameters
					//Which means we are using
					//keyboard and gamepad

					//(right, left)
					//right\0left\0

					//01234 56789 10
					//10 total size!
					//5 length of left side
					//4 length on right side

					//Find first parameter key using left side (null terminated)
					u8 j = 0;

					//If we use address array hacks, we can use the array's address + strlen to get the right side (null terminated)!


					//A little hack to save on stack and variables!
					tempVar = static_cast<u8>(tempBindings.size());
					char* rightSide = (&temp[0] + SDL_strlen(temp) + 1);
					for (; j < tempVar;j++) {
						//1st argument
						if (SDL_strcmp(tempBindings.at(j).name, temp) == 0) {
							tempAxis.back().binding1 = j;
							//Get the array index of the null terminated string
							
							//break;
						}
						//2nd argument
						else if (SDL_strcmp(tempBindings.at(j).name, rightSide) == 0) {
							tempAxis.back().binding2 = j;
							//break;
						}
					}
					if (j == tempVar) {
						Log("Failed to find input binding for axis with the name of (%s)! (binding name: %s)", tempAxis.back().name, temp);
					}
					
					mode = 3; //This was needed even without the hack, great minds think alike.
					doSkip = true;
				}
				else if (file[i] == ',') {
					//up(Up, dpup)
					//Up,dpup
					//0123456
					//tempIndex = 2
					//Skip so it doesn't get overwritten
					temp[tempIndex++] = '\0';
					goto _continueHack;
					//doSkip = true;
				}

			} break;

			} //End of switch case
			if (doSkip) {
				i++;
				doSkip = false;
				tempIndex = 0;
				continue;
			_continueHack:
				i++;
				continue;
			}

			if (tempIndex >= 32)
				throw("tempIndex is above 32!");
			temp[tempIndex++] = file[i];
			i++;
		}

		u32 bSize = static_cast<u32>(tempBindings.size());
		if (bSize > 0) {
			gBindings = new InputBinding[bSize];
			gStates = new InputState[bSize];
			for (u32 i = 0; i < bSize;i++) {
				InputBinding& b = tempBindings.at(i);
				for (u8 j = 0; j < 18 - 1;j++) {
					gBindings[i].name[j] = b.name[j];
				}
				gBindings[i].keyboard = b.keyboard;
				gBindings[i].button = b.button;
			}
			gBindingCount = bSize;
		}
		u32 aSize = static_cast<u32>(tempAxis.size());
		if (aSize > 0) {
			gAxis = new Axis[aSize];
			for (u32 i = 0; i < aSize;i++) {
				Axis& a = tempAxis.at(i);
				for (u8 j = 0; j < 18;j++) {
					gAxis[i].name[j] = a.name[j];
				}
				gAxis[i].binding1 = a.binding1;
				gAxis[i].binding2 = a.binding2;
			}
			gAxisCount = aSize;
		}
		return true;
	}
	
	void UpdateAllInput() {
		const bool* keyboard = SDL_GetKeyboardState(NULL);
		for (u8 i = 0; i < gBindingCount;i++) {
			gStates[i].previous = gStates[i].current;
			gStates[i].current = keyboard[gBindings[i].keyboard];
			//if (keyboard[i]) {
			//	SDL_Log("%s is true!", SDL_GetScancodeName((SDL_Scancode)i));
			//}
		}
	}

	//static_assert(typeid(inputname_t) == typeid(const char*), "Test");

	

	InputState* findStateForBinding(inputname_t pName) {
		for (u8 i = 0; i < gBindingCount;i++) {
			if (SDL_strcmp(gBindings[i].name, pName) == 0)
				return &gStates[i];
		}
		return nullptr;
	}

	bool GetBindingIsUp(inputname_t pCode) {
		InputState* b = findStateForBinding(pCode);
		if (!b) {
			Log("Input binding is undefined! (%s)", pCode);
			return false;
		}
		return b->current & 0;
	}
	bool GetBindingWentDown(inputname_t pCode) {
		InputState* b = findStateForBinding(pCode);
		if (!b) {
			Log("Input binding is undefined! (%s)", pCode);
			return false;
		}
		return b->current && !b->previous;
	}
	bool GetBindingIsDown(inputname_t pCode) {
		InputState* b = findStateForBinding(pCode);
		if (!b) {
			Log("Input binding is undefined! (%s)", pCode);
			return false;
		}
		return b->current & 1;
	}
	bool GetBindingWentUp(inputname_t pCode) {
		InputState* b = findStateForBinding(pCode);
		if (!b) {
			Log("Input binding is undefined! (%s)", pCode);
			return false;
		}
		return !b->current && b->previous;
	}

	SDL_Keycode gCurrentKey = 0;
	bool gIsShift = false;

	bool CheckForKeyboardKeyDown(SDL_Keycode pKeycode) {
		return (gCurrentKey == pKeycode);
	}

	char GetAxisState(inputname_t pCode) {
		for (u8 i = 0; i < gAxisCount;i++) {
			Axis& a = gAxis[i];
			if (SDL_strcmp(a.name, pCode) == 0) {
				
				return static_cast<char>(gStates[a.binding1].current - gStates[a.binding2].current);
			}
		}
		Log("(Input::GetAxisState) Failed to find input name for (%s)!", pCode);
		return 0;
	}

	COMPONENT_DEFINE_UNLOAD{
		delete[] gBindings;
		delete[] gStates;
		gBindingCount = 0;
		delete[] gAxis;
		gAxisCount = 0;
	}

}