#ifndef NDEBUG
#pragma once

#include <core/comopt.hpp>

#if CO_PACKED_FILES
#else

#include <core/comp/renderer.hpp>
#include <core/comp/input.hpp>
#include <magea/world.hpp>

#include <core/file.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
using namespace MGUI;

#include <core/common.hpp>

#include <magea/dev/tool.hpp>

#include <magea/dev/developer.hpp>

#include <api.hpp>

namespace Developer {

	struct Definition {
		char name[16] = "!";
		u8 width = 0, height = 0;
	};

	struct EditorObject {
		RRect position = { 0 };
		u8 id = 0; //The name's index in the definitions
	};

	void mguiMainButtonResetObjectsClicked();
	void mguiMainButtonResetCollisionClicked();

	void mguiMainEnumModeOnSelect(u8, u8);

	void mguiMainButtonTestClicked();

	void mguiMainEnumShapeTypeOnSelect(u8, u8);

	void mguiMainEnumObjectOnSelect(u8, u8);

	void exportWSFilePrompt(void* userdata, const char* const* filelist, int filter);

	class tEditor : public Tool {
	public:
		Screen* mScreenMain = nullptr;

		//MGUI::Screen* mScreenMain = nullptr;
		u8 mMode = 0;
		//0 - object placement
		//1 - collision placement

		//Object definitions
		Definition* mDefs = nullptr;
		u8 mDefCount = 0;

		vector mCameraPos = {-300, 0};

		struct {
			EditorObject createdObjects[World::OBJECT_MAX];
			u8 objectCount = 0;
			u8 curDef = 0;

			void reset() {
				for (u8 i = 0; i < objectCount;i++) {
					createdObjects[i] = { {0}, 0 };
				}
				objectCount = 0;
			}
		} mModeObject;

		struct {
			RRect rects[World::COLLISION_MAX];
			u8 count = 0;
			bool isBuilding = false;
			RRect origin;

			void reset() {
				count = 0;

			}
		} mModeCollision;

