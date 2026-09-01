#ifndef NDEBUG
#pragma once


#include <magea/dev/tool.hpp>

#include <api.hpp>

#include <SDL3/SDL.h>

#include <core/file.hpp>

#include <magea/world.hpp>

#include <core/rect.hpp>
#include <core/vector.hpp>

#include <vector>

namespace MGUI {
	class Screen;
}

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
		T* GetAs();
	};

	struct EditorObject {
		//The index of the object's name
		u8 ID = 0;
		Variable* variables = nullptr;
		u8 variableCount = 0;

		RRect rect = {0};

		Variable& GetVar(const char* pName);

		EditorObject() {}
		EditorObject(u8 pID, vector pPos);
	};
	//Stuff for file parsing
	namespace File {

		struct ObjectDefinition {
			char name[Editor::OBJECT_NAME_MAX + 1] = "!";
			std::vector<Variable> variables;

			Variable& GetVar(const char* pName);
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
		VariableDefinition* VAR_DEFS[VAR_DEFS_AMOUNT] = {
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

	struct EditorMode {
		//Runs when we are updating on this mode
		virtual void Update() {}
		//Only runs if we are on this mode
		virtual void Render() {}
		//Renders no matter what
		virtual void GlobalRender() {}
		//Resets all the data inside
		virtual void Reset() {}
	};

	enum ModeID : u8 {
		M_OBJECT = 0,
		M_COLLISION = 1,
		M_TILES = 2,
		M_COUNT = 3,
	};

	struct EM_Object : EditorMode {
		void Update() override;
		void Render() override;
		void GlobalRender() override;
		void Reset() override;
		File::ObjectDefinition& getDefFromCurrent();
		std::vector<File::ObjectDefinition> defs;
		Region<EditorObject> objects;
		u8 index = 0;

		EM_Object() {
			objects.Init(World::OBJECT_MAX);
		}
	};
	struct EM_Collision : EditorMode {
		void Update() override;
		void Render() override;
		void GlobalRender() override;
		void Reset() override;
		Region<RRect> rects;
		RRect current = {0};

		EM_Collision() {
			rects.Init(World::COLLISION_MAX);
		}
	};
	struct EM_Tiles : EditorMode {
		void Update() override;
		void Render() override;
		void GlobalRender() override;
		void Reset() override;
		u8* tiles = nullptr;
		void updateData();

		EM_Tiles() {
			updateData();
		}
	};

	class tEditor : public Tool {
	public:
		//Bruh
		EditorMode** mModes = nullptr;
		ModeID mModeIndex = M_OBJECT;

		MGUI::Screen* mScreen = nullptr;

		template<typename T> 
		T& GetMode();


		void OnLoad() override;
		void OnToggled() override;

		void Update() override;
	};
}

DEFINE_TOOL(DevToolEditor, Developer::tEditor)


#endif