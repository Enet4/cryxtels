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

#include "SDL.h"
#include <memory>

#ifndef far
#define far
#endif

constexpr double Pi = 3.141592653589793238462643383;

/// The software membory buffer to perform all rendering in.
extern std::unique_ptr<unsigned char[]> video_buffer;

///// The SDL surface to show a scaled result.
//extern SDL_Surface * p_surface_scaled;

/// Some old adapters.
//unsigned char far * adaptor = (unsigned char far *) 0xA0000000;
//unsigned char far * adapted = (unsigned char far *) 0x80000000;
//unsigned char far * fake_adaptor = (unsigned char far *) 0x80000000;

/// This is the main palette.
//extern unsigned char tmppal[768];

/// Trigonometric tables, allocated dynamically and written in runtime.
extern float *tcos, *tsin;
extern float *tcosx, *tsinx;
extern float *tcosy, *tsiny;

/// Camera position
extern double cam_x, cam_y, cam_z;

/// Alpha and Beta (camera orientation)
extern short int alfa,beta;

/// Something...
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

/// Do nothing here, it's no longer needed
inline void _80_25_C () {} // modo grafico 80x25 testo a colori.

/// Darken the screen once.
void darken_once(unsigned char inc = 1);

/// Render
void Render (void);

/// Take a snapshot to a BMP
void snapshot (void);

/// Create a palette table based on new_palette.
void tavola_colori (const unsigned char *new_palette,
		    unsigned int starting_color, unsigned int nr_colors,
		    char red_filter, char green_filter, char blue_filter);
/* Original prototype:
void tavola_colori (unsigned char *nuova_tavolozza,
		    unsigned colore_di_partenza, unsigned nr_colori,
		    char filtro_rosso, char filtro_verde, char filtro_blu);
*/

/// Copy a graphical page
// Ultraveloce copia di pagina grafica.
void pcopy (unsigned char *dest, const unsigned char *sorg);

/// Clear a graphical page with a pattern
// Ultraveloce cancella pagina grafica.
void pclear (unsigned char far *target, unsigned char pattern);

/// Initialization of a table of trigonometric calculation results.
// Inizializzazione.
void init ();

// Tracciamento linee 2d.
extern unsigned int ptr;
extern unsigned int global_x, global_y;

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

// Conversioni x, y, z -> share_x, share_y
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
