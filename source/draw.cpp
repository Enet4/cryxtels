/** \file draw.cpp
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

#include <iostream>
#include <cassert>
#include "draw.h"
#include "global.h"
#include "fast3d.h"
#include "text3d.h"
#include "input.h"
#include "utils.h"

#include "conf.h"

void draw_line_relative (double sx, double sy, double sz, double fx, double fy, double fz)
{
        z2 = sz * tcos[nav_a] + sy * tsin[nav_a];
        sy = sy * tcos[nav_a] - sz * tsin[nav_a];
        rx = sx * tcos[nav_b] + z2 * tsin[nav_b];
        sz = z2 * tcos[nav_b] - sx * tsin[nav_b];
        sx = rx;

        z2 = fz * tcos[nav_a] + fy * tsin[nav_a];
        fy = fy * tcos[nav_a] - fz * tsin[nav_a];
        rx = fx * tcos[nav_b] + z2 * tsin[nav_b];
        fz = z2 * tcos[nav_b] - fx * tsin[nav_b];
        fx = rx;

        sx += nav_x; sy += nav_y; sz += nav_z;
        fx += nav_x; fy += nav_y; fz += nav_z;

        Line3D (sx, sy, sz, fx, fy, fz);
}

void draw_text_relative (const char *testo, double x, double y, double z, double sx, double sy)
{
        int c=0;
        char transit[2] = {0, 0};
        double ntx, nty, ntz;

        z2 = z * tcos[nav_a] + y * tsin[nav_a];
        ntx = x * tcos[nav_b] + z2 * tsin[nav_b];
        nty = y * tcos[nav_a] - z * tsin[nav_a];
        ntz = z2 * tcos[nav_b] - x * tsin[nav_b];

        x += sx * mediumwidth;

        z2 = z * tcos[nav_a] + y * tsin[nav_a];
        double avan_x = x * tcos[nav_b] + z2 * tsin[nav_b];
        double avan_y = y * tcos[nav_a] - z * tsin[nav_a];
        double avan_z = z2 * tcos[nav_b] - x * tsin[nav_b];

        avan_x -= ntx;
        avan_y -= nty;
        avan_z -= ntz;

        while (testo[c]) {
                transit[0] = testo[c];
                Txt (transit, ntx, nty, ntz, sx, sy, nav_a, nav_b);
                ntx += avan_x; nty += avan_y; ntz += avan_z;
                c++;
        }
}

void draw_rect_relative (double x, double y, double z, double l, double h)
{
        draw_line_relative (x-l, y-h, z, x+l, y-h, z);
        draw_line_relative (x+l, y-h, z, x+l, y+h, z);
        draw_line_relative (x+l, y+h, z, x-l, y+h, z);
        draw_line_relative (x-l, y+h, z, x-l, y-h, z);
}

void draw_console_key (const char *serigraph, double x, char cod, char input, char current_state, char previous_state)
{
        if (EVA_in_progress) {
                // disable console keys when far away from the HUD
                d = sqrt(rel_x*rel_x + (rel_z+10)*(rel_z+10));
                if (d>25) {
                        input = 1;
                        cod = 0;
                }
        }

        // only show console key as pressed when
        // the key pressed is the same as the one bound to this console key
        // AND the key wasn't already pressed in the previous frame
        // (save for key code 1, which is a toggle button)
        if (input!=cod || (cod != 1 && current_state == previous_state)) {
                draw_line_relative (x, 3.5, 12.01, x+1, 3.5, 12.01);
                draw_line_relative (x, 3.5, 12.01, x, 4, 12.01);
                draw_line_relative (x+1, 3.5, 12.01, x+1, 4, 12.01);
                draw_line_relative (x, 3.5, 12.01, x, 4, 12.2);
                draw_line_relative (x+1, 3.5, 12.01, x+1, 4, 12.2);
                draw_line_relative (x, 4, 12.2, x+1, 4, 12.2);
                draw_line_relative (x, 4, 12.01, x, 4, 12.2);
                draw_line_relative (x+1, 4, 12.01, x+1, 4, 12.2);
                draw_text_relative (serigraph, x+0.2, 3.7, 12.01, 0.05, 0.05);
        }
        else {
                draw_rect_relative (x+0.5, 3.75, 12.31, 0.5, 0.25);
                //draw_line_relative (x, 4, 12.31, x+1, 4, 12.31);
                //draw_line_relative (x, 3.5, 12.31, x+1, 3.5, 12.31);
                //draw_line_relative (x, 3.5, 12.31, x, 4, 12.31);
                //draw_line_relative (x+1, 3.5, 12.31, x+1, 4, 12.31);
                draw_line_relative (x, 3.5, 12.31, x, 4, 12.5);
                draw_line_relative (x+1, 3.5, 12.31, x+1, 4, 12.5);
                draw_line_relative (x, 4, 12.5, x+1, 4, 12.5);
                draw_line_relative (x, 4, 12.31, x, 4, 12.5);
                draw_line_relative (x+1, 4, 12.31, x+1, 4, 12.5);
                draw_text_relative (serigraph, x+0.2, 3.7, 12.31, 0.05, 0.05);
                if (current_state!=previous_state) {
                        if (EVA_in_progress && globalvocfile[0]!='.') return;
                        //if (!sbp_stat) play (TASTO);
                        subs = 0;
                }
        }
}

void draw_indicator_crosshair()
{
    draw_line_relative (-1, -1, 18, -0.5, -0.5, 18);
    draw_line_relative (-1, 1, 18, -0.5, 0.5, 18);
    draw_line_relative (1, -1, 18, 0.5, -0.5, 18);
    draw_line_relative (1, 1, 18, 0.5, 0.5, 18);
}

void draw_indicator_prograde(int x, int y)
{
    static const u32 render_width = get_config().render_width;
    static const u32 render_height = get_config().render_height;

    static const int width = 5*render_height/200;

    static const int xlow = 5*render_width/320;
    static const int xhigh = 315*render_width/320;
    static const int ylow = xlow;
    static const int yhigh = 195*render_height/200; 
    if ( x>xlow && x<xhigh && y>ylow && y<yhigh) {
        Segmento (x - width, y - width, x + width, y + width);
        Segmento (x + width, y - width, x - width, y + width);
    }
}

void draw_indicator_retrograde(int x, int y)
{
    static const u32 render_width = get_config().render_width;
    static const u32 render_height = get_config().render_height;

    static const int outside = 9*render_height/200;
    static const int inside  = 5*render_height/200;

    static const int xlow = 10*render_width/320;
    static const int xhigh = 310*render_width/320;
    static const int ylow = xlow;
    static const int yhigh = 190*render_height/200; 
    if ( x>xlow && x<xhigh && y>ylow && y<yhigh) {
        Segmento (x - outside, y - inside, x + outside, y - inside);
        Segmento (x - outside, y + inside, x + outside, y + inside);
        Segmento (x - inside, y - outside, x - inside, y + outside);
        Segmento (x + inside, y - outside, x + inside, y + outside);
    }
}

void draw_indicator_closest(int x, int y)
{
    static const u32 render_width = get_config().render_width;
    static const u32 render_height = get_config().render_height;

    static const int width = 19*render_height/200;

    static const int xlow = 20*render_width/320;
    static const int xhigh = 300*render_width/320;
    static const int ylow = xlow;
    static const int yhigh = 180*render_height/200; 
    if ( x>xlow && x<xhigh && y>ylow && y<yhigh) {
        Segmento (x, y - width, x, y + width);
        Segmento (x - width, y, x + width, y);
    }
}

void draw_vehicle_interior()
{
    // info panel without readings
    draw_rect_relative (0, 4.95, 12.01, 6, 0.95);

    draw_rect_relative (0, 4.95, 14, 6, 0.95);
    draw_line_relative (-6, 4, 12.01, -6, 4, 14);
    draw_line_relative ( 6, 4, 12.01,  6, 4, 14);
    draw_line_relative (-6, 5.9, 12.01, -6, 5.9, 14);
    draw_line_relative ( 6, 5.9, 12.01,  6, 5.9, 14);

    // ship's front section
    draw_rect_relative (0, 13.5, 58, 4, 2.5);

    draw_line_relative (-10, 16, 16,  10, 16, 16);
    draw_line_relative (-10, -5, 16, -10, 16, 16);
    draw_line_relative ( 10, -5, 16,  10, 16, 16);
    draw_line_relative (-10,  1, 25,  -4, 11, 58);
    draw_line_relative (-10,  1, 25, -10, -5, 16);
    draw_line_relative (-10,  1, 25,-8.7, 16, 25);
    //draw_line_relative ( -4, 11, 58,   4, 11, 58);
    //draw_line_relative (  4, 11, 58,   4, 16, 58);
    //draw_line_relative ( -4, 11, 58,  -4, 16, 58);
    //draw_line_relative (  4, 16, 58,  -4, 16, 58);
    draw_line_relative (  4, 11, 58,  10,  1, 25);
    draw_line_relative ( 10,  1, 25,  10, -5, 16);
    draw_line_relative ( 10,  1, 25, 8.7, 16, 25);
    draw_line_relative (  4, 16, 58,  10, 16, 16);
    draw_line_relative ( -4, 16, 58, -10, 16, 16);
    draw_line_relative ( -4, 16, 58,   0, 16, 83);
    draw_line_relative (  0, 16, 83,   4, 16, 58);
    draw_line_relative (  0, 16, 83,  -4, 11, 58);
    draw_line_relative (  0, 16, 83,   4, 11, 58);
    draw_line_relative (  0, 13, 64,  -4, 11, 58);
    draw_line_relative (  0, 13, 64,   4, 11, 58);
    draw_line_relative (  0, 13, 64,  -4, 16, 58);
    draw_line_relative (  0, 13, 64,   4, 16, 58);
}

void draw_vehicle_exterior()
{
    // Attracchi.
    /*  
    _x = pixel_xdisloc[pix]+docksite_x[pixeltype[pix]];
    _y = pixel_ydisloc[pix]+docksite_y[pixeltype[pix]];
    _z = pixel_zdisloc[pix]+docksite_z[pixeltype[pix]];

    Line3D (nav_x-10, nav_y-5, nav_z+16, _x-50, _y, _z+50);
    Line3D (nav_x+10, nav_y-5, nav_z+16, _x+50, _y, _z+50);
    Line3D (nav_x-14, nav_y-10, nav_z-31, _x-50, _y, _z-50);
    Line3D (nav_x+14, nav_y-10, nav_z-31, _x+50, _y, _z-50);
    */

    // Cabina.
    draw_line_relative (-10,  -5,  16, -14, -10, -31);
    draw_line_relative ( 10,  -5,  16,  14, -10, -31);
    draw_line_relative (-10,  16,  16, -14,  16, -31);
    draw_line_relative ( 10,  16,  16,  14,  16, -31);
    draw_line_relative (-14, -10, -31,  14, -10, -31);
    draw_line_relative (-14, -10, -31, -10,   0, -43);
    draw_line_relative ( 14, -10, -31,  10,   0, -43);
    draw_line_relative (  0,  16, -69, -10,   0, -43);
    draw_line_relative (  0,  16, -69,  10,   0, -43);
    draw_line_relative (  0,  16, -69, -10,  16, -43);
    draw_line_relative (  0,  16, -69,  10,  16, -43);
    draw_line_relative (-10,  16, -43, -14,  16, -31);
    draw_line_relative ( 10,  16, -43,  14,  16, -31);
    draw_line_relative (-10,   0, -43,  10,   0, -43);
    draw_line_relative (-10,  16, -43,  10,  16, -43);
    draw_line_relative (-10,   0, -43, -10,  16, -43);
    draw_line_relative ( 10,   0, -43,  10,  16, -43);
    draw_line_relative (-14, -10, -31, -14,  16, -31);
    draw_line_relative ( 14, -10, -31,  14,  16, -31);

    // Cupola della cabina.
    draw_line_relative (-10,  -5,  16,  -6,  -9,   8);
    draw_line_relative ( -6,  -9,   8,   6,  -9,   8);
    draw_line_relative (  6,  -9,   8,  10,  -5,  16);
    draw_line_relative ( -6,  -9,   8, -10, -14, -26);
    draw_line_relative (-10, -14, -26, -14, -10, -31);
    draw_line_relative (-10, -14, -26,  10, -14, -26);
    draw_line_relative (  6,  -9,   8,  10, -14, -26);
    draw_line_relative ( 10, -14, -26,  14, -10, -31);

    // Ali.
    draw_line_relative (-10, 16,  16, -40, 16, -55);
    draw_line_relative (-40, 16, -55, -10, 16, -43);
    draw_line_relative ( 10, 16,  16,  40, 16, -55);
    draw_line_relative ( 40, 16, -55,  10, 16, -43);
}

