#include <magea/object.hpp>
#include <magea/asset.hpp>
#include <magea/sprite.hpp>

#include <core/comp/input.hpp>

#include <player.hpp>
#include <enemy.hpp>

#include <core/common.hpp>
#include <core/file.hpp>

#include <common.hpp>

void oPlayer::PreCache() {
	Asset::Request((void**) & mVisual.texture, "texture/plr.png", Asset::AL_Texture, true);
	//Asset::RequestTexture(&mVisual.texture, "texture/plr.png");
	mVisual.useSprite = false;
	mTransform.width = 50;
	mTransform.height = 100;
}

void oPlayer::OnTick() {
	char moveX = (Input::GetBindingIsDown("right") - Input::GetBindingIsDown("right"));
	mTransform.velocity.x = moveX * 10;
	if (Input::GetBindingWentDown("jump")) {
		mTransform.velocity.y = -10;
	}
	mVisual.UpdateSprite();
}
void oPlayer::CleanUp() {
	mVisual.sprite->texture->Destroy();
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
