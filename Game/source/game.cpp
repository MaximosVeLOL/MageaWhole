#include <player.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
#include <common.hpp>

#include <magea/world.hpp>
#include <magea/asset.hpp>


using namespace MGUI;

void FlushMGUI() {
	RemoveScreen(0);
}

void mguiMainButtonStartGameOnPressed() {
	Log("Start Game!");
	World::LoadFromFile("ws/test01", true);
	SetPostCallEvent(FlushMGUI);
}

EXPORT bool Init() {
	//Engine::gEngine.Log("!!Start of game!");
	Log("!!Start of game!!");
	Screen* gScreen = new Screen();
	Render::texture* t;
	Asset::Request((void**) & t, "texture/ui_titlescreen.png", Asset::AL_Texture, true);
	gScreen->AddWidget(new wTexture({ 0, 0, 960, 540 }, t));

	gScreen->AddWidget(new wLabel({480, 100}, "MAIN MENU"));
	s16 buttonOffset = 300;
	gScreen->AddWidget(new wButton({ 430, static_cast<s16>(buttonOffset + 20), 100, 20 }, mguiMainButtonStartGameOnPressed, "Start Game"));
	//gScreen->Destroy();

	AddScreen(gScreen);


	//Engine::gEngine.Log("!!Start of game!!");
	//World::AddObject(new oPlayer());
	return true;
}

EXPORT u8 GetExpectedEngineVersion() {
	return 0x00;
}

EXPORT void Update() {
}

EXPORT void PreUpdate() {

}

EXPORT void PostUpdate() {

}

EXPORT void OnRender() {

}

EXPORT void Unload() {
	Log("!!End of game!!");
	//Engine::gEngine.Log("!!End of game!!");
}
/*
EXPORT Engine::API* GetEngineFunctions() {
	return &Engine::gEngine;
}
*/
