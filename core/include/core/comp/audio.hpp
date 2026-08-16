#ifndef __AUDIO_H__
#define __AUDIO_H__

//The audio system, handles audio playing and other boring stuff
#include <core/common.hpp>
#include <SDL3_mixer/SDL_mixer.h>
#include <core/comp/component.hpp>
COMPONENT_DEFINE_START(useAudio, Audio, true)
	constexpr u8 MAX_SOUNDS = 8;

	CORE_EXPORT bool Init();

	//Return the channel index of the sound that is playing.
	//Returns MAX_CHANNELS if there are no empty channels.
	CORE_EXPORT u8 PlaySound(MIX_Audio* pAudio);

	CORE_EXPORT void StopChannel(u8 pTrack, s64 pFadeOutFrames = 0);

	CORE_EXPORT void PlayMusic(MIX_Audio* pMusic, bool pLoop);

	CORE_EXPORT void StopMusic(s64 pFadeOutFrames = 0);

	CORE_EXPORT COMPONENT_INCLUDE_UNLOAD;

	CORE_EXPORT MIX_Mixer* gMixer;


COMPONENT_DEFINE_END

#endif