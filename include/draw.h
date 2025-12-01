/** \file draw.h
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

// Draw header file
// it contains functions that draw common shapes, UI elements, etc
#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "primitives.h"

/// Draw a line relative to the position of the ship
void draw_line_relative (double sx, double sy, double sz, double fx, double fy, double fz);

/// Draw text relative to the position of the ship
void draw_text_relative (const char *testo, double x, double y, double z, double sx, double sy);

/// Draw rectangle relative to the position of the ship
void draw_rect_relative (double x, double y, double z, double l, double h);

/// Draw a console on the ship's console (up)
void draw_console_key_up(const char *serigraph, double x);

/// Draw a console on the ship's console (held down)
void draw_console_key_down(const char *serigraph, double x);

/* HUD indicators: */

// main crosshair in the middle of the HUD
void draw_indicator_crosshair();

// "X" crosshair in the direction of motion
void draw_indicator_prograde(int x, int y);

// "#" crosshair in the direction opposite to that of motion
void draw_indicator_retrograde(int x, int y);

// "+" crosshair on the closest pixel
void draw_indicator_closest(int x, int y);



/* ship geometry: */

// strictly necessary cockpit elements
void draw_vehicle_interior();

// the fly as seen from the outside
void draw_vehicle_exterior();

// attitude indicators are part of the vehicle
void draw_vehicle_attitude(i16 alpha, i16 beta, char blink);



/* info panel readings: */

void draw_readings_force(double force);
void draw_readings_speed(double speed);
void draw_readings_docked();
void draw_readings_heading(i16 alpha, i16 beta);
void draw_readings_position(double x, double y, double z);
void draw_readings_closest(double distance);


/// Fottifoh object model
extern const i8 fotty[277];

#endif