		void OnLoad() override {
			//mScreenMain = new MGUI::Screen();
			Log("(Tool) Loading editor.dll...");
			//mScreenMain->AddWidget(new MGUI::wLabel({20, 20}, "Editor Tool v0.1"));
			FileSystem::File defines;
			defines.Open("_dev/objects.txt");
			if (!defines.IsOpen()) {
				defines.Close();
				Log("Failed to load defines.txt! Reverting...");
				return;
			}

			//Are we reading the size data
			bool readSize = false;

			char temp[16] = "!";
			u8 tempIndex = 0;

			//Get definition amount
			temp[0] = defines.Read<char>();
			temp[1] = '\0';
			defines.Read<char>(); //Skip the \n
			defines.Read<char>(); //Skip the \n
			u8 defCount = SDL_atoi(temp);
			mDefs = new Definition[defCount];
			mDefCount = defCount;

			u8 defIndex = 0;
			for (u64 i = 0; i < defines.mInfo.size;i++) {
				char read = defines.Read<char>();
				if (readSize) {
					if (read == ' ' || read == 0x20) {
						temp[tempIndex] = '\0';
						mDefs[defIndex].width = SDL_atoi(temp);
						tempIndex = 0;
						continue;
					}
					else if (read == 0x0A || read == 0x0D) {
						temp[tempIndex] = '\0';
						mDefs[defIndex].height = SDL_atoi(temp);
						if (++defIndex >= defCount) {
							break;
						}
						readSize = false;
						tempIndex = 0;
						defines.Read<char>();
						continue;
					}

				}
				else {
					if (read == ' ' || read == 0x20) {
						temp[tempIndex] = '\0';
						SDL_strlcpy(mDefs[defIndex].name, temp, 16);
						readSize = true;
						tempIndex = 0;
						continue;
					}
				}
				temp[tempIndex++] = read;
			}

			defines.Close();

			mScreenMain = new Screen();
			mScreenMain->AddWidget(new wWindow({ 0, 0, 300, 540 }, Style::C_W_BACKGROUND, true));

			mScreenMain->AddWidget(new wLabel({ 20, 20, 0, 0 }, "Editor Mode"));
			const char* modeStrings[] = {
				"Object Placement",
				"Collision Placement"
			};
			mScreenMain->AddWidget(new wEnum({ 20, 40, 100, 20 }, modeStrings, 2, 20, mguiMainEnumModeOnSelect));

			mScreenMain->AddWidget(new wButton(RRect{ 20, 100, 100, 20 }, mguiMainButtonResetObjectsClicked, "Reset Objects"));
			mScreenMain->AddWidget(new wButton(RRect{ 20, 120, 100, 20 }, mguiMainButtonResetCollisionClicked, "Reset Collision"));
			mScreenMain->AddWidget(new wButton(RRect{ 20, 140, 100, 20 }, mguiMainButtonTestClicked, "Test in-game"));
			
			constexpr u16 offsetO = 200;
			constexpr u16 offsetC = 400;

			mScreenMain->AddWidget(new wLabel({ 20, offsetO, 0, 0 }, "Object Section"));
			mScreenMain->AddWidget(new wLabel({ 20, offsetO + 20, 0, 0 }, "Current Object"));
			char** objectNameStrings = new char* [mDefCount] {nullptr};
			string_size_t s;
			for (u8 i = 0; i < mDefCount;i++) {
				s = SDL_strlen(mDefs[i].name);
				objectNameStrings[i] = new char[s + 1];
				SDL_strlcpy(objectNameStrings[i], mDefs[i].name, s + 1);
			}

			RRect r = { (s16)20, static_cast<s16>(offsetO + 40), (s16)100, (s16)20 };
			wEnum* iHateYou = new wEnum(r, (const char**)objectNameStrings, (u8)mDefCount, (u8)20, mguiMainEnumObjectOnSelect);
			mScreenMain->AddWidget(iHateYou);

			mScreenMain->AddWidget(new wLabel({ 20, static_cast<s16>(offsetC), 0, 0}, "Collision Section"));
			mScreenMain->AddWidget(new wLabel({ 20, static_cast<s16>(offsetC + 20), 0, 0}, "Shape Type"));
			const char* shapeStrings[] = {
				"Box",
				"Slope",
			};
			mScreenMain->AddWidget(new wEnum({ 20, static_cast<s16>(offsetC + 40), 100, 20 }, shapeStrings, 2, 20, mguiMainEnumShapeTypeOnSelect));
		}

