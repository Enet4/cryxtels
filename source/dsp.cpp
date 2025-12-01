/** \file dsp.cpp
 *  This file is part of Crystal Pixels.
 *
 *  Crystal Pixels is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Crystal Pixels is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dsp.h"
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstring>

/** Global variable for whether audio is enabled. */
bool audioEnabled = true;

/** The total number of recognized audios,
 * and the maximum number of audio files that can be loaded simultaneously.
 */
constexpr unsigned int TOTAL_AUDIOS = 14;

/** The total number of SDL2 mixer channels (or tracks),
 * which define how many sounds can be played simultaneously.
 * 
 * As each channel can only play one sound at a time,
 * the following guidelines are set:
 * - track 0 for simpler sound effects
 * - track 1 for more complex sound effects of greater precedence
 * - track 2 for the music and the echo sonar
 */
constexpr unsigned int TOTAL_NUM_TRACKS = 3;

/** The SDL audio device ID. If 0, audio is not set */
SDL_AudioDeviceID deviceId = 0;

const char* AUDIO_FILENAMES[TOTAL_AUDIOS] = {
    // SOTTOFONDO is actually a placeholder
    // for a dynamically loaded audio
    nullptr,
    "TARGET.VOC",
    "ATTRACCO.VOC",
    "DISTACCO.VOC",
    "PRENDERE.VOC",
    "LASCIARE.VOC",
    "FID.VOC",
    "PASSO.VOC",
    "FLY_ON.VOC",
    "FLY_OFF.VOC",
    "TASTO.VOC",
    "READY.VOC",
    "DESTROY.VOC",
    "BOM.VOC",
};

Mix_Chunk * AUDIO_HANDLES[TOTAL_AUDIOS] = {
    // SOTTOFONDO
    0,
    // TARGET
    0,
    // ATTRACCO
    0,
    // DISTACCO
    0,
    // PRENDERE
    0,
    // LASCIARE
    0,
    // FID
    0,
    // PASSO
    0,
    // FLY_ON
    0,
    // FLY_OFF
    0,
    // TASTO
    0,
    // READY
    0,
    // DESTROY
    0,
    // BOM
    0,
};

/// The currently selected music file name
std::string MUSIC_SELECTED = "";

// the audio last played by each channel/trick
// (or -1 if none)
Audio last_played[TOTAL_NUM_TRACKS] = {
    -1,
    -1,
    -1,
};

SDL_AudioSpec wavSpec;

void init_audio() {
    deviceId = Mix_OpenAudioDevice(
        48000, // frequency
        AUDIO_S16SYS, // format
        2, // channels
        2048, // chunksize
        nullptr, // device
        0 // allowed_changes
    );
    if (deviceId != 0) {
        std::cerr << "Failed to open audio device: " << Mix_GetError() << std::endl;
        audioEnabled = false;
    } else {
        std::cout << "Audio device " << deviceId << " selected" << std::endl;
        SDL_PauseAudioDevice(deviceId, 0); // Start audio playback
    }

    // set three channels / tracks
    Mix_AllocateChannels(TOTAL_NUM_TRACKS);
}

bool set_sottofondo(const char* filename) {
    if (!audioEnabled) {
        return false;
    }

    // skip if already set (or both are null)
    if (
        (filename == nullptr && AUDIO_FILENAMES[0] == nullptr) ||
        (filename != nullptr && AUDIO_FILENAMES[0] != nullptr && strcasecmp(AUDIO_FILENAMES[0], filename) == 0)
    ) {
        return true;
    }

    // free the previous chunk
    if (AUDIO_HANDLES[0]) {
        // stop playing audio
        // to prevent use after free
        // (assuming track 2 because it's the one used by SOTTOFONDO)
        audio_stop(2);

        Mix_FreeChunk(AUDIO_HANDLES[0]);
        AUDIO_HANDLES[0] = nullptr;
    }

    if (filename == nullptr) {

        AUDIO_FILENAMES[0] = nullptr;
        return true;
    }

    AUDIO_FILENAMES[0] = filename;
    std::cout << "Sottofondo: file '" << filename << "' selected." << std::endl;

    // try to pre-load the audio
    auto chunk = AUDIO_HANDLES[0] = Mix_LoadWAV(filename);
    if (!chunk) {
        std::cerr << "Failed to load audio file '" << filename << "': " << Mix_GetError() << std::endl;
        AUDIO_FILENAMES[0] = nullptr;
        return false;
    }
    std::cout << "Audio file '" << filename << "' loaded." << std::endl;

    return true;
}

