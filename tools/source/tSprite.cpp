#ifndef NDEBUG
#pragma once

#include <core/common.hpp>
#include <magea/dev/tool.hpp>

#include <core/file.hpp>
#include <core/comp/renderer.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>

#include <magea/dev/developer.hpp>

//Duh
#include <magea/sprite.hpp>

//For loading sprites and textures
#include <magea/asset.hpp>

//For resizable arrays
#include <vector>

#include <api.hpp>

using namespace MGUI;
namespace Developer {

	//Create from image
	void mguiMainButtonCreatePressed();
	//Open an existing
	void mguiMainButtonOpenPressed();
	//Merge two texture pages into one
	void mguiMainButtonMergePressed();
	//Saves the opened file
	void mguiMainButtonSavePressed();
	//Closes the opened file
	void mguiMainButtonCloseOnClicked();

	void mguiMainButtonAnimPrevOnClicked();

	void mguiMainButtonCreateAnimOnClicked();

	void mguiMainButtonAnimNextOnClicked();

	void mguiMainButtonAnimFramePrevOnClicked();

	void mguiMainButtonAnimFrameNextOnClicked();

	void mguiMainButtonApplyOnClicked();

	void mguiMainButtonOpenOnClicked();

	RRect VIEWER_RECT = { 400, 20, 512, 512 };

	RRect OUTPUT_RECT = { 0, 0, 0, 0 };

	class tSprite : public Tool {
	public:
		FileSystem::File mCurrent;
		Render::texture mTexturePage;
		
		RRect mViewPos{ 0 };

		struct {
			std::vector<Sprite_Animation> anims;
			std::vector<u16> offsets;
			//Editor stuff
			u8 animIndex = 0;
			u8 frameIndex = 0;
			u8 frameDelay = 0;
		
		} mSprite;


		Screen* mMainScreen = nullptr;

		void OnLoad() override {
			mSprite.anims.push_back(Sprite_Animation());
			mSprite.offsets.push_back(0);
			mMainScreen = new Screen();
			
			mMainScreen->AddWidget(new wWindow({ 0, 0, 300, 540 }, Style::C_BACKGROUND, false));
			//File buttons
			mMainScreen->AddWidget(new wButton({ 0, 0, 100, 20 }, mguiMainButtonCreatePressed, "Create From Img"));
			mMainScreen->AddWidget(new wButton({ 0, 20, 100, 20 }, mguiMainButtonSavePressed, "Save Current"));
			mMainScreen->AddWidget(new wButton({ 100, 20, 100, 20 }, mguiMainButtonCloseOnClicked, "Close"));
			mMainScreen->AddWidget(new wButton({ 100, 0, 100, 20 }, mguiMainButtonOpenOnClicked, "Open Sprite"));
			mMainScreen->AddWidget(new wButton({ 200, 0, 100, 20 }, mguiMainButtonCreatePressed, "Merge Tex"));
			mMainScreen->AddWidget(new wButton({ 0, 60, 100, 20 }, mguiMainButtonCreateAnimOnClicked, "Create Anim"));
			
			s16 settingOffset = 100;

			mMainScreen->AddWidget(new wLabel({ 20, static_cast<s16>(settingOffset - 10), 0, 0 }, "Anim Index"));
			mMainScreen->AddWidget(new wButton({ 20, settingOffset, 20, 20 }, mguiMainButtonAnimPrevOnClicked, "<-"));
			mMainScreen->AddWidget(new wWindow({ 40, settingOffset, 40, 20}, Style::C_W_BACKGROUND, true));
			mMainScreen->AddWidget(new wLabel({ 42, static_cast<s16>(settingOffset + 10), 0, 0 }, "0"), "anim_label");
			mMainScreen->AddWidget(new wButton({ 80, settingOffset, 20, 20 }, mguiMainButtonAnimNextOnClicked, "->"));


			//Options
			mMainScreen->AddWidget(new wLabel({ 20, static_cast<s16>(settingOffset + 40), 0, 0 }, "Animation Width"));
			mMainScreen->AddWidget(new wTextInput({ 20, static_cast<s16>(settingOffset + 60), 100, 20 }, 5, wTextInput::F_NUMBERS), "anim_width");

			mMainScreen->AddWidget(new wLabel({ 20, static_cast<s16>(settingOffset + 90), 0, 0 }, "Animation Height"));
			mMainScreen->AddWidget(new wTextInput({ 20, static_cast<s16>(settingOffset + 100), 100, 20 }, 5, wTextInput::F_NUMBERS), "anim_height");

			mMainScreen->AddWidget(new wLabel({ 140, static_cast<s16>(settingOffset + 50), 0, 0 }, "Frame Amount"));
			mMainScreen->AddWidget(new wTextInput({ 140, static_cast<s16>(settingOffset + 60), 100, 20 }, 5, wTextInput::F_NUMBERS), "anim_frameAmt");

			mMainScreen->AddWidget(new wLabel({ 140, static_cast<s16>(settingOffset + 90), 0, 0 }, "Frame Delay"));
			mMainScreen->AddWidget(new wTextInput({ 140, static_cast<s16>(settingOffset + 100), 100, 20 }, 5, wTextInput::F_NUMBERS), "anim_frameDel");

			mMainScreen->AddWidget(new wButton({ 80, static_cast<s16>(settingOffset + 120), 100, 20 }, mguiMainButtonApplyOnClicked, "Apply Settings"));

			mMainScreen->AddWidget(new wLabel({ 140, static_cast<s16>(settingOffset + 200), 0, 0}, "Sprite Type"), "e_type");
			const char* spriteTypeValues[] = {
				"AnimOnly",
				"Paletted",
				"RGBAImg",
			};
			mMainScreen->AddWidget(new wEnum({ 140, static_cast<s16>(settingOffset + 220), 100, 20 }, spriteTypeValues, 3, 20));

			s16 viewOffset = 300;
			/*
			mMainScreen->AddWidget(new wLabel({ 20, static_cast<s16>(viewOffset), 0, 0 }, "Frame Index"));
			mMainScreen->AddWidget(new wButton({ 20, static_cast<s16>(viewOffset + 20), 20, 20 }, mguiMainButtonAnimFramePrevOnClicked, "<-"));
			mMainScreen->AddWidget(new wWindow({ 40, static_cast<s16>(viewOffset + 20), 40, 20 }, Style::C_W_BACKGROUND, true));
			mMainScreen->AddWidget(new wLabel({ 42, static_cast<s16>(viewOffset + 20), 0, 0 }, "0"), "anim_frameInd");
			mMainScreen->AddWidget(new wButton({ 80, static_cast<s16>(viewOffset + 20), 20, 20 }, mguiMainButtonAnimFrameNextOnClicked, "->"));
			*/


			//mMainScreen->AddWidget(new wScrollbar({ VIEWER_RECT.x, static_cast<s16>(VIEWER_RECT.y + VIEWER_RECT.height), VIEWER_RECT.width, 20 }, VIEWER_RECT.height));
		}

