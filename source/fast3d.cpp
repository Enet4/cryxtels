/** \file fast3d.cpp
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

#include "fast3d.h"

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "sdl_exception.h"
#include "conf.h"

// Variables!
//SDL_Surface * p_surface = nullptr;
std::unique_ptr<unsigned char[]> video_buffer;

SDL_Surface * p_surface_32 = nullptr;
SDL_Surface * p_surface_scaled = nullptr;
SDL_Window * p_window = nullptr;
SDL_Renderer * p_renderer = nullptr;
SDL_Texture * p_texture = nullptr;

float *tcos, *tsin;
float *tcosx, *tsinx;
float *tcosy, *tsiny;

double cam_x = 0;
double cam_y = 0;
double cam_z = 0;

short int alfa  = 0;
short int beta  = 0;

double kk;
double rx, ry, rz;
double x2, y2, z2;
double ox, oy, oz;

//#define lwx 3
const int lwx = 3;
//#define lwy 3
const int lwy = 3;

const int zbasex = WIDTH * 2 / 3, zbasey = 150*HEIGHT/200;
const int nav_zbasex = WIDTH-2, nav_zbasey = HEIGHT-2;
const int lowerbound_y = -static_cast<int>(HEIGHT/2-lwy), upperbound_y = static_cast<int>(HEIGHT/2-lwy);
const int lowerbound_x = -static_cast<int>(WIDTH/2-lwx), upperbound_x = static_cast<int>(WIDTH/2-lwx);

//#define upx 317
const int upx = WIDTH - 3;
//#define upy 197
const int upy = HEIGHT - 3;

int x_centro = WIDTH/2;
int y_centro = HEIGHT/2;

double uneg = 1;
double mindiff = 0.01;

/// New Palette definition
unsigned char tmppal[256 * 4]; // 256*4 bytes (RGBU)

// Old Palette definition
//unsigned char tmppal[768]; // 256*3 bytes (RGB)

using namespace std;

void init(void);

void init_video () // inizializza grafica a 320x200x256 colori.
{

    init();

    // Set SDL video mode (using surface rendering)
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    // create software video buffer
    video_buffer.reset(new unsigned char[WIDTH*HEIGHT]);
    memset(&video_buffer[0], 0, WIDTH*HEIGHT);

    p_surface_32 = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, rmask, gmask, bmask, amask);
    if (p_surface_32 == nullptr) throw sdl_exception();

    p_window = SDL_CreateWindow("Crystal Pixels",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH_SCALED, HEIGHT_SCALED, SDL_WINDOW_RESIZABLE);
    if (p_window == nullptr) throw sdl_exception();

    p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_TARGETTEXTURE);

    p_texture = SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_ABGR8888,
                                  SDL_TEXTUREACCESS_STREAMING, WIDTH,HEIGHT);

    //p_surface_scaled = SDL_GetWindowSurface(p_window);
    SDL_SetWindowMinimumSize(p_window, WIDTH, HEIGHT);
}

/// Darken the screen once.
void darken_once (unsigned char inc) {
    unsigned char* it = &video_buffer[0];
    unsigned int cx = WIDTH*HEIGHT;

    do {
        auto k = *it;
        if (k >= inc) {
             *it = k - inc;
        } else if (k > 0) {
            *it = 0;
        }
        ++it;
    } while(--cx != 0);
}

void toggle_fullscreen (void) {
    auto flags = SDL_GetWindowFlags(p_window);
    SDL_SetWindowFullscreen(p_window, flags ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void snapshot (void)
{
    SDL_SaveBMP(p_surface_32, "SNAP.bmp");
}

void Render (void)
{
    using namespace std;

    // convert indexed 8-bit to RGBA 32-bit colors
    //SDL_LockSurface(p_surface_32);
    SDL_LockTexture(p_texture, nullptr, &p_surface_32->pixels, &p_surface_32->pitch);
    // paint into surface pixels
    unsigned int rpos = 0;
    unsigned int rpos_32 = 0;
    unsigned char* p_orig = &video_buffer[0];
    unsigned int* p_dest = static_cast<unsigned int*>(p_surface_32->pixels);
    for (unsigned int y = 0 ; y < HEIGHT ; y++) {
        for (unsigned int x = 0 ; x < WIDTH ; x++) {
            p_dest[rpos_32 + x] = reinterpret_cast<unsigned int*>(tmppal)[p_orig[rpos+x]];
        }
        rpos += WIDTH;
        rpos_32 += (p_surface_32->pitch >> 2);
    }
    SDL_UnlockTexture(p_texture);
    //SDL_UnlockSurface(p_surface_32);
    SDL_RenderCopy(p_renderer, p_texture, nullptr, nullptr);
    SDL_RenderPresent(p_renderer);
}

void tavola_colori (unsigned char *nuova_tavolozza,
            unsigned int colore_di_partenza, unsigned int nr_colori,
            char filtro_rosso, char filtro_verde, char filtro_blu)
{
    constexpr unsigned int K_FILTER = 63; // original is 63
    unsigned int c, cc = 0;
    unsigned int temp;
    nr_colori *= 4; // using new padding in palette table
    //nr_colori *= 3;
    colore_di_partenza *= 4; // new padding
    //colore_di_partenza *= 3;

    int pad = 3;
    c = colore_di_partenza;
    while (c < nr_colori-colore_di_partenza) {
        tmppal[c] = nuova_tavolozza[cc];
        cc++;
        c++;
        if (--pad == 0) {
            tmppal[c] = 0;
            c++;
            pad = 3;
        }
    }

    c = colore_di_partenza;
    while (c<nr_colori+colore_di_partenza) {
        temp = tmppal[c];
        temp *= (unsigned char)filtro_rosso;
        temp /= K_FILTER;
        tmppal[c] = temp;
        c++;
        temp = tmppal[c];
        temp *= (unsigned char)filtro_verde;
        temp /= K_FILTER;
        tmppal[c] = temp;
        c++;
        temp = tmppal[c];
        temp *= (unsigned char)filtro_blu;
        temp /= K_FILTER;
        tmppal[c] = temp;
        c+=2;
        //tmppal[c] = 0;
        //c++;
    }
}

// This function is unsafe and should be removed
void pcopy (unsigned char *dest, const unsigned char *sorg) {
    // The old "it's assembly, so it's faster" ideology is
    // what will crack the skulls of future coders and is no
    // longer really true anyway.

    // let's just use memcpy
    memcpy(dest, sorg, WIDTH*HEIGHT*sizeof(*dest));
}


// This function is unsafe and should be removed
void pclear (unsigned char *target, unsigned char pattern) {
    // Again, let's just try something different.
    memset(target, pattern, WIDTH*HEIGHT*sizeof(*target));
}

void init () {
    /* Caricamento delle tabelle con i risultati delle funzioni
       trigonometriche, spesso molto lente nell'esecuzione. */

    int c;
    double ang;

    tsin  = new float[720]; // 2880 bytes
    tcos  = new float[720];

    tsinx = new float[722]; // 2888 bytes
    tsiny = new float[722];
    tcosx = new float[722];
    tcosy = new float[722];

    if (!tcosy) return;

    for (ang=0, c=0; c<720; ang+=Pi/180, c++) {
        tsin[c] = sin (ang);
        tcos[c] = cos (ang);
    }

    for (ang=0, c=0; c<361; ang+=Pi/180, c++) {
        tsinx[c] = sin (ang) * zbasex; tcosx[c] = cos (ang) * zbasex;
        tsiny[c] = sin (ang) * zbasey; tcosy[c] = cos (ang) * zbasey;
    }
    for (ang=0, c=361; c<722; ang+=Pi/180, c++) {
        tsinx[c] = sin (ang) * nav_zbasex; tcosx[c] = cos (ang) * nav_zbasex;
        tsiny[c] = sin (ang) * nav_zbasey; tcosy[c] = cos (ang) * nav_zbasey;
    }
}

