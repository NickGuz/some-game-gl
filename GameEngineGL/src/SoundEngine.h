#pragma once

#include <irrklang/irrKlang.h>
#include <unordered_map>
#include <string>

class SoundEngine 
{
public:
	SoundEngine() {
        sound_engine = irrklang::createIrrKlangDevice();
    }

	~SoundEngine() {}

    void set_volume(float volume) {
        sound_engine->setSoundVolume(volume);
    }

    void play_sound(const char* filename, bool loop = false) {
        sound_engine->play2D(filename, loop);
    }

    void play_if_not_already(const char* filename, bool loop = false) {
        if (sounds.find(filename) == sounds.end()) {
            irrklang::ISound *sound = sound_engine->play2D(filename, loop, false, true);
            sounds[filename] = sound;
            return;
        }

        irrklang::ISound *sound = sounds[filename];
        if (!sound_engine->isCurrentlyPlaying(sound->getSoundSource())) {
            sounds[filename] = sound_engine->play2D(filename, loop, false, true);
        }
    }

    irrklang::ISound *play_sound_track(const char* filename, bool loop = false) {
        return sound_engine->play2D(filename, loop, false, true);
    }

    bool is_currently_playing(irrklang::ISound *sound) {
        return sound_engine->isCurrentlyPlaying(sound->getSoundSource());
    }

private:
	irrklang::ISoundEngine *sound_engine;

    std::unordered_map<const char*, irrklang::ISound*> sounds;

};