void draw_readings_force(double force)
{
    char str[20];
    sprintf (str, "F=%04d/CGR", (int) force);
    draw_text_relative (str, -5.5, 4.5, 12.01, 0.07, 0.12);
}

// conversion factor from units per frame to km/h:
// 1 unit/frame = 0.90 m/s at 30 frames/s
// which means 1 unit/frame = 3.24 km/h (the original value was 1.9656)
void draw_readings_speed(double speed)
{
    char str[20];
    sprintf (str, "V=%04.0f:K/H", speed*3.24);
    draw_text_relative (str, -5.5, 5.4, 12.01, 0.07, 0.12);
}

void draw_readings_docked()
{
    char str[20];
    sprintf (str, "--DOCKED--");
    draw_text_relative (str, -5.5, 5.4, 12.01, 0.07, 0.12);
}

void draw_readings_heading(i16 alpha, i16 beta)
{
    char str[20];
    sprintf (str, "ASC=%03hd`", beta);
    draw_text_relative (str, -2.25, 4.5, 12.01, 0.07, 0.12);
    sprintf (str, "DEC=%03hd`", alpha);
    draw_text_relative (str, -2.25, 5.4, 12.01, 0.07, 0.12);
}

void draw_readings_position(double x, double y, double z)
{
    char str[20];
    sprintf (str, "H=%1.1f:KM", -y/33333.33333);
    draw_text_relative (str, 0.35, 4.5, 12.01, 0.06, 0.12);
    sprintf (str, "X=%1.1f:KM", x/33333.33333);
    draw_text_relative (str, 3.1, 4.5, 12.01, 0.07, 0.12);
    sprintf (str, "Z=%1.1f:KM", z/33333.33333);
    draw_text_relative (str, 3.1, 5.4, 12.01, 0.07, 0.12);
}

