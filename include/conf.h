/** \file conf.h
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
// Additional global configuration parameters
// These did not exist in the original Cryxtels
#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED

#include <string>

#include "SDL.h"
#include "primitives.h"

#define CONFIG_FILE_NAME "SETTINGS.INI"

typedef struct conf {
    u32 framerate_intro;
    u32 framerate;
    int ticks_per_second;
    int ticks_per_frame;    // == ticks_per_second / framerate
    int ticks_per_frame_intro; // == ticks_per_second / framerate_intro

    u32 render_width;
    u32 render_height;
    u32 window_scale;
    u32 window_width; // == render_width * window_scale
    u32 window_height; // == render_height * window_scale

    std::string definition_file; // path to .def file

    // the following can be overridden by command line arguments
    u32 cosm_pixels;
    u32 cosm_objects;
    std::string situation_file;
    // TODO: auto-save functionality?
} Config;

// Returns the config object, loading it from disk on the first call
const Config& get_config(); 

constexpr auto keymap_thrust    = SDLK_a;
constexpr auto keymap_back      = SDLK_z;
constexpr auto keymap_up        = SDLK_UP;
constexpr auto keymap_down      = SDLK_DOWN;
constexpr auto keymap_left      = SDLK_LEFT;
constexpr auto keymap_right     = SDLK_RIGHT;

/// The maximum number of pixels allowed ingame.
/// This was hardcoded to 500 in the original version.
constexpr u32 MAX_PIXELS = 500;

/// The maximum number of objects allowed ingame.
/// This was hardcoded to 500 in the original version.
constexpr u32 MAX_OBJECTS = 500;

/// The maximum number of pixel types allowed ingame.
/// This was hardcoded to 600 in the original version.
constexpr u32 MAX_PIXEL_TYPES = 650;

#endif // CONF_H_INCLUDED
