/** \file input.h
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
#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <array>

extern std::array<unsigned char,2> ctrlkeys;

int attendi_pressione_tasto (); // aspetta un tasto e d qual'.

int tasto_premuto (); // torna 1 se c' un tasto premuto da estrarre.

/* Lettura del mouse e ritorno nelle variabili indicate. */
/// Mouse reading. The function updates these variables.
extern int mdltx, mdlty, mx, my, mpul;
extern const bool grab_mouse;
void mouse_input ();

void keybuffer_cleaner();

void update_ctrlkeys();

/// Save state function.
// Funzioni di continuitá.
void Archivia_Situazione (char i);

/// Find and seek an id text in the open file at fh
char trova_id (const char *id);

/// Read until a specific character code is found
void leggi_t_fino_a (char codcar, int ptyp);

/// Load PIXELS.DEF
void load_pixels_def(int& ptyp);

/// Load the specified pixel type
// Carica il tipo di pixel specificato.
void LoadPtyp (int ptyp);


#endif // INPUT_H_INCLUDED
