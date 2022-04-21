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

#include <SDL2/SDL.h>
constexpr int FRAMES_PER_SECOND = 30;

constexpr unsigned int WIDTH = 320;
constexpr unsigned int HEIGHT = 200;

constexpr unsigned int WIDTH_SC = 2;
constexpr unsigned int HEIGHT_SC = 2;

constexpr unsigned int WIDTH_SCALED = WIDTH*WIDTH_SC;
constexpr unsigned int HEIGHT_SCALED = HEIGHT*HEIGHT_SC;

constexpr auto keymap_thrust    = SDLK_a;
constexpr auto keymap_back      = SDLK_z;
constexpr auto keymap_up        = SDLK_UP;
constexpr auto keymap_down      = SDLK_DOWN;
constexpr auto keymap_left      = SDLK_LEFT;
constexpr auto keymap_right     = SDLK_RIGHT;

#endif // CONF_H_INCLUDED
