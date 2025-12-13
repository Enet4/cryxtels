/** \file intro.cpp
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

#include "global.h"
#include "primitives.h"
#include "fast3d.h"
#include "text3d.h"
#include "transition.h"
#include "intro.h"
#include "dsp.h"

#include "SDL.h"
#include "conf.h"

static u32 tickrate;
static u32 width, height;
static u32 framebuffer_size;
static bool original_resolution;
static u16 fotty_viewport_upper;
static u16 fotty_viewport_lower;

extern void Object (int type);

void init_intro(void) {
    static const Config& config = get_config();
    
    tickrate = config.ticks_per_frame_intro;
    width = config.render_width;
    height = config.render_height;
    framebuffer_size = width * height;
    
    if (width == 320 && height == 200) {
        fotty_viewport_upper = 65;
        fotty_viewport_lower = 135;
        original_resolution = true;
    } else {
        fotty_viewport_upper = height * 65 / 200;
        fotty_viewport_lower = height * 135 / 200;
        original_resolution = false;
    }
}

void place_fottifoh(void) {
    c = (c+1)%360;
    i16 tmp = beta; beta = c;
    cam_y += 140;
    Object(0);
    cam_y -= 140;
    beta = tmp;
}

void original_twinkle_effect(void) {
    u16 time = (SDL_GetTicks()/tickrate) & 0xFFFF;
    u16 iteration = width*50;
    u16 halfway = iteration/2;
    do {
        // pixel value outside fottifoh row range
        if (time < width * fotty_viewport_upper + 4 ||
            time >= width * fotty_viewport_lower + 4)
        {
            if (time < framebuffer_size)
                video_buffer[time] >>= 1;
        }
        if (iteration >= halfway) {
            time += width + 1;
        } else {
            time += width - 1;
        }
    } while (--iteration > 0);
}

void twinkle_effect(void) {
    // adapt the effect's intensity to the new resolution
    float scale_factor = width / 320.0;
    u32 intensity = width * 50.0 * scale_factor;
    u32 halfway = intensity / 2;
    u32 canvas_size = framebuffer_size * 1.024;

    u32 time = (SDL_GetTicks()/tickrate) % canvas_size;
    u32 iteration = intensity;
    do {
        // pixel value outside fottifoh row range
        if (time < width * fotty_viewport_upper + 4 ||
            time >= width * fotty_viewport_lower + 4)
        {
            if (time < framebuffer_size)
                video_buffer[time] >>= 1;
        }
        if (iteration >= halfway) {
            time = (time + width + 1) % canvas_size;
        } else {
            time = (time + width - 1) % canvas_size;
        }
    } while (--iteration > 0);
}

void fade_out_effect(void) {
    auto di = &video_buffer[0];
    u32 iteration = framebuffer_size; // all space
    do {
        // fade out effect
        if (*di > 0) {
            *di -= 1;
        }
        di++;
    } while (--iteration > 0);
}

bool intro_loop(void) {
    // keep playing intro audio while in the loop
    play(SOTTOFONDO, 2);
    u32 sync = SDL_GetTicks(); //clock();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_RESIZED:
            break;
            case SDL_QUIT:
                alfin(true);
                exit(0);
            break;
            case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONDOWN:
                return false; // end intro
            default:
                break;
        }
    }

    Txt("CRYSTAL PIXELS", -77, 0, 100, 3, 4, 270, 0);
    Txt("WRITTEN BETWEEN 1994 AND 1997", -112, 0, 80, 2, 4, 270, 0);
    Txt("BY ALESSANDRO GHIGNOLA.", -78, 0, 60, 2, 4, 270, 0);
    Txt(t, (1-(double)strlen(t)) * 6, 0, -60, 3, 4, 270, 0); // microcosm author
    Txt("MODERN VERSION IN 2013-2025", -104, 0, -84, 2, 4, 270, 0);

    // rotate camera and approach text
    if (beta<360) {
        cam_y += 25;
        beta += 2;
        darken_once();
    }
    
    // once the text is up close:
    else {
        place_fottifoh();
        if (original_resolution)
            original_twinkle_effect();
        else
            twinkle_effect();
        fade_out_effect();
    }
    Render();

    unsigned long cticks = SDL_GetTicks();
    while (sync + tickrate > cticks) {
        SDL_Delay(3);
        cticks = SDL_GetTicks();
    }

    return true;
}