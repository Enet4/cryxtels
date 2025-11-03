/** \file transition.h
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

// Transition header file
// This file contains functions that handle state transition

#ifndef TRANSITION_H_INCLUDED
#define TRANSITION_H_INCLUDED

#include "primitives.h"

// enter a synchronous routine to fade out the screen
// (can be skipped by pressing any key)
void fade(u8 speed = 1);

// Terminate some stuff
void alfin (bool arc);

#endif