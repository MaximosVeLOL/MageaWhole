#include <core/common.hpp>
#include <magea/engine.hpp>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	void* memTest = SDL_malloc(1024);
	if (!memTest) {
		DisplayError("Failed to allocate 1024 bytes of ram!\nPlease have atleast 1GB of memory available.");
		SDL_free(memTest);
		return SDL_APP_FAILURE;
	}
	SDL_free(memTest);

	unsigned char result = Engine::Init(argc, argv);
	return (result == 0 ? SDL_APP_CONTINUE : SDL_APP_FAILURE);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	//Log("!!Start of AppIterate!!");
	//Log("!!End of AppIterate!!");
	Engine::UpdateAndRender();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	return Engine::HandleEvent(event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	Engine::Stop(result);
}