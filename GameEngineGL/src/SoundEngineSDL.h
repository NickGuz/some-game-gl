#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <thread>

#include "logger.h"

/* #define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__) */

/* bool check_al_errors(const std::string &filename, const std::uint_fast32_t line) { */
/*     ALenum error = alGetError(); */
/*     if (error != AL_NO_ERROR) { */
/*         log_error("***ERROR*** (" + filename + ": " + std::to_string(line) + ")"); */
        
/*         switch(error) */
/*         { */
/*         case AL_INVALID_NAME: */
/*             log_error("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function"); */
/*             break; */
/*         case AL_INVALID_ENUM: */
/*             log_error("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function"); */
/*             break; */
/*         case AL_INVALID_VALUE: */
/*             log_error("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function"); */
/*             break; */
/*         case AL_INVALID_OPERATION: */
/*             log_error("AL_INVALID_OPERATION: the requested operation is not valid"); */
/*             break; */
/*         case AL_OUT_OF_MEMORY: */
/*             log_error("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory"); */
/*             break; */
/*         default: */
/*             log_error("UNKNOWN AL ERROR: " + std::to_string(error)); */
/*         } */
/*         return false; */
/*     } */
/*     return true; */
/* } */

const std::size_t NUM_BUFFERS = 4;
const std::size_t BUFFER_SIZE = 65536; // 32kb of data in each buffer

class SoundEngineSDL
{
public:
	SoundEngineSDL();

	~SoundEngineSDL();

    void set_volume(float volume);

    void play_sound(const char *filename, bool loop = false);

    void play_sound_stream(const char *filename, bool loop = false);
    void _play_sound_stream(const char *filename, bool loop = false);

    void play_if_not_already(const char *filename, bool loop = false);

    void cleanup();

private:
    float volume;

    void update_stream(const ALuint source,
                       const ALenum &format,
                       const std::int32_t &sample_rate,
                       const char *sound_data,
                       const std::size_t sound_data_size,
                       std::size_t &cursor);

    std::int32_t convert_to_int(char *buffer, std::size_t len);
    
    bool load_wav_file_header(std::ifstream &file,
                              std::uint8_t &channels,
                              std::int32_t &sample_rate,
                              std::uint8_t &bits_per_sample,
                              ALsizei &size);

    char *load_wav(const std::string &filename,
                   std::uint8_t &channels,
                   std::int32_t &sample_rate,
                   std::uint8_t &bits_per_sample,
                   ALsizei &size);

    std::unordered_map<std::string, ALuint> sources;
    std::unordered_map<std::string, ALuint> buffers;
    std::vector<std::thread> threads;
};