unsigned int ptr;
unsigned int global_x, global_y;
int share_x;
int share_y;

void Segmento (unsigned int x, unsigned int y,
           unsigned int x2, unsigned int y2)
{
    // Pre-conditions
    SDL_assert(x >= 0);
    SDL_assert(y >= 0);
    SDL_assert(x < WIDTH);
    SDL_assert(y < HEIGHT);
    SDL_assert(x2 >= 0);
    SDL_assert(y2 >= 0);
    SDL_assert(x2 < WIDTH);
    SDL_assert(y2 < HEIGHT);

    unsigned char* si = &video_buffer[0];

    int temp;
    if (x==x2) {
        if (y>y2) {
            std::swap(y,y2);
        }
        y2++;
        unsigned int di = WIDTH*y + x;
        unsigned int _ax = WIDTH*y2;
        // now we're making a pointer to the scanline at y2
        unsigned char* ax = si + _ax;
        // now we'll make si point to the other scanline (y)
        si += di;
        while (si < ax) {
            if (*si < 32) {
                SDL_assert_paranoid(si-WIDTH >= &video_buffer[0]);
                SDL_assert_paranoid(si+WIDTH < &video_buffer[0]+WIDTH*HEIGHT);
                *(si-1)       += 2;
                *si           += 4;
                *(si+1)       += 2;
                *(si+WIDTH-1) += 1;
                *(si+WIDTH)   += 2;
                *(si+WIDTH+1) += 1;
                *(si-WIDTH-1) += 1;
                *(si-WIDTH)   += 2;
                *(si-WIDTH+1) += 1;
            }
            si += WIDTH;
        }
        return;
    }

    int a = x2-x;
    if (a<0) {
        temp = x2; x2 = x; x = temp;
        temp = y2; y2 = y; y = temp;
        a = x2-x;
    }

    int L = a;
    int b = y2-y;
    if (b>0) {
        if (b>L)
            L=b;
    }
    else {
        if (-b>L)
            L=-b;
    }
    L++;

    x2 <<= 16;
    a <<= 16; a /= L;
    b <<= 16; b /= L;

    global_y = y << 16;
    global_x = x << 16;

    /* Traccia il segmento tramite DMA. */

    //const unsigned int HACK_SHIFT = -4;
    do {
        unsigned int di = global_y >> 16;

        unsigned char* di2 = si + WIDTH*di + (global_x >> 16);

        if ( *di2 < 32 ) {
            SDL_assert_paranoid(di2-WIDTH >= &video_buffer[0]);
            SDL_assert_paranoid(di2+WIDTH < &video_buffer[0]+WIDTH*HEIGHT);
            *(di2+1)       += 2;
            *di2           += 4;
            *(di2+1)       += 2;
            *(di2+WIDTH-1) += 1;
            *(di2+WIDTH)   += 2;
            *(di2+WIDTH+1) += 1;
            *(di2-WIDTH-1) += 1;
            *(di2-WIDTH)   += 2;
            *(di2-WIDTH+1) += 1;
        }
        global_y += b;
        global_x += a;
    } while (global_x < x2);
}