void draw_readings_closest(double distance)
{
    char str[20];
    if (distance>1000)
        sprintf (str, "D=%1.1f:KM", distance/1000);
    else
        sprintf (str, "D=%1.1f:MT", distance);
    draw_text_relative (str, 0.35, 5.4, 12.01, 0.06, 0.12);
}

void draw_vehicle_attitude(i16 alpha, i16 beta, char blink)
{
    draw_text_relative ("N", -4, -4.5, 12.01, 0.07, 0.07);
    draw_text_relative ("S", -4, -3.5, 12.01, 0.07, 0.07);
    draw_text_relative ("E", -3.5, -4, 12.01, 0.07, 0.07);
    draw_text_relative ("W", -4.5, -4, 12.01, 0.07, 0.07);
    draw_line_relative (-4-tcos[beta+270]/2, -4-tsin[beta+270]/2, 12.01, -4+tcos[beta+270], -4+tsin[beta+270], 12.01);
    draw_line_relative (-4-tcos[beta+180]/2, -4-tsin[beta+180]/2, 12.01, -4+tcos[beta+180]/2, -4+tsin[beta+180]/2, 12.01);
    draw_text_relative ("Z", 4, -4.5, 12.01, 0.07, 0.07);
    draw_text_relative ("N", 4, -3.5, 12.01, 0.07, 0.07);
    draw_text_relative ("-", 4.5, -4, 12.01, 0.07, 0.07);
    if ( alpha>90 && alpha<270 && blink)
        draw_text_relative ("*", 3.5, -4, 12.01, 0.07, 0.07);
    i16 alphax = 359 - alpha;
    draw_line_relative (4-tcos[alphax]/2, -4-tsin[alphax]/2, 12.01, 4+tcos[alphax], -4+tsin[alphax], 12.01);
    draw_line_relative (4-tcos[alphax+90]/2, -4-tsin[alphax+90]/2, 12.01, 4+tcos[alphax+90]/2, -4+tsin[alphax+90]/2, 12.01);
}

