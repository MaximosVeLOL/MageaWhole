#ifndef NDEBUG
#pragma once

#include <magea/dev/tool.hpp>

#include <api.hpp>

#include <SDL3/SDL.h>

#include <core/file.hpp>

#include <magea/world.hpp>

#include <vector>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
using namespace MGUI;

#include <magea/dev/developer.hpp>

//Dev Tool: Editor
// This tool is meant for development of "worldspaces" that includes
// Objects, collision, tilesets, and backgrounds
// I think I will implement this by using a static world instance,
// So that way I wont have to implement a copy of an instance only for
// A developer tool.

namespace Developer {
	namespace Editor {
		constexpr u8 OBJECT_NAME_MAX = 32;
	}

	struct Variable {
		char name[16] = "!";
		//Uses the index of VAR_DEFS as the type
		u8 type = 0;
		void* value = nullptr;
		template<typename T>
		T* GetAs() {
			return static_cast<T*>(value);
		}
	};

	struct EditorObject {
		//The index of the object's name
		u8 ID = 0;
		Variable* variables = nullptr;
	};
	//Stuff for file parsing
	namespace File {

		struct ObjectDefinition {
			char name[Editor::OBJECT_NAME_MAX + 1] = "!";
			std::vector<Variable> variables;
		};

		class VariableDefinition {
		public:
			const char* name = nullptr;
			virtual void SetDefault(void** pOutput) {}
			virtual void SetValue(void** pOutput, const char* pValue) {}
		
			VariableDefinition() {}

			VariableDefinition(const char* pName) : name(pName) {}
		};

		//Basic integer types go here
		template<typename T>
		class v_BasicTypeTemplate : public VariableDefinition {
		public:
			void SetDefault(void** pOutput) override {
				*pOutput = new T();
			}
			void SetValue(void** pOutput, const char* pValue) override {
				int value = SDL_atoi(pValue);
				*static_cast<T*>(*pOutput) = static_cast<T>(value);
			}

			v_BasicTypeTemplate(const char* pName) : VariableDefinition(pName) {}
		};

		class v_Bool : public VariableDefinition {
		public:
			void SetDefault(void** pOutput) override {
				*pOutput = new bool(false);
			}
			void SetValue(void** pOutput, const char* pValue) override {
				//Set "false", and "0" to false, otherwise, it is true
				if (SDL_strcmp(pValue, "false") == 0 || SDL_strcmp(pValue, "0") == 0) {
					*static_cast<bool*>(*pOutput) = false;
				}
			}


			v_Bool() {
				name = "bool";
			}
		};

		class v_String : public VariableDefinition {
		public:
			void SetDefault(void** pOutput) override {
				//Since *pOutput is allready nullptr, we don't need to set it here
			}
			void SetValue(void** pOutput, const char* pValue) {
				//pValue = "string"
				string_size_t len = Strlen(pValue);
				char* out = new char[len - 1];
				//*pOutput = static_cast<void*>()
				for (string_size_t i = 1; i < len - 1;i++) {
					out[i - 1] = pValue[i];
				}
				*pOutput = (void*)out;
			}
			
			v_String() {
				name = "string";
			}
		};

		const u8 VAR_DEFS_AMOUNT = 9;
		VariableDefinition *VAR_DEFS[VAR_DEFS_AMOUNT] = {
			new v_BasicTypeTemplate<SIZE_TYPE>("SIZE_TYPE"),
			new v_BasicTypeTemplate<s8>("s8"),
			new v_BasicTypeTemplate<u8>("u8"),
			new v_BasicTypeTemplate<s16>("s16"),
			new v_BasicTypeTemplate<u16>("u16"),
			new v_BasicTypeTemplate<s32>("s32"),
			new v_BasicTypeTemplate<u32>("u32"),
			//Due to using stoi we cant use long longs
			//new v_BasicTypeTemplate<s64>("s64"),
			//new v_BasicTypeTemplate<u64>("u64"),
			new v_Bool(), //Anything but
			new v_String(),
		};

		
	}


	namespace event {
		void objectEnumOnChange(u8 pNewVal);
	}

	class tEditor : public Tool {
	public:

		struct {
			std::vector<File::ObjectDefinition> defs;

			//Region<EditorObject> objects;

