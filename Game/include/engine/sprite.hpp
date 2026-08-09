#ifndef __SPRITE_H__
#define __SPRITE_H__

//A sprite is a system that contains a texture page full of animation frames,
//And uses another file that shows each animations data

struct Sprite_Animation;

struct Sprite {
	//texture_t

	u8 animIndex = 0;

	u8 frameIndex = 0;

#ifndef CO_O_LOWRAM
	u8 frameIndexMax = 0; //Cached for performance
	u8 frameDelay = 0;
	u8 frameDelayMax = 0; //Cached for perforance
#else
	u8 frameDelay = 0;
#endif


	bool paused : 4;
	bool loop : 4;

	void* texture = nullptr;
	Sprite_Animation* anims = nullptr;
	u16* offsets = nullptr;

	void SetAnimationIndex(u8 pIndex);

	void SetFrameIndex(u8 pNewFrame);


	void Update();
};

#endif