		void Update() override {
			mMainScreen->UpdateAndRender();
			if (mTexturePage.mTexture) {
				Render::Rect(VIEWER_RECT, { 0, 0, 0, 255 });
				Render::Texture(&mTexturePage, VIEWER_RECT);
				s16 offset = 0;
				RRect r;
				u8 i;
				for (u8 aI = 0; aI < mSprite.anims.size();aI++) {
					Sprite_Animation& a = mSprite.anims.at(aI);
					for (i = 0; i < a.frameAmount;i++) {
						Render::Rect({ static_cast<s16>(VIEWER_RECT.x + (i * a.width)), static_cast<s16>(VIEWER_RECT.y + offset), a.width, a.height }, {0, 0, 0, 255});
						Render::Text({ static_cast<s16>(VIEWER_RECT.x + (i * a.width)), static_cast<s16>(VIEWER_RECT.y + offset + 16) }, "%d", (int)i);
					}
					offset += a.height;
				}

				Sprite_Animation& a = mSprite.anims.at(mSprite.animIndex);
				if (++mSprite.frameDelay >= a.frameDelay) {
					if (++mSprite.frameIndex >= a.frameAmount) {
						mSprite.frameIndex = 0;
					}
					mSprite.frameDelay = 0;
				}
				Render::Text({ OUTPUT_RECT.x, static_cast<s16>(OUTPUT_RECT.y - 20) }, "Sprite Output");
				Render::Rect(OUTPUT_RECT, { 0, 0, 0, 255 });
				Render::Texture(&mTexturePage, OUTPUT_RECT, { static_cast<s16>(a.width * mSprite.frameIndex), (s16)mSprite.offsets[mSprite.animIndex], a.width, a.height });

			}
		}

	};

