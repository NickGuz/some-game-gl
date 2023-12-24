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

    // TODO never removing from sounds map - I don't think it should ever get that large but
    //      something to keep an eye on
    void play_if_not_already(const char* filename, bool loop = false) {
        if (sounds.find(filename) == sounds.end()) {
            irrklang::ISoundSource *sound = sound_engine->play2D(filename, loop, false, true)->getSoundSource();
            sounds[filename] = sound;
            return;
        }

        irrklang::ISoundSource *sound = sounds[filename];
        if (!sound_engine->isCurrentlyPlaying(sound)) {
            sounds[filename] = sound_engine->play2D(filename, loop, false, true)->getSoundSource();
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

    std::unordered_map<const char*, irrklang::ISoundSource*> sounds;

};
