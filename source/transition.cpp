/** \file transition.cpp
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

#include <cstdio>
#include "global.h"
#include "fast3d.h"
#include "input.h"
#include "transition.h"

#include "SDL.h"
#include "conf.h"

/// enter a synchronous routine
/// to fade out the screen
/// (can be skipped by pressing any key)
void fade(u8 speed) {
    static const int tickrate = get_config().ticks_per_frame;

    keybuffer_cleaner();
    unsigned int dx = 0;
    auto skip = false;
    do {
        unsigned int sync = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN
                || event.type == SDL_MOUSEBUTTONDOWN) {
                skip = true;
            }
        }

        darken_once(speed);
        Render();
        unsigned long cticks = SDL_GetTicks();
        while (sync + tickrate > cticks) {
            SDL_Delay(3);
            cticks = SDL_GetTicks();
        }
    }
    while(!skip && dx++ < (70 / speed));
/*
rip:    mpul = 0; mouse_input ();
        _BL = tasto_premuto ();
        asm {   les di, dword ptr adaptor
                mov cx, 64000
                xor dx, dx }
__chic: asm {   cmp byte ptr es:[di], 0
                je __zero
                dec byte ptr es:[di]
                inc dx }
__zero: asm {   inc di
                dec cx
                jnz __chic
                cmp bl, 0
                jne halt
                cmp mpul, 0
                jne halt
                cmp dx, 100
                jnb rip }
halt:   keybuffer_cleaner ();
*/
}

void alfin (bool arc) {
        if (arc) fade(5);
        //dsp_driver_off (); // unused right now
        if (recfile) {
                //audio_stop (); // unused right now
                std::fwrite("\0", 1, 1, recfile);
                std::fclose(recfile);
                recfile = nullptr;
        }
        //file_driver_off ();
        ctrlkeys[0] = ctk;
}