		void Update() override {
			Render::FillRect({ static_cast<s16>(300 + mCameraPos.x), static_cast<s16>(300 + mCameraPos.y), 10000, 10000}, {0, 0, 0, 255});
			Render::FillRect({ static_cast<s16>(-mCameraPos.x), static_cast<s16>(-mCameraPos.y), 7000, 7000}, {255, 255, 255, 255});
			mScreenMain->UpdateAndRender();
			static u8 prevMouseState = 0;
			Definition& d = mDefs[mModeObject.curDef];
			//Render::Text({ 20, 120 }, "Mode: %d", (int)mMode);
			//Render::Text({ 20, 140 }, "Current Object: %s", d.name);


			mCameraPos.x += Input::GetAxisState("dev_e_horizontal") * 10;
			mCameraPos.y += Input::GetAxisState("dev_e_vertical") * 10;


			if (Input::GetBindingWentDown("dev_e_export")) {
				SDL_DialogFileFilter filters[] = {
					{"Worldspace Files", "ws"}
				};
				FileSystem::CreateFile(exportWSFilePrompt, filters, 1);
				//uExportWS();
			}
			else if (Input::GetBindingWentDown("dev_e_import")) {
				uImportWS();
			}

			if (!mScreenMain->mMouseOverWidget) {
				switch (mMode) {
				case 0: {
					RRect curObject = RRect{ static_cast<s16>(Input::gMouseState.x), static_cast<s16>(Input::gMouseState.y), d.width, d.height };
					Render::FillRect(curObject, Render::Color{ 0, 255, 0 });
					if (Input::gMouseState.state == SDL_BUTTON_LEFT && prevMouseState == 0) {
						if (mModeObject.objectCount + 1 >= World::OBJECT_MAX) {
							Log("Object limit reached!");
							return;
						}
						curObject.x += mCameraPos.x;
						curObject.y += mCameraPos.y;
						mModeObject.createdObjects[mModeObject.objectCount++] = { curObject, mModeObject.curDef };
					}
					if (Input::gMouseState.state == SDL_BUTTON_RIGHT) {
						for (u8 i = 0; i < mModeObject.objectCount;i++) {
							RRect& r = mModeObject.createdObjects[i].position;
							if (Input::gMouseState.x + mCameraPos.x >= r.x &&
								Input::gMouseState.x + mCameraPos.x <= r.x + r.width &&
								Input::gMouseState.y + mCameraPos.y >= r.y &&
								Input::gMouseState.y + mCameraPos.y <= r.y + r.height) {
								if (mModeObject.objectCount > 1) {
									for (u8 j = i;j < --mModeObject.objectCount;j++) {
										mModeObject.createdObjects[j] = mModeObject.createdObjects[j + 1];
									}
								}
								else {
									mModeObject.createdObjects[0] = { {0}, 0 };
									mModeObject.objectCount--;
								}
								break;
							}
						}
					}
					Render::Text({ (s16)Input::gMouseState.x, static_cast<s16>(Input::gMouseState.y - 8) }, "(%hu, %hu)", Input::gMouseState.x, Input::gMouseState.y);

				} break;

				case 1: {
					if (mModeCollision.isBuilding) {
						if (Input::gMouseState.state == SDL_BUTTON_RIGHT) {
							mModeCollision.isBuilding = false;
							break;
						}
						short w = (Input::gMouseState.x - mModeCollision.origin.x);
						short h = (Input::gMouseState.y - mModeCollision.origin.y);
						w = Grid<s16, s16>(w, World::GRID_SIZE);
						h = Grid<s16, s16>(h, World::GRID_SIZE);
						if (w < 0) {
							mModeCollision.origin.x += w;
							w = -w;
						}
						if (h < 0) {
							mModeCollision.origin.y += h;
							h = -h;
						}
						w = MIN(w, 32);
						h = MIN(h, 32);

						mModeCollision.origin.width = w;
						mModeCollision.origin.height = h;


						Render::FillRect(mModeCollision.origin, { 255, 0, 0 });
						if (Input::gMouseState.state == SDL_BUTTON_LEFT && prevMouseState == 0) {
							mModeCollision.origin.x = Grid<s16, s16>(mModeCollision.origin.x + mCameraPos.x, World::GRID_SIZE);
							mModeCollision.origin.y = Grid<s16, s16>(mModeCollision.origin.y + mCameraPos.y, World::GRID_SIZE);
							mModeCollision.rects[mModeCollision.count++] = mModeCollision.origin;
							mModeCollision.isBuilding = false;
						}
						Render::Text({ mModeCollision.origin.x, static_cast<s16>(mModeCollision.origin.y - 16) }, "(%hu, %hu, %hu, %hu)", Input::gMouseState.x, Input::gMouseState.y, w, h);

					}
					else {
						short x = Grid<s16, s16>(Input::gMouseState.x, World::GRID_SIZE);
						short y = Grid<s16, s16>(Input::gMouseState.y, World::GRID_SIZE);
						Render::FillRect({ x, y , World::GRID_SIZE, World::GRID_SIZE }, { 144, 0, 0 });
						Render::Text({ x, static_cast<s16>(y - 16) }, "(%hu, %hu)", Input::gMouseState.x, Input::gMouseState.y);
						if (Input::gMouseState.state == SDL_BUTTON_LEFT && prevMouseState == 0) {
							//mModeCollision.origin.x = Grid<s16, s16>(x + mCameraPos.x, World::GRID_SIZE);
							//mModeCollision.origin.y = Grid<s16, s16>(y + mCameraPos.x, World::GRID_SIZE);
							mModeCollision.origin.x = x;
							mModeCollision.origin.y = y;
							mModeCollision.isBuilding = true;
						}
						if (Input::gMouseState.state == SDL_BUTTON_RIGHT) {
							for (u8 i = 0; i < mModeCollision.count;i++) {
								RRect& r = mModeCollision.rects[i];
								if (Input::gMouseState.x + mCameraPos.x >= r.x &&
									Input::gMouseState.x + mCameraPos.x <= r.x + r.width &&
									Input::gMouseState.y + mCameraPos.y >= r.y &&
									Input::gMouseState.y + mCameraPos.y <= r.y + r.height) {
									if (mModeCollision.count > 1) {
										for (u8 j = i;j < --mModeCollision.count;j++) {
											mModeCollision.rects[j] = mModeCollision.rects[j + 1];
										}
									}
									else {
										mModeCollision.rects[0] = { 0, 0, 0, 0 };
										mModeCollision.count--;
									}
									break;
								}
							}
						}
					}

				} break;
				}

			}

			Render::gAlignMode.vertical = A_BOTTOM;
			for (u8 i = 0; i < mModeObject.objectCount;i++) {
				Render::FillRect({ static_cast<s16>(mModeObject.createdObjects[i].position.x - mCameraPos.x), static_cast<s16>(mModeObject.createdObjects[i].position.y - mCameraPos.y), mModeObject.createdObjects[i].position.width, mModeObject.createdObjects[i].position.height }, { 0, 144, 0 });
				Render::Text({ static_cast<s16>(mModeObject.createdObjects[i].position.x - mCameraPos.x), static_cast<s16>(mModeObject.createdObjects[i].position.y - mCameraPos.y) }, mDefs[mModeObject.createdObjects[i].id].name);
			}
			Render::gAlignMode.vertical = A_MIDDLE;
			for (u8 i = 0; i < mModeCollision.count;i++) {
				RRect& r = mModeCollision.rects[i];
				Render::FillRect({ static_cast<s16>(r.x - mCameraPos.x), static_cast<s16>(r.y - mCameraPos.y), r.width, r.height}, {0, 144, 0});
			}
			for (u8 i = 0; i < mModeCollision.count;i++) {
				RRect& r = mModeCollision.rects[i];
				Render::Text({ static_cast<s16>(r.x - mCameraPos.x), static_cast<s16>((r.y + 8) - mCameraPos.y) }, "(%hu, %hu, %hu, %hu, %hu, %hu)", r.x, r.y, r.width, r.height, 0, i);

			}

			prevMouseState = Input::gMouseState.state;
		}

