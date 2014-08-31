/** \file text3d.cpp
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
#include "text3d.h"

#include "fast3d.h"
#include <math.h>

char blank = 4; // Larghezza carattere spazio.
char mediumwidth = 4; // Larghezza media dei caratteri.

char symbol[65][41] = {
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 33:
3,  0, -2,  0,  1, -1,  1,  1,  2,  1,  1, -1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1, -2, -1, -1,  1, -2,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -1, -2, -1,  2,  1, -2,  1,  2, -2,  1,  2,  1, -2, -1,  2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
8,  1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -2,  0, -2,  0,  1,  0,  1,  0,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2,  0, -3,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,
7, -2, -1, -1, -1, -1, -1, -1, -2, -1, -2, -2, -1, -1,  2,  1, -2,  1,  2,  2,  1,  2,  1,  2,  2,  2,  2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
6,  2,  0,  0,  2,  0,  2, -1,  1, -1,  1,  1, -1,  1, -1,  0, -2,  0, -2, -1, -1, -1, -1,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
1,  0, -2, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4,  1, -3,  0, -2,  0, -2, -1,  0, -1,  0,  0,  2,  0,  2,  1,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -1, -3,  0, -2,  0, -2,  1,  0,  1,  0,  0,  2,  0,  2, -1,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -3,  0,  3,  0,  0, -3,  0,  3, -2, -2,  2,  2,  2, -2, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1,  0,  1,  0,  0, -1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
1, -1,  1, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
1, -1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -1,  2,  0,  2,  0,  2,  0,  1,  0,  1, -1,  1, -1,  1, -1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
1,  1, -2, -1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 48:
9, -1, -2,  1, -2,  1, -2,  2, -1,  2, -1,  2,  1,  2,  1,  1,  2,  1,  2, -1,  2, -1,  2, -2,  1, -2,  1, -2, -1, -2, -1, -1, -2,  0, -1,  0,  1,  0,  0,  0,  0,
2, -1, -1,  0, -2,  0, -2,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
5, -2, -1, -1, -2, -1, -2,  0, -2,  0, -2,  1, -1,  1, -1, -2,  2, -2,  2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
6, -2, -2,  0, -2,  0, -2,  1, -1,  1, -1, -1,  0, -1,  0,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3,  0,  2,  0, -2,  0, -2, -2,  1, -2,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
5,  1, -2, -2, -2, -2, -2, -2,  0, -2,  0,  1,  0,  1,  0,  1,  2,  1,  2, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
8,  1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -2,  1, -2,  1, -1,  2, -1,  2,  0,  2,  0,  2,  1,  1,  1,  1,  0,  0,  0,  0, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -2, -2,  1, -2,  1, -2,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
10,-1, -2,  1, -2,  1, -2,  2, -1,  2, -1,  1,  0,  1,  0,  2,  1,  2,  1,  1,  2,  1,  2, -1,  2, -1,  2, -2,  1, -2,  1, -1,  0, -1,  0, -2, -1, -2, -1, -1, -2,
8, -2, -1, -1, -2, -1, -2,  0, -2,  0, -2,  1, -1,  1, -1,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2, -2, -1, -1,  0, -1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
6,  0,  0,  0, -1,  0, -1, -1,  0, -1,  0,  0,  0,  0,  2, -1,  2, -1,  2,  0,  1,  0,  1,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
6,  0,  0,  1,  0,  1,  0, -1,  2, -1,  2,  0,  0,  0, -2,  1, -2,  1, -2,  0, -1,  0, -1,  0, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1,  0,  1, -1, -1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1, -1,  1, -1, -1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1, -1,  1,  0, -1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
7, -2, -2, -1, -3, -1, -3,  0, -3,  0, -3,  1, -2,  1, -2,  1, -1,  1, -1,  0, -1,  0, -1,  0,  0,  0,  1,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
10, 0,  1,  1,  0,  1,  0,  0, -1,  0, -1, -1,  0, -1,  0,  0,  1,  0,  1,  2,  0,  2,  0,  1, -2,  1, -2, -1, -2, -1, -2, -2,  0, -2,  0, -1,  2, -1,  2,  1,  2,
// 65:
3, -2,  2,  0, -2,  0, -2,  2,  2, -1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
8, -2, -2,  0, -2,  0, -2,  1, -1,  1, -1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2, -2,  2, -2, -2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
5,  1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -2,  1, -2,  1, -1,  2, -1,  2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
6, -2, -2,  0, -2,  0, -2,  1, -1,  1, -1,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2, -2,  2, -2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4,  1, -2, -2, -2, -2, -2, -2,  2, -2,  2,  1,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2, -2,  1, -2, -2, -2, -2,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
7,  1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -2,  1, -2,  1, -1,  2, -1,  2,  1,  2,  1,  2,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2, -2, -2,  2,  1, -2,  1,  2, -2,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -1, -2,  1, -2,  0, -2,  0,  2, -1,  2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
5, -2,  1, -1,  2, -1,  2,  0,  2,  0,  2,  1,  1,  1,  1,  1, -2,  0, -2,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2, -2, -2,  2, -2,  0,  1, -2, -2,  0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2, -2, -1, -2, -1, -2, -1,  2, -2,  2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -2,  2, -2, -2, -2, -2,  0,  0,  0,  0,  2, -2,  2, -2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2,  2, -2, -2, -2, -2,  2,  2,  2,  2,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
8, -1, -2,  1, -2,  1, -2,  2, -1,  2, -1,  2,  1,  2,  1,  1,  2,  1,  2, -1,  2, -1,  2, -2,  1, -2,  1, -2, -1, -2, -1, -1, -2,  0,  0,  0,  0,  0,  0,  0,  0,
5, -2, -2,  0, -2,  0, -2,  1, -1,  1, -1,  1,  0,  1,  0, -2,  0, -2,  2, -2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
10,-1, -2,  1, -2,  1, -2,  2, -1,  2, -1,  2,  1,  2,  1,  1,  2,  1,  2, -1,  2, -1,  2, -2,  1, -2,  1, -2, -1, -2, -1, -1, -2,  1,  1,  2,  2,  2,  2,  3,  2,
6, -2, -2,  0, -2,  0, -2,  1, -1,  1, -1,  1,  0,  1,  0, -2,  0, -2,  2, -2, -2, -1,  0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
7,  1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -2,  0, -2,  0,  1,  0,  1,  0,  1,  1,  1,  1,  0,  2,  0,  2, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -2, -2,  2, -2,  0, -2,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -2, -2, -2,  1, -2,  1, -1,  2, -1,  2,  1,  2,  1,  2,  1, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -2, -2,  0,  2,  0,  2,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
4, -2, -2, -1,  2, -1,  2,  1, -1, -1, -1,  1,  2,  1,  2,  2, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -2, -2,  2,  2,  2, -2, -2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -2, -2,  0,  0,  1, -2, -1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3, -2, -2,  2, -2,  2, -2, -2,  2, -2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3,  0, -2, -1, -2, -1, -2, -1,  2, -1,  2,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 92:
1, -1, -2,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
3,  0, -2,  1, -2,  1, -2,  1,  2,  1,  2,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
2, -1, -1,  0, -2,  0, -2,  1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
1, -2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 96: (accento = zerino)
4, -1, -2,  1, -1,  1, -1,  0, -1,  0, -1, -1, -1, -1, -1, -1, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };

void Txt (const char * testo, double x, double y, double z,
	  double scala_x, double scala_y, int alfa, int beta)
{
	double fxx=0, fyy=0, pxx=0, pyy=0;
	double lsx=0, lsy=0, lsz=0;
	double sx=0, sy=0, sz=0;

	int t; // char

	int f;
	int l;

	x += ox;
	y += oy;
	z += oz;

	int c = 0;
	int ttl = 0;
	while (testo[c]) c++;
	ttl = c;

	sx = x-cam_x; sy = y-cam_y; sz = z-cam_z;

	double icoeff = sqrt (sx*sx+sy*sy+sz*sz);
	double scala_d = scala_x * mediumwidth;
	int incr = icoeff / (50*scala_d); if (incr<1) incr = 1;
	double avan_x = scala_d * tcos[beta] * (double)incr;
	double avan_z = scala_d * tsin[beta] * (double)incr;

	scala_x *= 0.8;
	scala_y *= 0.8;

	c = 0;
	while (c<ttl) {
		t = testo[c] - 32;
		pxx = 1000;
		f = 1;
		for (l=0; l<symbol[t][0]; l++) {
			fxx = symbol[t][f+0]*scala_x;
			fyy = symbol[t][f+1]*scala_y;
			if (fxx==pxx&&fyy==pyy) {
				sx = lsx;
				sy = lsy;
				sz = lsz;
			}
			else {
				z2 = fyy * tsin[alfa];
				sy = fyy * tcos[alfa] + y;
				sz = z2  * tcos[beta] - fxx * tsin[beta] + z;
				sx = fxx * tcos[beta] + z2  * tsin[beta] + x;
			}
			pxx = symbol[t][f+2]*scala_x;
			pyy = symbol[t][f+3]*scala_y;
			z2  = pyy * tsin[alfa];
			lsy = pyy * tcos[alfa] + y;
			lsz = z2  * tcos[beta] - pxx * tsin[beta] + z;
			lsx = pxx * tcos[beta] + z2  * tsin[beta] + x;
			Line3D (sx, sy, sz, lsx, lsy, lsz);
			f += 4;
		}
		x += avan_x;
		z += avan_z;
		c += incr;
	}
}

//Instantiate them
/*
template void Txt<false> (const char * text, double x, double y, double z,
	  double scale_x, double scale_y, int alpha, int beta);
template void Txt<true> (const char * text, double x, double y, double z,
	  double scale_x, double scale_y, int alpha, int beta);
*/
