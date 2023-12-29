#include "SoundEngineSDL.h"
#include <cstring>

SoundEngineSDL::SoundEngineSDL() {
    // Initialize OpenAL context and device
    ALCdevice *device = alcOpenDevice(nullptr);
    if (!device) {
        log_error("Failed to open OpenAL device");
        return;
    }

    ALCcontext *context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);

    volume = 1.0f;
}

SoundEngineSDL::~SoundEngineSDL() {
    this->cleanup();

    for (std::thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void SoundEngineSDL::set_volume(float volume) {
    // set global volume
    this->volume = volume;

    // set volume for existing sources
    for (auto &it : sources) {
        ALuint source = it.second;
        alSourcef(source, AL_GAIN, volume);
    }
}

// TODO possibly keep map of SDL_AudioSpecs?
void SoundEngineSDL::play_sound(const char* filename, bool loop) {
    // Generate an OpenAL buffer and source
    ALuint buffer, source;
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    // Read audio data from file and fill the buffer
    std::uint8_t channels;
    std::int32_t sample_rate;
    std::uint8_t bits_per_sample;
    ALsizei size;
    char *sound_data;
    sound_data = load_wav(filename, channels, sample_rate, bits_per_sample, size);

    ALenum format;
    if (channels == 1 && bits_per_sample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bits_per_sample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bits_per_sample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bits_per_sample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        log_error("Unrecognized wave format");
        return;
    }

    alBufferData(buffer, format, sound_data, size, sample_rate); // Adjust the format and frequency as needed
    /* free(sound_data); */

    // Attach the buffer to the source
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, this->volume);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buffer);

    // Play the audio
    alSourcePlay(source);

    sources[filename] = source;
    buffers[filename] = buffer;
}

void SoundEngineSDL::play_sound_stream(const char *filename, bool loop) {
    std::thread t([this, filename, loop](){ this->_play_sound_stream(filename, loop); });
    /* std::thread t(&SoundEngineSDL::_play_sound_stream, filename, loop); */
    t.detach();
    threads.push_back(std::move(t));
    /* t.join(); */
}

void SoundEngineSDL::_play_sound_stream(const char *filename, bool loop) {
    // Read audio data from file and fill the buffer
    std::uint8_t channels;
    std::int32_t sample_rate;
    std::uint8_t bits_per_sample;
    /* std::vector<char> sound_data; */
    ALsizei size;
    char *sound_data;
    sound_data = load_wav(filename, channels, sample_rate, bits_per_sample, size);

    ALuint buffers[NUM_BUFFERS];
    alGenBuffers(NUM_BUFFERS, &buffers[0]);

    ALenum format;
    if (channels == 1 && bits_per_sample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bits_per_sample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bits_per_sample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bits_per_sample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        log_error("Unrecognized wave format");
        return;
    }

    for (std::size_t i = 0; i < NUM_BUFFERS; i++) {
        alBufferData(buffers[i], format, &sound_data[i * BUFFER_SIZE], BUFFER_SIZE, sample_rate);
    }

    // Attach the buffer to the source
    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, this->volume);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);

    // Play the audio
    alSourcePlay(source);

    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);

    std::size_t cursor = BUFFER_SIZE * NUM_BUFFERS;

    while (source_state == AL_PLAYING) {
        update_stream(source, format, sample_rate, sound_data, size, cursor);
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    }

    // cleanup
    alDeleteSources(1, &source);
    alDeleteBuffers(NUM_BUFFERS, &buffers[0]);

    /* sources[filename] = source; */
    /* buffers[filename] = buffer; */
}

void SoundEngineSDL::cleanup() {
    for (auto it : sources) {
        ALuint source = it.second;
        alDeleteSources(1, &source);
    }

    for (auto it : buffers) {
        ALuint buffer = it.second;
        alDeleteBuffers(1, &buffer);
    }
}

void SoundEngineSDL::play_if_not_already(const char* filename, bool loop) {
    if (sources.find(filename) != sources.end()) {
        ALint source_state;
        alGetSourcei(sources[filename], AL_SOURCE_STATE, &source_state);
        if (source_state != AL_PLAYING) {
            alSourcePlay(sources[filename]);
        }
        return;
    }

    play_sound(filename, loop);
}

