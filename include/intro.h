/** \file intro.h
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

// Intro sequence header file.
// It contains functions that should run as the program's intro sequence
#ifndef INTRO_H_INCLUDED
#define INTRO_H_INCLUDED

/**
 * Setup code for the intro. To be called once before intro_loop.
 */
void init_intro(void);

/**
 * The game introduction loop logic, called once per frame.
 * 
 * @return true if the introduction should continue normally,
 * false to end the introduction
 */
bool intro_loop(void);

#endif