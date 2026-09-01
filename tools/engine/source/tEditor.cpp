#include <tEditor.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
using namespace MGUI;

#include <magea/dev/developer.hpp>

#include <magea/asset.hpp>


template class Region<Developer::EditorObject>;
template class Region<RRect>;

namespace Developer {
	template<typename T>
	T* Variable::GetAs() {
		return static_cast<T*>(value);
	}
	

	EditorObject::EditorObject(u8 pID, vector pPos) : ID(pID) {
		EM_Object& o = GetTool<tEditor>("editor")->GetMode<EM_Object>();
		std::vector<Variable>& vars = o.defs.at(ID).variables;
		u8 vSize = static_cast<u8>(vars.size());
		variableCount = vSize;
		variables = new Variable[vSize]{Variable()};
		SIZE_TYPE* width = nullptr;
		SIZE_TYPE* height = nullptr;
		for (u8 i = 0; i < vSize;i++) {
			variables[i] = vars.at(i);
			if (SDL_strcmp(variables[i].name, "width") == 0) {
				width = variables[i].GetAs<SIZE_TYPE>();
			}
			else if (SDL_strcmp(variables[i].name, "height") == 0) {
				height = variables[i].GetAs<SIZE_TYPE>();
			}

		}
		SDL_assert(width);
		SDL_assert(height);
		rect = RRect{ pPos.x, pPos.y, static_cast<u16>(*width), static_cast<u16>(*height) };
	}

	Variable& EditorObject::GetVar(const char* pName) {
		for (u8 i = 0; i < variableCount;i++) {
			if (SDL_strcmp(variables[i].name, pName) == 0) {
				return variables[i];
			}
		}
		throw("Unable to find variable!");
	}
	Variable& File::ObjectDefinition::GetVar(const char* pName) {
		for (u8 i = 0; i < static_cast<u8>(variables.size());i++) {
			if (SDL_strcmp(variables.at(i).name, pName) == 0) {
				return variables.at(i);
			}
		}
		throw("Unable to find variable!");
	}

	void EM_Object::Update() {
		static u8 prevMouse = 0;
		if (Input::gMouseState.state == SDL_BUTTON_LEFT && prevMouse == 0) {
			objects.Add(EditorObject(GetTool<tEditor>("editor")->GetMode<EM_Object>().index, vector{ static_cast<s16>(Input::gMouseState.x), static_cast<s16>(Input::gMouseState.y) }));
		}
		prevMouse = Input::gMouseState.state;
	}

	void EM_Object::Render() {
		static EM_Object& o = GetTool<tEditor>("editor")->GetMode<EM_Object>();
		static SIZE_TYPE *curWidth = o.getDefFromCurrent().GetVar("width").GetAs<SIZE_TYPE>();
		static SIZE_TYPE *curHeight = o.getDefFromCurrent().GetVar("height").GetAs<SIZE_TYPE>();
		Render::FillRect(RRect{ static_cast<s16>(Input::gMouseState.x), static_cast<s16>(Input::gMouseState.y), static_cast<u16>(*curWidth), static_cast<u16>(*curHeight) }, { 0, 255, 0, 144 });
	
	}

	void EM_Object::GlobalRender() {
		Render::SetColor({ 0, 255, 0, 255 });
		for (u8 i = 0; i < objects.count;i++) {
			EditorObject& c = objects.At(i);
			Render::FillRect(c.rect);
		}
	}

	void EM_Object::Reset() {
		objects.Reset();
	}

	void EM_Collision::Update() {

	}

	void EM_Collision::Render() {

	}

	void EM_Collision::GlobalRender() {

	}
	void EM_Collision::Reset() {

	}

	void EM_Tiles::Update() {

	}
	void EM_Tiles::Render() {

	}
	void EM_Tiles::GlobalRender() {

	}
	void EM_Tiles::Reset() {

	}
	void EM_Tiles::updateData() {

	}

