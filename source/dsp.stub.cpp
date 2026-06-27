/** \file dsp.stub.cpp
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

// implements public functions in header as no-ops

/** Global variable for whether audio is enabled. */
bool audioEnabled = false;

void init_audio() {}

void destroy_audio() {}

bool set_sottofondo(const char*) {
    return true;
}

void play(Audio, int, int) {}


bool play_music(const char*, bool) {
    return true;
}

const char* last_music_playing() {
    return nullptr;
}

void audio_stop(int, int) {}