// Fottifoh model data
i8 fotty[277] = {
        -6, 0, +6,          -4, 0, +8,
        -4, 0, +8,          -2, 0, +8,
        -2, 0, +8,          -2, 0, +6,
        -2, 0, +6,           0, 0, +7,
         0, 0, +7,          +2, 0, +6,
        +2, 0, +6,          +2, 0, +8,
        +2, 0, +8,          +4, 0, +8,
        +4, 0, +8,          +6, 0, +6,
        +6, 0, +6,          +6, 0, +4,
        +6, 0, +4,          +4, 0, +4,
        +4, 0, +4,          +4, 0, +2,
        +4, 0, +2,          +2, 0, +1,
        +2, 0, +1,          +4, 0,  0,
        +4, 0,  0,          +5, 0, -2,
        +5, 0, -2,          +4, 0, -4,
        +4, 0, -4,          +4, 0, -6,
        +4, 0, -6,          +2, 0, -8,
        +2, 0, -8,           0, 0, -7,
         0, 0, -7,          -2, 0, -8,
        -2, 0, -8,          -4, 0, -6,
        -4, 0, -6,          -4, 0, -4,
        -4, 0, -4,          -5, 0, -2,
        -5, 0, -2,          -4, 0,  0,
        -4, 0,  0,          -2, 0, +1,
        -2, 0, +1,          -4, 0, +2,
        -4, 0, +2,          -4, 0, +4,
        -4, 0, +4,          -6, 0, +4,
        -6, 0, +4,          -6, 0, +6,
        -2, 0,  0,          -1, 0, -2,
        -1, 0, -2,          -2, 0, -4,
        -2, 0, -4,           0, 0, -5,
         0, 0, -5,          +2, 0, -4,
        +2, 0, -4,          +1, 0, -2,
        +1, 0, -2,          +2, 0,  0,
        -4, 0, -4,          -2, 0, -4,
        +2, 0, -4,          +4, 0, -4,
         0, 0, -5,           0, 0, -7,
        -2, 0, +1,          -2, 0, +3,
        -2, 0, +3,           0, 0, +4,
         0, 0, +4,          +2, 0, +3,
        +2, 0, +3,          +2, 0, +1,
        +2, 0, +1,           0, 0,  0,
         0, 0,  0,          -2, 0, +1,
        -1, 0, +3,          +1, 0, +3,
        +1, 0, +3,           0, 0, +1,
         0, 0, +1,          -1, 0, +3,
10 };

// Updates buffer with loaded pixels:
// Search for the pixel type of ID `typ`
// and load it if it is not loaded yet.
void cerca_e_carica (PixelTypeId typ) {
    if (loaded_pixeltypes) {
        for (auto iii = 0u; iii<loaded_pixeltypes; iii++) {
            if (typ==pixeltype_type[iii])
                return;
        }
    }
    LoadPtyp (typ);
}

// TODO: What does this do?
void Update()
{
    int p, o, f;

    loaded_pixeltypes = 0;

    for (p=0; p<pixels; p++) {
        if (pixel_absd[p]<pixelmass[pixeltype[p]])
            cerca_e_carica (pixeltype[p]);
    }

    for (o=0; o<_objects; o++) {
        if (objecttype[o]>=3) {
                f = 0;
                if (object_location[o]>-1) {
                        if (pixel_absd[object_location[o]]<4000)
                            f = 1;
                }
                else {
                    rx = absolute_x[o] - cam_x;
                        ry = absolute_y[o] - cam_y;
                        rz = absolute_z[o] - cam_z;
                        if (sqrt(rx*rx+ry*ry+rz*rz)<1000) f = 1;
                        }
                        if (f) cerca_e_carica (objecttype[o]+FRONTIER_M3);
                }
        }

        if (carry_type>=3)
                cerca_e_carica (carry_type+FRONTIER_M3);
}

void fail_pixel_def (int el, int pix)
{
    alfin(false);
    std::cerr << "Pixel definition error: Pixel of type " << pix << " does not specify element nr. " << (el+1) << std::endl
         << "0-valued parameters are invalid." << std::endl;
    exit(1);
}