	void updateOutputRect() {
		tSprite* s = GetTool<tSprite>("sprite");
		Sprite_Animation& a = s->mSprite.anims.at(s->mSprite.animIndex);
		OUTPUT_RECT.x = VIEWER_RECT.x + VIEWER_RECT.width;
		//Images should always be in powers of two, so this shouldn't be a floating point value.
		OUTPUT_RECT.y = static_cast<s16>(VIEWER_RECT.y + (VIEWER_RECT.height / 2) - (a.height / 2));
		OUTPUT_RECT.width = a.width * 2;
		OUTPUT_RECT.height = a.height * 2;
	}

	void createSprite(void* userdata, const char* const* filelist, int filter) {
		if (!filelist || !*filelist) return;
		tSprite* s = GetTool<tSprite>("sprite");
		char* fileDir = FileSystem::GetDirectoryBaseDirectory(*filelist, false);
		char* fileName = FileSystem::GetDirectoryFileName(*filelist, false);
		char* directory = format("%s/%s_sprite.spr", fileDir, fileName);
		Log("Output directory: %s", directory);
		s->mCurrent.Open(directory, "w", false);

		if (!s->mCurrent.IsOpen()) {
			Log("Failed to create sprite file!");
			delete[] directory;
			delete[] fileName;
			delete[] fileDir;
			return;
		}
		if (!Asset::Request(&s->mTexturePage.mTexture, *filelist, Asset::AL_Texture, false)) {
			Log("Failed to load texture page!");
			delete[] directory;
			delete[] fileName;
			delete[] fileDir;
			return;
		}
		RRect* r = &VIEWER_RECT;
		//r->x = 0;
		//r->y = 0;
		r->width = s->mTexturePage.GetWidth();
		r->height = s->mTexturePage.GetHeight();
		updateOutputRect();

		delete[] directory;
		delete[] fileName;
		delete[] fileDir;

		
	}

