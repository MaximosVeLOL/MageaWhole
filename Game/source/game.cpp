#include <player.hpp>
#include <engine/api.hpp>

EXPORT bool Init() {
	
	Engine::gEngine.Log("!!Start of game!!");
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