void Pixel (int typ)
{
    static const u32 width = get_config().render_width;

    double p0, p1, p2, p3, p4, p5, p6, crx=0, cry=0, crz=0;
    double first_x, first_y, first_z;

    int iii, pid = 3, nr_elem = 0;
    int jjj, bkuneg = uneg;
    unsigned vptr;

    if (typ>FRONTIER_M1) goto ricerca;

    ox = pixel_xdisloc[nopix];
    oy = pixel_ydisloc[nopix];
    oz = pixel_zdisloc[nopix];

    if (subsignal[9*typ]&&pixel_absd[nopix]>512) {
        p0 = pixel_absd[nopix] * 0.01; if (p0<628) p0 = 628;
        c = pixel_absd[nopix] / 700; if (c>45) c = 45; if (c<18) c = 18;
        for (a=0; a<360; a+=c) {
            for (b=c; b<180; b+=c) {
                if (C32(ox+p0*tsin[b]*tcos[a], oy+p0*tcos[b], oz-p0*tsin[a]*tsin[b])) {
                    // --- draw operation begin
                    auto di = share_x + width*share_y;
                    auto si = &video_buffer[0] + di;

                    if (*si < 32) {
                        *si += 7;
                        *(si+1) += 5; *(si-1) += 5;
                        *(si+width) += 5;  *(si-width) += 5;
                        *(si+width+1) += 3; *(si-width-1) += 3;
                        *(si-width+1) += 3; *(si+width-1) += 3;
                    }
                    // --- draw operation end
                }
            }
        }
    }

    if (pixel_absd[nopix]>pixelmass[typ]) {
        if (!C32(ox, oy, oz)) return;
        vptr = share_x+width*share_y;

        // --- draw operation begin
        auto cl = pixelmass[typ] / 1000; if (cl > 32) cl = 32;
        auto ch = cl / 2; auto dl = cl / 4;

        auto si = &video_buffer[0] + vptr;

        if (*si < 32 ) {
            *si += cl;
            *(si+1) += ch;
            *(si-1) += ch;
            *(si+2) += dl;
            *(si-2) += dl;
            *(si+width-1) += dl;
            *(si-width+1) += dl;
            *(si+width) += ch;
            *(si-width) += ch;
            *(si+width+1) += dl;
            *(si-width-1) += dl;
            *(si+width*2) += dl;
            *(si-width*2) += dl;
        }
        // --- draw operation end
    } else {
ricerca:
        if (loaded_pixeltypes) {
            for (iii = 0; iii<loaded_pixeltypes; iii++) {
                if (typ==pixeltype_type[iii])
                    goto trovato;
            }
        }
        Update();
        goto ricerca;
trovato:
        if (typ>FRONTIER_M1) {
            _x = ox - cam_x; _y = oy - cam_y; _z = oz - cam_z;
            d = sqrt(_x*_x+_y*_y+_z*_z);
            id = static_cast<int>(d / pixelmass[typ]);
            goto ogg_2;
        } else {
            id = static_cast<int>(pixel_absd[nopix]/CULLING);
            if (nopix==pix) explode = explode_count;
        }
        uneg += pixel_absd[nopix] / 100;
        if (uneg>1000) uneg = 1000;
ogg_2:
        if (id>3) id = 3;
        while (nr_elem<pixeltype_elements[iii]) {
            jjj = ELEMS * iii + nr_elem;
            if (pixel_elem_t[jjj]==DETTAGLIO) {
                if (id==pid) {
                    uneg = bkuneg;
                    explode = 0;
                    return;
                }
                pid--;
            }
            p0 = pixel_elem_x[jjj];
            p1 = pixel_elem_y[jjj];
            p2 = pixel_elem_z[jjj];
            p3 = pixel_elem_1[jjj];
            p4 = pixel_elem_2[jjj];
            p5 = pixel_elem_3[jjj];
            p6 = pixel_elem_4[jjj];
            switch (pixel_elem_t[jjj]) {
                case SPIRALE:
                    if (!p5) fail_pixel_def (nr_elem, pixeltype[nopix]);
                    c = p5 * (id+1);
                    p3 *= id+1;
                    switch (static_cast<int>(p4)) {
                        case 0:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0+k1*tcos[(int)crx], p1+k1*tsin[(int)crx], p2,
                                        p0+k0*tcos[(int)cry], p1+k0*tsin[(int)cry], p2);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                            break;
                        case 1:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0+k1*tcos[(int)crx], p1, p2+k1*tsin[(int)crx],
                                        p0+k0*tcos[(int)cry], p1, p2+k0*tsin[(int)cry]);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                            break;
                        case 2:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0, p1+k1*tcos[(int)crx], p2+k1*tsin[(int)crx], p0,
                                        p1+k0*tcos[(int)cry], p2+k0*tsin[(int)cry]);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                        }
                        break;
                    case PUNTO:
                        if (!id) xrel (p0, p1, p2);
                        break;
                    case LINEA:
                        rel (p0, p1, p2, p3, p4, p5);
                        break;
                    case RETTANGOLO:
                        rectrel (p0, p1, p2, p3, p4, p5);
                        break;
                    case SCATOLA:
                    case SOLID_BOX:
                        boxrel (p0, p1, p2, p3, p4, p5);
                        break;
                    case GRIGLIA:
                        p5 = (int) p5;
                        if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                        if (id) {
                            c = p5;
                            p5 /= id;
                            if (p5<1) p5 = 1;
                            p3 *= (float)c / p5;
                            p4 *= (float)c / p5;
                        }
                        _x = p3;
                        _y = p4;
                        switch ((int)p6) {
                            case 0:
                                p0 -= (p5*_x) / 2;
                                p1 -= (p5*_y) / 2;
                                p6 = p1 + _y*p5; _z = p0;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p6, p2);
                                    p0 += _x;
                                }
                                p0 = _z; p6 = p0 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p6, p1, p2);
                                    p1 += _y;
                                }
                                break;
                            case 1:
                                p0 -= (p5*_x) / 2;
                                p2 -= (p5*_y) / 2;
                                p6 = p2 + _y*p5; _z = p0;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p1, p6);
                                    p0 += _x;
                                }
                                p0 = _z; p6 = p0 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p6, p1, p2);
                                    p2 += _y;
                                }
                                break;
                            case 2:
                                p2 -= (p5*_x) / 2;
                                p1 -= (p5*_y) / 2;
                                p6 = p1 + _y*p5; _z = p2;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p6, p2);
                                    p2 += _x;
                                }
                                p2 = _z; p6 = p2 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p1, p6);
                                    p1 += _y;
                                }
                            }
                            break;
                        case DISEGNO_ELLITTICO:
                            p6 *= id+1;
                            if (p6>90) p6 = 90;
                            if (p5==0) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0+tcos[c]*p3, oy+p1+tsin[c]*p4, oz+p2)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            if (p5==1) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0+tcos[c]*p3, oy+p1, oz+p2+tsin[c]*p4)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            if (p5==2) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0, oy+p1+tsin[c]*p4, oz+p2+tcos[c]*p3)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            break;
                        case ONDA:
                            p6 *= id+1;
                            if (p6>90) p6 = 90;
                            switch ((int)p5) {
                                case 0:
                                    _x = p0 - p3*180 + p6*p3;
                                    _oy = p1; _ox = _x - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _y = p1 + tsin[c] * p4;
                                        rel (_x, _y, p2, _ox, _oy, p2);
                                        _oy = _y;
                                        _ox = _x;
                                        _x += p6*p3;
                                    }
                                    break;
                                case 1:
                                    _z = p2 - p3*180 + p6*p3;
                                    _ox = p0; _oz = _z - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _x = p0 + tsin[c] * p4;
                                        rel (_x, p1, _z, _ox, p1, _oz);
                                        _ox = _x;
                                        _oz = _z;
                                        _z += p6*p3;
                                    }
                                    break;
                                case 2:
                                    _z = p2 - p3*180 + p6*p3;
                                    _oy = p1; _oz = _z - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _y = p1 + tsin[c] * p4;
                                        rel (p0, _y, _z, p0, _oy, _oz);
                                        _oy = _y;
                                        _oz = _z;
                                        _z += p6*p3;
                                    }
                                }
                                break;
                            case COLONNA:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                _x = p0 + tcos[0]*p4;
                                _z = p2 + tsin[0]*p4;
                                _y = p1 - p5 / 2;
                                crx = p0 + tcos[0]*p3;
                                crz = p2 + tsin[0]*p3;
                                cry = p1 + p5 / 2;
                                for (a=p6; a<360; a+=p6) {
                                    _ox = _x;
                                    _oz = _z;
                                    first_x = crx;
                                    first_z = crz;
                                    _x = p0 + tcos[a]*p4;
                                    _z = p2 + tsin[a]*p4;
                                    crx = p0 + tcos[a]*p3;
                                    crz = p2 + tsin[a]*p3;
                                    rel (_x, _y, _z, crx, cry, crz);
                                    if (p4) rel (_x, _y, _z, _ox, _y, _oz);
                                    if (p3) rel (first_x, cry, first_z, crx, cry, crz);
                                }
                                _ox = _x;
                                _oz = _z;
                                first_x = crx;
                                first_z = crz;
                                _x = p0 + tcos[0]*p4;
                                _z = p2 + tsin[0]*p4;
                                crx = p0 + tcos[0]*p3;
                                crz = p2 + tsin[0]*p3;
                                rel (_x, _y, _z, crx, cry, crz);
                                if (p4) rel (_x, _y, _z, _ox, _y, _oz);
                                if (p3) rel (first_x, cry, first_z, crx, cry, crz);
                                break;
                            case ASTERISCO:
                                if (p4<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p3 = -p3;
                                p4 *= id+1;
                                if (p4>90) p4 = 90;
                                for (a=0; a<180; a+=p4)
                                    for (b=p4; b<180; b+=p4) {
                                        _oz = p3 * tcos[b];
                                        z2 = p3 * tsin[b];
                                        _ox = z2 * tcos[a];
                                        _oy = z2 * tsin[a];
                                        rel (p0-_ox, p1-_oz, p2+_oy, p0+_ox, p1+_oz, p2-_oy);
                                    }
                                break;
                            case SFERA:
                                if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                c = p5 * (id+1);
                                if (c>90) c = 90;
                                p3 = -p3;
                                _ox = ox;
                                _oy = oy;
                                _oz = oz;
                                crx = ox + p0;
                                cry = oy + p1;
                                crz = oz + p2;
                                for (a=0; a<360; a+=c)
                                    for (b=c; b<180; b+=c) {
                                        oz = p3 * tcos[b] * p4;
                                        z2 = p3 * tsin[b];
                                        ox = z2 * tcos[a];
                                        oy = z2 * tsin[a];
                                        if (C32(ox+crx, oz+cry, crz-oy)) {
                                            aux_plot();
                                        }
                                    }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
                            case TESTO:
                                sprintf (t, &pixel_elem_b[40*jjj], nopix);
                                Txt (t, p0, p1, p2, p3, p4, p5, p6);
                                break;
                            case ELLISSE:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                if (p5==0) {
                                    _x = p0+p3;
                                    _y = p1;
                                    for (c=p6; c<360; c+=p6) {
                                        crx = p0+tcos[c]*p3;
                                        cry = p1+tsin[c]*p4;
                                        rel (_x, _y, p2, crx, cry, p2);
                                        _x = crx;
                                        _y = cry;
                                    }
                                    rel (_x, _y, p2, p0+p3, p1, p2);
                                    break;
                                }
                                if (p5==1) {
                                    _x = p0+p3;
                                    _z = p2;
                                    for (c=p6; c<360; c+=p6) {
                                        crx = p0+tcos[c]*p3;
                                        crz = p2+tsin[c]*p4;
                                        rel (_x, p1, _z, crx, p1, crz);
                                        _x = crx;
                                        _z = crz;
                                    }
                                    rel (_x, p1, _z, p0+p3, p1, p2);
                                    break;
                                }
                                if (p5==2) {
                                    _y = p1+p3;
                                    _z = p2;
                                    for (c=p6; c<360; c+=p6) {
                                        cry = p1+tcos[c]*p3;
                                        crz = p2+tsin[c]*p4;
                                        rel (p0, _y, _z, p0, cry, crz);
                                        _y = cry;
                                        _z = crz;
                                    }
                                    rel (p0, _y, _z, p0, p1+p3, p2);
                                    break;
                                }
                                break;
                            case CIAMBELLA:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                _ox = ox;
                                _oy = oy;
                                _oz = oz;
                                ox += p0;
                                oy += p1;
                                oz += p2;
                                switch (int(p5)) {
                                    case 0:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (crx, crz, _y, first_x, first_z, first_y);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (crx, crz, _y, _x*tcos[(int)(a-p6)], - _x*tsin[(int)(a-p6)], _y);
                                            }
                                            rel (crx, crz, _y, cry, (p3-p4) * tsin[a], 0);
                                        }
                                        break;
                                    case 1:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (crx, _y, crz, first_x, first_y, first_z);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (crx, _y, crz, _x*tcos[(int)(a-p6)], _y, - _x*tsin[static_cast<int>(a-p6)]);
                                            }
                                            rel (crx, _y, crz, cry, 0, (p3-p4) * tsin[a]);
                                        }
                                        break;
                                    case 2:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (_y, crz, crx, first_y, first_z, first_x);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (_y, crz, crx, _y, - _x*tsin[(int)(a-p6)], _x*tcos[a-static_cast<int>(p6)]);
                                            }
                                            rel (_y, crz, crx, 0, (p3-p4) * tsin[a], cry);
                                        }
                                        break;
                                }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
                            case SFERA_RETICOLARE:
                                if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                c = p5 * (id+1);
                                if (c>90) c = 90;
                                p3 = -p3;
                                _ox = ox; ox += p0;
                                _oy = oy; oy += p1;
                                _oz = oz; oz += p2;
                                for (a=0; a<360; a+=c) {
                                    first_z = p3 * tcos[c] * p4;
                                    z2 = p3 * tsin[c];
                                    first_x = z2 * tcos[a];
                                    first_y = z2 * tsin[a];
                                    for (b=2*c; b<180; b+=c) {
                                        crz = p3 * tcos[b] * p4;
                                        k1 = p3 * tsin[b];
                                        crx = k1 * tcos[a];
                                        cry = k1 * tsin[a];
                                        rel (crx, crz, cry, first_x, first_z, first_y);
                                        rel (k1 * tcos[a+c], crz, k1 * tsin[a+c], first_x, first_z, first_y);
                                        first_x = crx;
                                        first_y = cry;
                                        first_z = crz;
                                    }
                                }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
            }
            nr_elem++;
        }
        uneg = bkuneg;
        explode = 0;
    }
}