char explode = 0;

void Line3D (double p_x, double p_y, double p_z,
         double x, double y, double z)
{
    int fx, fy, lx, ly;
    double p_rx, p_ry, p_rz, p_z2, diff;

    if (explode) {
        p_rx = (p_x + x) / 2;
        p_ry = (p_y + y) / 2;
        p_rz = (p_z + z) / 2;
        kk = (10 / static_cast<double>(explode)) - 0.1;
        rx = (p_rx - ox) * kk;
        ry = (p_ry - oy) * kk;
        rz = (p_rz - oz) * kk;
        p_x += rx; p_y += ry; p_z += rz;
        x += rx; y += ry; z += rz;
    }

    p_x -= cam_x;
    p_y -= cam_y;
    p_z -= cam_z;

    p_z2 = p_z * tcos[beta] - p_x * tsin[beta];
    p_rz = p_z2 * tcos[alfa] + p_y * tsin[alfa];

    x -= cam_x;
    y -= cam_y;
    z -= cam_z;

    z2 = z * tcos[beta] - x * tsin[beta];
    rz = z2 * tcos[alfa] + y * tsin[alfa];

    if (rz<uneg&&p_rz<uneg) return;

    p_rx = p_x * tcosx[beta] + p_z * tsinx[beta];
    p_ry = p_y * tcosy[alfa] - p_z2 * tsiny[alfa];

    rx = x * tcosx[beta] + z * tsinx[beta];
    ry = y * tcosy[alfa] - z2 * tsiny[alfa];

    /* Conversione punti alle spalle dell'osservatore rispetto al piano
       dello schermo. */

    if (rz<uneg) {
        diff = rz-p_rz;
        if (diff<-mindiff || diff>mindiff) {
            kk = (uneg-p_rz) / diff;
            rx = kk * (rx-p_rx) + p_rx;
            ry = kk * (ry-p_ry) + p_ry;
        }
        rz = uneg;
    }
    else {
        if (p_rz<uneg) {
            diff = p_rz-rz;
            if (diff<-mindiff || diff>mindiff) {
                kk = (uneg-rz) / diff;
                p_rx = kk * (p_rx-rx) + rx;
                p_ry = kk * (p_ry-ry) + ry;
            }
            p_rz = uneg;
        }
    }

    // Prospettiva.

    fx = rx / rz; fy = ry / rz;
    lx = p_rx / p_rz; ly = p_ry / p_rz;

    /* Ottimizza il tracciamento del "Segmento", tagliando via le parti
       che di sicuro non si vedono. Questa parte non  efficace se
       i punti di partenza e di arrivo fanno s che il segmento non
       intersechi alcun lato dell'area visibile: ma in questo peculiare
       caso le linee vengono escluse dai controlli successivi. */

    if (fx<lowerbound_x) {
        diff = fx-lx;
        if (diff<-mindiff || diff>mindiff) {
            kk = (lowerbound_x-lx) / diff;
            fy = kk * (fy-ly) + ly;
            fx = lowerbound_x;
        }
    }
    if (lx<lowerbound_x) {
        diff = lx-fx;
        if (diff<-mindiff || diff>mindiff) {
            kk = (lowerbound_x-fx) / diff;
            ly = kk * (ly-fy) + fy;
            lx = lowerbound_x;
        }
    }
    if (fy<lowerbound_y) {
        diff = fy-ly;
        if (diff<-mindiff || diff>mindiff) {
            kk = (lowerbound_y-ly) / diff;
            fx = kk * (fx-lx) + lx;
            fy = lowerbound_y;
        }
    }
    if (ly<lowerbound_y) {
        diff = ly-fy;
        if (diff<-mindiff || diff>mindiff) {
            kk = (lowerbound_y-fy) / diff;
            lx = kk * (lx-fx) + fx;
            ly = lowerbound_y;
        }
    }
    if (fx>upperbound_x) {
        diff = fx-lx;
        if (diff<-mindiff || diff>mindiff) {
            kk = (upperbound_x-lx) / diff;
            fy = kk * (fy-ly) + ly;
            fx = upperbound_x;
        }
    }
    if (lx>upperbound_x) {
        diff = lx-fx;
        if (diff<-mindiff || diff>mindiff) {
            kk = (upperbound_x-fx) / diff;
            ly = kk * (ly-fy) + fy;
            lx = upperbound_x;
        }
    }
    if (fy>upperbound_y) {
        diff = fy-ly;
        if (diff<-mindiff || diff>mindiff) {
            kk = (upperbound_y-ly) / diff;
            fx = kk * (fx-lx) + lx;
            fy = upperbound_y;
        }
    }
    if (ly>upperbound_y) {
        diff = ly-fy;
        if (diff<-mindiff || diff>mindiff) {
            kk = (upperbound_y-fy) / diff;
            lx = kk * (lx-fx) + fx;
            ly = upperbound_y;
        }
    }

    if (fx==lx&&fy==ly) return; // Esclude le linee costituite da un punto solo.

    if (fy<lowerbound_y||ly<lowerbound_y) return; // Esclude le linee che mai e poi mai si vedranno.
    if (fy>upperbound_y||ly>upperbound_y) return;
    if (fx<lowerbound_x||lx<lowerbound_x) return;
    if (fx>upperbound_x||lx>upperbound_x) return;

    Segmento (fx+x_centro, fy+y_centro, lx+x_centro, ly+y_centro);
}