void SoundEngineSDL::update_stream(const ALuint source,
                   const ALenum &format,
                   const std::int32_t &sample_rate,
                   const char *sound_data,
                   const std::size_t sound_data_size,
                   std::size_t &cursor) {
    ALint buffers_processed = 0;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffers_processed);

    if (buffers_processed <= 0) {
        return;
    }

    while (buffers_processed--) {
        ALuint buffer;
        alSourceUnqueueBuffers(source, 1, &buffer);

        ALsizei data_size = BUFFER_SIZE;
        
        char *data = new char[data_size];
        std::memset(data, 0, data_size);  // can't we just calloc instead?

        std::size_t data_size_to_copy = BUFFER_SIZE;
        if (cursor + BUFFER_SIZE > sound_data_size) {
            data_size_to_copy = sound_data_size - cursor;
        }

        std::memcpy(&data[0], &sound_data[cursor], data_size_to_copy);
        cursor += data_size_to_copy;

        if (data_size_to_copy < BUFFER_SIZE) {
            cursor = 0;
            std::memcpy(&data[data_size_to_copy], &sound_data[cursor], BUFFER_SIZE - data_size_to_copy);
            cursor = BUFFER_SIZE - data_size_to_copy;
        }

        alBufferData(buffer, format, data, BUFFER_SIZE, sample_rate);
        alSourceQueueBuffers(source, 1, &buffer);

        delete[] data;
    }
}

std::int32_t SoundEngineSDL::convert_to_int(char *buffer, std::size_t len) {
    std::int32_t a = 0;
    if (std::endian::native == std::endian::little) {
        std::memcpy(&a, buffer, len);
    } else {
        for (std::size_t i = 0; i < len; ++i) {
            reinterpret_cast<char *>(&a)[3 - i] = buffer[i];
        }
    }
    return a;
}

bool SoundEngineSDL::load_wav_file_header(std::ifstream &file,
                                          std::uint8_t &channels,
                                          std::int32_t &sample_rate,
                                          std::uint8_t &bits_per_sample,
                                          ALsizei &size) {
    char buffer[4];

    if (!file.is_open()) {
        return false;
    }

    // the RIFF
    if (!file.read(buffer, 4)) {
        log_error("Could not read RIFF");
        return false;
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0) {
        log_error("File is not a valid WAVE file (header doesn't begin with RIFF)");
        return false;
    }

    // the size of the file
    if (!file.read(buffer, 4)) {
        log_error("Could not read size of file");
        return false;
    }

    // the WAVE
    if (!file.read(buffer, 4)) {
        log_error("Could not read WAVE");
        return false;
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0) {
        log_error("File is not a valid WAVE file (header doesn't contain WAVE)");
        return false;
    }

    // "fmt/0"
    if (!file.read(buffer, 4)) {
        log_error("Could not read fmt/0");
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4)) {
        log_error("Could not read the 16");
        return false;
    }

    // PCM should be 1?
    if (!file.read(buffer, 2)) {
        log_error("Could not read PCM");
        return false;
    }

    // the number of channels
    if (!file.read(buffer, 2)) {
        log_error("Could not read number of channels");
        return false;
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4)) {
        log_error("Could not read sample rate");
        return false;
    }
    sample_rate = convert_to_int(buffer, 4);

    // byterate : (sample_rate * bits_per_sample * channels) / 8
    if (!file.read(buffer, 4)) {
        log_error("Could not read byterate : (sample_rate * bits_per_sample * channels) / 8");
        return false;
    }

    // blockalign
    if (!file.read(buffer, 2)) {
        log_error("Could not read blockalign");
        return false;
    }

    // bits_per_sample
    if (!file.read(buffer, 2)) {
        log_error("Could not read bits per sample");
        return false;
    }
    bits_per_sample = convert_to_int(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4)) {
        log_error("Could not read data chunk header");
        return false;
    }
    if (std::strncmp(buffer, "data", 4) != 0) {
        log_error("File is not a valid WAVE file (doesn't have 'data' tag)");
        return false;
    }

    // size of data
    if (!file.read(buffer, 4)) {
        log_error("Could not read data size");
        return false;
    }
    size = convert_to_int(buffer, 4);

    // cannot be at end of file
    if (file.eof()) {
        log_error("Reached EOF on the file");
        return false;
    }
    if (file.fail()) {
        log_error("Fail state set on the file");
        return false;
    }

    return true;
}

char *SoundEngineSDL::load_wav(const std::string &filename,
                               std::uint8_t &channels,
                               std::int32_t &sample_rate,
                               std::uint8_t &bits_per_sample,
                               ALsizei &size) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        log_error("Could not open \"" + filename + "\"");
        return nullptr;
    }

    if (!load_wav_file_header(in, channels, sample_rate, bits_per_sample, size)) {
        log_error("Could not load wav header or \"" + filename + "\"");
        return nullptr;
    }

    char *data = new char[size];
    in.read(data, size);
    return data;
}
