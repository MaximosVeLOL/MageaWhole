#include <engine/object.hpp>
#include <engine/api.hpp>

#include <player.hpp>
#include <enemy.hpp>

void oPlayer::PreCache() {
	Engine::gEngine.Asset_Request(&mVisual.texture, "texture/plr.png", Engine::gEngine.AL_Texture, true);
	//Asset::RequestTexture(&mVisual.texture, "texture/plr.png");
	mVisual.useSprite = false;
	mTransform.width = 50;
	mTransform.height = 100;
}

void oPlayer::OnTick() {
	char moveX = (Engine::gEngine.Input_GetBindingIsDown("right") - Engine::gEngine.Input_GetBindingIsDown("right"));
	mTransform.velocity.x = moveX * 10;
	if (Engine::gEngine.Input_GetBindingWentDown("jump")) {
		mTransform.velocity.y = -10;
	}
	mVisual.UpdateSprite();
}
void oPlayer::CleanUp() {
}
DEFINE_OBJECT(oPlayerExt, oPlayer);

void oEnemy::PreCache() {
	mVisual.LoadSprite("texture/enemy.png", "texture/enemy_sprite.png");
	mVisual.useSprite = true;
	mTransform.width = 32;
	mTransform.height = 32;
}

void oEnemy::OnTick() {
	mTransform.velocity.x = 2;

}
DEFINE_OBJECT(oEnemyExt, oEnemy);