int C32 (double x, double y, double z)
{
    x -= cam_x;
    y -= cam_y;
    z -= cam_z;

    z2 = z * tcos[beta] - x * tsin[beta];
    rz = z2 * tcos[alfa] + y * tsin[alfa];

    if (rz<uneg) return 0; // Il punto non si vede. Inutile continuare.

    ry = y * tcosy[alfa] - z2 * tsiny[alfa];
    share_y = ry / rz;

    if (share_y<lowerbound_y || share_y>upperbound_y)
        return 0;

    rx = x * tcosx[beta] + z * tsinx[beta];
    share_x = rx / rz;

    if (share_x<lowerbound_x || share_x>upperbound_x)
        return 0;

    share_x += x_centro;
    share_y += y_centro;

    return 1;
}

/// Make one thick plot at (share_x,share_y)
void aux_plot(void) {
    aux_plot(static_cast<unsigned int>(share_x), static_cast<unsigned int>(share_y));
}

/// Make one thick plot
void aux_plot(unsigned int x, unsigned int y) {
    // Pre-conditions
    SDL_assert(x > 0);
    SDL_assert(y > 0);
    SDL_assert(x < WIDTH-1);
    SDL_assert(y < HEIGHT-1);

    auto di = x+WIDTH*y;
    auto si = &video_buffer[0] + di;

    if (*si < 32) {
        *si += 8;
        *(si+1) += 4;
        *(si-1) += 4;
        *(si+WIDTH) += 4;
        *(si-WIDTH) += 4;
//    }
    }
}