	void mguiMainButtonCreatePressed() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (s->mCurrent.IsOpen()) s->mCurrent.Close();
		SDL_DialogFileFilter filters[] = {
			{".PNG files", "png"}
		};
		FileSystem::OpenFile(createSprite, filters, 1);

	}

	void mguiMainButtonSavePressed() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (!s->mCurrent.IsOpen()) return;
		FileSystem::File& f = s->mCurrent;
		char header[] = { 's', 'p', 'r' };
		f.Write(header, 3);
		u8 type = s->mMainScreen->FindWidget("e_type")->GetAs<wEnum>()->dIndex;
		f.Write<u8>(type);
		f.Write<u8>(static_cast<u8>(s->mSprite.anims.size()));
		for (Sprite_Animation& a : s->mSprite.anims) {
			f.Write<SIZE_TYPE>(a.width);
			f.Write<SIZE_TYPE>(a.height);
			f.Write<SIZE_TYPE>(a.frameAmount);
			f.Write<SIZE_TYPE>(a.frameDelay);
		}
		switch (type) {
			case FileSystem::S_PaletteImage:
				SDL_Color* outPalette;
				u8 outPalCount;
				u8* outImage;
				FileSystem::ConvertTextureToPaletteImage(&s->mTexturePage, outPalette, &outPalCount, outImage);
				f.Write<u8>(outPalCount);
				f.Write<SIZE_TYPE>(s->mTexturePage.GetWidth());
				f.Write<SIZE_TYPE>(s->mTexturePage.GetHeight());
				f.Write(outPalette, outPalCount * sizeof(SDL_Color));
				f.Write(outImage, s->mTexturePage.GetWidth() * s->mTexturePage.GetHeight());
			break;

			case FileSystem::S_RgbImage:
				break;
			break;
		}
		f.Close();
		Log("Wrote sprite!");
		//FileSystem::CreateFile(saveSprite, filters, 1);
	}

	void mguiMainButtonAnimPrevOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (s->mSprite.animIndex - 1 < 0) return;
		s->mSprite.animIndex--;
		s->mMainScreen->FindWidget("anim_label")->GetAs<wLabel>()->SetText(format("%d", (int)s->mSprite.animIndex));
		updateOutputRect();
	}

	void mguiMainButtonAnimNextOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (s->mSprite.animIndex + 1 >= s->mSprite.anims.size()) return;
		s->mSprite.animIndex++;
		s->mMainScreen->FindWidget("anim_label")->GetAs<wLabel>()->SetText(format("%d", (int)s->mSprite.animIndex));
		updateOutputRect();

	}

	void mguiMainButtonCreateAnimOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		s->mSprite.anims.push_back(Sprite_Animation());
		s->mSprite.offsets.push_back(0);
		s->mSprite.animIndex = static_cast<u8>(s->mSprite.anims.size() - 1);
		s->mMainScreen->FindWidget("anim_label")->GetAs<wLabel>()->SetText(format("%d", (int)s->mSprite.animIndex));
		u16 totalOffset = 0;
		for (u8 i = 0; i < (u8)s->mSprite.anims.size();i++) {
			s->mSprite.offsets.at(i) = totalOffset;
			totalOffset += s->mSprite.anims.at(i).height;
		}
	}

	void mguiMainButtonAnimFramePrevOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (s->mSprite.anims.size() == 0 || s->mSprite.frameIndex + 1 >= s->mSprite.anims.at(s->mSprite.animIndex).frameAmount) return;
		s->mSprite.frameIndex--;
	}
	void mguiMainButtonAnimFrameNextOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		if (s->mSprite.frameIndex - 1 < 0) return;
		s->mSprite.frameIndex++;
	}

	void mguiMainButtonApplyOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");

		int width = s->mMainScreen->FindWidget("anim_width")->GetAs<wTextInput>()->uGetAsInteger();
		int height = s->mMainScreen->FindWidget("anim_height")->GetAs<wTextInput>()->uGetAsInteger();
		int frameAmt = s->mMainScreen->FindWidget("anim_frameAmt")->GetAs<wTextInput>()->uGetAsInteger();
		int frameDel = s->mMainScreen->FindWidget("anim_frameDel")->GetAs<wTextInput>()->uGetAsInteger();
		Sprite_Animation& a = s->mSprite.anims.at(s->mSprite.animIndex);
		a.width = static_cast<SIZE_TYPE>(width);
		a.height = static_cast<SIZE_TYPE>(height);
		a.frameAmount = static_cast<u8>(frameAmt);
		a.frameDelay = static_cast<u8>(frameDel);
		updateOutputRect();
	}

	void openFile(void* userdata, const char* const* filelist, int filter) {
		if (!filelist || !*filelist) return;
		tSprite* s = GetTool<tSprite>("sprite");
		Sprite output = { 0 };
		bool b = Asset::Request((void**)&output, *filelist, Asset::AL_Sprite, false);
		if (!b) {
			Log("Failed to load sprite!");
			return;
		}
		s->mTexturePage.mTexture = output.texture.mTexture;
		//Reset
		s->mSprite.animIndex = 0;
		s->mSprite.frameIndex = 0;
		s->mSprite.offsets.clear();
		s->mSprite.anims.clear();
		
		for (u8 i = 0; i < output.uGetAnimCount();i++) {
			s->mSprite.anims.push_back(output.anims[i]);
			s->mSprite.offsets.push_back(output.offsets[i]);
		}

		/*
		FileSystem::File& f = s->mCurrent;
		f.Open(*filelist, "r", false);
		if (!f) {
			Log("failed to open sprite file! (%s)", *filelist);
			return;
		}
		
		char* read = (char*)f.Read(3);
		if (read[0] != 's' || read[1] != 'p' || read[2] != 'r') {
			delete[] read;
			Log("Invalid header!");
			f.Close();
			return;
		}
		delete[] read;
		u8 animCount = f.Read<u8>();
		s->mSprite.anims.resize(animCount, Sprite_Animation());
		s->mSprite.offsets.resize(animCount, 0);
		u16 totalOffset = 0;
		
		for (u8 i = 0; i < animCount;i++) {
			Sprite_Animation& c = s->mSprite.anims.at(i);
			s->mSprite.offsets.at(i) = totalOffset;
			c.width = f.Read<SIZE_TYPE>();
			c.height = f.Read<SIZE_TYPE>();
			c.frameAmount = f.Read<u8>();
			c.frameDelay = f.Read<u8>();
			totalOffset += c.height;
		}
		*/
	}

	void mguiMainButtonOpenOnClicked() {
		SDL_DialogFileFilter filters[] = {
			{"Sprite files", "spr"}
		};
		FileSystem::OpenFile(openFile, filters, 1);
	}

	void mguiMainButtonCloseOnClicked() {
		tSprite* s = GetTool<tSprite>("sprite");
		s->mCurrent.Close();
		s->mSprite.anims.resize(0);
		s->mSprite.offsets.resize(0);
		s->mSprite.animIndex = 0;
		s->mSprite.frameDelay = 0;
		s->mSprite.frameIndex = 0;
	}


}

DEFINE_TOOL(DevToolSprite, Developer::tSprite)
#endif