		void uExportWS(FileSystem::File& pOutput) {
			char header[] = { 'w', 'o', 'r', 'd' };
			pOutput.Write(header, 4);
			pOutput.Write(&mModeObject.objectCount, 1);
			u8 strLen = 0;
			u8 i = 0;
			for (; i < mModeObject.objectCount;i++) {
				//Should be 8 bytes
				pOutput.Write(&mModeObject.createdObjects[i].position, 8);
				strLen = (u8)SDL_strlen(mDefs[mModeObject.createdObjects[i].id].name);
				pOutput.Write<u8>(strLen);
				pOutput.Write(mDefs[mModeObject.createdObjects[i].id].name, strLen);
				//pOutput.Write<u8>(0xFF);
			}
			pOutput.Write(&mModeCollision.count, 1);
			for (u8 i = 0; i < mModeCollision.count;i++) {
				//Up to 80World::GRID_SIZE values of data
				pOutput.Write<s16>(mModeCollision.rects[i].x);
				pOutput.Write<s16>(mModeCollision.rects[i].y);
				pOutput.Write<s16>(mModeCollision.rects[i].width);
				pOutput.Write<s16>(mModeCollision.rects[i].height);
				//Placeholder
				pOutput.Write<u8>(World::CS_BOX);
				Log("Wrote %h %h %h %h", mModeCollision.rects[i].x, mModeCollision.rects[i].y, mModeCollision.rects[i].width, mModeCollision.rects[i].height);
			}


			pOutput.Close();
		}
		void uImportWS() {
			FileSystem::File pInput("_dev/worldspacetest.ws", "r");
			if (!pInput.IsOpen())
				return;
			char* header = (char*)pInput.Read(4);
			if (header[0] != 'w' || header[1] != 'o' || header[2] != 'r' || header[3] != 'd') {
				Log("Unexpected header!");
				pInput.Close();
				return;
			}
			delete[] header;
			mModeObject.objectCount = pInput.Read<u8>();
			RRect* temp = nullptr;
			char readName[World::GRID_SIZE] = "!";
			u8 readIndex = 0;
			char read = 0;

			u8 d;

			u8 strLen = 0;
			u8 i = 0;
			for (; i < mModeObject.objectCount;i++) {
				temp = reinterpret_cast<RRect*>(pInput.Read(8));
				mModeObject.createdObjects[i].position = *temp;
				delete temp;
				strLen = pInput.Read<u8>();

				//The null terminated string
				char* readName = new char[strLen + 1];
				//The read file
				char* readString = (char*)pInput.Read(strLen);
				SDL_strlcpy(readName, readString, strLen + 1);

				for (d = 0; d < mDefCount;d++) {
					if (SDL_strcmp(mDefs[d].name, readName) == 0) {
						mModeObject.createdObjects[i].id = d;
						break;
					}
				}
				delete[] readName;
				delete[] readString;
				if (d == mDefCount) {
					throw("Failed to find object definition!");
				}
			}
			mModeCollision.count = pInput.Read<u8>();
			for (i = 0; i < mModeCollision.count;i++) {
				mModeCollision.rects[i].x = pInput.Read<s16>();
				mModeCollision.rects[i].y = pInput.Read<s16>();
				mModeCollision.rects[i].width = pInput.Read<s16>();
				mModeCollision.rects[i].height = pInput.Read<s16>();
				//Skip the shape byte!
				pInput.Read<u8>();
				//mModeCollision.rects[i].shape = (CollisionShape)pInput.Read<u8>();
			}

			pInput.Close();
		}
	};

