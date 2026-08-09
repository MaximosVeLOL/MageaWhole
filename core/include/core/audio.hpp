#ifndef __AUDIO_H__
#define __AUDIO_H__

//The audio system, handles audio playing and other boring stuff
#include <core/common.hpp>
#include <SDL3_mixer/SDL_mixer.h>
#include <core/component.hpp>
COMPONENT_DEFINE_START(useAudio, Audio)
	constexpr u8 MAX_SOUNDS = 8;

	extern MIX_Mixer* gMixer;

	[[nodiscard]] COMPONENT_INCLUDE_INIT;

	//Return the channel index of the sound that is playing.
	//Returns MAX_CHANNELS if there are no empty channels.
	u8 PlaySound(MIX_Audio* pAudio);

	void StopChannel(u8 pTrack, Sint64 pFadeOutFrames = 0);

	void PlayMusic(MIX_Audio* pMusic, bool pLoop);

	void StopMusic(Sint64 pFadeOutFrames = 0);

	COMPONENT_INCLUDE_UNLOAD;


COMPONENT_DEFINE_END

#endif