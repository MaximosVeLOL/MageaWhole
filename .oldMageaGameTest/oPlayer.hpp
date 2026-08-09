#pragma once

#include "object.h"
#include "engine.hpp"

class oPlayer : public Object {
public:

	MIX_Audio* testSound = nullptr;

	void PreCache() override {
		mTransform.position = { 20, 20 };
		mTransform.width = 64;
		mTransform.height = 64;
		mVisual.useSprite = true;
		ASSET_REQUEST_TEXTURE(&mVisual.texture, "texture/test.png");
		ASSET_REQUEST_AUDIO(&testSound, "sound/test.mp3");
	}

	void Tick() override {
		char moveX (gEngine.Input_GetBindingIsDown("right") - gEngine.Input_GetBindingIsDown("left"));
		mTransform.velocity.x = (moveX * 10);
		if (gEngine.Input_GetBindingWentDown("jump")) {
			gEngine.Audio_PlaySound(testSound);
		}
	}
};
DEFINE_OBJECT(oPlayerExt, oPlayer);