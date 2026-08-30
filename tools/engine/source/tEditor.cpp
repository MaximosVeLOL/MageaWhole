#ifndef NDEBUG
#pragma once

#include <magea/dev/tool.hpp>

#include <api.hpp>

#include <SDL3/SDL.h>

#include <core/file.hpp>

#include <magea/world.hpp>

#include <vector>

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
				if (SDL_strcmp(pValue, "false") == 0) {
					*static_cast<bool*>(*pOutput) = false;
				}
			}


			v_Bool() {
				name = "bool";
			}
		};

		const u8 VAR_DEFS_AMOUNT = 10;
		VariableDefinition *VAR_DEFS[VAR_DEFS_AMOUNT] = {
			new v_BasicTypeTemplate<SIZE_TYPE>("SIZE_TYPE"),
			new v_BasicTypeTemplate<s8>("s8"),
			new v_BasicTypeTemplate<u8>("u8"),
			new v_BasicTypeTemplate<s16>("s16"),
			new v_BasicTypeTemplate<u16>("u16"),
			new v_BasicTypeTemplate<s32>("s32"),
			new v_BasicTypeTemplate<u32>("u32"),
			new v_BasicTypeTemplate<s64>("s64"),
			new v_BasicTypeTemplate<u64>("u64"),
			new v_Bool(),
		};

		
	}




	class tEditor : public Tool {
	public:

		struct {
			std::vector<File::ObjectDefinition> defs;

			Region<EditorObject> objects;

		} mmObject;


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
				switch (mode) {
				case M_O_GET_NAME:
					if (r == '{') {
						if (mGetName.isDerived) {
							//TODO - Implement like this:
							//Find object by name
							//Copy variables to created object
							tempRead[tempReadIndex] = '\0';
							for (u8 i = 0; i < static_cast<u8>(defs.size());i++) {
								if (SDL_strcmp(defs.at(i).name, tempRead) == 0) {
									for (u8 i = 0; i < static_cast<u8>(defs.at(i).variables.size());i++) {
										defs.back().variables.push_back(defs.at(i).variables.at(i));
									}
								}
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
						while (tempRead[tempReadIndex] != '\0') {
							defs.back().name[tempReadIndex] = tempRead[tempReadIndex];
							tempReadIndex++;
						}
						defs.back().name[tempReadIndex] = '\0';
						tempReadIndex = 0;
					}
					else if (r == '#') {
						mGetName.isTemplate = true;
					}


					break;

				case M_O_GET_VARNAME:
					if (r == ':') {
						defs.back().variables.push_back(Variable());
						tempRead[tempReadIndex] = '\0';
						SDL_strlcpy(defs.back().variables.back().name, tempRead, 16);
						mode = M_O_GET_VALUE;
						mainDoReset = true;
					}
					else if (r == '}') {
						mode = M_O_GET_NAME;
						mainDoReset = true;
					}
					break;

				case M_O_GET_VALUE:
					if (r == '(') {
						tempRead[tempReadIndex] = '\0';
						u8 i = 0;
						for (; i < File::VAR_DEFS_AMOUNT;i++) {
							File::VariableDefinition& d = *File::VAR_DEFS[i];
							if (SDL_strcmp(d.name, tempRead) == 0) {
								defs.back().variables.back().type = i;
								d.SetDefault(&defs.back().variables.back().value);
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
						mGetValue.curDef->SetValue(&defs.back().variables.back().value, test);
					}
					
					else if (r == ';') {
						mode = M_O_GET_VARNAME;
						mGetValue.reset();
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
					//_continue:
					continue;
				}
				tempRead[tempReadIndex++] = r;
				if (tempReadIndex >= Editor::OBJECT_NAME_MAX) {
					throw("tempIndex exceeded 32!");
				}
			}
			defs.empty();
		}
	};
}

DEFINE_TOOL(DevToolEditor, Developer::tEditor)


#endif