	void mguiMainButtonResetObjectsClicked() {
		GetTool<tEditor>("editor")->mModeObject.reset();
	}
	void mguiMainButtonResetCollisionClicked() {
		GetTool<tEditor>("editor")->mModeCollision.reset();
	}

	void mguiMainEnumModeOnSelect(u8 pPrev, u8 pNext) {
		GetTool<tEditor>("editor")->mMode = pNext;
	}

	void mguiMainButtonTestClicked() {
		//GetTool<tEditor>("editor")->uExportWS();
		ToggleTools();
		World::LoadFromFile("_dev/worldspacetest.ws", true);
	}

	void mguiMainEnumShapeTypeOnSelect(u8 pPrev, u8 pNext) {
		Log("Placeholder!");
		//GetTool<tEditor>("editor")->mModeCollision;
	}

	void mguiMainEnumObjectOnSelect(u8 pPrev, u8 pNext) {
		GetTool<tEditor>("editor")->mModeObject.curDef = pNext;
	}

	void exportWSFilePrompt(void* userdata, const char* const* filelist, int filter) {
		if (!filelist || !*filelist) return;
		//"_dev/worldspacetest.ws"
		FileSystem::File pOutput(*filelist, "w", false);
		if (!pOutput.IsOpen())
			return;
		GetTool<tEditor>("editor")->uExportWS(pOutput);
	}
}

DEFINE_TOOL(DevToolEditor, Developer::tEditor)


#endif //CO_PACKED_FILES

#endif //NDEBUG