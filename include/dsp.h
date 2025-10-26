/** \file dsp.h
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
#ifndef DSP_H_INCLUDED
#define DSP_H_INCLUDED

#include "primitives.h"

/** A number identifying a recognized audio.
 * See the global definitions in global.h for the constant definitions
 */
using Audio = i16;

extern bool audioEnabled;

/** initialize the audio device */
void init_audio();

/** Set the audio file for the depth sounder (sottofondo).
 *
 * Then you can play this file with `SOTTOFONDO` as the audio identifier.
 * Pass `nulltpr` to reset the depth sounder audio.
 *
 * \return 1 if audio is disabled or the file loads successfully, 0 if it fails
 */
bool set_sottofondo(const char* filename);

/** try to play an audio.
 * Consider using a different channel for music or longer sounds.
 */
void play(Audio audio, int channel = 0, int loops = 0);

/**
 * Play an audio file as music.
 * 
 * If the given file name has no extension,
 * it will be inferred as either .VOC, .OGG, .MP3, or .WAV.
 */
bool play_music(const char* name, bool repeat);

/** Get the name of the music audio which was last playing. */
const char* last_music_playing();

/** Stop playing audio.
 *
 * Stopping channel 2 will also stop any music being played.
 * If `channel_num` is -1 (default), stops all audio channels.
 * If `fadeout` > 0, the audio will fade out
 * with the given number of milliseconds.
 */
void audio_stop(int channel = -1, int fadeout = 0);

#endif