void Object (int tipo)
{
    FILE* th;
    static const int tickrate = get_config().ticks_per_frame;

    switch (tipo) {
        // Fottifoh
        case 0:
            a = 0;
            while (fotty[a]!=10) {
                rel (fotty[a], fotty[a+1], fotty[a+2],
                        fotty[a+3], fotty[a+4], fotty[a+5]);
                a += 6;
            }
            rectrel (-2, 0, 4, 0.5, 0.5, 1);
            rectrel (2, 0, 4, 0.5, 0.5, 1);
            break;
        // Orbital engine
        case 1:
            for (   a = static_cast<int>((SDL_GetTicks()/tickrate)%45);
                    a <= 135+(int)((SDL_GetTicks()/tickrate)%45);
                    a += 45) {
                _ox = 25 * tcos[a]; _oy = 25 * tsin[a];
                rel (-_ox, 0, _oy, _ox, 0, -_oy);
            }
            break;
        // Record player
        case 2:
            b = 0;
            if (/* sbf_stat&& */pix==pixel_sonante)
                b = ((SDL_GetTicks()/tickrate)%3)*15;
            for (a=b; a<=135+b; a+=45) {
                _ox = 5 * tcos[a]; _oy = 5 * tsin[a];
                rel (-_ox, 0, _oy, _ox, 0, -_oy);
            }
            rectrel (0, 0, 0, 6, 6, 1);
            if (recfile) Txt ("> RECORD >", -5.5, 0, -5, 0.1, 0.15, 270, 0);
            if (globalvocfile[0]!='.') Txt ("> PLAY >", -5.5, 0, -4, 0.1, 0.15, 270, 0);
            sprintf (t, "INPUT: %s", source_name[static_cast<int>(source)]);
            Txt (t, -5.5, 0, 5, 0.1, 0.15, 270, 0);
            sprintf (t, "ACCURATEZZA: %s", record_qlty[static_cast<int>(quality)]);
            Txt (t, -5.5, 0, 4, 0.1, 0.15, 270, 0);
            if (repeat) Txt ("REPEAT", -5.5, 0, 3, 0.1, 0.15, 270, 0);
            break;
        
        // other objects
        // TODO: is this code reading from a file on each tick? Aren't objects loaded into memory on startup?
        default:
            Pixel (tipo+FRONTIER_M3);
            if (id) break;
            if (!type_mode) break; // ctrlkeys[0]&16 is scroll lock, use something else
            if (!memcmp(&subsignal[9*(tipo+FRONTIER_M3)], "TEXT-" /*"TESTO-"*/, 5/*6*/)) {
                a = subsignal[9*(tipo+FRONTIER_M3)+5/*6*/];
                memset (buffer, ' ', 512);
                // safeguard buffer from reading beyond the end of the text
                buffer[512] = '\0';
                sprintf (autore_forme, "TEXT-%03d.FIX", tipo-3);
                th = std::fopen (autore_forme, "r+b");
                if (th) {
                    buffer[32] = '\0';
                    k1 = 5.32;
                    int bytes_read = std::fread(&buffer[33], 1, 512, th);
                    for (c=0; c<bytes_read; c+=32) {
                        memcpy (buffer, buffer+33+c, 32);
                        if (a=='V') {
                            Txt (reinterpret_cast<char*>(buffer), -6.82, -k1, 0, 0.11, 0.14, 0, 0);
                        } else if (a=='H') {
                            Txt (reinterpret_cast<char*>(buffer), -6.82, 0, k1, 0.11, 0.14, 270, 0);
                        }
                        k1 -= 0.7;
                    }
                    if (d<15||tipo==carry_type) {
                        fermo_li = 1;
                        memcpy (buffer+545, buffer+33, 512);
                        /*
                        if (tasto_premuto()) {
                            c = attendi_pressione_tasto();
                            if ((globalvocfile[0]=='.'&&EVA_in_progress)||!EVA_in_progress) play (TARGET);
                            switch (c) {
                                case 0:
                                    switch (attendi_pressione_tasto()) {
                                        case 77:
                                            if (cursore<511) cursore++;
                                            break;
                                        case 75:
                                            if (cursore) cursore--;
                                            break;
                                        case 80:
                                            if (cursore<480) cursore += 32;
                                            break;
                                        case 72:
                                            if (cursore>31) cursore -= 32;
                                            break;
                                        case 64:
                                            snapshot();
                                }
                                break;
                        case 13:
                                if (cursore<480) {
                                    cursore = (cursore/32) * 32;
                                    cursore += 32;
                                }
                                break;
                        case 8:
                                if (cursore) {
                                        cursore--;
                                        buffer[cursore+33] = 32;
                                }
                                break;
                        case 27:
                                alfin (true);
                                exit (0);
                        default:
                            if (cursore<512) {
                                if (c>='a'&&c<='z') c -= 32;
                                if (c>31 && c<97) {
                                    buffer[cursore+33] = c;
                                    if (cursore<511) cursore++;
                                }
                            }
                            */
                    }
                    if (cursore<512) {
                        if (type_this != 0) {
                            assert (type_this >= 32 && type_this <= 96);
                            buffer[cursore+33] = type_this;
                            if (cursore<511) cursore++;
                        }
                        type_this = 0;
                    }
                    //}
                    if (a=='V') Txt ("*", -6.82+(cursore%32)*0.44, (cursore/32)*0.7-5.32, 0, 0.5, 0.5, 0, 0);
                    else if (a=='H') Txt ("*", -6.82+(cursore%32)*0.44, 0, 5.32-(cursore/32)*0.7, 0.5, 0.5, 270, 0);
                    if (memcmp (buffer+33, buffer+545, 512)) {
                        std::fseek (th, 0, SEEK_SET);
                        std::fwrite (buffer+33, 1, 512, th);
                    }
                //}
                    std::fclose (th);
                }
                else {
                    th = std::fopen(autore_forme, "wb");
                    if (th) {
                        std::fwrite (buffer, 1, 512, th);
                        std::fclose (th);
                    }
                }
        }
    }
}