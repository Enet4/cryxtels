/** \file text3d.h
 * Inclusion of text for 3D rendering.
 *
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
#ifndef TEXT3D_H_INCLUDED
#define TEXT3D_H_INCLUDED

// Font incluso.
// Lettere da A a Z uppercase.
// Numeri da 0 a 9.
// Simboli speciali: . , ? ! ' : ; + - * < = > / \ ( ) " # $ ^ _ % &

extern char blank; // Larghezza carattere spazio.
extern char mediumwidth; // Larghezza media dei caratteri.

/// Draw text
void Txt (const char * text, double x, double y, double z,
	  double scale_x, double scale_y, int alpha, int beta);

#endif