void rel (double sx, double sy, double sz, double fx, double fy, double fz)
{
    sx += ox; fx += ox;
    sy += oy; fy += oy;
    sz += oz; fz += oz;

    Line3D (sx, sy, sz, fx, fy, fz);
}


void rectrel (double cx, double cy, double cz, double hx, double hz, char orient)
{
    cx += ox; cy += oy; cz += oz;

    if (orient==0) {
            Line3D (cx-hx, cy-hz, cz, cx+hx, cy-hz, cz);
            Line3D (cx+hx, cy-hz, cz, cx+hx, cy+hz, cz);
            Line3D (cx+hx, cy+hz, cz, cx-hx, cy+hz, cz);
            Line3D (cx-hx, cy+hz, cz, cx-hx, cy-hz, cz);
    } else if (orient==1) {
            Line3D (cx-hx, cy, cz+hz, cx+hx, cy, cz+hz);
            Line3D (cx+hx, cy, cz+hz, cx+hx, cy, cz-hz);
            Line3D (cx+hx, cy, cz-hz, cx-hx, cy, cz-hz);
            Line3D (cx-hx, cy, cz-hz, cx-hx, cy, cz+hz);
    } else {
            Line3D (cx, cy-hz, cz-hx, cx, cy-hz, cz+hx);
            Line3D (cx, cy-hz, cz+hx, cx, cy+hz, cz+hx);
            Line3D (cx, cy+hz, cz+hx, cx, cy+hz, cz-hx);
            Line3D (cx, cy+hz, cz-hx, cx, cy-hz, cz-hx);
        }
}

void boxrel (double cx, double cy, double cz,
         double hx, double hy, double hz)
{
    cx += ox; cy += oy; cz += oz;
    Line3D (cx-hx, cy-hy, cz+hz, cx-hx, cy+hy, cz+hz);
    Line3D (cx+hx, cy-hy, cz+hz, cx+hx, cy+hy, cz+hz);
    Line3D (cx+hx, cy-hy, cz-hz, cx+hx, cy+hy, cz-hz);
    Line3D (cx-hx, cy-hy, cz-hz, cx-hx, cy+hy, cz-hz);
    Line3D (cx-hx, cy-hy, cz+hz, cx+hx, cy-hy, cz+hz);
    Line3D (cx+hx, cy-hy, cz+hz, cx+hx, cy-hy, cz-hz);
    Line3D (cx+hx, cy-hy, cz-hz, cx-hx, cy-hy, cz-hz);
    Line3D (cx-hx, cy-hy, cz-hz, cx-hx, cy-hy, cz+hz);
    Line3D (cx-hx, cy+hy, cz+hz, cx+hx, cy+hy, cz+hz);
    Line3D (cx+hx, cy+hy, cz+hz, cx+hx, cy+hy, cz-hz);
    Line3D (cx+hx, cy+hy, cz-hz, cx-hx, cy+hy, cz-hz);
    Line3D (cx-hx, cy+hy, cz-hz, cx-hx, cy+hy, cz+hz);
}

void xrel (double px, double py, double pz)
{
    if (C32 (ox+px, oy+py, oz+pz)) {

        unsigned int di = share_x+WIDTH*share_y;

        unsigned char* si = &video_buffer[0];
        si += di; // get to point position
        if (*si < 32) {

            // the following code seems to paint this point's pixel
            // and adjacent pixels to make it look thicker

            *si += 8;
            *(si+1) += 4;
            *(si-1) += 4;
            *(si+WIDTH-1) += 2;
            *(si-WIDTH+1) += 2;
            *(si+WIDTH) += 4;
            *(si-WIDTH) += 4;
            *(si+WIDTH+1) += 2;
            *(si-WIDTH-1) += 2;
        }
    }
}
