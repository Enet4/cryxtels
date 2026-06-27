/** \file fast3d.h
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
// Fast 3D header file.
// It contains some related global variables and the declarations of all functions related to video and drawing.
#ifndef FAST3D_H_INCLUDED
#define FAST3D_H_INCLUDED

#include <SDL3/SDL.h>
#include <memory>
#include "primitives.h"

constexpr double Pi = 3.141592653589793238462643383;

/// The software memory buffer to perform all rendering in.
extern std::unique_ptr<u8[]> video_buffer;

/// Global pointer to the main window
extern SDL_Window * p_window;

/// Trigonometric tables, allocated dynamically and written in runtime.
extern float *tcos, *tsin;
extern float *tcosx, *tsinx;
extern float *tcosy, *tsiny;

/// Camera position
extern double cam_x, cam_y, cam_z;

/// Alpha and Beta (camera orientation)
extern i16 alpha, beta;

/// Distance to the closest pixel.
extern double kk;

/// Some position...
extern double rx, ry, rz;
/// Some other position...
extern double x2, y2, z2;
/// Could be the cryxtel's origin position.
extern double ox, oy, oz;

extern double uneg;

/// Initialize video.
void init_video ();

/// (new) Toggle fullscreen mode
void toggle_fullscreen();

/// Darken the screen once.
void darken_once(u8 inc = 1);

/// Render
void Render (void);

/// Take a snapshot to a BMP
void snapshot (void);

/// Update the pallette with the given saturation of yellow
/// (for proximity to Sunny)
///
/// \param saturation the amount of yellow, between 0 and 255
void update_palette(u8 saturation);

/// Copy a graphical page
// Ultraveloce copia di pagina grafica.
void pcopy (u8 *dest, const u8 *sorg);

/// Clear a graphical page with a pattern
// Ultraveloce cancella pagina grafica.
void pclear (u8 *target, u8 pattern);

/// Initialization of a table of trigonometric calculation results.
// Inizializzazione.
void init_trig ();

// Tracciamento linee 2d.
extern unsigned int ptr;

/// Make one thick plot
void aux_plot(unsigned int x, unsigned int y);

/// Make one thick plot at (share_x,share_y)
void aux_plot(void);

void Segmento (unsigned int x, unsigned int y,
	       unsigned int x2, unsigned int y2);

// Tracciamento linee 3d.
extern char explode;
void Line3D (double p_x, double p_y, double p_z,
	     double x, double y, double z);

// Compute whether a point is visible from the camera.
// If it is, set share_x and share_y to its coordinates on-screen
extern int share_x;
extern int share_y;
int C32 (double x, double y, double z);

/// Draw a line relative to the origin coordinate of a crystal pixel.
/* Traccia una linea relativamente alle coordinate dell'origine di un
   Crystal Pixel. */
void rel (double sx, double sy, double sz,
        double fx, double fy, double fz);

/// Draw a rectangle relative to the origin coordinate of a cryxtel.
/// Width and Height are 2*hx and 2*hz respectively
/* Traccia un rettangolo relativamente alle coordinate dell'origine di un
   Crystal Pixel. Lunghezza e larghezza sono date da 2*hx e 2*hz. */
void rectrel (double cx, double cy, double cz,
        double hx, double hz, char orient);

/// Draw a box relative to the origin coordinate of a cryxtel.
/// The height is 2*hy, the width is 2*hx and the depth is 2*hz.
/* Traccia un parallelepipedo relativamente alle coordinate dell'origine
   di un Crystal Pixel. L'altezza e data da 2*hy, la lunghezza da 2*hx
   e la profondit da 2*hz. */
void boxrel (double cx, double cy, double cz,
        double hx, double hy, double hz);

/// Draw a point. Coordinates are relative to pixel.
// Fa un puntino. Coordinate relative al pixel.
void xrel (double px, double py, double pz);


#endif // FAST3D_H_INCLUDED
