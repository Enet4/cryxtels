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

#include "draw.h"
#include "conf.h"
#include "global.h"
#include "fast3d.h"
#include "text3d.h"

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
    static const int width = 5*HEIGHT/200;

    static const int xlow = 5*WIDTH/320;
    static const int xhigh = 315*WIDTH/320;
    static const int ylow = xlow;
    static const int yhigh = 195*HEIGHT/200; 
    if ( x>xlow && x<xhigh && y>ylow && y<yhigh) {
        Segmento (x - width, y - width, x + width, y + width);
        Segmento (x + width, y - width, x - width, y + width);
    }
}

void draw_indicator_retrograde(int x, int y)
{
    static const int outside = 9*HEIGHT/200;
    static const int inside  = 5*HEIGHT/200;

    static const int xlow = 10*WIDTH/320;
    static const int xhigh = 310*WIDTH/320;
    static const int ylow = xlow;
    static const int yhigh = 190*HEIGHT/200; 
    if ( x>xlow && x<xhigh && y>ylow && y<yhigh) {
        Segmento (x - outside, y - inside, x + outside, y - inside);
        Segmento (x - outside, y + inside, x + outside, y + inside);
        Segmento (x - inside, y - outside, x - inside, y + outside);
        Segmento (x + inside, y - outside, x + inside, y + outside);
    }
}

void draw_indicator_closest(int x, int y)
{
    static const int width = 19*HEIGHT/200;

    static const int xlow = 20*WIDTH/320;
    static const int xhigh = 300*WIDTH/320;
    static const int ylow = xlow;
    static const int yhigh = 180*HEIGHT/200; 
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

void draw_readings_speed(double speed)
{
    char str[20];
    sprintf (str, "V=%04.0f:K/H", speed*1.9656);
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