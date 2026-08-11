#include <player.hpp>
#include <engine/api.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
using namespace MGUI;

#pragma region Real Stuff

class menuHack : public Object {
public:
	Screen mScreen;

	void OnTick() override {
		mScreen.UpdateAndRender();
	}
};

void mguiMainButtonStartGameOnPressed() {
	Log("Start Game!");
	World::LoadFromFile("ws/test01.ws");
}


#pragma endregion


EXPORT bool Init() {
	Engine::gEngine.Log("!!Start of game!");
	menuHack* h = new menuHack();
	h->mScreen.AddWidget(new wLabel({480, 100}, "MAIN MENU"));
	s16 buttonOffset = 300;
	h->mScreen.AddWidget(new wButton({ 480 - 50, buttonOffset + 20, 100, 20 }, mguiMainButtonStartGameOnPressed, "Start Game"));
	World::AddObject(h);

	//Engine::gEngine.Log("!!Start of game!!");
	//World::AddObject(new oPlayer());
	return true;
}

EXPORT u8 GetExpectedEngineVersion() {
	return 0x00;
}

EXPORT void Unload() {
	Engine::gEngine.Log("!!End of game!!");
}

EXPORT Engine::API* GetEngineFunctions() {
	return &Engine::gEngine;
}