			u8 index = 0;

		} mmObject;

		Screen* mScreen = nullptr;


		void OnLoad() override {
			Log("tEditor begin");
			size_t fSize = 0;
			char* file = static_cast<char*>(SDL_LoadFile(FileSystem::GetStringAsAsset("_dev/objects.def"), &fSize));
			if (!file) {
				Log("Failed to load _dev/objects.def");
				return;
			}

			enum Mode : u8 {
				M_O_GET_NAME = 0,
				M_O_GET_VARNAME = 1,
				M_O_GET_VALUE = 2,
			};

			Mode mode = M_O_GET_NAME;
			std::vector<File::ObjectDefinition>& defs = mmObject.defs;
			
			char tempRead[Editor::OBJECT_NAME_MAX + 1] = "!";
			u8 tempReadIndex = 0;

			struct {
				bool isDerived = false;
				bool isTemplate = false;

				void reset() {
					isDerived = false;
					isTemplate = false;
				}
			} mGetName;

			struct {
				bool isOverride = false;
				Variable* targetVar = nullptr;
				void reset() {
					isOverride = false;
					targetVar = nullptr;
				}
			} mGetVarName;

			struct {
				bool gotType = false;
				File::VariableDefinition* curDef = nullptr;
				u8 parameterCount = 0;
				void reset() {
					gotType = false;
					curDef = nullptr;
				}
			} mGetValue;

			bool mainDoReset = false;
			const char* test;
			for (size_t i = 0; i < fSize;i++) {
				char& r = file[i];
				//Skip spaces and newlines
				if (r == 0x20 || r == 0x09 || r == 0x0A || r == 0x0D) continue;
				if (r == '/' && file[i + 1] == '/') {
					while (file[i++] != '\n');
					continue;
				}
				switch (mode) {
				case M_O_GET_NAME:
					if (r == '{') {
						if (mGetName.isDerived) {
							//TODO - Implement like this:
							//Find object by name
							//Copy variables to created object
							tempRead[tempReadIndex] = '\0';
							for (u8 i = 0; i < static_cast<u8>(defs.size());i++) {
								char* temp = format("#%s", tempRead);
								std::vector<Variable>& vars = defs.at(i).variables;
								if (SDL_strcmp(defs.at(i).name, temp) == 0) {
									for (u8 i = 0; i < static_cast<u8>(vars.size());i++) {
										defs.back().variables.push_back(vars.at(i));
									}
									break;
								}
								delete[] temp;
								
							}
						}
						else {
							tempRead[tempReadIndex] = '\0';
							defs.push_back(File::ObjectDefinition());
							tempReadIndex = 0;
							while (tempRead[tempReadIndex] != '\0') {
								defs.back().name[tempReadIndex] = tempRead[tempReadIndex];
								tempReadIndex++;
							}
							defs.back().name[tempReadIndex] = '\0';
						}
						mode = M_O_GET_VARNAME;
						mainDoReset = true;
						mGetName.reset();
					}
					else if (r == ':') {
						mGetName.isDerived = true;
						tempRead[tempReadIndex] = '\0';
						defs.push_back(File::ObjectDefinition());
						tempReadIndex = 0;
						while (tempRead[tempReadIndex + 1] != '\0') {
							defs.back().name[tempReadIndex] = tempRead[tempReadIndex + 1];
							tempReadIndex++;
						}
						defs.back().name[tempReadIndex] = '\0';
						tempReadIndex = 0;
						goto _continue;
					}
					else if (r == '#') {
						mGetName.isTemplate = true;
						//tempReadIndex = 0;
						//goto _continue;
					}


					break;

				case M_O_GET_VARNAME:
					if (r == ':') {
						tempRead[tempReadIndex] = '\0';
						if (mGetVarName.isOverride) {
							//Set the target variable
							u8 i = 0;
							u8 defSize = static_cast<u8>(defs.back().variables.size());
							for (; i < defs.back().variables.size();i++) {
								if (SDL_strcmp(defs.back().variables.at(i).name, tempRead) == 0) {
									mGetVarName.targetVar = &defs.back().variables.at(i);
									break;
								}
							}
							if (i == defSize) {
								throw("Failed to find override value!");
							}
							
						}
						else {
							
							defs.back().variables.push_back(Variable());
							
							SDL_strlcpy(defs.back().variables.back().name, tempRead, 16);
							mGetVarName.targetVar = &defs.back().variables.back();
						}
						mode = M_O_GET_VALUE;
						mainDoReset = true;
					}
					else if (r == '}') {
						//Check for duplicate variables
						std::vector<Variable>& vars = defs.back().variables;
						u8 varSize = static_cast<u8>(vars.size());
						for (u8 i = 0; i < varSize - 1;i++) {
							for (u8 j = i + 1; j < varSize;j++) {
								if (SDL_strcmp(vars.at(i).name, vars.at(j).name) == 0) {
									Log("Two variables have the name name at indexes (%d) and (%d)! (%s)", (int)i, (int)j, vars.at(i).name);
								}
							}
						}
						mode = M_O_GET_NAME;
						mainDoReset = true;
					}
					else if (r == '#') {
						mGetVarName.isOverride = true;
						tempReadIndex = 0;
						goto _continue;
					}
					break;

				case M_O_GET_VALUE:
					if (r == '(') {
						tempRead[tempReadIndex] = '\0';
						//Find variable definition
						u8 i = 0;
						for (; i < File::VAR_DEFS_AMOUNT;i++) {
							File::VariableDefinition& d = *File::VAR_DEFS[i];
							if (SDL_strcmp(d.name, tempRead) == 0) {
								mGetVarName.targetVar->type = i;
								d.SetDefault(&mGetVarName.targetVar->value);
								mGetValue.curDef = &d;
								break;
							}
						}
						if (i == File::VAR_DEFS_AMOUNT) {
							throw("Failed to find variable type!");
						}
						tempReadIndex = 0;
					}
					else if (r == ')') {
						tempRead[tempReadIndex] = '\0';
						test = (tempRead + 1);
						mGetValue.curDef->SetValue(&mGetVarName.targetVar->value, test);
					}
					
					else if (r == ';') {
						mode = M_O_GET_VARNAME;
						mGetValue.reset();
						mGetVarName.reset();
						mainDoReset = true;

					}
					/*
					Should this be here? Since variable definitions have derived functions for handling it, we should be able to seperate the parameters,
					But that will be alot of code if we have it in the class's code for each class
					else if (r == ',') {
						mGetValue.parameterCount++;
						tempRead[tempReadIndex] = '\0'; //Put a null-terminator here so we can distingush it when copying
						goto _continue;
					}
					*/
					break;
				}
				if (mainDoReset) {
					tempReadIndex = 0;
					mainDoReset = false;
					_continue:
					continue;
				}
				tempRead[tempReadIndex++] = r;
				if (tempReadIndex >= Editor::OBJECT_NAME_MAX) {
					throw("tempIndex exceeded 32!");
				}
			}

			mScreen = new Screen();
			u8 mWindow = mScreen->AddWidget(new wWindow({ 0, 0, 300, 540 }, Style::C_W_BACKGROUND, true, true));

			u8 objectCount = defs.size();
			char** names = new char* [objectCount] {nullptr};
			for (u8 i = 0; i < objectCount;i++) {
				if (defs.at(i).name[0] == '#') {
					defs.erase(defs.begin() + i);
					objectCount--;
					continue;
				}
				names[i] = format("%s", defs.at(i).name);
			}
			mScreen->AddWidget(new wEnum(RRect{ 20, 40, 100, 20 }, (const char**)names, objectCount, 20, ), nullptr, mWindow);
			mScreen->active = false;
			mScreen->AddWidget(new wLabel(RRect{20, 20, 0, 0 }, "Current Object"));

			MGUI::AddScreen(mScreen);
			//defs.empty();
		}
		void OnToggled() override {
			//GetScreen(0)->active = !GetScreen(0)->active;
			for (u8 i = 0; i < MGUI::gScreenCount;i++) {
				if (i == mScreen->id) continue;
				MGUI::gScreens[i]->active = !MGUI::gScreens[i]->active;
			}
			mScreen->active = !mScreen->active;
		}
	};

	namespace event {
		void objectEnumOnChange(u8 pNewVal) {
			GetTool<tEditor>("editor")->mmObject.index = pNewVal;
		}
	}
}

DEFINE_TOOL(DevToolEditor, Developer::tEditor)


#endif