	File::ObjectDefinition& EM_Object::getDefFromCurrent() {
		return GetTool<tEditor>("editor")->GetMode<EM_Object>().defs.at(index);
	}

	template<typename T>
	T& tEditor::GetMode() {
		ModeID id = M_OBJECT;
		if(typeid(T) == typeid(EM_Object))
			return *static_cast<T*>(mModes[M_OBJECT]);
		if(typeid(T) == typeid(EM_Collision))
			return *static_cast<T*>(mModes[M_COLLISION]);
		if(typeid(T) == typeid(EM_Tiles))
			return *static_cast<T*>(mModes[M_TILES]);
		throw("Undefined editor type!");
	}

	void tEditor::OnLoad() {
		mModes = new EditorMode*[M_COUNT]{
			new EM_Object(),
			new EM_Collision(),
			new EM_Tiles(),
		};

		Log("tEditor begin");
		size_t fSize = 0;
		char* file = static_cast<char*>(SDL_LoadFile(FileSystem::GetStringAsAsset("_dev/objects.def"), &fSize));
		if (!file) {
			Log("Failed to load _dev/objects.def");
			return;
		}
#pragma region Object Def File Loading
		enum Mode : u8 {
			M_O_GET_NAME = 0,
			M_O_GET_VARNAME = 1,
			M_O_GET_VALUE = 2,
		};

		Mode mode = M_O_GET_NAME;
		EM_Object& mmObject = GetMode<EM_Object>();

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
#pragma endregion

		mScreen = new Screen();
		u8 oWindow = mScreen->AddWidget(new wWindow({ 0, 0, 200, 300 }, Style::C_W_BACKGROUND, true, true));
#pragma region Enum Names
		u8 objectCount = defs.size();
		char** names = new char* [objectCount] {nullptr};
		for (u8 i = 0; i < objectCount;i++) {
			if (defs.at(i).name[0] == '#') {
				defs.erase(defs.begin() + i);
				objectCount--;
				i--;
				continue;
			}
			names[i] = format("%s", defs.at(i).name);
		}
#pragma endregion
		mScreen->AddWidget(new wLabel(RRect{ 20, 40, 0, 0 }, "Current Object"), nullptr, oWindow);
		mScreen->AddWidget(new wEnum(RRect{ 20, 60, 100, 20 }, (const char**)names, objectCount, 20, [&mmObject](u8 pPrev, u8 pNext) -> void {
			mmObject.index = pNext;
		})
		, nullptr, oWindow);

		u8 tWindow = mScreen->AddWidget(new wWindow({ 0, 300, 300, 200 }, Style::C_W_BACKGROUND, true, true));
		mScreen->AddWidget(new wLabel(RRect{ 0, 30, 0, 0 }, "Layer"), nullptr, tWindow);
		mScreen->AddWidget(new wNumber<u8>(RRect{60, 20, 140, 20}, 0, 2), nullptr, tWindow);
		Render::texture* t = new Render::texture();
		Asset::Request((void**)t, "texture/tileset.png", Asset::AL_Texture);
		mScreen->AddWidget(new wTexture(RRect{ 0, 40, static_cast<u16>(16 * 5), static_cast<u16>(16 * 4)}, t), nullptr, tWindow);

		mScreen->active = false;
		

		MGUI::AddScreen(mScreen);
		//defs.empty();
	}

	void tEditor::OnToggled() {
		//GetScreen(0)->active = !GetScreen(0)->active;
		for (u8 i = 0; i < MGUI::gScreenCount;i++) {
			if (i == mScreen->id) continue;
			MGUI::gScreens[i]->active = !MGUI::gScreens[i]->active;
		}
		mScreen->active = !mScreen->active;
	}

	void tEditor::Update() {
		for (u8 i = 0; i < M_COUNT;i++) {
			mModes[i]->GlobalRender();
			if (mScreen->mMouseOverWidget) continue;
			if (mModeIndex == i) {
				mModes[i]->Update();
				mModes[i]->Render();
			}
			
		}
	}

}