/** try to play an audio */
void play(Audio audio, int channel_num, int loops) {
    if (!audioEnabled) {
        return;
    }
    SDL_assert(audio < TOTAL_AUDIOS);

    // load file if necessary
    auto chunk = AUDIO_HANDLES[audio];

    auto filename = AUDIO_FILENAMES[audio];

    if (!filename) {
        // filename not set up, ignoring
        std::cerr << "Audio #" << audio << " was not set up" << std::endl;
        return;
    }

    if (!chunk) {
        chunk = AUDIO_HANDLES[audio] = Mix_LoadWAV(filename);
        // if it still couldn't load, give up
        if (!chunk) {
            std::cerr << "Failed to load audio file '" << filename << "': " << Mix_GetError() << std::endl;
            return;
        }
        std::cout << "Audio file '" << filename << "' loaded." << std::endl;
    }

    // if this was the audio last played in this channel
    if (last_played[channel_num] == audio) {
        // check whether the channel is still playing
        if (Mix_Playing(channel_num)) {
            // still playing, do not proceed
            return;
        }
    }

    auto err = Mix_PlayChannel(channel_num, chunk, loops);
    if (err != -1) {
        last_played[err] = audio;
    }
}

std::string find_music(const char* name) {
    // see if the name as it is exists in the file system
    std::string fname = name;
    FILE* f = fopen(fname.c_str(), "rb");
    if (f) {
        fclose(f);
        return fname;
    }
    // try again with .VOC extension
    fname += ".VOC";
    f = fopen(fname.c_str(), "rb");
    if (f) {
        fclose(f);
        return fname;
    }
    // try again with .OGG extension
    fname.erase(fname.size() - 4);
    fname += ".OGG";
    f = fopen(fname.c_str(), "rb");
    if (f) {
        fclose(f);
        return fname;
    }
    // try again with .MP3 extension
    fname.erase(fname.size() - 4);
    fname += ".MP3";
    f = fopen(fname.c_str(), "rb");
    if (f) {
        fclose(f);
        return fname;
    }
    // try again with .WAV extension
    fname.erase(fname.size() - 4);
    fname += ".WAV";
    f = fopen(fname.c_str(), "rb");
    if (f) {
        fclose(f);
        return fname;
    }
    return "";
}

bool play_music(const char* name, bool repeat) {
    if (!audioEnabled || name == nullptr) {
        return false;
    }

    // check whether the requested music is already playing
    if (MUSIC_SELECTED == name && Mix_Playing(2)) {
        // already playing, do nothing
        return true;
    }

    // clear previous music if any
    // (we need this because AUDIO_FILENAME[0] might be using the same buffer)
    if (MUSIC_SELECTED != "" && MUSIC_SELECTED != name) {
        Mix_FreeChunk(AUDIO_HANDLES[0]);
        AUDIO_HANDLES[0] = nullptr;
        AUDIO_FILENAMES[0] = nullptr;
        MUSIC_SELECTED = "";
    }

    std::string fname = find_music(name);
    if (fname.empty()) {
        std::cerr << "Music file '" << name << "' not found" << std::endl;
        return false;
    }

    set_sottofondo(fname.c_str());
    MUSIC_SELECTED = name;
    play(0, 2, repeat ? -1 : 0);
    return true;
}

const char* last_music_playing() {
    return MUSIC_SELECTED == "" ? nullptr : MUSIC_SELECTED.c_str();
}

void audio_stop(int channel_num, int fadeout) {
    if (!audioEnabled) {
        return;
    }
    if (channel_num == -1) {
        if (fadeout > 0) {
            Mix_FadeOutChannel(-1, fadeout);
        } else {
            Mix_HaltChannel(-1);
        }
        // reset all last played trackers
        for (unsigned int i = 0; i < TOTAL_NUM_TRACKS; ++i) {
            last_played[i] = -1;
        }

        // also music
        MUSIC_SELECTED = "";

    } else {
        if (fadeout > 0) {
            Mix_FadeOutChannel(channel_num, fadeout);
        } else {
            Mix_HaltChannel(channel_num);
        }
        last_played[channel_num] = -1;

        if (channel_num == 2) {
            MUSIC_SELECTED = "";
        }
    }
}
