#include <core/audio.hpp>



namespace Audio {

	//Array of MIX_Mixers
	MIX_Mixer* gMixer = nullptr;

	MIX_Track* gMusicTrack = nullptr;

	MIX_Track** gSoundTracks = nullptr;
	u8 gClosetUsable = 0;

	u8 getAvailableChannel() {
		for (u8 i = 0; i < MAX_SOUNDS;i++) {
			//if (gSoundTracks[i]->state == MIX_STATE_STOPPED)
			//	return i;
		}
		return MAX_SOUNDS;
	}


	[[nodiscard]] COMPONENT_INCLUDE_INIT {
		if (!MIX_Init()) return false;

		gMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (!gMixer) return false;

		gSoundTracks = new MIX_Track * [MAX_SOUNDS];

		for (u8 i = 0; i < MAX_SOUNDS;i++) {
			gSoundTracks[i] = MIX_CreateTrack(gMixer);
			if (!gSoundTracks[i]) [[unlikely]] {
				Log("(Audio Init) Failed to create sound track at index %d!", i);
			}
			
		}
		return (gMixer);
	}

	COMPONENT_INCLUDE_UNLOAD {
		for (u8 i = 0; i < MAX_SOUNDS;i++) {
			MIX_DestroyTrack(gSoundTracks[i]);
		}
		MIX_DestroyTrack(gMusicTrack);
		delete[] gMusicTrack;

		MIX_DestroyMixer(gMixer);
	}

	u8 PlaySound(MIX_Audio* pAudio) {

		u8 a = getAvailableChannel();
		//Ensure we are in a valid track, as it can happen
		if (a >= MAX_SOUNDS) [[unlikely]]
			return MAX_SOUNDS;
		MIX_SetTrackAudio(gSoundTracks[a], pAudio);
		MIX_PlayTrack(gSoundTracks[a], 0);
		return a;
	}

	void StopChannel(u8 pTrack, Sint64 pFadeOutFrames) {
		MIX_StopTrack(gSoundTracks[pTrack], pFadeOutFrames);
		MIX_SetTrackAudio(gSoundTracks[pTrack], NULL);
	}

	void PlayMusic(MIX_Audio* pMusic, bool pLoop) {
		MIX_SetTrackAudio(gMusicTrack, pMusic);
		if(pLoop) MIX_SetTrackLoops(gMusicTrack, -1);
	}

	void StopMusic(Sint64 pFadeOutFrames) {
		MIX_StopTrack(gMusicTrack, pFadeOutFrames);
	}

}